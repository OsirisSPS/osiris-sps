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

#ifndef _OS_HTTP_IHTTPSERVER_H
#define _OS_HTTP_IHTTPSERVER_H

#include "boost/thread/shared_mutex.hpp"
#include "datetime.h"
#include "growablecache.h"
#include "http.h"
#include "iserver.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpPath;
class HttpPhysicalDirectory;
class HttpSession;
class HttpSessionState;
class IHttpDirectory;

//////////////////////////////////////////////////////////////////////

class HttpExport IHttpServer : public IServer
{
	typedef IServer ServerBase;
	typedef unordered_map<String, shared_ptr<HttpSessionState> >::type SessionsStates;

// Construction
public:
	IHttpServer(const std::string &name, bool allowRemoteConnections = false, uint32 sessionDuration = OS_HTTP_DEFAULT_SESSION_DURATION, boost::optional<uint32> workerThreadsCount = boost::none);
	virtual ~IHttpServer();

// Attributes
public:
	inline const std::string & getName() const;

	inline shared_ptr<IHttpDirectory> getRootDirectory() const;
	inline bool hasDirectory(const String &name) const;

	bool getAllowRemoteConnections() const;
	void setAllowRemoteConnections(bool allow);

	String getDefaultDirectory() const;
	void setDefaultDirectory(const String &directory);

	const TimeDuration & getSessionsDuration() const;
	void setSessionsDuration(const TimeDuration &duration);

	shared_ptr<IHttpDirectory> getDirectory(const String &name) const;

	inline boost::shared_mutex & getRequestMutex();

// Operations
public:
	bool addDirectory(shared_ptr<IHttpDirectory> directory);
	bool removeDirectory(shared_ptr<IHttpDirectory> directory);

	shared_ptr<HttpPhysicalDirectory> addResourceLocation(const String &directory, const String &path);

	shared_ptr<HttpBuffer> peekHttpBuffer();

private:
	void _sessionStarted(shared_ptr<HttpSessionState> session);
	void _sessionElapsed(shared_ptr<HttpSessionState> session);
	void _validateActiveSessions();

// Overridables
public:
	virtual bool processRequest(shared_ptr<HttpSession> session, const HttpPath &path);
	virtual shared_ptr<HttpSessionState> getSessionState(shared_ptr<HttpSession> session, const HttpPath &path);

// IServer overrides
protected:
	virtual bool onAccept(shared_ptr<IConnection> connection);
	virtual bool onIdle();

protected:
	std::string m_name;
	bool m_allowRemoteConnections;
	shared_ptr<IHttpDirectory> m_root;
	String m_defaultDirectory;
	SessionsStates m_sessionsStates;
	TimeDuration m_sessionsDuration;
	GrowableCache<HttpBuffer> m_buffersCache;

private:
	mutable boost::recursive_mutex m_optionsCS;
	mutable boost::recursive_mutex m_sessionsCS;	
	mutable boost::recursive_mutex m_buffersCacheCS;	

	mutable boost::shared_mutex m_requestsCS;
};

//////////////////////////////////////////////////////////////////////

inline const std::string & IHttpServer::getName() const { return m_name; }

inline shared_ptr<IHttpDirectory> IHttpServer::getRootDirectory() const { return m_root; }
inline bool IHttpServer::hasDirectory(const String &name) const { return getDirectory(name) != null; }
inline boost::shared_mutex & IHttpServer::getRequestMutex() { return m_requestsCS; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_IHTTPSERVER_H
