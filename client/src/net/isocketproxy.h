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

#ifndef _OS_NET_ISOCKETPROXY_H
#define _OS_NET_ISOCKETPROXY_H

#include "boost/bind.hpp"
#include "conversions.h"
#include "isocketlayer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename P>
class ISocketProxy : public ISocketLayer<P>
{
public:
	typedef ISocketLayer<P> LayerBase;
	typedef typename LayerBase::resolver_type resolver_type;
	typedef typename LayerBase::socket_type socket_type;
	typedef typename LayerBase::endpoint_type endpoint_type;
	typedef typename LayerBase::callback_type callback_type;

// Construction
public:
	ISocketProxy(shared_ptr<socket_type> socket);
	virtual ~ISocketProxy();

// Attributes
public:
	inline const std::string & getProxyHost() const;
	inline void setProxyHost(const std::string &proxyHost);

	inline uint32 getProxyPort() const;
	inline void setProxyPort(uint32 proxyPort);

	inline const endpoint_type & getProxyEndpoint() const;
	inline void setProxyEndpoint(const endpoint_type &proxyEndpoint);

// Operations
private:
	void async_attempt_connection(typename resolver_type::iterator endpoint, shared_ptr<callback_type> callback);
	void async_connection_callback(const boost::system::error_code &e, typename resolver_type::iterator endpoint, shared_ptr<callback_type> callback);

// ISocketLayer interface
public:
	virtual void connect(const endpoint_type &endpoint, boost::system::error_code &e);
	virtual void async_connect(const endpoint_type &endpoint, const callback_type &callback);

// Interface
protected:
	virtual void proxy_connect(boost::system::error_code &e) = 0;
	virtual void proxy_async_connect(const boost::system::error_code &e, shared_ptr<callback_type> callback) = 0;

// Overridables
protected:
	virtual void proxy_resolve_callback(const boost::system::error_code &e, typename resolver_type::iterator endpoint, shared_ptr<callback_type> callback);

protected:
	std::string m_proxyHost;
	uint32 m_proxyPort;
	endpoint_type m_proxyEndpoint;
	resolver_type m_proxyResolver;
};

//////////////////////////////////////////////////////////////////////

template <typename P>
ISocketProxy<P>::ISocketProxy(shared_ptr<socket_type> socket) : LayerBase(socket),
																m_proxyResolver(socket->get_io_service()),
																m_proxyPort(0)
{

}

template <typename P>
ISocketProxy<P>::~ISocketProxy()
{

}

template <typename P>
inline const std::string & ISocketProxy<P>::getProxyHost() const
{
	return m_proxyHost;
}

template <typename P>
inline void ISocketProxy<P>::setProxyHost(const std::string &proxyHost)
{
	m_proxyHost = proxyHost;
}

template <typename P>
inline uint32 ISocketProxy<P>::getProxyPort() const
{
	return m_proxyPort;
}

template <typename P>
inline void ISocketProxy<P>::setProxyPort(uint32 proxyPort)
{
	m_proxyPort = proxyPort;
}

template <typename P>
inline const typename ISocketProxy<P>::endpoint_type & ISocketProxy<P>::getProxyEndpoint() const
{
	return m_proxyEndpoint;
}

template <typename P>
inline void ISocketProxy<P>::setProxyEndpoint(const endpoint_type &proxyEndpoint)
{
	m_proxyEndpoint = proxyEndpoint;
}

template <typename P>
void ISocketProxy<P>::async_attempt_connection(typename resolver_type::iterator endpoint, shared_ptr<callback_type> callback)
{
	OS_ASSERT(endpoint != boost::asio::ip::tcp::resolver::iterator());

	boost::system::error_code tmp;
	LayerBase::close(tmp);

	// N.B.: creare un endpoint temporaneo o *endpoint verrebbe deferenziato dopo l'incremento di ++endpoint (con assert/crash se a quel punto è in posizione finale)
	LayerBase::getSocket()->async_connect(*endpoint, boost::bind(&ISocketProxy<P>::async_connection_callback, this, boost::asio::placeholders::error, ++typename resolver_type::iterator(endpoint), callback));
}

template <typename P>
void ISocketProxy<P>::async_connection_callback(const boost::system::error_code &e, typename resolver_type::iterator endpoint, shared_ptr<callback_type> callback)
{
	if(e)
	{
		// Connessione fallita

		this->notify_error(*callback, e);
	}
	else if(endpoint != boost::asio::ip::tcp::resolver::iterator())
	{
		// Connessione fallita, altri endpoints disponibili

		async_attempt_connection(endpoint, callback);
	}
	else
	{
		// Connessione riuscita

		proxy_async_connect(e, callback);
	}
}

template <typename P>
void ISocketProxy<P>::connect(const endpoint_type &endpoint, boost::system::error_code &e)
{
	OS_ASSERT(endpoint != endpoint_type());
	setProxyEndpoint(endpoint);

	OS_ASSERT(m_proxyHost.empty() == false);
	OS_ASSERT(m_proxyPort != 0);
	typename resolver_type::query query(m_proxyHost, conversions::to_string(m_proxyPort));
	boost::asio::ip::tcp::resolver::iterator iterator = m_proxyResolver.resolve(query);
	boost::asio::ip::tcp::resolver::iterator end;

	e = boost::asio::error::host_not_found;
	while(e && iterator != end)
	{
		boost::system::error_code tmp;
		LayerBase::close(tmp);

		LayerBase::getSocket()->connect(*iterator++, e);
	}

	if(e)
		return;

	proxy_connect(e);
}

template <typename P>
void ISocketProxy<P>::async_connect(const endpoint_type &endpoint, const callback_type &callback)
{
	OS_ASSERT(endpoint != endpoint_type());
	setProxyEndpoint(endpoint);

	boost::shared_ptr<callback_type> c(OS_NEW_T(callback_type)(callback), os_delete_t());	// Evita di copiare il callback

	OS_ASSERT(m_proxyHost.empty() == false);
	OS_ASSERT(m_proxyPort != 0);
	typename resolver_type::query query(m_proxyHost, conversions::to_string(m_proxyPort));
	m_proxyResolver.async_resolve(query, boost::bind(&ISocketProxy<P>::proxy_resolve_callback, this, _1, _2, c));
}

template <typename P>
void ISocketProxy<P>::proxy_resolve_callback(const boost::system::error_code &e, typename resolver_type::iterator i, shared_ptr<callback_type> callback)
{
	if(e)
	{
		this->notify_error(*callback, e);
		return;
	}

	async_attempt_connection(i, callback);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_ISOCKETPROXY_H
