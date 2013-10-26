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

#ifndef _OS_ENGINE_ISISCONNECTION_H
#define _OS_ENGINE_ISISCONNECTION_H

#include "asynchttpclient.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ConnectionStatusGuard;
class IsisConnectionStatus;
class IsisEndpoint;
class Portal;
class RealtimeStatsScopeTimer;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport IsisConnection : public AsyncHttpClient
{
	typedef AsyncHttpClient ConnectionBase;

// Enumerated types
public:
	enum ErrorCode
	{
		ieSuccess			= 0,	// Ok
		ieFatal				= 1,	// Fatal Error, Isis must be stopped permanently.
		ieError				= 2,	// Normal Error, Retry later.
		ieRetry				= 3,	// 'Lite' Error, Retry as soon as possible (for example, a timeout error)
		ieSystemUpdate		= 4,	// System options must be updated
	};

// Construction
public:
	IsisConnection(shared_ptr<Portal> portal, shared_ptr<IsisEndpoint> endpoint, shared_ptr<ConnectionsManager> connectionsManager, shared_ptr<TCPSocket> socket, const HttpUrl &url);
	virtual ~IsisConnection();

// Attributes
public:
	inline shared_ptr<IsisEndpoint> getEndpoint() const;

	shared_ptr<Portal> getPortal() const;

// Operations
public:
	shared_ptr<IsisConnectionStatus> takeStatusSnapshot() const;

private:
	shared_ptr<ConnectionStatusGuard> lockStatus();

// AsyncHttpClient overrides
public:
	virtual void run();

protected:
	virtual boost::posix_time::time_duration getTimeout() const;

	virtual void onResponseCompleted(shared_ptr<ConnectionScope> scope);
	virtual void onResponseError(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);
	virtual void onTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);

	shared_ptr<HttpRequest> initializeRequest(const String& url);
	void sendTheRequest(shared_ptr<ConnectionScope> scope);	
	void step(shared_ptr<ConnectionScope> scope, shared_ptr<XMLNode> data);	
	void log(const String& desc);
	void realtimeStatsSet(const uint32& status);
	void error(shared_ptr<ConnectionScope> scope, const String& desc, bool fatal, bool retry = false);

private:	
    weak_ptr<IsisEndpoint> m_endpoint;
	
	String m_systemLastModification; // Queste andrebbero in IsisEndPoint?
	String m_systemUser;
	String m_systemRedirect;

	bool m_firstrequest; // VERYURGENT
	String m_realtimeStatsCounterName;
	shared_ptr<IsisConnectionStatus> m_status;
	weak_ptr<ConnectionStatusGuard> m_statusGuard;
	mutable boost::recursive_mutex m_statusCS;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IsisEndpoint> IsisConnection::getEndpoint() const { return m_endpoint.lock(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_ISISCONNECTION_H
