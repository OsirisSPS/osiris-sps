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
#include "ircconnection.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "conversions.h"
#include "iirccommand.h"
#include "ircservice.h"
#include "ircsession.h"
#include "log.h"
#include "regexmanager.h"
#include "tcpsocket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IRCConnection::IRCConnection(shared_ptr<IRCSession> session, shared_ptr<TCPSocket> socket, shared_ptr<ConnectionsManager> connectionsManager) : ConnectionBase(connectionsManager),
																																				m_session(session),
																																				m_socket(socket),
																																				m_buffer(OS_NEW_T(boost::asio::streambuf), os_delete_t())
{
	OS_ASSERT(session != nullptr);
	OS_ASSERT(connectionsManager);

	shared_ptr<boost::asio::io_service> service = connectionsManager->getService();
	OS_ASSERT(service != nullptr);

	m_resolver = createAsioObject<boost::asio::ip::tcp::resolver>(*service);
	m_sslContext = createAsioSSLObject<boost::asio::ssl::context, boost::asio::ssl::context_base::method>(*service, boost::asio::ssl::context::sslv23);
//	m_sslSocket = createAsioSSLObject<boost::asio::ssl::stream<TCPSocket>, boost::asio::ssl::context &>(*service, *m_sslContext);
	// VERYURGENT: verificare se sotto Linux da comunque i problemi riportati nelle note di asio.h/asio_ssl.h (in tal caso creare un wrapper di allocazione/deallocazione)
	OS_ASSERT(m_socket != nullptr);
	m_sslStream.reset(OS_NEW_T(boost::asio::ssl::stream<TCPSocket &>)(*m_socket, *m_sslContext), os_delete_t());
}

IRCConnection::~IRCConnection()
{

}

boost::posix_time::time_duration IRCConnection::getTimeout() const
{
	uint32 timeout = OS_IRC_DEFAULT_TIMEOUT;

	shared_ptr<IRCSession> session = getSession();
	if(session != nullptr)
		timeout = session->getTimeout();

	return boost::posix_time::milliseconds(timeout);
}

bool IRCConnection::getEnableSSL() const
{
	shared_ptr<IRCSession> session = getSession();
	if(session != nullptr)
		return session->getEnableSSL();

	return false;
}

void IRCConnection::poll()
{
	shared_ptr<IRCSession> session = getSession();
	if(session != nullptr)
	{
		for(;;)
		{
			shared_ptr<IIRCCommand> request = session->popRequest();
			if(request == nullptr)
				break;

			pollCommand(request);
		}		
	}
}

TCPSocket & IRCConnection::getSocket()
{
	return m_sslStream->next_layer();
}

void IRCConnection::run()
{
	resolveIP(createScope(getTimeout()));
}

void IRCConnection::onTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{
	ConnectionBase::onTimeout(e, scope);

	OS_LOG_ERROR(_S("IRC timeout"));
}

void IRCConnection::onFinalize()
{
	notifyStatusChanged(ircConnectionStatusDisconnected);

	ConnectionBase::onFinalize();		
}

void IRCConnection::handleError(const boost::system::error_code &e)
{
	if(isAsioOperationAborted(e) == false)
		OS_LOG_ERROR(_S("IRC error: ") + e.message().c_str());
}

void IRCConnection::resolveIP(shared_ptr<ConnectionScope> scope)
{
	notifyStatusChanged(ircConnectionStatusConnecting);

	shared_ptr<IRCSession> session = getSession();
	if(session != nullptr)
	{
		OS_ASSERT(session->getServer().empty() == false);
		OS_ASSERT(session->getPort() != 0);
		boost::asio::ip::tcp::resolver::query query(session->getServer(), conversions::to_string(session->getPort()));
		m_resolver->async_resolve(query, boost::bind(&IRCConnection::resolveCallback, get_this_ptr<IRCConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::iterator, scope));
	}
}

void IRCConnection::openConnection(boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope)
{
	OS_ASSERT(endpoint != boost::asio::ip::tcp::resolver::iterator());

	close();
	// N.B.: creare un endpoint temporaneo o *endpoint verrebbe deferenziato dopo l'incremento di ++endpoint (con assert/crash se a quel punto è in posizione finale)
	getSocket().async_connect(*endpoint, boost::bind(&IRCConnection::connectionCallback, get_this_ptr<IRCConnection>(), boost::asio::placeholders::error, ++boost::asio::ip::tcp::resolver::iterator(endpoint), scope->extendTimeout()));
}

void IRCConnection::resolveCallback(const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
		handleError(e);
	else
		openConnection(endpoint, scope);
}

void IRCConnection::connectionCallback(const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
	{
		// Connessione fallita
		handleError(e);
	}
	else if(endpoint != boost::asio::ip::tcp::resolver::iterator())
	{
		// Connessione fallita, altri endpoints disponibili
		openConnection(endpoint, scope);
	}
	else
	{
		notifyStatusChanged(ircConnectionStatusConnected);

		if(getEnableSSL())
			m_sslStream->async_handshake(boost::asio::ssl::stream_base::client, boost::bind(&IRCConnection::handshakeCallback, get_this_ptr<IRCConnection>(), boost::asio::placeholders::error, scope->extendTimeout()));
		else
			update(scope);
	}
}

void IRCConnection::handshakeCallback(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
		handleError(e);
	else
		update(scope);
}

void IRCConnection::readCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<boost::asio::streambuf> buffer, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
	{
		handleError(e);
	}
	else
	{
		std::istream stream(buffer.get());
		std::string command;
		std::getline(stream, command);
		algorithms::trim(command);

		if(handleResponse(command))
			update(scope);
	}
}

void IRCConnection::writeCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<boost::asio::streambuf> buffer, shared_ptr<IIRCCommand> command, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
	{
		handleError(e);
	}
	else
	{
		notifyCommand(command);
		update(scope);
	}
}

void IRCConnection::pollCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<boost::asio::streambuf> buffer, shared_ptr<IIRCCommand> command)
{
	if(e)
	{
		handleError(e);
	}
	else
	{
		notifyCommand(command);
		poll();
	}
}

bool IRCConnection::handleResponse(const std::string &command)
{
	shared_ptr<IRCSession> session = getSession();
	if(session == nullptr)
		return false;

	return session->handleResponse(command);
}

void IRCConnection::readCommand(shared_ptr<ConnectionScope> scope)
{
	// N.B.: non usare qui un buffer temporaneo per la async_read_until

	if(getEnableSSL())
		boost::asio::async_read_until(*m_sslStream, *m_buffer, "\n", boost::bind(&IRCConnection::readCallback, get_this_ptr<IRCConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, m_buffer, scope->extendTimeout()));
	else
		boost::asio::async_read_until(getSocket(), *m_buffer, "\n", boost::bind(&IRCConnection::readCallback, get_this_ptr<IRCConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, m_buffer, scope->extendTimeout()));
}

void IRCConnection::writeCommand(shared_ptr<IIRCCommand> command, shared_ptr<ConnectionScope> scope)
{
	if(command == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	std::string str = command->toString();
	if(str.empty())
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<boost::asio::streambuf> buffer(OS_NEW_T(boost::asio::streambuf), os_delete_t());
	std::ostream stream(buffer.get());
	stream << str;

	if(getEnableSSL())
		boost::asio::async_write(*m_sslStream, *buffer, boost::asio::transfer_all(), boost::bind(&IRCConnection::writeCallback, get_this_ptr<IRCConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, buffer, command, scope->extendTimeout()));
	else
		boost::asio::async_write(getSocket(), *buffer, boost::asio::transfer_all(), boost::bind(&IRCConnection::writeCallback, get_this_ptr<IRCConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, buffer, command, scope->extendTimeout()));
}

void IRCConnection::pollCommand(shared_ptr<IIRCCommand> command)
{
	if(command == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	std::string str = command->toString();
	if(str.empty())
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<boost::asio::streambuf> buffer(OS_NEW_T(boost::asio::streambuf), os_delete_t());
	std::ostream stream(buffer.get());
	stream << str;

	if(getEnableSSL())
		boost::asio::async_write(*m_sslStream, *buffer, boost::asio::transfer_all(), boost::bind(&IRCConnection::pollCallback, get_this_ptr<IRCConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, buffer, command));
	else
		boost::asio::async_write(getSocket(), *buffer, boost::asio::transfer_all(), boost::bind(&IRCConnection::pollCallback, get_this_ptr<IRCConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, buffer, command));
}

void IRCConnection::notifyStatusChanged(IRCConnectionStatus status)
{
	shared_ptr<IRCSession> session = getSession();
	if(session != nullptr)
		session->notifyConnectionStatusChanged(status);
}

void IRCConnection::notifyCommand(shared_ptr<IIRCCommand> command)
{
	if(command == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<IRCSession> session = getSession();
	if(session != nullptr)
		session->notifyCommand(command);
}

void IRCConnection::update(shared_ptr<ConnectionScope> scope)
{
	OS_ASSERT(scope != nullptr);

	shared_ptr<IRCSession> session = getSession();
	if(session == nullptr)
		return;

	shared_ptr<IIRCCommand> command = session->popRequest();
	if(command != nullptr)
		writeCommand(command, scope);
	else
		readCommand(scope);	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
