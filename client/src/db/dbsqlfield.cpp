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
#include "dbsqlfield.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DbSqlField::DbSqlField(const String &name, const String &table, const String &alias)
{
	m_name = name;
	m_table = table;
	m_alias = alias;
	m_attribute = faNone;
	m_order = foNone;
}

DbSqlField::DbSqlField(const DbSqlField &second) : BaseClass(second)
{
	*this = second;
}

DbSqlField::~DbSqlField()
{

}

String DbSqlField::formatName() const
{
	if(hasTable())
		return String::format(_S("%S.%S").c_str(), getTable().c_str(), getName().c_str());

	return getName();
}

DbSqlField & DbSqlField::operator =(const DbSqlField &second)
{
	m_name = second.m_name;
	m_table = second.m_table;
	m_alias = second.m_alias;
	m_attribute = second.m_attribute;
	m_order = second.m_order;

	return *this;
}

bool DbSqlField::operator ==(const DbSqlField &second)
{
	if(m_name != second.m_name)
		return false;

	if(m_table != second.m_table)
		return false;

	if(m_alias != second.m_alias)
		return false;

	if(m_attribute != second.m_attribute)
		return false;

	if(m_order != second.m_order)
		return false;

	return true;
}

bool DbSqlField::operator !=(const DbSqlField &second)
{
	return (*this == second) == false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
