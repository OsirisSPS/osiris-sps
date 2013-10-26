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

#ifndef _OS_CORE_LOGMANAGER_H
#define _OS_CORE_LOGMANAGER_H

#include "base/object.h"
#include "boost/signals.hpp"
#include "constants.h"
#include "format.h"
#include "log.h"
#include "pimpl_ptr.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class LogMessage;

//////////////////////////////////////////////////////////////////////

class CoreExport LogManager : public StaticSingleton<LogManager>,
							  public Object
{
	friend class Logger;	

// Construction
protected:
	LogManager();

public:
	virtual ~LogManager();

// Attributes
public:
	bool getEnabled() const;
	void setEnabled(bool enabled);

	LogLevel getLevel() const;
	void setLevel(LogLevel level);

private:
	boost::recursive_mutex & getMutex();

// Operations
public:
	void init();
	void cleanup();

	// N.B.: non chiamare direttamente questa funzione, utilizzare invece le macro relative
	void log(const String &message, LogLevel level = logLevelDefault, const std::string &fileName = constants::empty_string, uint32 fileLine = 0, const std::string &functionName = constants::empty_string);

private:
	boost::signals::connection connect(const boost::signal<void (shared_ptr<LogMessage>)>::slot_type &slot);	

private:
	pimpl_of<LogManager>::type m_impl;	
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_LOGMANAGER_H
