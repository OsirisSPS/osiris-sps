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
#include "platformlinux.h"

#include "algorithms.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/filesystem.hpp"
#include "buffer.h"
#include "conversions.h"
#include "cryptmanager.h"
#include <dlfcn.h>
#include "filesystem.h"
#include "format.h"
#include <fstream>
#include <iostream>
#include "log.h"
#include <pthread.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static int32 convertThreadPriority(TaskPriority priority)
{
    // N.B.: in linux valori negativi indicano una priorità maggiore (il range va da -20 a 20)

	switch(priority)
	{
	case taskPriorityCritical:		return -20;
	case taskPriorityHigh:			return -15;
	case taskPriorityAboveNormal:	return -10;
	case taskPriorityNormal:		return 0;
	case taskPriorityBelowNormal:	return 10;
	case taskPriorityLow:			return 15;
	case taskPriorityIdle:			return 20;

	default:				        OS_ASSERTFALSE();
                                    break;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////

pimpl<PlatformManager>::pimpl()
{
    gettimeofday(&m_clock, NULL);
}

pimpl<PlatformManager>::~pimpl()
{

}

void pimpl<PlatformManager>::init(PlatformManager::ApplicationType type)
{
	loadMimeTypes();
}

void pimpl<PlatformManager>::cleanup()
{

}

String pimpl<PlatformManager>::getExpandedEnvironmentPath(const String &path) const
{
    // Non ne ho trovata una generica...
    String output = path;
    output.replace_all(_S("$HOME"), String(::getenv("HOME")));

	return output;
}

String pimpl<PlatformManager>::getUserDataPath() const
{
    char *homeDir = ::getenv("HOME");
    if(homeDir != nullptr)
        return homeDir;

	OS_ASSERTFALSE();
	return String::EMPTY;
}

String pimpl<PlatformManager>::getTempPath() const
{
    return _S("/tmp");
}

String pimpl<PlatformManager>::getTempFilePath() const
{
    // TODO: da rivedere

    String tempPath = getTempPath();

    String filePath;

    bool exists = true;
    while(exists)
    {
        // Genera un nome di file casuale
        String filename = CryptManager::instance()->randomMD5().toHex();
        filename.append(_S(".tmp"));
        // Compone il path completo
        filePath = utils::makeFilePath(tempPath, filename);
        // Controlla se il file esiste già
        if(FileSystem::instance()->exists(filePath) == false)
        {
            // Crea il file
            std::ofstream tempFile(filePath.to_utf8().c_str(), std::ios::out);
            if(tempFile.is_open() == false)
                return String::EMPTY;

            tempFile.close();
            exists = false;
        }
    }

    return filePath;
}

bool pimpl<PlatformManager>::isPrivilegedUser() const
{
	// L'utente root ha come ID zero
	return geteuid() == 0;
}

int pimpl<PlatformManager>::getTimeOffset() const
{
	//It is used to access DST and other timezone related information and sets all timezone global variables
    tzset(); 

	return -timezone; // CEST, not WEST
}

int pimpl<PlatformManager>::getTimeDST() const
{
	//It is used to access DST and other timezone related information and sets all timezone global variables
    tzset(); 

	// TODO: Not universally correct.
	if(daylight != 0)
		return 3600; // One hour is the default
	else
		return 0;
}

void * pimpl<PlatformManager>::findProcedure(handle hLibrary, const String &procedure) const
{
    return dlsym(reinterpret_cast<void *>(hLibrary), procedure.to_utf8().c_str());
}

handle pimpl<PlatformManager>::loadLibrary(const String &libraryName) const
{
    handle h = reinterpret_cast<handle>(dlopen(libraryName.to_utf8().c_str(), RTLD_LAZY));
    if(h == nullptr)
        OS_LOG_ERROR(_S("Error loading library '") + libraryName + _S("':") + dlerror());

    return h;
}

void pimpl<PlatformManager>::unloadLibrary(handle hLibrary) const
{
    dlclose(reinterpret_cast<void *>(hLibrary));
}

boost::any pimpl<PlatformManager>::getCurrentThreadID()
{
    return pthread_self();
}

bool pimpl<PlatformManager>::setThreadPriority(const boost::any &threadID, TaskPriority priority)
{

	OS_ASSERT(compareThreadsID(threadID, getCurrentThreadID()));

	// 0.13
    //return setpriority(PRIO_PROCESS, 0, convertThreadPriority(priority)) == 0;

    // 0.14 - Equal to PlatformMac version. The above method don't work under Mac.
    int policy = 0;
	sched_param param;
	if(pthread_getschedparam(boost::any_cast<pthread_t>(threadID), &policy, &param) != 0)
		return false;

	param.sched_priority = convertThreadPriority(priority);

	return pthread_setschedparam(boost::any_cast<pthread_t>(threadID), policy, &param) == 0;
}

bool pimpl<PlatformManager>::compareThreadsID(const boost::any &first, const boost::any &second)
{
	OS_ASSERT(boost::any_cast<pthread_t>(&first) != nullptr);
	OS_ASSERT(boost::any_cast<pthread_t>(&second) != nullptr);

	return pthread_equal(boost::any_cast<pthread_t>(first), boost::any_cast<pthread_t>(second)) != 0;
}

uint32 pimpl<PlatformManager>::getCurrentProcessID()
{
	return static_cast<uint32>(getpid());
}

shared_ptr<PlatformManager::ProcessDetails> pimpl<PlatformManager>::getProcessDetails(uint32 processID)
{
	/*
	int32 result = kill(static_cast<pid_t>(processID), 0);
	return (result == 0 || result == EPERM);
	*/

	int32 result = kill(static_cast<pid_t>(processID), 0);
	bool exists = (result == 0 || result == EPERM);
	if(exists == false)
		return nullptr;

	shared_ptr<PlatformManager::ProcessDetails> processDetails(new PlatformManager::ProcessDetails(processID));

	char buffer[OS_MAX_PATH];
    OS_ZEROMEMORY(buffer, OS_MAX_PATH);
	ssize_t count = readlink((boost::format("/proc/%d/exe") % processID).str().c_str(), buffer, OS_MAX_PATH);
    if(count != -1)
    {
		std::string path;
		path.assign(buffer, count);
        processDetails->executablePath = utils::standardisePath(path, false);
    }

	return processDetails;
}

double pimpl<PlatformManager>::tickCount()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return static_cast<double>((now.tv_sec - m_clock.tv_sec)*1000+(now.tv_usec-m_clock.tv_usec)/1000);
}

void pimpl<PlatformManager>::trace(const std::string &message, const char *file, size_t line, const char *function)
{
    if((file != nullptr) && file[0])
    {
        std::clog << file;

        if(line != 0)
            std::clog << " (line: " << line << ")";

        if(function != nullptr)
            std::clog << " (function: " << function << ")";

        std::clog << ": ";
    }

    std::clog << message;
}

String pimpl<PlatformManager>::normalizePath(const String& path) const
{
	return path;
}

bool pimpl<PlatformManager>::contentTypeFromExtension(const String &extension, String &content_type)
{
	OS_ASSERT(extension == algorithms::to_lower_copy(extension) && extension.starts_with(_S(".")) == false);

	std::multimap<String, String>::const_iterator i = m_extensions.find(extension);
	if(i == m_extensions.end())
		return false;

	content_type = i->second;
	return true;
}

bool pimpl<PlatformManager>::extensionFromContentType(const String &content_type, String &extension)
{
	OS_ASSERT(content_type == algorithms::to_lower_copy(content_type));

	std::multimap<String, String>::const_iterator i = m_mimeTypes.find(content_type);
	if(i == m_mimeTypes.end())
		return false;

	extension = i->second;
	return true;
}

bool pimpl<PlatformManager>::execute(const String &command, const String &parameters)
{
    // URGENT: fixme

    String str = command;
    if(parameters.empty() == false)
    {
        str.append(_S(" "));
        str.append(parameters);
    }

    system(str.to_utf8().c_str());

    return true;
}

String pimpl<PlatformManager>::getSystemCulture()
{
    String envLang = String(::getenv("LANG")); // Ex. "it_IT.UTF-8"
    envLang.replace_all(_S("_"),_S("-"));

    String::size_type posDot = envLang.find(_S("."));
    if(posDot == String::npos)
        return String::EMPTY;
    else
        return envLang.substr(0,posDot);
}

String pimpl<PlatformManager>::getEnv(const String &name)
{
	return utils::safe_string<std::string>(getenv(name.to_ascii().c_str()));
}

bool pimpl<PlatformManager>::setEnv(const String &name, const String &value)
{
	// N.B.: non usare putenv sotto linux perchè il puntatore passato viene salvato internamente
    return setenv(name.to_ascii().c_str(), value.to_ascii().c_str(), 1) == 0;
}

String pimpl<PlatformManager>::getLastError() const
{
    char buffer[1024];
    OS_ZEROMEMORY(buffer, 1024);
    //if(strerror_r(errno, buffer, 1024) == nullptr)
    if(strerror_r(errno, buffer, 1024) == 0)
        return String::EMPTY;

    return buffer;
}

void pimpl<PlatformManager>::sleep(uint32 milliseconds)
{
   usleep(milliseconds * 1000);
}

FILE * pimpl<PlatformManager>::fileOpen(const String &filename, const String &mode)
{
    // TODO: fixme
	return fopen(filename.to_utf8().c_str(), mode.to_utf8().c_str());
}

bool pimpl<PlatformManager>::fileClose(FILE *handle)
{
	return fclose(handle) == 0;
}

uint32 pimpl<PlatformManager>::fileRead(FILE *handle, void *v, uint32 size)
{
	return static_cast<uint32>(fread(reinterpret_cast<byte *>(v), 1, size, handle));
}

uint32 pimpl<PlatformManager>::fileWrite(FILE *handle, const void *v, uint32 size)
{
	return static_cast<uint32>(fwrite(reinterpret_cast<const byte *>(v), 1, size, handle));
}

bool pimpl<PlatformManager>::fileSeek(FILE *handle, uint64 offset, int32 origin)
{
	// TODO: supportare uint64 per i files oltre 2 giga
	return fseek(handle, offset, origin) == 0;
}

uint64 pimpl<PlatformManager>::filePosition(FILE *handle)
{
	// TODO: supportare uint64 per i files oltre 2 giga
	return ftell(handle);
}

bool pimpl<PlatformManager>::fileFlush(FILE *handle)
{
	return fflush(handle) == 0;
}

bool pimpl<PlatformManager>::fileEof(FILE *handle)
{
	return feof(handle) != 0;
}

bool pimpl<PlatformManager>::fileStats(FILE *handle, boost::posix_time::ptime *timeCreation, boost::posix_time::ptime *timeLastModify, boost::posix_time::ptime *timeLastAccess)
{
	if(handle == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	struct stat stats;
	if(fstat(fileno(handle), &stats) != 0)
		return false;

	if(timeCreation != nullptr)
		*timeCreation = boost::posix_time::from_time_t(stats.st_ctime);

	if(timeLastAccess != nullptr)
		*timeLastAccess = boost::posix_time::from_time_t(stats.st_atime);

	if(timeLastModify != nullptr)
		*timeLastModify = boost::posix_time::from_time_t(stats.st_mtime);

	return true;
}

bool pimpl<PlatformManager>::loadMimeTypes()
{
	m_extensions.clear();
	m_mimeTypes.clear();

	std::ifstream file("/etc/mime.types");
    if(file.is_open() == false || file.bad())
        return false;

	while(file.eof() == false)
    {
		std::string tmp;
        std::getline(file, tmp);

		String str(tmp);
		if(str.empty() || str.starts_with(_S("#")))
			continue;

		StringVector tokens;
		if(utils::split(str, _S(" \t"), tokens, true) > 1)
		{
			String contentType = algorithms::to_lower_copy(tokens[0]);
			for(size_t i = 1; i < tokens.size(); i++)
			{
				String extension = algorithms::to_lower_copy(tokens[i]);

				m_extensions.insert(std::make_pair(extension, contentType));
				m_mimeTypes.insert(std::make_pair(contentType, extension));
			}
		}
    }

	return true;
}

void pimpl<PlatformManager>::debugBreak()
{
	// TODO: fixme
	OS_ASSERTFALSE();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
