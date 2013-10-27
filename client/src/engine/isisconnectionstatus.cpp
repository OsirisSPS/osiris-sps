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
#include "isisconnectionstatus.h"

#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IsisConnectionStatus::IsisConnectionStatus(shared_ptr<Portal> portal, const String &name) : m_portal(portal),
                                                                                                     m_name(name),
																									 m_requests(0),
                                                                                                     m_status(isNone)
{

}

IsisConnectionStatus::~IsisConnectionStatus()
{

}

shared_ptr<Portal> IsisConnectionStatus::getPortal() const
{
	OS_LOCK(m_cs);
	return m_portal.lock();
}

String IsisConnectionStatus::getName() const
{
	OS_LOCK(m_cs);
	return m_name;
}

IsisConnectionStatus::Status IsisConnectionStatus::getStatus() const
{
	OS_LOCK(m_cs);
	return m_status;
}

void IsisConnectionStatus::setStatus(Status status)
{
	OS_LOCK(m_cs);

	if(m_status != status)
	{
		m_status = status;
		m_modifiedFlag = true;
	}
}

uint32 IsisConnectionStatus::getRequests() const
{
	OS_LOCK(m_cs);
	return m_requests;
}

String IsisConnectionStatus::getLastEvent() const
{
	OS_LOCK(m_cs);
	return m_lastEvent;
}

void IsisConnectionStatus::setLastEvent(const String &lastEvent)
{
	OS_LOCK(m_cs);

	m_lastEvent = lastEvent;
	m_modifiedFlag = true;
}

uint32 IsisConnectionStatus::addRequest()
{
	OS_LOCK(m_cs);
	return ++m_requests;
}

shared_ptr<IsisConnectionStatus> IsisConnectionStatus::clone() const
{
	OS_LOCK(m_cs);

	shared_ptr<IsisConnectionStatus> connectionStatus(OS_NEW IsisConnectionStatus(m_portal.lock(), m_name));
	connectionStatus->m_status = m_status;
	connectionStatus->m_requests = m_requests;
	connectionStatus->m_lastEvent = m_lastEvent;

	return connectionStatus;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
