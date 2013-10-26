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
#include "platformmanager.h"

#if OS_PLATFORM == OS_PLATFORM_WIN
	#include "platformwin.h"
#elif OS_PLATFORM == OS_PLATFORM_LINUX
	#include "platformlinux.h"
#elif OS_PLATFORM == OS_PLATFORM_MAC
	#include "platformmac.h"
#else
	#error "unsupported platform"
#endif

#include "boostthread.h"
#include "filesystem.h"
#include "unicode/utypes.h"
#include "unicode/uclean.h"
#include "unicode/uloc.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PlatformManager::ProcessDetails::ProcessDetails(uint32 id) : processID(id)
{

}

//////////////////////////////////////////////////////////////////////

PlatformManager::PlatformManager()
{

}

PlatformManager::~PlatformManager()
{

}

void PlatformManager::init(PlatformManager::ApplicationType type)
{
	// ICU Init
	UErrorCode status = U_ZERO_ERROR;
    u_init(&status);
	if(U_FAILURE(status))
	{
		const char* error = u_errorName(status);
		OS_TRACE("ICU init failed.");
		OS_TRACE(error);
		OS_ASSERTFALSE();
	}

	//uloc_setDefault("en_US", &status); 

	m_impl->init(type);	
}

void PlatformManager::cleanup()
{
	m_impl->cleanup();
}

String PlatformManager::getExpandedEnvironmentPath(const String &path) const
{
	return utils::standardisePath(m_impl->getExpandedEnvironmentPath(path));
}

String PlatformManager::getUserDataPath() const
{
	return utils::standardisePath(m_impl->getUserDataPath());
}

String PlatformManager::getTempPath() const
{
	return utils::standardisePath(m_impl->getTempPath());
}

String PlatformManager::getTempFilePath() const
{
	return utils::standardisePath(m_impl->getTempFilePath(), false);
}

bool PlatformManager::isPrivilegedUser() const
{
	return m_impl->isPrivilegedUser();
}

void * PlatformManager::findProcedure(handle hLibrary, const String &procedure) const
{
	return m_impl->findProcedure(hLibrary, procedure);
}

handle PlatformManager::loadLibrary(const String &libraryName) const
{
	return m_impl->loadLibrary(libraryName);
}

void PlatformManager::unloadLibrary(handle hLibrary) const
{
	m_impl->unloadLibrary(hLibrary);
}

boost::any PlatformManager::getCurrentThreadID()
{
	return m_impl->getCurrentThreadID();
}

bool PlatformManager::setThreadPriority(const boost::any &threadID, TaskPriority priority)
{
	return m_impl->setThreadPriority(threadID, priority);
}

bool PlatformManager::compareThreadsID(const boost::any &first, const boost::any &second)
{
	return m_impl->compareThreadsID(first, second);
}

uint32 PlatformManager::getCurrentProcessID()
{
	return m_impl->getCurrentProcessID();
}

shared_ptr<PlatformManager::ProcessDetails> PlatformManager::getProcessDetails(uint32 processID)
{
	shared_ptr<PlatformManager::ProcessDetails> processDetails = m_impl->getProcessDetails(processID);
	OS_ASSERT((processDetails == null) || (processDetails->executablePath == utils::standardisePath(processDetails->executablePath, false)));	// Se i dettagli del processo sono stati caricati, verifica che il path dell'eseguibile sia stato standardizzato
	return processDetails;
}

int PlatformManager::getTimeOffset() const
{
	return m_impl->getTimeOffset();
}

int PlatformManager::getTimeDST() const
{
	return m_impl->getTimeDST();
}

uint32 PlatformManager::getOptimalHardwareConcurrency() const
{
	return static_cast<uint32>(boost::thread::hardware_concurrency() + 1);
}



double PlatformManager::tickCount()
{
	return m_impl->tickCount();
}

void PlatformManager::trace(const std::string &message, const char *file, size_t line, const char *function)
{
	pimpl<PlatformManager>::trace(message, file, line, function);
}

void PlatformManager::sleep(uint32 milliseconds)
{
	m_impl->sleep(milliseconds);
}

void PlatformManager::yield()
{
	boost::thread::yield();
}

String PlatformManager::normalizePath(const String& path) const
{
	return m_impl->normalizePath(path);
}

bool PlatformManager::contentTypeFromExtension(const String &extension, String &content_type)
{
	return m_impl->contentTypeFromExtension(extension, content_type);
}

bool PlatformManager::extensionFromContentType(const String &content_type, String &extension)
{
	return m_impl->extensionFromContentType(content_type, extension);
}

bool PlatformManager::execute(const String &command,const String &parameters)
{
	return m_impl->execute(command, parameters);
}

String PlatformManager::getSystemCulture()
{
	return m_impl->getSystemCulture();
}

String PlatformManager::getEnv(const String &name)
{
	return m_impl->getEnv(name);
}

bool PlatformManager::setEnv(const String &name, const String &value)
{
	OS_ASSERT(name.empty() == false);
	return m_impl->setEnv(name, value);
}

String PlatformManager::getLastError() const
{
    return m_impl->getLastError();
}

FILE * PlatformManager::fileOpen(const String &filename, const String &mode)
{
	return m_impl->fileOpen(filename, mode);
}

bool PlatformManager::fileClose(FILE *handle)
{
	return m_impl->fileClose(handle);
}

uint32 PlatformManager::fileRead(FILE *handle, void *v, uint32 size)
{
	return m_impl->fileRead(handle, v, size);
}

uint32 PlatformManager::fileWrite(FILE *handle, const void *v, uint32 size)
{
	return m_impl->fileWrite(handle, v, size);
}

bool PlatformManager::fileSeek(FILE *handle, uint64 offset, int32 origin)
{
	return m_impl->fileSeek(handle, offset, origin);
}

uint64 PlatformManager::filePosition(FILE *handle)
{
	return m_impl->filePosition(handle);
}

bool PlatformManager::fileFlush(FILE *handle)
{
	return m_impl->fileFlush(handle);
}

bool PlatformManager::fileEof(FILE *handle)
{
	return m_impl->fileEof(handle);
}

bool PlatformManager::fileStats(FILE *handle, boost::posix_time::ptime *timeCreation, boost::posix_time::ptime *timeLastModify, boost::posix_time::ptime *timeLastAccess)
{
	return m_impl->fileStats(handle, timeCreation, timeLastModify, timeLastAccess);
}

void PlatformManager::debugBreak()
{
	return m_impl->debugBreak();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
