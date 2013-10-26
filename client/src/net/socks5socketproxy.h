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

#ifndef _OS_NET_SOCKS5SOCKETPROXY_H
#define _OS_NET_SOCKS5SOCKETPROXY_H

#include "buffer.h"
#include "isocketproxy.h"

//////////////////////////////////////////////////////////////////////

#define OS_SOCKS5PROXY_VERSION										5

#define OS_SOCKS5PROXY_ADDRESS_TYPE_IPV4							1
#define OS_SOCKS5PROXY_ADDRESS_TYPE_DOMAINNAME						3
#define OS_SOCKS5PROXY_ADDRESS_TYPE_IPV6							4

#define OS_SOCKS5PROXY_AUTHENTICATIONMETHOD_NONE					0
#define OS_SOCKS5PROXY_AUTHENTICATIONMETHOD_GSSAPI					1
#define OS_SOCKS5PROXY_AUTHENTICATIONMETHOD_USERNAME_PASSWORD		2

#define OS_SOCKS5PROXY_REQUEST_CONNECT								1
#define OS_SOCKS5PROXY_REQUEST_BIND									2
#define OS_SOCKS5PROXY_REQUEST_UDP_ASSOCIATE						3

#define OS_SOCKS5PROXY_REPLY_SUCCEEDED								0
#define OS_SOCKS5PROXY_REPLY_GENERIC_ERROR							1
#define OS_SOCKS5PROXY_REPLY_CONNECTION_NOT_ALLOWED					2
#define OS_SOCKS5PROXY_REPLY_NETWORK_UNREACHABLE					3
#define OS_SOCKS5PROXY_REPLY_HOST_UNREACHABLE						4
#define OS_SOCKS5PROXY_REPLY_CONNECTION_REFUSED						5
#define OS_SOCKS5PROXY_REPLY_TIMED_OUT								6
#define OS_SOCKS5PROXY_REPLY_COMMAND_NOT_SUPPORTED					7
#define OS_SOCKS5PROXY_REPLY_ADDRESS_TYPE_NOT_SUPPORTED				8

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename P>
class Socks5SocketProxy : public ISocketProxy<P>
{
public:
	typedef ISocketProxy<P> ProxyBase;
	typedef typename ProxyBase::socket_type socket_type;
	typedef typename ProxyBase::endpoint_type endpoint_type;
	typedef typename ProxyBase::callback_type callback_type;

// Construction
public:
	Socks5SocketProxy(shared_ptr<socket_type> socket);
	virtual ~Socks5SocketProxy();

// Attributes
public:
	inline const std::string & getProxyUsername() const;
	inline void setProxyUsername(const std::string &proxyUsername);

	inline const std::string & getProxyPassword() const;
	inline void setProxyPassword(const std::string &proxyPassword);

// Operations
private:
	void initHelloBuffer();
	void initAuthenticationBuffer();
	void initRequestBuffer();
	void handshake1(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void handshake2(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void handshake3(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void handshake4(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void socks5_connect(shared_ptr<callback_type> callback);
	void connected1(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void connected2(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void connected3(const boost::system::error_code &e, shared_ptr<callback_type> callback);

// ISocketProxy interface
protected:
	virtual void proxy_connect(boost::system::error_code &e);
	virtual void proxy_async_connect(const boost::system::error_code &e, shared_ptr<callback_type> callback);

protected:
	std::string m_proxyUsername;
	std::string m_proxyPassword;
	scoped_ptr<Buffer> m_buffer;
};

//////////////////////////////////////////////////////////////////////

template <typename P>
Socks5SocketProxy<P>::Socks5SocketProxy(shared_ptr<socket_type> socket) : ProxyBase(socket),
																		  m_buffer(OS_NEW Buffer())
{

}

template <typename P>
Socks5SocketProxy<P>::~Socks5SocketProxy()
{

}

template <typename P>
inline const std::string & Socks5SocketProxy<P>::getProxyUsername() const
{
	return m_proxyUsername;
}

template <typename P>
inline void Socks5SocketProxy<P>::setProxyUsername(const std::string &proxyUsername)
{
	m_proxyUsername = proxyUsername;
}

template <typename P>
inline const std::string & Socks5SocketProxy<P>::getProxyPassword() const
{
	return m_proxyPassword;
}

template <typename P>
inline void Socks5SocketProxy<P>::setProxyPassword(const std::string &proxyPassword)
{
	m_proxyPassword = proxyPassword;
}

template <typename P>
void Socks5SocketProxy<P>::initHelloBuffer()
{
//	+----+----------+----------+
//	|VER | NMETHODS | METHODS  |
//  +----+----------+----------+
//  | 1  |    1     | 1 to 255 |
//  +----+----------+----------+

	m_buffer->clear();

	m_buffer->write<uint8>(OS_SOCKS5PROXY_VERSION);
	if(m_proxyUsername.empty())
	{
		// Invia come metodo di autenticazione supportato solo "no authentication"

		m_buffer->write<uint8>(1);
		m_buffer->write<uint8>(OS_SOCKS5PROXY_AUTHENTICATIONMETHOD_NONE);
	}
	else
	{
		// Invia due metodi di autenticazione supportati: "no authentication" e "username/password"

		m_buffer->write<uint8>(2);
		m_buffer->write<uint8>(OS_SOCKS5PROXY_AUTHENTICATIONMETHOD_NONE);
		m_buffer->write<uint8>(OS_SOCKS5PROXY_AUTHENTICATIONMETHOD_USERNAME_PASSWORD);
	}
}

template <typename P>
void Socks5SocketProxy<P>::initAuthenticationBuffer()
{
	OS_ASSERT(m_proxyUsername.empty() == false);

	m_buffer->clear();

	m_buffer->write<uint8>(1);
	m_buffer->write<uint8>(static_cast<uint8>(m_proxyUsername.size()));
	m_buffer->write(m_proxyUsername.data(), static_cast<uint32>(m_proxyUsername.size()));
	m_buffer->write<uint8>(static_cast<uint8>(m_proxyPassword.size()));
	m_buffer->write(m_proxyPassword.data(), static_cast<uint32>(m_proxyPassword.size()));
}

template <typename P>
void Socks5SocketProxy<P>::initRequestBuffer()
{
//	+----+-----+-------+------+----------+----------+
//	|VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
//  +----+-----+-------+------+----------+----------+
//  | 1  |  1  | X'00' |  1   | Variable |    2     |
//	+----+-----+-------+------+----------+----------+

	m_buffer->clear();

	const endpoint_type &endpoint = ProxyBase::getProxyEndpoint();
	boost::asio::ip::address address = endpoint.address();

	m_buffer->write<uint8>(OS_SOCKS5PROXY_VERSION);
	// Connect command
	m_buffer->write<uint8>(OS_SOCKS5PROXY_REQUEST_CONNECT);
	// Reserved
	m_buffer->write<uint8>(0);
	// Address type
	m_buffer->write<uint8>(address.is_v4() ? OS_SOCKS5PROXY_ADDRESS_TYPE_IPV4 : OS_SOCKS5PROXY_ADDRESS_TYPE_IPV6);
	if(address.is_v4())
	{
		m_buffer->write<uint32>(static_cast<uint32>(address.to_v4().to_ulong()));
	}
	else
	{
		boost::asio::ip::address_v6::bytes_type bytes = address.to_v6().to_bytes();
		m_buffer->write(bytes.data(), static_cast<uint32>(bytes.size()));
	}

	m_buffer->write<uint16>(endpoint.port());
}

template <typename P>
void Socks5SocketProxy<P>::handshake1(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

//	+----+--------+
//  |VER | METHOD |
//  +----+--------+
//  | 1  |   1    |
//  +----+--------+

	m_buffer->clear();
	m_buffer->reserve(2);

	boost::asio::async_read(*ProxyBase::getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&Socks5SocketProxy<P>::handshake2, this, _1, callback));
}

template <typename P>
void Socks5SocketProxy<P>::handshake2(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

	OS_ASSERT(m_buffer->getPosition() == m_buffer->getData());

	// Legge la versione del proxy
	uint8 version = 0;
	if(m_buffer->read(&version, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	if(version != OS_SOCKS5PROXY_VERSION)
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::operation_not_supported);
		return;
	}

	// Legge il metodo di autenticazione scelto dal proxy
	uint8 method = 0;
	if(m_buffer->read(&method, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	switch(method)
	{
	case OS_SOCKS5PROXY_AUTHENTICATIONMETHOD_NONE:					// Il proxy non richiede autenticazione, quindi effettua la connessione

																	socks5_connect(callback);
																	break;

	case OS_SOCKS5PROXY_AUTHENTICATIONMETHOD_USERNAME_PASSWORD:		// Invia il pacchetto di autenticazione

																	initAuthenticationBuffer();
																	OS_ASSERT(m_buffer->empty() == false);
																	boost::asio::async_write(*ProxyBase::getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&Socks5SocketProxy<P>::handshake3, this, _1, callback));
																	break;

	default:														// Il proxy ha risposto con un metodo di autenticazione non supportato: verificare i metodi supportati in initHelloBuffer

																	OS_ASSERTFALSE();		
																	notify_error(*callback, boost::asio::error::operation_not_supported);
																	break;
	}
}

template <typename P>
void Socks5SocketProxy<P>::handshake3(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

//	+----+--------+
//  |VER | STATUS |
//  +----+--------+
//  | 1  |   1    |
//  +----+--------+

	m_buffer->clear();
	m_buffer->reserve(2);

	boost::asio::async_read(*ProxyBase::getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&Socks5SocketProxy<P>::handshake4, this, _1, callback));
}

template <typename P>
void Socks5SocketProxy<P>::handshake4(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

	OS_ASSERT(m_buffer->getPosition() == m_buffer->getData());

	// Legge la versione del pacchetto
	uint8 version = 0;
	if(m_buffer->read(&version, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	if(version != 1)
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::operation_not_supported);
		return;
	}

	// Legge lo stato di autentificazione
	uint8 status = 0;
	if(m_buffer->read(&status, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	if(status != 0)
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::operation_not_supported);
		return;
	}

	socks5_connect(callback);
}

template <typename P>
void Socks5SocketProxy<P>::socks5_connect(shared_ptr<callback_type> callback)
{
	initRequestBuffer();
	OS_ASSERT(m_buffer->empty() == false);
	boost::asio::async_write(*ProxyBase::getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&Socks5SocketProxy<P>::connected1, this, _1, callback));
}

template <typename P>
void Socks5SocketProxy<P>::connected1(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

	// Risposta proxy
//	+----+-----+-------+------+----------+----------+
//  |VER | REP |  RSV  | ATYP | BND.ADDR | BND.PORT |
//  +----+-----+-------+------+----------+----------+
//  | 1  |  1  | X'00' |  1   | Variable |    2     |
//  +----+-----+-------+------+----------+----------+

	m_buffer->clear();
	m_buffer->reserve(4);	// VER + REP + RSV + ATYP

	boost::asio::async_read(*ProxyBase::getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&Socks5SocketProxy<P>::connected2, this, _1, callback));
}

template <typename P>
void Socks5SocketProxy<P>::connected2(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

	OS_ASSERT(m_buffer->getPosition() == m_buffer->getData());

	// Legge la versione del proxy
	uint8 version = 0;
	if(m_buffer->read(&version, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	if(version != OS_SOCKS5PROXY_VERSION)
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::operation_not_supported);
		return;
	}

	uint8 reply = 0;
	if(m_buffer->read(&reply, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	if(reply != OS_SOCKS5PROXY_REPLY_SUCCEEDED)
	{
		boost::system::error_code e = boost::asio::error::fault;
		switch(reply)
		{
		case OS_SOCKS5PROXY_REPLY_GENERIC_ERROR:				e = boost::asio::error::fault; 
																break;

		case OS_SOCKS5PROXY_REPLY_CONNECTION_NOT_ALLOWED:		e = boost::asio::error::no_permission; 
																break;

		case OS_SOCKS5PROXY_REPLY_NETWORK_UNREACHABLE:			e = boost::asio::error::network_unreachable; 
																break;

		case OS_SOCKS5PROXY_REPLY_HOST_UNREACHABLE:				e = boost::asio::error::host_unreachable; 
																break;

		case OS_SOCKS5PROXY_REPLY_CONNECTION_REFUSED:			e = boost::asio::error::connection_refused; 
																break;

		case OS_SOCKS5PROXY_REPLY_TIMED_OUT:					e = boost::asio::error::timed_out; 
																break;

		case OS_SOCKS5PROXY_REPLY_COMMAND_NOT_SUPPORTED:		e = boost::asio::error::operation_not_supported; 
																break;

		case OS_SOCKS5PROXY_REPLY_ADDRESS_TYPE_NOT_SUPPORTED:	e = boost::asio::error::address_family_not_supported; 
																break;

		default:												OS_ASSERTFALSE();
																break;
		}

		notify_error(*callback, e);
		return;
	}

	// Salta il campo reserved
	m_buffer->seekOffset(sizeof(uint8));

	uint8 addressType = 0;
	if(m_buffer->read(&addressType, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();

		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	uint8 size = 0;
	switch(addressType)
	{
	case OS_SOCKS5PROXY_ADDRESS_TYPE_IPV4:			size = 4;
													break;

	case OS_SOCKS5PROXY_ADDRESS_TYPE_DOMAINNAME:	if(m_buffer->read(&size, sizeof(uint8)) != sizeof(uint8))
													{
														OS_ASSERTFALSE();
														notify_error(*callback, boost::asio::error::no_recovery);
														return;
													}

													break;

	case OS_SOCKS5PROXY_ADDRESS_TYPE_IPV6:			size = 16;													
													break;

	default:										OS_ASSERTFALSE();										
													break;
	}

	if(size == 0)
	{
		notify_error(*callback, boost::asio::error::operation_not_supported);
		return;
	}

	size += 2;	// BND.PORT

	m_buffer->clear();
	m_buffer->reserve(size);

	boost::asio::async_read(*ProxyBase::getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&Socks5SocketProxy<P>::connected3, this, _1, callback));
}

template <typename P>
void Socks5SocketProxy<P>::connected3(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}	

	(*callback)(e);	
}

template <typename P>
void Socks5SocketProxy<P>::proxy_connect(boost::system::error_code &e)
{
	// TODO: fixme
	OS_ASSERTFALSE();
}

template <typename P>
void Socks5SocketProxy<P>::proxy_async_connect(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	initHelloBuffer();
	OS_ASSERT(m_buffer->empty() == false);
	boost::asio::async_write(*ProxyBase::getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&Socks5SocketProxy<P>::handshake1, this, _1, callback));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_SOCKS5SOCKETPROXY_H
