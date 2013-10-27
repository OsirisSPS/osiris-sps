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
#include "dataistatistics.h"

#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "objectsiobject.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DataIStatistics::DataIStatistics() : RecordBase(dataRecordTypeStatistics)
{

}

DataIStatistics::~DataIStatistics()
{

}

bool DataIStatistics::create_ZZZ(shared_ptr<IPortalDatabase> db)
{
	OS_ASSERT(db != null);	
	return db->insertRecord(get_this_ptr());
}

bool DataIStatistics::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RecordBase::validate(database) == false)
		return false;
	
	if(reference->empty())
		return false;

	return true;
}

String DataIStatistics::getTableName() const
{
	return String::EMPTY;
}

String DataIStatistics::getRecordKeyName() const
{
	return DBTABLES::REFERENCE;
}

String DataIStatistics::getRecordKeyValue() const
{
	//return reference->toUTF16();
	return reference;
}

bool DataIStatistics::onRead(const DataTableRow &row)
{
	reference.read(row, DBTABLES::REFERENCE);

	return true;
}

bool DataIStatistics::onWrite(DbSqlValues &values) const
{
	reference.write(values, DBTABLES::REFERENCE);

	return true;
}

void DataIStatistics::exportXML(shared_ptr<XMLPortalExporter> exporter)
{

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
