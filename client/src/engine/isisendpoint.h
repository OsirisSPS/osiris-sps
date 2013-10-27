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

#ifndef _OS_ENGINE_ISISENDPOINT_H
#define _OS_ENGINE_ISISENDPOINT_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "enable_this_ptr.h"
#include "httpurl.h"
#include "portalsportals.h"
#include "xmloptions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ConnectionsManager;
class IsisConnection;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport IsisEndpoint : public Object,
                                  public enable_this_ptr<IsisEndpoint>
{
// Construction
public:
	IsisEndpoint(const PortalID &portalID, const PovID &povID);
	virtual ~IsisEndpoint();

// Attributes
public:
    inline shared_ptr<IsisConnection> getActiveConnection() const;

	String getName() const;
	void setName(const String &name);

	HttpUrl getUrl() const;
	void setUrl(const HttpUrl &url);

	bool getEnabled() const;
	void setEnabled(bool enabled);

	String getPassword() const;
	void setPassword(const String &password);

	//ObjectID getUser() const;
	//void setUser(const ObjectID &userID);

	String getLastEvent() const;
	void setLastEvent(const String &lastEvent);

	bool isValid() const;

	inline const XMLOptions & getIsisOptions() const;

// Operations
public:
	bool matchUrl(const HttpUrl &url);
	void update(shared_ptr<ConnectionsManager> connectionsManager);
	void close();

	void parseSystemInformation(shared_ptr<XMLNode> root);
	//String getSnapshotProfileID();
	bool canStart();

private:
	mutable boost::recursive_mutex m_cs;
	String m_name;
	HttpUrl m_url;
	bool m_enabled;
	String m_password;	
	String m_lastEvent;
	PortalID m_portalID;
	PovID m_povID;
	weak_ptr<IsisConnection> m_activeConnection;
	XMLOptions m_isisOptions;	
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IsisConnection> IsisEndpoint::getActiveConnection() const { return m_activeConnection.lock(); }

inline const XMLOptions & IsisEndpoint::getIsisOptions() const { return m_isisOptions; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_ISISENDPOINT_H
