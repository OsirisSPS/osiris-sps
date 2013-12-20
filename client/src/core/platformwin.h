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

#ifndef _OS_CORE_PLATFORMWIN_H
#define _OS_CORE_PLATFORMWIN_H

#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template<>
class pimpl<PlatformManager>
{
	class RegKey;

// Construction
public:
	pimpl();
	virtual ~pimpl();

// Operations
public:
	void init(PlatformManager::ApplicationType type);
	void cleanup();

	String getExpandedEnvironmentPath(const String &path) const;
	String getUserDataPath() const;
	String getTempPath() const;
	String getTempFilePath() const;	
	bool isPrivilegedUser() const;
	int getTimeOffset() const;
	int getTimeDST() const;
	void * findProcedure(handle hLibrary, const String &procedure) const;
	handle loadLibrary(const String &libraryName) const;
	void unloadLibrary(handle hLibrary) const;
	boost::any getCurrentThreadID();
	bool setThreadPriority(const boost::any &threadID, TaskPriority priority);
	bool compareThreadsID(const boost::any &first, const boost::any &second);
	uint32 getCurrentProcessID();
	shared_ptr<PlatformManager::ProcessDetails> getProcessDetails(uint32 processID);	
	double tickCount();
	static void trace(const std::string &str, const char *file, size_t line, const char *function);
	String normalizePath(const String& path) const;
	bool contentTypeFromExtension(const String &extension, String &content_type);
	bool extensionFromContentType(const String &content_type, String &extension);
	bool execute(const String &command, const String &parameters);
	String getSystemCulture();

	String getEnv(const String &name);
	bool setEnv(const String &name, const String &value);

	String getLastError() const;

	void sleep(uint32 milliseconds);

	FILE * fileOpen(const String &filename, const String &mode);
	bool fileClose(FILE *handle);
	uint32 fileRead(FILE *handle, void *v, uint32 size);
	uint32 fileWrite(FILE *handle, const void *v, uint32 size);
	bool fileSeek(FILE *handle, uint64 offset, int32 origin);
	uint64 filePosition(FILE *handle);
	bool fileFlush(FILE *handle);
	bool fileEof(FILE *handle);
	bool fileStats(FILE *handle, boost::posix_time::ptime *timeCreation, boost::posix_time::ptime *timeLastModify = nullptr, boost::posix_time::ptime *timeLastAccess = nullptr);

	void debugBreak();

private:
	double hiResFrequency;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_PLATFORMWIN_H
