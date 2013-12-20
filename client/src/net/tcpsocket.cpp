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
#include "tcpsocket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

TCPSocket::TCPSocket(boost::asio::io_service &service) : SocketBase(service)
{

}

TCPSocket::~TCPSocket()
{

}

void TCPSocket::connect(const endpoint_type &endpoint)
{
	boost::system::error_code e;
	connect(endpoint, e);
	boost::asio::detail::throw_error(e);
}

void TCPSocket::connect(const endpoint_type &endpoint, boost::system::error_code &e)
{
	if(m_layer != nullptr)
		m_layer->connect(endpoint, e);
	else
		m_socket->connect(endpoint, e);
}

void TCPSocket::async_connect(const endpoint_type &endpoint, const callback_type &callback)
{
	if(m_layer != nullptr)
		m_layer->async_connect(endpoint, callback);
	else
		m_socket->async_connect(endpoint, callback);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
