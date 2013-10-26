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

#ifndef _OBJECTS_SECTION_H
#define _OBJECTS_SECTION_H

#include "objectsdescriptor.h"
#include "objectsirevisionable.h"
#include "extensionsextensions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IExtensionsComponent;

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsSection : public ObjectsIRevisionable
{
	typedef ObjectsIRevisionable RevisionableBase;

// Construction
public:
	ObjectsSection();
	virtual ~ObjectsSection();

// Attributes
public:
	//inline bool hasComponent() const;

	ExtensionsComponentID getComponentID(shared_ptr<Portal> portal) const;
	shared_ptr<IExtensionsComponent> getComponent(shared_ptr<Portal> portal) const;

	static shared_ptr<ObjectsSection> getRootSection();
	static shared_ptr<ObjectsSection> getRecursiveSection();
	static shared_ptr<ObjectsSection> getOrphanSection();
	static shared_ptr<ObjectsSection> getSkippedSection();

// IRevisionable overrides
public:
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;

	virtual const String & getTitle() const;
	virtual const String & getDescription() const;

	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<String> title;
	DbValue<String> description;
	DbValue<ExtensionsComponentID> component;
	DbValue<String> data;

public:
	static uint32 VERSION;
};

//////////////////////////////////////////////////////////////////////

//inline bool Section::hasComponent() const { return component->empty() == false; }

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsSection> objects_section_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsSection>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsSectionDescriptor : public ObjectsDescriptor<ObjectsSectionDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsSectionDescriptor, portalObjectTypeSection);

	typedef ObjectsDescriptor<ObjectsSectionDescriptor> DescriptorBase;

// Construction
public:
	ObjectsSectionDescriptor();
	virtual ~ObjectsSectionDescriptor();

// IDescriptor interface
public:
	virtual shared_ptr<ObjectsIObject> createObject();
	virtual String getTableName();

	virtual String getTypeName();
	virtual String getGroupName();

// IDescriptor overrides
public:
	virtual void getAllowedChilds(ObjectsTypes &allowed_childs) const;
	virtual OrderMode getOrderMode() const;
	virtual bool hasStatistics();
	virtual shared_ptr<ISearchOptions> getSearchOptions();
	virtual ide::edit::editor_ptr createEditControl(shared_ptr<EntitiesEntity> entity = null, shared_ptr<EntitiesEntity> parent = null);
	virtual ide::view::viewer_ptr createViewControl(shared_ptr<EntitiesEntity> entity);

	virtual void createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	virtual void computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	virtual void removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object = null);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_SECTION_H
