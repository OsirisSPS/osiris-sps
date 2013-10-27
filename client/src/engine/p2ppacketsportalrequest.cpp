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
#include "p2ppacketsportalrequest.h"

#include "datatree.h"
//#include "portalsportalid.h"
//#include "ids.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

const String PortalRequest::PORTALPOV = _S("portal");

//////////////////////////////////////////////////////////////////////

PortalRequest::PortalRequest() : PacketBase(ptPortalRequest)
{

}

PortalRequest::~PortalRequest()
{

}

std::string PortalRequest::getPortalPov() const
{
	return static_cast<String>(getData()->getV(PORTALPOV)).to_ascii();
}

void PortalRequest::setPortalPov(const std::string &id)
{
	getData()->setV(PORTALPOV, String(id));
}

bool PortalRequest::create()
{
	return true;
}

bool PortalRequest::parse()
{
	return getPortalPov().empty() == false;
}

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
