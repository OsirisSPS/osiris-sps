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

#ifndef _DB_INDEXFIELD_H
#define _DB_INDEXFIELD_H

#include "base/object.h"
#include "dbdb.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport DbSchemaIndexField : public Object
{
// Enumerated types
public:
	enum Order
	{
		ifoAsc		= 0,
		ifoDesc		= 1,
		ifoDefault	= ifoAsc,
	};

// Construction
public:
	DbSchemaIndexField(shared_ptr<DbSchemaIndex> table = nullptr);
	virtual ~DbSchemaIndexField();

// Attributes
public:
	inline shared_ptr<DbSchemaIndex> getIndex() const;

	inline const String & getName() const;
	inline Order getOrder() const;

// Operations
public:
	// Carica la struttura di un campo a partire da un nodo xml
	bool parse(shared_ptr<XMLNode> nodeField);
	// Effettua il confronto con un altro campo (restituisce true se sono identici)
	bool compare(shared_ptr<DbSchemaIndexField> second);

private:
    bool _parseName(const String &value);
	bool _parseOrder(const String &value);

public:
	static const String NAME;
	static const String ORDER;

	static const String ORDER_ASC;
	static const String ORDER_DESC;

private:
	weak_ptr<DbSchemaIndex> m_index;
	String m_name;
	Order m_order;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DbSchemaIndex> DbSchemaIndexField::getIndex() const { return m_index.lock(); }

inline const String & DbSchemaIndexField::getName() const { return m_name; }
inline DbSchemaIndexField::Order DbSchemaIndexField::getOrder() const { return m_order; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_INDEXFIELD_H
