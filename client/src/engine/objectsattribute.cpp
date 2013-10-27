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
#include "objectsattribute.h"

#include "objectssigner.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "datatree.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsAttributeDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ObjectsAttribute::ObjectsAttribute() : EntryBase(portalObjectTypeAttribute, OS_PORTAL_OBJECT_ATTRIBUTE_VERSION)
{
	type = 0;
}

ObjectsAttribute::~ObjectsAttribute()
{

}

void ObjectsAttribute::getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies)
{
	EntryBase::getDependencies(db, dependencies);

#ifndef OS_TODOCIP
	dependencies.push_back(reference->toObjectID());
#endif
}

LanguageResult ObjectsAttribute::acceptable(shared_ptr<IPortalDatabase> database) const
{
	LanguageResult result = EntryBase::acceptable(database);
	if(result.empty() == false)
		return result;

	if(reference->empty())
		return LanguageResult("reference_required");
	
	return LanguageResult();
}

bool ObjectsAttribute::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(EntryBase::onSign(signer) == false)
		return false;

	signer->add(reference);
	signer->add(type);
	signer->add(value);

	return true;
}

bool ObjectsAttribute::onRead(const ObjectID &id, const DataTree &data)
{
	if(EntryBase::onRead(id, data) == false)
		return false;

	reference.read(data, DBTABLES::ATTRIBUTES::REFERENCE);
	type.read(data, DBTABLES::ATTRIBUTES::TYPE);
	value.read(data, DBTABLES::ATTRIBUTES::VALUE);

	return true;
}

bool ObjectsAttribute::onWrite(DataTree &data) const
{
	if(EntryBase::onWrite(data) == false)
		return false;

	reference.write(data, DBTABLES::ATTRIBUTES::REFERENCE);
	type.write(data, DBTABLES::ATTRIBUTES::TYPE);
	value.write(data, DBTABLES::ATTRIBUTES::VALUE);

	return true;
}

bool ObjectsAttribute::onRead(const DataTableRow &row)
{
	if(EntryBase::onRead(row) == false)
		return false;

	reference.read(row, DBTABLES::ATTRIBUTES::REFERENCE);
	type.read(row, DBTABLES::ATTRIBUTES::TYPE);
	value.read(row, DBTABLES::ATTRIBUTES::VALUE);

	return true;
}

bool ObjectsAttribute::onWrite(DbSqlValues &values) const
{
	if(EntryBase::onWrite(values) == false)
		return false;

	reference.write(values, DBTABLES::ATTRIBUTES::REFERENCE);
	type.write(values, DBTABLES::ATTRIBUTES::TYPE);
	value.write(values, DBTABLES::ATTRIBUTES::VALUE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsAttributeDescriptor::ObjectsAttributeDescriptor()
{

}

ObjectsAttributeDescriptor::~ObjectsAttributeDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsAttributeDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsAttribute());
}

String ObjectsAttributeDescriptor::getTableName()
{
	return DBTABLES::ATTRIBUTES_TABLE;
}

String ObjectsAttributeDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_ATTRIBUTE_TYPENAME;
}

String ObjectsAttributeDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_ATTRIBUTE_GROUPNAME;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
