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

//////////////////////////////////////////////////////////////////////

void Parser::parseCommand(sql::command_ptr command, String &sql)
{
	switch(command->getType())
	{
	case sql::ctSelect:			parseSelect(boost::dynamic_pointer_cast<sql::Select>(command), sql);
								break;

	case sql::ctInsert:			parseInsert(boost::dynamic_pointer_cast<sql::Insert>(command), sql);
								break;

	case sql::ctUpdate:			parseUpdate(boost::dynamic_pointer_cast<sql::Update>(command), sql);
								break;

	case sql::ctDelete:			parseDelete(boost::dynamic_pointer_cast<sql::Delete>(command), sql);
								break;

	case sql::ctCreateTable:	parseCreateTable(boost::dynamic_pointer_cast<sql::CreateTable>(command), sql);
								break;

	case sql::ctRenameTable:	parseRenameTable(boost::dynamic_pointer_cast<sql::RenameTable>(command), sql);
								break;

	case sql::ctDropTable:		parseDropTable(boost::dynamic_pointer_cast<sql::DropTable>(command), sql);  									
								break;

	case sql::ctCreateField:	parseCreateField(boost::dynamic_pointer_cast<sql::CreateField>(command), sql);  									
								break;

	case sql::ctCreateIndex:	parseCreateIndex(boost::dynamic_pointer_cast<sql::CreateIndex>(command), sql);  									
								break;

	case sql::ctDropIndex:		parseDropIndex(boost::dynamic_pointer_cast<sql::DropIndex>(command), sql);  									
								break;

	case sql::ctRebuildTable:	parseRebuildTable(boost::dynamic_pointer_cast<sql::RebuildTable>(command), sql);  									
								break;

	default:					OS_EXCEPTION("Unknown sql command");
								break;
	}
}

void Parser::parseSelect(sql::select_ptr command, String &sql)
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
		case sql::Select::ctNone:			OS_EXCEPTION("Invalid connection type");
											break;

		case sql::Select::ctUnion:			sql += _S(" union ") + right;
											break;

		case sql::Select::ctUnionAll:		sql += _S(" union all ") + right;
											break;

		case sql::Select::ctIntersect:		sql += _S(" intersect ") + right;
											break;

		case sql::Select::ctExcept:			sql += _S(" except ") + right;
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

void Parser::parseInsert(sql::insert_ptr command, String &sql)
{
	OS_EXCEPTION("Unimplemented");
}

void Parser::parseUpdate(sql::update_ptr command, String &sql)
{
	OS_EXCEPTION("Unimplemented");
}

void Parser::parseDelete(sql::delete_ptr command, String &sql)
{
	OS_EXCEPTION("Unimplemented");
}

void Parser::parseCreateTable(shared_ptr<sql::CreateTable> command, String &sql)
{
	parseCreateTable(command->getTable(), sql);
}

void Parser::parseRenameTable(shared_ptr<sql::RenameTable> command, String &sql)
{
	sql = String::format(_S("alter table %S rename to %S").c_str(), OS_MYSQL_ESCAPE(command->getOldTable()).c_str(), OS_MYSQL_ESCAPE(command->getNewTable()).c_str());
}

void Parser::parseCreateField(shared_ptr<sql::CreateField> command, String &sql)
{
	schema::field_ptr field = command->getField();

	String field_sql;
	_generateFieldSql(field, field_sql);

	sql = String::format(_S("alter table %S add %S").c_str(), field->getTableName().c_str(), field_sql.c_str());

	/*
	switch(command->getPosition())
	{
	case sql::CreateField::begin:	sql = String::format(_S("alter table %S add %S first").c_str(), OS_MYSQL_ESCAPE(field->getTableName()).c_str(), field_sql.c_str());
									break;

	case sql::CreateField::after:	sql = String::format(_S("alter table %S add %S after %S").c_str(), OS_MYSQL_ESCAPE(field->getTableName()).c_str(), field_sql.c_str(), OS_MYSQL_ESCAPE(command->getPrevField()->getName()));
									break;

	case sql::CreateField::end:		sql = String::format(_S("alter table %S add %S").c_str(), OS_MYSQL_ESCAPE(field->getTableName()).c_str(), field_sql.c_str());
									break;

	default:						OS_EXCEPTION(_S("Unknown field"));
									break;
	}
	*/
}

void Parser::parseCreateIndex(shared_ptr<sql::CreateIndex> command, String &sql)
{
	_generateIndexSql(command->getIndex(), sql);
}

void Parser::parseDropIndex(shared_ptr<sql::DropIndex> command, String &sql)
{
	db::schema::index_ptr index = command->getIndex();
	OS_ASSERT(index != nullptr);

	sql = _S("drop index ") + OS_MYSQL_ESCAPE(index->getName()) + _S(" on ") + OS_MYSQL_ESCAPE(index->getTableName());
}

void Parser::parseRebuildTable(shared_ptr<sql::RebuildTable> command, String &sql)
{
	schema::table_ptr newTable = command->getNewTable();
	schema::table_ptr oldTable = command->getOldTable();

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
	const sql::RebuildTable::ImportFields &importFields = command->getImportFields();
	for(sql::RebuildTable::ImportFields::const_iterator i = importFields.begin(); i != importFields.end(); ++i)
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

void Parser::parseDropTable(shared_ptr<sql::DropTable> command, String &sql)
{
	sql = _S("drop table ") + OS_MYSQL_ESCAPE(command->getTable());
}

void Parser::parseFields(const sql::Fields &fields, String &sql)
{
	for(sql::Fields::const_iterator i = fields.begin(); i != fields.end(); ++i)
	{
		const sql::Field &field = *i;

		if(sql.empty() == false)
			sql += _S(", ");
		
		sql += parseField(*i);
	}
}

String Parser::parseField(const sql::Field &field)
{
	String field_name = field.formatName();
	if(field.hasAlias())
		field_name += _S(" as ") + field.getAlias();

	String value;

	switch(field.getAttribute())
	{
	case sql::Field::faNone:	value = field_name;
								break;

	case sql::Field::faCount:	value = String::format(_S("count(%S)").c_str(), field_name.c_str());
								break;

	case sql::Field::faMin:		value = String::format(_S("min(%S)").c_str(), field_name.c_str());
								break;

	case sql::Field::faMax:		value = String::format(_S("max(%S)").c_str(), field_name.c_str());
								break;

	default:					OS_EXCEPTION("Unknown field attribute");
								break;
	}

	switch(field.getOrder())
	{
	case sql::Field::foNone:	break;

	case sql::Field::foAsc:		value += _S(" asc");
								break;

	case sql::Field::foDesc:	value += _S(" desc");
								break;
	}

	return value;
}

void Parser::parseTables(const sql::Tables &tables, String &sql)
{
    for(sql::Tables::const_iterator i = tables.begin(); i != tables.end(); ++i)
	{
		const sql::Table &table = *i;

		if(sql.empty() == false)
			sql += _S(", ");

		sql += table.getName();

		const String &suggestedIndex = table.getSuggestedIndex();
		if(suggestedIndex.empty() == false)
			sql += " use index (" + suggestedIndex + ")";
	}
}

void Parser::parseWhere(const sql::Where &where, String &sql)
{
	parseConditions(where.conditions, sql);
}

void Parser::parseConditions(const sql::Conditions &conditions, String &sql)
{
	for(sql::Conditions::const_iterator i = conditions.begin(); i != conditions.end(); ++i)	
	{
		const sql::Condition &condition = *i;

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

void Parser::parseGroupBy(const sql::GroupBy &groupBy, String &sql)
{
	parseFields(groupBy.fields, sql);
}

void Parser::parseOrderBy(const sql::OrderBy &orderBy, String &sql)
{
	parseFields(orderBy.fields, sql);
}

void Parser::parseLimit(const sql::Limit &limit, String &sql)
{
	if(limit.exists())
	{
		if(limit.hasOffset())
			sql = String::format(_S("%d, %d").c_str(), limit.getOffset(), limit.getCount());
		else
			sql = conversions::to_utf16(limit.getCount());
	}
}

bool Parser::parseCreateTable(schema::table_ptr table, String &sql)
{
	sql = _S("CREATE TABLE ") + OS_MYSQL_ESCAPE(table->getName()) + _S(" (\r\n");

	String primary_key;

	const schema::Fields &fields = table->getFields();
	for(schema::Fields::const_iterator i = fields.begin(); i != fields.end(); ++i)
	{
		schema::field_ptr field = *i;

		String field_sql;
		if(_generateFieldSql(field, field_sql) == false)
			return false;
	
		sql += field_sql;
		sql += _S(",\r\n");

		if(field->getPrimary())
		{
			if(primary_key.empty())
				primary_key = _S("PRIMARY KEY (") + OS_MYSQL_ESCAPE(field->getName());
			else
				primary_key += _S(", ") + OS_MYSQL_ESCAPE(field->getName());
		}
	}

	if(primary_key.empty() == false)
	{
		primary_key += _S("),\r\n");
		sql += primary_key;
	}

	const schema::Indexes &indexes = table->getIndexes();
	if(indexes.empty() == false)
	{
		for(schema::Indexes::const_iterator ii = indexes.begin(); ii != indexes.end(); ++ii)
		{
			schema::index_ptr index = *ii;

			const schema::IndexFields &indexFields = index->getFields();
			if(indexFields.empty() == false)
			{
				if(index->getUnique())
					sql += _S("UNIQUE KEY ");
				else
					sql += _S("KEY ");

				sql += OS_MYSQL_ESCAPE(index->getName()) + _S(" (");

				for(schema::IndexFields::const_iterator ifi = indexFields.begin(); ifi != indexFields.end(); ++ifi)
				{
					schema::index_field_ptr indexField = *ifi;
					
					if(ifi != indexFields.begin())
						sql += _S(", ");

					sql += OS_MYSQL_ESCAPE(indexField->getName());

					switch(indexField->getOrder())
					{
					case schema::IndexField::ifoAsc:		sql += _S(" ASC");
															break;

					case schema::IndexField::ifoDesc:		sql += _S(" DESC");
															break;

					default:								OS_EXCEPTION("Unknown index field order");
															break;
					}
				}

				sql += _S("),\r\n");
			}
		}
	}
	
	sql.trim_right(_S(",\r\n"));
	sql += _S("\r\n) ENGINE=MyISAM DEFAULT CHARSET=utf8;");

	return true;
}

bool Parser::_generateFieldSql(schema::field_ptr field, String &sql)
{
	switch(field->getType())
	{
	case schema::ftUnknown:		return false;
									
	case schema::ftBool:		return _generateBoolFieldSql(field, sql);

	case schema::ftInt:			return _generateIntFieldSql(field, false, sql);

	case schema::ftUint:		return _generateIntFieldSql(field, true, sql);

	case schema::ftDouble:		return _generateDoubleFieldSql(field, sql);

	case schema::ftVarchar:		return _generateVarcharFieldSql(field, sql);

	case schema::ftText:		return _generateTextFieldSql(field, sql);

	case schema::ftBlob:		return _generateBlobFieldSql(field, sql);

	default:					OS_ASSERTFALSE();
								break;
	}

	return false;
}

bool Parser::_generateIndexSql(schema::index_ptr index, String &sql)
{
	OS_ASSERT(index != nullptr);

	const db::schema::IndexFields &indexFields = index->getFields();
	if(indexFields.empty())
		return false;

	if(index->getUnique())
		sql = _S("create unique index");
	else
		sql = _S("create index");

	sql += String::format(_S(" %S on %S (").c_str(), OS_MYSQL_ESCAPE(index->getName()).c_str(), OS_MYSQL_ESCAPE(index->getTableName()).c_str());
	
	for(db::schema::IndexFields::const_iterator i = indexFields.begin(); i != indexFields.end(); ++i)
	{
		if(i != indexFields.begin())
		{
			sql += _S(", ");
		}

		db::schema::index_field_ptr indexField = *i;

		String order;
		switch(indexField->getOrder())
		{
		case db::schema::IndexField::ifoAsc:		order = _S("asc");
													break;

		case db::schema::IndexField::ifoDesc:		order = _S("desc");
													break;

		default:									OS_ASSERTFALSE();
													break;
		}

		sql += String::format(_S("%S %S").c_str(), OS_MYSQL_ESCAPE(indexField->getName()).c_str(), order.c_str());
	}

	sql += _S(")");

	return true;
}

bool Parser::_generateBoolFieldSql(schema::field_ptr field, String &sql)
{
	sql = String::format(_S("%S tinyint unsigned").c_str(), OS_MYSQL_ESCAPE(field->getName()).c_str());

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

bool Parser::_generateIntFieldSql(schema::field_ptr field, bool unsigned_type, String &sql)
{
	String type;
	switch(field->getSize())
	{
	case 1:		type = _S("tinyint");
				break;

	case 2:		type = _S("smallint");
				break;

	default:	type = _S("int");
				break;
	}

	sql = String::format(_S("%S %S").c_str(), OS_MYSQL_ESCAPE(field->getName()).c_str(), type.c_str());

	if(unsigned_type)
		sql += _S(" unsigned");

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
		sql += String::format(_S(" default '%d'").c_str(), unsigned_type ? conversions::from_utf16<uint32>(value) : conversions::from_utf16<int32>(value));

	return true;
}

bool Parser::_generateDoubleFieldSql(schema::field_ptr field, String &sql)
{
	sql = String::format(_S("%S double").c_str(), OS_MYSQL_ESCAPE(field->getName()).c_str());

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
		sql += String::format(_S(" default '%g'").c_str(), conversions::from_utf16<double>(value));

	return true;
}

bool Parser::_generateVarcharFieldSql(schema::field_ptr field, String &sql)
{
	sql = String::format(_S("%S varchar(%d)").c_str(), OS_MYSQL_ESCAPE(field->getName()).c_str(), field->getSize());

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
		sql += String::format(_S(" default '%S'").c_str(), value.c_str());

	return true;
}

bool Parser::_generateTextFieldSql(schema::field_ptr field, String &sql)
{
	sql = String::format(_S("%S text").c_str(), OS_MYSQL_ESCAPE(field->getName()).c_str());

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
		sql += String::format(_S(" default '%S'").c_str(), value.c_str());

	return true;
}

bool Parser::_generateBlobFieldSql(schema::field_ptr field, String &sql)
{
	sql = String::format(_S("%S longblob").c_str(), OS_MYSQL_ESCAPE(field->getName()).c_str());

	if(field->getNull() == false)
		sql += _S(" NOT null");

	String value = field->getDefault();
	if(value.empty() == false)
		sql += String::format(_S(" default '%S'").c_str(), value.c_str());

	return true;
}
