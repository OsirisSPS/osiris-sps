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

#ifndef _OBJECTS_POLL_H
#define _OBJECTS_POLL_H

#include "objectsdescriptor.h"
#include "objectsicommentable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsPoll : public ObjectsICommentable
{
	typedef ObjectsICommentable CommentableBase;

// Construction
public:
	ObjectsPoll();
	virtual ~ObjectsPoll();

// ICommentable overrides
public:
	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<uint32> type;

public:
	static uint32 VERSION;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsPoll> objects_poll_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsPoll>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsPollDescriptor : public ObjectsDescriptor<ObjectsPollDescriptor, ObjectsICommentableDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsPollDescriptor, portalObjectTypePoll);

	typedef ObjectsDescriptor<ObjectsPollDescriptor, ObjectsICommentableDescriptor> DescriptorBase;

// Construction
public:
	ObjectsPollDescriptor();
	virtual ~ObjectsPollDescriptor();

// IDescriptor interface
public:
	virtual shared_ptr<ObjectsIObject> createObject();
	virtual String getTableName();

	virtual String getTypeName();
	virtual String getGroupName();

// IDescriptor overrides
public:
	virtual void getAllowedChilds(ObjectsTypes &allowed_childs) const;
	virtual bool hasStatistics();
	virtual shared_ptr<ISearchOptions> getSearchOptions();
	virtual ide::edit::editor_ptr createEditControl(shared_ptr<EntitiesEntity> entity = nullptr, shared_ptr<EntitiesEntity> parent = nullptr);
	virtual ide::view::viewer_ptr createViewControl(shared_ptr<EntitiesEntity> entity);

	virtual void createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	virtual void computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	virtual void removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object = nullptr);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_POLL_H
