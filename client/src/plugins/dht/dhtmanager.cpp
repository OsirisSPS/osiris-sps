// <osiris_sps_source_header>
// This file is part of Osiris Serverless Portal System.
// Copyright (C)2005-2012 Osiris Team (info@osiris-sps.org) / http://www.osiris-sps.org )
//
// Osiris Serverless Portal System is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Osiris Serverless Portal System is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Osiris Serverless Portal System.  If not, see <http://www.gnu.org/licenses/>.
// </osiris_sps_source_header>

#include "stdafx.h"
#include "dhtmanager.h"

#include "boost/bind.hpp"
#include "dht.h"
#include "dhtthread.h"
#include "options.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DHTManager::DHTManager(const String &rootPath) : m_rootPath(rootPath),
												 m_socket(-1),
												 m_port(0)
{
	Options::instance()->ensureOption<uint32>(OS_DHT_OPTION_PORT, 0, true);
	//Options::instance()->ensureOption<String>(OS_DHT_OPTION_BOOTSTRAP_ROUTERS, "dht.transmissionbt.com:6881;router.bittorrent.com:6881;router.utorrent.com:6881");
	Options::instance()->ensureOption<String>(OS_DHT_OPTION_BOOTSTRAP_ROUTERS, "dht.osiris-sps.org:6881;dht2.osiris-sps.org:6881", true);
	Options::instance()->ensureOption<uint32>(OS_DHT_OPTION_NODES_CACHE_SIZE, 100, true);

	Options::instance()->subscribeUpdateHandler(boost::bind(&DHTManager::updateDHT, this));
}

DHTManager::~DHTManager()
{
	stopDHT();
}

bool DHTManager::startDHT()
{
	OS_ASSERT(m_socket == -1);

	m_socket = socket(PF_INET, SOCK_DGRAM, 0);
	if(m_socket < 0)
		return false;

	m_port = Options::instance()->getOption<uint32>(OS_DHT_OPTION_PORT);

	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = htons(m_port);

    if(bind(m_socket, reinterpret_cast<sockaddr *>(&m_sockaddr), sizeof(m_sockaddr)) < 0)
	{
#if OS_PLATFORM == OS_PLATFORM_WIN
		closesocket(m_socket);
#else
        close(m_socket);
#endif
		m_socket = -1;

		return false;
	}

#ifdef OS_DEBUG
	dht_debug = stdout;
#endif

	dht_random_bytes(m_id, OS_DHT_HASH_SIZE);

	if(dht_init(static_cast<int>(m_socket), -1, m_id, reinterpret_cast<const unsigned char *>("OS\0\0")) < 0)
	{
#if OS_PLATFORM == OS_PLATFORM_WIN
		closesocket(m_socket);
#else
        close(m_socket);
#endif
		m_socket = -1;

		OS_ASSERTFALSE();
		return false;
    }

	m_dhtThread.reset(OS_NEW DHTThread(m_rootPath, m_socket));
	m_dhtThread->start();

	return true;
}

void DHTManager::stopDHT()
{
	if(m_dhtThread != nullptr)
	{
		m_dhtThread->stop();
		m_dhtThread.reset();
	}

	if(m_socket != -1)
	{
		dht_uninit(1);

#if OS_PLATFORM == OS_PLATFORM_WIN
		closesocket(m_socket);
#else
        close(m_socket);
#endif
		m_socket = -1;
	}
}

bool DHTManager::updateDHT()
{
	// TODO: attualmente il DHT va di pari passo con il P2P, andrebbe creata un'opzione adhoc
	if(Options::instance()->getOption<bool>(Options::p2p_options::enable) == false)
	{
		stopDHT();
		return true;
	}

	bool needRestart = false;

	if(m_dhtThread != nullptr && m_dhtThread->running())
	{
		if(Options::instance()->getOption<uint32>(OS_DHT_OPTION_PORT) != m_port)
			needRestart = true;
	}
	else
	{
		needRestart = true;
	}

	if(needRestart == false)
		return true;

	stopDHT();
	return startDHT();
}

bool DHTManager::start()
{
	return updateDHT();
}

void DHTManager::stop()
{
	stopDHT();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
