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
#include "p2ppacketshello.h"

#include "buffer.h"
#include "datatree.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

const String Hello::ID = _S("id");
const String Hello::SIGNATURE = _S("signature");
const String Hello::PORT = _S("port");

//////////////////////////////////////////////////////////////////////

Hello::Hello() : PacketBase(ptHello)
{

}

Hello::~Hello()
{

}

Buffer Hello::getMachineID() const
{
	return getData()->getV(ID);
}

void Hello::setMachineID(const Buffer &id)
{
	getData()->setV(ID, id);
}

Buffer Hello::getSignature() const
{
	return getData()->getV(SIGNATURE);
}

void Hello::setSignature(const Buffer &signature)
{
	getData()->setV(SIGNATURE, signature);
}

uint32 Hello::getServerPort() const
{
	return getData()->getV(PORT, static_cast<uint32>(OS_P2P_OLD_DEFAULT_SERVER_PORT));
}

void Hello::setServerPort(uint32 port)
{
	getData()->setV(PORT, port);
}

bool Hello::create()
{
	return true;
}

bool Hello::parse()
{
	return true;
}

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
