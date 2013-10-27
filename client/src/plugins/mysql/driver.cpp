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
#include "dbschemaschema.h"
#include "filesystem.h"
#include "log.h"
#include "mysqloptions.h"
#include "options.h"
#include "parser.h"
#include "platformmanager.h"
#include "pluginsmanager.h"

//////////////////////////////////////////////////////////////////////

const std::string Driver::NAME("mysql");
const std::string Driver::DESCRIPTION("Mysql");

//////////////////////////////////////////////////////////////////////

Driver::Driver(const String &rootPath) : m_rootPath(rootPath)
{

}

Driver::~Driver()
{

}

String Driver::getRelativeFolder(const String &path)
{
	return utils::makeFolderPath(m_rootPath, path);
}

String Driver::getDataFolder()
{
	return getRelativeFolder(OS_MYSQL_DATA);
}

String Driver::getShareFolder()
{
	return getRelativeFolder(OS_MYSQL_SHARE);
}

bool Driver::_createDatabase(const String &database, shared_ptr<IDatabaseOptions> options)
{
	shared_ptr<Connection> connection(OS_NEW Connection(String::EMPTY, options, get_this_ptr()));
	connection->open();		
	
	connection->execute(String::format(_S("create database %S default character set utf8 collate utf8_general_ci").c_str(), database.c_str()));		
	return connection->ok();
}

bool Driver::_removeDatabase(const String &database, shared_ptr<IDatabaseOptions> options)
{
	shared_ptr<Connection> connection(OS_NEW Connection(String::EMPTY, options, get_this_ptr()));
	connection->open();		
	
	connection->execute(String::format(_S("drop database %S").c_str(), database.c_str()));		
	return connection->ok();
}

bool Driver::_databaseExists(const String &database, shared_ptr<IDatabaseOptions> options) const
{
	shared_ptr<Connection> connection(OS_NEW Connection(String::EMPTY, options, const_cast<Driver *>(this)->get_this_ptr()));
	connection->open();		
	
	return connection->databaseExists(database);
}

std::string Driver::getName() const
{
	return NAME;
}

std::string Driver::getDescription() const
{
	return DESCRIPTION;
}

shared_ptr<IDatabaseOptions> Driver::createOptions()
{
	return shared_ptr<IDatabaseOptions>(OS_NEW Options());
}

shared_ptr<IDatabaseConnection> Driver::createConnection(const std::string &database, shared_ptr<IDatabaseOptions> options)
{
	return shared_ptr<IDatabaseConnection>(OS_NEW Connection(database, options, get_this_ptr()));
}

bool Driver::createDatabase(const std::string &name, shared_ptr<IDatabaseOptions> options)
{
	try
	{
		_createDatabase(name, options);		
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());	
		return false;	
	}

	return true;
}

bool Driver::removeDatabase(const std::string &name, shared_ptr<IDatabaseOptions> options)
{
	return _removeDatabase(name, options);
}

bool Driver::databaseExists(const std::string &name, shared_ptr<IDatabaseOptions> options) const
{
	return _databaseExists(name, options);
}
