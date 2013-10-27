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

#ifndef _OS_P2P_PACKETS_IPACKET_H
#define _OS_P2P_PACKETS_IPACKET_H

#include "base/object.h"
#include "p2pp2p.h"
#include "p2ppacketspackets.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class DataTree;

//////////////////////////////////////////////////////////////////////

namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

class EngineExport IPacket : public Object
{
public:
	#pragma pack(push, 1)
	struct HEADER
	{
		HEADER();
		uint16 type;		// Tipologia di pacchetto
		uint32 size;		// Dimensione del pacchetto
	};
	#pragma pack(pop)

// Construction
protected:
	IPacket(PacketType type);
	virtual ~IPacket();

// Attributes
public:
	inline PacketType getType() const;

	DataTree * getData() const;

// Operations
public:
	virtual bool read(const Buffer &buffer);
	virtual bool write(Buffer &buffer);

	virtual bool validate(packet_ptr packet);

// Interface
public:
	virtual bool create() = 0;

protected:
	virtual bool parse() = 0;

protected:
	mutable DataTree *m_data;
	PacketType m_type;
	uint32 m_index;
};

//////////////////////////////////////////////////////////////////////

inline PacketType IPacket::getType() const { return m_type; }

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_PACKETS_IPACKET_H
