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

#ifndef _OS_NET_SOCKET_H
#define _OS_NET_SOCKET_H

#include "base/object.h"
#include "boostasio.h"
#include "boost/function.hpp"
#include "isocketlayer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename P>
class Socket : public Object
{
public:
	typedef P protocol;
	typedef typename protocol::resolver resolver_type;
	typedef typename protocol::socket socket_type;
	typedef typename socket_type::lowest_layer_type lowest_layer_type;
	typedef typename socket_type::endpoint_type endpoint_type;
	typedef boost::function<void(const boost::system::error_code &)> callback_type;
	typedef ISocketLayer<protocol> layer_type;

// Construction
public:
	Socket(boost::asio::io_service &service);
	virtual ~Socket();

// Attributes
public:
	inline boost::asio::io_service & get_io_service();

	inline shared_ptr<layer_type> getLayer() const;
	inline void setLayer(shared_ptr<layer_type> layer);

	inline endpoint_type local_endpoint(boost::system::error_code &e) const;
	endpoint_type local_endpoint() const;

	inline endpoint_type remote_endpoint(boost::system::error_code &e) const;
	endpoint_type remote_endpoint() const;	

	inline lowest_layer_type & lowest_layer();
	inline const lowest_layer_type & lowest_layer() const;

	inline bool is_open() const;

// Operations
public:
	template <typename T>
	void io_control(T &command);
	template <typename T>
	void io_control(T &command, boost::system::error_code &e);

	template <typename T>
	void set_option(const T &option);
	template <typename T>
	boost::system::error_code set_option(const T &option, boost::system::error_code &e);

	void bind(const endpoint_type &endpoint);
	void bind(const endpoint_type &endpoint, boost::system::error_code &e);

	void open(const protocol &p);
	void open(const protocol &p, boost::system::error_code &e);

	void close();
	void close(boost::system::error_code &e);

	void shutdown(typename socket_type::shutdown_type what);
	void shutdown(typename socket_type::shutdown_type what, boost::system::error_code &e);

	template <typename T>
	shared_ptr<T> addLayer();
	shared_ptr<layer_type> removeLayer();

protected:
	shared_ptr<boost::asio::io_service> m_service;
	shared_ptr<socket_type> m_socket;
	shared_ptr<layer_type> m_layer;
};

//////////////////////////////////////////////////////////////////////

template <typename P>
Socket<P>::Socket(boost::asio::io_service &service) : m_socket(createAsioObject<socket_type>(service))
{

	// TIMEOUT?
	/*
#if defined _WINDOWS
    int32_t timeout = 5000;
    setsockopt(m_socket->native_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(m_socket->native_handle(), SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#else
    struct timeval tv;
    tv.tv_sec  = 5; 
    tv.tv_usec = 0;         
    setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
#endif
	*/

}

template <typename P>
Socket<P>::~Socket()
{
	close();
}

template <typename P>
inline boost::asio::io_service & Socket<P>::get_io_service()
{
	return m_socket->get_io_service();
}

template <typename P>
inline shared_ptr<typename Socket<P>::layer_type> Socket<P>::getLayer() const
{
	return m_layer;
}

template <typename P>
inline void Socket<P>::setLayer(shared_ptr<layer_type> layer)
{
	OS_ASSERT(is_open() == false);
	m_layer = layer;
}

template <typename P>
inline typename Socket<P>::endpoint_type Socket<P>::local_endpoint(boost::system::error_code &e) const
{
	return m_socket->local_endpoint(e);
}

template <typename P>
typename Socket<P>::endpoint_type Socket<P>::local_endpoint() const
{
	boost::system::error_code e;
	endpoint_type result = local_endpoint(e);
	boost::asio::detail::throw_error(e);
	return result;
}

template <typename P>
inline typename Socket<P>::endpoint_type Socket<P>::remote_endpoint(boost::system::error_code &e) const
{
	return m_socket->remote_endpoint(e);
}

template <typename P>
typename Socket<P>::endpoint_type Socket<P>::remote_endpoint() const
{
	boost::system::error_code e;
	endpoint_type result = remote_endpoint(e);
	boost::asio::detail::throw_error(e);
	return result;
}

template <typename P>
inline typename Socket<P>::lowest_layer_type & Socket<P>::lowest_layer()
{
	return m_socket->lowest_layer();
}

template <typename P>
inline const typename Socket<P>::lowest_layer_type & Socket<P>::lowest_layer() const
{
	return m_socket->lowest_layer();
}

template <typename P>
inline bool Socket<P>::is_open() const
{
	return m_socket->is_open();
}

template <typename P>
template <typename T>
void Socket<P>::io_control(T &command)
{
	boost::system::error_code e;
	io_control(command, e);
	boost::asio::detail::throw_error(e);
}

template <typename P>
template <typename T>
void Socket<P>::io_control(T &command, boost::system::error_code &e)
{
	m_socket->io_control(command, e);
}

template <typename P>
template <typename T>
void Socket<P>::set_option(const T &option)
{
	boost::system::error_code e;
	set_option(option, e);
	boost::asio::detail::throw_error(e);
}

template <typename P>
template <typename T>
boost::system::error_code Socket<P>::set_option(const T &option, boost::system::error_code &e)
{
	return m_socket->set_option(option, e);
}

template <typename P>
void Socket<P>::bind(const endpoint_type &endpoint)
{
	boost::system::error_code e;
	bind(endpoint, e);
	boost::asio::detail::throw_error(e);
}

template <typename P>
void Socket<P>::bind(const endpoint_type &endpoint, boost::system::error_code &e)
{
	m_socket->bind(endpoint, e);
}

template <typename P>
void Socket<P>::open(const protocol &p)
{
	boost::system::error_code e;
	open(p, e);
	boost::asio::detail::throw_error(e);
}

template <typename P>
void Socket<P>::open(const protocol &p, boost::system::error_code &e)
{
	if(m_layer != null)
		m_layer->open(p, e);
	else
		m_socket->open(p, e);
}

template <typename P>
void Socket<P>::close()
{
	boost::system::error_code e;
	close(e);
	boost::asio::detail::throw_error(e);
}

template <typename P>
void Socket<P>::close(boost::system::error_code &e)
{
	if(m_layer != null)
		m_layer->close(e);
	else
		m_socket->close(e);
}

template <typename P>
void Socket<P>::shutdown(typename socket_type::shutdown_type what)
{
	m_socket->shutdown(what);
}

template <typename P>
void Socket<P>::shutdown(typename socket_type::shutdown_type what, boost::system::error_code &e)
{
	m_socket->shutdown(what, e);
}

template <typename P>
template <typename T>
shared_ptr<T> Socket<P>::addLayer()
{
	OS_ASSERT(is_open() == false);

	shared_ptr<T> layer(OS_NEW T(m_socket));
	m_layer = layer;
	return layer;
}

template <typename P>
shared_ptr<typename Socket<P>::layer_type> Socket<P>::removeLayer()
{
	OS_ASSERT(is_open() == false);

	shared_ptr<layer_type> layer = m_layer;
	m_layer.reset();
	return layer;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_SOCKET_H
