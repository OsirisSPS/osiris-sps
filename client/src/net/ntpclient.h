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

#ifndef _OS_NET_NTPCLIENT_H
#define _OS_NET_NTPCLIENT_H

#include "base/object.h"
#include "boostasio.h"
#include "connectionscope.h"
#include "enable_this_ptr.h"
#include "ntptime.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class UDPSocket;

//////////////////////////////////////////////////////////////////////

struct NTPResponse
{
    int32 leap_indicator;				// 0: no warning
										// 1: last minute in day has 61 seconds
										// 2: last minute has 59 seconds
										// 3: clock not synchronized

	int32 stratum;						// 0: unspecified or unavailable
										// 1: primary reference (e.g., radio clock)
										// 2-15: secondary reference (via NTP or SNTP)
										// 16-255: reserved

    NTPTime originate_time;				// Time when the request was sent from the client to the SNTP server
	NTPTime receive_time;				// Time when the request was received by the server
	NTPTime transmit_time;				// Time when the server sent the request back to the client
	
	double roundtrip_delay;				// Round trip time in seconds
	double localclock_offset;			// Local clock offset relative to the server
};

class NetExport NTPClient : public enable_this_ptr<NTPClient>,
							public ConnectionScope::IListener,
							public Object
{
protected:
	#pragma pack(push, 1)
	struct Packet
	{
		uint8 liVnMode;					// leap indicator, version and mode
		uint8 stratum;					// peer stratum
		uint8 poll;						// peer poll interval
		int8 precision;					// peer clock precision
		uint32 rootDelay;				// distance to primary clock
		uint32 rootDispersion;			// clock dispersion
		uint32 referenceID;				// reference clock ID
		TimeStamp reference_time;
		TimeStamp originate_time;
		TimeStamp receive_time;
		TimeStamp transmit_time;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct Authentication				// Opzionale
	{
		unsigned long id;
		byte digest[16];
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct FullPacket
	{
		Packet basic;
		Authentication authentication;
	};
	#pragma pack(pop)

// Construction
protected:
  NTPClient(shared_ptr<boost::asio::io_service> service, shared_ptr<UDPSocket> socket);
public:
  virtual ~NTPClient();

// Operations
public:
    static shared_ptr<NTPResponse> query(shared_ptr<boost::asio::io_service> service, shared_ptr<UDPSocket> socket, const String &server, uint32 port);

private:
	void query(const boost::asio::ip::udp::resolver::query &query);
	void resolveCallback(const boost::system::error_code &e, boost::asio::ip::udp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope);
	void requestCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<ConnectionScope> scope);
	void responseCallback(const boost::system::error_code &e, size_t transferredBytes, const NTPTime &destinationTime, shared_ptr<ConnectionScope> scope);

// ConnectionScope::IListener interface
protected:
	virtual void onTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);
	virtual void onFinalize();

protected:
	uint32 m_timeout;
	shared_ptr<boost::asio::io_service> m_service;
	shared_ptr<NTPResponse> m_response;
	shared_ptr<boost::asio::ip::udp::resolver> m_resolver;
	shared_ptr<UDPSocket> m_socket;
	Packet m_requestPacket;
	FullPacket m_responsePacket;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_NTPCLIENT_H
