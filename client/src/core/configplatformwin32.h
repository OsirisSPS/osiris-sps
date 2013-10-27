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

#ifndef _OS_CORE_CONFIGPLATFORMWIN32_H
#define _OS_CORE_CONFIGPLATFORMWIN32_H

//////////////////////////////////////////////////////////////////////

#ifdef OS_ENABLE_LEAKS_DETECTION
	// N.B.: creano conflitti con eventuali metodi "malloc" e "free" (es.: boost::pool)
//	#define _CRTDBG_MAP_ALLOC 
//	#define _CRTDBG_MAP_ALLOC_NEW
	#include <crtdbg.h>

	#define OS_INIT_PLATFORM_LEAKS_DETECTION _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#endif

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
#endif

#ifndef WINVER
	#define WINVER 0x0501
#endif

#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef _WINDOWS_
    #define _WINDOWS_
#endif

#define OS_MODULE_ENTRY_POINT()			BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return 1; }

#define OS_SHARED_LIBRARY_EXTENSION		_S("dll")

#define OS_DYNAMIC_LIBRARY_EXPORT		_declspec(dllexport)
#define OS_DYNAMIC_LIBRARY_IMPORT		_declspec(dllimport)

#define OS_STATIC_LIBRARY_EXPORT
#define OS_STATIC_LIBRARY_IMPORT

#ifdef OS_STATIC_LIBRARY
	#define OS_LIBRARY_EXPORT			OS_STATIC_LIBRARY_EXPORT
	#define OS_LIBRARY_IMPORT			OS_STATIC_LIBRARY_IMPORT
#else
	#define OS_LIBRARY_EXPORT			OS_DYNAMIC_LIBRARY_EXPORT
	#define OS_LIBRARY_IMPORT			OS_DYNAMIC_LIBRARY_IMPORT
#endif

#define OS_HAS_NATIVE_UCHAR

#define OS_MAX_PATH						MAX_PATH

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_CONFIGPLATFORMWIN32_H
