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

#ifndef _OS_FIREFOX_STDAFX_H
#define _OS_FIREFOX_STDAFX_H

//////////////////////////////////////////////////////////////////////

// Prerequisites
#include "sdk.h"

#include <string>

//////////////////////////////////////////////////////////////////////

#if defined __WIN32__ || defined _WIN32 || defined WIN32
	#define XP_WIN
	#define XP_WIN32

	#include <windows.h>

	#pragma warning (disable:4311)		// 'type cast' : pointer truncation from <type1> to <type2>
	#pragma warning (disable:4312)		// 'type cast' : conversion from <type1> to <type2> of greater size
	#pragma warning (disable:4244)		// 'argument' : conversion from <type1> to <type2>, possible loss of data
#elif defined __APPLE_CC__
	#define XP_MACOSX

	#include <dlfcn.h>

    #include "xpcom-config.h"
	#undef HAVE_CPP_2BYTE_WCHAR_T
#else
	#define XP_UNIX

	#include <dlfcn.h>

	#include "xpcom-config.h"
	#undef HAVE_CPP_2BYTE_WCHAR_T
#endif

//////////////////////////////////////////////////////////////////////

#include "nsIGenericFactory.h"
#include "nsMemory.h"
#include "nsStringAPI.h"

//////////////////////////////////////////////////////////////////////

#ifdef XP_WIN
	#pragma warning (default:4311)
	#pragma warning (default:4312)
	#pragma warning (default:4244)
#endif // XP_WIN

//////////////////////////////////////////////////////////////////////

#endif // _OS_FIREFOX_STDAFX_H
