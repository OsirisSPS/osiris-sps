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

#ifndef _DB_TABLE_H
#define _DB_TABLE_H

#include "dbdb.h"
#include "enable_this_ptr.h"
#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport DbSchemaTable : public Object,
						   public enable_this_ptr<DbSchemaTable>
{
// Construction
public:
	DbSchemaTable(shared_ptr<DbSchemaSchema> schema = null);
	virtual ~DbSchemaTable();

// Attributes
public:
	inline shared_ptr<DbSchemaSchema> getSchema() const;

	inline const String & getName() const;
	inline const String & getOldName() const;
	inline bool wasRenamed() const;

	inline const DbSchemaFields & getFields() const;
	inline uint32 getFieldsCount() const;

	inline const DbSchemaIndexes & getIndexes() const;
	inline uint32 getIndexesCount() const;

	inline bool hasField(const String &name, bool checkOldName = false) const;
	inline bool hasIndex(const String &name) const;

	shared_ptr<DbSchemaField> getField(const String &name, bool checkOldName = false) const;
	shared_ptr<DbSchemaIndex> getIndex(const String &name) const;

// Operations
public:
	// Carica la struttura della tabella a partire da un nodo xml
	bool parse(shared_ptr<XMLNode> table);
	// Genera i comandi necessari per ricreare la nuova tabella a partire da quella corrente
	bool generateDifferences(shared_ptr<DbSchemaTable> new_table, DbSqlCommands &commands);

private:
	void _addField(shared_ptr<DbSchemaField> field);
	void _addIndex(shared_ptr<DbSchemaIndex> index);

	bool _parseFields(shared_ptr<XMLNode> nodeFields);
	bool _parseField(shared_ptr<XMLNode> nodeField);
	bool _parseIndexes(shared_ptr<XMLNode> nodeIndexes);
	bool _parseIndex(shared_ptr<XMLNode> nodeIndex);

protected:
	void clear();

public:
	static const String NAME;
	static const String OLD_NAME;
	static const String FIELDS;
	static const String FIELD;
	static const String INDEXES;
	static const String INDEX;

protected:
	weak_ptr<DbSchemaSchema> m_schema;
	String m_name;
	String m_oldname;
	DbSchemaFields m_fields;
	DbSchemaIndexes m_indexes;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DbSchemaSchema> DbSchemaTable::getSchema() const { return m_schema.lock(); }

inline const String & DbSchemaTable::getName() const { return m_name; }
inline const String & DbSchemaTable::getOldName() const { return m_oldname; }
inline bool DbSchemaTable::wasRenamed() const { return m_oldname.empty() == false; }

inline const DbSchemaFields & DbSchemaTable::getFields() const { return m_fields; }
inline uint32 DbSchemaTable::getFieldsCount() const { return static_cast<uint32>(m_fields.size()); }

inline const DbSchemaIndexes & DbSchemaTable::getIndexes() const { return m_indexes; }
inline uint32 DbSchemaTable::getIndexesCount() const { return static_cast<uint32>(m_indexes.size()); }

inline bool DbSchemaTable::hasField(const String &name, bool checkOldName) const { return getField(name, checkOldName) != null; }
inline bool DbSchemaTable::hasIndex(const String &name) const { return getIndex(name) != null; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_TABLE_H
