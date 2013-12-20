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

#ifndef _OS_NET_TCPSOCKET_H
#define _OS_NET_TCPSOCKET_H

#include "socket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class NetExport TCPSocket : public Socket<boost::asio::ip::tcp>
{
	typedef Socket<boost::asio::ip::tcp> SocketBase;
	
public:
	typedef SocketBase::resolver_type resolver_type;
	typedef SocketBase::socket_type socket_type;
	typedef SocketBase::lowest_layer_type lowest_layer_type;
	typedef SocketBase::endpoint_type endpoint_type;

// Construction
public:
	TCPSocket(boost::asio::io_service &service);
	virtual ~TCPSocket();
	
// Operations
public:
	void connect(const endpoint_type &endpoint);
	void connect(const endpoint_type &endpoint, boost::system::error_code &e);
	void async_connect(const endpoint_type &endpoint, const callback_type &callback);

	template <typename T>
	std::size_t read_some(const T &buffers);
	template <typename T>
	std::size_t read_some(const T &buffers, boost::system::error_code &e);
	template <typename T, typename H>
	void async_read_some(const T &buffers, const H &handler);

	template <typename T>
	std::size_t write_some(const T &buffers);
	template <typename T>
	std::size_t write_some(const T &buffers, boost::system::error_code &e);
	template <typename T, typename H>
	void async_write_some(const T &buffers, const H &handler);
};

//////////////////////////////////////////////////////////////////////

template <typename T>
std::size_t TCPSocket::read_some(const T &buffers)
{
	boost::system::error_code e;
	std::size_t result = read_some(buffers, e);
	boost::asio::detail::throw_error(e);
	return result;
}

template <typename T>
std::size_t TCPSocket::read_some(const T &buffers, boost::system::error_code &e)
{
	if(m_layer != nullptr)
		return m_layer->read_some(buffers, e);

	return m_socket->read_some(buffers, e);
}

template <typename T, typename H>
void TCPSocket::async_read_some(const T &buffers, const H &handler)
{
	if(m_layer != nullptr)
		m_layer->async_read_some(buffers, handler);
	else
		m_socket->async_read_some(buffers, handler);
}

template <typename T>
std::size_t TCPSocket::write_some(const T &buffers)
{
	boost::system::error_code e;
	std::size_t result = write_some(buffers, e);
	boost::asio::detail::throw_error(e);
	return result;
}

template <typename T>
std::size_t TCPSocket::write_some(const T &buffers, boost::system::error_code &e)
{
	if(m_layer != nullptr)
		return m_layer->write_some(buffers, e);

	return m_socket->write_some(buffers, e);
}

template <typename T, typename H>
void TCPSocket::async_write_some(const T &buffers, const H &handler)
{
	if(m_layer != nullptr)
		m_layer->async_write_some(buffers, handler);
	else
		m_socket->async_write_some(buffers, handler);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_TCPSOCKET_H
