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

#ifndef _ISISSYSTEM_H
#define _ISISSYSTEM_H

#include "boost/thread/mutex.hpp"
#include "datetime.h"
#include "entitiesentities.h"
#include "extensionsextensions.h"
#include "isystem.h"
#include "ordered_map.h"
#include "portalsportals.h"
#include "singleton.h"
#include "taskthread.h"
#include "uniqueid.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport IsisSystem : public DynamicSingleton<IsisSystem>,
									  public ISystem
{
public:

	class EngineExport IsisSystemThread : public TaskThread
	{	
	// Construction
	public:
		IsisSystemThread();
		virtual ~IsisSystemThread();

	// Thread interface
	public:
		virtual bool executeTask();
	};


// Construction
public:
	IsisSystem();
	virtual ~IsisSystem();

// Attributes
public:
	inline int getStatus();
	inline bool getOnline();
	inline String getLastError();
	inline DateTime getLastRequest();

	inline String getLatestOsirisVersion();
	inline String getLatestOsirisNotes();
	inline std::string getIp();
	inline String getCountryCode();
	inline String getCountryName();
	inline bool getReachable();
	inline int32 getTor();
	inline DateTime getLastValidationDate();
	inline DateTime getLastCheckDate();

	inline std::string getInternetDateTimeSyncMethod();
	
// Operations
public:
	uint32 doTask(bool force);
		
	bool isInternetDateTimeAvailable();
	DateTime getInternetDateTime();
			
	String getUrl(ordered_map<std::wstring, std::wstring> &params) const;
	//String getPortalUrl(shared_ptr<Portal> portal) const; // TOCLEAN
	//void updatePortal(shared_ptr<Portal> portal);
	std::string resolveItemLink(const std::string &id);
	
private:
	uint32 requestIsis(bool force);
	void doSynchronizeClockWithNTP();
	DateTime getClockWithNTP();
	
// ISystem interface
public:
	virtual String getName() const;

	virtual bool start(bool recovery);
	virtual void stop();

protected:
	mutable boost::mutex m_cs;
	shared_ptr<IsisSystemThread> m_thread;

	int m_status;
	bool m_online;
	String m_lastError;
	DateTime m_lastRequest;

	String m_latestOsirisVersion;
	String m_latestOsirisNotes;
	std::string m_ip;
	String m_countryCode;
	String m_countryName;
	
	bool m_reachable;
	int32 m_tor;
	DateTime m_lastValidationDate;
	DateTime m_lastCheckDate;	

	mutable boost::mutex m_clockCS;
	DateTime m_synchronizedClock;
	uint32 m_lastClockSynchronization;
	std::string m_synchronizedClockMethod;

	// Isis Server-side load limits
	int m_intervalUpdate;
	int m_nMaxPortals;	
};

//////////////////////////////////////////////////////////////////////

inline int IsisSystem::getStatus()  { return m_status; }
inline bool IsisSystem::getOnline()  { return m_online; }
inline String IsisSystem::getLastError()  { return m_lastError; }
inline DateTime IsisSystem::getLastRequest()  { return m_lastRequest; }
inline String IsisSystem::getLatestOsirisVersion() { return m_latestOsirisVersion; }
inline String IsisSystem::getLatestOsirisNotes() { return m_latestOsirisNotes; }
inline std::string IsisSystem::getIp()  { return m_ip; }
inline String IsisSystem::getCountryCode() { return m_countryCode; }
inline String IsisSystem::getCountryName() { return m_countryName; }
inline bool IsisSystem::getReachable() { return m_reachable; }
inline int32 IsisSystem::getTor() { return m_tor; }
inline DateTime IsisSystem::getLastValidationDate() { return m_lastValidationDate; }
inline DateTime IsisSystem::getLastCheckDate() { return m_lastCheckDate; }
inline std::string IsisSystem::getInternetDateTimeSyncMethod() { return m_synchronizedClockMethod; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ISISSYSTEM_H
