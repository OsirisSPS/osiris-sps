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
#include "dhtportalsearch.h"

#include "ipaddress.h"
#include "log.h"
#include "portalspeersmanager.h"
#include "portalsportal.h"

//////////////////////////////////////////////////////////////////////

#define OS_PEER_ORIGIN_DHT		"dht"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DHTPortalSearch::DHTPortalSearch(shared_ptr<Portal> portal, uint32 announcePort) : SearchBase(announcePort),
																				   m_portal(portal)
{
	OS_ASSERT(portal != nullptr);
}

DHTPortalSearch::~DHTPortalSearch()
{

}

void DHTPortalSearch::processResult(uint32 ip, uint32 port)
{
	shared_ptr<Portal> portal = getPortal();
	if(portal == nullptr)
		return;

	shared_ptr<IPAddress> address(OS_NEW IPAddress(ip, port));

	OS_LOG_DEBUG(String::format(_S("Found DHT peer '%S:%d' for portal %S").c_str(), address->toString().c_str(), port, portal->getName().c_str())); 

	portal->getPeersManager()->savePeer(address, false, OS_PEER_ORIGIN_DHT);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
