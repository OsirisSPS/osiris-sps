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

#ifndef _OS_CORE_DATATABLE_H
#define _OS_CORE_DATATABLE_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DataItem;
class DataTable;

//////////////////////////////////////////////////////////////////////

class CoreExport DataTableRow
{
// Construction
public:
	DataTableRow(DataTable *table, uint32 index);
	DataTableRow(const DataTableRow &row);
	virtual ~DataTableRow();

// Attributes
public:
	uint32 columns() const;

	DataItem * get(uint32 column) const;
	DataItem * get(const String &column) const;

	void set(uint32 column, const DataItem &value);
	void set(const String &column, const DataItem &value);

// Operators
public:
	DataItem * operator [](uint32 column) const;
	DataItem * operator [](const String &column) const;

	DataTableRow & operator =(const DataTableRow &row);

	operator uint32() const;

private:
	DataTable *m_table;
	mutable uint32 m_index;
};

//////////////////////////////////////////////////////////////////////

class CoreExport DataTable : public Object
{
	typedef deque<DataItem *>::type DataItems;

// Construction
public:
	DataTable(uint32 rows = 0, uint32 columns = 0);
	virtual ~DataTable();

// Attributes
public:
	inline bool hasColumn(uint32 column) const;
	inline bool hasRow(uint32 row) const;

	inline bool empty() const;
	inline bool exists(uint32 row, uint32 column) const;

	inline uint32 columns() const;
	inline uint32 rows() const;

	uint32 getIndex(const String &column) const;

	String getColumnName(uint32 column) const;
	bool setColumnName(uint32 column, const String &name);

// Operations
public:
	DataTableRow addRow();
	void removeAllRows();

	DataItem * get(uint32 row, uint32 column) const;
	DataItem * get(uint32 row, const String &column) const;

	void set(uint32 row, uint32 column, const DataItem &value);
	void set(uint32 row, const String &column, const DataItem &value);

	void setColumns(uint32 columns);
	void setRows(uint32 rows);
	void setSize(uint32 rows, uint32 columns);

	void clear();

// Operators
public:
	DataTableRow operator [](uint32 row);

private:
	void _clear();
	void _resize(uint32 rows, uint32 columns);

public:
	static uint32 npos;
	static const DataTable EMPTY;

protected:
	uint32 m_columns;
	uint32 m_rows;

	StringVector m_names;
	scoped_ptr<DataItems, os_delete_t> m_values;
};

//////////////////////////////////////////////////////////////////////

inline bool DataTable::hasColumn(uint32 column) const { return column < m_columns; }
inline bool DataTable::hasRow(uint32 row) const { return row < m_rows; }

inline bool DataTable::empty() const { return m_columns == 0 && m_rows == 0; }
inline bool DataTable::exists(uint32 row, uint32 column) const { return hasRow(row) && hasColumn(column); }

inline uint32 DataTable::columns() const { return m_columns; }
inline uint32 DataTable::rows() const { return m_rows; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_DATATABLE_H
