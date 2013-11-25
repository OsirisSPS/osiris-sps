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
#include "filesystem.h"

#include "algorithms.h"
#include "boost/filesystem/operations.hpp"
#include "log.h"
#include "path.h"
#include "platformmanager.h"
#include "unicode.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static boost::filesystem::wpath createPath(const String &path)
{
	return boost::filesystem::wpath(path.to_wide()/*, boost::filesystem::native*/);
}

//////////////////////////////////////////////////////////////////////

FileSystem::SpaceInfo::SpaceInfo()
{
	capacity = unknown;
	free = unknown;
	available = unknown;
	used = unknown;
};

const uint64 FileSystem::SpaceInfo::unknown = static_cast<uint64>(-1);

//////////////////////////////////////////////////////////////////////

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{

}

bool FileSystem::isDirectory(const String &path) const
{	
   	try
	{
		return boost::filesystem::is_directory(createPath(path));
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool FileSystem::isRelativePath(const String &path) const
{
	String standardized = utils::standardisePath(path, false);
    // TODO: da pulire
    return (standardized.starts_with(_S("./")) || (standardized.starts_with(_S("../"))));
}

bool FileSystem::exists(const String &path) const
{
    try
	{
		return boost::filesystem::exists(createPath(path));
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool FileSystem::fileExists(const String &filename) const
{
	return exists(filename) && (isDirectory(filename) == false);
}

bool FileSystem::directoryExists(const String &path) const
{
	bool c1 = exists(path);
	bool c2 = isDirectory(path);
	return exists(path) && isDirectory(path);
}

FileSystem::SpaceInfo FileSystem::getSpaceInfo() const
{
	return getSpaceInfo(_S("/"));
}

FileSystem::SpaceInfo FileSystem::getSpaceInfo(const String &path) const
{
	SpaceInfo info;
	try
	{
		boost::filesystem::space_info si = boost::filesystem::space(createPath(path));
		info.capacity = si.capacity;
		info.free = si.free;
		info.available = si.available;
		if(si.capacity != SpaceInfo::unknown && si.free != SpaceInfo::unknown)
			info.used = si.capacity - si.free;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return info;
}

String FileSystem::getFullPath(const String &relativePath) const
{
	try
	{
		return utils::standardisePath(boost::filesystem::system_complete(createPath(relativePath)).string());
	}
	catch(boost::system::system_error &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return relativePath;
}

String FileSystem::getExecutablePath()
{
	return getFilePath(m_executableName);
}

void FileSystem::initExecutableName(const String &arg0)
{
	//String exePath = utils::makeFilePath(PlatformManager::instance()->getCurrentWorkingPath(), arg0);	
	m_executableName = PlatformManager::instance()->normalizePath(boost::filesystem::system_complete(createPath(arg0)).string());
}

bool FileSystem::remove(const String &path)
{
	try
	{
		boost::filesystem::remove(createPath(path));
		return true;
	}
	catch(boost::system::system_error &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool FileSystem::rename(const String &oldName, const String &newName)
{
	try
	{
		boost::filesystem::rename(createPath(oldName), createPath(newName));
		return true;
	}
	catch(boost::system::system_error &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool FileSystem::createDirectory(const String &path)
{
	try
	{
		return boost::filesystem::create_directory(createPath(path));
	}
	catch(boost::system::system_error &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool FileSystem::removeDirectory(const String &path)
{
	try
	{
		return boost::filesystem::remove_all(createPath(path)) > 0;
	}
	catch(boost::system::system_error &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool FileSystem::ensureDirectory(const String &path)
{
	StringList directories;
	Path dir = path;
	while(dir.empty() == false)
	{
		directories.push_front(dir.path());
		dir = dir.branch_path();
	}

	for(StringList::const_iterator i = directories.begin(); i != directories.end(); ++i)
	{
		String path = *i;
		if(directoryExists(path) == false)
		{
			if(createDirectory(path) == false)
				return false;
		}
	}

	return true;
}

String FileSystem::getExtension(const String &filename, bool lower)
{
	size_t pos = filename.find_last_of(_S("."));
	if(pos == String::npos)
		return String::EMPTY;

	String extension = filename.mid(pos + 1);
	if(lower)
		extension.to_lower();

	return extension;
}

String FileSystem::getFileName(const String &path)
{
    String filename = path;
	filename.trim_right(_S("\\/"));
	size_t pos = filename.find_last_of(_S("\\/"));
	if(pos != String::npos)
		return filename.mid(pos + 1);

	return filename;
}

String FileSystem::getFileTitle(const String &path)
{
	String name = getFileName(path);

	size_t pos = name.find_last_of(_S("."));
	if(pos == String::npos)
		return name;
	else
		return name.mid(0, pos);
}

uint32 FileSystem::getFileSize(const String &path)
{
	try
	{
		return static_cast<uint32>(boost::filesystem::file_size(path.to_wide()));
	}
	catch(boost::system::system_error &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return 0;
}

String FileSystem::getFilePath(const String &fullPath)
{
	try
	{
		return createPath(fullPath).parent_path().string();
	}
	catch(boost::system::system_error &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return String::EMPTY;
}

String FileSystem::makeFilename(const String &name, const String &extension)
{
	if(name.empty())
	{
		OS_ASSERTFALSE();
		return String::EMPTY;
	}

	if(extension.empty())
		return name;

	return name + _S(".") + extension;
}

String FileSystem::createValidFilename(const String &str)
{
	return algorithms::erase_all_copy(str.to_wide(), "\\/:*?\"<>|");
}

bool FileSystem::getDirectories(const String &path, StringList &directories, bool fullpath)
{
	try
	{
		directories.clear();

		boost::filesystem::directory_iterator i(createPath(path));
		boost::filesystem::directory_iterator end;
		while(i != end)
		{
			if(boost::filesystem::is_directory(*i))
			{
				if(fullpath)
					directories.push_back(utils::standardisePath(i->path().string()));
				else
					directories.push_back(i->path().leaf().string());
			}

			++i;
		}

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool FileSystem::getFiles(const String &path, StringList &files, bool fullpath)
{
	try
	{
		files.clear();

		boost::filesystem::directory_iterator i(createPath(path));
		boost::filesystem::directory_iterator end;
		while(i != end)
		{
			if(boost::filesystem::is_regular_file(*i))
			{
				if(fullpath)
					files.push_back(utils::standardisePath(i->path().string(), false));
				else
					files.push_back(i->path().leaf().string());
			}

			++i;
		}

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool FileSystem::copyFile(const String &sourceFile, const String &targetFile)
{
	try
	{
		boost::filesystem::copy_file(sourceFile.to_wide(), targetFile.to_wide());
		return true;
	}
	catch(boost::system::system_error &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

bool FileSystem::moveFile(const String &sourceFile, const String &targetFile)
{
	// TODO: boost::filesystem non sembra supportare al momento una move file, per adesso
	// si effettua una copia e successivamente si rimuove il vecchio file

	if(copyFile(sourceFile, targetFile) == false)
		return false;

	return remove(sourceFile);
}

bool FileSystem::comparePaths(const String &first, const String &second)
{
	// URGENT: fixme
	
	return first == second;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
