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

#ifndef _OS_CORE_BUILDOPTIONS_H
#define _OS_CORE_BUILDOPTIONS_H

//////////////////////////////////////////////////////////////////////

#if defined UNICODE || defined _UNICODE
	#ifndef _UNICODE
		#define _UNICODE
	#endif
	#ifndef UNICODE
		#define UNICODE
	#endif

	#define OS_UNICODE
#else
	#define OS_ANSI
#endif

#if defined DEBUG || defined _DEBUG
	#ifndef _DEBUG
		#define _DEBUG		1
	#endif
	#ifndef DEBUG
		#define DEBUG		1
	#endif

	#define OS_DEBUG
#else
	#ifndef NDEBUG
		#define NDEBUG
	#endif

	#define OS_NDEBUG
#endif

//////////////////////////////////////////////////////////////////////

#ifdef OS_DEBUG
	#define OS_ENABLE_LEAKS_DETECTION
#endif // OS_DEBUG

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_BUILDOPTIONS_H
