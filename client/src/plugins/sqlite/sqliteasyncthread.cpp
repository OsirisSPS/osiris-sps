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
#include "sqliteasyncthread.h"

#include "log.h"
#include "sqlite/sqlite3async.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

SqliteAsyncThread::SqliteAsyncThread() : m_initialized(false)
{
	
}

SqliteAsyncThread::~SqliteAsyncThread()
{
	if(m_initialized)
	{
		m_initialized = false;
		sqlite3async_shutdown();
	}
}

bool SqliteAsyncThread::init()
{
	OS_ASSERT(m_initialized == false);

	// N.B.: la registrazione del filesystem asincrono va fatta prima di ogni altra operazione su sqlite e pertanto non all'interno del thread
	if(sqlite3async_initialize(NULL, 1) != SQLITE_OK)
		return false;

	m_initialized = true;

	// La versione più "pulita" sarebbe quella di usare SQLITEASYNC_HALT_IDLE in modo che
	// sqlite3async_run() ritorni quando non ci sono operazione di background, ma una semplice
	// yield() in seguito sqlite3async_run() comporta un utilizzo di processo elevato.
	// sqlite3async_run() inoltre è più ottimizzata per girare con SQLITEASYNC_HALT_NEVER in
	// quanto attende l'accodamento di operazioni di I/O tramite eventi e non con delle sleep

//	if(sqlite3async_control(SQLITEASYNC_HALT, SQLITEASYNC_HALT_IDLE) != SQLITE_OK)
//		return false;

#ifdef OS_DEBUG
	sqlite_int currentHalt = 0;
	OS_ASSERT(sqlite3async_control(SQLITEASYNC_GET_HALT, &currentHalt) == SQLITE_OK);
	OS_ASSERT(currentHalt == SQLITEASYNC_HALT_NEVER);
#endif // OS_DEBUG

	return true;
}

void SqliteAsyncThread::run()
{
	sqlite3async_run();
}

void SqliteAsyncThread::stop()
{
	// Viene qui usato SQLITEASYNC_HALT_IDLE e non SQLITEASYNC_HALT_NOW per consentire il complementamento di tutte le operazioni di I/O accodate
	if(sqlite3async_control(SQLITEASYNC_HALT, SQLITEASYNC_HALT_IDLE) != SQLITE_OK)
	{
		OS_ASSERTFALSE();
		OS_LOG_ERROR("Cannot stop Sqlite async module!");
	}
	
	ThreadBase::stop();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
