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

#ifndef _OS_ENGINE_P2PCONNECTION_H
#define _OS_ENGINE_P2PCONNECTION_H

#include "boost/array.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "constants.h"
#include "datadata.h"
#include "dbdb.h"
#include "iconnection.h"
#include "buffer.h"
#include "log.h"
#include "p2pnodesession.h"
#include "objectsobjects.h"
#include "p2pp2p.h"
#include "p2pnodestatus.h"
#include "p2ppacketspackets.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ConnectionStatusGuard;
class IPortalDatabase;
class P2PServer;
class PortalID;
class PortalOptions;

//////////////////////////////////////////////////////////////////////

class EngineExport P2PConnection : public IConnection
{
    typedef IConnection ConnectionBase;

	class P2PScope;

// Construction
public:
	P2PConnection(shared_ptr<P2PServer> server, shared_ptr<TCPSocket> socket, uint32 timeout, bool outgoing, const String &origin);
	virtual ~P2PConnection();

// Attributes
public:
	inline shared_ptr<P2PServer> getServer() const;
	inline const String & getOrigin() const;

	String getPeerHostname() const;

	shared_ptr<Portal> getPortal() const;
	shared_ptr<PortalOptions> getPortalOptions() const;

	String getPortalName() const;
	std::string getPortalPovID() const;
	bool setPortal(shared_ptr<Portal> portal);
	bool setPortalPov(const std::string &id);

private:
	bool getRun() const;
	void setRun(bool run);

protected:
	bool isAllowed();

// Operations
public:
	void changeStatus(p2p::NodeStatus::Status status);
	bool compareSessionKey(const Buffer &public_key) const;
	shared_ptr<p2p::NodeStatus> takeStatusSnapshot() const;

protected:
	void openConnection(shared_ptr<IPAddress> address, shared_ptr<ConnectionScope> scope);
	void acceptConnection(shared_ptr<ConnectionScope> scope);
	void finalizeConnection(bool successful);

	bool handlePacket(p2p::packets::packet_ptr &local, p2p::packets::packet_ptr &remote, shared_ptr<P2PScope> scope);
	bool validatePacket(p2p::packets::packet_ptr &local, p2p::packets::packet_ptr &remote);
	bool validateType(p2p::packets::PacketType type);

	p2p::packets::packet_ptr allocPacket(p2p::packets::PacketType type);

	p2p::packets::packet_ptr createHelloPacket();

	virtual bool onHello(p2p::packets::hello_ptr hello, p2p::packets::packet_ptr &response, shared_ptr<P2PScope> scope);
	virtual bool onTerminate(p2p::packets::terminate_ptr terminate, p2p::packets::packet_ptr &response, shared_ptr<P2PScope> scope);
	virtual bool onAuthenticationRequest(p2p::packets::authentication_request_ptr authentication_request, p2p::packets::packet_ptr &response);
	virtual bool onAuthenticationResponse(p2p::packets::authentication_response_ptr authentication_response, p2p::packets::packet_ptr &response);
	virtual bool onPortalRequest(p2p::packets::portal_request_ptr portal_request, p2p::packets::packet_ptr &response);
	virtual bool onPortalResponse(p2p::packets::portal_response_ptr portal_response, p2p::packets::packet_ptr &response);
	virtual bool onQueryRequest(p2p::packets::query_request_ptr request, p2p::packets::packet_ptr &response);
	virtual bool onQueryResponse(p2p::packets::query_response_ptr response, p2p::packets::packet_ptr &request);
	virtual bool onObjectsRequest(p2p::packets::objects_request_ptr request, p2p::packets::packet_ptr &response);
	virtual bool onObjectsResponse(p2p::packets::objects_response_ptr response, p2p::packets::packet_ptr &request);
	virtual bool onBusy(p2p::packets::busy_ptr response, p2p::packets::packet_ptr &request, shared_ptr<P2PScope> scope);
	virtual bool onError(p2p::packets::error_ptr response, p2p::packets::packet_ptr &request);

	virtual bool onAuthenticationCompleted(p2p::packets::packet_ptr &request);

	void processObject(shared_ptr<IPortalDatabase> database, shared_ptr<ObjectsIObject> object);

	bool createHelloResponse(p2p::packets::packet_ptr &packet);

	void savePeer(shared_ptr<IPAddress> address, bool connectedSuccessfully, const String &origin);
	void validatePeer(shared_ptr<IPAddress> address);
	void invalidatePeer(shared_ptr<IPAddress> address);
	void removePeer(shared_ptr<IPAddress> address);

	void updateLastEvent(const String &description, LogLevel level);

private:
	static void generateRandomSeed(byte &size, Buffer &data);
	void _clear();

	void _applyPortalKeyDerivation(p2p::LocalSession::DerivationType type);

	void peekPacket(p2p::packets::packet_ptr &packet);

	bool loadRequest(p2p::packets::query_request_ptr request);
	void saveRequest(p2p::packets::query_request_ptr request);

	shared_ptr<IPAddress> getPeer() const;
	void setPeer(shared_ptr<IPAddress> peer);

	shared_ptr<ConnectionStatusGuard> lockStatus();

	void connectionCallback(const boost::system::error_code &e, shared_ptr<P2PScope> scope);
	void handshakeRequestCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> transferredData, shared_ptr<P2PScope> scope);
	
	void readResponseCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> transferredData, shared_ptr<P2PScope> scope);
	void handshakeResponseCallback(const Buffer &remotePublicKey, shared_ptr<P2PScope> scope);	
	bool handleHandshakeResponse(const Buffer &remotePublicKey);

	void readHandshakeRequest(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> request, shared_ptr<P2PScope> scope);
	void handleHandshakeRequest(shared_ptr<Buffer> request, shared_ptr<P2PScope> scope);
	void handshakeResponseSent(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> response, shared_ptr<P2PScope> scope);

	void startExchange(shared_ptr<P2PScope> scope, bool starter);

	bool asyncSendPacket(p2p::packets::packet_ptr packet, shared_ptr<P2PScope> scope);
	void handlePacketSended(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> packetData, shared_ptr<P2PScope> scope);
	void onPacketSended(shared_ptr<P2PScope> scope);

	void asyncReceivePacket(shared_ptr<P2PScope> scope);
	void handlePacketHeader(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> encryptedPacketHeader, shared_ptr<P2PScope> scope);
	void handlePacketBody(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> encryptedPacketBody, shared_ptr<P2PScope> scope);
	void onPacketReceived(shared_ptr<P2PScope> scope);

	void updateDownload(size_t size);
	void updateUpload(size_t size);

	void handleLoopback();

	bool getLoopback() const;

// IConnection overrides
public:
	virtual shared_ptr<ConnectionScope> createScope(const boost::posix_time::time_duration &timeout = boost::posix_time::milliseconds(0));

// IConnection interface
public:
	virtual TCPSocket & getSocket();

protected:
	weak_ptr<P2PServer> m_server;

	bool m_run;
	bool m_loopback;

	p2p::local_session_ptr m_localSession;			// Dati sulla sessione locale
	p2p::remote_session_ptr m_remoteSession;		// Dati sulla sessione remota

	p2p::packets::packet_ptr m_localPacket;			// ultimo pacchetto locale
	p2p::packets::packet_ptr m_remotePacket;		// ultimo pacchetto remoto

	p2p::exchange_context_ptr m_exchangeContext;
	p2p::exchange_session_ptr m_exchangeSession;

	uint32 m_timeout;
	String m_origin;
	shared_ptr<TCPSocket> m_socket;

	shared_ptr<p2p::NodeStatus> m_status;
	weak_ptr<ConnectionStatusGuard> m_statusGuard;
	mutable boost::recursive_mutex m_statusCS;

	mutable boost::recursive_mutex m_cs;

	boost::array<char, OS_P2P_HANDSHAKE_BUFFER_SIZE> m_handshakeBuffer;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<P2PServer> P2PConnection::getServer() const { return m_server.lock(); }
inline const String & P2PConnection::getOrigin() const { return m_origin; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_P2PCONNECTION_H
