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

#ifndef _DB_SQL_FIELD_H
#define _DB_SQL_FIELD_H

#include "base/object.h"
#include "dbdb.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DatabaseExport DbSqlField : public Object
{
    typedef Object BaseClass;

public:
	enum Attribute
	{
		faNone		= 0,
		faCount		= 1,
		faMin		= 2,
		faMax		= 3,
	};

	enum Order
	{
		foNone		= 0,
		foAsc		= 1,
		foDesc		= 2,
	};

// Construction
public:
	DbSqlField(const String &name = String::EMPTY, const String &table = String::EMPTY, const String &alias = String::EMPTY);
	DbSqlField(const DbSqlField &second);
	virtual ~DbSqlField();

// Attributes
public:
	inline const String & getName() const;
	inline void setName(const String &name);

	inline const String & getTable() const;
	inline void setTable(const String &table);
	inline bool hasTable() const;

	inline const String & getAlias() const;
	inline void setAlias(const String &alias);
	inline bool hasAlias() const;

	inline Attribute getAttribute() const;
	inline void setAttribute(Attribute attribute);
	inline bool hasAttribute() const;

	inline Order getOrder() const;
	inline void setOrder(Order order);
	inline bool hasOrder() const;

// Operations
public:
	String formatName() const;

// Operators
public:
	DbSqlField & operator =(const DbSqlField &second);

	bool operator ==(const DbSqlField &second);
	bool operator !=(const DbSqlField &second);

protected:
	String m_name;
	String m_table;
	String m_alias;
	Attribute m_attribute;
	Order m_order;
};

//////////////////////////////////////////////////////////////////////

typedef DbSqlCollection<DbSqlField> DbSqlFields;

//////////////////////////////////////////////////////////////////////

inline const String & DbSqlField::getName() const { return m_name; }
inline void DbSqlField::setName(const String &name) { m_name = name; }

inline const String & DbSqlField::getTable() const { return m_table; }
inline void DbSqlField::setTable(const String &table) { m_table = table; }
inline bool DbSqlField::hasTable() const { return m_table.empty() == false; }

inline const String & DbSqlField::getAlias() const { return m_alias; }
inline void DbSqlField::setAlias(const String &alias) { m_alias = alias; }
inline bool DbSqlField::hasAlias() const { return m_alias.empty() == false; }

inline DbSqlField::Attribute DbSqlField::getAttribute() const { return m_attribute; }
inline void DbSqlField::setAttribute(Attribute attribute) { m_attribute = attribute; }
inline bool DbSqlField::hasAttribute() const { return m_attribute != faNone; }

inline DbSqlField::Order DbSqlField::getOrder() const { return m_order; }
inline void DbSqlField::setOrder(Order order) { m_order = order; }
inline bool DbSqlField::hasOrder() const { return m_order != foNone; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_SQL_FIELD_H
