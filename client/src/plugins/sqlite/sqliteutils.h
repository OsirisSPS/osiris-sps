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

#ifndef _SQLITE_UTILS_H
#define _SQLITE_UTILS_H

//////////////////////////////////////////////////////////////////////

namespace sqlite {

//////////////////////////////////////////////////////////////////////

int32 safe_sqlite_finalize(sqlite3_stmt *stmt);
int32 safe_sqlite_prepare(sqlite3 *db, const String &sql, sqlite3_stmt **stmt, const void **reminder = nullptr);
int32 safe_sqlite_prepare(sqlite3 *db, const void *buffer, int32 bytes, sqlite3_stmt **stmt, const void **reminder = nullptr);
int32 safe_sqlite_step(sqlite3_stmt *stmt);

std::string sqlite_error_code(int32 code);
std::string sqlite_last_error(sqlite3 *db, int32 code);

//////////////////////////////////////////////////////////////////////

} // sqlite

//////////////////////////////////////////////////////////////////////

#endif // _SQLITE_UTILS_H
