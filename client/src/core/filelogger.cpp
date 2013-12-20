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
#include "filelogger.h"

#include "algorithms.h"
#include "logmessage.h"
#include "textfile.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

FileLogger::FileLogger(const String &filename) : m_logTimestamp(true),
												 m_logLevel(true)
{
	if(filename.empty() == false)
		open(filename);
}

FileLogger::~FileLogger()
{

}

bool FileLogger::open(const String &filename)
{
	m_file.reset(OS_NEW TextFile(TextFile::feUtf8));
	return m_file->open(filename, TextFile::ofWrite);
}

void FileLogger::log(shared_ptr<LogMessage> message)
{
	OS_ASSERT(message != nullptr);
	OS_ASSERT(m_file != nullptr && m_file->is_open());

	String line;
	if(getLogTimestamp())
	{
		line.append(boost::posix_time::to_simple_string(utils::ptime_truncate_milliseconds(message->getTimestamp())));
		line.append(" ");
	}

	if(getLogLevel())
	{
		switch(message->getLevel())
		{
		case logLevelDebug:		line.append("[debug]");
								break;

		case logLevelNotice:	line.append("[notice]");
								break;

		case logLevelInfo:		line.append("[info]");
								break;

		case logLevelWarning:	line.append("[warning]");
								break;

		case logLevelError:		line.append("[error]");
								break;

		case logLevelCritical:	line.append("[critical]");
								break;

		default:				OS_ASSERTFALSE();
								line.append("[unknown]");
								break;
		}
		
		line.append(" ");
	}

	line.append(algorithms::trim_copy(message->getMessage()));
	m_file->writeLine(line);
	m_file->flush();	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
