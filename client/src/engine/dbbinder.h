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

#ifndef _DB_BINDER_H
#define _DB_BINDER_H

#include "base/object.h"
#include "datetime.h"
#include "dbdb.h"
#include "dbsqlvalues.h"
#include "buffer.h"
#include "dataitem.h"
#include "datatable.h"
#include "objectsobjects.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport DbBinder : public Object
{
// Construction
public:
	DbBinder();
	virtual ~DbBinder();

// Operations
public:
	template <typename T>
    void get(const DataTableRow &row, const String &field, T &v);

	template <typename T, typename V>
	void get(const DataTableRow &row, const String &field, T &v, const V &def);

	void get(const DataTableRow &row, const String &field, bool &v, const bool &def);
	void get(const DataTableRow &row, const String &field, int32 &v, const int32 &def = 0);
	void get(const DataTableRow &row, const String &field, uint32 &v, const uint32 &def = 0);
	void get(const DataTableRow &row, const String &field, String &v, const String &def = String::EMPTY);
	void get(const DataTableRow &row, const String &field, DateTime &v, const DateTime &def = DateTime::EMPTY);
	void get(const DataTableRow &row, const String &field, Buffer &v, const Buffer &def = Buffer::EMPTY);

	template <typename T>
	void set(DbSqlValues &values, const String &name, const T &value);
};

//////////////////////////////////////////////////////////////////////

template <typename T>
void DbBinder::get(const DataTableRow &row, const String &field, T &v)
{
	DataItem *value = row[field];
	OS_EXCEPT_IF(value == nullptr, String::format(_S("'%S': invalid value").c_str(), field.c_str()));

	v = *value;
}

template <typename T, typename V>
void DbBinder::get(const DataTableRow &row, const String &field, T &v, const V &def)
{
	DataItem *value = row[field];
	if(value == nullptr)
	{
	    v = def;
	}
	else
	{
	    V *value_ptr = static_cast<V *>(*value);
	    if(value_ptr != nullptr)
            // Se la tipologia coincide evita una copia temporanea dell'oggetto
            v = *value_ptr;
        else
            // Forza un cast a T per evitare conflitti
            v = value->get<V>();
	}
}

template <typename T>
void DbBinder::set(DbSqlValues &values, const String &name, const T &value)
{
	values.set(name, value);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_BINDER_H
