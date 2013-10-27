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

#ifndef _OS_ENGINE_OSIRIS_H
#define _OS_ENGINE_OSIRIS_H

//////////////////////////////////////////////////////////////////////

#define OS_OSIRIS_APPLICATION_NAME				_S("Osiris")
#define OS_OSIRIS_VERSION_MAJOR					1
#define OS_OSIRIS_VERSION_MINOR					0

#if (OS_OSIRIS_VERSION_MAJOR>=1) // FORCOMPATIBILITY
	#define	OS_OSIRIS_VERSION_RAZOR					
#endif

#define OS_OSIRIS_VERSION()						OS_MAKEVERSION(OS_OSIRIS_VERSION_MINOR, OS_OSIRIS_VERSION_MAJOR)

#define OS_OSIRIS_PROCESS_GUID					"56B525A5-E871-4C2C-BA4C-101C7E41F8D9"

#define OS_DATA_PATH							_S("data")
#define OS_DB_PATH								_S("db")
#define OS_ERRORS_PATH							_S("errors")
#define OS_HTDOCS_PATH							_S("htdocs")
#define OS_LANGUAGES_PATH						_S("languages")
#define OS_PORTALS_PATH							_S("portals")
#define OS_PLUGINS_PATH							_S("plugins")
#define OS_SCHEMAS_PATH							_S("schemas")
#define OS_CONFIG_PATH							_S("config")
#define OS_ROOT_PATH							_S("osiris")
#define OS_UPDATES_PATH							_S("updates")
#define OS_STORE_PATH							_S("store")

#define OS_FAVICON								_S("favicon.ico")

#define OS_SKINBASE								_S("base")

#define OS_LOG_FILENAME							_S("osiris-sps.log")

#define OS_CULTURESXML							_S("cultures.xml")
#define OS_SKINXML								_S("skin.xml")
#define OS_MANIFESTXML							_S("manifest.xml")

#define OS_TIME_DETECTION_SYSTEM				10000000
#define OS_TIME_DETECTION_CULTURE				10000001

#define OS_DEFAULT_LANGUAGE						_S("")
#define OS_DEFAULT_ALTERNATIVE_LANGUAGE			_S("en-US")
#define OS_DEFAULT_TIME_OFFSET					OS_TIME_DETECTION_SYSTEM		// Seconds, or OS_TIME_DETECTION_SYSTEM, or OS_TIME_DETECTION_CULTURE
#define OS_DEFAULT_TIME_DST						OS_TIME_DETECTION_SYSTEM		// Seconds, or OS_TIME_DETECTION_SYSTEM

#define OS_REPUTATION_MIN						-1
#define OS_REPUTATION_MAX						1

#define OS_DEFAULT_PEERSREQUEST					5
#define OS_MAX_PEERSREQUEST						50

#define OS_ENTRYRANK_DECREASEFACTOR				1
#define OS_ENTRYRANK_INCREASEFACTOR				3

#ifdef OS_DEBUG
	//#define OS_ENABLE_NTP_DIFFERENCES
#endif

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_OSIRIS_H
