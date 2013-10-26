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
#include "timerperformance.h"

#include "boost/thread/recursive_mutex.hpp"
#include "lock.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

TimerPerformance::TimerPerformance()
{
	m_tickStart = 0;
}

TimerPerformance::TimerPerformance(const String &desc, double minForNotify)
{
	reset(desc);
	m_minForNotify = minForNotify;
}

TimerPerformance::~TimerPerformance()
{
	stop();
}

void TimerPerformance::stop()
{
	if(m_tickStart != 0)
	{
		TimerPerformanceManager::addInfo(this);
		m_tickStart = 0;
	}
}

void TimerPerformance::reset(const String &desc)
{
	if(desc.empty() == false)
		m_description = desc;

	m_tickStart = PlatformManager::instance()->tickCount();

	m_ownerThread = PlatformManager::instance()->getCurrentThreadID();

	TimerPerformanceManager::addPending(this);
}

//////////////////////////////////////////////////////////////////////

TimerPerformanceInfo::TimerPerformanceInfo()
{
	m_nSamples = 0;
	m_totalTick = 0;
	m_firstTick = 0;
	m_lastTick = 0;

	m_minElapsed = 0;
	m_maxElapsed = 0;
	m_lastElapsed = 0;
}

//////////////////////////////////////////////////////////////////////

TimerPerformanceManager::InfosMap TimerPerformanceManager::m_infos;
TimerPerformanceManager::PendingsMap TimerPerformanceManager::m_pendings;
boost::recursive_mutex TimerPerformanceManager::m_cs;

void TimerPerformanceManager::addInfo(TimerPerformance *timer)
{
	OS_LOCK(m_cs);

	removePending(timer);

	String desc = timer->getDescription();
	double t = PlatformManager::instance()->tickCount();
	double elapsed = t - timer->getTickStart();
	if(elapsed<timer->getMinForNotify())
		return;

	InfosMap::iterator i = m_infos.find(desc);

	TimerPerformanceInfo info;
	if (i != m_infos.end())
	{
		 info = i->second;
	}
	else
	{
		info.m_firstTick = timer->getTickStart();
		info.m_minElapsed = elapsed;
		info.m_maxElapsed = elapsed;
	}

	info.m_lastTick = t;

	info.m_nSamples++;
	info.m_totalTick += elapsed;
	info.m_lastElapsed = elapsed;
	info.m_minElapsed = std::min<double>(info.m_minElapsed, elapsed);
	info.m_maxElapsed = std::max<double>(info.m_maxElapsed, elapsed);

	m_infos[desc] = info;
}

void TimerPerformanceManager::addPending(TimerPerformance *timer)
{
	OS_LOCK(m_cs);

	m_pendings[timer]= timer;
}

void TimerPerformanceManager::removePending(TimerPerformance *timer)
{
	OS_LOCK(m_cs);

	PendingsMap::iterator i = m_pendings.find(timer);

	if(i != m_pendings.end())
		m_pendings.erase(i);
}

String TimerPerformanceManager::toString()
{
	if (m_infos.empty()) return String::EMPTY;

	OS_LOCK(m_cs);

	String output;

	output += _S("TimerPerformanceManager:\tnSamp.\tMin(ms)\tMax(ms)\tAvg(ms)\tLast(ms)\tTotal(ms)\tRange(ms)\r\n");
	output += _S("---------------------------------------------------------------------------------\r\n");

	for(InfosMap::const_iterator i = m_infos.begin(); i != m_infos.end(); ++i)
	{
		String desc = i->first;
		TimerPerformanceInfo info = i->second;

		output += desc + _S("\r\n");

		output += String::format(_S("\t\t\t%06d\t%06.4f\t%06.4f\t%06.4f\t%06.4f\t%06.4f\t%06.4f\r\n").c_str(), info.m_nSamples,info.m_minElapsed,info.m_maxElapsed,double(info.m_totalTick/info.m_nSamples),info.m_lastElapsed,info.m_totalTick,info.m_lastTick-info.m_firstTick);
		output += _S("---------------------------------------------------------------------------------\r\n");
	}

	return output;
}

String TimerPerformanceManager::toHtml()
{
	if (m_infos.empty()) return String::EMPTY;

	OS_LOCK(m_cs);

	String output;

	// Infos
	output += _S("<table>");
	output += _S("<tr>");
	output += _S("<th>Name:</th><th>nSamp.</th><th>Min(ms)</th><th>Max(ms)</th><th>Avg(ms)</th><th>Last(ms)</th><th>Total(ms)</th><th>Range(ms)</th>");
	output += _S("</tr>");

	for(InfosMap::const_iterator i = m_infos.begin(); i != m_infos.end(); ++i)
	{
		String desc = i->first;
		TimerPerformanceInfo info = i->second;

		output += _S("<tr>");
		output += String::format(_S("<td>%S</td><td>%06d</td><td>%06.4f</td><td>%06.4f</td><td>%06.4f</td><td>%06.4f</td><td>%06.4f</td><td>%06.4f</td>").c_str(), desc.c_str(), info.m_nSamples, info.m_minElapsed, info.m_maxElapsed, double(info.m_totalTick/info.m_nSamples), info.m_lastElapsed, info.m_totalTick, info.m_lastTick-info.m_firstTick);
		output += _S("</tr>");
	}

	output += _S("</table>");

	// Pendings
	output += _S("<table>");
	output += _S("<tr>");
	output += _S("<th>Name</th><th>Delay</th><th>Thread</th>");
	output += _S("</tr>");

	double currentTick = PlatformManager::instance()->tickCount();

	for(PendingsMap::const_iterator i = m_pendings.begin(); i != m_pendings.end(); ++i)
	{
		TimerPerformance *timer = i->second;

		String desc = timer->getDescription();
		double delay = currentTick - timer->getTickStart();

		boost::any threadID = timer->getOwnerThread();
		// Tenta un cast a uint32
		uint32 *thread = boost::any_cast<uint32>(&threadID);

		output += _S("<tr>");
		output += String::format(_S("<td>%S</td><td>%06.4f</td><td>%u</td>").c_str(), desc.c_str(), delay, thread != null ? *thread : 0);
		output += _S("</tr>");
	}

	output += _S("</table>");

	return output;
}

void TimerPerformanceManager::trace()
{
	OS_TRACE(toString().to_ascii());
}

void TimerPerformanceManager::reset(const String &desc)
{
	OS_LOCK(m_cs);

	InfosMap::iterator i = m_infos.find(desc);

	if(i != m_infos.end())
		m_infos.erase(i);
}

void TimerPerformanceManager::resetAll()
{
	OS_LOCK(m_cs);

	m_infos.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
