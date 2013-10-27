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
#include "dbsqlwhere.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DbSqlWhere::DbSqlWhere()
{

}

DbSqlWhere::~DbSqlWhere()
{

}

DbSqlCondition & DbSqlWhere::add(const String &field, const String &value, uint32 flags)
{
	return add(DbSqlField(field), value, flags);
}

DbSqlCondition & DbSqlWhere::add(const DbSqlField &field, const String &value, uint32 flags)
{
	return conditions.add(DbSqlCondition(field, value, flags));
}

DbSqlCondition & DbSqlWhere::add(const String &field, const StringList &values, uint32 flags)
{
	return conditions.add(DbSqlCondition(field, values, flags));
}

DbSqlCondition & DbSqlWhere::add(const DbSqlField &field, const StringList &values, uint32 flags)
{
	return conditions.add(DbSqlCondition(field, values, flags));
}

DbSqlCondition & DbSqlWhere::add(const DbSqlField &first, const DbSqlField &second, uint32 flags)
{
	return add(first, second.formatName(), flags);
}

DbSqlCondition & DbSqlWhere::add(const String &field, shared_ptr<DbSqlICommand> command, uint32 flags)
{
	return add(DbSqlField(field), command, flags);
}

DbSqlCondition & DbSqlWhere::add(const DbSqlField &field, shared_ptr<DbSqlICommand> command, uint32 flags)
{
	return conditions.add(DbSqlCondition(field, command, flags));
}

void DbSqlWhere::clear()
{
	conditions.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
