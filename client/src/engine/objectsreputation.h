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

#ifndef _OBJECTS_REPUTATION_H
#define _OBJECTS_REPUTATION_H

#include "objectsdescriptor.h"
#include "objectsientry.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsReputation : public ObjectsIEntry
{
	typedef ObjectsIEntry EntryBase;

// Construction
public:
	ObjectsReputation();
	virtual ~ObjectsReputation();

// IEntry overrides
public:
	virtual void ensureID(const shared_ptr<IPortalDatabase> &db);
	virtual void getDependencies(const shared_ptr<IPortalDatabase> &db, list<ObjectID>::type &dependencies);
	virtual void exportXML(shared_ptr<XMLPortalExporter> exporter);
	virtual bool validate(shared_ptr<IPortalDatabase> database) const;
	virtual LanguageResult acceptable(shared_ptr<IPortalDatabase> database) const;

protected:
	virtual bool onSign(const shared_ptr<ObjectsSigner> &signer) const;

	virtual bool onRead(const ObjectID &id, const DataTree &data);
	virtual bool onWrite(DataTree &data) const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<ObjectID> reference;
	DbValue<double> score;
	DbValue<String> description;
	DbValue<bool> follow;

public:
	static uint32 VERSION;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ObjectsReputation> objects_reputation_cast(shared_ptr<ObjectsIObject> object) { return boost::dynamic_pointer_cast<ObjectsReputation>(object); }

//////////////////////////////////////////////////////////////////////

class ObjectsReputationDescriptor : public ObjectsDescriptor<ObjectsReputationDescriptor>
{
	OS_DECLARE_DESCRIPTOR(ObjectsReputationDescriptor, portalObjectTypeReputation);

	typedef ObjectsDescriptor<ObjectsReputationDescriptor> DescriptorBase;

// Construction
public:
	ObjectsReputationDescriptor();
	virtual ~ObjectsReputationDescriptor();

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

#endif // _OBJECTS_REPUTATION_H
