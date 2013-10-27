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
#include "datairecord.h"

#include "idbconnection.h"
#include "convert.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DataIRecord::DataIRecord(DataRecordType record_type) : m_recordType(record_type)
{

}

DataIRecord::~DataIRecord()
{

}

bool DataIRecord::read(const DataTableRow &row)
{
	return onRead(row);
}

bool DataIRecord::write(DbSqlValues &values) const
{
	return onWrite(values);
}

bool DataIRecord::validate(shared_ptr<IPortalDatabase> database) const
{
	return true;
}

/*
LanguageResult DataIRecord::acceptable(shared_ptr<IPortalDatabase> database) const
{
	return LanguageResult();
}
*/

String DataIRecord::getRecordKey2Name() const
{
	return String::EMPTY;
}

String DataIRecord::getRecordKey2Value() const
{
	return String::EMPTY;
}

bool DataIRecord::insert(shared_ptr<IDbConnection> connection)
{
	// CLODOURGENT deve fare la validateRecord? si no forse...
	DbSqlValues values;
	write(values);
	return connection->executeStatement(connection->prepare_insert(getTableName(), values), values);		
}

bool DataIRecord::update(shared_ptr<IDbConnection> connection) 
{
	DbSqlValues values;
	write(values);
	if(getRecordKey2Name().empty())
		return connection->executeStatement(connection->prepare_update(getTableName(), values, getRecordKeyName(), Convert::toSQL(getRecordKeyValue())), values);
	else
		return connection->executeStatement(connection->prepare_update(getTableName(), values, getRecordKeyName(), Convert::toSQL(getRecordKeyValue()), getRecordKey2Name(), Convert::toSQL(getRecordKey2Value())), values);		
}

bool DataIRecord::remove(shared_ptr<IDbConnection> connection) 
{
	if(getRecordKey2Name().empty())
		return (connection->execute(connection->sql_remove(getTableName(), getRecordKeyName(), Convert::toSQL(getRecordKeyValue()))) == 1);
	else
		return (connection->execute(connection->sql_remove(getTableName(), getRecordKeyName(), Convert::toSQL(getRecordKeyValue()), getRecordKey2Name(), Convert::toSQL(getRecordKey2Value()))) == 1);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
