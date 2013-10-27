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

#ifndef _SQLITE_MACROS_H
#define _SQLITE_MACROS_H

//////////////////////////////////////////////////////////////////////

#ifdef OS_UNICODE
	#define _tsqlite3_column_name			(const uchar *)sqlite3_column_name16
	#define _tsqlite3_prepare				sqlite3_prepare16
	#define _tsqlite3_open					sqlite3_open16
	#define _tsqlite3_column_text			(const uchar *)sqlite3_column_text16
	#define _tsqlite3_errmsg				(const uchar *)sqlite3_errmsg16
#else
	#define _tsqlite3_column_name			sqlite3_column_name
	#define _tsqlite3_prepare				sqlite3_prepare
	#define _tsqlite3_open					sqlite3_open
	#define _tsqlite3_column_text			sqlite3_column_text
	#define _tsqlite3_errmsg				sqlite3_errmsg
#endif // OS_UNICODE

//////////////////////////////////////////////////////////////////////

#endif // _SQLITE_MACROS_H
