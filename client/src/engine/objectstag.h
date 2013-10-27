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

#ifndef _OBJECTS_TAG_H
#define _OBJECTS_TAG_H

#include "objectsdescriptor.h"
#include "objectsirevisionable.h"
#include "buffer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsTag : public ObjectsIRevisionable
{
	typedef ObjectsIRevisionable RevisionableBase;

// Construction
public:
	ObjectsTag();
	virtual ~ObjectsTag();

// IRevisionable overrides
public:
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;

	virtual const String & getTitle() const;

	virtual bool requireParent() const;

	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onCreate(const shared_ptr<IPortalDatabase> &db, const Buffer &privateKey);
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<String> name;

public:
	static uint32 VERSION;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsTag> objects_tag_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsTag>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsTagDescriptor : public ObjectsDescriptor<ObjectsTagDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsTagDescriptor, portalObjectTypeTag);

	typedef ObjectsDescriptor<ObjectsTagDescriptor> DescriptorBase;

// Construction
public:
	ObjectsTagDescriptor();
	virtual ~ObjectsTagDescriptor();

// IDescriptor interface
public:
	virtual shared_ptr<ObjectsIObject> createObject();
	virtual String getTableName();

	virtual String getTypeName();
	virtual String getGroupName();

// IDescriptor overrides
public:
	virtual bool isGroupable();
	virtual shared_ptr<ISearchOptions> getSearchOptions();

	virtual ide::edit::editor_ptr createEditControl(shared_ptr<EntitiesEntity> entity = null, shared_ptr<EntitiesEntity> parent = null);
	virtual ide::view::viewer_ptr createViewControl(shared_ptr<EntitiesEntity> entity);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_TAG_H
