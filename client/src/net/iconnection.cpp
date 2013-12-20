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
#include "iconnection.h"

#include "boost/bind.hpp"
#include "connectionsmanager.h"
#include "iserver.h"
#include "lock.h"
#include "log.h"
#include "platformmanager.h"
#include "tcpsocket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IConnection::IConnection(shared_ptr<ConnectionsManager> connectionsManager) : m_connectionsManager(connectionsManager),
																			  m_id(connectionsManager->peekConnectionID())
{

}

IConnection::~IConnection()
{

}

shared_ptr<boost::asio::io_service> IConnection::getService() const
{
	shared_ptr<ConnectionsManager> connectionsManager = getConnectionsManager();
	if(connectionsManager == nullptr)
		return nullptr;

	return connectionsManager->getService();
}

void IConnection::addListener(const ConnectionEvent::slot_type &slot)
{
	OS_LOCK(m_eventsCS);
	m_stateChangedEvent.connect(slot);
}

void IConnection::close(bool graceful)
{
	boost::system::error_code e;

	if(graceful)
		getSocket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, e);
	
	getSocket().close(e);	
}

void IConnection::notifyCompleted()
{
	shared_ptr<ConnectionsManager> connectionsManager = getConnectionsManager();
	if(connectionsManager != nullptr)
		connectionsManager->removeConnection(get_this_ptr(), true);	
}

void IConnection::fireStateChanged()
{
	OS_LOCK(m_eventsCS);
	m_stateChangedEvent(get_this_ptr());
}

shared_ptr<ConnectionScope> IConnection::createScope(const boost::posix_time::time_duration &timeout)
{
	shared_ptr<ConnectionScope> scope(OS_NEW ConnectionScope(getService(), get_this_ptr()));
	scope->extendTimeout(timeout);
	return scope;	
}

void IConnection::onTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{
	OS_ASSERT(isAsioOperationAborted(e) == false);

	// Di default chiude la connessione in seguito ad un timeout
	close();
}

void IConnection::onFinalize()
{
	notifyCompleted();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
