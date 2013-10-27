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
#include "parser.h"

#include "convert.h"
#include "conversions.h"
#include "dbschemafield.h"
#include "dbschemaindex.h"
#include "dbschemaindexfield.h"
#include "dbschematable.h"
#include "dbsqlactions.h"
#include "dbsqlselect.h"
#include "dbsqlinsert.h"
#include "dbsqlupdate.h"
#include "dbsqldelete.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

void Parser::parseCommand(shared_ptr<DbSqlICommand> command, String &sql)
{
	switch(command->getType())
	{
	case ctSelect:			parseSelect(boost::dynamic_pointer_cast<DbSqlSelect>(command), sql);
								break;

	case ctInsert:			parseInsert(boost::dynamic_pointer_cast<DbSqlInsert>(command), sql);
								break;

	case ctUpdate:			parseUpdate(boost::dynamic_pointer_cast<DbSqlUpdate>(command), sql);
								break;

	case ctDelete:			parseDelete(boost::dynamic_pointer_cast<DbSqlDelete>(command), sql);
								break;

	case ctCreateTable:	parseCreateTable(boost::dynamic_pointer_cast<DbSqlCreateTable>(command), sql);
								break;

	case ctRenameTable:	parseRenameTable(boost::dynamic_pointer_cast<DbSqlRenameTable>(command), sql);
								break;

	case ctDropTable:		parseDropTable(boost::dynamic_pointer_cast<DbSqlDropTable>(command), sql);  									
								break;

	case ctCreateField:	parseCreateField(boost::dynamic_pointer_cast<DbSqlCreateField>(command), sql);  									
								break;

	case ctCreateIndex:	parseCreateIndex(boost::dynamic_pointer_cast<DbSqlCreateIndex>(command), sql);  									
								break;

	case ctDropIndex:		parseDropIndex(boost::dynamic_pointer_cast<DbSqlDropIndex>(command), sql);  									
								break;
	
	case ctRebuildTable:	parseRebuildTable(boost::dynamic_pointer_cast<DbSqlRebuildTable>(command), sql);  									
								break;

	default:					OS_EXCEPTION("Unknown sql command");
								break;
	}
}

void Parser::parseSelect(shared_ptr<DbSqlSelect> command, String &sql)
{
	sql = command->distinct ? _S("select distinct ") : _S("select ");

	if(command->count)
	{
		sql += _S("count(*)");
	}
	else
	{
		String fields;
		parseFields(command->fields, fields);
		if(fields.empty())
			sql += _S("*");
		else
			sql += fields;
	}

	String tables;
	parseTables(command->tables, tables);
	sql += _S(" from ") + tables;

	String where;
	parseWhere(command->where, where);
	if(where.empty() == false)
		sql += _S(" where ") + where;

	String groupBy;
	parseGroupBy(command->groupBy, groupBy);
	if(groupBy.empty() == false)
		sql += _S(" group by ") + groupBy;

	if(command->isCompound())
	{
		String right;
		parseSelect(command->getConnection(), right);

		switch(command->getConnectionType())
		{
		case DbSqlSelect::ctNone:			OS_EXCEPTION("Invalid connection type");
											break;

		case DbSqlSelect::ctUnion:			sql += _S(" union ") + right;
											break;

		case DbSqlSelect::ctUnionAll:		sql += _S(" union all ") + right;
											break;

		case DbSqlSelect::ctIntersect:		sql += _S(" intersect ") + right;
											break;

		case DbSqlSelect::ctExcept:			sql += _S(" except ") + right;
											break;
		}
	}

	String orderBy;
	parseOrderBy(command->orderBy, orderBy);
	if(orderBy.empty() == false)
		sql += _S(" order by ") + orderBy;

	String limit;
	parseLimit(command->limit, limit);
	if(limit.empty() == false)
		sql += _S(" limit ") + limit;
}

void Parser::parseInsert(shared_ptr<DbSqlInsert> command, String &sql)
{
	OS_EXCEPTION("Unimplemented");
}

void Parser::parseUpdate(shared_ptr<DbSqlUpdate> command, String &sql)
{
	OS_EXCEPTION("Unimplemented");
}

void Parser::parseDelete(shared_ptr<DbSqlDelete> command, String &sql)
{
	OS_EXCEPTION("Unimplemented");
}

void Parser::parseCreateTable(shared_ptr<DbSqlCreateTable> command, String &sql)
{
	parseCreateTable(command->getTable(), sql);
}

void Parser::parseRenameTable(shared_ptr<DbSqlRenameTable> command, String &sql)
{
	sql = String::format(_S("alter table %S rename to %S").c_str(), command->getOldTable().c_str(), command->getNewTable().c_str());
}

void Parser::parseCreateField(shared_ptr<DbSqlCreateField> command, String &sql)
{
	shared_ptr<DbSchemaField> field = command->getField();

	String field_sql;
	_generateFieldSql(field, field_sql);

	sql = String::format(_S("alter table %S add %S").c_str(), field->getTableName().c_str(), field_sql.c_str());
}

void Parser::parseCreateIndex(shared_ptr<DbSqlCreateIndex> command, String &sql)
{
	_generateIndexSql(command->getIndex(), sql);
}

void Parser::parseDropIndex(shared_ptr<DbSqlDropIndex> command, String &sql)
{
	shared_ptr<DbSchemaIndex> index = command->getIndex();
	OS_ASSERT(index != null);

	sql = _S("drop index if exists ") + index->getName();
}

void Parser::parseRebuildTable(shared_ptr<DbSqlRebuildTable> command, String &sql)
{
	shared_ptr<DbSchemaTable> newTable = command->getNewTable();
	shared_ptr<DbSchemaTable> oldTable = command->getOldTable();

	// Genera un nome di una tabella temporanea
	String temporanyTable = _S("temp_") + oldTable->getName();
	sql = _S("create table ") + temporanyTable + _S(" as select * from ") + oldTable->getName() + _S(";\r\n");
	// Droppa la vecchia tabella
	sql += _S("drop table ") + oldTable->getName() + _S(";\r\n");
	String sqlNewTable;
	// Genera l'sql per la creazione della nuova tabella
	parseCreateTable(newTable, sqlNewTable);
	// Aggiunge l'sql per la creazione della nuova tabella
	sql += sqlNewTable;
	sql += _S("\r\n");

	String newFieldsList;
	String oldFieldsList;
	// Estra i campi che devono essere esportati dalla tabella temporanea
	const DbSqlRebuildTable::ImportFields &importFields = command->getImportFields();
	for(DbSqlRebuildTable::ImportFields::const_iterator i = importFields.begin(); i != importFields.end(); ++i)
	{
		if(i != importFields.begin())
		{
			newFieldsList += _S(", ");
			oldFieldsList += _S(", ");
		}

		newFieldsList += i->first;
		oldFieldsList += i->second;
	}
	sql += _S("insert into ") + newTable->getName() + _S("(") + newFieldsList + _S(") select ") + oldFieldsList + _S(" from ") + temporanyTable + _S(";\r\n");
	sql += _S("drop table ") + temporanyTable + _S(";\r\n");
}

void Parser::parseDropTable(shared_ptr<DbSqlDropTable> command, String &sql)
{
	sql = _S("drop table ") + command->getTable();
}

void Parser::parseFields(const DbSqlFields &fields, String &sql)
{
	for(DbSqlFields::const_iterator i = fields.begin(); i != fields.end(); ++i)
	{
		const DbSqlField &field = *i;

		if(sql.empty() == false)
			sql += _S(", ");
		
		sql += parseField(*i);
	}
}

String Parser::parseField(const DbSqlField &field)
{
	String field_name = field.formatName();
	if(field.hasAlias())
		field_name += _S(" as ") + field.getAlias();

	String value;

	switch(field.getAttribute())
	{
	case DbSqlField::faNone:	value = field_name;
								break;

	case DbSqlField::faCount:	value = String::format(_S("count(%S)").c_str(), field_name.c_str());
								break;

	case DbSqlField::faMin:		value = String::format(_S("min(%S)").c_str(), field_name.c_str());
								break;

	case DbSqlField::faMax:		value = String::format(_S("max(%S)").c_str(), field_name.c_str());
								break;

	default:					OS_EXCEPTION("Unknown field attribute");
								break;
	}

	switch(field.getOrder())
	{
	case DbSqlField::foNone:	break;

	case DbSqlField::foAsc:		value += _S(" asc");
								break;

	case DbSqlField::foDesc:	value += _S(" desc");
								break;
	}

	return value;
}

void Parser::parseTables(const DbSqlTables &tables, String &sql)
{
    for(DbSqlTables::const_iterator i = tables.begin(); i != tables.end(); ++i)
	{
		const DbSqlTable &table = *i;

		if(sql.empty() == false)
			sql += _S(", ");

		sql += table.getName();

		const String &suggestedIndex = table.getSuggestedIndex();
		if(suggestedIndex.empty() == false)
			sql += " indexed by " + suggestedIndex;		
	}
}

void Parser::parseWhere(const DbSqlWhere &where, String &sql)
{
	parseConditions(where.conditions, sql);
}

void Parser::parseConditions(const DbSqlConditions &conditions, String &sql)
{
	for(DbSqlConditions::const_iterator i = conditions.begin(); i != conditions.end(); ++i)	
	{
		const DbSqlCondition &condition = *i;

		if(sql.empty() == false)
		{
			if(condition.isAnd())
				sql += _S(" and ");
			else if(condition.isOr())
				sql += _S(" or ");
			else
				OS_EXCEPTION("Unimplemented");
		}

		String type;
		
		if(condition.hasMinor())
			type += _S("<");
		if(condition.hasMajor())
			type += _S(">");
		if(condition.hasEqual())
			type += _S("=");
		if(condition.hasDifferent())
			type += _S("!=");
		if(condition.hasLike())
			type += _S("like");		
		if(condition.hasIn())
			type += _S("in");

		if(condition.hasCommand())
		{
			String command;
			parseCommand(condition.getCommand(), command);
		
			sql += String::format(_S("%S %S (%S)").c_str(), parseField(condition.getField()).c_str(), type.c_str(), command.c_str());
		}
		else
		{
			sql += String::format(_S("%S %S %S").c_str(), parseField(condition.getField()).c_str(), type.c_str(), condition.getValue().c_str());
		}
	}
}

void Parser::parseGroupBy(const DbSqlGroupBy &groupBy, String &sql)
{
	parseFields(groupBy.fields, sql);
}

void Parser::parseOrderBy(const DbSqlOrderBy &orderBy, String &sql)
{
	parseFields(orderBy.fields, sql);
}

void Parser::parseLimit(const DbSqlLimit &limit, String &sql)
{
	if(limit.exists())
	{
		if(limit.hasOffset())
			sql = String::format(_S("%d, %d").c_str(), limit.getOffset(), limit.getCount());
		else
			sql = conversions::to_utf16(limit.getCount());
	}
}

bool Parser::parseCreateTable(shared_ptr<DbSchemaTable> table, String &sql)
{
	sql = _S("CREATE TABLE ") + table->getName() + _S(" (\r\n");

	String primary_key;

	const DbSchemaFields &fields = table->getFields();
	for(DbSchemaFields::const_iterator fi = fields.begin(); fi != fields.end(); ++fi)
	{
		shared_ptr<DbSchemaField> field = *fi;

		String field_sql;
		if(_generateFieldSql(field, field_sql) == false)
			return false;
	
		sql += field_sql;
		sql += _S(",\r\n");

		if(field->getPrimary())
		{
			if(primary_key.empty())
				primary_key = _S("PRIMARY KEY (") + field->getName();
			else
				primary_key += _S(", ") + field->getName();
		}
	}

	if(primary_key.empty() == false)
		primary_key += _S("),\r\n");

	sql += primary_key;

	sql.trim_right(_S(",\r\n"));
	sql += _S("\r\n);");

	const DbSchemaIndexes &indexes = table->getIndexes();
	if(indexes.empty() == false)
	{
		sql += _S("\r\n");

		for(DbSchemaIndexes::const_iterator ii = indexes.begin(); ii != indexes.end(); ++ii)
		{
			String indexSQL;
			if(_generateIndexSql(*ii, indexSQL))
			{
				sql += indexSQL;
				sql += _S(";\r\n");
			}
		}
	}		

	return true;
}

bool Parser::_generateFieldSql(shared_ptr<DbSchemaField> field, String &sql)
{
	switch(field->getType())
	{
	case ftUnknown:		return false;
									
	case ftBool:		return _generateBoolFieldSql(field, sql);

	case ftInt:			
	case ftUint:		// Gli interi senza segno non sono supportati in sqlite
								return _generateIntFieldSql(field, sql);

	case ftDouble:		return _generateDoubleFieldSql(field, sql);

	case ftVarchar:		return _generateVarcharFieldSql(field, sql);

	case ftText:		return _generateTextFieldSql(field, sql);

	case ftBlob:		return _generateBlobFieldSql(field, sql);

	default:					OS_ASSERTFALSE();
								break;
	}

	return false;
}

bool Parser::_generateIndexSql(shared_ptr<DbSchemaIndex> index, String &sql)
{
	OS_ASSERT(index != null);

	const DbSchemaIndexFields &indexFields = index->getFields();
	if(indexFields.empty())
		return false;

	if(index->getUnique())
		sql = _S("create unique index");
	else
		sql = _S("create index");

	sql += String::format(_S(" if not exists %S on %S (").c_str(), index->getName().c_str(), index->getTableName().c_str());	

	for(DbSchemaIndexFields::const_iterator i = indexFields.begin(); i != indexFields.end(); ++i)
	{
		if(i != indexFields.begin())
		{
			sql += _S(", ");
		}

		shared_ptr<DbSchemaIndexField> indexField = *i;

		String order;
		switch(indexField->getOrder())
		{
		case DbSchemaIndexField::ifoAsc:		order = _S("asc");
													break;

		case DbSchemaIndexField::ifoDesc:		order = _S("desc");
													break;

		default:									OS_ASSERTFALSE();
													break;
		}

		sql += String::format(_S("%S %S").c_str(), indexField->getName().c_str(), order.c_str());
	}

	sql += _S(")");

	return true;
}

bool Parser::_generateBoolFieldSql(shared_ptr<DbSchemaField> field, String &sql)
{
	sql = String::format(_S("%S int(1)").c_str(), field->getName().c_str());

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
	{
		if(conversions::from_utf16<bool>(value))
			sql += _S(" default '1'");
		else
			sql += _S(" default '0'");
	}

	return true;
}

bool Parser::_generateIntFieldSql(shared_ptr<DbSchemaField> field, String &sql)
{
	sql = String::format(_S("%S int(%d)").c_str(), field->getName().c_str(), field->getSize());

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
		sql += String::format(_S(" default '%d'").c_str(), conversions::from_utf16<int32>(value));

	return true;
}

bool Parser::_generateDoubleFieldSql(shared_ptr<DbSchemaField> field, String &sql)
{
	sql = String::format(_S("%S real").c_str(), field->getName().c_str());

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
		sql += String::format(_S(" default '%g'").c_str(), conversions::from_utf16<double>(value));

	return true;
}

bool Parser::_generateVarcharFieldSql(shared_ptr<DbSchemaField> field, String &sql)
{
	sql = String::format(_S("%S varchar(%d)").c_str(), field->getName().c_str(), field->getSize());

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
		sql += String::format(_S(" default '%S'").c_str(), value.c_str());

	return true;
}

bool Parser::_generateTextFieldSql(shared_ptr<DbSchemaField> field, String &sql)
{
	sql = String::format(_S("%S text").c_str(), field->getName().c_str());

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
		sql += String::format(_S(" default '%S'").c_str(), value.c_str());

	return true;
}

bool Parser::_generateBlobFieldSql(shared_ptr<DbSchemaField> field, String &sql)
{
	sql = String::format(_S("%S blob").c_str(), field->getName().c_str());

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
		sql += String::format(_S(" default '%S'").c_str(), value.c_str());

	return true;
}
