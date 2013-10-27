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
#include "datainstancecache.h"

#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DataInstanceCache::DataInstanceCache() : RecordBase(dataRecordTypeInstanceCache)
{

}

DataInstanceCache::~DataInstanceCache()
{
 
}

bool DataInstanceCache::elapsed(const TimeDuration &duration) const
{
	if(cache_date->isNull())
		return true;

	DateTime now = DateTime::now();
	if(now < *cache_date)
	{
		OS_ASSERTFALSE();	// Non dovrebbe verificarsi
		return true;		// Restituisce true in modo che venga aggiornata
	}

	TimeDuration offset = now - *cache_date;
	return offset > duration;
}

void DataInstanceCache::update(const String &data)
{
	cache = data;
	cache_date = DateTime::now();
}

bool DataInstanceCache::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RecordBase::validate(database) == false)
		return false;
	
	if(entity->empty())
		return false;

	return true;
}

String DataInstanceCache::getTableName() const
{
	return DBTABLES::INSTANCES_CACHE_TABLE;
}

String DataInstanceCache::getRecordKeyName() const
{
	return DBTABLES::INSTANCES_CACHE::ENTITY;
}

String DataInstanceCache::getRecordKeyValue() const
{
	return entity->toUTF16();
}

bool DataInstanceCache::onRead(const DataTableRow &row)
{
	entity.read(row, DBTABLES::INSTANCES_CACHE::ENTITY);
	cache.read(row, DBTABLES::INSTANCES_CACHE::CACHE);
	cache_date.read(row, DBTABLES::INSTANCES_CACHE::CACHE_DATE);

	return true;
}

bool DataInstanceCache::onWrite(DbSqlValues &values) const
{
	entity.write(values, DBTABLES::INSTANCES_CACHE::ENTITY);
	cache.write(values, DBTABLES::INSTANCES_CACHE::CACHE);
	cache_date.write(values, DBTABLES::INSTANCES_CACHE::CACHE_DATE);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
