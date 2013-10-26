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

#ifndef _OS_CORE_LOG_H
#define _OS_CORE_LOG_H

#include "boost/format.hpp"
#include "boost/utility/enable_if.hpp"
#include "constants.h"
#include "mpl.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

enum LogLevel
{
	logLevelDebug		= 0,
	logLevelNotice		= 1,
	logLevelInfo		= 2,
	logLevelWarning		= 3,
	logLevelError		= 4,
	logLevelCritical	= 5,
	logLevelDefault		= logLevelInfo,
};

void CoreExport log(const String &message, LogLevel level, const std::string &fileName = constants::empty_string, uint32 fileLine = 0, const std::string &functionName = constants::empty_string);

template <typename T>
void log(const T &format, LogLevel level, const std::string &fileName = constants::empty_string, uint32 fileLine = 0, const std::string &functionName = constants::empty_string, typename boost::enable_if<has_string_type<T> >::type * = 0)
{
	log(static_cast<String>(boost::str(format)), level, fileName, fileLine, functionName);	// Supporto boost::format e boost::wformat
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#define OS_LOG_DEBUG(s)			OS_NAMESPACE_NAME::log(s, OS_NAMESPACE_NAME::logLevelDebug, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)
#define OS_LOG_NOTICE(s)		OS_NAMESPACE_NAME::log(s, OS_NAMESPACE_NAME::logLevelNotice, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)
#define OS_LOG_INFO(s)			OS_NAMESPACE_NAME::log(s, OS_NAMESPACE_NAME::logLevelInfo, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)
#define OS_LOG_WARNING(s)		OS_NAMESPACE_NAME::log(s, OS_NAMESPACE_NAME::logLevelWarning, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)
#define OS_LOG_ERROR(s)			OS_NAMESPACE_NAME::log(s, OS_NAMESPACE_NAME::logLevelError, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)
#define OS_LOG_CRITICAL(s)		OS_NAMESPACE_NAME::log(s, OS_NAMESPACE_NAME::logLevelCritical, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_LOG_H
