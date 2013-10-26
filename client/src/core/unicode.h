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

#ifndef _OS_CORE_UNICODE_H
#define _OS_CORE_UNICODE_H

//////////////////////////////////////////////////////////////////////

#ifdef OS_UNICODE
	#define os_chdir			_wchdir
	#define os_finddata			_wfinddata_t
	#define os_findfirst		_wfindfirst
	#define os_findnext			_wfindnext
	#define os_splitpath		_wsplitpath
	#define os_vsnprintf		_vsnwprintf

	#define os_makepath			_wmakepath
	#define os_splitpath		_wsplitpath
//	#define os_sscanf			swscanf
	#define os_strftime			wcsftime

	#define os_sprintf			swprintf
	#define os_vsnprintf		_vsnwprintf

	#define os_fopen			_wfopen

	#define os_strtod			wcstod

	#define os_toi				_wtoi
	#define os_toi64			_wtoi64
	#define os_tol				_wtol

	#define os_ctime			_wctime

	#define os_strcmp			wcscmp
	#define os_strcmpi			_wcsicmp
	#define os_strlen			wcslen
	#define os_strstr			wcsstr
	#define os_strtok			wcstok

	#define os_getenv			_wgetenv
	#define os_putenv			_wputenv

	#define os_vsprintf			vswprintf

    #if OS_COMPILER == OS_COMPILER_MSVC
		#define os_vscprintf	_vscwprintf
	#endif
#else
	#define os_chdir			_chdir
	#define os_finddata			_finddata_t
	#define os_findfirst		_findfirst
	#define os_findnext			_findnext
	#define os_splitpath		_splitpath
	#define os_vsnprintf		_vsnprintf

	#define os_sprintf			sprintf
	#define os_vsnprintf		_vsnprintf

	#define os_makepath			_makepath
	#define os_splitpath		_splitpath
//	#define os_sscanf			sscanf
	#define os_strftime			strftime

	#define os_fopen			fopen

	#define os_strtod           strtod

	#define os_toi				atoi
	#define os_toi64			_atoi64
	#define os_tol				atol

	#define os_ctime			ctime

	#define os_strcmp			strcmp
	#define os_strcmpi			strcmpi
	#define os_strlen			strlen
	#define os_strstr			strstr
	#define os_strtok			strtok

	#define os_getenv			getenv
	#define os_putenv			putenv

	#define os_vsprintf			vsprintf

	#if OS_COMPILER == OS_COMPILER_MSVC
		#define os_vscprintf	_vscprintf
	#endif
#endif // OS_UNICODE

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_UNICODE_H
