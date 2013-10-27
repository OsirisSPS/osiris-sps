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

#ifndef _OS_DHT_DHTMANAGER_H
#define _OS_DHT_DHTMANAGER_H

#include "base/object.h"
#include "boostasio.h"
#include "boost/signals/trackable.hpp"
#include "irunnable.h"

//////////////////////////////////////////////////////////////////////

#define OS_DHT_HASH_SIZE		20

#define OS_DHT_OPTION_PORT		"plugins.dht.port"
#define OS_DHT_OPTION_BOOTSTRAP_ROUTERS		"plugins.dht.bootstrap_routers"
#define OS_DHT_OPTION_NODES_CACHE_SIZE		"plugins.dht.nodes_cache_size"


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DHTThread;

//////////////////////////////////////////////////////////////////////

class DHTManager : public Object,
				   public IRunnable,
				   public boost::signals::trackable
{
// Construction
public:
	DHTManager(const String &rootPath);
	virtual ~DHTManager();

// Operations
private:
	bool startDHT();
	void stopDHT();
	bool updateDHT();

// IRunnable interface
public:
	virtual bool start();
	virtual void stop();

private:
	String m_rootPath;
	boost::asio::ip::udp::socket::native_type m_socket;
	sockaddr_in m_sockaddr;
	uint32 m_port;
	unsigned char m_id[OS_DHT_HASH_SIZE];
	shared_ptr<DHTThread> m_dhtThread;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_DHT_DHTMANAGER_H
