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

#include "dataitem.h"
#include "datatable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_datatable_construction)
{
	DataTable table;
	BOOST_CHECK(table.empty());
	BOOST_CHECK(table.hasRow(0) == false);
	BOOST_CHECK(table.hasColumn(0) == false);
	BOOST_CHECK(table.getPtr(0, 0) == nullptr);
	BOOST_CHECK(table.get(0, 0) == DataItem());
}

BOOST_AUTO_TEST_CASE(test_datatable_operations)
{
	DataTable table;
	table.setSize(2, 1);
	BOOST_CHECK(table.setColumnName(0, "alfa"));
	BOOST_CHECK(table.setColumnName(1, "beta") == false);
	BOOST_CHECK(table.hasRow(0));
	BOOST_CHECK(table.hasRow(1));
	BOOST_CHECK(table.hasRow(2) == false);
	BOOST_CHECK(table.hasColumn(0));
	BOOST_CHECK(table.getColumnName(0) == "alfa");
	BOOST_CHECK(table.hasColumn(1) == false);
	table.setRows(3);
	BOOST_CHECK(table.hasRow(0));
	BOOST_CHECK(table.hasRow(1));
	BOOST_CHECK(table.hasRow(2));
	BOOST_CHECK(table.hasRow(3) == false);
	BOOST_CHECK(table.hasColumn(0));
	BOOST_CHECK(table.getColumnName(0) == "alfa");
	BOOST_CHECK(table.hasColumn(1) == false);
	BOOST_CHECK(table.setColumnName(0, "alfa2"));
	table.setColumns(2);
	BOOST_CHECK(table.hasRow(0));
	BOOST_CHECK(table.hasRow(1));
	BOOST_CHECK(table.hasRow(2));
	BOOST_CHECK(table.hasRow(3) == false);
	BOOST_CHECK(table.hasColumn(0));
	BOOST_CHECK(table.getColumnName(0) == "alfa2");
	BOOST_CHECK(table.hasColumn(1));
	BOOST_CHECK(table.getColumnName(1) == String::EMPTY);
	BOOST_CHECK(table.hasColumn(2) == false);
	
	BOOST_CHECK(table.setColumnName(0, "alfa"));
	BOOST_CHECK(table.setColumnName(1, "beta"));

	table.setSize(1, 3);
	BOOST_CHECK(table.hasRow(0));
	BOOST_CHECK(table.hasRow(1) == false);
	BOOST_CHECK(table.hasColumn(0));
	BOOST_CHECK(table.hasColumn(1));
	BOOST_CHECK(table.hasColumn(2));
	BOOST_CHECK(table.hasColumn(3) == false);
	BOOST_CHECK(table.getColumnName(0) == "alfa");
	BOOST_CHECK(table.getColumnName(1) == "beta");
	BOOST_CHECK(table.getColumnName(2) == String::EMPTY);
	BOOST_CHECK(table.getColumnName(3) == String::EMPTY);
}

BOOST_AUTO_TEST_CASE(test_datatablerow_operations)
{
	DataTable table;	
	table.setSize(2, 4);

	DataTableRow row0 = table[0];
	BOOST_CHECK(row0.columns() == 4);
	BOOST_CHECK(*row0[0] == DataItem::EMPTY);
	BOOST_CHECK(*row0[1] == DataItem::EMPTY);
	BOOST_CHECK(*row0[2] == DataItem::EMPTY);
	BOOST_CHECK(*row0[3] == DataItem::EMPTY);

	DataTableRow row1 = table[0];
	BOOST_CHECK(row1.columns() == 4);
	BOOST_CHECK(*row1[0] == DataItem::EMPTY);
	BOOST_CHECK(*row1[1] == DataItem::EMPTY);
	BOOST_CHECK(*row1[2] == DataItem::EMPTY);
	BOOST_CHECK(*row1[3] == DataItem::EMPTY);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
