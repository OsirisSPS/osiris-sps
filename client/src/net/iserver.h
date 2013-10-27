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

#ifndef _OS_NET_ISERVER_H
#define _OS_NET_ISERVER_H

#include "networkservice.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class NetExport IServer : public NetworkService
{
	typedef NetworkService ServiceBase;	

// Construction
public:
	IServer(boost::optional<uint32> workerThreadsCount = boost::none);
	virtual ~IServer();

// Attributes
public:
	inline boost::asio::ip::tcp::endpoint getEndpoint() const;
	
	bool getListening() const;

	bool getBindAddress(boost::asio::ip::address &address) const;
	bool getBindPort(uint32 &port) const;	
	
// Operations
public:
	virtual bool bind(const String &bindIP = String::EMPTY, uint32 port = 0);

private:
	void acceptConnections(bool accept);
	void acceptCallback(const boost::system::error_code &e, shared_ptr<IConnection> connection);
	
// NetworkService overrides
public:
	virtual void stop();

protected:
	virtual bool onEnter();

// Overridables
protected:
	virtual bool onAccept(shared_ptr<IConnection> connection);	

// Interface
protected:
	virtual shared_ptr<IConnection> createConnection() = 0;

private:
	shared_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
};

//////////////////////////////////////////////////////////////////////

inline boost::asio::ip::tcp::endpoint IServer::getEndpoint() const { return m_acceptor->local_endpoint(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_ISERVER_H
