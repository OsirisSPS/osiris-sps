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

#ifndef _DB_FIELD_H
#define _DB_FIELD_H

#include "dbdb.h"
#include "enable_this_ptr.h"
#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport DbSchemaField : public Object,
						   public enable_this_ptr<DbSchemaField>
{
// Construction
public:
	DbSchemaField(shared_ptr<DbSchemaTable> table = null);
	virtual ~DbSchemaField();

// Attributes
public:
	inline shared_ptr<DbSchemaTable> getTable() const;

	// Restituisce il nome del campo
	inline const String & getName() const;
	// Restituisce il vecchio nome del campo
    inline const String & getOldName() const;
	// Restituisce la tipologia di campo
	inline DbSchemaFieldType getType() const;
	// Definisce la dimensione del campo in relazione alla sua tipologia (ftInt, ftVarchar, ecc...)
	inline uint32 getSize() const;
	// Indica se il campo ammette valori nulli
	inline bool getNull() const;
	// Indica il valore di default del campo (in relazione alla tipologia)
	inline const String & getDefault() const;
	// Indica se il campo è una chiave primaria
	inline bool getPrimary() const;

	inline bool wasRenamed() const;

	const String & getTableName() const;

// Operations
public:
	// Carica la struttura di un campo a partire da un nodo xml
	bool parse(shared_ptr<XMLNode> nodeField);
	// Effettua il confronto con un altro campo (restituisce true se sono identici)
	bool compare(shared_ptr<DbSchemaField> second);

private:
    bool _parseName(const String &value);
	bool _parseOldName(const String &value);
	bool _parseType(const String &value);
	bool _parseSize(const String &value);
	bool _parseNull(const String &value);
	bool _parseDefault(const String &value);
	bool _parsePrimary(const String &value);

public:
	static const String NAME;
	static const String OLD_NAME;
	static const String TYPE;
	static const String SIZE;
	static const String ALLOW_NULL;
	static const String DEFAULT;
	static const String PRIMARY;

	static const String TYPE_BOOL;
	static const String TYPE_INT;
	static const String TYPE_UINT;
	static const String TYPE_DOUBLE;
	static const String TYPE_VARCHAR;
	static const String TYPE_TEXT;
	static const String TYPE_BLOB;

protected:
	weak_ptr<DbSchemaTable> m_table;
	String m_name;
	String m_oldname;
	DbSchemaFieldType m_type;
	uint32 m_size;
	bool m_null;
	String m_default;
	bool m_primary;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DbSchemaTable> DbSchemaField::getTable() const { return m_table.lock(); }

inline const String & DbSchemaField::getName() const { return m_name; }
inline const String & DbSchemaField::getOldName() const { return m_oldname; }
inline DbSchemaFieldType DbSchemaField::getType() const { return m_type; }
inline uint32 DbSchemaField::getSize() const { return m_size; }
inline bool DbSchemaField::getNull() const { return m_null; }
inline const String & DbSchemaField::getDefault() const { return m_default; }
inline bool DbSchemaField::getPrimary() const { return m_primary; }

inline bool DbSchemaField::wasRenamed() const { return m_oldname.empty() == false; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_FIELD_H
