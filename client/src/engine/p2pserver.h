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

#ifndef _OS_ENGINE_P2PSERVER_H
#define _OS_ENGINE_P2PSERVER_H

#include "iserver.h"
#include "portalsportals.h"
#include "upnpserver.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class P2PConnection;
class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport P2PServer : public UPnPServer<IServer>
{
	typedef UPnPServer<IServer> ServerBase;
	typedef set<shared_ptr<IConnection> >::type ConnectionsSet;

// Construction
public:
	P2PServer(boost::optional<uint32> workerThreadsCount = boost::none);
	virtual ~P2PServer();

// Attributes
public:
	uint32 getMaxIncomingConnections() const;
	void setMaxIncomingConnections(uint32 maxIncomingConnections);

	uint32 getMaxOutgoingConnections() const;
	void setMaxOutgoingConnections(uint32 maxOutgoingConnections);

	uint32 getConnectionsTimeout() const;	
	void setConnectionsTimeout(uint32 connectionsTimeout);	

	uint32 getPendingsOutgoingConnections() const;	

	bool getAllowConnection(shared_ptr<IConnection> connection) const;

private:
	bool isOutgoingConnection(shared_ptr<IConnection> connection) const;

	shared_ptr<Portal> peekNextPortal(uint32 portals);

// Operations
public:
	shared_ptr<P2PConnection> detectLoopbackConnection(const Buffer &public_key) const;
	
private:
	void _startClients();

// IServer interface
protected:
	virtual shared_ptr<IConnection> createConnection();

// IServer overrides
protected:
	virtual bool onIdle();

	virtual void onConnectionAdded(shared_ptr<IConnection> connection);
	virtual void onConnectionRemoved(shared_ptr<IConnection> connection);

private:
	uint32 m_maxIncomingConnections;				// Numero massimo di connessioni in entrata
	uint32 m_maxOutgoingConnections;				// Numero massimo di connessioni in uscita
	uint32 m_outgoingConnections;					// Numero di connessioni in uscita
	uint32 m_connectionsTimeout;					// Timeout delle connessioni
	mutable ConnectionsSet m_allowedConnections;	// Connessioni in entrata consentite
	uint32 m_portal;
	mutable boost::recursive_mutex m_p2pServerCS;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_P2PSERVER_H
