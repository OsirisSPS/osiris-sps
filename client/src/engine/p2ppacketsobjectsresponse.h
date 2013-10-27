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

#ifndef _OS_P2P_PACKETS_OBJECTSRESPONSE_H
#define _OS_P2P_PACKETS_OBJECTSRESPONSE_H

#include "objectsobjects.h"
#include "dbdb.h"
#include "ids.h"
#include "p2ppacketsipacket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class ObjectID;

//////////////////////////////////////////////////////////////////////

namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsResponse : public IPacket
{
	typedef IPacket PacketBase;

// Construction
public:
	ObjectsResponse();
	virtual ~ObjectsResponse();

// Attributes
public:
	// Carica l'oggetto con l'hash specificato
	shared_ptr<ObjectsIObject> getObject(const ObjectID &id) const;
	// Carica gli id degli oggetti disponibili
	void getObjects(StringList &ids);

protected:
	DataTree * getRoot() const;

// Operations
public:
	// Salva l'oggetto da serializzare in fase di costruzione del pacchetto
	bool insert(shared_ptr<IPortalDatabase> database, const String &id);

protected:
	bool store(const shared_ptr<ObjectsIObject> &object);

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

#endif // _OS_P2P_PACKETS_OBJECTSRESPONSE_H
