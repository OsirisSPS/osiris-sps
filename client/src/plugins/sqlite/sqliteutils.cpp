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
#include "sqliteutils.h"

#include "driver.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

namespace sqlite {

//////////////////////////////////////////////////////////////////////

sqlite_int safe_sqlite_finalize(sqlite3_stmt *stmt)
{
	sqlite_int res = SQLITE_OK;
	for(;;)
	{
		res = sqlite3_finalize(stmt);
		if(res != SQLITE_BUSY)
			break;

		PlatformManager::instance()->sleep(Driver::BUSY_TIMEOUT);
	}

	return res;
}

sqlite_int safe_sqlite_prepare(sqlite3 *db, const String &sql, sqlite3_stmt **stmt, const void **reminder)
{
	return safe_sqlite_prepare(db, sql.buffer(), static_cast<int32>(sql.buffer_size()), stmt, reminder);
}

int32 safe_sqlite_prepare(sqlite3 *db, const void *buffer, int32 bytes, sqlite3_stmt **stmt, const void **reminder)
{
	int32 res = SQLITE_OK;
	for(;;)
	{
		res = _tsqlite3_prepare(db, buffer, bytes, stmt, reminder);
		if(res != SQLITE_BUSY)
			break;

		PlatformManager::instance()->sleep(Driver::BUSY_TIMEOUT);
	}

	return res;
}

sqlite_int safe_sqlite_step(sqlite3_stmt *stmt)
{
	sqlite_int res = SQLITE_OK;
	for(;;)
	{
		res = sqlite3_step(stmt);
		if(res != SQLITE_BUSY)
			break;

		PlatformManager::instance()->sleep(Driver::BUSY_TIMEOUT);
	}

	return res;
}

std::string sqlite_error_code(int32 code)
{
	switch(code)
	{
	case SQLITE_OK:			return "SQLITE_OK";
	case SQLITE_ERROR:		return "SQLITE_ERROR";
	case SQLITE_INTERNAL:	return "SQLITE_INTERNAL";
	case SQLITE_PERM:		return "SQLITE_PERM";
	case SQLITE_ABORT:		return "SQLITE_ABORT";
	case SQLITE_BUSY:		return "SQLITE_BUSY";
	case SQLITE_LOCKED:		return "SQLITE_LOCKED";
	case SQLITE_NOMEM:		return "SQLITE_NOMEM";
	case SQLITE_READONLY:	return "SQLITE_READONLY";
	case SQLITE_INTERRUPT:	return "SQLITE_INTERRUPT";
	case SQLITE_IOERR:		return "SQLITE_IOERR";
	case SQLITE_CORRUPT:	return "SQLITE_CORRUPT";
	case SQLITE_NOTFOUND:	return "SQLITE_NOTFOUND";
	case SQLITE_FULL:		return "SQLITE_FULL";
	case SQLITE_CANTOPEN:	return "SQLITE_CANTOPEN";
	case SQLITE_PROTOCOL:	return "SQLITE_PROTOCOL";
	case SQLITE_EMPTY:		return "SQLITE_EMPTY";
	case SQLITE_SCHEMA:		return "SQLITE_SCHEMA";
	case SQLITE_TOOBIG:		return "SQLITE_TOOBIG";
	case SQLITE_CONSTRAINT: return "SQLITE_CONSTRAINT";
	case SQLITE_MISMATCH:	return "SQLITE_MISMATCH";
	case SQLITE_MISUSE:		return "SQLITE_MISUSE";
	case SQLITE_NOLFS:		return "SQLITE_NOLFS";
	case SQLITE_AUTH:		return "SQLITE_AUTH";
	case SQLITE_FORMAT:		return "SQLITE_FORMAT";
	case SQLITE_RANGE:		return "SQLITE_RANGE";
	case SQLITE_ROW:		return "SQLITE_ROW";
	case SQLITE_DONE:		return "SQLITE_DONE";
	}

	return "SQLITE_UNKNOWN_ERROR";
}

std::string sqlite_last_error(sqlite3 *db, int32 code)
{
	String error;
	if(db != null)
	{
		const uchar *desc = _tsqlite3_errmsg(db);
		if(code != -1)
			error = String::format(_S("%S[%d]: %S").c_str(), String(sqlite_error_code(code)).c_str(), code, desc);
		else
			error = desc;
	}

	return error.to_ascii();
}

//////////////////////////////////////////////////////////////////////

} // sqlite

//////////////////////////////////////////////////////////////////////
