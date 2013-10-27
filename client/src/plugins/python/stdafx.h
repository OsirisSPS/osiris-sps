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

#ifndef _OSIRIS_PYTHON_STDAFX_H
#define _OS_PYTHON_STDAFX_H

//////////////////////////////////////////////////////////////////////

#include "archivesconfig.h"
#include "coreconfig.h"
#include "dbconfig.h"
#include "engineconfig.h"
#include "httpconfig.h"
#include "langconfig.h"
#include "netconfig.h"
#include "webconfig.h"
#include "xmlconfig.h"

#include "boostpython.h"

//////////////////////////////////////////////////////////////////////

// N.B.: le seguenti macro sono necessarie in quanto py++ genera le versioni espanse dei templates e generando i wrappers su piattaforme
// diverse è possibile che vengano usati allocatori diversi (comportando così errori di compilazione). 

#if OS_ALLOCATOR_TYPE != OS_ALLOCATOR_TYPE_NED
    #define NedAllocator OS_DEFAULT_ALLOCATOR
#endif

#if OS_ALLOCATOR_TYPE != OS_ALLOCATOR_TYPE_SYS
    #define SysAllocator OS_DEFAULT_ALLOCATOR
#endif

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_STDAFX_H
