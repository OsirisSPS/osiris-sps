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
#include "sockssocketproxy.h"

#include "boost/bind.hpp"
#include "netutils.h"

//////////////////////////////////////////////////////////////////////

#define OS_SOCKSSOCKETPROXY_V4											4
#define OS_SOCKSSOCKETPROXY_V5											5

#define OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_IPV4							1
#define OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_DOMAINNAME						3
#define OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_IPV6							4

#define OS_SOCKSSOCKETPROXY_AUTHENTICATIONMETHOD_NONE					0
#define OS_SOCKSSOCKETPROXY_AUTHENTICATIONMETHOD_GSSAPI					1
#define OS_SOCKSSOCKETPROXY_AUTHENTICATIONMETHOD_USERNAME_PASSWORD		2

#define OS_SOCKSSOCKETPROXY_COMMAND_CONNECT								1
#define OS_SOCKSSOCKETPROXY_COMMAND_BIND								2
#define OS_SOCKSSOCKETPROXY_COMMAND_UDP_ASSOCIATE						3

#define OS_SOCKSSOCKETPROXY_V4_REPLY_REQUEST_GRANTED					90
#define OS_SOCKSSOCKETPROXY_V4_REPLY_REQUEST_REJECTED					91
#define OS_SOCKSSOCKETPROXY_V4_REPLY_NO_IDENTD							92
#define OS_SOCKSSOCKETPROXY_V4_REPLY_IDENTD_ERROR						93

#define OS_SOCKSSOCKETPROXY_V5_REPLY_SUCCEEDED							0
#define OS_SOCKSSOCKETPROXY_V5_REPLY_GENERIC_ERROR						1
#define OS_SOCKSSOCKETPROXY_V5_REPLY_CONNECTION_NOT_ALLOWED				2
#define OS_SOCKSSOCKETPROXY_V5_REPLY_NETWORK_UNREACHABLE				3
#define OS_SOCKSSOCKETPROXY_V5_REPLY_HOST_UNREACHABLE					4
#define OS_SOCKSSOCKETPROXY_V5_REPLY_CONNECTION_REFUSED					5
#define OS_SOCKSSOCKETPROXY_V5_REPLY_TIMED_OUT							6
#define OS_SOCKSSOCKETPROXY_V5_REPLY_COMMAND_NOT_SUPPORTED				7
#define OS_SOCKSSOCKETPROXY_V5_REPLY_ADDRESS_TYPE_NOT_SUPPORTED			8

////////////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

////////////////////////////////////////////////////////////////////////////

const SocksSocketProxy::endpoint_type SocksSocketProxy::null_endpoint;

////////////////////////////////////////////////////////////////////////////

SocksSocketProxy::SocksSocketProxy(shared_ptr<socket_type> socket, Version proxyVersion) : ProxyBase(socket),
																						   m_proxyVersion(proxyVersion),
																						   m_proxyCommand(connect),
																						   m_buffer(OS_NEW Buffer())
{

}

SocksSocketProxy::~SocksSocketProxy()
{

}

void SocksSocketProxy::setBindAddress(const boost::asio::ip::address &address)
{
	if(m_bindEndpoint == nullptr)
		m_bindEndpoint.reset(OS_NEW_T(endpoint_type));

	m_bindEndpoint->address(address);
}

void SocksSocketProxy::setBindPort(uint32 port)
{
	if(m_bindEndpoint == nullptr)
		m_bindEndpoint.reset(OS_NEW_T(endpoint_type));

	m_bindEndpoint->port(port);
}

bool SocksSocketProxy::createHelloRequestV5()
{
//	+----+----------+----------+
//	|VER | NMETHODS | METHODS  |
//  +----+----------+----------+
//  | 1  |    1     | 1 to 255 |
//  +----+----------+----------+

	m_buffer->reset();

	m_buffer->write<uint8>(OS_SOCKSSOCKETPROXY_V5);
	if(m_proxyUsername.empty())
	{
		// Invia come metodo di autenticazione supportato solo "no authentication"

		m_buffer->write<uint8>(1);
		m_buffer->write<uint8>(OS_SOCKSSOCKETPROXY_AUTHENTICATIONMETHOD_NONE);
	}
	else
	{
		// Invia due metodi di autenticazione supportati: "no authentication" e "username/password"

		m_buffer->write<uint8>(2);
		m_buffer->write<uint8>(OS_SOCKSSOCKETPROXY_AUTHENTICATIONMETHOD_NONE);
		m_buffer->write<uint8>(OS_SOCKSSOCKETPROXY_AUTHENTICATIONMETHOD_USERNAME_PASSWORD);
	}

	return true;
}

bool SocksSocketProxy::prepareHelloResponseV5()
{
//	+----+--------+
//  |VER | METHOD |
//  +----+--------+
//  | 1  |   1    |
//  +----+--------+

	m_buffer->reset();
	return m_buffer->reserve(2);
}

bool SocksSocketProxy::parseHelloResponseV5(uint8 &method)
{
	// Legge la versione del proxy
	uint8 version = 0;
	if(m_buffer->read(&version, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(version != OS_SOCKSSOCKETPROXY_V5)
	{
		OS_ASSERTFALSE();
		return false;
	}

	// Legge il metodo di autenticazione scelto dal proxy
	if(m_buffer->read(&method, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();
		return false;
	}

	return true;
}

void SocksSocketProxy::createAuthenticationRequest()
{
	OS_ASSERT(m_proxyUsername.empty() == false);

	m_buffer->reset();

	m_buffer->write<uint8>(1);
	m_buffer->write<uint8>(static_cast<uint8>(m_proxyUsername.size()));
	m_buffer->write(m_proxyUsername.data(), static_cast<uint32>(m_proxyUsername.size()));
	m_buffer->write<uint8>(static_cast<uint8>(m_proxyPassword.size()));
	m_buffer->write(m_proxyPassword.data(), static_cast<uint32>(m_proxyPassword.size()));
}

bool SocksSocketProxy::prepareAuthenticationResponse()
{
//	+----+--------+
//  |VER | STATUS |
//  +----+--------+
//  | 1  |   1    |
//  +----+--------+

	m_buffer->reset();
	return m_buffer->reserve(2);
}

bool SocksSocketProxy::parseAuthenticationResponse()
{
	// Legge la versione del pacchetto
	uint8 version = 0;
	if(m_buffer->read(&version, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(version != 1)
	{
		OS_ASSERTFALSE();
		return false;
	}

	// Legge lo stato di autentificazione
	uint8 status = 0;
	if(m_buffer->read(&status, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(status != 0)
	{
		OS_ASSERTFALSE();
		return false;
	}

	return true;
}

bool SocksSocketProxy::createConnectionRequest()
{
	const endpoint_type &endpoint = m_proxyCommand == connect ? ProxyBase::getProxyEndpoint() : null_endpoint;

	switch(m_proxyVersion)
	{
	case v4:
	case v4a:
				return createConnectionRequestV4(endpoint);

	case v5:	return createConnectionRequestV5(endpoint);

	default:	OS_ASSERTFALSE();
				break;
	}

	return false;
}

bool SocksSocketProxy::createConnectionRequestV4(const endpoint_type &endpoint)
{
	OS_ASSERT(m_proxyPassword.empty());	// Non supportato nella versione 4

	m_buffer->reset();	

	boost::asio::ip::address address = endpoint.address();

	if(address.is_v6())
	{
		// Non supportato nella versione 4
		OS_ASSERTFALSE();
		return false;
	}

	uint8 command = 0;
	switch(m_proxyCommand)
	{
	case connect:			command = OS_SOCKSSOCKETPROXY_COMMAND_CONNECT;
							break;

	case bind:				command = OS_SOCKSSOCKETPROXY_COMMAND_BIND;
							break;

	default:				// N.B.: udp_associate non è supportato nella versione 4
		
							OS_ASSERTFALSE();
							return false;
	}

	// Version
    m_buffer->write<uint8>(OS_SOCKSSOCKETPROXY_V4);
	// Command
    m_buffer->write<uint8>(command);
	// Port
	m_buffer->write<uint16>(utils::hostToNetwork16(endpoint.port()));
	// Address
	m_buffer->write<uint32>(utils::hostToNetwork32(static_cast<uint32>(address.to_v4().to_ulong())));
	// Username
	if(m_proxyUsername.empty() == false)
		m_buffer->write(m_proxyUsername.data(), static_cast<uint32>(m_proxyUsername.size()));
	// Username terminator
	m_buffer->write<uint8>(0);

	return true;
}

bool SocksSocketProxy::createConnectionRequestV5(const endpoint_type &endpoint)
{
//	+----+-----+-------+------+----------+----------+
//	|VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
//  +----+-----+-------+------+----------+----------+
//  | 1  |  1  | X'00' |  1   | Variable |    2     |
//	+----+-----+-------+------+----------+----------+

	m_buffer->reset();

	boost::asio::ip::address address = endpoint.address();

	uint8 command = 0;
	switch(m_proxyCommand)
	{
	case connect:			command = OS_SOCKSSOCKETPROXY_COMMAND_CONNECT;
							break;

	case bind:				command = OS_SOCKSSOCKETPROXY_COMMAND_BIND;
							break;

	case udp_associate:		command = OS_SOCKSSOCKETPROXY_COMMAND_UDP_ASSOCIATE;
							break;

	default:				OS_ASSERTFALSE();
							return false;
	}

	// Version
	m_buffer->write<uint8>(OS_SOCKSSOCKETPROXY_V5);
	// Command
	m_buffer->write<uint8>(command);
	// Reserved
	m_buffer->write<uint8>(0);
	// Address type
	m_buffer->write<uint8>(address.is_v4() ? OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_IPV4 : OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_IPV6);
	// Address
	if(address.is_v4())
	{
		m_buffer->write<uint32>(utils::hostToNetwork32(static_cast<uint32>(address.to_v4().to_ulong())));
	}
	else
	{
		boost::asio::ip::address_v6::bytes_type bytes = address.to_v6().to_bytes();		
		for(boost::asio::ip::address_v6::bytes_type::iterator i = bytes.begin() , end(bytes.end()); i != end; ++i)
		{
			m_buffer->write<uint8>(*i);			
		}					
	}
	// Port
	m_buffer->write<uint16>(utils::hostToNetwork16(endpoint.port()));

	return true;
}

bool SocksSocketProxy::prepareConnectionResponse()
{
	// N.B.: data la struttura delle risposte v4/v4a/v5 viene allocata come dimensione minima 4 bytes e poi, in base alla versione, gestita 
	// un'ulteriore read per i dati mancanti

	// v4/v4a:	VERSION(1) + RESPONSE(1) + PORT(2) + ADDRESS(4)
	// v5:		VERSION(1) + RESPONSE(1) + RESERVED(1) + ADDRESS_TYPE(1) + BND.ADDR(variable) + BND.PORT(2)

	m_buffer->reset();
	return m_buffer->reserve(4);
}

bool SocksSocketProxy::parseConnectionResponse(boost::system::error_code &e, uint8 &addressType, uint32 &pending)
{
	switch(m_proxyVersion)
	{
	case v4:
	case v4a:
				return parseConnectionResponseV4(e, addressType, pending);

	case v5:	return parseConnectionResponseV5(e, addressType, pending);

	default:	OS_ASSERTFALSE();
				break;
	}

	return false;
}

bool SocksSocketProxy::parseConnectionResponseV4(boost::system::error_code &e, uint8 &addressType, uint32 &pending)
{
	uint8 version = 0;
	if(m_buffer->read(&version, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(version != 0)	// Nella versione 4 il primo byte è sempre zero
	{
		OS_ASSERTFALSE();
		return false;
	}

	uint8 reply = 0;
	if(m_buffer->read(&reply, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(reply != OS_SOCKSSOCKETPROXY_V4_REPLY_REQUEST_GRANTED)
	{
		switch(reply)
		{
		case OS_SOCKSSOCKETPROXY_V4_REPLY_REQUEST_REJECTED:		e = boost::asio::error::connection_refused; 
																break;

		case OS_SOCKSSOCKETPROXY_V4_REPLY_NO_IDENTD:
		case OS_SOCKSSOCKETPROXY_V4_REPLY_IDENTD_ERROR:			e = boost::asio::error::no_permission; 
																break;

		default:												OS_ASSERTFALSE();
																e = boost::asio::error::fault; 
																break;
		}

		return false;
	}

	uint16 port = 0;
	if(m_buffer->read(&port, sizeof(uint16)) != sizeof(uint16))
	{
		OS_ASSERTFALSE();
		return false;
	}
	setBindPort(utils::networkToHost16(port));

	addressType = OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_IPV4;
	pending = 4;		// BND.ADDRESS

	return true;
}

bool SocksSocketProxy::parseConnectionResponseV5(boost::system::error_code &e, uint8 &addressType, uint32 &pending)
{
	uint8 version = 0;
	if(m_buffer->read(&version, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(version < OS_SOCKSSOCKETPROXY_V5)
	{
		OS_ASSERTFALSE();
		return false;
	}

	uint8 reply = 0;
	if(m_buffer->read(&reply, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(reply != OS_SOCKSSOCKETPROXY_V5_REPLY_SUCCEEDED)
	{		
		switch(reply)
		{
		case OS_SOCKSSOCKETPROXY_V5_REPLY_GENERIC_ERROR:				e = boost::asio::error::fault; 
																		break;

		case OS_SOCKSSOCKETPROXY_V5_REPLY_CONNECTION_NOT_ALLOWED:		e = boost::asio::error::no_permission; 
																		break;

		case OS_SOCKSSOCKETPROXY_V5_REPLY_NETWORK_UNREACHABLE:			e = boost::asio::error::network_unreachable; 
																		break;

		case OS_SOCKSSOCKETPROXY_V5_REPLY_HOST_UNREACHABLE:				e = boost::asio::error::host_unreachable; 
																		break;

		case OS_SOCKSSOCKETPROXY_V5_REPLY_CONNECTION_REFUSED:			e = boost::asio::error::connection_refused; 
																		break;

		case OS_SOCKSSOCKETPROXY_V5_REPLY_TIMED_OUT:					e = boost::asio::error::timed_out; 
																		break;

		case OS_SOCKSSOCKETPROXY_V5_REPLY_COMMAND_NOT_SUPPORTED:		e = boost::asio::error::operation_not_supported; 
																		break;

		case OS_SOCKSSOCKETPROXY_V5_REPLY_ADDRESS_TYPE_NOT_SUPPORTED:	e = boost::asio::error::address_family_not_supported; 
																		break;

		default:														OS_ASSERTFALSE();
																		e = boost::asio::error::fault;
																		break;
		}

		return false;
	}

	// Salta il campo "reserved"
	if(m_buffer->seekOffset(sizeof(uint8)) == false)
		return false;

	if(m_buffer->read(&addressType, sizeof(uint8)) != sizeof(uint8))
	{
		OS_ASSERTFALSE();
		return false;
	}
	
	switch(addressType)
	{
	case OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_IPV4:			pending = 4;
														break;

	case OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_DOMAINNAME:	{
															uint8 size = 0;
															if(m_buffer->read(&size, sizeof(uint8)) != sizeof(uint8))
															{
																OS_ASSERTFALSE();
																return false;
															}

															pending = size;
														}

														break;

	case OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_IPV6:			pending = 16;													
														break;

	default:											OS_ASSERTFALSE();
														addressType = 0;
														break;
	}

	if(pending == 0)
		return false;
	
	pending += 2;	// BND.PORT
	
	return true;
}

bool SocksSocketProxy::parseConnectionResponseData(boost::system::error_code &e, uint8 addressType)
{
	switch(m_proxyVersion)
	{
	case v4:
	case v4a:
				return parseConnectionResponseDataV4(e, addressType);

	case v5:	return parseConnectionResponseDataV5(e, addressType);

	default:	OS_ASSERTFALSE();
				break;
	}

	return false;
}

bool SocksSocketProxy::parseConnectionResponseDataV4(boost::system::error_code &e, uint8 addressType)
{
	if(addressType != OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_IPV4)
	{
		OS_ASSERTFALSE();
		return false;
	}

	uint32 address = 0;
	if(m_buffer->read(&address, sizeof(uint32)) != sizeof(uint32))
	{
		OS_ASSERTFALSE();
		return false;
	}
	setBindAddress(boost::asio::ip::address_v4::address_v4(utils::networkToHost32(address)));
	
	return true;
}

bool SocksSocketProxy::parseConnectionResponseDataV5(boost::system::error_code &e, uint8 addressType)
{
	if(addressType != OS_SOCKSSOCKETPROXY_ADDRESS_TYPE_IPV4)
	{
		// TODO: fixme
		e = boost::asio::error::operation_not_supported; 
		return false;
	}

	uint32 address = 0;
	if(m_buffer->read(&address, sizeof(uint32)) != sizeof(uint32))
	{
		OS_ASSERTFALSE();
		e = boost::asio::error::eof;
		return false;
	}
	setBindAddress(boost::asio::ip::address_v4::address_v4(utils::networkToHost32(address)));

	uint16 port = 0;
	if(m_buffer->read(&port, sizeof(uint16)) != sizeof(uint16))
	{
		OS_ASSERTFALSE();
		e = boost::asio::error::eof;
		return false;
	}
	setBindPort(utils::networkToHost16(port));

	return true;
}

bool SocksSocketProxy::socksConnect(boost::system::error_code &e)
{
	if(createConnectionRequest() == false)
		return false;

	writePacket(e);
	if(e)
		return false;

	if(prepareConnectionResponse() == false)
		return false;

	readPacket(e);
	if(e)
		return false;

	uint8 addressType = 0;
	uint32 pending = 0;
	if(parseConnectionResponse(e, addressType, pending) == false)
		return false;
	
	if(e)
		return false;

	if(pending > 0)
	{
		m_buffer->reset();
		m_buffer->reserve(pending);		

		readPacket(e);
		if(e)
			return false;

		if(parseConnectionResponseData(e, addressType) == false)
			return false;

		if(e)
			return false;
	}

	return true;
}

bool SocksSocketProxy::socksAuthenticate(boost::system::error_code &e)
{
	createAuthenticationRequest();
	
	writePacket(e);
	if(e)
		return false;

	if(prepareAuthenticationResponse() == false)
		return false;
	
	readPacket(e);
	if(e)
		return false;

	if(parseAuthenticationResponse() == false)
		return false;

	return socksConnect(e);
}

bool SocksSocketProxy::socksConnectV4(boost::system::error_code &e)
{
	return socksConnect(e);
}

bool SocksSocketProxy::socksConnectV5(boost::system::error_code &e)
{
	if(createHelloRequestV5() == false)
		return false;
	
	writePacket(e);
	if(e)
		return false;

	if(prepareHelloResponseV5() == false)
		return false;

	readPacket(e);
	if(e)
		return false;

	uint8 method = 0;
	if(parseHelloResponseV5(method) == false)
		return false;

	switch(method)
	{
	case OS_SOCKSSOCKETPROXY_AUTHENTICATIONMETHOD_NONE:					// Il proxy non richiede autenticazione, quindi effettua la connessione

																		return socksConnect(e);
																	
	case OS_SOCKSSOCKETPROXY_AUTHENTICATIONMETHOD_USERNAME_PASSWORD:	// Invia il pacchetto di autenticazione

																		return socksAuthenticate(e);

	default:															// Il proxy ha risposto con un metodo di autenticazione non supportato: verificare i metodi supportati in createHelloRequestV5

																		OS_ASSERTFALSE();		
																		e = boost::asio::error::operation_not_supported;

																		break;
	}

	return false;
}

void SocksSocketProxy::handshakeCallback1(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

	if(prepareHelloResponseV5() == false)
	{
		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	boost::asio::async_read(*getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&SocksSocketProxy::handshakeCallback2, this, _1, callback));
}

void SocksSocketProxy::handshakeCallback2(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

	uint8 method = 0;
	if(parseHelloResponseV5(method) == false)
	{
		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	switch(method)
	{
	case OS_SOCKSSOCKETPROXY_AUTHENTICATIONMETHOD_NONE:					// Il proxy non richiede autenticazione, quindi effettua la connessione

																		asyncSocksConnect(callback);

																		break;

	case OS_SOCKSSOCKETPROXY_AUTHENTICATIONMETHOD_USERNAME_PASSWORD:	// Invia il pacchetto di autenticazione

																		createAuthenticationRequest();																	
																		boost::asio::async_write(*getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&SocksSocketProxy::handshakeCallback3, this, _1, callback));

																		break;

	default:															// Il proxy ha risposto con un metodo di autenticazione non supportato: verificare i metodi supportati in createHelloRequestV5

																		OS_ASSERTFALSE();		
																		notify_error(*callback, boost::asio::error::operation_not_supported);

																		break;
	}
}

void SocksSocketProxy::handshakeCallback3(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

	if(prepareAuthenticationResponse() == false)
	{
		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	boost::asio::async_read(*getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&SocksSocketProxy::handshakeCallback4, this, _1, callback));
}

void SocksSocketProxy::handshakeCallback4(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

	if(parseAuthenticationResponse() == false)
	{	
		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	asyncSocksConnect(callback);
}

void SocksSocketProxy::connectedCallback1(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

	if(prepareConnectionResponse() == false)
	{
		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	boost::asio::async_read(*getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&SocksSocketProxy::connectedCallback2, this, _1, callback));
}

void SocksSocketProxy::connectedCallback2(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}

	boost::system::error_code responseError;
	uint8 addressType = 0;
	uint32 pending = 0;
	if(parseConnectionResponse(responseError, addressType, pending) == false)
	{
		notify_error(*callback, responseError ? responseError : boost::asio::error::no_recovery);
		return;
	}	

	if(responseError)
	{
		(*callback)(e);	
	}
	else
	{		 
		if(pending > 0)
		{
			m_buffer->reset();
			m_buffer->reserve(pending);		

			boost::asio::async_read(*getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&SocksSocketProxy::connectedCallback3, this, _1, addressType, callback));
		}
		else
		{
			(*callback)(e);	
		}
	}
}

void SocksSocketProxy::connectedCallback3(const boost::system::error_code &e, uint8 addressType, shared_ptr<callback_type> callback)
{
	if(e)
	{
		notify_error(*callback, e);
		return;
	}	

	boost::system::error_code responseError;
	if(parseConnectionResponseData(responseError, addressType) == false)
	{
		notify_error(*callback, responseError ? responseError : boost::asio::error::no_recovery);		
		return;
	}
	
	(*callback)(e);	
}

void SocksSocketProxy::asyncSocksConnect(shared_ptr<callback_type> callback)
{
	if(createConnectionRequest() == false)
	{
		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}

	boost::asio::async_write(*getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&SocksSocketProxy::connectedCallback1, this, _1, callback));
}

void SocksSocketProxy::asyncSocksConnectV4(shared_ptr<callback_type> callback)
{
	asyncSocksConnect(callback);
}

void SocksSocketProxy::asyncSocksConnectV5(shared_ptr<callback_type> callback)
{
	if(createHelloRequestV5() == false)
	{
		notify_error(*callback, boost::asio::error::no_recovery);
		return;
	}
	
	boost::asio::async_write(*getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::bind(&SocksSocketProxy::handshakeCallback1, this, _1, callback));
}

void SocksSocketProxy::readPacket(boost::system::error_code &e)
{
	OS_ASSERT(m_buffer->getPosition() == m_buffer->getData());
	OS_ASSERT(m_buffer->empty() == false);

	boost::asio::read(*getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::asio::transfer_all(), e);
}

void SocksSocketProxy::writePacket(boost::system::error_code &e)
{
	OS_ASSERT(m_buffer->empty() == false);

	boost::asio::write(*getSocket(), boost::asio::buffer(m_buffer->getData(), m_buffer->getSize()), boost::asio::transfer_all(), e);
}

void SocksSocketProxy::reset()
{
	m_bindEndpoint.reset();
}

void SocksSocketProxy::proxy_connect(boost::system::error_code &e)
{
	reset();

	switch(m_proxyVersion)
	{
	case v4:
	case v4a:
				socksConnectV4(e);
				break;

	case v5:	socksConnectV5(e);
				break;

	default:	OS_ASSERTFALSE();
				break;
	}	
}

void SocksSocketProxy::proxy_async_connect(const boost::system::error_code &e, shared_ptr<callback_type> callback)
{
	reset();

	switch(m_proxyVersion)
	{
	case v4:
	case v4a:
				asyncSocksConnectV4(callback);

				break;

	case v5:	asyncSocksConnectV5(callback);
				
				break;

	default:	OS_ASSERTFALSE();
				notify_error(*callback, boost::asio::error::no_recovery);

				break;
	}	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
