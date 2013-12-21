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

#ifndef _OS_ENGINE_ENGINE_H
#define _OS_ENGINE_ENGINE_H

#include "boostasio.h"
#include "datetime.h"
#include "dbdb.h"
#include "osiriscommon.h"
#include "pimpl_ptr.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

#define OS_ENGINE_DEFAULT_BACKGROUND_TASKS			1

//#define OS_CLOCK_SYNCHRONIZATION_METHOD_NTP			_S("ntp") // TOCLEAN
//#define OS_CLOCK_SYNCHRONIZATION_METHOD_ISIS		_S("isis") // TOCLEAN

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IBackgroundJob;
class IDbConnection;
class DateTime;
class File;
class HttpClient;
class HttpUrl;
class ISystem;
class OsirisLink;
class PortalID;
class TCPSocket;
class TimeDuration;
class UDPSocket;
class XMLDocument;

//////////////////////////////////////////////////////////////////////

class EngineExport Engine : public DynamicSingleton<Engine>
{
public:
	typedef deque<shared_ptr<IBackgroundJob> >::type BackgroundJobs;	

// Construction
public:
	Engine();
	virtual ~Engine();

// Attributes
public:
	std::string getMachineID();
	//bool getSynchronizedDateTime(DateTime &time); // TOCLEAN

	TimeDuration getAliveDelta() const;
	bool getAliveSince(const TimeDuration &duration) const;

	static uint32 getVersionMajor();
	static uint32 getVersionMinor();
	static std::string getVersionName(bool user); // if "user" is true, also instance name are appended.
	static std::string getPlatformName();
	static std::string getDefaultHttpUserAgent();
	
	String getText(const String &key) const;

// Operations
public:
	bool start(bool recovery);
	void stop();

	bool executeCommand(const String &command);
	uint32 processFile(const String &filename, const String &password);
	uint32 processFile(shared_ptr<File> file, const String &password);
	
	bool processLink(const OsirisLink &link, String &href);

	// Apre una connessione al database di sistema
	shared_ptr<IDbConnection> createSystemConnection() const;

	// Abilita/disabilita la serializzazione del log su disco
	void enableLogger(bool enable);
		
	void waitJobs();

	void addBackgroundJob(shared_ptr<IBackgroundJob> job);
	bool hasBackgroundJobs() const;
	shared_ptr<IBackgroundJob> getBackgroundJob(uint32 id) const;	
	shared_ptr<BackgroundJobs> getBackgroundJobs() const;	
	shared_ptr<Engine::BackgroundJobs> getLastCompletedBackgroundJobs() const;
	void clearCompletedBackgroundJobs();	
	uint32 peekBackgroundJobID(); 
	//shared_ptr<IBackgroundJob> getBackgroundPortalJob(PortalID portal) const;
		
	void startExtensionsJob(const String &jobType, const std::string &extensionID, const String &downloadUrl);	

	shared_ptr<TCPSocket> createTCPSocket(shared_ptr<boost::asio::io_service> io_service, bool outgoing, bool unsafe);
	shared_ptr<UDPSocket> createUDPSocket(shared_ptr<boost::asio::io_service> io_service, bool outgoing, bool unsafe);
	
	shared_ptr<HttpClient> createHttpClient();
	shared_ptr<XMLDocument> parseUrlXml(HttpUrl& url);
	int getTimeOffset();
	int getTimeDST();
	String formatSystemDate(const boost::posix_time::ptime &time, DateTime::DateTimePattern pattern = DateTime::dpShortDateTimePattern);

private:
	void onUpdateOptions();

	void updateOptions();

	//bool doSynchronizeClock(DateTime &dt); // TOCLEAN
	//bool doSynchronizeClockWithNTP(DateTime &dt, uint32 retries = 3); // TOCLEAN
	//bool doSynchronizeClockWithIsis(DateTime &dt, uint32 retries = 3); // TOCLEAN

	void initAdditionalLibraries();
	void cleanupAdditionalLibraries();
	
	void initLanguage();
	void initBackgroundJobs();	

private:
	bool _configure();
	bool _start(bool recovery);
	void _stop();

protected:
	pimpl_of<Engine>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_ENGINE_H
