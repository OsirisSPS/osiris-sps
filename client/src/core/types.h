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

#ifndef _OS_CORE_TYPES_H
#define _OS_CORE_TYPES_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

#if OS_PLATFORM == OS_PLATFORM_WIN
	typedef __int8					int8;
	typedef __int16					int16;
	typedef __int32					int32;
	typedef __int64					int64;
	typedef unsigned __int8			uint8;
	typedef unsigned __int16		uint16;
	typedef unsigned __int32		uint32;
	typedef unsigned __int64		uint64;

	typedef SOCKET					socket_t;
#else
	typedef int8_t		 			int8;
	typedef int16_t		 			int16;
	typedef int32_t					int32;
	typedef int64_t			 		int64;
	typedef uint8_t		 			uint8;
	typedef uint16_t	 			uint16;
	typedef uint32_t	 			uint32;
	typedef uint64_t				uint64;

	typedef int						socket_t;
#endif

typedef uint8						byte;
typedef uint16						word;
typedef uint32						dword;

typedef void *						handle;

typedef char						achar;
typedef wchar_t						wchar;

#ifdef OS_HAS_NATIVE_UCHAR
	typedef wchar					uchar;
#else
	typedef uint16					uchar;
#endif

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_TYPES_H
