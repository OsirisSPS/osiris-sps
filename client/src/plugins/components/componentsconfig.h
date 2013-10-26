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

#ifndef _COMPONENTS_CONFIG_H
#define _COMPONENTS_CONFIG_H

//////////////////////////////////////////////////////////////////////

#include "osiriscommon.h"

#define OS_COMPONENTS_EXTENSION_ID			_S("F81C732A924C42CB32201B84D5372C9322770CDC")

#define OS_COMPONENTS_FOLDER				_S("components")
#define OS_COMPONENTS_DB_FOLDER				_S("db")
#define OS_COMPONENTS_SCHEMA				_S("schema")

#ifdef OS_OSIRIS_VERSION_RAZOR	
	#define OS_COMPONENTS_SCHEMA_VERSION		OS_MAKEVERSION(0, 1)
#else
	#define OS_COMPONENTS_SCHEMA_VERSION		OS_MAKEVERSION(1, 0)
#endif

//////////////////////////////////////////////////////////////////////

#endif // _COMPONENTS_CONFIG_H
