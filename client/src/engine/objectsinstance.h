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

#ifndef _OBJECTS_INSTANCE_H
#define _OBJECTS_INSTANCE_H

#include "collections.h"
#include "objectsdescriptor.h"
#include "objectsirevisionable.h"
#include "iextensionsmodule.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsInstance : public ObjectsIRevisionable
{
	typedef ObjectsIRevisionable RevisionableBase;

// Construction
public:
	ObjectsInstance();
	virtual ~ObjectsInstance();

// Attributes
public:
	shared_ptr<IExtensionsModule> getModule() const;

	inline ObjectsInstanceInherit getInherit() const;
	inline void setInherit(const ObjectsInstanceInherit &inherit);

	inline ObjectsInstanceArea getArea() const;
	inline void setArea(const ObjectsInstanceArea &value);

	BlockType getBlockType() const;
	void setBlockType(BlockType type);

	static TimeDuration getCacheDuration();

// IRevisionable overrides
public:
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;

	virtual const String & getTitle() const;
	virtual const String & getContent() const;

	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<String> title;
	DbValue<uint32> area;
	DbValue<uint32> inherit;
	DbValue<ExtensionsModuleID> module;
	DbValue<String> data;
	DbValue<uint32> block;
	DbValue<String> style;

public:
	static uint32 VERSION;
};

//////////////////////////////////////////////////////////////////////

inline ObjectsInstanceInherit ObjectsInstance::getInherit() const { return static_cast<ObjectsInstanceInherit>(*inherit); }
inline void ObjectsInstance::setInherit(const ObjectsInstanceInherit &value) { inherit = static_cast<uint32>(value); }

inline ObjectsInstanceArea ObjectsInstance::getArea() const { return static_cast<ObjectsInstanceArea>(*area); }
inline void ObjectsInstance::setArea(const ObjectsInstanceArea &value) { area = static_cast<uint32>(value); }

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsInstance> objects_instance_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsInstance>(object); }

/////////////////////////////////////////////////////////////////////

class ObjectsInstanceDescriptor : public ObjectsDescriptor<ObjectsInstanceDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsInstanceDescriptor, portalObjectTypeInstance);

	typedef ObjectsDescriptor<ObjectsInstanceDescriptor> DescriptorBase;

// Construction
public:
	ObjectsInstanceDescriptor();
	virtual ~ObjectsInstanceDescriptor();

// IDescriptor interface
public:
	virtual shared_ptr<ObjectsIObject> createObject();
	virtual String getTableName();

	virtual String getTypeName();
	virtual String getGroupName();

// IDescriptor overrides
public:
	virtual OrderMode getOrderMode() const;
	virtual shared_ptr<ISearchOptions> getSearchOptions();

	virtual ide::edit::editor_ptr createEditControl(shared_ptr<EntitiesEntity> entity = null, shared_ptr<EntitiesEntity> parent = null);
	virtual ide::view::viewer_ptr createViewControl(shared_ptr<EntitiesEntity> entity = null);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_INSTANCE_H
