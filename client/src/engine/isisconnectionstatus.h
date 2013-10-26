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

#ifndef _OS_ENGINE_ISISCONNECTIONSTATUS_H
#define _OS_ENGINE_ISISCONNECTIONSTATUS_H

#include "connectionstatus.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport IsisConnectionStatus : public ConnectionStatus
{
// Enumerated types
public:
	enum Status
	{
		isNone				= 0,
		isSendSystem		= 1,
		isReceiveSystem		= 2,
		isSendRequest		= 3,
		isReceiveRequest	= 4,
		isReceiveResponse	= 5,
	};

// Construction
public:
	IsisConnectionStatus(shared_ptr<Portal> portal, const String &name);
	virtual ~IsisConnectionStatus();

// Attributes
public:
	shared_ptr<Portal> getPortal() const;

    String getName() const;

	Status getStatus() const;
	void setStatus(Status status);

	uint32 getRequests() const;

	String getLastEvent() const;
	void setLastEvent(const String &lastEvent);

// Operations
public:
	uint32 addRequest();

	shared_ptr<IsisConnectionStatus> clone() const;

private:
	weak_ptr<Portal> m_portal;
	String m_name;
	Status m_status;
	uint32 m_requests;
	String m_lastEvent;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_ISISCONNECTIONSTATUS_H
