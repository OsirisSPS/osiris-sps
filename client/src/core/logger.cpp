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
#include "logger.h"

#include "boost/thread/recursive_mutex.hpp"
#include "lock.h"
#include "logmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

Logger::Logger()
{

}

Logger::Logger(const boost::signal<void (shared_ptr<LogMessage> message)>::slot_type &slot)
{
	connect(slot);
}

Logger::~Logger()
{
	disconnect();
}

bool Logger::connected() const
{
	OS_LOCK(getMutex());
	return m_connection.connected();
}

boost::recursive_mutex & Logger::getMutex() const
{
	// N.B.: boost::signal non è threadsafe quidi è necessario effettuare la sincronizzazione su un unico mutex che deve essere condiviso tra tutti i riferimenti al segnale stesso
	return LogManager::instance()->getMutex();	
}

shared_ptr<Logger> Logger::create()
{
	return shared_ptr<Logger>(OS_NEW Logger());
}

shared_ptr<Logger> Logger::create(const boost::signal<void (shared_ptr<LogMessage> message)>::slot_type &slot)
{
	return shared_ptr<Logger>(OS_NEW Logger(slot));
}

void Logger::block()
{
	OS_LOCK(getMutex());
	m_connection.block();
}

void Logger::unblock()
{
	OS_LOCK(getMutex());
	m_connection.unblock();
}

bool Logger::connect(const boost::signal<void (shared_ptr<LogMessage> message)>::slot_type &slot)
{
	disconnect();

	OS_LOCK(getMutex());

	m_connection = LogManager::instance()->connect(slot);
	return m_connection.connected();
}

void Logger::disconnect()
{
	OS_LOCK(getMutex());
	m_connection.disconnect();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
