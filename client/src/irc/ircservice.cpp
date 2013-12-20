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
#include "ircservice.h"

#include "irc.h"
#include "ircconnection.h"
#include "ircsession.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IRCService::IRCService(boost::optional<uint32> workerThreadsCount) : ServiceBase(workerThreadsCount),
																	 m_sessionsID(0)
{

}

IRCService::~IRCService()
{

}

shared_ptr<IRCSession> IRCService::addSession()
{
	shared_ptr<ConnectionsManager> connectionsManager = getConnectionsManager();
	if(connectionsManager == nullptr)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	OS_LOCK(m_cs);

	shared_ptr<IRCSession> session(OS_NEW IRCSession(m_sessionsID, connectionsManager));
	m_sessions[m_sessionsID];
	m_sessionsID++;

	return session;
}

bool IRCService::removeSession(uint32 id)
{
	OS_LOCK(m_cs);
	
	Sessions::iterator i = m_sessions.find(id);
	if(i == m_sessions.end())
		return false;

	m_sessions.erase(i);
	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
