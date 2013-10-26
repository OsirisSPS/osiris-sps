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
#include "iapplication.h"

#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/interprocess/file_mapping.hpp"
#include "boost/interprocess/mapped_region.hpp"
#include "boost/interprocess/sync/interprocess_mutex.hpp"
#include "boost/interprocess/sync/scoped_lock.hpp"
#include "buffer.h"
#include "condition.h"
#include "conversions.h"
#include "engine.h"
#include "file.h"
#include "filesystem.h"
#include "logmanager.h"
#include "lucene.h"
#include "notificationsmanager.h"
#include "options.h"
#include "thread.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

#define OS_PROCESS_SHARED_MEMORY_TIMER				1000
#define OS_PROCESS_SHARED_MEMORY_LOCK_TIMEOUT		10000

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IApplication::NotifyListener : public Thread
{
	typedef Thread ThreadBase;

	typedef boost::function<void ()> Callback;

// Construction
public:
	NotifyListener(const Callback &callback);
	virtual ~NotifyListener();

// Thread interface
public:
	virtual void run();

// Thread overrides
public:
	virtual void stop();

private:
	Condition m_condition;
	Callback m_callback;
};

//////////////////////////////////////////////////////////////////////

IApplication::NotifyListener::NotifyListener(const Callback &callback) : m_callback(callback)
{

}

IApplication::NotifyListener::~NotifyListener()
{

}

void IApplication::NotifyListener::run()
{
	while(stopped() == false)
	{
		m_callback();
		m_condition.wait(OS_PROCESS_SHARED_MEMORY_TIMER); 
	}
}

void IApplication::NotifyListener::stop()
{
	m_condition.reset();

	ThreadBase::stop();
}

//////////////////////////////////////////////////////////////////////

struct IApplication::ProcessData
{
	ProcessData() : processID(0)
	{

	}

	static std::string getSyncFilePath()
	{
		return utils::makeFilePath(Options::instance()->getTempPath(), OS_OSIRIS_PROCESS_GUID).to_ascii();
	}

	static std::string getDataFilePath(uint32 id)
	{
		return utils::makeFilePath(Options::instance()->getTempPath(), std::string(OS_OSIRIS_PROCESS_GUID) + "_" + conversions::to_string(id) + "_data").to_ascii();
	}

	std::string getDataFilePath()
	{
		return getDataFilePath(processID);
	}

	boost::interprocess::interprocess_mutex mutex;
	uint32 processID;
};

//////////////////////////////////////////////////////////////////////

IApplication::IApplication(PlatformManager::ApplicationType applicationType) : m_recoveryMode(false),
																			   m_verbose(false),
																			   m_processID(0),
																			   m_primaryProcessID(0)
{
	PlatformManager::instance()->init(applicationType);
	LogManager::instance()->init();
}

IApplication::~IApplication()
{
	// Termina lucene
	_lucene_shutdown();

	LogManager::instance()->cleanup();
	PlatformManager::instance()->cleanup();
}

bool IApplication::isPrimaryProcess() const
{
	OS_ASSERT(m_processID != 0);
	OS_ASSERT(m_primaryProcessID != 0);

	return m_processID == m_primaryProcessID;
}

bool IApplication::start()
{
	if(getVerbose())
	{
		Options::instance()->setOption<bool>(Options::log_options::enable, true);
		Options::instance()->setOption<uint32>(Options::log_options::level, logLevelDebug);
		Options::instance()->setOption<bool>(Options::log_options::serialize, true);		
	}

	OS_ASSERT(Engine::exists() == false);
	if(Engine::create()->start(getRecoveryMode()) == false)
		return false;

	// N.B.: è necessario avviare il thread di listener dopo aver avviato l'engine in modo da evitare di processare comandi prima che quest'ultimo sia stato inizializzato
	if(m_notifyListener != null)
		m_notifyListener->start();

	return true;
}

void IApplication::stop()
{
	if(m_notifyListener != null)
		m_notifyListener->stop();

    if(Engine::exists())
	{
        Engine::instance()->stop();
		Engine::destroy();
	}
}

bool IApplication::init(int argc, char *argv[])
{
	if(argc == 0)
	{
		OS_ASSERTFALSE();

		OS_LOG_ERROR("Invalid args");
		return false;
	}

	FileSystem::instance()->initExecutableName(argv[0]);
	return init(FileSystem::instance()->getExecutablePath());		
}

bool IApplication::init(int argc, wchar_t *argv[])
{
	if(argc == 0)
	{
		OS_ASSERTFALSE();

		OS_LOG_ERROR("Invalid args");
		return false;
	}

	FileSystem::instance()->initExecutableName(argv[0]);
	return init(FileSystem::instance()->getExecutablePath());		
}

bool IApplication::init(const String &rootPath)
{
	String versionName;
	versionName.append(OS_OSIRIS_APPLICATION_NAME);
	versionName.append(String::format(_S(" %d.%d").c_str(), OS_OSIRIS_VERSION_MAJOR, OS_OSIRIS_VERSION_MINOR));
	/* // Show release name only with "... is ready.".
	if(OS_OSIRIS_RELEASE_NAME != String::EMPTY)
	{
		versionName.append(_S(" "));
		versionName.append(OS_OSIRIS_RELEASE_NAME);
	}
	*/
	NotificationsManager::instance()->notify(versionName + _S(" is starting..."));
	
	if(PlatformManager::instance()->isPrivilegedUser())
		NotificationsManager::instance()->notify(_S("Running with a user with system privileges."));
	else
		NotificationsManager::instance()->notify(_S("Running with a user without system privileges. Some options maybe unavailable."));

	NotificationsManager::instance()->notify(_S("Platform: ") + Engine::getPlatformName());

	NotificationsManager::instance()->notify("Initializing options...");

	if(Options::instance()->load(rootPath) == false)
	{
		OS_LOG_ERROR("Error loading options");
		return false;
	}

	// Compatibility - TOCLEAN - Inutile collocarla più pulita, tanto la tolgo proprio con Razor (che non supporterà l'aggiornamento da 0.X).
	if(FileSystem::instance()->directoryExists(utils::makeFolderPath(Options::instance()->getDataPath(), OS_PLUGINS_PATH)))
		FileSystem::instance()->rename(utils::makeFolderPath(Options::instance()->getDataPath(), OS_PLUGINS_PATH), utils::makeFolderPath(Options::instance()->getDataPath(), OS_STORE_PATH));

	initProcess();

	if(isPrimaryProcess() == false)
		return false;

	return true;
}

void IApplication::cleanup()
{
	Options::instance()->save();
	
	cleanupProcess();

	Options::instance()->cleanup();
}

void IApplication::addProcessCommand(const String &command)
{
	OS_ASSERT(Engine::exists() == false);

	if(command.empty())
	{
		OS_ASSERTFALSE();
		return;
	}

	if(m_commands.empty() == false)
		m_commands.append(OS_IAPPLICATION_COMMANDS_SEPARATOR);

	m_commands.append(command);
}

void IApplication::initProcess()
{
	m_processID = PlatformManager::instance()->getCurrentProcessID();

	if(queryPrimaryProcessID(m_processID, m_primaryProcessID) == false)
	{
		m_primaryProcessID = m_processID;
		registerAsPrimaryProcess();
	}

	if(m_commands.empty() == false)
	{
		postPrimaryProcessCommand(m_primaryProcessID, m_commands.to_ascii());
		m_commands.clear();
	}

	if(isPrimaryProcess())
		m_notifyListener.reset(OS_NEW NotifyListener(boost::bind(&IApplication::handleProcessCommandsCallback, this)));
}

void IApplication::cleanupProcess()
{
	OS_ASSERT(isPrimaryProcess());

	if(m_notifyListener != null)
	{
		OS_ASSERT(m_notifyListener->running() == false);
		m_notifyListener.reset();
	}

	try
	{
		String syncFilePath = ProcessData::getSyncFilePath();

		{
			boost::interprocess::file_mapping shared_file(syncFilePath.to_ascii().c_str(), boost::interprocess::read_write);

			boost::interprocess::mapped_region shared_region(shared_file, boost::interprocess::read_write, 0, sizeof(ProcessData));

			ProcessData *processData = OS_PLACEMENT_NEW(shared_region.get_address(), ProcessData);

			boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(processData->mutex, boost::posix_time::microsec_clock::universal_time() + boost::posix_time::millisec(OS_PROCESS_SHARED_MEMORY_LOCK_TIMEOUT));
			if(lock)
			{
				String dataFilePath = ProcessData::getDataFilePath(m_processID);
				if(FileSystem::instance()->remove(dataFilePath) == false)
					OS_LOG_ERROR("Error removing file '" + dataFilePath + "'");
			}
			else
			{
				OS_LOG_ERROR("Process exception: lock timeout");
			}
		}

		if(isPrimaryProcess())
		{			
			if(boost::interprocess::file_mapping::remove(syncFilePath.to_ascii().c_str()) == false)
				OS_LOG_ERROR("Process exception: error removing file '" + syncFilePath + "'");
		}
	}
	catch(std::exception &e)
	{
		OS_LOG_DEBUG(_S("Process exception: ") + e.what());
	}
}

bool IApplication::registerAsPrimaryProcess()
{
	OS_ASSERT(m_processID != 0);
	OS_ASSERT(isPrimaryProcess());

	try
	{
		Buffer buffer;
		buffer.reserve(sizeof(ProcessData));

		ProcessData *processData = OS_PLACEMENT_NEW(buffer.getData(), ProcessData);

		processData->processID = m_processID;
		if(buffer.save(ProcessData::getSyncFilePath()) == false)
			return false;	
	}
	catch(std::exception &e)
	{
		OS_LOG_DEBUG(_S("Process exception: ") + e.what());
		return false;
	}

	return true;
}

bool IApplication::queryPrimaryProcessID(uint32 currentProcessID, uint32 &primaryProcessID) const
{
	primaryProcessID = 0;
	uint32 lastProcessID = 0;
	bool fileOpened = false;

	try
	{
		String syncFilePath = ProcessData::getSyncFilePath();
		boost::interprocess::file_mapping shared_file(syncFilePath.to_ascii().c_str(), boost::interprocess::read_write);

		fileOpened = true;
		boost::interprocess::mapped_region shared_region(shared_file, boost::interprocess::read_write);
		if(shared_region.get_size() != sizeof(ProcessData))
		{
			OS_LOG_ERROR("Process exception: corrupted file '" + syncFilePath + "'");
			return false;
		}

		ProcessData *processData = static_cast<ProcessData *>(shared_region.get_address());			

		boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(processData->mutex, boost::posix_time::microsec_clock::universal_time() + boost::posix_time::millisec(OS_PROCESS_SHARED_MEMORY_LOCK_TIMEOUT));
		if(!lock)
		{
			OS_LOG_ERROR(_S("Process exception: lock timeout"));
			return false;
		}

		lastProcessID = processData->processID;
	}
	catch(std::exception &e)
	{
		if(fileOpened)
			OS_LOG_DEBUG(_S("Process exception: ") + e.what());

		return false;
	}

	shared_ptr<PlatformManager::ProcessDetails> lastProcessDetails = PlatformManager::instance()->getProcessDetails(lastProcessID);
	if(lastProcessDetails == null)
		return false;

	// N.B.: non è sufficiente verificare se il processo è ancora attivo in quanto potrebbe essere terminato e il sistema operativo
	// abbia assegnato ad un nuovo processo l'ID appena letto, pertanto, se disponibile, bisogna verificare che i path degli eseguibili coincidano

	shared_ptr<PlatformManager::ProcessDetails> currentProcessDetails = PlatformManager::instance()->getProcessDetails(currentProcessID);
	OS_ASSERT(currentProcessDetails != null);	// I dettagli sul processo corrente dovrebbero essere sempre disponibili
	if(currentProcessDetails != null)
	{
		// Verifica che sia stato possibile determinare il path degli eseguibili di entrambe i processi
		if((currentProcessDetails->executablePath.empty() == false) && (lastProcessDetails->executablePath.empty() == false))
		{
			if(FileSystem::instance()->comparePaths(FileSystem::instance()->getFilePath(currentProcessDetails->executablePath), FileSystem::instance()->getFilePath(lastProcessDetails->executablePath)) == false)
				return false;			
		}
	}

	primaryProcessID = lastProcessID;
	return true;
}

bool IApplication::postPrimaryProcessCommand(uint32 primaryProcessID, std::string command)
{
	if(command.empty())
	{
		OS_ASSERTFALSE();
		return false;
	}

	try
	{
		String syncFilePath = ProcessData::getSyncFilePath();
		boost::interprocess::file_mapping shared_file(syncFilePath.to_ascii().c_str(), boost::interprocess::read_write);
		boost::interprocess::mapped_region shared_region(shared_file, boost::interprocess::read_write);
		if(shared_region.get_size() != sizeof(ProcessData))
		{
			OS_LOG_ERROR("Process exception: corrupted file '" + syncFilePath + "'");
			return false;
		}

		ProcessData *processData = static_cast<ProcessData *>(shared_region.get_address());			

		boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(processData->mutex, boost::posix_time::microsec_clock::universal_time() + boost::posix_time::millisec(OS_PROCESS_SHARED_MEMORY_LOCK_TIMEOUT));
		if(!lock)
		{
			OS_LOG_ERROR(_S("Process exception: lock timeout"));
			return false;
		}

		String dataFilePath = processData->getDataFilePath();

		shared_ptr<File> dataFile(OS_NEW File());
		if(dataFile->open(dataFilePath, File::ofWrite | File::ofNoTruncate) == false)
		{
			OS_LOG_ERROR("Error opening or creating file '" + dataFilePath + "'");
			return false;
		}

		if(dataFile->size() > 0)
		{
			dataFile->seekToEnd();
			command.insert(0, OS_IAPPLICATION_COMMANDS_SEPARATOR);
		}
		
		if(dataFile->write(command.data(), static_cast<uint32>(command.size())) != static_cast<uint32>(command.size()))
		{
			OS_LOG_ERROR("Error writing file '" + dataFilePath + "'");
			return false;
		}
	}
	catch(std::exception &e)
	{
		OS_LOG_DEBUG(_S("Process exception: ") + e.what());
		return false;
	}

	return true;
}

bool IApplication::peekProcessData(std::string &data)
{
	data.clear();

	try
	{
		String syncFilePath = ProcessData::getSyncFilePath();
		boost::interprocess::file_mapping shared_file(syncFilePath.to_ascii().c_str(), boost::interprocess::read_write);
		boost::interprocess::mapped_region shared_region(shared_file, boost::interprocess::read_write);
		if(shared_region.get_size() != sizeof(ProcessData))
		{
			OS_LOG_ERROR("Process exception: corrupted file '" + syncFilePath + "'");
			return false;
		}

		ProcessData *processData = static_cast<ProcessData *>(shared_region.get_address());			

		boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(processData->mutex, boost::posix_time::microsec_clock::universal_time() + boost::posix_time::millisec(OS_PROCESS_SHARED_MEMORY_LOCK_TIMEOUT));
		if(!lock)
		{
			OS_LOG_ERROR(_S("Process exception: lock timeout"));
			return false;
		}

		String dataFilePath = processData->getDataFilePath();

		Buffer dataFileBuffer;
		if(dataFileBuffer.load(dataFilePath) == false)
			return false;

		if(FileSystem::instance()->remove(dataFilePath) == false)
		{
			OS_LOG_ERROR("Cannot remove file '" + dataFilePath + "'");
			return false;
		}

		data.assign(reinterpret_cast<const char *>(dataFileBuffer.getData()), dataFileBuffer.getSize());
	}
	catch(std::exception &e)
	{
		OS_LOG_DEBUG(_S("Process exception: ") + e.what());
		return false;
	}

	return data.empty() == false;
}

void IApplication::handleProcessCommandsCallback()
{
	std::string data;
	if(peekProcessData(data) == false)
		return;

	OS_ASSERT(data.empty() == false);

	handleProcessCommands(data);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
