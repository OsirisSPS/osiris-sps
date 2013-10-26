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
#include "p2ppacketserror.h"

#include "datatree.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

const String Error::ERROR_CODE = _S("error");		// ERROR  definito come macro...

//////////////////////////////////////////////////////////////////////

Error::Error(PacketError error) : PacketBase(ptError)
{
	if(error != peUnknown)
		setError(error);
}

Error::~Error()
{

}

PacketError Error::getError() const
{
	uint32 error = static_cast<uint32>(getData()->getV(ERROR_CODE, static_cast<uint32>(OS_PACKET_ERROR_UNKNOWN)));
	switch(error)
	{
	case peUnknown:
	case peInvalidPortal:
									return static_cast<PacketError>(error);

	default:						OS_ASSERTFALSE();
									break;
	}

	return peUnknown;
}

void Error::setError(PacketError error)
{
	getData()->setV(ERROR_CODE, static_cast<uint32>(error));
}

bool Error::create()
{
	return true;
}

bool Error::parse()
{
	return true;
}

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
