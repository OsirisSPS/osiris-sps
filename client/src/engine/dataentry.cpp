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
#include "dataentry.h"

#include "dbbinder.h"
#include "dbtables.h"
#include "constants.h"
#include "convert.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DataEntry::DataEntry() : RecordBase(dataRecordTypeEntry)					 
{		
	submit_date = DateTime::EMPTY;
	insert_date = DateTime::EMPTY;
	type = OS_PORTAL_OBJECT_TYPE_UNKNOWN;
	rank = 0;
}

DataEntry::~DataEntry()
{

}

PortalObjectType DataEntry::getObjectType() const 
{ 
	return Convert::toObjectType(type); 
}

String DataEntry::getTableName() const
{
	return DBTABLES::ENTRIES_TABLE;
}

String DataEntry::getRecordKeyName() const
{
	return DBTABLES::ENTRIES::ID;
}

String DataEntry::getRecordKeyValue() const
{
	return id->toUTF16();
}

bool DataEntry::onRead(const DataTableRow &row)
{
	id.read(row, DBTABLES::ENTRIES::ID);
	parent.read(row, DBTABLES::ENTRIES::PARENT);
#ifdef OS_TODOCIP
	entity.read(row, DBTABLES::ENTRIES::ENTITY);
	entity_author.read(row, DBTABLES::ENTRIES::ENTITY_AUTHOR);
	entity_date.read(row, DBTABLES::ENTRIES::ENTITY_DATE);
	//entity_parent.read(row, DBTABLES::ENTRIES::ENTITY_PARENT); // TOCLEAN_DATAENTRY
#else
	revision.read(row, DBTABLES::ENTRIES::REVISION);
#endif

	author.read(row, DBTABLES::ENTRIES::AUTHOR);
	submit_date.read(row, DBTABLES::ENTRIES::SUBMIT_DATE);
	insert_date.read(row, DBTABLES::ENTRIES::INSERT_DATE);
	type.read(row, DBTABLES::ENTRIES::TYPE);
	rank.read(row, DBTABLES::ENTRIES::RANK);

	return true;
}

bool DataEntry::onWrite(DbSqlValues &values) const
{
	id.write(values, DBTABLES::ENTRIES::ID);
	parent.write(values, DBTABLES::ENTRIES::PARENT);
#ifdef OS_TODOCIP
	entity.write(values, DBTABLES::ENTRIES::ENTITY);
	entity_author.write(values, DBTABLES::ENTRIES::ENTITY_AUTHOR);
	entity_date.write(values, DBTABLES::ENTRIES::ENTITY_DATE);
	//entity_parent.write(values, DBTABLES::ENTRIES::ENTITY_PARENT); // TOCLEAN_DATAENTRY
#else
	revision.write(values, DBTABLES::ENTRIES::REVISION);
#endif
	author.write(values, DBTABLES::ENTRIES::AUTHOR);
	submit_date.write(values, DBTABLES::ENTRIES::SUBMIT_DATE);
	insert_date.write(values, DBTABLES::ENTRIES::INSERT_DATE);
	type.write(values, DBTABLES::ENTRIES::TYPE);
	rank.write(values, DBTABLES::ENTRIES::RANK);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
