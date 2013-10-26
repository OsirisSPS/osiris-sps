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

#ifndef _DB_DATABASESSYSTEM_H
#define _DB_DATABASESSYSTEM_H

#include "dbdb.h"
#include "objectsobjects.h"
#include "isystem.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

#define OS_SYSTEM_DB_VERSION			OS_MAKEVERSION(0, 1)
#define OS_SYSTEM_DB_TYPE				_S("system")
#define OS_SYSTEM_DB_NAME				_S("system")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IDbConnection;
class IDbDriver;
class IDbOptions;
class Version;

//////////////////////////////////////////////////////////////////////

class EngineExport DatabasesSystem : public DynamicSingleton<DatabasesSystem>,
									 public ISystem
{
// Construction
public:
	DatabasesSystem();
	virtual ~DatabasesSystem();

// Attributes
public:
	inline const list<shared_ptr<IDbDriver> >::type & getDrivers() const;
	inline bool hasDriver(const std::string &name) const;

	shared_ptr<IDbDriver> getDriver(const std::string &name) const;
	shared_ptr<IDbDriver> getDefaultDriver() const;

	// Restituisce il path dello schema del database della versione indicata
	String getDatabasePath(const String &type, const Version &version, bool allowDownload) const;

// Operations
public:
	bool addDriver(shared_ptr<IDbDriver> driver);
	bool createDatabase(shared_ptr<IDbDriver> driver, const String &database, shared_ptr<IDbOptions> options, shared_ptr<DbSchemaSchema> schema);
	bool loadSchema(shared_ptr<IDbConnection> connection, shared_ptr<DbSchemaSchema> schema);

// ISystem interface
public:
	virtual String getName() const;

	virtual bool start(bool recovery);
	virtual void stop();

protected:
	list<shared_ptr<IDbDriver> >::type m_drivers;
};

//////////////////////////////////////////////////////////////////////

inline const list<shared_ptr<IDbDriver> >::type & DatabasesSystem::getDrivers() const { return m_drivers; }
inline bool DatabasesSystem::hasDriver(const std::string &name) const { return getDriver(name) != null; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_DATABASESSYSTEM_H
