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
#include "dbsqlcondition.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DbSqlCondition::DbSqlCondition()
{
	m_flags = cfEqual;
}

DbSqlCondition::DbSqlCondition(const DbSqlField &field, const String &value, uint32 flags)
{
	setField(field);
	setValue(value);
	m_flags = flags;
}

DbSqlCondition::DbSqlCondition(const DbSqlField &field, shared_ptr<DbSqlICommand> command, uint32 flags)
{
	setField(field);
	setCommand(command);
	m_flags = flags;
}

DbSqlCondition::DbSqlCondition(const DbSqlField &field, const StringList &values, uint32 flags)
{
	OS_ASSERT(values.empty() == false);

	String value;
	if(values.size() == 1)
	{
		value = values.front();	
		if((flags & DbSqlCondition::cfIn) == DbSqlCondition::cfIn)
		{
			flags &= ~DbSqlCondition::cfIn;	
			flags |= DbSqlCondition::cfEqual;
		}
	}
	else
	{
		value = _S("(");
		for(StringList::const_iterator i = values.begin(); i != values.end(); ++i)
		{
			if(i != values.begin())
				value += _S(", ");

			OS_ASSERT((*i).empty() == false);
			value += *i;
		}
		value += _S(")");
	}

	setField(field);
	setValue(value);
	m_flags = flags;
}

DbSqlCondition::DbSqlCondition(const DbSqlCondition &second)
{
	*this = second;
}

DbSqlCondition::~DbSqlCondition()
{

}

DbSqlCondition & DbSqlCondition::operator =(const DbSqlCondition &second)
{
	m_field = second.m_field;
	m_conditionValue = second.m_conditionValue;
	m_conditionCommand = second.m_conditionCommand;
	m_flags = second.m_flags;

	return *this;
}

bool DbSqlCondition::operator ==(const DbSqlCondition &second)
{
	return m_field == second.m_field && 
		   m_conditionValue == second.m_conditionValue && 
		   m_conditionCommand == second.m_conditionCommand;
}

bool DbSqlCondition::operator !=(const DbSqlCondition &second)
{
	return (*this == second) == false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
