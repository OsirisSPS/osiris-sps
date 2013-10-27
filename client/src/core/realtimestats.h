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

#ifndef _OS_CORE_REALTIMESTATS_H
#define _OS_CORE_REALTIMESTATS_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

/*
class CoreExport RealtimeStatsScope
{
public:

	RealtimeStatsScope(const String &name);
	~RealtimeStatsScope();

private:
	String m_name;
};
*/

//////////////////////////////////////////////////////////////////////

class CoreExport RealtimeStatsCounter : public enable_this_ptr<RealtimeStatsCounter>,
										public Object
{
public:
	typedef list<double>::type Values;

	enum CounterType
	{
		ctRelative	 = 0,
		ctAbsolute	 = 1,
	};

public:
	RealtimeStatsCounter();
	virtual ~RealtimeStatsCounter();

// Attributes:
	inline uint32 getId();
	bool isVisible();
	void getValueBorder(double& minValue, double& maxValue, double& lastValue);	

// Operation:
	void clear();

// Events:
public:
	virtual void onStep();

public:
	uint32 m_id;
	String m_group;
	String m_name;
	CounterType m_type;
	bool m_temporary;
	String m_unit;
	double m_scale;
	bool m_active;
	
	Values m_values;
	double m_last;
	double m_min;
	double m_max;
	uint32 m_zeroSteps;

	double m_currentValue;
};

//////////////////////////////////////////////////////////////////////

class CoreExport RealtimeStatsPreset : public enable_this_ptr<RealtimeStatsPreset>,
									   public Object
{
public:
	RealtimeStatsPreset(const String& name, double scaleMin, double scaleMax);
	virtual ~RealtimeStatsPreset();

// Attributes:
	inline String getName() const;
	
// Operation:
	
// Events:
public:
	
public:
	String m_name;	
	double m_scaleMin;
	double m_scaleMax;
};

//////////////////////////////////////////////////////////////////////

class CoreExport RealtimeStatsScopeTimerCounter : public RealtimeStatsCounter
{
public:

	RealtimeStatsScopeTimerCounter();
	~RealtimeStatsScopeTimerCounter();

	void start();
	void stop();

private:
	void signal();

// Events:
public:
	virtual void onStep();

private:
	double m_tickStart;
};

class CoreExport RealtimeStatsScopeTimer
{
public:

	RealtimeStatsScopeTimer(const String &group, const String &name);
	~RealtimeStatsScopeTimer();

	void start();
	void stop();

private:
	shared_ptr<RealtimeStatsScopeTimerCounter> m_counter;
};

//////////////////////////////////////////////////////////////////////

class CoreExport RealtimeStatsManager
{
public:
	typedef map<String,shared_ptr<RealtimeStatsCounter> >::type CountersMap;
	typedef vector<shared_ptr<RealtimeStatsPreset> >::type PresetsVector;

	static boost::recursive_mutex & getLock();

	static inline void setPause(bool pause);
	static inline bool getPause();
	static inline PresetsVector& getPresets();
	
	static void reset();
	static void update();
	static void align();
	static void signal(const String &name, const double &value);
	static void signal(const String &group, const String &name, const double &value, const RealtimeStatsCounter::CounterType &type, bool temporary, const String &unit);	

	static bool exists(const String &name);
	static shared_ptr<RealtimeStatsCounter> get(const String &name);
	static bool isActive(const String &name);
	static void add(shared_ptr<RealtimeStatsCounter> counter);
	static shared_ptr<RealtimeStatsCounter> add(const String &group, const String &name, const RealtimeStatsCounter::CounterType &type, bool temporary, const String &unit);

	static void addPreset(const String &name, double scaleMin, double scaleMax);
	static void usePreset(const String& name);
	static bool matchPreset(const String& group);
	
public:
	static bool m_pause;

	static double m_resolution; // Tick count tra uno step e l'altro
	static uint32 m_maxItems;
	static double m_minScaleY;
	static double m_maxScaleY;

	static double m_tickLastProcess;
	
	static CountersMap m_counters;		
	static PresetsVector m_presets;	

	static shared_ptr<RealtimeStatsPreset> m_currentPreset;
	static boost::recursive_mutex m_cs;
};

//////////////////////////////////////////////////////////////////////

uint32 RealtimeStatsCounter::getId() { return m_id; }

String RealtimeStatsPreset::getName() const { return m_name; }

void RealtimeStatsManager::setPause(bool pause) { m_pause = pause; }
bool RealtimeStatsManager::getPause() { return m_pause; }
RealtimeStatsManager::PresetsVector& RealtimeStatsManager::getPresets() { return m_presets; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_REALTIMESTATS_H
