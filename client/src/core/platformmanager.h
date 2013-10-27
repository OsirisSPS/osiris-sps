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

#ifndef _OS_CORE_PLATFORMMANAGER_H
#define _OS_CORE_PLATFORMMANAGER_H

#include "boost/signals.hpp"
#include "base/object.h"
#include "pimpl_ptr.h"
#include "singleton.h"
#include "threads.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Thread;

//////////////////////////////////////////////////////////////////////

class CoreExport PlatformManager : public StaticSingleton<PlatformManager>,
								   public Object
{
// Enumerated types
public:
	enum ApplicationType
	{
		applicationConsole	= 0,
		applicationGUI		= 1,
	};

	struct CoreExport ProcessDetails
	{
		ProcessDetails(uint32 id);

		uint32 processID;
		String executablePath;
	};

// Construction
protected:
	PlatformManager();

public:
	virtual ~PlatformManager();

// Operations
public:
	void init(ApplicationType type);
	void cleanup();

	String getExpandedEnvironmentPath(const String &path) const;
	String getUserDataPath() const;

	String getTempPath() const;
	String getTempFilePath() const;

	bool isPrivilegedUser() const;

	void * findProcedure(handle hLibrary, const String &procedure) const;
	handle loadLibrary(const String &libraryName) const;
	void unloadLibrary(handle hLibrary) const;

	boost::any getCurrentThreadID();
	bool setThreadPriority(const boost::any &threadID, TaskPriority priority);
	bool compareThreadsID(const boost::any &first, const boost::any &second);

	uint32 getCurrentProcessID();
	shared_ptr<ProcessDetails> getProcessDetails(uint32 processID);

	int getTimeOffset() const;
	int getTimeDST() const;

	uint32 getOptimalHardwareConcurrency() const;

	double tickCount();
	static void trace(const std::string &message, const char *file = 0, size_t line = 0, const char *function = 0);	// Non chiamare questa funzione direttamente, usare OS_TRACE
	void sleep(uint32 milliseconds);
	void yield();

	String normalizePath(const String& path) const;
	bool contentTypeFromExtension(const String &extension, String &content_type);
	bool extensionFromContentType(const String &content_type, String &extension);

	bool execute(const String &command, const String &parameters = String::EMPTY);
	String getSystemCulture();

	String getEnv(const String &name);
	bool setEnv(const String &name, const String &value);

	String getLastError() const;

	FILE * fileOpen(const String &filename, const String &mode);
	bool fileClose(FILE *handle);
	uint32 fileRead(FILE *handle, void *v, uint32 size);
	uint32 fileWrite(FILE *handle, const void *v, uint32 size);
	bool fileSeek(FILE *handle, uint64 offset, int32 origin);
	uint64 filePosition(FILE *handle);
	bool fileFlush(FILE *handle);
	bool fileEof(FILE *handle);
	bool fileStats(FILE *handle, boost::posix_time::ptime *timeCreation, boost::posix_time::ptime *timeLastModify = null, boost::posix_time::ptime *timeLastAccess = null);

	void debugBreak();

private:
	pimpl_of<PlatformManager>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#ifdef OS_DEBUG
	#define OS_TRACE(message)										OS_NAMESPACE_NAME::PlatformManager::trace(message)
	#define OS_TRACE_LOCALIZED(message)								OS_NAMESPACE_NAME::PlatformManager::trace(message, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)
	#define OS_TRACE_CUSTOM(message, file, line, function)			OS_NAMESPACE_NAME::PlatformManager::trace(message, file, line, function)
#else
	#if OS_COMPILER	== OS_COMPILER_MSVC
		#define OS_TRACE(message)									__noop
	#else
		#define OS_TRACE(message)									((void)0)
	#endif

	#define OS_TRACE_LOCALIZED(message)								OS_TRACE(message)
	#define OS_TRACE_CUSTOM(message, file, line, function)			OS_TRACE(message)
#endif // OS_DEBUG

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_PLATFORMMANAGER_H
