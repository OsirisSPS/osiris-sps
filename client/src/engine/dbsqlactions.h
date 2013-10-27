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

#ifndef _DB_SQL_ACTIONS_H
#define _DB_SQL_ACTIONS_H

#include "dbsqlicommand.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport DbSqlCreateTable : public DbSqlICommand
{
// Construction
public:
	DbSqlCreateTable(shared_ptr<DbSchemaTable> table);
	virtual ~DbSqlCreateTable();

// Attributes
public:
	inline shared_ptr<DbSchemaTable> getTable() const;

private:
	shared_ptr<DbSchemaTable> m_table;		// Struttura della nuova tabella
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DbSchemaTable> DbSqlCreateTable::getTable() const { return m_table; }

//////////////////////////////////////////////////////////////////////

class EngineExport DbSqlRenameTable : public DbSqlICommand
{
// Construction
public:
	DbSqlRenameTable(const String &oldTable, const String &newTable);
	virtual ~DbSqlRenameTable();

// Attributes
public:
	inline const String & getOldTable() const;
	inline const String & getNewTable() const;

private:
	String m_oldTable;
	String m_newTable;
};

//////////////////////////////////////////////////////////////////////

inline const String & DbSqlRenameTable::getOldTable() const { return m_oldTable; }
inline const String & DbSqlRenameTable::getNewTable() const { return m_newTable; }

//////////////////////////////////////////////////////////////////////

class EngineExport DbSqlDropTable : public DbSqlICommand
{
// Construction
public:
	DbSqlDropTable(const String &table);
	virtual ~DbSqlDropTable();

// Attributes
public:
	inline const String & getTable() const;

private:
	String m_table;
};

//////////////////////////////////////////////////////////////////////

inline const String & DbSqlDropTable::getTable() const { return m_table; }

//////////////////////////////////////////////////////////////////////

class EngineExport DbSqlCreateField : public DbSqlICommand
{
// Construction
public:
	DbSqlCreateField(shared_ptr<DbSchemaField> field);
	virtual ~DbSqlCreateField();

// Attributes
public:
	inline shared_ptr<DbSchemaField> getField() const;

private:
	shared_ptr<DbSchemaField> m_field;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DbSchemaField> DbSqlCreateField::getField() const { return m_field; }

//////////////////////////////////////////////////////////////////////

class EngineExport DbSqlCreateIndex : public DbSqlICommand
{
// Construction
public:
	DbSqlCreateIndex(shared_ptr<DbSchemaIndex> index);
	virtual ~DbSqlCreateIndex();

// Attributes
public:
	inline shared_ptr<DbSchemaIndex> getIndex() const;

private:
	shared_ptr<DbSchemaIndex> m_index;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DbSchemaIndex> DbSqlCreateIndex::getIndex() const { return m_index; }

//////////////////////////////////////////////////////////////////////

class EngineExport DbSqlDropIndex : public DbSqlICommand
{
// Construction
public:
	DbSqlDropIndex(shared_ptr<DbSchemaIndex> index);
	virtual ~DbSqlDropIndex();

// Attributes
public:
	inline shared_ptr<DbSchemaIndex> getIndex() const;

private:
	shared_ptr<DbSchemaIndex> m_index;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DbSchemaIndex> DbSqlDropIndex::getIndex() const { return m_index; }

//////////////////////////////////////////////////////////////////////

class EngineExport DbSqlRebuildTable : public DbSqlICommand
{
// Types
public:
	typedef list<StringPair>::type ImportFields;

// Construction
public:
	DbSqlRebuildTable(shared_ptr<DbSchemaTable> new_table, shared_ptr<DbSchemaTable> old_table);
	virtual ~DbSqlRebuildTable();

// Attributes
public:
	inline shared_ptr<DbSchemaTable> getNewTable() const;
	inline shared_ptr<DbSchemaTable> getOldTable() const;
	inline const ImportFields & getImportFields() const;

// Operations
public:
	void importField(const String &newField, const String &oldField);

private:
	shared_ptr<DbSchemaTable> m_newTable;			// Struttura della nuova tabella
	shared_ptr<DbSchemaTable> m_oldTable;			// Struttura della vecchia tabella
	ImportFields m_importFields;			// Dati da importare nella nuova tabella
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DbSchemaTable> DbSqlRebuildTable::getNewTable() const { return m_newTable; }
inline shared_ptr<DbSchemaTable> DbSqlRebuildTable::getOldTable() const { return m_oldTable; }
inline const DbSqlRebuildTable::ImportFields & DbSqlRebuildTable::getImportFields() const { return m_importFields; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_SQL_ACTIONS_H
