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

#ifndef _OS_CORE_LOGMESSAGE_H
#define _OS_CORE_LOGMESSAGE_H

#include "base/object.h"
#include "boost/operators.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "log.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport LogMessage : public boost::equality_comparable<LogMessage>,
							  public Object
{
	typedef Object BaseClass;

// Construction
public:
	LogMessage(const String &message, LogLevel level, const boost::posix_time::ptime &timestamp, const std::string &fileName, uint32 fileLine, const std::string &functionName);
	LogMessage(const LogMessage &second);
	virtual ~LogMessage();

// Attributes
public:
	// Restituisce il contenuto del messaggio
	inline const String & getMessage() const;
	// Restituisce il livello del messaggio
	inline LogLevel getLevel() const;
	// Restituisce il timestamp del messaggio
	inline const boost::posix_time::ptime & getTimestamp() const;
	// Restituisce il file di riferimento
	inline const std::string & getFileName() const;
	// Restituisce la linea di riferimento
	inline uint32 getFileLine() const;
	// Restituisce la funzione di riferimento
	inline const std::string & getFunctionName() const;
	
// Operators
public:
	LogMessage & operator =(const LogMessage &second);
	bool operator ==(const LogMessage &second) const;	

protected:
	String m_message;
	LogLevel m_level;	
	boost::posix_time::ptime m_timestamp;
	std::string m_fileName;
	uint32 m_fileLine;
	std::string m_functionName;
};

//////////////////////////////////////////////////////////////////////

inline const String & LogMessage::getMessage() const { return m_message; }
inline LogLevel LogMessage::getLevel() const { return m_level; }
inline const boost::posix_time::ptime & LogMessage::getTimestamp() const { return m_timestamp; }
inline const std::string & LogMessage::getFileName() const { return m_fileName; }
inline uint32 LogMessage::getFileLine() const { return m_fileLine; }
inline const std::string & LogMessage::getFunctionName() const { return m_functionName; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_LOGMESSAGE_H
