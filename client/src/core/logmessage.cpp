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
#include "logmessage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

LogMessage::LogMessage(const String &message, LogLevel level, const boost::posix_time::ptime &timestamp, const std::string &fileName, uint32 fileLine, const std::string &functionName) : m_message(message),
																																														  m_level(level),
																																													      m_timestamp(timestamp),
																																														  m_fileName(fileName),
																																														  m_fileLine(fileLine),
																																														  m_functionName(functionName)
{

}

LogMessage::LogMessage(const LogMessage &second) : BaseClass(second)
{
	*this = second;
}

LogMessage::~LogMessage()
{

}

LogMessage & LogMessage::operator =(const LogMessage &second)
{
	m_message = second.m_message;
	m_level = second.m_level;
	m_timestamp = second.m_timestamp;
	m_fileName = second.m_fileName;
	m_fileLine = second.m_fileLine;
	m_functionName = second.m_functionName;

	return *this;
}

bool LogMessage::operator ==(const LogMessage &second) const
{
	if(m_message != second.m_message)
		return false;

	if(m_level != second.m_level)
		return false;

	if(m_timestamp != second.m_timestamp)
		return false;

	if(m_fileName != second.m_fileName)
		return false;

	if(m_fileLine != second.m_fileLine)
		return false;

	if(m_functionName != second.m_functionName)
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
