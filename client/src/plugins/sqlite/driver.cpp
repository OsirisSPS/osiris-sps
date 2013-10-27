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
#include "driver.h"

#include "connection.h"
#include "convert.h"
#include "file.h"
#include "filesystem.h"
#include "log.h"
#include "options.h"
#include "osiriscommon.h"
#include "parser.h"
#include "platformmanager.h"
#include "plugin.h"
#include "pluginsmanager.h"
#include "sqliteoptions.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////

const uint32 Driver::BUSY_TIMEOUT = 100;

const std::string Driver::NAME = "sqlite";
const std::string Driver::DESCRIPTION = "Sqlite";
const std::string Driver::DEFAULT_EXTENSION = "db";

//////////////////////////////////////////////////////////////////////

Driver::Driver(const String &rootPath, shared_ptr<Thread> asyncThread) : m_rootPath(rootPath),
																	     m_asyncThread(asyncThread)
{
	OS_ASSERT(m_rootPath.empty() == false);
	OS_ASSERT(asyncThread == null || asyncThread->running());

	// Fix di compatibilità per le directory con il vecchio nome 
	// Compatibility - TOCLEAN
	String oldRootPath;
	if(getOldRootPath(oldRootPath))
	{
		OS_ASSERT(FileSystem::instance()->exists(oldRootPath));

		if(FileSystem::instance()->rename(oldRootPath, m_rootPath) == false)
			OS_LOG_CRITICAL(String::format(_S("Cannot rename directory '%S' to '%S', please fix it manually").c_str(), oldRootPath.c_str(), m_rootPath.c_str()));
	}	
}

Driver::~Driver()
{
	if(m_asyncThread != null)
	{
		m_asyncThread->stop();
		m_asyncThread.reset();
	}
}

String Driver::getRootPath() const
{
	return m_rootPath;
}

String Driver::getDatabasePath(const String &database) const
{
	String filepath = utils::makeFilePath(getRootPath(), database);
	if(FileSystem::instance()->getExtension(database) != DEFAULT_EXTENSION)
		filepath += _S(".") + DEFAULT_EXTENSION;

	return filepath;
}

bool Driver::getOldRootPath(String &oldRootPath) const
{
	oldRootPath.clear();

	String oldRootPath1 = utils::makeFolderPath(OS_NAMESPACE_NAME::Options::instance()->getDataPath(), utils::makeFolderPath(OS_PLUGINS_PATH, "sqlite"));
	if(FileSystem::instance()->exists(oldRootPath1))
	{
		oldRootPath = oldRootPath1;
		return true;
	}

	String oldRootPath2 = utils::makeFolderPath(OS_NAMESPACE_NAME::Options::instance()->getDataPath(), utils::makeFolderPath(OS_PLUGINS_PATH, "00000006532E9E0A68EB22E08240965CFA9366DFA6A26A62"));
	if(FileSystem::instance()->exists(oldRootPath2))
	{
		oldRootPath = oldRootPath2;
		return true;
	}

	return false;
}

void Driver::_createDatabase(const String &database)
{
	FileSystem::instance()->ensureDirectory(getRootPath());

	OS_EXCEPT_IF(_databaseExists(database), "'" + database.to_ascii() + "' already exists");

	String pathname = getDatabasePath(database);

	File file;
	if(file.open(pathname, File::ofWrite) == false)
		OS_EXCEPTION("Cannot create file '" + pathname.to_ascii() + "'");
}

bool Driver::_removeDatabase(const String &database)
{
	return FileSystem::instance()->remove(getDatabasePath(database));
}

bool Driver::_databaseExists(const String &database) const
{
	return FileSystem::instance()->exists(getDatabasePath(database));
}

std::string Driver::getName() const
{
	return NAME;
}

std::string Driver::getDescription() const
{
	return DESCRIPTION;
}

shared_ptr<IDbOptions> Driver::createOptions()
{
	return shared_ptr<IDbOptions>(OS_NEW Options());
}

shared_ptr<IDbConnection> Driver::createConnection(const std::string &database, shared_ptr<IDbOptions> options)
{
	// 23/12/2008
	if(databaseExists(database, options) == false)
		return null;
	
	return shared_ptr<IDbConnection>(OS_NEW Connection(database, options, get_this_ptr()));
}

bool Driver::createDatabase(const std::string &name, shared_ptr<IDbOptions> options)
{
	try
	{
		_createDatabase(name);
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return false;
	}

	return true;
}

bool Driver::removeDatabase(const std::string &name, shared_ptr<IDbOptions> options)
{
	return _removeDatabase(name);
}

bool Driver::databaseExists(const std::string &name, shared_ptr<IDbOptions> options) const
{
	return _databaseExists(name);
}
