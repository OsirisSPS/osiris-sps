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

#ifndef _OS_P2P_PACKETS_OBJECTSREQUEST_H
#define _OS_P2P_PACKETS_OBJECTSREQUEST_H

#include "dbdb.h"
#include "datadata.h"
#include "p2ppacketsipacket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;

//////////////////////////////////////////////////////////////////////

namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsRequest : public IPacket
{
	typedef IPacket PacketBase;

// Construction
public:
	ObjectsRequest(const StringList *objects = nullptr);
	virtual ~ObjectsRequest();

private:
	void construct(IPortalDatabase *pDatabase);

// Attributes
public:
	void getObjects(StringList &objects);

protected:
	inline DataTree * getRoot() const;

// Operations
public:
	void request(const String &id);

// Packet interface
public:
	virtual bool create();

protected:
	virtual bool parse();

private:
	static const String OBJECTS;
};

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_PACKETS_OBJECTSREQUEST_H
