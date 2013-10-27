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
#include "p2ppacketsobjectsresponse.h"

#include "convert.h"
#include "objectssystem.h"
#include "iportaldatabase.h"
#include "datatree.h"
#include "objectsiobject.h"
#include "p2pexchangesession.h"
#include "portalsportal.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

const String ObjectsResponse::OBJECTS = _S("objects");

//////////////////////////////////////////////////////////////////////

ObjectsResponse::ObjectsResponse() : PacketBase(ptObjectsResponse)
{
	getData()->setT(OBJECTS);
}

ObjectsResponse::~ObjectsResponse()
{

}

shared_ptr<ObjectsIObject> ObjectsResponse::getObject(const ObjectID &id) const
{
	DataTree *root = getRoot();
	OS_ASSERT(root != null);
	OS_ASSERT(root->exists(id.toUTF16()));

	// Ottiene il puntatore alla root dell'oggetto
	DataTree *objectData = root->getTP(id.toUTF16());
	if(objectData == null)
		return null;

	PortalObjectType objectType = Convert::toObjectType(static_cast<uint32>(objectData->getV(DBTABLES::ENTRIES::TYPE)));

	// Alloca l'oggetto in base alla tipologia
	shared_ptr<ObjectsIObject> object = ObjectsSystem::instance()->createObject(objectType);
	if(object == null)
		return null;

    // Carica l'oggetto
	object->read(id, *objectData);

	return object;
}

void ObjectsResponse::getObjects(StringList &ids)
{
	DataTree *root = getRoot();
	if(root != null)
		for(DataTree::iterator i = root->begin(); i != root->end(); ids.push_back(i->first), ++i);
}

DataTree * ObjectsResponse::getRoot() const
{
	return getData()->getTP(OBJECTS);
}

bool ObjectsResponse::insert(shared_ptr<IPortalDatabase> database, const String &id)
{
    if(database == null || id.empty())
    {
        OS_ASSERTFALSE();
        return false;
    }

    shared_ptr<ObjectsIObject> object = database->getPortal()->getObject(database, id.to_ascii());
    if(object == null)
        return false;

    return store(object);
}

bool ObjectsResponse::store(const shared_ptr<ObjectsIObject> &object)
{
	OS_ASSERT(object != null);

	DataTree *root = getRoot();

	OS_ASSERT(root != null);
	OS_ASSERT(root->exists(object->id->toUTF16()) == false);

	// Crea la root dell'oggetto
	DataTree *objectData = root->setT(object->id->toUTF16());
	if(objectData == null)
		return false;

	// Salva l'oggetto
	return object->write(*objectData);
}

bool ObjectsResponse::create()
{
	return true;
}

bool ObjectsResponse::parse()
{
	// Valida il pacchetto se  presente la root degli oggetti
	return getRoot() != null;
}

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
