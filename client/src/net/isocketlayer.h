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

#ifndef _OS_NET_ISOCKETLAYER_H
#define _OS_NET_ISOCKETLAYER_H

#include "base/object.h"
#include "boostasio.h"
#include "boost/function.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename P>
class ISocketLayerImpl;

//////////////////////////////////////////////////////////////////////

template <typename P>
class ISocketLayer : public ISocketLayerImpl<P>,
					 public Object
{
	typedef ISocketLayerImpl<P> SocketLayerImpl;

public:
	typedef typename SocketLayerImpl::protocol protocol;
	typedef typename SocketLayerImpl::resolver_type resolver_type;
	typedef typename SocketLayerImpl::socket_type socket_type;
	typedef typename SocketLayerImpl::lowest_layer_type lowest_layer_type;
	typedef typename SocketLayerImpl::endpoint_type endpoint_type;
	typedef typename SocketLayerImpl::callback_type callback_type;

// Construction
public:
	ISocketLayer(shared_ptr<socket_type> socket);
	virtual ~ISocketLayer();

// Operations
protected:
	void notify_error(const callback_type &callback, const boost::system::error_code &e, bool closeLayer = true);

// Overridables
public:
	virtual void open(const protocol &p, boost::system::error_code &e);
	virtual void close(boost::system::error_code &e);
};

//////////////////////////////////////////////////////////////////////

template <typename P>
ISocketLayer<P>::ISocketLayer(shared_ptr<socket_type> socket) : SocketLayerImpl(socket)
{

}

template <typename P>
ISocketLayer<P>::~ISocketLayer()
{

}

template <typename P>
void ISocketLayer<P>::notify_error(const callback_type &callback, const boost::system::error_code &e, bool closeLayer)
{
	if(closeLayer)
	{
		boost::system::error_code tmp;
		close(tmp);
	}

	callback(e);
}

template <typename P>
void ISocketLayer<P>::open(const protocol &p, boost::system::error_code &e)
{
	SocketLayerImpl::getSocket()->open(p, e);
}

template <typename P>
void ISocketLayer<P>::close(boost::system::error_code &e)
{
	SocketLayerImpl::getSocket()->close(e);
}

//////////////////////////////////////////////////////////////////////

template<>
class NetExport ISocketLayerImpl<boost::asio::ip::tcp>
{
public:
	typedef boost::asio::ip::tcp protocol;
	typedef protocol::resolver resolver_type;
	typedef protocol::socket socket_type;
	typedef socket_type::lowest_layer_type lowest_layer_type;
	typedef socket_type::endpoint_type endpoint_type;
	typedef boost::function<void(const boost::system::error_code &)> callback_type;

// Construction
public:
	ISocketLayerImpl(shared_ptr<socket_type> socket);
	virtual ~ISocketLayerImpl();

// Attributes
public:
	inline shared_ptr<socket_type> getSocket() const;

// Interface
public:
	virtual void connect(const endpoint_type &endpoint, boost::system::error_code &e) = 0;
	virtual void async_connect(const endpoint_type &endpoint, const callback_type &callback) = 0;

// Operations
public:
	template <typename T>
	std::size_t read_some(const T &buffers, boost::system::error_code &e);

	template <typename T, typename H>
	void async_read_some(const T &buffers, const H &handler);

	template <typename T>
	std::size_t write_some(const T &buffers, boost::system::error_code &e);

	template <typename T, typename H>
	void async_write_some(const T &buffers, const H &handler);

private:
	shared_ptr<socket_type> m_socket;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ISocketLayerImpl<boost::asio::ip::tcp>::socket_type> ISocketLayerImpl<boost::asio::ip::tcp>::getSocket() const
{
	return m_socket;
}

template <typename T>
std::size_t ISocketLayerImpl<boost::asio::ip::tcp>::read_some(const T &buffers, boost::system::error_code &e)
{
	return getSocket()->read_some(buffers, e);
}

template <typename T, typename H>
void ISocketLayerImpl<boost::asio::ip::tcp>::async_read_some(const T &buffers, const H &handler)
{
	getSocket()->async_read_some(buffers, handler);
}

template <typename T>
std::size_t ISocketLayerImpl<boost::asio::ip::tcp>::write_some(const T &buffers, boost::system::error_code &e)
{
	return getSocket()->write_some(buffers, e);
}

template <typename T, typename H>
void ISocketLayerImpl<boost::asio::ip::tcp>::async_write_some(const T &buffers, const H &handler)
{
	getSocket()->async_write_some(buffers, handler);
}

//////////////////////////////////////////////////////////////////////

template<>
class NetExport ISocketLayerImpl<boost::asio::ip::udp>
{
public:
	typedef boost::asio::ip::udp protocol;
	typedef protocol::resolver resolver_type;
	typedef protocol::socket socket_type;
	typedef socket_type::lowest_layer_type lowest_layer_type;
	typedef socket_type::endpoint_type endpoint_type;
	typedef boost::function<void(const boost::system::error_code &)> callback_type;

// Construction
public:
	ISocketLayerImpl(shared_ptr<socket_type> socket);
	virtual ~ISocketLayerImpl();

// Attributes
public:
	inline shared_ptr<socket_type> getSocket() const;

// Operations
public:
	template <typename T, typename H>
	void async_send_to(const T &buffers, const endpoint_type &destination, const H &handler);

	template <typename T, typename H>
	void async_receive_from(const T &buffers, endpoint_type &sender_endpoint, const H &handler);

private:
	shared_ptr<socket_type> m_socket;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ISocketLayerImpl<boost::asio::ip::udp>::socket_type> ISocketLayerImpl<boost::asio::ip::udp>::getSocket() const
{
	return m_socket;
}

template <typename T, typename H>
void ISocketLayerImpl<boost::asio::ip::udp>::async_send_to(const T &buffers, const endpoint_type &destination, const H &handler)
{
	getSocket()->async_send_to(buffers, destination, handler);
}

template <typename T, typename H>
void ISocketLayerImpl<boost::asio::ip::udp>::async_receive_from(const T &buffers, endpoint_type &sender_endpoint, const H &handler)
{
	getSocket()->async_receive_from(buffers, sender_endpoint, handler);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_ISOCKETLAYER_H
