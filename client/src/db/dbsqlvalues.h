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

#ifndef _DB_SQL_VALUES_H
#define _DB_SQL_VALUES_H

#include "dataitem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DatabaseExport DbSqlValues
{
public:
	typedef std::pair<String, DataItem> value_type;
	typedef list<value_type>::type values_list;
	typedef values_list::const_iterator iterator;

// Construction
public:
	DbSqlValues();
	virtual ~DbSqlValues();

// Attributes
public:
	inline iterator begin() const;
	inline iterator end() const;

	inline bool empty() const;

// Operations
public:
	void set(const String &name, const DataItem &value);

protected:
	values_list m_values;
};

//////////////////////////////////////////////////////////////////////

inline DbSqlValues::iterator DbSqlValues::begin() const { return m_values.begin(); }
inline DbSqlValues::iterator DbSqlValues::end() const { return m_values.end(); }

inline bool DbSqlValues::empty() const { return m_values.empty(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_SQL_VALUES_H
