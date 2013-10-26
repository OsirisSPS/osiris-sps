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

#ifndef _OS_CORE_MACROS_H
#define _OS_CORE_MACROS_H

//////////////////////////////////////////////////////////////////////

#include "boost/current_function.hpp"
#include "boost/preprocessor/seq/for_each.hpp"
#include "boost/preprocessor/seq/push_front.hpp"

//////////////////////////////////////////////////////////////////////

#define OS_CURRENT_FILE                     __FILE__
#define OS_CURRENT_LINE                     __LINE__
#define OS_CURRENT_FUNCTION					BOOST_CURRENT_FUNCTION

#define OS_ASSERT(exp)						BOOST_ASSERT(exp)
#define OS_ASSERTFALSE()					OS_ASSERT(false)

#define OS_STATIC_ASSERT(exp)				BOOST_STATIC_ASSERT(exp)

#ifdef OS_INIT_PLATFORM_LEAKS_DETECTION
	#define OS_INIT_LEAKS_DETECTION()		OS_INIT_PLATFORM_LEAKS_DETECTION
#else
	#define OS_INIT_LEAKS_DETECTION()
#endif // OS_INIT_PLATFORM_LEAKS_DETECTION

#ifdef OS_DEBUG
	#define OS_VERIFY(exp)					OS_ASSERT(exp)
#else
	#define OS_VERIFY(exp)					((void)(exp))
#endif // OS_DEBUG

#define OS_MAKE_UINT8(l, h)					((uint8)(((uint8)(l)) & 0xF) | (((uint8)(h)) << 4))
#define OS_UINT8_LO(b)						((uint8)(((uint8)(b)) & 0xF))
#define OS_UINT8_HI(b)						((uint8)(((uint8)(b)) >> 4))

#define OS_MAKE_UINT16(lo, hi)				((uint16)(((uint8)((uint32)(lo) & 0xff)) | ((uint16)((uint8)((uint32)(hi) & 0xff))) << 8))
#define OS_UINT16_LO(v)						((uint8)((uint32)(v) & 0xff))
#define OS_UINT16_HI(v)						((uint8)((uint32)(v) >> 8))

#define OS_MAKE_UINT32(lo, hi)				((uint32)(((uint16)((uint32)(lo) & 0xffff)) | ((uint32)((uint16)((uint32)(hi) & 0xffff))) << 16))
#define OS_UINT32_LO(v)						((uint16)((uint32)(v) & 0xffff))
#define OS_UINT32_HI(v)						((uint16)((uint32)(v) >> 16))

#define OS_MAKE_UINT64(lo, hi)				((uint64)(((uint32)((uint64)(lo) & 0xffffffff)) | ((uint64)((uint32)((uint64)(hi) & 0xffffffff))) << 32))
#define OS_UINT64_LO(v)						((uint32)((uint64)(v) & 0xffffffff))
#define OS_UINT64_HI(v)						((uint32)((uint64)(v) >> 32))

#define OS_MAKEVERSION(l, h)				OS_MAKE_UINT16(l, h)
#define OS_VERSION_MINOR(v)					OS_UINT16_LO(v)
#define OS_VERSION_MAJOR(v)					OS_UINT16_HI(v)

#define OS_ZEROMEMORY(Dest, Len)			memset((Dest), 0, (Len))
#define OS_COUNTOF(array)					(sizeof(array) / sizeof(array[0]))

#define OS_ERR_UNKNOWN(c)					(_S("Unknown ") + _S(#c) + _S(" error"))

#define OS_BOOL_CAST(x)						((x) ? true : false)

#define OS_NAMESPACE_NAME					osiris
#define OS_NAMESPACE_OPEN_MACRO(r, p, n)	namespace n {
#define OS_NAMESPACE_OPEN_REAL(n)			BOOST_PP_SEQ_FOR_EACH(OS_NAMESPACE_OPEN_MACRO, _, n)
#define OS_NAMESPACE_OPEN(n)				OS_NAMESPACE_OPEN_REAL(BOOST_PP_SEQ_PUSH_FRONT(n, OS_NAMESPACE_NAME))
#define OS_NAMESPACE_CLOSE_MACRO(r, p, n)	}
#define OS_NAMESPACE_CLOSE_REAL(n)			BOOST_PP_SEQ_FOR_EACH(OS_NAMESPACE_CLOSE_MACRO, _, n)
#define OS_NAMESPACE_CLOSE(n)				OS_NAMESPACE_CLOSE_REAL(BOOST_PP_SEQ_PUSH_FRONT(n, OS_NAMESPACE_NAME))
#define OS_NAMESPACE_BEGIN()				OS_NAMESPACE_OPEN(BOOST_PP_SEQ_NIL)
#define OS_NAMESPACE_END()					OS_NAMESPACE_CLOSE(BOOST_PP_SEQ_NIL)

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_MACROS_H
