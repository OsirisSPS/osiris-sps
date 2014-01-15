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

#ifndef _OS_CORE_CONFIGCOMPILERGCC_H
#define _OS_CORE_CONFIGCOMPILERGCC_H

#include "configcompilergccwarnings.h"

//////////////////////////////////////////////////////////////////////

#if defined __BIG_ENDIAN__
	#define OS_ENDIANITY					OS_ENDIANITY_BIG
#else
	#define OS_ENDIANITY					OS_ENDIANITY_LITTLE
#endif

#if __GNUC__ >= 4 && !defined(OS_GCCXML)
	#define OS_SUPPORT_CPP0X
#endif

#define OS_FORCEINLINE						__attribute__((always_inline))

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_CONFIGCOMPILERGCC_H
