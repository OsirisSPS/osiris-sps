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
#include "idbresult.h"

#include "dataitem.h"
#include "datatable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IDbResult::IDbResult()
{

}

IDbResult::~IDbResult()
{

}

void IDbResult::init(DataTable &table)
{
	uint32 count = columns();
	table.setColumns(count);
	for(uint32 i = 0;  i < count; i++)
		table.setColumnName(i, columnName(i));
}

void IDbResult::bind(DataTableRow &row)
{
	uint32 count = columns();
	for(uint32 i = 0; i < count; i++)
	{
		DataItem *value = row[i];
		OS_EXCEPT_IF(value == nullptr, "Invalid row value !");
		value->clear();

		get(i, *value);
	}
}

void IDbResult::bind(DataTable &table)
{
	init(table);
	while(end() == false)
	{
	    DataTableRow row = table.addRow();
		bind(row);
		moveNext();
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
