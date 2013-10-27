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

#ifndef _OS_NET_ICONNECTION_H
#define _OS_NET_ICONNECTION_H

#include "base/object.h"
#include "boostasio_fwd.h"
#include "boost/noncopyable.hpp"
#include "boost/signals.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "connectionscope.h"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ConnectionsManager;
class TCPSocket;

//////////////////////////////////////////////////////////////////////

class NetExport IConnection : public enable_this_ptr<IConnection>,
							  public ConnectionScope::IListener,
							  public Object,
							  public boost::noncopyable
{
	typedef boost::signal<void (shared_ptr<IConnection>)> ConnectionEvent;

// Construction
public:
	IConnection(shared_ptr<ConnectionsManager> connectionsManager);
	virtual ~IConnection();

// Attributes
public:
	inline shared_ptr<ConnectionsManager> getConnectionsManager() const;
	inline uint32 getID() const;

	shared_ptr<boost::asio::io_service> getService() const;

// Operations
public:
	void addListener(const ConnectionEvent::slot_type &slot);
	void close(bool graceful = true);
	void notifyCompleted();
	void fireStateChanged();

// Overridables
public:
	virtual shared_ptr<ConnectionScope> createScope(const boost::posix_time::time_duration &timeout = boost::posix_time::milliseconds(0));

// ConnectionScope::IListener interface
protected:
	virtual void onTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);
	virtual void onFinalize();

// Interface
public:
	virtual TCPSocket & getSocket() = 0;
	virtual void run() = 0;

protected:
	weak_ptr<ConnectionsManager> m_connectionsManager;

private:
	uint32 m_id;
	mutable boost::recursive_mutex m_eventsCS;
	ConnectionEvent m_stateChangedEvent;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ConnectionsManager> IConnection::getConnectionsManager() const { return m_connectionsManager.lock(); }
inline uint32 IConnection::getID() const { return m_id; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_ICONNECTION_H
