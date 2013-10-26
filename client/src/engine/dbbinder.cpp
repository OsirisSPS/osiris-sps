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
#include "dbbinder.h"

#include "dbsqlvalues.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DbBinder::DbBinder()
{

}

DbBinder::~DbBinder()
{

}

void DbBinder::get(const DataTableRow &row, const String &field, bool &v, const bool &def)
{
	get<bool, bool>(row, field, v, def);
}

void DbBinder::get(const DataTableRow &row, const String &field, int32 &v, const int32 &def)
{
	get<int32, int32>(row, field, v, def);
}

void DbBinder::get(const DataTableRow &row, const String &field, uint32 &v, const uint32 &def)
{
	get<uint32, uint32>(row, field, v, def);
}

void DbBinder::get(const DataTableRow &row, const String &field, String &v, const String &def)
{
	get<String, String>(row, field, v, def);
}

void DbBinder::get(const DataTableRow &row, const String &field, DateTime &v, const DateTime &def)
{
	get<DateTime, String>(row, field, v, def.toString());
}

void DbBinder::get(const DataTableRow &row, const String &field, Buffer &v, const Buffer &def)
{
	get<Buffer, Buffer>(row, field, v, def);
}

template <>
void DbBinder::set<ObjectID>(DbSqlValues &values, const String &name, const ObjectID &value)
{
	values.set(name, value.toUTF16());
}

/*
template <>
void DbBinder::set<ExtensionsComponentID>(DbSqlValues &values, const String &name, const ExtensionsComponentID &value)
{
	values.set(name, value.toUTF16());
}

template <>
void DbBinder::set<ExtensionsModuleID>(DbSqlValues &values, const String &name, const ExtensionsModuleID &value)
{
	values.set(name, value.toUTF16());
}
*/

template <>
void DbBinder::set<DateTime>(DbSqlValues &values, const String &name, const DateTime &value)
{
	values.set(name, value.toString());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
