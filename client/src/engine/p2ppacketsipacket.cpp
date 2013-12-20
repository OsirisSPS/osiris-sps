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
#include "p2ppacketsipacket.h"

#include "boost/static_assert.hpp"
#include "convert.h"
#include "buffer.h"
#include "datatree.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

BOOST_STATIC_ASSERT(sizeof(IPacket::HEADER) == 6);		// uint16 + uint32

//////////////////////////////////////////////////////////////////////

IPacket::HEADER::HEADER() 
{ 
	OS_ZEROMEMORY(this, sizeof(HEADER)); 
}

//////////////////////////////////////////////////////////////////////

IPacket::IPacket(PacketType type)
{
	m_data = nullptr;
	m_type = type;
	m_index = 0;
}

IPacket::~IPacket()
{
	delete m_data;
	m_data = nullptr;	
}

DataTree * IPacket::getData() const
{
	if(m_data == nullptr)
		m_data = OS_NEW DataTree();

	return m_data;
}

bool IPacket::read(const Buffer &buffer)
{
	if(getData()->read(buffer) == false)
		return false;

	return parse();
}

bool IPacket::write(Buffer &buffer)
{
	// N.B.: il pacchetto potrebbe non avere body, non chiamare getData()
	if(m_data == nullptr)
		return true;

	return m_data->write(buffer);
}

bool IPacket::validate(packet_ptr packet)
{
	return true;
}

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
