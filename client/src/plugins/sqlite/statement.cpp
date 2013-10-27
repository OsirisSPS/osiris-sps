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
#include "sqliteutils.h"

//////////////////////////////////////////////////////////////////////

Statement::Statement(boost::recursive_mutex &cs) : m_lock(cs)
{
	m_statement = null;
	m_connection = null;
}

Statement::~Statement()
{
	close();
}

void Statement::prepare(sqlite3 *connection, const String &sql)
{
	OS_ASSERT(m_statement == null);

	OS_EXCEPT_IF(connection == null, "Invalid connection");
	m_connection = connection;

	sqlite_int res = safe_sqlite_prepare(connection, sql, &m_statement);
	OS_EXCEPT_IF(res != SQLITE_OK, sqlite_last_error(connection, res));
}

uint32 Statement::neededParameters()
{
	return sqlite3_bind_parameter_count(m_statement);
}

void Statement::close()
{
	if(m_statement != null)
	{
		sqlite3_stmt *statement = m_statement;
		m_statement = null;		// Resetta il puntatore allo statement

		sqlite_int res = safe_sqlite_finalize(statement);
		// La chiusura  richiamata nel distruttore pertanto non deve lanciare eccezioni
		if(res != SQLITE_OK)
		{
			OS_TRACE(sqlite_last_error(m_connection, res));
			OS_ASSERTFALSE();
		}
	}

	m_connection = null;
}

shared_ptr<IDbParameter> Statement::createParameter(uint32 index)
{
	// Nota: gli indici dei parametri in sqlite sono 1 based
	return shared_ptr<IDbParameter>(OS_NEW Parameter(m_statement, index + 1));
}

void Statement::run()
{
	sqlite_int res = safe_sqlite_step(m_statement);
	OS_EXCEPT_IF(res != SQLITE_OK && res != SQLITE_ROW && res != SQLITE_DONE, sqlite_last_error(m_connection, res));
}
