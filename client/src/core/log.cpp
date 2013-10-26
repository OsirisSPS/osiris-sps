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

#include "stdafx.h"
#include "log.h"

#include "logmanager.h"
#include "conversions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((conversions))

//////////////////////////////////////////////////////////////////////

template<>
CoreExport String to_utf16<LogLevel>(const LogLevel &v)
{
	return to_utf16<uint32>(static_cast<uint32>(v));
}

template<>
CoreExport LogLevel from_utf16<LogLevel>(const String &v)
{
	uint32 level = from_utf16<uint32>(v);
	switch(level)
	{
	case logLevelDebug:
	case logLevelNotice:
	case logLevelInfo:
	case logLevelWarning:
	case logLevelError:
	case logLevelCritical:
                            return static_cast<LogLevel>(level);

	default:			    OS_ASSERTFALSE();
                            break;
	}

	return logLevelDefault;
}

template<>
CoreExport LogLevel toEnumeratedType(uint32 v)
{
	switch(v)
	{
	case logLevelDebug:
	case logLevelNotice:
	case logLevelInfo:
	case logLevelWarning:
	case logLevelError:
	case logLevelCritical:
							return static_cast<LogLevel>(v);

	default:				OS_ASSERTFALSE();
							break;
	}

	return logLevelDefault;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((conversions))

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

void CoreExport log(const String &message, LogLevel level, const std::string &fileName, uint32 fileLine, const std::string &functionName)
{
	LogManager::instance()->log(message, level, fileName, fileLine, functionName);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
