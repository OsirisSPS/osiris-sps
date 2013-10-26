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

#ifndef _OS_NET_NETWORKSERVICE_H
#define _OS_NET_NETWORKSERVICE_H

#include "boostasio.h"
#include "connectionsmanager.h"
#include "enable_this_ptr.h"
#include "pimpl_ptr.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class NetExport NetworkService : public Thread,
								 public enable_this_ptr<NetworkService>
{
	typedef Thread ThreadBase;

// Construction
public:
	NetworkService(boost::optional<uint32> workerThreadsCount = boost::none);
	virtual ~NetworkService();

// Attributes
public:
	shared_ptr<boost::asio::io_service> getService() const;
	shared_ptr<ConnectionsManager> getConnectionsManager() const;

	uint32 getWorkerThreadsCount() const;
	void setWorkerThreadsCount(uint32 workerThreadsCount);

// Operations
private:
	void handleIdle();

	void startTimer();
	void runService();
	void stopService();

	void connectionsCallback(shared_ptr<IConnection> connection, ConnectionsManager::ConnectionEvent e);

// Overridables
protected:
	virtual bool onIdle();

	virtual void onConnectionAdded(shared_ptr<IConnection> connection);
	virtual void onConnectionRemoved(shared_ptr<IConnection> connection);

// Thread overrides
public:
	virtual void stop();

// Thread interface
protected:
	virtual void run();	

private:
	pimpl_of<NetworkService>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_NETWORKSERVICE_H
