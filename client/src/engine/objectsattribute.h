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

#ifndef _OBJECTS_ATTRIBUTE_H
#define _OBJECTS_ATTRIBUTE_H

#include "objectsdescriptor.h"
#include "objectsientry.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsAttribute : public ObjectsIEntry
{
	typedef ObjectsIEntry EntryBase;

// Construction
public:
	ObjectsAttribute();
	virtual ~ObjectsAttribute();

// IEntry overrides
public:
	virtual void getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies);
	virtual LanguageResult acceptable(shared_ptr<IPortalDatabase> database) const;

protected:
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<EntityID> reference;
	DbValue<uint32> type;
	DbValue<uint32> value;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsAttribute> objects_attribute_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsAttribute>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsAttributeDescriptor : public ObjectsDescriptor<ObjectsAttributeDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsAttributeDescriptor, portalObjectTypeAttribute);

	typedef ObjectsDescriptor<ObjectsAttributeDescriptor> DescriptorBase;

// Construction
public:
	ObjectsAttributeDescriptor();
	virtual ~ObjectsAttributeDescriptor();

// IDescriptor interface
public:
	virtual shared_ptr<ObjectsIObject> createObject();
	virtual String getTableName();

	virtual String getTypeName();
	virtual String getGroupName();
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_ATTRIBUTE_H
