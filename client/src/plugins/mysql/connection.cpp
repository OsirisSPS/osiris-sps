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
#include "connection.h"

#include "datatable.h"
#include "dbioptions.h"
#include "driver.h"
#include "parser.h"
#include "platformmanager.h"
#include "result.h"
#include "statement.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

Connection::Connection(const String &database, shared_ptr<IDatabaseOptions> options, shared_ptr<IDatabaseDriver> driver) : ConnectionBase(database, options, driver),
																														   m_connection(null)
{
	
}

Connection::~Connection()
{
	close();
}

std::string Connection::error() const
{
	return mysql_error(m_connection);
}

unsigned int Connection::errorCode() const
{
	return mysql_errno(m_connection);
}

bool Connection::ok() const
{
	// TODO: verificare il valore di ritorno
	return errorCode() == MYSQL_OK;
}

bool Connection::databaseExists(const String &name) const
{
	scoped_ptr<Result> result(OS_NEW Result(m_cs));
	// Enumera i database
	result->prepare(mysql_list_dbs(m_connection, null));

	DataTable table;
	result->bind(table);

	uint32 rows = table.rows();
	if(rows > 0)
	{
		for(uint32 i = 0; i < rows; i++)
		{
			String current = *table[i][0];
			if(current.compare_no_case(name))	// I nomi dei database sono case-insensitive
				return true;
		}
	}

	return false;
}

void Connection::connect(const String &name, const String &host, const String &user, const String &password)
{
	close();

	m_connection = mysql_init(null);
	OS_EXCEPT_IF(m_connection == null, "Internal mysql error");

	String mysql_host = host.empty() ? OS_MYSQL_LOCALHOST : host;
	String mysql_user = user.empty() ? OS_MYSQL_ROOT : user;
	String mysql_pass = password;
	String mysql_db = name;

	bool done = mysql_real_connect(m_connection, mysql_host.to_utf8().c_str(), mysql_user.to_utf8().c_str(), mysql_pass.to_utf8().c_str(), mysql_db.to_utf8().c_str(), MYSQL_PORT, null, CLIENT_MULTI_STATEMENTS) != null;
	OS_EXCEPT_IF(done == false, error());

	// Dopo 8 ore di inattivit mysql chiude in automatico la connessione, settando questo flag viene riaperta in automatico (l'opzione va settata dopo l'apertura della connessione, altrimenti verrebbe resettata)
    my_bool my_true = true;
	mysql_options(m_connection, MYSQL_OPT_RECONNECT, &my_true);
}

void Connection::_checkConnection()
{
	OS_EXCEPT_IF(m_connection == null, "Invalid mysql connection");
}

void Connection::open()
{
	shared_ptr<IDatabaseOptions> options = getOptions();
	connect(getDatabase(), options->getParam(OS_MYSQL_HOST), options->getParam(OS_MYSQL_USER), options->getParam(OS_MYSQL_PASSWORD));
}

void Connection::close()
{
	if(m_connection != null)
	{
		mysql_close(m_connection);
		m_connection = null;
	}
}

void Connection::beginTransaction()
{
	execute(_S("START TRANSACTION"));
}

void Connection::commit()
{
	execute(_S("COMMIT"));
}

void Connection::rollback()
{
	execute(_S("ROLLBACK"));
}

uint32 Connection::execute(const String &sql)
{
	OS_LOCK(m_cs);

	_checkConnection();

	bool done = mysql_query(m_connection, sql.to_utf8().c_str()) == 0;
    OS_EXCEPT_IF(done == false, error());

	my_ulonglong result = mysql_affected_rows(m_connection);
	OS_EXCEPT_IF(result == static_cast<my_ulonglong>(-1), error());

	return static_cast<uint32>(result);
}

shared_ptr<IDatabaseStatement> Connection::prepare(const String &sql)
{
	shared_ptr<Statement> statement(OS_NEW Statement(m_cs));
	statement->prepare(m_connection, sql);

	return statement;
}

shared_ptr<IDatabaseResult> Connection::query(const String &sql)
{
	// Nota: la bind del risultato al DataTable va loccata altrimenti si pu generare un errore di tipo "Commands out of sync" (essendo la funzione chiamata da threads diversi)
	// If you get Commands out of sync; you can't run this command now in your client code, you are calling client functions in the wrong order!
	// This can happen, for example, if you are using mysql_use_result() and try to execute a new query before you have called mysql_free_result(). It can also happen if you try to execute two queries that return data without a mysql_use_result() or mysql_store_result() in between.

	OS_LOCK(m_cs);

	_checkConnection();

	bool done = mysql_query(m_connection, sql.to_utf8().c_str()) == 0;
    OS_EXCEPT_IF(done == false, error());

	shared_ptr<Result> result(OS_NEW Result(m_cs));
	result->prepare(this);
	return result;
}

void Connection::parse(sql::command_ptr command, String &sql)
{
	Parser::parseCommand(command, sql);
}

void Connection::analyze()
{
	
}

void Connection::reindex()
{
	
}

void Connection::vacuum()
{
	
}

String Connection::escape(const String &name) const
{
	return OS_MYSQL_ESCAPE(name);
}
