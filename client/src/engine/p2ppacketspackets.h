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

#ifndef _OS_P2P_PACKETS_PACKETS_H
#define _OS_P2P_PACKETS_PACKETS_H

//////////////////////////////////////////////////////////////////////

#define OS_PACKET_UNKNOWN						0
#define OS_PACKET_ERROR							1
#define OS_PACKET_BUSY							2
#define OS_PACKET_HELLO							3
#define OS_PACKET_TERMINATE						4
#define OS_PACKET_AUTHENTICATIONREQUEST			5
#define OS_PACKET_AUTHENTICATIONRESPONSE		6
#define OS_PACKET_PORTALREQUEST					7
#define OS_PACKET_PORTALRESPONSE				8
#define OS_PACKET_QUERYREQUEST					9
#define OS_PACKET_QUERYRESPONSE					10
#define OS_PACKET_OBJECTSREQUEST				11
#define OS_PACKET_OBJECTSRESPONSE				12

#define OS_PACKET_ERROR_UNKNOWN					0
#define OS_PACKET_ERROR_INVALIDPORTAL			1

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

enum PacketType
{
	ptUnknown									= OS_PACKET_UNKNOWN,
	ptError										= OS_PACKET_ERROR,
	ptBusy										= OS_PACKET_BUSY,
	ptHello										= OS_PACKET_HELLO,
	ptTerminate									= OS_PACKET_TERMINATE,
	ptAuthenticationRequest						= OS_PACKET_AUTHENTICATIONREQUEST,
	ptAuthenticationResponse					= OS_PACKET_AUTHENTICATIONRESPONSE,
	ptPortalRequest								= OS_PACKET_PORTALREQUEST,
	ptPortalResponse							= OS_PACKET_PORTALRESPONSE,
	ptQueryRequest								= OS_PACKET_QUERYREQUEST,
	ptQueryResponse								= OS_PACKET_QUERYRESPONSE,
	ptObjectsRequest							= OS_PACKET_OBJECTSREQUEST,
	ptObjectsResponse							= OS_PACKET_OBJECTSRESPONSE,
};

enum PacketError
{
	peUnknown									= OS_PACKET_ERROR_UNKNOWN,
	peInvalidPortal								= OS_PACKET_ERROR_INVALIDPORTAL,	
};

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_PACKETS_PACKETS_H
