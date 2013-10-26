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
#include "isisservice.h"

#include "lock.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IsisService::IsisService(boost::optional<uint32> workerThreadsCount) : ServiceBase(workerThreadsCount)
{
	
}

IsisService::~IsisService()
{

}

shared_ptr<Portal> IsisService::peekPortal()
{
	OS_LOCK(m_cs);

	if(m_portals.empty())
	{
		m_portals = PortalsSystem::instance()->getPortals();
		if(m_portals.empty())
			return null;
	}

	return utils::pop_front(m_portals);
}

bool IsisService::onIdle()
{
	shared_ptr<Portal> portal = peekPortal();
	if(portal != null && portal->getLoaded())
		portal->getOptions()->updateIsisEndpoints(getConnectionsManager());		

	return ServiceBase::onIdle();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
