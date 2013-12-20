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

#include "dbdatabasessystem.h"
#include "driver.h"
#include "format.h"
#include "log.h"
#include "options.h"
#include "osirisplugins.h"
#include "platformmanager.h"
#include "plugin.h"
#include "sqliteasyncthread.h"
#include "thirdpartylibrary.h"
#include "thirdpartylibrariesreporter.h"

//////////////////////////////////////////////////////////////////////

#define OS_SQLITE_THREADSAFE_SINGLE_THREAD		0				// In this mode, all mutexes are disabled and SQLite is unsafe to use in more than a single thread at once.
#define OS_SQLITE_THREADSAFE_SERIALIZED			1				// In serialized mode, SQLite can be safely used by multiple threads with no restriction.
#define OS_SQLITE_THREADSAFE_MULTI_THREAD		2				// In this mode, SQLite can be safely used by multiple threads provided that no single database connection is used simulataneously in two or more threads.

#define OS_SQLITE_ROUND8(x)						(((x)+7)&~7)	// Round up a number to the next larger multiple of 8.  This is used to force 8-byte alignment on 64-bit architectures.

//////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////

OS_MODULE_ENTRY_POINT()

//////////////////////////////////////////////////////////////////////

OS_STATIC_ASSERT(sizeof(sqlite3_int64) == 8);

static void * sqliteAllocate(int nByte)
{
	OS_ASSERT(nByte > 0);

	nByte = OS_SQLITE_ROUND8(nByte);	// sqlite3_int64 è già allineato a 8 bytes
	sqlite3_int64 *p = static_cast<sqlite3_int64 *>(OS_ALLOCATE(nByte + sizeof(sqlite3_int64)));
	if(p)
	{
		p[0] = nByte;
		p++;
	}

	return static_cast<void *>(p);
}

static void sqliteDeallocate(void *pPrior)
{
	OS_ASSERT(pPrior != 0);

	sqlite3_int64 *p = static_cast<sqlite3_int64 *>(pPrior);	
	p--;
	OS_DEALLOCATE(p);
}

static void * sqliteReallocate(void *pPrior, int nByte)
{
	OS_ASSERT(pPrior != 0 && nByte > 0);

	sqlite3_int64 *p = static_cast<sqlite3_int64 *>(pPrior);  
	nByte = OS_SQLITE_ROUND8(nByte);	// sqlite3_int64 è già allineato a 8 bytes
	p--;
	p = static_cast<sqlite3_int64 *>(OS_REALLOCATE(p, nByte + sizeof(sqlite3_int64)));
	if(p)
	{
		p[0] = nByte;
		p++;
	}
	
	return static_cast<void *>(p);
}

static int sqliteAllocationSize(void *pPrior)
{
	if(pPrior == 0) 
		return 0;

	sqlite3_int64 *p = static_cast<sqlite3_int64 *>(pPrior);
	p--;
	return static_cast<int>(p[0]);
}

static int sqliteAllocationRoundup(int n)
{
	return OS_SQLITE_ROUND8(n);
}

static int sqliteAllocationInit(void *)
{
	return SQLITE_OK;
}

static void sqliteAllocationShutdown(void *)
{
	
}

//////////////////////////////////////////////////////////////////////

static void sqliteFinalizeDriver(Driver *driver)
{
	OS_DELETE driver;

	sqlite3_shutdown();
}

//////////////////////////////////////////////////////////////////////

static bool pluginInitialize(Plugin &plugin)
{
	// Options
	OS_NAMESPACE_NAME::Options::instance()->ensureOption<uint32>(OS_SQLITE_OPTION_CACHE_SIZE, 10000, true);
	OS_NAMESPACE_NAME::Options::instance()->ensureOption<String>(OS_SQLITE_OPTION_SYNCHRONOUS, _S("FULL"), true);		
	OS_NAMESPACE_NAME::Options::instance()->ensureOption<uint32>(OS_SQLITE_OPTION_TEMP_STORE, 2, true);
	OS_NAMESPACE_NAME::Options::instance()->ensureOption<bool>(OS_SQLITE_OPTION_ENABLE_ASYNCIO, false, true);
	OS_NAMESPACE_NAME::Options::instance()->ensureOption<bool>(OS_SQLITE_OPTION_ENABLE_MEMORYMANAGER, true, true);

	int32 threadsafe = sqlite3_threadsafe();
	if(threadsafe != OS_SQLITE_THREADSAFE_SERIALIZED)
	{
		OS_ASSERTFALSE();		
		OS_LOG_WARNING(boost::format("Sqlite threadsafe option '%d' is not supported") % threadsafe);
	}		

	if(sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 0) != SQLITE_OK)
		OS_LOG_WARNING("Error disabling Sqlite memory tracker");

	// TODO: valutare http://www.sqlite.org/sharedcache.html

	if(OS_NAMESPACE_NAME::Options::instance()->getOption<bool>(OS_SQLITE_OPTION_ENABLE_MEMORYMANAGER))
	{
		static sqlite3_mem_methods sqliteMemoryManager;
		OS_ZEROMEMORY(&sqliteMemoryManager, sizeof(sqlite3_mem_methods));
		sqliteMemoryManager.xMalloc = sqliteAllocate;
		sqliteMemoryManager.xFree = sqliteDeallocate;
		sqliteMemoryManager.xRealloc = sqliteReallocate;
		sqliteMemoryManager.xSize = sqliteAllocationSize;
		sqliteMemoryManager.xRoundup = sqliteAllocationRoundup;
		// Le funzioni xInit/xShutdown sono necessarie in quanto internamente l'sqlite non verifica se siano settate o meno
		sqliteMemoryManager.xInit = sqliteAllocationInit;
		sqliteMemoryManager.xShutdown = sqliteAllocationShutdown;
	  
		if(sqlite3_config(SQLITE_CONFIG_MALLOC, &sqliteMemoryManager) != SQLITE_OK)
		{
			OS_ASSERTFALSE();
			OS_LOG_CRITICAL("Sqlite memory manager initialization error");
			return false;
		}
	}

	if(sqlite3_initialize() != SQLITE_OK)
	{
		OS_ASSERTFALSE();
		OS_LOG_CRITICAL("Sqlite initialization error");
		return false;
	}

	// Tenere una variabile statica perchè l'sqlite non effettua una copia del valore, ma tiene semplicemente un puntatore
	static std::string tempPath = OS_NAMESPACE_NAME::Options::instance()->getTempPath().to_utf8();
	if(tempPath.empty() == false)
		sqlite3_temp_directory = const_cast<char *>(tempPath.c_str());
	
	shared_ptr<SqliteAsyncThread> sqliteAsyncThread;
	if(OS_NAMESPACE_NAME::Options::instance()->getOption<bool>(OS_SQLITE_OPTION_ENABLE_ASYNCIO))
	{
		OS_LOG_DEBUG("Initializing Sqlite async I/O module");

		sqliteAsyncThread.reset(OS_NEW SqliteAsyncThread());
		if(sqliteAsyncThread->init())
		{
			sqliteAsyncThread->start();
		}
		else
		{
			sqliteAsyncThread.reset();
			OS_LOG_ERROR("Cannot start Sqlite async I/O module");
		}
	}

	if(DatabasesSystem::instance()->addDriver(shared_ptr<IDbDriver>(OS_NEW Driver(plugin.getDataPath(), sqliteAsyncThread), &sqliteFinalizeDriver)) == false)
		return false;

	shared_ptr<ThirdPartyLibrary> sqliteLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
	OS_ASSERT(sqliteLibrary != nullptr);
	sqliteLibrary->setName("Sqlite");
	sqliteLibrary->setVersion(sqlite3_libversion());
    sqliteLibrary->setDescription("SQLite is a in-process library that implements a self-contained, serverless, zero-configuration, transactional SQL database engine.");
    sqliteLibrary->setUrl("http://www.sqlite.org/");
    sqliteLibrary->setLicense("Public domain");		
	sqliteLibrary->setLicenseUrl("http://en.wikipedia.org/wiki/Public_domain");

	return true;
}

static bool pluginTerminate()
{
	return true;
}

//////////////////////////////////////////////////////////////////////

OS_PLUGIN_DECLARE(pluginInitialize, pluginTerminate)

//////////////////////////////////////////////////////////////////////
