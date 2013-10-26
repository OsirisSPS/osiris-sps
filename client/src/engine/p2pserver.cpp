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
#include "p2pserver.h"

#include "connectionsmanager.h"
#include "limitedconnection.h"
#include "locked.h"
#include "log.h"
#include "options.h"
#include "p2pclient.h"
#include "p2psession.h"
#include "portalspeer.h"
#include "portalspeersmanager.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

P2PServer::P2PServer(boost::optional<uint32> workerThreadsCount) : ServerBase(workerThreadsCount),
																   m_maxIncomingConnections(0),
																   m_maxOutgoingConnections(0),
																   m_outgoingConnections(0),
																   m_connectionsTimeout(OS_DEFAULT_CONNECTION_TIMEOUT),
																   m_portal(0)
{
	setPriority(taskPriorityLow);
}

P2PServer::~P2PServer()
{
	
}

uint32 P2PServer::getMaxIncomingConnections() const
{
	OS_LOCK(m_p2pServerCS);
	return m_maxIncomingConnections;
}

void P2PServer::setMaxIncomingConnections(uint32 maxIncomingConnections)
{
	OS_LOCK(m_p2pServerCS);
	m_maxIncomingConnections = maxIncomingConnections;
}

uint32 P2PServer::getMaxOutgoingConnections() const
{
	OS_LOCK(m_p2pServerCS);
	return m_maxOutgoingConnections;
}

void P2PServer::setMaxOutgoingConnections(uint32 maxOutgoingConnections)
{
	OS_LOCK(m_p2pServerCS);
	m_maxOutgoingConnections = maxOutgoingConnections;
}

uint32 P2PServer::getConnectionsTimeout() const
{
	OS_LOCK(m_p2pServerCS);
	return m_connectionsTimeout;
}

void P2PServer::setConnectionsTimeout(uint32 connectionsTimeout)
{
	OS_LOCK(m_p2pServerCS);
	m_connectionsTimeout = connectionsTimeout;
}

uint32 P2PServer::getPendingsOutgoingConnections() const
{
	OS_LOCK(m_p2pServerCS);

	if(m_outgoingConnections > m_maxOutgoingConnections)
		return 0;	// URGENT: capita, da verificare

	return m_maxOutgoingConnections - m_outgoingConnections;
}

bool P2PServer::getAllowConnection(shared_ptr<IConnection> connection) const
{
	// N.B.: essendo ogni connessione sempre accettata non è possibile determinare se si accetta una connessione
	// in entrata tenendo semplicemente un contatore (con un numero elevato di connessioni si rischia di rifiutare sempre la connessione)
	// pertanto viene tenuta traccia delle connessioni accettate

	OS_LOCK(m_p2pServerCS);

	OS_ASSERT(connection != null);
	
	if(isOutgoingConnection(connection))
		return true;

	ConnectionsSet::const_iterator i = m_allowedConnections.find(connection);
	if(i != m_allowedConnections.end())
		return true;

	if(m_allowedConnections.size() >= m_maxIncomingConnections)
		return false;

	m_allowedConnections.insert(connection);
	return true;
}

shared_ptr<Portal> P2PServer::peekNextPortal(uint32 portals)
{
	OS_LOCK(m_p2pServerCS);

	if(m_portal >= portals)
		m_portal = 0;

	shared_ptr<Portal> portal = PortalsSystem::instance()->getPortalByIndex(m_portal);
	m_portal++;

	return portal;
}

bool P2PServer::isOutgoingConnection(shared_ptr<IConnection> connection) const
{
	return boost::dynamic_pointer_cast<P2PClient>(connection) != null;
}

shared_ptr<P2PConnection> P2PServer::detectLoopbackConnection(const Buffer &public_key) const
{
	Locked<const ConnectionsManager::Connections>::unique connections = getConnectionsManager()->getConnections();
	for(ConnectionsManager::Connections::const_iterator i = connections->begin(); i != connections->end(); ++i)
	{
		shared_ptr<P2PConnection> connection = boost::dynamic_pointer_cast<P2PConnection>(i->second);
		if(connection != null && connection->compareSessionKey(public_key))
			return connection;		
	}

	return null;
}

void P2PServer::_startClients()
{
	try
	{	
		uint32 skippedPortals = 0;
		for(uint32 i = getPendingsOutgoingConnections(); i > 0;)
		{
			uint32 portals = PortalsSystem::instance()->getPortalsCount();
			if(portals == 0 || skippedPortals >= portals)
				return;

			shared_ptr<Portal> portal = peekNextPortal(portals);

			bool added = false;
			if(portal != null && portal->getOptions()->getExchangeEnabled())
			{
				shared_ptr<PortalsPeer> peer = portal->getPeersManager()->peekPeer();
				if(peer != null)
				{	
					OS_ASSERT(IPAddress().setEndpoint(peer->getIP(), peer->getPort()));

					shared_ptr<IPAddress> address(OS_NEW IPAddress());
					if(address->setEndpoint(peer->getIP(), peer->getPort()))
					{
						shared_ptr<P2PClient> client(OS_NEW P2PClient(address, get_this_ptr<P2PServer>(), getConnectionsTimeout(), peer->getOrigin()));
						if(client->setPortal(portal))
							added = getConnectionsManager()->addConnection(client, true);						
					}					
				}
			}					

			if(added)
			{
				i--;
				skippedPortals = 0;
			}
			else
			{
				skippedPortals++;
			}
		}
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
}

shared_ptr<IConnection> P2PServer::createConnection()
{
	return shared_ptr<IConnection>(OS_NEW P2PSession(get_this_ptr<P2PServer>(), getConnectionsTimeout(), OS_PEER_ORIGIN_INCOMING));
}

bool P2PServer::onIdle()
{
	_startClients();

	return ServerBase::onIdle();
}

void P2PServer::onConnectionAdded(shared_ptr<IConnection> connection)
{
	ServerBase::onConnectionAdded(connection);
	
	OS_LOCK(m_p2pServerCS);

	if(isOutgoingConnection(connection))
		m_outgoingConnections++;
}

void P2PServer::onConnectionRemoved(shared_ptr<IConnection> connection)
{
	ServerBase::onConnectionRemoved(connection);

	OS_LOCK(m_p2pServerCS);

	if(isOutgoingConnection(connection))
	{
		OS_ASSERT(m_outgoingConnections > 0);
		m_outgoingConnections--;
	}
	else
	{
		m_allowedConnections.erase(connection);
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
