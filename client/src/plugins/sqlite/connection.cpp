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
#include "driver.h"
#include "idboptions.h"
#include "file.h"
#include "filesystem.h"
#include "log.h"
#include "options.h"
#include "parser.h"
#include "platformmanager.h"
#include "realtimestats.h"
#include "regexmanager.h"
#include "result.h"
#include "sqliteutils.h"
#include "statement.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

const String Connection::NAME = _S("name");

//////////////////////////////////////////////////////////////////////

Connection::Connection(const String &database, shared_ptr<IDbOptions> options, shared_ptr<IDbDriver> driver) : ConnectionBase(database, options, driver)
{
	m_connection = nullptr;

	m_showSql = OS_NAMESPACE_NAME::Options::instance()->getOption<bool>(_S("developer.show_sql"));	

#ifdef OS_ENABLE_DATABASE_ANALYZE
	m_transactionId = 0;
#endif
}

Connection::~Connection()
{
	close();
}

String Connection::_getPath() const
{
	shared_ptr<Driver> driver = boost::dynamic_pointer_cast<Driver>(getDriver());
	if(driver == nullptr)
	{
		OS_ASSERTFALSE();
		return String::EMPTY;
	}

	return driver->getDatabasePath(getDatabase());
}

void Connection::connect(const String &filename)
{
	close();

	// _tsqlite3_open crea un nuovo database se il file non esiste: dovrebbe essere il driver a monte che non crea la connessione se il file non esiste
	OS_ASSERT(FileSystem::instance()->fileExists(filename));

	int32 res = _tsqlite3_open(filename.c_str(), &m_connection);
	OS_EXCEPT_IF(res != SQLITE_OK, sqlite_last_error(m_connection, res));
	m_filename = filename;

	uint32 cacheSize = OS_NAMESPACE_NAME::Options::instance()->getOption<uint32>(OS_SQLITE_OPTION_CACHE_SIZE);
	String synchronous = static_cast<String>(OS_NAMESPACE_NAME::Options::instance()->getOption<String>(OS_SQLITE_OPTION_SYNCHRONOUS)).to_upper();
	if((synchronous != _S("FULL")) && (synchronous != _S("NORMAL")) && (synchronous != _S("OFF")))
		synchronous = _S("FULL");
	uint32 tempStore = OS_NAMESPACE_NAME::Options::instance()->getOption<uint32>(OS_SQLITE_OPTION_TEMP_STORE);
	if(tempStore > 2)
		tempStore = 0;

	doExecute(m_connection, _S("PRAGMA legacy_file_format = OFF;"));
	doExecute(m_connection, String::format(_S("PRAGMA cache_size = %d;").c_str(), cacheSize)); // Default di sqlite:2000
	doExecute(m_connection, String::format(_S("PRAGMA synchronous = %S;").c_str(), synchronous.c_str())); // FULL/NORMAL/OFF, Default di sqlite: FULL
	doExecute(m_connection, String::format(_S("PRAGMA temp_store = %d").c_str(), tempStore)); // 0(Default)/1(File)/2(Memory), Default di sqlite: 0
}

void Connection::_check(sqlite3 *connection)
{
	OS_EXCEPT_IF(m_connection == nullptr, "Connection not opened");
}

#ifdef OS_ENABLE_DATABASE_ANALYZE
void Connection::postAnalyze(const String &sql)
{
	// Per vedere le sql nel charts:
	// RealtimeStatsManager::signal(sql,0);
}

String Connection::preAnalyze(const String &sql)
{
	static boost::recursive_mutex cs;
	OS_LOCK(cs);

	// Per vedere le sql nel charts:
	// RealtimeStatsManager::signal(sql, 1, RealtimeStatsCounter::ctAbsolute, _S("status"));

	// Da notare che vengono archiviate nella os_monitor solo le query analizzate dalla "explain".
	// Una "insert", non usando indici, non viene catalogata, a meno che non abbia sotto-query.

	if(sql.find(_S("explain query plan ")) != String::npos)
		return _S("");

	if(sql.find(_S("os_monitor")) != String::npos)
		return _S("");

	static shared_ptr<Connection> connectionAnalyze = nullptr;
	if(connectionAnalyze == nullptr)
	{
		shared_ptr<Driver> driver = boost::dynamic_pointer_cast<Driver>(getDriver());
		String path = driver->getDatabasePath(_S("monitor.db"));
		if(FileSystem::instance()->exists(path) == false)
		{
			// Crea il database se non esiste
			File file;
			String msg = _S("Cannot create file '") + path + _S("'");
			OS_EXCEPT_IF(file.open(path, File::ofWrite) == false, msg.to_ascii());
		}

		connectionAnalyze.reset(OS_NEW Connection(path, getDriver()->createOptions(), getDriver()));

		int32 res = _tsqlite3_open(path.c_str(), &connectionAnalyze->m_connection);
		OS_EXCEPT_IF(res != SQLITE_OK, sqlite_last_error(connectionAnalyze->m_connection, res));

		// Creazione tabella
		String sqlCreate = _S("CREATE TABLE IF NOT EXISTS os_monitor (filename text,sql text,thread integer,conn integer,trans integer, qorder integer,qfrom integer,detail text,indexUsed text)");
		doExecute(connectionAnalyze->m_connection, sqlCreate);
	}

	// I valori dei campi li setto a ?, per poter fare delle distinct sui risultati
	String keySql = sql;
	keySql.trim();
	keySql = regex_replace(keySql, RegexManager::instance()->getRegex(_S("'(.*?)'")), _S("<s>")); // Valori dei campi
	keySql = regex_replace(keySql, RegexManager::instance()->getRegex(_S("[-+]?[0-9]*\\.?[0-9]+")), _S("<n>")); // Numeri
	keySql = keySql.replace_all(_S("\n"),_S(""));
	keySql = keySql.replace_all(_S("\r"),_S(""));
	keySql = keySql.replace_all(_S("\t"),_S(""));
	keySql = keySql.replace_all(_S("'"),_S("")); // Futile, per evitare errori nella insert nella os_monitor
	for(;;) // Comprimo gli spazi
	{
		String x = keySql;
		keySql = keySql.replace_all(_S("  "),_S(" "));
		if(x == keySql)
			break;
	}

	uint32 conn = uint32(reinterpret_cast<uint64>(this));
	boost::any threadID = PlatformManager::instance()->getCurrentThreadID();
	// Tenta un cast a uint32
	uint32 *thread = boost::any_cast<uint32>(&threadID);
	int32 trans = m_transactionId;

	DataTable result;
	doQuery(m_connection, _S("explain query plan ") + sql,result);

	for(uint32 r=0;r<result.rows();r++)
	{
		int32 order = *result.get(r,_S("order"));
		int32 from = *result.get(r,_S("from"));
		String detail = *result.get(r,_S("detail"));

		String indexUsed;
		String::size_type posWithIndex = detail.find(_S("WITH INDEX"));
		if(posWithIndex != String::npos)
		{
			indexUsed = detail.substr(posWithIndex+10);
			indexUsed = indexUsed.replace_all(_S("ORDER BY"),String::EMPTY);
			indexUsed.trim();
		}

		String sqlInsert = String::format(_S("insert into os_monitor values ('%S','%S',%u,%u,%u,%d,%d,'%S','%S')").c_str(), m_filename.c_str(), keySql.c_str(), conn, thread != nullptr ? *thread : 0, trans, order, from, detail.c_str(), indexUsed.c_str());
		doExecute(connectionAnalyze->m_connection, sqlInsert);
	}

	String trace = String::format(_S("Sqlite monitor: sql='%S',conn=%u,thread=%u,trans=%u\n").c_str(), sql.c_str(), conn, thread != nullptr ? *thread : 0, trans);
	//OS_TRACE(trace);

	//RealtimeStatsManager::signal(_S("Sqlite ") + keySql.mid(0,6), 1, RealtimeStatsCounter::ctRelative, _S("items"));
	return keySql;
}
#endif

void Connection::open()
{
	connect(_getPath());
}

void Connection::close()
{
	if(m_connection != nullptr)
	{
		sqlite3_close(m_connection);
		m_connection = nullptr;
	}
}

void Connection::beginTransaction()
{
	doExecute(m_connection,_S("BEGIN"));
#ifdef OS_ENABLE_DATABASE_ANALYZE
	static uint32 transactionStaticID = 0;
	m_transactionId = transactionStaticID++;
#endif
}

void Connection::commit()
{
	doExecute(m_connection,_S("COMMIT"));
#ifdef OS_ENABLE_DATABASE_ANALYZE
	m_transactionId = 0;
#endif
}

void Connection::rollback()
{
	doExecute(m_connection,_S("ROLLBACK"));
#ifdef OS_ENABLE_DATABASE_ANALYZE
	m_transactionId = 0;
#endif
}

uint32 Connection::_execute(const String &sql)
{
	OS_LOCK(m_cs);

	//RealtimeStatsScopeTimer RSS(_S("Debug"), _S("Sqlite - Connection::execute"));
	//OS_LOG_WARNING(sql);
#ifdef OS_ENABLE_DATABASE_ANALYZE
	RealtimeStatsScopeTimer RSS(_S("Debug"), _S("Sqlite - ") + preAnalyze(sql));
#endif

	uint32 result = doExecute(m_connection, sql);
#ifdef OS_ENABLE_DATABASE_ANALYZE
	postAnalyze(sql);
#endif
	return result;
}

uint32 Connection::doExecute(sqlite3 *connection, const String &sql)
{
	if(m_showSql)
		OS_LOG_NOTICE(sql);
	_check(connection);

	scoped_ptr<Result> result(OS_NEW Result(connection, m_cs));
	result->execute(sql);
	// Restituisce il numero di records affetti
	return sqlite3_changes(connection);
}

shared_ptr<IDbStatement> Connection::_prepare(const String &sql)
{
	//RealtimeStatsScopeTimer RSS(_S("Debug"), _S("Sqlite - Connection::prepare"));
	//OS_LOG_WARNING(sql);

#ifdef OS_ENABLE_DATABASE_ANALYZE
	RealtimeStatsScopeTimer RSS(_S("Debug"), _S("Sqlite - ") + preAnalyze(sql));
#endif

	shared_ptr<Statement> statement(OS_NEW Statement(m_cs));
	statement->prepare(m_connection, sql);

#ifdef OS_ENABLE_DATABASE_ANALYZE
	postAnalyze(sql);
#endif

	return statement;
}

shared_ptr<IDbResult> Connection::_query(const String &sql)
{
	OS_LOCK(m_cs);

#ifdef OS_ENABLE_DATABASE_ANALYZE
	RealtimeStatsScopeTimer RSS(_S("Debug"), _S("Sqlite - ") + preAnalyze(sql));
#endif

	shared_ptr<IDbResult> result = doQuery(m_connection, sql);

#ifdef OS_ENABLE_DATABASE_ANALYZE
	postAnalyze(sql);
#endif

	return result;
}

void Connection::doQuery(sqlite3 *connection, const String &sql, DataTable &table)
{
	table.clear();

	shared_ptr<Result> result = doQuery(connection, sql);
	if(result != nullptr)
		result->bind(table);
}

shared_ptr<Result> Connection::doQuery(sqlite3 *connection, const String &sql)
{
	if(m_showSql)
		OS_LOG_NOTICE(sql);
	// Nota: la bind del risultato al DataTable va loccata altrimenti si può generare un errore di tipo "Commands out of sync" (essendo la funzione chiamata da threads diversi)

	_check(connection);

	shared_ptr<Result> result(OS_NEW Result(connection, m_cs));
	result->prepare(sql);
	return result;
}

void Connection::_parse(shared_ptr<DbSqlICommand> command, String &sql)
{
	Parser::parseCommand(command, sql);
}

void Connection::analyze()
{
	doExecute(m_connection, _S("analyze"));
}

void Connection::reindex()
{
	doExecute(m_connection, _S("reindex"));
}

void Connection::vacuum()
{
	doExecute(m_connection, _S("vacuum"));
}
