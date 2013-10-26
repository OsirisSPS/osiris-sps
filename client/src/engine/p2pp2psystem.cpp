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
#include "p2pp2psystem.h"

#include "boost/bind.hpp"
#include "cryptmanager.h"
#include "buffer.h"
#include "log.h"
#include "networksystem.h"
#include "options.h"
#include "p2pkeyscache.h"
#include "p2plocalmachine.h"
#include "p2pmachine.h"
#include "p2pmachinescache.h"
#include "p2pserver.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SYSTEM(OS_NAMESPACE_NAME::p2p::P2PSystem)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

P2PSystem::P2PSystem() : m_server(OS_NEW P2PServer()),
						 m_machines(OS_NEW MachinesCache()),
						 m_keys(OS_NEW KeysCache(OS_P2P_KEYS_CACHE_SIZE, OS_P2P_KEY_AGREEMENT_SIZE, OS_P2P_KEYS_CACHE_MAX_KEY_USAGE))
{
	Options::instance()->subscribeUpdateHandler(boost::bind(&P2PSystem::updateOptions, this));	
}

P2PSystem::~P2PSystem()
{
	
}

Buffer P2PSystem::generateMachineID(const Buffer &publicKey) const
{
	return CryptManager::instance()->SHA(publicKey.getData(), publicKey.getSize());
}

bool P2PSystem::validateMachineID(const Buffer &id, const Buffer &publicKey) const
{
	if(id.empty())
		return false;

	return generateMachineID(publicKey) == id;
}

shared_ptr<P2pMachine> P2PSystem::addMachine(const Buffer &id, const Buffer &publicKey)
{
	return m_machines->addMachine(id, publicKey);
}

bool P2PSystem::isKnownMachine(const Buffer &id) const
{
	return getMachine(id) != null;
}

shared_ptr<P2pMachine> P2PSystem::getMachine(const Buffer &id) const
{
	return m_machines->get(id.toHex(), 0);
}

shared_ptr<CryptKeyAgreement> P2PSystem::peekKey()
{
	return m_keys->peekKey();
}

void P2PSystem::updateServer()
{	
	try
	{
		OS_ASSERT(m_server != null);

		// Controlla se l'allineamento è disabilitato
		if(Options::instance()->getOption<bool>(Options::p2p_options::enable) == false)
		{
			stopServer();
			return;
		}

		bool needRestart = false;

		String bindIP = Options::instance()->getOption<String>(Options::p2p_options::server_bind_ip);
		uint32 bindPort = Options::instance()->getOption<uint32>(Options::p2p_options::server_port);
		uint32 hardwareConcurrency = Options::instance()->getHardwareConcurrency();
		uint32 connectionsTimeout = Options::instance()->getOption<uint32>(Options::net_options::connection_timeout);		
		uint32 maxIncomingConnections = Options::instance()->getOption<uint32>(Options::p2p_options::max_incoming_connections);
		uint32 maxOutgoingConnections = Options::instance()->getOption<uint32>(Options::p2p_options::max_outgoing_connections);		
		bool listen = maxIncomingConnections > 0;
		bool enableUPnP = listen && Options::instance()->getOption<bool>(Options::p2p_options::server_enable_upnp);

		if(m_server->running())
		{
			if(m_server->getListening() != listen)
				needRestart = true;

			boost::asio::ip::address currentBindAddress;
			if(m_server->getBindAddress(currentBindAddress))
			{
				if(NetworkSystem::instance()->compareAddresses(bindIP.to_ascii(), currentBindAddress) == false)
					needRestart = true;
			}

			uint32 currentBindPort = 0;
			if(m_server->getBindPort(currentBindPort))
			{
				if(currentBindPort != bindPort)
					needRestart = true;
			}

			if(m_server->getUPnPEnabled() != enableUPnP)
				needRestart = true;

			if(m_server->getWorkerThreadsCount() != hardwareConcurrency)
				needRestart = true;
		}
		else
		{
			needRestart = true;			
		}
		
		if(needRestart)
		{
			stopServer();
			
			m_server->setWorkerThreadsCount(hardwareConcurrency);
			m_server->setUPnPEnabled(enableUPnP);

			if(listen)
			{
				if(m_server->bind(bindIP, bindPort))
				{
					uint32 listeningPort = 0;
					if(m_server->getBindPort(listeningPort))
						OS_LOG_INFO(String::format(_S("P2P server listening on port '%d'").c_str(), listeningPort));							
				}
				else
				{
					if(bindIP.empty())
						OS_LOG_CRITICAL(String::format(_S("P2P server error: cannot bind port '%d'").c_str(), bindPort));
					else
						OS_LOG_CRITICAL(String::format(_S("P2P server error: cannot bind '%S:%d'").c_str(), bindIP.c_str(), bindPort));

					return;
				}
			}

			// Se listen == false avvia comunque il server per gestire le connessioni in uscita
			m_server->start();
		}
		
		// E' necessario ricaricare la configurazione ogni volta perchè potrebbe verificarsi la seguente circostanza: l'allineamento viene disabilitato, successivamente si modificano i parametri sulle connessione ed infine si riattiva l'allineamento
		m_server->setConnectionsTimeout(connectionsTimeout);
		m_server->setMaxIncomingConnections(maxIncomingConnections);
		m_server->setMaxOutgoingConnections(maxOutgoingConnections);

		if(bindPort == OS_P2P_NEW_DEFAULT_SERVER_PORT)
		{
			if(m_server->getBindPort(bindPort))
				// Se si tratta del primo avvio salva la porta di ascolto
				Options::instance()->setOption(Options::p2p_options::server_port, bindPort);
		}
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
}

void P2PSystem::stopServer()
{
	OS_ASSERT(m_server != null);
	m_server->stop();
}

void P2PSystem::updateOptions()
{
	updateServer();	
}

String P2PSystem::getName() const
{
	return _S("p2p");
}

bool P2PSystem::start(bool recovery)
{
	if(LocalMachine::instance()->init() == false)
		return false;

	updateServer();

	return true;
}

void P2PSystem::stop()
{
	stopServer();
}

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
