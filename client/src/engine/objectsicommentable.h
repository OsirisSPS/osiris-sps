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

#ifndef _ENGINE_OBJECTSICOMMENTABLE_H
#define _ENGINE_OBJECTSICOMMENTABLE_H

#include "objectsdescriptor.h"
#include "objectsirevisionable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsICommentable : public ObjectsIRevisionable
{
	typedef ObjectsIRevisionable RevisionableBase;

// Construction
public:
	ObjectsICommentable(PortalObjectType object_type, uint32 object_version);
	virtual ~ObjectsICommentable();

// Attributes
public:
	inline bool hasModel() const;

// Operations
private:
	bool _applyModel(String &str, shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal);
	
// IRevisionable overrides
public:
	virtual void getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies);

	virtual bool validate(shared_ptr<IPortalDatabase> database) const;
	virtual LanguageResult acceptable(shared_ptr<IPortalDatabase> database) const;

	virtual const String & getTitle() const;
	virtual const String & getDescription() const;
	virtual const String & getContent() const;

	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);

	virtual void onExportContent(String &content, shared_ptr<XMLPortalExporter> exporter);

protected:
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<String> title;
	DbValue<String> description;
	DbValue<String> content;
	DbValue<EntityID> model;
	DbValue<bool> pinned;
	DbValue<DateTime> closed_date;
};

//////////////////////////////////////////////////////////////////////

inline bool ObjectsICommentable::hasModel() const { return model->empty() == false; }

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsICommentable> objects_commentable_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsICommentable>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsICommentableDescriptor : public ObjectsIDescriptor
{
	typedef ObjectsIDescriptor DescriptorBase;

// Construction
public:
	ObjectsICommentableDescriptor();
	virtual ~ObjectsICommentableDescriptor();

// Operations
public:
	//void computeVotesStats(const shared_ptr<IPortalDatabase> database, shared_ptr<ObjectsIObject> object, DbValue<uint32>& outCount, DbValue<double>& outAverage);
	//void computeLastObjectStats(const shared_ptr<IPortalDatabase> database, shared_ptr<ObjectsIObject> object, DbValue<ObjectID>& outID, DbValue<DateTime> &outDate);

// IDescriptor overrides
public:
	virtual void getAllowedChilds(ObjectsTypes &allowed_childs) const;

	virtual bool hasStatistics();

	virtual void createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	virtual void computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);
	virtual void removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object = nullptr);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_OBJECTSICOMMENTABLE_H
