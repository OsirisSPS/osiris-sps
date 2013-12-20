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

#include "stdafx.h"
#include "datatable.h"

#include "constants.h"
#include "dataitem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DataTableRow::DataTableRow(DataTable *table, uint32 index) : m_table(table),
															 m_index(index)
{
	OS_ASSERT(m_table->hasRow(m_index));
}

DataTableRow::DataTableRow(const DataTableRow &row) : m_table(row.m_table),
													  m_index(row.m_index)
{

}

DataTableRow::~DataTableRow()
{

}

uint32 DataTableRow::columns() const
{
	return m_table->columns();
}

DataItem * DataTableRow::getPtr(uint32 column) const
{
	return m_table->getPtr(m_index, column);
}

DataItem * DataTableRow::getPtr(const String &column) const
{
	return m_table->getPtr(m_index, column);
}

DataItem DataTableRow::get(uint32 column) const
{
	DataItem* result = getPtr(column);
	if(result == nullptr)
		return DataItem();
	else
		return *result;	
}

DataItem DataTableRow::get(const String &column) const
{
	DataItem* result = getPtr(column);
	if(result == nullptr)
		return DataItem();
	else
		return *result;	
}

	

void DataTableRow::set(uint32 column, const DataItem &value)
{
	m_table->set(m_index, column, value);
}

void DataTableRow::set(const String &column, const DataItem &value)
{
	m_table->set(m_index, column, value);
}

DataItem * DataTableRow::operator [](uint32 column) const
{
	return getPtr(column);
}

DataItem * DataTableRow::operator [](const String &column) const
{
	return getPtr(column);
}

DataTableRow & DataTableRow::operator =(const DataTableRow &row)
{
	m_table = row.m_table;
	m_index = row.m_index;

	return *this;
}

DataTableRow::operator uint32() const
{
	return m_index;
}

//////////////////////////////////////////////////////////////////////

uint32 DataTable::npos = static_cast<uint32>(-1);
const DataTable DataTable::EMPTY;

//////////////////////////////////////////////////////////////////////

DataTable::DataTable(uint32 rows, uint32 columns) : m_columns(0),
													m_rows(0),
													m_values(OS_NEW_T(DataItems))
{
	_resize(rows, columns);
}

DataTable::~DataTable()
{
	clear();
}

uint32 DataTable::getIndex(const String &column) const
{
	for(size_t i = 0; i < m_names.size(); i++)
	{
		if(m_names[i] == column)
			return static_cast<uint32>(i);
	}

	return npos;
}

String DataTable::getColumnName(uint32 column) const
{
	if(hasColumn(column))
		return m_names[column];

	return String::EMPTY;
}

bool DataTable::setColumnName(uint32 column, const String &name)
{
	if(hasColumn(column) == false)
		return false;

	m_names[column] = name;
	return true;
}

DataItem * DataTable::getPtr(uint32 row, uint32 column) const
{
	if(exists(row, column))
		return (*m_values)[m_columns * row + column];

	return nullptr;
}

DataItem * DataTable::getPtr(uint32 row, const String &column) const
{
	uint32 index = getIndex(column);
	if(index == npos)
		return nullptr;

	return getPtr(row, index);
}

DataItem DataTable::get(uint32 row, uint32 column) const
{
	DataItem* result = getPtr(row, column);
	if(result == nullptr)
		return DataItem();
	else
		return *result;	
}

DataItem DataTable::get(uint32 row, const String &column) const
{
	DataItem* result = getPtr(row, column);
	if(result == nullptr)
		return DataItem();
	else
		return *result;	
}

void DataTable::set(uint32 row, uint32 column, const DataItem &value)
{
	OS_ASSERT(exists(row, column));
	if(exists(row, column))
		*(*m_values)[m_columns * row + column] = value;
}

void DataTable::set(uint32 row, const String &column, const DataItem &value)
{
	uint32 index = getIndex(column);
	if(index != npos)
		set(row, index, value);
}

void DataTable::setColumns(uint32 columns)
{
	_resize(m_rows, columns);
}

void DataTable::setRows(uint32 rows)
{
	_resize(rows, m_columns);
}

void DataTable::setSize(uint32 rows, uint32 columns)
{
	_resize(rows, columns);
}

void DataTable::clear()
{
	m_columns = 0;
	m_names.clear();

	_clear();
}

DataTableRow DataTable::operator [](uint32 row)
{
	return DataTableRow(this, row);
}

void DataTable::_clear()
{
	m_rows = 0;

	for(DataItems::iterator i = m_values->begin(); i != m_values->end();)
	{
		DataItems::iterator y = i;
		++i;

		OS_DELETE *y;
	}

	m_values->clear();
}

void DataTable::_resize(uint32 rows, uint32 columns)
{
	if(m_rows == rows && m_columns == columns)
		return;

	scoped_ptr<DataItems, os_delete_t> values(OS_NEW_T(DataItems));

	for(uint32 row = 0; row < rows; row++)
	{
		for(uint32 column = 0; column < columns; column++)
		{
			DataItem *item;

			if(column < m_columns && row < m_rows)
				item = OS_NEW DataItem(*getPtr(row, column));
			else
				item = OS_NEW DataItem();

			values->push_back(item);
		}
	}

	_clear();

	m_values.swap(values);

	m_names.resize(columns);

	m_columns = columns;
	m_rows = rows;
}

DataTableRow DataTable::addRow()
{
	if(m_columns == 0)
		return DataTableRow(this, npos);

    for(uint32 column = 0; column < m_columns; m_values->push_back(OS_NEW DataItem()), column++);

	return DataTableRow(this, m_rows++);
}

void DataTable::removeAllRows()
{
	m_values->clear();
	m_rows = 0;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
