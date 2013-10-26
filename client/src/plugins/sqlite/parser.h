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

#ifndef _SQLITE_PARSER_H
#define _SQLITE_PARSER_H

#include "dbsqlfield.h"
#include "dbsqlcondition.h"
#include "dbsqltable.h"

//////////////////////////////////////////////////////////////////////

namespace sqlite {

//////////////////////////////////////////////////////////////////////

class Parser
{
// Operations
public:
	static void parseCommand(shared_ptr<DbSqlICommand> command, String &sql);

	static void parseSelect(shared_ptr<DbSqlSelect> command, String &sql);
	static void parseInsert(shared_ptr<DbSqlInsert> command, String &sql);
	static void parseUpdate(shared_ptr<DbSqlUpdate> command, String &sql);
	static void parseDelete(shared_ptr<DbSqlDelete> command, String &sql);
	static void parseCreateTable(shared_ptr<DbSqlCreateTable> command, String &sql);
	static void parseRenameTable(shared_ptr<DbSqlRenameTable> command, String &sql);
	static void parseDropTable(shared_ptr<DbSqlDropTable> command, String &sql);
	static void parseCreateField(shared_ptr<DbSqlCreateField> command, String &sql);
	static void parseCreateIndex(shared_ptr<DbSqlCreateIndex> command, String &sql);
	static void parseDropIndex(shared_ptr<DbSqlDropIndex> command, String &sql);
	static void parseRebuildTable(shared_ptr<DbSqlRebuildTable> command, String &sql);

	static void parseFields(const DbSqlFields &fields, String &sql);
	static String parseField(const DbSqlField &field);
	static void parseTables(const DbSqlTables &tables, String &sql);
	static void parseWhere(const DbSqlWhere &where, String &sql);
	static void parseConditions(const DbSqlConditions &conditions, String &sql);
	static void parseGroupBy(const DbSqlGroupBy &groupBy, String &sql);
	static void parseOrderBy(const DbSqlOrderBy &orderBy, String &sql);
	static void parseLimit(const DbSqlLimit &limit, String &sql);

	static bool parseCreateTable(shared_ptr<DbSchemaTable> table, String &sql);

private:
	static bool _generateFieldSql(shared_ptr<DbSchemaField> field, String &sql);
	static bool _generateIndexSql(shared_ptr<DbSchemaIndex> index, String &sql);
	static bool _generateBoolFieldSql(shared_ptr<DbSchemaField> field, String &sql);
	static bool _generateIntFieldSql(shared_ptr<DbSchemaField> field, String &sql);
	static bool _generateDoubleFieldSql(shared_ptr<DbSchemaField> field, String &sql);
	static bool _generateVarcharFieldSql(shared_ptr<DbSchemaField> field, String &sql);
	static bool _generateTextFieldSql(shared_ptr<DbSchemaField> field, String &sql);
	static bool _generateBlobFieldSql(shared_ptr<DbSchemaField> field, String &sql);
};

//////////////////////////////////////////////////////////////////////

} // sqlite

//////////////////////////////////////////////////////////////////////

#endif // _SQLITE_PARSER_H
