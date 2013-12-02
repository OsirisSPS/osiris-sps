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
#include "engine.h"

#include "algorithms.h"
#include "bandwidthmanager.h"
#include "buffer.h"
#include "boost/bind.hpp"
#include "conversions.h"
#include "convert.h"
#include "dbdatabasessystem.h"
#include "idbconnection.h"
#include "idbdriver.h"
#include "dbschemaschema.h"
#include "cryptmanager.h"
#include "extensionssystem.h"
#include "file.h"
#include "filelogger.h"
#include "filesystem.h"
#include "httpclient.h"
#include "httprequest.h"
#include "httpsocketproxy.h"
#include "httpurl.h"
#include "idesystem.h"
#include "ibackgroundjob.h"
#include "isis.h"
#include "isissystem.h"
#include "jobspool.h"
#include "jobcallback.h"
#include "languagefolder.h"
#include "languagemanager.h"
#include "iloghandler.h"
#include "languageculture.h"
#include "lock.h"
#include "logmanager.h"
#include "logmessage.h"
#include "mainaddonsjob.h"
#include "networksystem.h"
#include "notificationsmanager.h"
#include "ntpclient.h"
#include "objectssystem.h"
#include "omlmanager.h"
#include "options.h"
#include "osiriscommon.h"
#include "osirislink.h"
#include "ntp.h"
#include "p2plocalmachine.h"
#include "p2pp2psystem.h"
#include "platformmanager.h"
#include "pluginsmanager.h"
#include "portalsportal.h"
#include "portalsimporterjob.h"
#include "portalsportalssystem.h"
#include "portalsserializer.h"
#include "schedulablethread.h"
#include "socks5socketproxy.h"
#include "tcpsocket.h"
#include "textfile.h"
#include "thirdpartylibrariesreporter.h"
#include "thirdpartylibrary.h"
#include "udpsocket.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

#define OS_ENGINE_MAX_COMPLETED_BACKGROUND_JOBS		25

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SINGLETON(OS_NAMESPACE_NAME::Engine)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<Engine>
{
public:
	typedef list<shared_ptr<ISystem> >::type Systems;

// Construction
public:
	pimpl();
	virtual ~pimpl();

// Operations
public:
	template <typename T>
	void configureSystem();

	void addBackgroundJob(shared_ptr<IBackgroundJob> job);
	bool hasBackgroundJobs() const;
	shared_ptr<IBackgroundJob> getBackgroundJob(uint32 id) const;
	shared_ptr<Engine::BackgroundJobs> getBackgroundJobs() const;
	shared_ptr<Engine::BackgroundJobs> getLastCompletedBackgroundJobs() const;
	void clearCompletedBackgroundJobs();
	uint32 peekBackgroundJobID();
	//shared_ptr<IBackgroundJob>  getBackgroundPortalJob(PortalID portal) const;
	void resetJobs();

	//bool processLinkPortal(shared_ptr<OsirisLink> link, String &href);
	//bool processLinkIsis(shared_ptr<OsirisLink> link, String &href);
	bool processLinkPortal(const OsirisLink &link, String &href);
	bool processLinkIsis(const OsirisLink &link, String &href);

	void updateOptions();

private:
	shared_ptr<JobsPool> ensureJobsPool();
	void registerLibraries();
	void handleBackgroundJobCompleted(shared_ptr<IJob> job);

public:
	Systems systems;
	shared_ptr<FileLogger> logger;
	DateTime synchronizedClock;
	DateTime creationTimestamp;
	uint32 lastClockSynchronization;
	boost::recursive_mutex clockCS;

private:
	mutable boost::mutex m_backgroundJobsCS;
	shared_ptr<JobsPool> m_backgroundJobsPool;
	uint32 m_backgroundJobsID;
	Engine::BackgroundJobs m_lastCompletedBackgroundJobs;
};

//////////////////////////////////////////////////////////////////////

pimpl<Engine>::pimpl() : lastClockSynchronization(0),
						 creationTimestamp(DateTime::now()),
						 m_backgroundJobsID(0)
{
	registerLibraries();	
}

pimpl<Engine>::~pimpl()
{
	resetJobs();
}

template <typename T>
void pimpl<Engine>::configureSystem()
{
	shared_ptr<ISystem> system(T::create());
	OS_EXCEPT_IF(system == null, "Invalid system");

	OS_ASSERT(utils::exists(systems, system) == false);		// Essendo tutti i sistemi dei singleton è possibile effettuare il controllo
	systems.push_back(system);
}

void pimpl<Engine>::addBackgroundJob(shared_ptr<IBackgroundJob> job)
{
	boost::mutex::scoped_lock lock(m_backgroundJobsCS);

	ensureJobsPool()->addJob(job);
}

bool pimpl<Engine>::hasBackgroundJobs() const
{
	boost::mutex::scoped_lock lock(m_backgroundJobsCS);

	if(m_backgroundJobsPool == null)
		return false;

	return m_backgroundJobsPool->hasJobs();
}

shared_ptr<IBackgroundJob> pimpl<Engine>::getBackgroundJob(uint32 id) const
{
	boost::mutex::scoped_lock lock(m_backgroundJobsCS);

	for(Engine::BackgroundJobs::const_iterator i = m_lastCompletedBackgroundJobs.begin(); i != m_lastCompletedBackgroundJobs.end(); ++i)
	{
		if((*i)->getID() == id)
			return *i;
	}

	if(m_backgroundJobsPool != null)
		return boost::dynamic_pointer_cast<IBackgroundJob>(m_backgroundJobsPool->getJob(id));

	return null;
}

shared_ptr<Engine::BackgroundJobs> pimpl<Engine>::getBackgroundJobs() const
{
	shared_ptr<Engine::BackgroundJobs> jobs(OS_NEW_T(Engine::BackgroundJobs), os_delete_t());

	boost::mutex::scoped_lock lock(m_backgroundJobsCS);

	if(m_backgroundJobsPool != null)
	{
		JobsPool::Jobs poolJobs;
		m_backgroundJobsPool->getJobs(poolJobs);
		for(JobsPool::Jobs::const_iterator i = poolJobs.begin(); i != poolJobs.end(); ++i)
		{
			shared_ptr<IBackgroundJob> job = boost::dynamic_pointer_cast<IBackgroundJob>(*i);
			OS_ASSERT(job != null);
			if(job != null)
				jobs->push_back(job);
		}
	}

	return jobs;
}

shared_ptr<Engine::BackgroundJobs> pimpl<Engine>::getLastCompletedBackgroundJobs() const
{
	shared_ptr<Engine::BackgroundJobs> jobs(OS_NEW_T(Engine::BackgroundJobs), os_delete_t());

	boost::mutex::scoped_lock lock(m_backgroundJobsCS);

	utils::copy(*jobs, m_lastCompletedBackgroundJobs);

	return jobs;
}

void pimpl<Engine>::clearCompletedBackgroundJobs()
{
	boost::mutex::scoped_lock lock(m_backgroundJobsCS);

	m_lastCompletedBackgroundJobs.clear();
}

uint32 pimpl<Engine>::peekBackgroundJobID()
{
	boost::mutex::scoped_lock lock(m_backgroundJobsCS);

	return m_backgroundJobsID++;
}

/*
shared_ptr<IBackgroundJob> pimpl<Engine>::getBackgroundPortalJob(PortalID portal) const
{
	//RAFFO: ????

	for(Engine::BackgroundJobs::const_iterator i = m_lastCompletedBackgroundJobs.begin(); i != m_lastCompletedBackgroundJobs.end(); ++i)
	{
		if((*i)->getID() == id)
			return *i;
	}
	return null;
}
*/

void Engine::startExtensionsJob(const String &jobType, const std::string &extensionID, const String &downloadUrl)
{
	// Note 29/08/2011: Job creation was in "addons.py". But, if i created it from python, during execution of the job it maintain a PythonState lock, that cause locks problems.
	addBackgroundJob(shared_ptr<MainAddonsJob>(OS_NEW MainAddonsJob(peekBackgroundJobID(), jobType, extensionID, downloadUrl)));	
}

void pimpl<Engine>::resetJobs()
{
	shared_ptr<JobsPool> pool;

	{
		boost::mutex::scoped_lock lock(m_backgroundJobsCS);

		m_lastCompletedBackgroundJobs.clear();
		pool = m_backgroundJobsPool;
		m_backgroundJobsPool.reset();
	}

	if(pool != null)
		pool->stop();
}

//bool pimpl<Engine>::processLinkPortal(shared_ptr<OsirisLink> link, String &href)
bool pimpl<Engine>::processLinkPortal(const OsirisLink &link, String &href)
{
	OS_ASSERT(href.empty());

	/*
	if(link == null)
	{
		OS_ASSERTFALSE();
		return false;
	}
	*/

	OS_ASSERT(link.getType() == OsirisLink::linkPortal);

	const PortalID &id = link.getPortal();
	const PovID &pov = link.getPov();
	if(PortalsSystem::instance()->hasPortal(id,pov))
	{
		href = IdeSystem::instance()->getLocalUrl(PortalsSystem::instance()->getPortalLink(PortalsSystem::instance()->getPortal(id,pov), "view"));
	}
	else
	{
		ordered_map<std::wstring, std::wstring> params;
		//params.set(OS_URL_PARAM_PORTAL, id.toWide()); // CLODOURGENT testare, serviva?
		params.append(link.getParams2());

		href = IdeSystem::instance()->getLocalUrl(PortalsSystem::instance()->getMainLink("subscribe", params));
	}

	return href.empty() == false;
}

void pimpl<Engine>::updateOptions()
{
	boost::mutex::scoped_lock lock(m_backgroundJobsCS);

	if(m_backgroundJobsPool != null)
		m_backgroundJobsPool->resize(Options::instance()->getOption<uint32>(Options::system_options::background_tasks));
}

//bool pimpl<Engine>::processLinkIsis(shared_ptr<OsirisLink> link, String &href)
bool pimpl<Engine>::processLinkIsis(const OsirisLink &link, String &href)
{
	OS_ASSERT(href.empty());

	/*
	if(link == null)
	{
		OS_ASSERTFALSE();
		return false;
	}
	*/

	OS_ASSERT(link.getType() == OsirisLink::linkIsis);
	
	const PortalID &portalID = link.getPortal();
	PovID povID = utils::wtoa(link.getParam("pov"));
	
	shared_ptr<Portal> portal = PortalsSystem::instance()->getPortal(portalID,povID);
	
	if(portal != null)	
		href = IdeSystem::instance()->getLocalUrl(PortalsSystem::instance()->getSettingsLink(portal, link.getParams2()));
	else
		href = IdeSystem::instance()->getLocalUrl(PortalsSystem::instance()->getErrorLink(Engine::instance()->getText(_S("common.errors.invalid_portal"))));

	return href.empty() == false;
}

shared_ptr<JobsPool> pimpl<Engine>::ensureJobsPool()
{
	// N.B.: la funzione va loccata a monte

	if(m_backgroundJobsPool == null)
	{
		m_backgroundJobsPool.reset(OS_NEW JobsPool(Options::instance()->getOption<uint32>(Options::system_options::background_tasks), taskPriorityIdle));
		m_backgroundJobsPool->getJobCompletedEvent()->connect(boost::bind(&pimpl<Engine>::handleBackgroundJobCompleted, this, _1));
	}

	return m_backgroundJobsPool;
}

void pimpl<Engine>::registerLibraries()
{
	{
		shared_ptr<ThirdPartyLibrary> cluceneLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
		cluceneLibrary->setName("CLucene");
		cluceneLibrary->setDescription("CLucene is a high-performance, scalable, cross platform, full-featured, open-source indexing and searching API");
		cluceneLibrary->setUrl("http://clucene.wiki.sourceforge.net/");
		cluceneLibrary->setLicense("Apache License");
		cluceneLibrary->setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0");
	}

	{
		shared_ptr<ThirdPartyLibrary> miniupnpLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
		miniupnpLibrary->setName("MiniUPnP");
		miniupnpLibrary->setDescription("The MiniUPnP project offers software which supports the UPnP Internet Gateway Device (IGD) specifications");
		miniupnpLibrary->setUrl("http://miniupnp.free.fr/");
		miniupnpLibrary->setLicense("BSD License");
		miniupnpLibrary->setLicenseUrl("http://en.wikipedia.org/wiki/BSD_license");
	}

	/*
	{
		shared_ptr<ThirdPartyLibrary> prototypeLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
		prototypeLibrary->setName("Prototype");
		prototypeLibrary->setVersion("1.6.0.3");
		prototypeLibrary->setDescription("Prototype is a JavaScript Framework that aims to ease development of dynamic web applications");
		prototypeLibrary->setUrl("http://www.prototypejs.org/");
		prototypeLibrary->setLicense("MIT License");
		prototypeLibrary->setLicenseUrl("http://en.wikipedia.org/wiki/MIT_License");
	}

	{
		shared_ptr<ThirdPartyLibrary> scriptaculousLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
		scriptaculousLibrary->setName("script.aculo.us");
		scriptaculousLibrary->setVersion("1.8.2");
		scriptaculousLibrary->setDescription("script.aculo.us provides you with easy-to-use, cross-browser user interface JavaScript libraries to make your web sites and web applications fly.");
		scriptaculousLibrary->setUrl("http://script.aculo.us");
		scriptaculousLibrary->setLicense("MIT License");
		scriptaculousLibrary->setLicenseUrl("http://en.wikipedia.org/wiki/MIT_License");
	}
	*/

	{
		shared_ptr<ThirdPartyLibrary> scriptaculousLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
		scriptaculousLibrary->setName("jQuery");
		scriptaculousLibrary->setVersion("1.4.2");
		scriptaculousLibrary->setDescription("jQuery is a fast and concise JavaScript Library that simplifies HTML document traversing, event handling, animating, and Ajax interactions for rapid web development.");
		scriptaculousLibrary->setUrl("http://jquery.com/");
		scriptaculousLibrary->setLicense("MIT/GPL License");
		scriptaculousLibrary->setLicenseUrl("http://jquery.org/license");
	}

	{
		shared_ptr<ThirdPartyLibrary> syntaxHighlighterLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
		syntaxHighlighterLibrary->setName("SyntaxHighlighter");
		syntaxHighlighterLibrary->setVersion("3.0.83");
		syntaxHighlighterLibrary->setDescription("JavaScript code syntax highlighter.");
		syntaxHighlighterLibrary->setUrl("http://alexgorbatchev.com/SyntaxHighlighter");
		syntaxHighlighterLibrary->setLicense("MIT License");
		syntaxHighlighterLibrary->setLicenseUrl("http://en.wikipedia.org/wiki/MIT_License");
	}
}

void pimpl<Engine>::handleBackgroundJobCompleted(shared_ptr<IJob> job)
{
	OS_ASSERT(job != null && job->getStatus() == IJob::jobComplete);

	shared_ptr<IBackgroundJob> backgroundJob = boost::dynamic_pointer_cast<IBackgroundJob>(job);
	if(backgroundJob == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	boost::mutex::scoped_lock lock(m_backgroundJobsCS);

	m_lastCompletedBackgroundJobs.push_back(backgroundJob);
	utils::pop_front_until(m_lastCompletedBackgroundJobs, OS_ENGINE_MAX_COMPLETED_BACKGROUND_JOBS);
}

//////////////////////////////////////////////////////////////////////

Engine::Engine()
{
	Options::instance()->subscribeUpdateHandler(boost::bind(&Engine::onUpdateOptions, this));
}

Engine::~Engine()
{
	cleanupAdditionalLibraries();
}

std::string Engine::getMachineID()
{
	return p2p::LocalMachine::instance()->getID().toHex();
}

/* // TOCLEAN
bool Engine::getSynchronizedDateTime(DateTime &dt)
{
	// Assicura che sia stata effettuata una sincronizzazione ntp
	if(synchronizeClock(false) == false)
		return false;

	OS_LOCK(m_impl->clockCS);

#ifdef OS_ENABLE_NTP_DIFFERENCES // TOCLEAN?
	DateTime test;
	uint32 delta = static_cast<uint32>(PlatformManager::instance()->tickCount());
	doSynchronizeClock(test);
	delta = static_cast<uint32>(PlatformManager::instance()->tickCount()) - delta;
#endif // OS_ENABLE_NTP_DIFFERENCES

	// Calcola l'offset trascorso dall'ultima sincronizzazione
	TimeDuration offset = TimeDuration::millisec(static_cast<uint32>(PlatformManager::instance()->tickCount()) - m_impl->lastClockSynchronization);
	// Calcola l'ora ntp corrente
	dt = m_impl->synchronizedClock + offset;

#ifdef OS_ENABLE_NTP_DIFFERENCES
	// Verifica che la richiesta ntp sia andata a buon fine
	if(test.isNull() == false)
	{
		DateTime maxTime = std::max(test, dt);
		DateTime minTime = std::min(test, dt);
		// Calcola l'errore
		TimeDuration difference = maxTime - minTime;
		uint32 delta_seconds = TimeDuration::millisec(delta).getTotalSeconds();
		//OS_ASSERT((difference.getTotalSeconds() - delta_seconds) <= 1);	// Se lo scarto è oltre il secondo c'è qualcosa che non va...
		OS_LOG_DEBUG(String::format(_S("difference from ntp %d second(s)").c_str(), difference.getTotalSeconds()));
	}
#endif // OS_ENABLE_NTP_DIFFERENCES

	return true;
}
*/

TimeDuration Engine::getAliveDelta() const
{
	return DateTime::now() - m_impl->creationTimestamp;
}

bool Engine::getAliveSince(const TimeDuration &duration) const
{
	return getAliveDelta() >= duration;
}

uint32 Engine::getVersionMajor()
{
	return OS_OSIRIS_VERSION_MAJOR;
}

uint32 Engine::getVersionMinor()
{
	return OS_OSIRIS_VERSION_MINOR;
}

std::string Engine::getVersionName(bool user)
{
	String versionName;

	if(user)
	{
		String instanceName = Options::instance()->getOption<String>(Options::system_options::instance_name);
		if(instanceName != String::EMPTY)
		{
			versionName.append(instanceName);
			versionName.append(_S(" - "));
		}
	}

	versionName.append(OS_OSIRIS_APPLICATION_NAME);
	versionName.append(String::format(_S(" %d.%d").c_str(), OS_OSIRIS_VERSION_MAJOR, OS_OSIRIS_VERSION_MINOR));

	//String releaseName = _S("RC1"); // Ex OS_OSIRIS_RELEASE_NAME. Use here custom beta-tester RC1, RC2 etc. Change here to avoid OS_OSIRIS_RELEASE_NAME recompiling.
	String releaseName = String::EMPTY; // Final (not RC) release

	if(releaseName != String::EMPTY)
	{
		versionName.append(_S(" "));
		versionName.append(releaseName);
	}

	return versionName.to_ascii();
}

std::string Engine::getPlatformName()
{
	std::string out;

#if OS_PLATFORM == OS_PLATFORM_WIN
	out += "Windows";
#elif OS_PLATFORM == OS_PLATFORM_LINUX
	out += "Linux";
#elif OS_PLATFORM == OS_PLATFORM_MAC
	out += "Mac OS-X";
#else
	out += "Unknown";
#endif

	out += " - ";

#if OS_ARCHITECTURE == OS_ARCHITECTURE_I386
	out += "32 bit (i386)";
#elif OS_ARCHITECTURE == OS_ARCHITECTURE_X86_64
	out += "64 bit (x86_64)";
#else
	out += "Unknown";
#endif

	return out;
}

std::string Engine::getDefaultHttpUserAgent()
{
	return String::format(_S("%S/%d.%d").c_str(), OS_OSIRIS_APPLICATION_NAME.c_str(), OS_OSIRIS_VERSION_MAJOR, OS_OSIRIS_VERSION_MINOR).to_ascii();
}

String Engine::getText(const String &key) const
{
	//return LanguageManager::instance()->getRootFolder()->getText(Options::instance()->getDefaultLanguage(), key);
	return LanguageManager::instance()->getText(Options::instance()->getDefaultLanguage(), key);
}

bool Engine::_configure()
{
	initAdditionalLibraries();

	// Avvia i sistemi

	m_impl->configureSystem<PluginsManager>();	
	m_impl->configureSystem<DatabasesSystem>();
	m_impl->configureSystem<IdeSystem>();
	m_impl->configureSystem<ObjectsSystem>();
	m_impl->configureSystem<ExtensionsSystem>();
	m_impl->configureSystem<PortalsSystem>();
	m_impl->configureSystem<p2p::P2PSystem>();
	m_impl->configureSystem<IsisSystem>();

	return true;
}

bool Engine::_start(bool recovery)
{
	for(pimpl<Engine>::Systems::iterator i = m_impl->systems.begin(); i != m_impl->systems.end(); ++i)
	{
		shared_ptr<ISystem> system = *i;
		OS_ASSERT(system != null);

		NotificationsManager::instance()->notify(_S("Starting system: ") + system->getName());
		if(system->start(recovery) == false)
			return false;
	}

	return true;
}

void Engine::_stop()
{
	m_impl->resetJobs();

	for(pimpl<Engine>::Systems::reverse_iterator i = m_impl->systems.rbegin(); i != m_impl->systems.rend(); ++i)
	{
		shared_ptr<ISystem> system = *i;

		NotificationsManager::instance()->notify(_S("Stopping system: ") + system->getName());
		system->stop();
	}
}

bool Engine::start(bool recovery)
{
	initLanguage();

	// In caso di modalità di recovery ripristina le opzioni al valore di default
	// 1.0, removed.
	/*
	if(recovery)
	{
		Options::instance()->toDefault();
		if(Options::instance()->save() == false)
		{
			OS_LOG_ERROR(_S("Error saving options"));
			return false;
		}
	}
	*/

	// Crea i sistemi
	if(_configure() == false)
		return false;

	// Inizializza le opzioni
	updateOptions();

	// Avvia i sistemi
	if(_start(recovery) == false)
		return false;

	NotificationsManager::instance()->notify(_S("Initializing background jobs"));
	initBackgroundJobs();

	//waitJobs(); // CLODOURGENT

	LanguageManager::instance()->setDumpKey(OS_NAMESPACE_NAME::Options::instance()->getOption<bool>(_S("developer.show_language_keys")));

	NotificationsManager::instance()->notify(getVersionName(true) + _S(" is ready"));

	return true;
}

void Engine::stop()
{
	// Ferma i sistemi
	_stop();

	// Rilascia i sistemi a partire dall'ultimo allocato
	utils::flush_back(m_impl->systems);
}

bool Engine::executeCommand(const String &command)
{
	return false;
}

uint32 Engine::processFile(const String &filename, const String &password)
{
	shared_ptr<File> file(OS_NEW File());
	if(file->open(filename, File::ofRead) == false)
		return 0;
	
	return processFile(file, password);
}

uint32 Engine::processFile(shared_ptr<File> file, const String &password)
{
	shared_ptr<PortalsSerializer> serializer(OS_NEW PortalsSerializer());

	shared_ptr<PortalsSerializer::IResult> result = serializer->parsePortalLink(file);
	if(result == null)
		return 0;
	
	shared_ptr<OsirisLink> portalLink = result->getPortalLink();
	if(portalLink == null)
		return 0;
	
	shared_ptr<Portal> portal = PortalsSystem::instance()->ensurePortal(portalLink, password);
	if(portal == null)
		return 0;
	
	uint32 jobID = Engine::instance()->peekBackgroundJobID();

	shared_ptr<PortalsImporterJob> job(OS_NEW PortalsImporterJob(jobID, portal, file));

	Engine::instance()->addBackgroundJob(job);

	return jobID;
}

bool Engine::processLink(const OsirisLink &link, String &href)
{
	OS_ASSERT(link.empty() == false);

	switch(link.getType())
	{
	case OsirisLink::linkPortal:	//return m_impl->processLinkPortal(boost::dynamic_pointer_cast<ShellLinkPortal>(link.getLink()), href);
									return m_impl->processLinkPortal(link, href);

	case OsirisLink::linkIsis:		//return m_impl->processLinkIsis(boost::dynamic_pointer_cast<ShellLinkIsis>(link.getLink()), href);
									return m_impl->processLinkIsis(link, href);

	default:						OS_ASSERTFALSE();
									break;
	}	

	return false;
}

shared_ptr<IDbConnection> Engine::createSystemConnection() const
{
	shared_ptr<IDbDriver> driver = DatabasesSystem::instance()->getDefaultDriver();
	OS_EXCEPT_IF(driver == null, "Need a default driver");

	shared_ptr<IDbConnection> connection = driver->createConnection(OS_SYSTEM_DB_NAME.to_ascii(), driver->createOptions());
	connection->open();

	return connection;
}

void Engine::enableLogger(bool enable)
{
	// Controlla se lo stato coincide con quello attuale
	if(enable == (m_impl->logger != null))
		return;

	if(enable)
	{
		// Inizializza la directory dei logs
		String logPath = Options::instance()->getLogPath();
		if(FileSystem::instance()->ensureDirectory(logPath) == false)
		{
			OS_LOG_ERROR(_S("Cannot ensure log directory \"") + logPath + _S("\""));
			return;
		}

		shared_ptr<FileLogger> logger = FileLogger::create();
		if(logger != null)
		{
			String filename = utils::makeFilePath(logPath, OS_LOG_FILENAME);
			if(logger->open(filename))
				m_impl->logger = logger;
			else
				OS_LOG_ERROR("Cannot open log file '" + filename + "' for write");
		}
	}
	else
	{
		m_impl->logger.reset();
	}
}

/*
bool Engine::synchronizeClock(bool force)
{
	OS_LOCK(m_impl->clockCS);

	// Controlla se è già stata effettuata una sincronizzazione in precedenza
	if(m_impl->synchronizedClock.isNull() == false)
	{
		// Verifica che non sia stato richiesto di effettuare una sincronizzazione forzata
		if(force == false)
			return true;
	}

	if(doSynchronizeClock(m_impl->synchronizedClock) == false)
		return false;

	m_impl->lastClockSynchronization = static_cast<uint32>(PlatformManager::instance()->tickCount());
	return true;
}
*/

void Engine::waitJobs()
{
	NotificationsManager::instance()->notify(_S("Waiting for background jobs"));
	for(;;)
	{			
		shared_ptr<Engine::BackgroundJobs> jobs = Engine::instance()->getBackgroundJobs();
		if(jobs->size() == 0)
			break;

		for(Engine::BackgroundJobs::const_iterator i = jobs->begin(); i != jobs->end(); ++i)
		{
			shared_ptr<IBackgroundJob> job = *i;
			shared_ptr<IBackgroundJob::Details> jobDetails = job->getDetails();
			bool v = job->getVisible();
			String name = jobDetails->getName();

			bool h = v;
		}

		if(hasBackgroundJobs() == false)
			break;

		PlatformManager::instance()->sleep(500);
	}
}


void Engine::addBackgroundJob(shared_ptr<IBackgroundJob> job)
{
	m_impl->addBackgroundJob(job);
}

bool Engine::hasBackgroundJobs() const
{
	return m_impl->hasBackgroundJobs();
}

shared_ptr<IBackgroundJob> Engine::getBackgroundJob(uint32 id) const
{
	return m_impl->getBackgroundJob(id);
}

shared_ptr<Engine::BackgroundJobs> Engine::getBackgroundJobs() const
{
	return m_impl->getBackgroundJobs();
}

shared_ptr<Engine::BackgroundJobs> Engine::getLastCompletedBackgroundJobs() const
{
	return m_impl->getLastCompletedBackgroundJobs();
}

void Engine::clearCompletedBackgroundJobs()
{
	m_impl->clearCompletedBackgroundJobs();
}

uint32 Engine::peekBackgroundJobID()
{
	return m_impl->peekBackgroundJobID();
}

/*
shared_ptr<IBackgroundJob> Engine::getBackgroundPortalJob(PortalID portal) const
{
	return m_impl->getBackgroundPortalJob(portal);
}
*/

shared_ptr<TCPSocket> Engine::createTCPSocket(shared_ptr<boost::asio::io_service> io_service, bool outgoing, bool unsafe)
{
	OS_ASSERT((unsafe == false) || outgoing);		// Le connessioni che non vanno all'esterno non possono essere insicure

	if(io_service == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	shared_ptr<TCPSocket> socket(OS_NEW TCPSocket(*io_service));

	if(outgoing)
	{
		// Locca le opzioni per assicurare che la lettura di ogni parametro di configurazione del proxy sia allineato
		shared_ptr<boost::recursive_mutex::scoped_lock> optionsLock = Options::instance()->lock();

		// TODO: qui non dovrebbe leggere le opzioni ma dovrebbe inizializzare il socket in base ad una configurazione
		// salvata (m_proxyType, m_proxyHost, m_proxyPort ecc...). Quando scatta l'evento di modifica delle opzioni, se cambia la configurazione del proxy,
		// andrebbero riavviate le connessioni in uscita...

		switch(Options::instance()->getOption<uint32>(Options::net_options::proxy_type))
		{
		case OS_PROXY_TYPE_NONE:
										break;

		case OS_PROXY_TYPE_HTTP:		{
											shared_ptr<HttpSocketProxy<boost::asio::ip::tcp> > proxy = socket->addLayer<HttpSocketProxy<boost::asio::ip::tcp> >();
											proxy->setProxyHost(Options::instance()->getOption<String>(Options::net_options::proxy_host).to_ascii());
											proxy->setProxyPort(Options::instance()->getOption<uint32>(Options::net_options::proxy_port));
											proxy->setProxyUsername(Options::instance()->getOption<String>(Options::net_options::proxy_username).to_ascii());
											proxy->setProxyPassword(Options::instance()->getOption<String>(Options::net_options::proxy_password).to_ascii());
										}

										break;

		case OS_PROXY_TYPE_SOCKS5:		{
											shared_ptr<Socks5SocketProxy<boost::asio::ip::tcp> > proxy = socket->addLayer<Socks5SocketProxy<boost::asio::ip::tcp> >();
											proxy->setProxyHost(Options::instance()->getOption<String>(Options::net_options::proxy_host).to_ascii());
											proxy->setProxyPort(Options::instance()->getOption<uint32>(Options::net_options::proxy_port));
											proxy->setProxyUsername(Options::instance()->getOption<String>(Options::net_options::proxy_username).to_ascii());
											proxy->setProxyPassword(Options::instance()->getOption<String>(Options::net_options::proxy_password).to_ascii());
										}

										break;

		default:						OS_ASSERTFALSE();
										OS_LOG_ERROR("Invalid proxy type");
										break;
		}
	}

	return socket;
}

shared_ptr<UDPSocket> Engine::createUDPSocket(shared_ptr<boost::asio::io_service> io_service, bool outgoing, bool unsafe)
{
	OS_ASSERT((unsafe == false) || outgoing);		// Le connessioni che non vanno all'esterno non possono essere insicure

	if(io_service == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	shared_ptr<UDPSocket> socket(OS_NEW UDPSocket(*io_service));

	// VERYURGENT: se il proxy supporta l'UDP configurare il socket
	/*
	if(outgoing)
	{
		// Locca le opzioni per assicurare che la lettura di ogni parametro di configurazione del proxy sia allineato
		shared_ptr<boost::recursive_mutex::scoped_lock> optionsLock = Options::instance()->lock();

		...
	}
	*/

	return socket;
}

int Engine::getTimeOffset()
{
	int timeOffset = Options::instance()->getOption<int32>(Options::language_options::time_offset);

	if(timeOffset == OS_TIME_DETECTION_SYSTEM)
	{
		return PlatformManager::instance()->getTimeOffset();
	}
	else if(timeOffset == OS_TIME_DETECTION_CULTURE)
	{
		shared_ptr<LanguageCulture> culture = LanguageManager::instance()->getCulture(Options::instance()->getDefaultLanguage());
		if(culture != null)
			return culture->getDefaultTimeOffset() * 60 * 60;
	}
	
	return timeOffset;
}

int Engine::getTimeDST()
{
	int timeDST = Options::instance()->getOption<int32>(Options::language_options::time_dst);

	if(timeDST == OS_TIME_DETECTION_SYSTEM)
	{
		return PlatformManager::instance()->getTimeDST();
	}
	
	return timeDST;
}

String Engine::formatSystemDate(const boost::posix_time::ptime &time, DateTime::DateTimePattern pattern)
{
	// 0.14
	shared_ptr<LanguageCulture> culture = LanguageManager::instance()->getCulture(Options::instance()->getDefaultLanguage());
	OS_ASSERT(culture != null);
	return culture->formatDate(time, pattern, getTimeOffset(), getTimeDST());

	/*
	String str;
	shared_ptr<LanguageCulture> systemCulture = LanguageManager::instance()->getSystemCulture();
	OS_ASSERT(culture != null);
	if(systemCulture != null)
		// VERYURGENT: corretto? Non bisogna chiamare systemCulture->getDefaultTimeOffset() perchè il timeoffset è già "applicato" al timestamp
		str = systemCulture->formatDate(time, pattern, 0, LanguageManager::instance()->getRootFolder());
	else
		str = boost::posix_time::to_simple_string(utils::ptime_truncate_milliseconds(time));

	return str;
	*/
}

/*
RAFFO: Ho creato questa, perchè:
- non riesco ad accedere a page.session.getService() da python
- Mi pare (non son sicuro) che manca qualche wrapper (Service o TCPSocket)
- Qualsiasi libreria di script ha una funzione per fare un fetch di un url, parametri minimi, roseo senza queste complicazioni.
Infatti pure noi lato c++ ho visto che generalmente creiamo service e socket al volo, al max si riusa il service della pagina.
Ho pensato che sarebbe stato meglio un costruttore HttpClient senza parametri, ma da lì non posso chiamare la createTCPSocket
(son dentro la lib Http e dovrei accedere a roba dentro Engine).

In realtà, attualmente non la uso in Python, uso quella sotto (parseXmlUrl).
Questo perchè non son riuscito a usare il seguente codice:

client = osiris.Engine.instance().createHttpClient()
client.perform(osiris.HttpUrl("http://www.osiris-sps.org/isis/packages.php?act=popular"))
document.parseBuffer(client.response.getResponseBuffer())

mi dice "invalid document structure" e non riesco a capire il perchè.
E' inutile che indago se poi sta roba va spostata, e non posso chiamare la document.parseUrl perchè non so come passare dei parametri.

non riesco ad accedere a getContent di IHttpMethod?
*/
shared_ptr<HttpClient> Engine::createHttpClient()
{
	shared_ptr<boost::asio::io_service> service = createAsioService();
	shared_ptr<TCPSocket> socket = Engine::instance()->createTCPSocket(service, true, true);
	shared_ptr<HttpClient> client(OS_NEW HttpClient(service,socket));
	client->getRequest()->setUserAgent(getDefaultHttpUserAgent());
	return client;
}

shared_ptr<XMLDocument> Engine::parseUrlXml(HttpUrl& url)
{
	shared_ptr<boost::asio::io_service> service = createAsioService();
	shared_ptr<TCPSocket> socket = Engine::instance()->createTCPSocket(service, true, true);

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());

	if(document->parseUrl(url, getDefaultHttpUserAgent(), service, socket) == true)
		return document;
	else
		return null;
}

void Engine::onUpdateOptions()
{
	updateOptions();
}

void Engine::updateOptions()
{
	m_impl->updateOptions();

	bool logEnabled = Options::instance()->getOption<bool>(Options::log_options::enable);

	LogManager::instance()->setEnabled(logEnabled);
	LogManager::instance()->setLevel(static_cast<LogLevel>(Options::instance()->getOption<uint32>(Options::log_options::level)));

	enableLogger(logEnabled && Options::instance()->getOption<bool>(Options::log_options::serialize));

	BandwidthManager::instance()->setMaxDownload(Options::instance()->getOption<uint32>(Options::net_options::max_download));
	BandwidthManager::instance()->setMaxUpload(Options::instance()->getOption<uint32>(Options::net_options::max_upload));

	LanguageManager::instance()->setAlternative(Options::instance()->getOption<String>(Options::language_options::alternative));
}

/* TOCLEAN
bool Engine::doSynchronizeClock(DateTime &dt)
{
	// Carica il sistema predefinito di aggiornamento del clock dalle opzioni
	String method = Options::instance()->getOption<String>(Options::system_options::clock_synchronization_method);
	if(method.empty())
		method = OS_CLOCK_SYNCHRONIZATION_METHOD_NTP;	// Di default usa l'ntp

	if(method == OS_CLOCK_SYNCHRONIZATION_METHOD_ISIS)
	{
		if(doSynchronizeClockWithIsis(dt))
			return true;

		if(doSynchronizeClockWithNTP(dt))
		{
			// Se la sincronizzazione con Isis fallisce ma l'NTP va a buon fine tiene quest'ultimo come metodo predefinito
			Options::instance()->setOption(Options::system_options::clock_synchronization_method, OS_CLOCK_SYNCHRONIZATION_METHOD_NTP);
			return true;
		}
	}
	else
	{
		if(doSynchronizeClockWithNTP(dt))
			return true;

		if(doSynchronizeClockWithIsis(dt))
		{
			// Se la sincronizzazione NTP fallisce ma con Isis va a buon fine tiene quest'ultimo come metodo predefinito
			Options::instance()->setOption(Options::system_options::clock_synchronization_method, OS_CLOCK_SYNCHRONIZATION_METHOD_ISIS);
			return true;
		}
	}

#ifdef OS_DEBUG
	OS_LOG_WARNING(_S("Cannot synchronize clock, using system's datetime debug configuration"));
	dt = DateTime::now();
	return true;
#endif

	OS_LOG_ERROR(_S("Cannot synchronize clock"));
	return false;
}
*/

/* TOCLEAN
bool Engine::doSynchronizeClockWithNTP(DateTime &dt, uint32 retries)
{
	// N.B.: creare qui un io_service temporaneo da passare a NTPClient::query perchè internamente chiama una io_service::run (va ristrutturata)
	shared_ptr<boost::asio::io_service> service = createAsioService();
	if(service == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	OS_ASSERT(retries > 0);
	while(retries > 0)
	{
		shared_ptr<NTPResponse> response = NTPClient::query(service, createUDPSocket(service, true, true), OS_NTP_DEFAULT_SERVER_NAME, OS_NTP_DEFAULT_SERVER_PORT);
		if(response != null)
		{
			dt = static_cast<boost::posix_time::ptime>(response->transmit_time);
			return true;
		}

		retries--;
	}

	return false;
}
*/

/* TOCLEAN
bool Engine::doSynchronizeClockWithIsis(DateTime &dt, uint32 retries)
{
	String systemUrl = Options::instance()->getIsisSecureLink(OS_ISIS_PAGE_CLOCK);
	if(systemUrl.empty())
		return false;

	shared_ptr<boost::asio::io_service> service = createAsioService();
	if(service == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	if(document->parseUrl(systemUrl.to_ascii(), getDefaultHttpUserAgent(), service, createTCPSocket(service, true, true)) == false)
	{
		OS_LOG_ERROR(_S("Cannot open url '") + systemUrl + _S("'"));
		return false;
	}

	shared_ptr<XMLNode> root = document->getRoot();
	if(root == null)
		return false;

	String clock = root->getAttributeString(_S("GMT"));
	return dt.fromString(clock);
}
*/

void Engine::initAdditionalLibraries()
{

}

void Engine::cleanupAdditionalLibraries()
{

}

void Engine::initLanguage()
{
	NotificationsManager::instance()->notify(_S("Initializing language..."));

	LanguageManager::instance()->loadCultures(utils::makeFilePath(Options::instance()->getSharePath(), OS_CULTURESXML));
	// LanguageManager::instance()->getRootFolder()->addPath(utils::makeFolderPath(Options::instance()->getSharePath(), OS_LANGUAGES_PATH));
}

void Engine::initBackgroundJobs() // TOCLEAN
{
	//addBackgroundJob(shared_ptr<IBackgroundJob>(OS_NEW BackgroundJobCallback<>(L"clock", false, peekBackgroundJobID(), boost::bind(&Engine::configureClock, this))));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END() 

//////////////////////////////////////////////////////////////////////
