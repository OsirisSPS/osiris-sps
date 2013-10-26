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

#ifndef _DB_DB_H
#define _DB_DB_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// Tipo di campo
enum DbSchemaFieldType
{
	ftUnknown	= 0,		// non valido (utilizzo interno)
	ftBool		= 1,		// bool
	ftInt		= 2,		// intero
	ftUint		= 3,		// interso senza segno
	ftDouble	= 4,		// double
	ftVarchar	= 5,		// testo dimensione predefinata
	ftText		= 6,		// testo dimensione variabile
	ftBlob		= 7,		// binario dimensione variabile
};

// Classes
class DbSchemaField;
class DbSchemaIndex;
class DbSchemaIndexField;
class DbSchemaSchema;
class DbSchemaTable;

// Types
typedef list<shared_ptr<DbSchemaField> >::type DbSchemaFields;
typedef list<shared_ptr<DbSchemaIndex> >::type DbSchemaIndexes;
typedef deque<shared_ptr<DbSchemaIndexField> >::type DbSchemaIndexFields;
typedef list<shared_ptr<DbSchemaTable> >::type DbSchemaTables;


// Enumerated types
enum DbSqlCommandType
{
	ctSelect		= 0,
	ctInsert		= 1,
	ctUpdate		= 2,
	ctDelete		= 3,
	ctCreateTable	= 4,
	ctRenameTable	= 5,
	ctDropTable		= 6,
	ctCreateField	= 7,
	ctCreateIndex	= 8,
	ctDropIndex		= 9,
	ctRebuildTable	= 10,
};

// Interfaces
class DbSqlICommand;

// Classes
class DbSqlCondition;
class DbSqlCreateField;
class DbSqlCreateIndex;
class DbSqlCreateTable;
class DbSqlDelete;
class DbSqlDropIndex;
class DbSqlDropTable;
class DbSqlField;
class DbSqlGroupBy;
class DbSqlInsert;
class DbSqlLimit;
class DbSqlOrderBy;
class DbSqlRenameTable;
class DbSqlRebuildTable;
class DbSqlSelect;
class DbSqlUpdate;
class DbSqlTable;
class DbSqlValues;
class DbSqlWhere;

template <typename T> class DbSqlCollection;

// Types
typedef list<shared_ptr<DbSqlICommand> >::type DbSqlCommands;

// Classes
class DbBinder;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_DB_H
