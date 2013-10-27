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

#ifndef _OS_IRC_IRCCONNECTION_H
#define _OS_IRC_IRCCONNECTION_H

#include "boostasio_ssl.h"
#include "iconnection.h"
#include "irc.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IIRCCommand;
class IRCSession;

//////////////////////////////////////////////////////////////////////

class IRCConnection : public IConnection
{
	typedef IConnection ConnectionBase;

// Construction
public:
	IRCConnection(shared_ptr<IRCSession> session, shared_ptr<TCPSocket> socket, shared_ptr<ConnectionsManager> connectionsManager);
	virtual ~IRCConnection();

// Attributes
public:
	inline shared_ptr<IRCSession> getSession() const;

	boost::posix_time::time_duration getTimeout() const;
	bool getEnableSSL() const;

// Operations
public:
	void poll();

// IConnection interface
public:
	virtual TCPSocket & getSocket();
	virtual void run();

// IConnection overrides
protected:
	virtual void onTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);
	virtual void onFinalize();

private:
	void handleError(const boost::system::error_code &e);

	void resolveIP(shared_ptr<ConnectionScope> scope);
	void openConnection(boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope);

	void resolveCallback(const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope);
	void connectionCallback(const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope);
	void handshakeCallback(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);

	void readCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<boost::asio::streambuf> buffer, shared_ptr<ConnectionScope> scope);
	void writeCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<boost::asio::streambuf> buffer, shared_ptr<IIRCCommand> command, shared_ptr<ConnectionScope> scope);
	void pollCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<boost::asio::streambuf> buffer, shared_ptr<IIRCCommand> command);

	bool handleResponse(const std::string &command);

	void readCommand(shared_ptr<ConnectionScope> scope);
	void writeCommand(shared_ptr<IIRCCommand> command, shared_ptr<ConnectionScope> scope);
	void pollCommand(shared_ptr<IIRCCommand> command);
	
	void notifyStatusChanged(IRCConnectionStatus status);
	void notifyCommand(shared_ptr<IIRCCommand> command);

	void update(shared_ptr<ConnectionScope> scope);

private:
	weak_ptr<IRCSession> m_session;
	shared_ptr<boost::asio::ip::tcp::resolver> m_resolver;
	shared_ptr<boost::asio::ssl::context> m_sslContext;
	shared_ptr<TCPSocket> m_socket;
	shared_ptr<boost::asio::ssl::stream<TCPSocket &> > m_sslStream;
	shared_ptr<boost::asio::streambuf> m_buffer;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IRCSession> IRCConnection::getSession() const { return m_session.lock(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCCONNECTION_H
