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

#ifndef _OS_NET_CONNECTIONSMANAGER_H
#define _OS_NET_CONNECTIONSMANAGER_H

#include "base/object.h"
#include "boostasio.h"
#include "boost/signals.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "locked.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IConnection;

//////////////////////////////////////////////////////////////////////

class NetExport ConnectionsManager : public Object
{
public:
	enum ConnectionEvent
	{
		connectionAdded		= 0,
		connectionRemoved	= 1,
	};

	typedef boost::signal<void (shared_ptr<IConnection>, ConnectionEvent)> ConnectionSignal;
	typedef unordered_map<uint32, shared_ptr<IConnection> >::type Connections;

// Construction
public:
	ConnectionsManager(shared_ptr<boost::asio::io_service> service, bool enabled = true);
	virtual ~ConnectionsManager();

// Attributes
public:
	inline shared_ptr<boost::asio::io_service> getService() const;
	
	shared_ptr<IConnection> getConnection(uint32 id) const;
	
	Locked<const Connections>::unique getConnections() const;
	bool hasConnections() const;	

	bool getEnabled() const;

// Operations
public:
	boost::signals::connection addListener(const ConnectionSignal::slot_type &slot);

	bool addConnection(shared_ptr<IConnection> connection, bool run);
	bool removeConnection(shared_ptr<IConnection> connection, bool close);
	
	uint32 peekConnectionID();

	void enable();
	void disable();

private:
	bool _add(shared_ptr<IConnection> connection);
	bool _remove(shared_ptr<IConnection> connection, bool close);
	void _fireEvent(shared_ptr<IConnection> connection, ConnectionEvent e);
	void _removeAllConnections();

protected:
	mutable boost::recursive_mutex m_dataCS;
	mutable boost::recursive_mutex m_connectionsCS;
	mutable boost::recursive_mutex m_eventsCS;
	Connections m_connections;
	ConnectionSignal m_connectionEvent;	
	shared_ptr<boost::asio::io_service> m_service;	
	uint32 m_currentID;
	bool m_enabled;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<boost::asio::io_service> ConnectionsManager::getService() const { return m_service; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_CONNECTIONSMANAGER_H
