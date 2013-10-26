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

#ifndef _DB_SCHEMA_H
#define _DB_SCHEMA_H

#include "dbdb.h"
#include "enable_this_ptr.h"
#include "base/object.h"
#include "version.h"

//////////////////////////////////////////////////////////////////////

#define OS_SCHEMA_DB					_S("db_razor.xsd")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport DbSchemaSchema : public Object,
							public enable_this_ptr<DbSchemaSchema>
{
// Construction
public:
	DbSchemaSchema();
	virtual ~DbSchemaSchema();

// Attributes
public:
	inline const Version & getVersion() const;
	inline const Version & getPrevVersion() const;

	inline const DbSchemaTables & getTables() const;
	inline uint32 getTablesCount() const;

	inline bool hasTable(const String &name, bool checkOldName = false) const;

	shared_ptr<DbSchemaTable> getTable(const String &name, bool checkOldName = false) const;

// Operations
public:
	// Carica uno schema da un file xml
	bool parse(const String &filename);
	// Genera i comandi necessari per ricreare il nuovo schema a partire da quello corrente
	bool generateDifferences(shared_ptr<DbSchemaSchema> new_schema, DbSqlCommands &commands);

private:
	void _addTable(shared_ptr<DbSchemaTable> table);
	bool _parseSchema(shared_ptr<XMLNode> schema);
	bool _parseOptions(shared_ptr<XMLNode> schema);
	bool _parseTables(shared_ptr<XMLNode> tables);
	bool _parseTable(shared_ptr<XMLNode> table);

protected:
	void clear();

public:
	static const String SCHEMA;
	static const String TABLES;
	static const String TABLE;
	static const String VERSION;
	static const String PREV_VERSION;

protected:
	Version m_version;
	Version m_prevVersion;
	DbSchemaTables m_tables;
};

//////////////////////////////////////////////////////////////////////

inline const Version & DbSchemaSchema::getVersion() const { return m_version; }
inline const Version & DbSchemaSchema::getPrevVersion() const { return m_prevVersion; }

inline const DbSchemaTables & DbSchemaSchema::getTables() const { return m_tables; }
inline uint32 DbSchemaSchema::getTablesCount() const { return static_cast<uint32>(m_tables.size()); }

inline bool DbSchemaSchema::hasTable(const String &name, bool checkOldName) const { return getTable(name, checkOldName) != null; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_SCHEMA_H
