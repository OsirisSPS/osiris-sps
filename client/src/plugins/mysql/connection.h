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

#ifndef _MYSQL_CONNECTION_H
#define _MYSQL_CONNECTION_H

#include "boost/thread/recursive_mutex.hpp"
#include "dbiconnection.h"

//////////////////////////////////////////////////////////////////////

namespace mysql {

//////////////////////////////////////////////////////////////////////

class Connection : public IDatabaseConnection
{
	typedef IDatabaseConnection ConnectionBase;

// Construction
public:
	Connection(const String &database, shared_ptr<IDatabaseOptions> options, shared_ptr<IDatabaseDriver> driver);
	virtual ~Connection();

// Attributes
public:
	inline MYSQL * mysql() const;

	std::string error() const;

	unsigned int errorCode() const;
	bool ok() const;

	bool databaseExists(const String &name) const;

// Operations
public:
	void connect(const String &name, const String &host = String::EMPTY, const String &user = String::EMPTY, const String &password = String::EMPTY);

private:
	void _checkConnection();

// IConnection interface
public:
	virtual void open();
	virtual void close();
	virtual void beginTransaction();
	virtual void commit();
	virtual void rollback();
	virtual uint32 execute(const String &sql);
	virtual shared_ptr<IDatabaseStatement> prepare(const String &sql);
	virtual shared_ptr<IDatabaseResult> query(const String &sql);
	virtual void parse(sql::command_ptr command, String &sql);
	virtual void analyze();
	virtual void reindex();
	virtual void vacuum();

// IConnection overrides
public:
	virtual String escape(const String &name) const;

protected:
	mutable boost::recursive_mutex m_cs;
	MYSQL *m_connection;
};

//////////////////////////////////////////////////////////////////////

inline MYSQL * Connection::mysql() const { return m_connection; }

//////////////////////////////////////////////////////////////////////

} // mysql

//////////////////////////////////////////////////////////////////////

#endif // _MYSQL_CONNECTION_H
