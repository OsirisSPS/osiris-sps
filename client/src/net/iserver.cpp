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
#include "iserver.h"

#include "boost/bind.hpp"
#include "connectionsmanager.h"
#include "conversions.h"
#include "iconnection.h"
#include "log.h"
#include "tcpsocket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IServer::IServer(boost::optional<uint32> workerThreadsCount) : ServiceBase(workerThreadsCount)
{
	m_acceptor = createAsioObject<boost::asio::ip::tcp::acceptor>(*getService());
}

IServer::~IServer()
{

}

bool IServer::getListening() const
{
	return m_acceptor->is_open();
}

bool IServer::getBindAddress(boost::asio::ip::address &address) const
{
	try
	{
		address = getEndpoint().address();
		return true;
	}
	catch(std::exception &)
	{
		
	}

	return false;
}

bool IServer::getBindPort(uint32 &port) const
{
	try
	{
		port = getEndpoint().port();
		return true;
	}
	catch(std::exception &)
	{
		
	}

	return false;
}

bool IServer::bind(const String &bindIP, uint32 port)
{
	try
	{
		m_acceptor->close();

		shared_ptr<boost::asio::ip::tcp::endpoint> bindEndpoint;
		if(bindIP.empty())
			bindEndpoint.reset(OS_NEW_T(boost::asio::ip::tcp::endpoint)(boost::asio::ip::tcp::v4(), port), os_delete_t());
		else
			bindEndpoint.reset(OS_NEW_T(boost::asio::ip::tcp::endpoint)(boost::asio::ip::address::from_string(bindIP.to_utf8()), port), os_delete_t());	
		
		m_acceptor->open(bindEndpoint->protocol());

		// VERYURGENT: Evita sotto Linux problemi sulla bind (vedere TIME_WAIT), da valutare se settarla anche sotto MAC e se sotto windows settare SO_EXCLUSIVEADDRUSE (SO_REUSEADDR sotto Windows ha un comportamento diverso)
#if OS_PLATFORM == OS_PLATFORM_LINUX
		m_acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
#endif

		m_acceptor->bind(*bindEndpoint);
		m_acceptor->listen();		

		return true;
	}
	catch(boost::system::system_error &e)
	{
		OS_LOG_ERROR(e.what());		
	}

	return false;
}

void IServer::acceptConnections(bool accept)
{
	if(accept)
	{
		if(m_acceptor->is_open() == false)
			return;
		
		shared_ptr<IConnection> connection = createConnection();
		m_acceptor->async_accept(connection->getSocket().lowest_layer(), boost::bind(&IServer::acceptCallback, this, boost::asio::placeholders::error, connection));	
	}
	else
	{
		m_acceptor->close();
	}
}

void IServer::acceptCallback(const boost::system::error_code &e, shared_ptr<IConnection> connection)
{
	if(e)
	{
		if(isAsioOperationAborted(e) == false)
			OS_LOG_ERROR(_S("Server error: ") + e.message().c_str());

		connection->close();
	}
	else
	{	
		if(onAccept(connection))
			getConnectionsManager()->addConnection(connection, true);	
		else
			connection->close();		
	}

	acceptConnections(true);		
}

void IServer::stop()
{
	// m_acceptor deve essere chiuso dall'esterno e non in fase di uscita del thread
	acceptConnections(false);
	
	ServiceBase::stop();
}

bool IServer::onEnter()
{
	if(ServiceBase::onEnter() == false)
		return false;

	acceptConnections(true);

	return true;
}
/*
bool IServer::onIdle()
{
	if(m_acceptor->is_open() == false)
		return false;

	return ServiceBase::onIdle();
}
*/
bool IServer::onAccept(shared_ptr<IConnection> connection)
{
	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
