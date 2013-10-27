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
#include "realtimestats.h"

#include "boost/thread/recursive_mutex.hpp"
#include "lock.h"
#include "platformmanager.h"
#include "regex.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

/*
RealtimeStatsScope::RealtimeStatsScope(const String &name)
{
	m_name = name;

	if(!RealtimeStatsManager::exists(name))
		RealtimeStatsManager::add(name, RealtimeStatsCounter::ctAbsolute, _S("state"));
	
	RealtimeStatsManager::signal(m_name, 1);
}

RealtimeStatsScope::~RealtimeStatsScope()
{
	RealtimeStatsManager::signal(m_name, 0);
}
*/

RealtimeStatsScopeTimer::RealtimeStatsScopeTimer(const String &group, const String &name)
{
	if(RealtimeStatsManager::getPause())
		return;
	
	//shared_ptr<RealtimeStatsScopeTimerCounter>
	m_counter = boost::dynamic_pointer_cast<RealtimeStatsScopeTimerCounter>(RealtimeStatsManager::get(name));
	if(m_counter == null)
	{
		m_counter.reset(OS_NEW RealtimeStatsScopeTimerCounter());
		m_counter->m_group = group;
		m_counter->m_name = name;	
		RealtimeStatsManager::add(m_counter);
	}
	
	start();	
}

RealtimeStatsScopeTimer::~RealtimeStatsScopeTimer()
{
	if(m_counter != null)
	{
		m_counter->stop();
		m_counter = null;
	}
}

void RealtimeStatsScopeTimer::start()
{
	if(m_counter != null)
		m_counter->start();
}

void RealtimeStatsScopeTimer::stop()
{
	if(m_counter != null)
		m_counter->stop();
}

RealtimeStatsScopeTimerCounter::RealtimeStatsScopeTimerCounter() : RealtimeStatsCounter()
{
	m_type = RealtimeStatsCounter::ctRelative;
	m_unit = _S("%");	

	m_tickStart = 0;
}

RealtimeStatsScopeTimerCounter::~RealtimeStatsScopeTimerCounter()
{	
}

void RealtimeStatsScopeTimerCounter::start()
{
	if(m_tickStart == 0)
		m_tickStart = PlatformManager::instance()->tickCount();
}

void RealtimeStatsScopeTimerCounter::stop()
{
	signal();
	m_tickStart = 0;	
}

void RealtimeStatsScopeTimerCounter::signal()
{
	if(m_tickStart != 0)
	{
		double tickNow = PlatformManager::instance()->tickCount();
		double tickDelta = tickNow - m_tickStart;
		//double val = (tickDelta / RealtimeStatsManager::m_resolution)*100;
		double val = (tickDelta*100) / RealtimeStatsManager::m_resolution;
		//val = utils::round(val);
		
		RealtimeStatsManager::signal(m_name, val);
		m_tickStart = tickNow;
	}
}

void RealtimeStatsScopeTimerCounter::onStep()
{
	signal();
}

//////////////////////////////////////////////////////////////////////

RealtimeStatsCounter::RealtimeStatsCounter()
{
	static uint32 id=0;
	id++;
	m_id = id;
	clear();
}

RealtimeStatsCounter::~RealtimeStatsCounter()
{

}

bool RealtimeStatsCounter::isVisible()
{
	return ( (m_active) && (!m_values.empty()) );
}

void RealtimeStatsCounter::getValueBorder(double& minValue, double& maxValue, double& lastValue)
{
	uint32 index = 0;
	for(Values::const_iterator i = m_values.begin(); i != m_values.end(); ++i)
	{
		double value = *i;
		if(index == 0)
		{
			minValue = value;
			maxValue = value;
		}
		else
		{
			minValue = std::min(minValue, value);
			maxValue = std::max(maxValue, value);
		}		
		
		lastValue = value;

		index++;
	}
}


void RealtimeStatsCounter::clear()
{
	m_name.clear();
	m_type = ctAbsolute;
	m_temporary = true;
	m_unit.clear();
	m_scale = 0; // TODO: Se scale==0, scala automatica; altrimenti fare un combo per la scelta scala da 0.000001 a 100000.0
	m_active = true; // URGENT

	m_values.clear();
	m_last = 0;
	m_min = 0;
	m_max = 0;
	m_zeroSteps = 0;
	m_currentValue = 0;
}

void RealtimeStatsCounter::onStep()
{
}

//////////////////////////////////////////////////////////////////////

RealtimeStatsPreset::RealtimeStatsPreset(const String& name, double scaleMin, double scaleMax)
{
	m_name = name;
	m_scaleMin = scaleMin;
	m_scaleMax = scaleMax;
}

RealtimeStatsPreset::~RealtimeStatsPreset()
{

}

//////////////////////////////////////////////////////////////////////

bool RealtimeStatsManager::m_pause = true;

double RealtimeStatsManager::m_resolution = 1000;
uint32 RealtimeStatsManager::m_maxItems = 50;
double RealtimeStatsManager::m_minScaleY;
double RealtimeStatsManager::m_maxScaleY;

double RealtimeStatsManager::m_tickLastProcess;

RealtimeStatsManager::CountersMap RealtimeStatsManager::m_counters;
RealtimeStatsManager::PresetsVector RealtimeStatsManager::m_presets;
shared_ptr<RealtimeStatsPreset> RealtimeStatsManager::m_currentPreset;

boost::recursive_mutex RealtimeStatsManager::m_cs;

boost::recursive_mutex & RealtimeStatsManager::getLock()
{
	return m_cs;
}

void RealtimeStatsManager::reset()
{
	OS_LOCK(getLock());

	for(CountersMap::iterator i = m_counters.begin(); i != m_counters.end(); ++i)
	{
		shared_ptr<RealtimeStatsCounter> counter = i->second;
		counter->m_values.clear();
	}

	m_tickLastProcess = PlatformManager::instance()->tickCount();	
}

void RealtimeStatsManager::update()
{
	OS_LOCK(getLock());

	if(m_pause)
		return;

	// Notifica ai contatori che derivano onStep di notificare il valore
	for(CountersMap::iterator i = m_counters.begin(); i != m_counters.end(); ++i)
	{
		shared_ptr<RealtimeStatsCounter> counter = i->second;
		counter->onStep();
	}
}

void RealtimeStatsManager::align()
{
	OS_LOCK(getLock());

	if(m_pause)
		return;	

	double tickCurrent = PlatformManager::instance()->tickCount();

	// Ottimizzo il m_tickLastProcess: se è di molto tempo fa, è inutile inserire valori che poi verranno rimossi.
	double tickMin = tickCurrent - m_resolution * static_cast<double>(m_maxItems);
	if(m_tickLastProcess < tickMin)
		m_tickLastProcess = tickMin;

	for(;m_tickLastProcess<tickCurrent;)
	{
		m_tickLastProcess += m_resolution;
		for(CountersMap::iterator i = m_counters.begin(); i != m_counters.end();)
		{
			shared_ptr<RealtimeStatsCounter> counter = i->second;			
			
			if(counter->m_values.size() == 0)
			{
				counter->m_min = counter->m_currentValue;
				counter->m_max = counter->m_currentValue;				
			}
			else
			{
				counter->m_min = std::min(counter->m_min, counter->m_currentValue);
				counter->m_max = std::max(counter->m_max, counter->m_currentValue);
			}

			bool remove = false;
			if(counter->m_temporary)
			{
				if(counter->m_currentValue == 0)
				{
					counter->m_zeroSteps++;
					if(counter->m_zeroSteps == m_maxItems)
						remove = true;				
				}
				else
				{
					counter->m_zeroSteps = 0;				
				}
			}

			if(remove)
			{
				CountersMap::iterator ie = i;				
				++i;
				m_counters.erase(ie);
			}
			else
			{				
				counter->m_last = counter->m_currentValue;
				counter->m_values.push_back(counter->m_currentValue);
				// Se il tipo di contatore è "relative", sia azzera.
				if(counter->m_type == RealtimeStatsCounter::ctRelative)
					counter->m_currentValue = 0;

				utils::pop_front_until(counter->m_values, m_maxItems);
				++i;
			}
		}
	}		
}

void RealtimeStatsManager::signal(const String &name, const double &value)
{
	if(m_pause)
		return;

	OS_LOCK(getLock());

	CountersMap::const_iterator i = m_counters.find(name);
	if(i != m_counters.end())
	{
		shared_ptr<RealtimeStatsCounter> counter = i->second;

		// Se è assoluto, lo setto anche se non è attivo.
		// Altrimenti ad esempio non vedrei lo stato di stabilità finchè non arriva una "signal" dopo che il counter è stato attivato.
		
		if(counter->m_active)
			align();

		switch(counter->m_type)
		{
		case RealtimeStatsCounter::ctAbsolute:
			{
				counter->m_currentValue = value;
			} break;
		case RealtimeStatsCounter::ctRelative:
			{
				if(counter->m_active)
					counter->m_currentValue += value;
			} break;
		}

		if( (counter->m_active) && (counter->m_temporary) )
			counter->m_zeroSteps = 0;			
		
		/*
		if(counter->m_active)
		{
			align();

			switch(counter->m_type)
			{
			case RealtimeStatsCounter::ctAbsolute:
				{
					counter->m_currentValue = value;
				} break;
			case RealtimeStatsCounter::ctRelative:
				{
					counter->m_currentValue += value;
				} break;
			}

			if(counter->m_temporary)
				counter->m_zeroSteps = 0;			
		}
		*/
	}	
}

void RealtimeStatsManager::signal(const String &group, const String &name, const double &value, const RealtimeStatsCounter::CounterType &type, bool temporary, const String &unit)
{
	OS_LOCK(getLock());

	if(!exists(name))
		add(group, name, type, temporary, unit);
	signal(name,value);
}

bool RealtimeStatsManager::exists(const String &name)
{	
	OS_LOCK(getLock());

	return (m_counters.find(name) != m_counters.end());
}

shared_ptr<RealtimeStatsCounter> RealtimeStatsManager::get(const String &name)
{
	OS_LOCK(getLock());

	CountersMap::const_iterator i = m_counters.find(name);
	if(i != m_counters.end())
		return i->second;
	else
		return null;	
}

bool RealtimeStatsManager::isActive(const String &name)
{
	OS_LOCK(getLock());

	shared_ptr<RealtimeStatsCounter> counter = get(name);
	if(counter != null)
		return counter->m_active;
	
	return false;
}

void RealtimeStatsManager::add(shared_ptr<RealtimeStatsCounter> counter)
{
	OS_LOCK(getLock());

	// Se ci son già oltre 100 contatori, non lo aggiunge.
	// Non è il massimo, ma è per evitare situazioni di bug non gestite che provocano aggiunte incontrollate di contatori.
	if(m_counters.size()>200)
	{
		OS_ASSERTFALSE();
		return;
	}

	// Imposta se è attivo in base al preset corrente.
	counter->m_active = matchPreset(counter->m_group);

	// Se è temporaneo ma non è nel preset, non lo inserisce.
	if( (counter->m_temporary) && (counter->m_active == false) )
		return;
		
	m_counters[counter->m_name] = counter;
}

shared_ptr<RealtimeStatsCounter> RealtimeStatsManager::add(const String &group, const String &name, const RealtimeStatsCounter::CounterType &type, bool temporary, const String &unit)
{
	OS_LOCK(getLock());

	// Se è temporaneo ma non è nel preset, non lo crea.
	if(temporary)
	{
		if(matchPreset(group) == false)
			return null;
	}
	
	shared_ptr<RealtimeStatsCounter> counter = get(name);
	if(counter != null)
		return counter;
	else
	{
		counter.reset(OS_NEW RealtimeStatsCounter());
		counter->m_group = group;
		counter->m_name = name;		
		counter->m_type = type;
		counter->m_unit = unit;
		counter->m_temporary = temporary;
		add(counter);
		return counter;
	}	
}
/*
weak_ptr<RealtimeStatsScopeTimer> RealtimeStatsManager::addScopeTimer(const String &group, const String &name)
{
	if(m_pause)
		return null;

	return weak_ptr<OS_NEW RealtimeStatsScopeTimer>(new RealtimeStatsScopeTimer(group,name));
	
}
*/
void RealtimeStatsManager::addPreset(const String &name, double scaleMin, double scaleMax)
{
	shared_ptr<RealtimeStatsPreset> preset(OS_NEW RealtimeStatsPreset(name, scaleMin, scaleMax));

	m_presets.push_back(preset);
}

void RealtimeStatsManager::usePreset(const String& name)
{
	OS_LOCK(getLock());

	m_currentPreset = null;
	for(PresetsVector::iterator i = m_presets.begin(); i != m_presets.end(); i++)
	{		
		shared_ptr<RealtimeStatsPreset> preset2 = *i;
		if(preset2->getName() == name)
		{
			m_currentPreset = preset2;
			break;
		}
	}

	for(CountersMap::iterator i = m_counters.begin(); i != m_counters.end();)
	{
		shared_ptr<RealtimeStatsCounter> counter = i->second;			

		counter->m_active = matchPreset(counter->m_group);
		
		if( (counter->m_temporary) && (counter->m_active == false) )
		{
			CountersMap::iterator ie = i;				
			i++;
			m_counters.erase(ie);
		}
		else
		{		
			i++;
		}

	}
}

bool RealtimeStatsManager::matchPreset(const String& group)
{
	if(m_currentPreset == null)
		return false;
	if(m_currentPreset->getName() == _S("All"))
		return  true;
	return (m_currentPreset->getName() == group);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
