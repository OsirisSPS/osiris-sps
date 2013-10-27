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

#ifndef _MYSQL_PARSER_H
#define _MYSQL_PARSER_H

#include "dbsqlcondition.h"
#include "dbsqlfield.h"
#include "dbsqltable.h"

//////////////////////////////////////////////////////////////////////

namespace mysql {

//////////////////////////////////////////////////////////////////////

class Parser
{
// Operations
public:
	static void parseCommand(sql::command_ptr command, String &sql);

	static void parseSelect(sql::select_ptr command, String &sql);
	static void parseInsert(sql::insert_ptr command, String &sql);
	static void parseUpdate(sql::update_ptr command, String &sql);
	static void parseDelete(sql::delete_ptr command, String &sql);
	static void parseCreateTable(shared_ptr<sql::CreateTable> command, String &sql);
	static void parseRenameTable(shared_ptr<sql::RenameTable> command, String &sql);
	static void parseDropTable(shared_ptr<sql::DropTable> command, String &sql);
	static void parseCreateField(shared_ptr<sql::CreateField> command, String &sql);
	static void parseCreateIndex(shared_ptr<sql::CreateIndex> command, String &sql);
	static void parseDropIndex(shared_ptr<sql::DropIndex> command, String &sql);
	static void parseRebuildTable(shared_ptr<sql::RebuildTable> command, String &sql);

	static void parseFields(const sql::Fields &fields, String &sql);
	static String parseField(const sql::Field &field);
	static void parseTables(const sql::Tables &tables, String &sql);
	static void parseWhere(const sql::Where &where, String &sql);
	static void parseConditions(const sql::Conditions &conditions, String &sql);
	static void parseGroupBy(const sql::GroupBy &groupBy, String &sql);
	static void parseOrderBy(const sql::OrderBy &orderBy, String &sql);
	static void parseLimit(const sql::Limit &limit, String &sql);

	static bool parseCreateTable(schema::table_ptr table, String &sql);

private:
	static bool _generateFieldSql(schema::field_ptr field, String &sql);
	static bool _generateIndexSql(schema::index_ptr index, String &sql);
	static bool _generateBoolFieldSql(schema::field_ptr field, String &sql);
	static bool _generateIntFieldSql(schema::field_ptr field, bool unsigned_type, String &sql);
	static bool _generateDoubleFieldSql(schema::field_ptr field, String &sql);
	static bool _generateVarcharFieldSql(schema::field_ptr field, String &sql);
	static bool _generateTextFieldSql(schema::field_ptr field, String &sql);
	static bool _generateBlobFieldSql(schema::field_ptr field, String &sql);
};

//////////////////////////////////////////////////////////////////////

} // mysql

//////////////////////////////////////////////////////////////////////

#endif // _MYSQL_PARSER_H
