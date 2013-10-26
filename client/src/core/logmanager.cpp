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
#include "logmanager.h"

#include "boost/bind.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "condition.h"
#include "lock.h"
#include "logmessage.h"
#include "platformmanager.h"
#include "thread.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

#define OS_LOGMANAGER_THREAD_DELAY		1000

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<LogManager> : public Thread
{
	typedef list<shared_ptr<LogMessage> >::type LogMessages;

	typedef Thread ThreadBase;

// Construction
public:
	pimpl();
	~pimpl();

// Attributes
public:
	bool getEnabled() const;
	void setEnabled(bool enabled);

	LogLevel getLevel() const;
	void setLevel(LogLevel level);

	boost::recursive_mutex & getMutex();

// Operations
public:
	boost::signals::connection connect(const boost::signal<void (shared_ptr<LogMessage> message)>::slot_type &slot);
	void log(const String &message, LogLevel level, const std::string &fileName, uint32 fileLine, const std::string &functionName);
	bool flush();

private:
	shared_ptr<LogMessage> peekMessage();

// Thread overrides
public:
	virtual void stop();

// Thread interface
protected:
	virtual void run();

private:
	mutable boost::recursive_mutex m_dataCS;
	bool m_enabled;
	LogLevel m_level;
	LogMessages m_messages;
	boost::recursive_mutex m_signalCS;
	boost::signal<void (shared_ptr<LogMessage>)> m_signal;
	Condition m_flushCondition;
};

//////////////////////////////////////////////////////////////////////

pimpl<LogManager>::pimpl()
{
	m_enabled = true;

#ifdef OS_DEBUG
	m_level = logLevelDebug;
#else
	m_level = logLevelInfo;
#endif // OS_DEBUG

	setPriority(taskPriorityIdle);
}

pimpl<LogManager>::~pimpl()
{

}

bool pimpl<LogManager>::getEnabled() const
{
	OS_LOCK(m_dataCS);
	return m_enabled;
}

void pimpl<LogManager>::setEnabled(bool enabled)
{
	OS_LOCK(m_dataCS);
	m_enabled = enabled;
}

LogLevel pimpl<LogManager>::getLevel() const
{
	OS_LOCK(m_dataCS);
	return m_level;
}

void pimpl<LogManager>::setLevel(LogLevel level)
{
	OS_LOCK(m_dataCS);
	m_level = level;
}

boost::recursive_mutex & pimpl<LogManager>::getMutex()
{
	return m_signalCS;
}

boost::signals::connection pimpl<LogManager>::connect(const boost::signal<void (shared_ptr<LogMessage>)>::slot_type &slot)
{
	OS_LOCK(m_signalCS);
	return m_signal.connect(slot);
}

void pimpl<LogManager>::log(const String &message, LogLevel level, const std::string &fileName, uint32 fileLine, const std::string &functionName)
{
	{
		OS_LOCK(m_dataCS);

		if(m_enabled == false || level < m_level)
			return;

		OS_ASSERT(running());
		m_messages.push_back(shared_ptr<LogMessage>(OS_NEW LogMessage(message, level, utils::utc_time(), fileName, fileLine, functionName)));

		OS_TRACE_CUSTOM(message.to_ascii(), fileName.c_str(), fileLine, functionName.c_str());
		OS_TRACE("\n");
	}

	m_flushCondition.signal();
}

bool pimpl<LogManager>::flush()
{
	shared_ptr<LogMessage> message = peekMessage();
	if(message == null)
		return false;

	while(message != null)
	{
		{
			OS_LOCK(m_signalCS);
			m_signal(message);
		}

		message = peekMessage();
	}

	return true;
}

shared_ptr<LogMessage> pimpl<LogManager>::peekMessage()
{
	OS_LOCK(m_dataCS);

	if(m_messages.empty())
		return null;

	shared_ptr<LogMessage> message = m_messages.front();
	m_messages.pop_front();

	return message;
}

void pimpl<LogManager>::stop()
{
	m_flushCondition.reset();

	ThreadBase::stop();
}

void pimpl<LogManager>::run()
{
	while(stopped() == false)
	{
		flush();
		m_flushCondition.wait(OS_LOGMANAGER_THREAD_DELAY);
	}
}

//////////////////////////////////////////////////////////////////////

LogManager::LogManager()
{
	
}

LogManager::~LogManager()
{

}

bool LogManager::getEnabled() const
{
	return m_impl->getEnabled();
}

void LogManager::setEnabled(bool enabled)
{
	m_impl->setEnabled(enabled);
}

LogLevel LogManager::getLevel() const
{
	return m_impl->getLevel();
}

void LogManager::setLevel(LogLevel level)
{
	m_impl->setLevel(level);
}

boost::recursive_mutex & LogManager::getMutex()
{
	return m_impl->getMutex();
}

void LogManager::init()
{
	if(getEnabled())
		m_impl->start();
}

void LogManager::cleanup()
{
	m_impl->stop();
	m_impl->flush();
}

void LogManager::log(const String &message, LogLevel level, const std::string &fileName, uint32 fileLine, const std::string &functionName)
{
	m_impl->log(message, level, fileName, fileLine, functionName);
}

boost::signals::connection LogManager::connect(const boost::signal<void (shared_ptr<LogMessage> message)>::slot_type &slot)
{
	return m_impl->connect(slot);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
