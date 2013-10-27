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

#ifndef _OS_UI_CONNECTIONINFO_H
#define _OS_UI_CONNECTIONINFO_H

#include "datetime.h"
#include "connectionstatus.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ConnectionInfo : public Object
{
// Construction
public:
	ConnectionInfo(uint32 connectionID, const DateTime &timestamp, const String &origin = String::EMPTY);
	virtual ~ConnectionInfo();

// Attributes
public:
	uint32 getConnectionID() const;
	DateTime getTimestamp() const;
	String getOrigin() const;

	bool isElapsed() const;

	bool getInitialized() const;
	void setInitialized(bool initialized);

	bool getRemoved() const;
	void setRemoved(bool removed);	

	DateTime getLastAction() const;
	void setLastAction(const DateTime &lastAction);

	shared_ptr<ConnectionStatus> getConnectionStatus() const;
	void setConnectionStatus(shared_ptr<ConnectionStatus> connectionStatus);

	bool getModified() const;
	
// Operations
public:
	void resetModifiedFlag();

private:
	mutable boost::recursive_mutex m_cs;
	uint32 m_connectionID;		
	DateTime m_timestamp;	
	DateTime m_lastAction;
	String m_origin;
	bool m_initialized;
	bool m_removed;
	shared_ptr<ConnectionStatus> m_connectionStatus;
	bool m_modified;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_CONNECTIONINFO_H
