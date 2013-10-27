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
#include "connectionsmanager.h"

#include "boost/bind.hpp"
#include "iconnection.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ConnectionsManager::ConnectionsManager(shared_ptr<boost::asio::io_service> service, bool enabled) : m_service(service),
																									m_enabled(enabled),
																									m_currentID(0)
{
	OS_ASSERT(service != null);
}

ConnectionsManager::~ConnectionsManager()
{

}

shared_ptr<IConnection> ConnectionsManager::getConnection(uint32 id) const
{
	OS_LOCK(m_connectionsCS);

	Connections::const_iterator i = m_connections.find(id);
	if(i == m_connections.end())
		return null;

	return i->second;
}

Locked<const ConnectionsManager::Connections>::unique ConnectionsManager::getConnections() const
{
	return Locked<const Connections>::unique(m_connections, m_connectionsCS); 
}

bool ConnectionsManager::hasConnections() const
{
	OS_LOCK(m_connectionsCS);
	return m_connections.empty() == false;
}

bool ConnectionsManager::getEnabled() const
{
	OS_LOCK(m_connectionsCS);
	return m_enabled;
}

boost::signals::connection ConnectionsManager::addListener(const ConnectionSignal::slot_type &slot)
{
	OS_LOCK(m_eventsCS);
	return m_connectionEvent.connect(slot);
}

bool ConnectionsManager::addConnection(shared_ptr<IConnection> connection, bool run)
{
	if(_add(connection) == false)
		return false;

	_fireEvent(connection, connectionAdded);

	if(run)
	{
		// Schedula l'operazione di gestione della connessione in modo da restituire subito il controllo
		getService()->post(boost::bind(&IConnection::run, connection));
	}	

	return true;
}

bool ConnectionsManager::removeConnection(shared_ptr<IConnection> connection, bool close)
{
	if(_remove(connection, close) == false)
		return false;

	_fireEvent(connection, connectionRemoved);

	return true;
}

uint32 ConnectionsManager::peekConnectionID()
{
	OS_LOCK(m_dataCS);
	return m_currentID++;
}

void ConnectionsManager::enable()
{
	OS_LOCK(m_connectionsCS);
	m_enabled = true;	
}

void ConnectionsManager::disable()
{
	OS_LOCK(m_connectionsCS);
	m_enabled = false;	
	
	_removeAllConnections();
}

bool ConnectionsManager::_add(shared_ptr<IConnection> connection)
{
	OS_LOCK(m_connectionsCS);

	if(m_enabled == false)
		return false;

	if(connection == null || m_connections.find(connection->getID()) != m_connections.end())
	{
		OS_ASSERTFALSE();
		return false;
	}

	m_connections[connection->getID()] = connection;

	return true;
}

bool ConnectionsManager::_remove(shared_ptr<IConnection> connection, bool close)
{
	if(connection == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	OS_LOCK(m_connectionsCS);	

	Connections::iterator i = m_connections.find(connection->getID());
	if(i == m_connections.end())
		return false;

	if(close)
		connection->close();

	m_connections.erase(i);
	return true;
}

void ConnectionsManager::_fireEvent(shared_ptr<IConnection> connection, ConnectionEvent e)
{
	OS_LOCK(m_eventsCS);
	m_connectionEvent(connection, e);
}

void ConnectionsManager::_removeAllConnections()
{
	// N.B.: questa funzione deve essere loccata a monte

	while(m_connections.empty() == false)
	{
		removeConnection(m_connections.begin()->second, true);
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
