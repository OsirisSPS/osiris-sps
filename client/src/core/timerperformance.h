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

#ifndef _OS_CORE_TIMERPERFORMANCE_H
#define _OS_CORE_TIMERPERFORMANCE_H

#include "boost/any.hpp"
#include "boost/thread/recursive_mutex.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport TimerPerformance
{
// Construction
public:
	TimerPerformance();
	TimerPerformance(const String &desc, double minForNotify = 0);
	~TimerPerformance();

// Attributes
public:	
	inline String getDescription() const;
	inline double getTickStart() const;
	inline boost::any getOwnerThread() const;
	inline double getMinForNotify() const;

// Operations
public:
	void reset(const String &desc);
	void stop();
	
// Internal:
protected:
	String m_description;
	double m_tickStart;
	boost::any m_ownerThread;
	double m_minForNotify;
};

//////////////////////////////////////////////////////////////////////

inline String TimerPerformance::getDescription() const { return m_description; }
inline double TimerPerformance::getTickStart() const { return m_tickStart; }
inline boost::any TimerPerformance::getOwnerThread() const { return m_ownerThread; }
inline double TimerPerformance::getMinForNotify() const { return m_minForNotify; }

//////////////////////////////////////////////////////////////////////

class TimerPerformanceInfo
{
public:
	TimerPerformanceInfo();

	int m_nSamples;
	double m_totalTick;
	double m_firstTick;
	double m_lastTick;
	double m_minElapsed;
	double m_maxElapsed;
	double m_lastElapsed;
	double m_minToNotify;
};

//////////////////////////////////////////////////////////////////////

class CoreExport TimerPerformanceManager
{
	typedef map<String, TimerPerformanceInfo>::type InfosMap;
	typedef map<void *, TimerPerformance *>::type PendingsMap;	

// Operations
public:
	static void reset(const String &desc);
	static void resetAll();
	
	static void addInfo(TimerPerformance *timer);

	static void addPending(TimerPerformance *timer);
	static void removePending(TimerPerformance *timer);

	static String toString();
	static String toHtml();
	static void trace();
	
// Internal:
protected:
	static InfosMap m_infos;	
	static PendingsMap m_pendings;
	static boost::recursive_mutex m_cs;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#ifdef OS_DEBUG
	#define OS_TIMER_PERFORMANCE(var, desc)		OS_NAMESPACE_NAME::TimerPerformance var(desc)
#else
	#define OS_TIMER_PERFORMANCE(var, desc)		
#endif // OS_DEBUG

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_TIMERPERFORMANCE_H
