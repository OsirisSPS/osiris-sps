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
#include "idbconnection.h"

#include "datatable.h"
#include "dbsqlselect.h"
#include "idbparameter.h"
#include "idbresult.h"
#include "idbstatement.h"
#include "log.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// URGENT: clean me
inline static String toSQL(const String &v)
{
	String sql = v;
	sql.replace_all(_S("'"), _S("''"));
	return _S("'") + sql + _S("'");
}

//////////////////////////////////////////////////////////////////////

IDbConnection::IDbConnection(const String &database, shared_ptr<IDbOptions> options, shared_ptr<IDbDriver> driver)
{
	m_database = database;

	OS_ASSERT(options != nullptr);
	m_options = options;

	OS_ASSERT(driver != nullptr);
	m_driver = driver;
}

IDbConnection::~IDbConnection()
{

}

void IDbConnection::logError(const String &sql, std::exception &e) const
{
	OS_ASSERTFALSE();

	OS_LOG_ERROR(e.what() + _S(" - DB: ") + getDatabase() + _S(" - Sql: ") + sql);
}

uint32 IDbConnection::execute(const String &sql)
{
	try
	{
		return _execute(sql);
	}
	catch(std::exception &e)
	{
		logError(sql, e);
		throw(e);

		return 0;
	}
}

shared_ptr<IDbResult> IDbConnection::query(const String &sql)
{
	try
	{		
		return _query(sql);
	}
	catch(std::exception &e)
	{
		logError(sql, e);
		throw(e);

		return nullptr;
	}
}

bool IDbConnection::executeStatement(const String &sql, const DbSqlValues &values)
{
	try
	{
		shared_ptr<IDbStatement> statement = _prepare(sql);
		for(DbSqlValues::iterator i = values.begin(); i != values.end(); i++)
		{
			statement->addParameter()->setValue(i->second);
		}

		statement->execute();

		return true;
	}
	catch(std::exception &e)
	{
		logError(sql, e);
	}

	return false;
}

uint32 IDbConnection::execute(shared_ptr<DbSqlICommand> command)
{
	String sql;
	_parse(command, sql);
	return execute(sql);	
}

shared_ptr<IDbResult> IDbConnection::query(shared_ptr<DbSqlICommand> select)
{
	String sql;
	_parse(select, sql);
	return query(sql);	
}

bool IDbConnection::query(shared_ptr<DbSqlICommand> select, DataTable &table)
{
	String sql;
	_parse(select, sql);

	return query(sql, table);
}

DataItem IDbConnection::queryValue(const String &sql)
{
	DataItem value;

	DataTable result;
	if(query(sql, result))
		if(result.rows() > 0 && result.columns() > 0)
			value = *result[0][0];			
	return value;
}

bool IDbConnection::query(const String &sql, DataTable &table)
{
	shared_ptr<IDbResult> result = query(sql);
	if(result != nullptr)
	{
		result->bind(table);
		return true;
	}
	else
		return false;
}

uint32 IDbConnection::value_of(const String &sql) // TOCLEAN, rimpiazzare con la queryValue
{
	DataTable result;
	if(query(sql, result) == false)
		return false;

	uint32 value = 0;
	if(result.rows() > 0 && result.columns() > 0)
		value = *result[0][0];

	return value;
}

uint32 IDbConnection::value_of(shared_ptr<DbSqlICommand> command)
{
	try
	{
		String sql;
		_parse(command, sql);

		return value_of(sql);
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return 0;
}

String IDbConnection::escape(const String &name) const
{
	return name;
}

String IDbConnection::prepare_insert(const String &table, const DbSqlValues &values) const
{
	String record_fields;
	String record_values;

	record_fields = _S("(");
	record_values = _S("(");

	for(DbSqlValues::iterator i = values.begin(); i != values.end(); ++i)
	{
		if(i != values.begin())
		{
			record_fields += _S(", ");
			record_values += _S(", ");
		}

		record_fields += escape(i->first);
		record_values += _S("?");
	}

	record_fields += _S(")");
	record_values += _S(")");

	return _S("insert into ") + escape(table) + _S(" ") + record_fields + _S(" values ") + record_values;
}

String IDbConnection::prepare_update(const String &table, const DbSqlValues &values, const String &field, const String &value) const
{
	String record_values;

	for(DbSqlValues::iterator i = values.begin(); i != values.end(); ++i)
	{
		if(i != values.begin())
			record_values += _S(", ");

		record_values += escape(i->first) + _S(" = ?");
	}

	return _S("update ") + escape(table) + _S(" set ") + record_values + _S(" where ") + escape(field) + _S(" = ") + value;
}

String IDbConnection::prepare_update(const String &table, const DbSqlValues &values, const String &f1, const String &v1, const String &f2, const String &v2) const
{
	String record_values;

	for(DbSqlValues::iterator i = values.begin(); i != values.end(); ++i)
	{
		if(i != values.begin())
			record_values += _S(", ");

		record_values += escape(i->first) + _S(" = ?");
	}

	return _S("update ") + escape(table) + _S(" set ") + record_values + _S(" where ") + escape(f1) + _S(" = ") + v1 + _S(" and ") + escape(f2) + _S(" = ") + v2;
}

String IDbConnection::sql_remove(const String &table, const String &field, const String &value) const
{
	return _S("delete from ") + escape(table) + _S(" where ") + escape(field) + _S(" = ") + toSQL(value);
}

String IDbConnection::sql_remove(const String &table, const String &f1, const String &v1, const String &f2, const String &v2) const
{
	return _S("delete from ") + escape(table) + _S(" where ") + escape(f1) + _S(" = ") + toSQL(v1) + _S(" and ") + escape(f2) + _S(" = ") + toSQL(v2);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
