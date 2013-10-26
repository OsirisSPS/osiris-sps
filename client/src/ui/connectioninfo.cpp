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
#include "connectioninfo.h"

#include "lock.h"
#include "uistandards.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ConnectionInfo::ConnectionInfo(uint32 connectionID, const DateTime &timestamp, const String &origin) : m_connectionID(connectionID),
																									   m_timestamp(timestamp),
																									   m_lastAction(timestamp),
																									   m_origin(origin),
																									   m_initialized(false),
																									   m_removed(false),
																									   m_modified(true)
{		
	
}

ConnectionInfo::~ConnectionInfo()
{

}

uint32 ConnectionInfo::getConnectionID() const
{
	OS_LOCK(m_cs);
	return m_connectionID;
}

DateTime ConnectionInfo::getTimestamp() const
{
	OS_LOCK(m_cs);
	return m_timestamp;
}

String ConnectionInfo::getOrigin() const
{
	OS_LOCK(m_cs);
	return m_origin;
}

bool ConnectionInfo::isElapsed() const
{
	OS_LOCK(m_cs);
	return (DateTime::now() - m_lastAction) >= TimeDuration::millisec(OS_UI_CONNECTIONS_TTL_MIN);	
}

bool ConnectionInfo::getInitialized() const
{
	OS_LOCK(m_cs);
	return m_initialized;
}

void ConnectionInfo::setInitialized(bool initialized)
{
	OS_LOCK(m_cs);
	m_initialized = initialized;
	m_modified = true;
}

bool ConnectionInfo::getRemoved() const
{
	OS_LOCK(m_cs);
	return m_removed;
}

void ConnectionInfo::setRemoved(bool removed)
{
	OS_LOCK(m_cs);
	m_removed = removed;
	m_modified = true;
}

DateTime ConnectionInfo::getLastAction() const
{
	OS_LOCK(m_cs);
	return m_lastAction;
}

void ConnectionInfo::setLastAction(const DateTime &lastAction)
{
	OS_LOCK(m_cs);
	m_lastAction = lastAction;
	m_modified = true;
}

shared_ptr<ConnectionStatus> ConnectionInfo::getConnectionStatus() const
{
	OS_LOCK(m_cs);
	return m_connectionStatus;
}

void ConnectionInfo::setConnectionStatus(shared_ptr<ConnectionStatus> connectionStatus)
{
	OS_LOCK(m_cs);
	m_connectionStatus = connectionStatus;
	m_modified = true;
}

bool ConnectionInfo::getModified() const
{
	OS_LOCK(m_cs);
	return m_modified;
}

void ConnectionInfo::resetModifiedFlag()
{
	OS_LOCK(m_cs);
	m_modified = false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
