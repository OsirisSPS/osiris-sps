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

#ifndef _OS_IRC_IRCSERVICE_H
#define _OS_IRC_IRCSERVICE_H

#include "boost/thread/recursive_mutex.hpp"
#include "networkservice.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCConnection;
class IRCSession;

//////////////////////////////////////////////////////////////////////

class IRCExport IRCService : public NetworkService
{	
	typedef NetworkService ServiceBase;

	typedef unordered_map<uint32, shared_ptr<IRCSession> >::type Sessions; 

// Construction
public:
	IRCService(boost::optional<uint32> workerThreadsCount = boost::none);
	virtual ~IRCService();

// Operations
public:
	shared_ptr<IRCSession> addSession();
	bool removeSession(uint32 id);
	
private:
	weak_ptr<IRCConnection> m_connection;
	mutable boost::recursive_mutex m_cs;
	uint32 m_sessionsID;
	Sessions m_sessions;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCSERVICE_H
