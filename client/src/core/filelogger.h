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

#ifndef _OS_CORE_FILELOGGER_H
#define _OS_CORE_FILELOGGER_H

#include "base/object.h"
#include "iloghandler.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class TextFile;

//////////////////////////////////////////////////////////////////////

class CoreExport FileLogger : public ILogHandler<FileLogger>,
							  public Object
{
// Construction
protected:
	FileLogger(const String &filename = String::EMPTY);

public:
	virtual ~FileLogger();

// Attributes
public:
	inline bool getLogTimestamp() const;
	inline void setLogTimestamp(bool logTimestamp);

	inline bool getLogLevel() const;
	inline void setLogLevel(bool logLevel);

// Operations
public:
	bool open(const String &filename);

// ILogHandler interface
public:
	virtual void log(shared_ptr<LogMessage> message);

private:
	shared_ptr<TextFile> m_file;
	bool m_logTimestamp;
	bool m_logLevel;
};

//////////////////////////////////////////////////////////////////////

inline bool FileLogger::getLogTimestamp() const { return m_logTimestamp; }
inline void FileLogger::setLogTimestamp(bool logTimestamp) { m_logTimestamp = logTimestamp; }

inline bool FileLogger::getLogLevel() const { return m_logLevel; }
inline void FileLogger::setLogLevel(bool logLevel) { m_logLevel = logLevel; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_FILELOGGER_H
