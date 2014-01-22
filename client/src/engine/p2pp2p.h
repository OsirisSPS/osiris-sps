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

#ifndef _OS_P2P_P2P_H
#define _OS_P2P_P2P_H

//////////////////////////////////////////////////////////////////////

#define OS_P2P_BOOTSTRAP_REQUEST_DELAY				10		// in minuti
#define OS_P2P_BOOTSTRAP_NOTIFY_DELAY				180		// in minuti

#define OS_P2P_OLD_DEFAULT_SERVER_PORT				6060
#define OS_P2P_NEW_DEFAULT_SERVER_PORT				0		// any port

#define OS_P2P_MAX_INCOMING_CONNECTIONS				5
#define OS_P2P_MAX_OUTGOING_CONNECTIONS				5

#define OS_P2P_MAX_PACKET_SIZE						1024*1024*32

#define OS_P2P_OBJECTS_EXCHANGE_COUNT				10

#define OS_P2P_KEYS_CACHE_SIZE						5000 // VERYURGENT, da ricavare o metter come opzione
#define OS_P2P_KEYS_CACHE_MAX_KEY_USAGE				10

#define OS_P2P_KEY_AGREEMENT_SIZE					128
#define OS_P2P_KEY_AGREEMENT_MODULUS_SIZE			16
#define OS_P2P_KEY_AGREEMENT_GENERATOR_SIZE			1
#define OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE		16
#define OS_P2P_KEY_AGREEMENT_HEADER_SIZE			33		// 16+1+16

#define OS_P2P_HANDSHAKE_BUFFER_SIZE				512

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// Classes
class P2pMachine;

//////////////////////////////////////////////////////////////////////

namespace p2p {

//////////////////////////////////////////////////////////////////////

namespace packets
{
	// Interfaces
	class IPacket;

	// Classes
	class AuthenticationRequest;
	class AuthenticationResponse;
	class Busy;
	class Error;
	class Hello;
	class ObjectsRequest;
	class ObjectsResponse;
	class PortalRequest;
	class PortalResponse;
	class QueryRequest;
	class QueryResponse;
	class Terminate;

	// Types
	typedef shared_ptr<IPacket> packet_ptr;
	typedef shared_ptr<AuthenticationRequest> authentication_request_ptr;
	typedef shared_ptr<AuthenticationResponse> authentication_response_ptr;
	typedef shared_ptr<Busy> busy_ptr;
	typedef shared_ptr<Error> error_ptr;
	typedef shared_ptr<Hello> hello_ptr;
	typedef shared_ptr<ObjectsRequest> objects_request_ptr;
	typedef shared_ptr<ObjectsResponse> objects_response_ptr;
	typedef shared_ptr<PortalRequest> portal_request_ptr;
	typedef shared_ptr<PortalResponse> portal_response_ptr;
	typedef shared_ptr<QueryRequest> query_request_ptr;
	typedef shared_ptr<QueryResponse> query_response_ptr;
	typedef shared_ptr<Terminate> terminate_ptr;
}

// Enumerated types
enum RequestType
{
	rtQuery 		= 1,
	rtObjects 		= 2,
};

// Interfaces
class IMachine;

// Classes
class ExchangeContext;
class ExchangeSession;
class KeysCache;
class LocalSession;
class LocalMachine;
class MachinesCache;
class NodeSession;
class NodeStatus;
class RemoteSession;
class Request;

// Types
//typedef shared_ptr<Machine> machine_ptr;

typedef shared_ptr<ExchangeContext> exchange_context_ptr;
typedef shared_ptr<ExchangeSession> exchange_session_ptr;
typedef shared_ptr<LocalSession> local_session_ptr;
typedef shared_ptr<NodeSession> node_session_ptr;
typedef shared_ptr<RemoteSession> remote_session_ptr;
typedef shared_ptr<Request> request_ptr;

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_P2P_H
