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
#include "ntpclient.h"

#include "boost/bind.hpp"
#include "conversions.h"
#include "constants.h"
#include "log.h"
#include "ntp.h"
#include "platformmanager.h"
#include "udpsocket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

NTPClient::NTPClient(shared_ptr<boost::asio::io_service> service, shared_ptr<UDPSocket> socket) : m_service(service),
																								  m_socket(socket),
																								  m_timeout(OS_NTP_REQUEST_TIMEOUT),
																								  m_resolver(createAsioObject<boost::asio::ip::udp::resolver>(*service))
{	
	OS_ASSERT(m_service != null);
	OS_ASSERT(m_socket != null);
	m_socket->open(boost::asio::ip::udp::v4());		// URGENT: non andrebbe a monte?
}

NTPClient::~NTPClient()
{

}

shared_ptr<NTPResponse> NTPClient::query(shared_ptr<boost::asio::io_service> service, shared_ptr<UDPSocket> socket, const String &server, uint32 port)
{
	try
	{
		if(socket == null)
		{
			OS_ASSERTFALSE();
			return null;
		}
		
		shared_ptr<NTPClient> client(OS_NEW NTPClient(service, socket));
		client->query(boost::asio::ip::udp::resolver::query(boost::asio::ip::udp::v4(), server.to_ascii(), conversions::to_string(port)));
		// VERYURGENT: fixme (sul servizio potrebbero essere accodate altre operazione che non sono da processare...)
		// O si fa una versione sincrona o si gestisce dall'esterno in versione asincrona		
		service->run();

		return client->m_response;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(_S("Ntp error: ") + e.what());
	}

	return null;
}

void NTPClient::query(const boost::asio::ip::udp::resolver::query &query)
{
	m_response.reset();
	shared_ptr<ConnectionScope> scope(OS_NEW ConnectionScope(m_service, get_this_ptr()));
	scope->extendTimeout(boost::posix_time::milliseconds(m_timeout));
	m_resolver->async_resolve(query, boost::bind(&NTPClient::resolveCallback, get_this_ptr(), boost::asio::placeholders::error, boost::asio::placeholders::iterator, scope));
}

void NTPClient::resolveCallback(const boost::system::error_code &e, boost::asio::ip::udp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
		throw boost::system::system_error(e);

	OS_ZEROMEMORY(&m_requestPacket, sizeof(Packet));
	m_requestPacket.liVnMode = 27; //Encoded representation which represents NTP Client Request & NTP version 3.0 (i.e. LI=0, VN=3, Mode=3)
	m_requestPacket.transmit_time = NTPTime::GetCurrentTime();

	m_socket->async_send_to(boost::asio::buffer(&m_requestPacket, sizeof(Packet)), *endpoint, boost::bind(&NTPClient::requestCallback, get_this_ptr(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, scope->extendTimeout()));
}

void NTPClient::requestCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
		throw boost::system::system_error(e);

	if(transferredBytes != sizeof(Packet))
		OS_EXCEPTION("request error");

	OS_ZEROMEMORY(&m_responsePacket, sizeof(FullPacket));

    boost::asio::ip::udp::endpoint localEndpoint;
	m_socket->async_receive_from(boost::asio::buffer(&m_responsePacket, sizeof(FullPacket)), localEndpoint, boost::bind(&NTPClient::responseCallback, get_this_ptr(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, NTPTime::GetCurrentTime(), scope->extendTimeout()));
}

void NTPClient::responseCallback(const boost::system::error_code &e, size_t transferredBytes, const NTPTime &destinationTime, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
		throw boost::system::system_error(e);

	if(transferredBytes < sizeof(Packet))
		OS_EXCEPTION("response error");

	m_response.reset(OS_NEW_T(NTPResponse), os_delete_t());
	m_response->stratum = m_responsePacket.basic.stratum;
	m_response->leap_indicator = (m_responsePacket.basic.liVnMode & 0xC0) >> 6;
	m_response->originate_time = m_responsePacket.basic.originate_time;
	m_response->receive_time = m_responsePacket.basic.receive_time;
	m_response->transmit_time = m_responsePacket.basic.transmit_time;
	m_response->roundtrip_delay = (destinationTime - m_response->originate_time) - (m_response->transmit_time - m_response->receive_time);
	m_response->localclock_offset = ((m_response->receive_time - m_response->originate_time) + (m_response->transmit_time - destinationTime)) / 2;
}

void NTPClient::onTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{
	OS_LOG_ERROR(_S("Ntp timeout"));
	m_socket->close();
}

void NTPClient::onFinalize()
{
	m_socket->close();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
