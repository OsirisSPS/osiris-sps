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
#include "statement.h"

#include "boost/thread/recursive_mutex.hpp"
#include "parameter.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

static std::string getStatementError(MYSQL_STMT *statement)
{
	return mysql_stmt_error(statement);
}

//////////////////////////////////////////////////////////////////////

Statement::Statement(boost::recursive_mutex &cs) : m_lock(cs)
{
	m_statement = null;
}

Statement::~Statement()
{
	close();
}

std::string Statement::error() const
{
	return getStatementError(m_statement);
}

void Statement::prepare(MYSQL *connection, const String &sql)
{
	OS_ASSERT(m_statement == null);

	OS_EXCEPT_IF(connection == null, "Invalid connection");

	m_statement = mysql_stmt_init(connection);
	OS_EXCEPT_IF(m_statement == null, "Mysql internal error");

	std::string query = sql.to_utf8();
	int32 result = mysql_stmt_prepare(m_statement, query.c_str(), static_cast<unsigned long>(query.size()));
	OS_EXCEPT_IF(result != MYSQL_OK, error());

	uint32 needed = neededParameters();
	m_binders.reset(OS_ALLOCATE_T(MYSQL_BIND, needed), os_deallocate_t());
	OS_ZEROMEMORY(m_binders.get(), sizeof(MYSQL_BIND) * needed);
}

uint32 Statement::neededParameters()
{
	return static_cast<uint32>(mysql_stmt_param_count(m_statement));
}

void Statement::close()
{
    if(m_statement != null)
	{
		MYSQL_STMT *statement = m_statement;
		m_statement = null;		// Resetta il puntatore allo statement

		int32 result = mysql_stmt_close(statement);
		
		// La chiusura  richiamata nel distruttore pertanto non deve lanciare eccezioni
		OS_ASSERT(result == MYSQL_OK);
	}
}

shared_ptr<IDatabaseParameter> Statement::createParameter(uint32 index)
{		
	return shared_ptr<IDatabaseParameter>(OS_NEW Parameter(&m_binders[index]));
}

void Statement::run()
{
	int32 result = mysql_stmt_bind_param(m_statement, m_binders.get());
	OS_EXCEPT_IF(result != MYSQL_OK, error());

	result = mysql_stmt_execute(m_statement);
	OS_EXCEPT_IF(result != MYSQL_OK, error());
}
