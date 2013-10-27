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

#ifndef _MYSQL_DRIVER_H
#define _MYSQL_DRIVER_H

#include "dbdb.h"
#include "dbidriver.h"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

namespace mysql {

//////////////////////////////////////////////////////////////////////

class Driver : public IDatabaseDriver,
			   public enable_this_ptr<Driver>
{
	typedef IDatabaseDriver DriverBase;

// Construction
public:
	Driver(const String &rootPath);
	virtual ~Driver();

// Attributes
public:
	String getRelativeFolder(const String &path);
	String getDataFolder();
	String getShareFolder();

// Operations
private:
	bool _createDatabase(const String &database, shared_ptr<IDatabaseOptions> options);
	bool _removeDatabase(const String &database, shared_ptr<IDatabaseOptions> options);
	bool _databaseExists(const String &database, shared_ptr<IDatabaseOptions> options) const;

// IDriver interface
public:
	virtual std::string getName() const;
	virtual std::string getDescription() const;
	virtual shared_ptr<IDatabaseOptions> createOptions();
	virtual shared_ptr<IDatabaseConnection> createConnection(const std::string &database, shared_ptr<IDatabaseOptions> options);
	virtual bool createDatabase(const std::string &name, shared_ptr<IDatabaseOptions> options);
	virtual bool removeDatabase(const std::string &name, shared_ptr<IDatabaseOptions> options);
	virtual bool databaseExists(const std::string &name, shared_ptr<IDatabaseOptions> options) const;

public:
	static const std::string NAME;
	static const std::string DESCRIPTION;

private:
	String m_rootPath;
};

//////////////////////////////////////////////////////////////////////

} // mysql

//////////////////////////////////////////////////////////////////////

#endif // _MYSQL_DRIVER_H
