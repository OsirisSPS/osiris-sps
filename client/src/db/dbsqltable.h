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

#ifndef _DB_SQL_TABLE_H
#define _DB_SQL_TABLE_H

#include "dbdb.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DatabaseExport DbSqlTable
{
// Construction
public:
	DbSqlTable(const String &name);
	DbSqlTable(const DbSqlTable &second);
	virtual ~DbSqlTable();

// Attributes
public:
	inline const String & getName() const;
	inline void setName(const String &name);

	inline const String & getSuggestedIndex() const;
	inline void setSuggestedIndex(const String &suggestedIndex);

// Operators
public:
	DbSqlTable & operator =(const DbSqlTable &second);

	bool operator ==(const DbSqlTable &second);
	bool operator !=(const DbSqlTable &second);

protected:
	String m_name;
	String m_suggestedIndex;
};

//////////////////////////////////////////////////////////////////////

typedef DbSqlCollection<DbSqlTable> DbSqlTables;

//////////////////////////////////////////////////////////////////////

inline const String & DbSqlTable::getName() const { return m_name; }
inline void DbSqlTable::setName(const String &name) { m_name = name; }

inline const String & DbSqlTable::getSuggestedIndex() const { return m_suggestedIndex; }
inline void DbSqlTable::setSuggestedIndex(const String &suggestedIndex) { m_suggestedIndex = suggestedIndex; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_SQL_TABLE_H
