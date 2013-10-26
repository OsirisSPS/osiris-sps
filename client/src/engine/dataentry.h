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

#ifndef _ENGINE_DATAENTRY_H
#define _ENGINE_DATAENTRY_H

#include "datetime.h"
#include "dbvalue.h"
#include "datairecord.h"
#include "objectsobjects.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport DataEntry : public DataIRecord
{
public:
	typedef list<shared_ptr<DataEntry> >::type DataEntryList;

private:
	typedef DataIRecord RecordBase;

// Construction
public:
	DataEntry();
	virtual ~DataEntry();

// Attributes
public:
	PortalObjectType getObjectType() const;

// IRecord interface
protected:
	virtual String getTableName() const;
	virtual String getRecordKeyName() const;
	virtual String getRecordKeyValue() const;

	virtual bool onRead(const DataTableRow &row);
	virtual bool onWrite(DbSqlValues &values) const;

public:
	DbValue<ObjectID> id;
	DbValue<EntityID> parent;
#ifdef OS_TODOCIP
	DbValue<EntityID> entity;
	DbValue<ObjectID> entity_author; // TOCLEAN_DATAENTRY: potrei toglierlo se non è usato, soprattutto da SQL..
	DbValue<DateTime> entity_date; // TOCLEAN_DATAENTRY: potrei toglierlo se non è usato, soprattutto da SQL..
	//DbValue<ObjectID> entity_parent; // TOCLEAN_DATAENTRY
#else
	DbValue<EntityID> revision;
#endif
	DbValue<ObjectID> author;
	//DbValue<ObjectID> pov; // TOCLEAN_DATAENTRY
	DbValue<DateTime> submit_date;
	DbValue<DateTime> insert_date;
	DbValue<uint32> type;
	DbValue<uint32> rank;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DataEntry> dataentry_cast(shared_ptr<DataIRecord> record) { return boost::dynamic_pointer_cast<DataEntry>(record); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_DATAENTRY_H
