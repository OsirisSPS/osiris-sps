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

#ifndef _OS_CORE_ENVIRONMENT_H
#define _OS_CORE_ENVIRONMENT_H

//////////////////////////////////////////////////////////////////////

#define OS_PLATFORM_WIN					1
#define OS_PLATFORM_LINUX				2
#define OS_PLATFORM_MAC					3

#if defined _WINDOWS || defined __WIN32__ || defined _WIN32 || defined WIN32
	#define OS_PLATFORM					OS_PLATFORM_WIN
#elif defined __APPLE_CC__
	#define OS_PLATFORM					OS_PLATFORM_MAC
#else
	#define OS_PLATFORM					OS_PLATFORM_LINUX
#endif

//////////////////////////////////////////////////////////////////////

#define OS_ARCHITECTURE_UNKNOWN		0
#define OS_ARCHITECTURE_I386		1
#define OS_ARCHITECTURE_X86_64		2

#if OS_PLATFORM == OS_PLATFORM_WIN
	#ifdef _WIN64
		#define OS_ARCHITECTURE OS_ARCHITECTURE_X86_64			
	#elif _WIN32
		#define OS_ARCHITECTURE OS_ARCHITECTURE_I386		
	#else
		#define OS_ARCHITECTURE OS_ARCHITECTURE_UNKNOWN
	#endif
#elif OS_PLATFORM == OS_PLATFORM_MAC
	#ifdef __LP64__
		#define OS_ARCHITECTURE OS_ARCHITECTURE_X86_64			
	#elif __LP32__
		#define OS_ARCHITECTURE OS_ARCHITECTURE_I386		
	#else
		#define OS_ARCHITECTURE OS_ARCHITECTURE_UNKNOWN
	#endif
#elif OS_PLATFORM == OS_PLATFORM_LINUX
	#ifdef __x86_64__
		#define OS_ARCHITECTURE OS_ARCHITECTURE_X86_64			
	#elif __i386__
		#define OS_ARCHITECTURE OS_ARCHITECTURE_I386		
	#else
		#define OS_ARCHITECTURE OS_ARCHITECTURE_UNKNOWN
	#endif
#else
	#define OS_ARCHITECTURE OS_ARCHITECTURE_UNKNOWN
#endif

//////////////////////////////////////////////////////////////////////

#define OS_COMPILER_MSVC				1
#define OS_COMPILER_GCC					2

#if defined _MSC_VER
	#define OS_COMPILER					OS_COMPILER_MSVC	
#elif defined __GNUC__
	#define OS_COMPILER					OS_COMPILER_GCC	
#else
	#error "Unknown compiler"
#endif

//////////////////////////////////////////////////////////////////////

#define OS_ENDIANITY_LITTLE				0
#define OS_ENDIANITY_BIG				1

//////////////////////////////////////////////////////////////////////

#include "buildoptions.h"

//////////////////////////////////////////////////////////////////////

#if OS_PLATFORM == OS_PLATFORM_WIN
	#include "configplatformwin32.h"
#elif OS_PLATFORM == OS_PLATFORM_LINUX
	#include "configplatformlinux.h"
#elif OS_PLATFORM == OS_PLATFORM_MAC
	#include "configplatformmac.h"
#endif

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#include "configcompilermsvc.h"
#elif OS_COMPILER == OS_COMPILER_GCC
	#include "configcompilergcc.h"
#endif

//////////////////////////////////////////////////////////////////////

#if (!defined OS_ENDIANITY) || (OS_ENDIANITY == OS_ENDIANITY_BIG)
	#error "Unsupported endianity"
#endif

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ENVIRONMENT_H
