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

#ifndef _OS_NET_UDPSOCKET_H
#define _OS_NET_UDPSOCKET_H

#include "socket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class NetExport UDPSocket : public Socket<boost::asio::ip::udp>
{
	typedef Socket<boost::asio::ip::udp> SocketBase;

public:
	typedef SocketBase::resolver_type resolver_type;
	typedef SocketBase::socket_type socket_type;
	typedef SocketBase::lowest_layer_type lowest_layer_type;
	typedef SocketBase::endpoint_type endpoint_type;

// Construction
public:
	UDPSocket(boost::asio::io_service &service);
	virtual ~UDPSocket();

// Operations
public:
	template <typename T, typename H>
	void async_send_to(const T &buffers, const endpoint_type &destination, const H &handler);

	template <typename T, typename H>
	void async_receive_from(const T &buffers, endpoint_type &sender_endpoint, const H &handler);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename H>
void UDPSocket::async_send_to(const T &buffers, const endpoint_type &destination, const H &handler)
{
	if(m_layer != null)
		m_layer->async_send_to(buffers, destination, handler);
	else
		m_socket->async_send_to(buffers, destination, handler);
}

template <typename T, typename H>
void UDPSocket::async_receive_from(const T &buffers, endpoint_type &sender_endpoint, const H &handler)
{
	if(m_layer != null)
		m_layer->async_receive_from(buffers, sender_endpoint, handler);
	else
		m_socket->async_receive_from(buffers, sender_endpoint, handler);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_UDPSOCKET_H
