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
#include "ihttpserver.h"

#include "boost/bind.hpp"
#include "httppath.h"
#include "httprootdirectory.h"
#include "httpphysicaldirectory.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpsession.h"
#include "httpsessionstate.h"
#include "limitedconnection.h"
#include "lock.h"
#include "log.h"
#include "networksystem.h"
#include "platformmanager.h"
#include "tcpsocket.h"

//////////////////////////////////////////////////////////////////////

#define OS_HTTP_SERVER_OPTIMAL_CACHE_SIZE		64	// OS_HTTP_DEFAULT_BUFFER_SIZE * 64 = 512kb

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IHttpServer::IHttpServer(const std::string &name, bool allowRemoteConnections, uint32 sessionDuration, boost::optional<uint32> workerThreadsCount) : ServerBase(workerThreadsCount),
																																					 m_name(name),
																																					 m_allowRemoteConnections(allowRemoteConnections),
																																					 m_root(OS_NEW HttpRootDirectory()),
																																					 m_sessionsDuration(TimeDuration::minutes(sessionDuration)),
																																					 m_buffersCache(OS_HTTP_SERVER_OPTIMAL_CACHE_SIZE)
{	

}

IHttpServer::~IHttpServer()
{

}

bool IHttpServer::getAllowRemoteConnections() const
{
	OS_LOCK(m_optionsCS);
	return m_allowRemoteConnections;
}

void IHttpServer::setAllowRemoteConnections(bool allow)
{
	OS_LOCK(m_optionsCS);
	m_allowRemoteConnections = allow;
}

String IHttpServer::getDefaultDirectory() const
{
	OS_LOCK(m_optionsCS);
	return m_defaultDirectory;
}

void IHttpServer::setDefaultDirectory(const String &directory)
{
	OS_LOCK(m_optionsCS);
	m_defaultDirectory = directory;
}

const TimeDuration & IHttpServer::getSessionsDuration() const
{
	OS_LOCK(m_optionsCS);
	return m_sessionsDuration;
}

void IHttpServer::setSessionsDuration(const TimeDuration &duration)
{
	OS_LOCK(m_optionsCS);
	m_sessionsDuration = duration;
}

shared_ptr<IHttpDirectory> IHttpServer::getDirectory(const String &name) const
{
	return m_root->getDirectory(name);
}

bool IHttpServer::addDirectory(shared_ptr<IHttpDirectory> directory)
{
	return m_root->addDirectory(directory);
}

bool IHttpServer::removeDirectory(shared_ptr<IHttpDirectory> directory)
{
	return m_root->removeDirectory(directory);
}

shared_ptr<HttpPhysicalDirectory> IHttpServer::addResourceLocation(const String &name, const String &path)
{
	shared_ptr<HttpPhysicalDirectory> directory(OS_NEW HttpPhysicalDirectory(name, path));
	if(addDirectory(directory) == false)
		return null;

	return directory;
}

shared_ptr<HttpBuffer> IHttpServer::peekHttpBuffer()
{
	OS_LOCK(m_buffersCacheCS);
	return m_buffersCache.peekEntry();
}

bool IHttpServer::onAccept(shared_ptr<IConnection> connection)
{
	if(ServerBase::onAccept(connection) == false)
		return false;

	boost::asio::ip::address remoteAddress = connection->getSocket().remote_endpoint().address();
	if(NetworkSystem::instance()->isLocalAddress(remoteAddress.to_string().c_str()))
		return true;	// Le connessioni locali sono sempre accettate

	return m_allowRemoteConnections;
}

bool IHttpServer::processRequest(shared_ptr<HttpSession> session, const HttpPath &path)
{	
	// This to lock requests when some system-level changes occurs (for example, installing extensions).
	boost::shared_lock<boost::shared_mutex> lock(getRequestMutex());
	
	if(path.isSafe() == false)
		return false;

	String directory_name = path.getDirectory();
	if(directory_name.empty())
		directory_name = getDefaultDirectory();

	shared_ptr<IHttpDirectory> directory = getDirectory(directory_name);
	return directory != null ? directory->process(session, path.getTarget()) : false;
}

shared_ptr<HttpSessionState> IHttpServer::getSessionState(shared_ptr<HttpSession> session, const HttpPath &path)
{
	OS_LOCK(m_sessionsCS);

	// Cerca la directory richiesta
	shared_ptr<IHttpDirectory> directory = getDirectory(path.getDirectory());
	if(directory == null)
		return null;

	String session_cookie = directory->getSessionCookie(session);
	String session_id;
	shared_ptr<HttpSessionState> session_state;

	// Verifica che l'id della sessione sia valido
	if(session->getRequest()->hasCookie(session_cookie.to_ascii()))
	{		
		// Carica l'id della sessione
		session_id = session->getRequest()->getCookie(session_cookie.to_ascii());
		if(session_id.empty())
			return null;

		// Cerca la sessione richiesta
		SessionsStates::iterator i = m_sessionsStates.find(session_id);
		if(i != m_sessionsStates.end())
		{
			// Verifica che la sessione non sia scaduta
			if(i->second->isElapsed())
				_sessionElapsed(i->second);
			else
				session_state = i->second;
		}
	}

	if(session_state == null)
	{
		OS_TRACE("\r\nNew session..."); // COOKIEURGENT
		session_state = directory->createSessionState(session, getSessionsDuration());
		if(session_state == null)
			return null;

		session_id = session_state->getID();

		OS_EXCEPT_IF(session_id.empty(), "Invalid session id");
		OS_EXCEPT_IF(m_sessionsStates.find(session_id) != m_sessionsStates.end(), "Duplicated session id");

		// Salva la sessione
		_sessionStarted(session_state);

		// Salva il cookie della sessione
		session->getResponse()->setCookie(session_cookie.to_ascii(), session_id.to_ascii());
	}

	// Valida la sessione
	session_state->validate();

	return session_state;
}

void IHttpServer::_sessionStarted(shared_ptr<HttpSessionState> session)
{
	m_sessionsStates[session->getID()] = session;
}

void IHttpServer::_sessionElapsed(shared_ptr<HttpSessionState> session)
{
	m_sessionsStates.erase(session->getID());
}

void IHttpServer::_validateActiveSessions()
{
	OS_LOCK(m_sessionsCS);

	for(SessionsStates::iterator i = m_sessionsStates.begin(); i != m_sessionsStates.end();)
	{
		SessionsStates::iterator tmp = i;
		++i;

		if(tmp->second->isElapsed())
		{
			_sessionElapsed(tmp->second);
		}
	}
}

bool IHttpServer::onIdle()
{
	_validateActiveSessions();

	return ServerBase::onIdle();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
