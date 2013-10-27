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

#ifndef _OBJECTS_POLLOPTION_H
#define _OBJECTS_POLLOPTION_H

#include "objectsdescriptor.h"
#include "objectsirevisionable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsPollOption : public ObjectsIRevisionable
{
	typedef ObjectsIRevisionable RevisionableBase;

// Construction
public:
	ObjectsPollOption();
	virtual ~ObjectsPollOption();

// Attributes
public:
	inline const EntityID & getPoll() const;

// IRevisionable overrides
public:
	virtual bool requireParent() const;

	virtual bool validate(shared_ptr<IPortalDatabase> database) const;

	virtual const String & getTitle() const;

	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<String> title;

public:
	static uint32 VERSION;
};

//////////////////////////////////////////////////////////////////////

inline const EntityID & ObjectsPollOption::getPoll() const { return getParent(); }

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsPollOption> objects_poll_option_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsPollOption>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsPollOptionDescriptor : public ObjectsDescriptor<ObjectsPollOptionDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsPollOptionDescriptor, portalObjectTypePollOption);

	typedef ObjectsDescriptor<ObjectsPollOptionDescriptor> DescriptorBase;

// Construction
public:
	ObjectsPollOptionDescriptor();
	virtual ~ObjectsPollOptionDescriptor();

// IDescriptor interface
public:
	virtual shared_ptr<ObjectsIObject> createObject();
	virtual String getTableName();

	virtual String getTypeName();
	virtual String getGroupName();

// IDescriptor overrides
public:
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

#endif // _OBJECTS_POLLOPTION_H
