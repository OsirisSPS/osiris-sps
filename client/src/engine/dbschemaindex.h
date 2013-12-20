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

#ifndef _DB_INDEX_H
#define _DB_INDEX_H

#include "dbdb.h"
#include "enable_this_ptr.h"
#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport DbSchemaIndex : public Object,
						   public enable_this_ptr<DbSchemaIndex>
{
// Construction
public:
	DbSchemaIndex(shared_ptr<DbSchemaTable> table = nullptr);
	virtual ~DbSchemaIndex();

// Attributes
public:
	inline shared_ptr<DbSchemaTable> getTable() const;

	// Restituisce il nome dell'indice
	inline const String & getName() const;
	// Indica se l'indice è unico
	inline bool getUnique() const;

	// Restitusce i campi dell'indice
	inline const DbSchemaIndexFields & getFields() const;
	inline uint32 getFieldsCount() const;

	inline bool hasField(const String &name) const;

	shared_ptr<DbSchemaIndexField> getField(const String &name) const;

	const String & getTableName() const;

// Operations
public:
	// Carica la struttura di un campo a partire da un nodo xml
	bool parse(shared_ptr<XMLNode> field);
	// Effettua il confronto con un altro indice (restituisce true se sono identici)
	bool compare(shared_ptr<DbSchemaIndex> second);

private:
	void _addField(shared_ptr<DbSchemaIndexField> field);

    bool _parseName(const String &value);
	bool _parseUnique(const String &value);
	bool _parseFields(shared_ptr<XMLNode> nodeIndex);
	bool _parseField(shared_ptr<XMLNode> nodeField);
	
	static String _generateIndexName(shared_ptr<DbSchemaTable> table, const DbSchemaIndexFields &fields);

public:
	static const String NAME;
	static const String UNIQUE;
	static const String FIELDS;
	static const String FIELD;

protected:
	weak_ptr<DbSchemaTable> m_table;
	String m_name;
	bool m_unique;	
	DbSchemaIndexFields m_fields;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DbSchemaTable> DbSchemaIndex::getTable() const { return m_table.lock(); }

inline const String & DbSchemaIndex::getName() const { return m_name; }
inline bool DbSchemaIndex::getUnique() const { return m_unique; }

inline const DbSchemaIndexFields & DbSchemaIndex::getFields() const { return m_fields; }
inline uint32 DbSchemaIndex::getFieldsCount() const { return static_cast<uint32>(m_fields.size()); }

inline bool DbSchemaIndex::hasField(const String &name) const { return getField(name) != nullptr; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_INDEX_H
