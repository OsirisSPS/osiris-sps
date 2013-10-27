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

#ifndef _SQLITE_DRIVER_H
#define _SQLITE_DRIVER_H

#include "dbdb.h"
#include "idbdriver.h"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Thread;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

namespace sqlite {

//////////////////////////////////////////////////////////////////////

class Driver : public IDbDriver,
			   public enable_this_ptr<Driver>
{
	typedef IDbDriver DriverBase;

// Construction
public:
	Driver(const String &rootPath, shared_ptr<Thread> asyncThread);
	virtual ~Driver();

// Attributes
public:
	String getRootPath() const;
	String getDatabasePath(const String &database) const;

private:
	bool getOldRootPath(String &oldRootPath) const;

// Operations
private:
	void _createDatabase(const String &database);
	bool _removeDatabase(const String &database);
	bool _databaseExists(const String &database) const;

// IDriver interface
public:
	virtual std::string getName() const;
	virtual std::string getDescription() const;
	virtual shared_ptr<IDbOptions> createOptions();
	virtual shared_ptr<IDbConnection> createConnection(const std::string &database, shared_ptr<IDbOptions> options);
	virtual bool createDatabase(const std::string &name, shared_ptr<IDbOptions> options);
	virtual bool removeDatabase(const std::string &name, shared_ptr<IDbOptions> options);
	virtual bool databaseExists(const std::string &name, shared_ptr<IDbOptions> options) const;

public:
	static const uint32 BUSY_TIMEOUT;

public:
	static const std::string NAME;
	static const std::string DESCRIPTION;
	static const std::string DEFAULT_EXTENSION;

private:
	String m_rootPath;
	shared_ptr<Thread> m_asyncThread;
};

//////////////////////////////////////////////////////////////////////

} // sqlite

//////////////////////////////////////////////////////////////////////

#endif // _SQLITE_DRIVER_H
