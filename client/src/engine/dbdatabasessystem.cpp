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
#include "dbdatabasessystem.h"

#include "idbconnection.h"
#include "idbdriver.h"
#include "iportaldatabase.h"
#include "dbschemaschema.h"
#include "dbsqlactions.h"
#include "engine.h"
#include "filesystem.h"
#include "httpurl.h"
#include "isis.h"
#include "log.h"
#include "options.h"
#include "platformmanager.h"
#include "xmldocument.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SYSTEM(OS_NAMESPACE_NAME::DatabasesSystem)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
 
//////////////////////////////////////////////////////////////////////

DatabasesSystem::DatabasesSystem()
{

}

DatabasesSystem::~DatabasesSystem()
{

}

String DatabasesSystem::getDatabasePath(const String &type, const Version &version, bool allowDownload) const
{
	OS_ASSERT(type.empty() == false);

	String filename = type + _S("_") + version.toString() + _S(".xml");

	String filepath = utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(), OS_DB_PATH), filename);
	if(allowDownload && FileSystem::instance()->fileExists(filepath) == false)
	{
		std::string fileurl = Options::instance()->getIsisSecureLink((utils::makeFilePath(_S("/") + OS_ISIS_DIRECTORY_DB, filename)).to_ascii());

		shared_ptr<XMLDocument> xml(OS_NEW XMLDocument());

		shared_ptr<boost::asio::io_service> service = createAsioService();
		OS_EXCEPT_IF(xml->parseUrl(fileurl, Engine::getDefaultHttpUserAgent(), service, Engine::instance()->createTCPSocket(service, true, true)) == false, "Cannot download '" + fileurl + "'");
		OS_EXCEPT_IF(xml->writeFile(filepath) == false, "Cannot write '" + filepath.to_ascii() + "'");
	}

	return filepath;
}

shared_ptr<IDbDriver> DatabasesSystem::getDriver(const std::string &name) const
{
	for(list<shared_ptr<IDbDriver> >::type::const_iterator i = m_drivers.begin(); i != m_drivers.end(); ++i)
	{
		if(algorithms::compare_no_case((*i)->getName(), name))
			return *i;
	}

	return null;
}

shared_ptr<IDbDriver> DatabasesSystem::getDefaultDriver() const
{
	shared_ptr<IDbDriver> driver = getDriver(Options::instance()->getOption<String>(Options::db_options::default_driver).to_ascii());
	if(driver == null && m_drivers.empty() == false)
		driver = m_drivers.front();

	return driver;
}

bool DatabasesSystem::addDriver(shared_ptr<IDbDriver> driver)
{
	if(driver == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	m_drivers.push_back(driver);
	return true;
}

bool DatabasesSystem::createDatabase(shared_ptr<IDbDriver> driver, const String &database, shared_ptr<IDbOptions> options, shared_ptr<DbSchemaSchema> schema)
{
	bool done = false;
	shared_ptr<IDbConnection> connection;

	try
	{
		// Testa se il database specificato  gi esistente
		if(driver->databaseExists(database.to_ascii(), options))
			return false;

		// Crea il database
		if(driver->createDatabase(database.to_ascii(), options) == false)
			return false;

		// Apre una connessione al database
		connection = driver->createConnection(database.to_ascii(), options);
		connection->open();
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	if(connection != null)
	{
		// Carica lo schema specificato
		done = loadSchema(connection, schema);
	}

	if(done == false)
	{
		// Rimuove il database eventualmente creato
		driver->removeDatabase(database.to_ascii(), options);
	}

	return done;
}

bool DatabasesSystem::loadSchema(shared_ptr<IDbConnection> connection, shared_ptr<DbSchemaSchema> schema)
{
	OS_ASSERT(connection != null);
	OS_ASSERT(schema != null);

	try
	{
		// Enumera le tabelle del db
		const DbSchemaTables &tables = schema->getTables();
		for(DbSchemaTables::const_iterator i = tables.begin(); i != tables.end(); ++i)
		{
			// Crea la tabella corrente
			shared_ptr<DbSqlICommand> createTableCommand(OS_NEW DbSqlCreateTable(*i));

			/*
			String sql;
			connection->parse(createTableCommand, sql);
			*/
			connection->execute(createTableCommand);
		}

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

String DatabasesSystem::getName() const
{
	return _S("database");
}

bool DatabasesSystem::start(bool recovery)
{
	shared_ptr<IDbDriver> driver = getDefaultDriver();
	if(driver == null)
	{
		OS_LOG_ERROR(_S("Need a default database driver"));
		return false;
	}

	shared_ptr<IDbOptions> options = driver->createOptions();
	// Controlla se il database di sistema non esiste
	if(driver->databaseExists(OS_SYSTEM_DB_NAME.to_ascii(), options) == false)
	{
		shared_ptr<DbSchemaSchema> schema(OS_NEW DbSchemaSchema());
		if(schema->parse(getDatabasePath(OS_SYSTEM_DB_TYPE, OS_SYSTEM_DB_VERSION, false)) == false)
		{
			OS_LOG_ERROR(_S("Cannot parse system's database schema"));
			return false;
		}

		// Crea il database di sistema
		if(createDatabase(driver, OS_SYSTEM_DB_NAME, options, schema) == false)
			return false;
	}

	return true;
}

void DatabasesSystem::stop()
{

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
