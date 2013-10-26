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

#ifndef _OS_CORE_FILESYSTEM_H
#define _OS_CORE_FILESYSTEM_H

#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport FileSystem : public StaticSingleton<FileSystem>
{
// Internal
public:
	struct CoreExport SpaceInfo
    {
		SpaceInfo();

		uint64 capacity;
		uint64 free;
		uint64 available;
		uint64 used;

		static const uint64 unknown;
    };

// Construction
protected:
	FileSystem();

public:
	virtual ~FileSystem();

// Attributes
public:
	bool isDirectory(const String &path) const;
	bool isRelativePath(const String &path) const;

	bool exists(const String &path) const;
	bool fileExists(const String &filename) const;
	bool directoryExists(const String &path) const;

	SpaceInfo getSpaceInfo() const;
	SpaceInfo getSpaceInfo(const String &path) const;

	String getFullPath(const String &relativePath) const;

	String getExecutablePath();
	inline String getExecutableName();

protected:
	inline static bool isDots(const String &name);

// Operations
public:
	void initExecutableName(const String &arg0);

	bool remove(const String &path);
	bool rename(const String &oldName, const String &newName);

	bool createDirectory(const String &path);
	bool removeDirectory(const String &path);
	bool ensureDirectory(const String &path);

	String getExtension(const String &path, bool lower = true);

	String getFileName(const String &path);
	String getFileTitle(const String &path);
	uint32 getFileSize(const String &path);
	String getFilePath(const String &fullPath);

	String makeFilename(const String &name, const String &extension);
	String createValidFilename(const String &str);

	bool getDirectories(const String &path, StringList &directories, bool fullpath);
	bool getFiles(const String &path, StringList &files, bool fullpath);

	bool copyFile(const String &sourceFile, const String &targetFile);
	bool moveFile(const String &sourceFile, const String &targetFile);

	// Restituisce true se i due paths sono identici
	bool comparePaths(const String &first, const String &second);

private:
	String m_executableName;
};

//////////////////////////////////////////////////////////////////////

inline bool FileSystem::isDots(const String &name) { return name == _S(".") || name == _S(".."); }
inline String FileSystem::getExecutableName() { return m_executableName; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_FILESYSTEM_H
