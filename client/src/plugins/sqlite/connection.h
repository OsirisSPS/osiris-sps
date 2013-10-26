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

#ifndef _SQLITE_CONNECTION_H
#define _SQLITE_CONNECTION_H

#include "boost/thread/recursive_mutex.hpp"
#include "idbconnection.h"
#include "osiriscommon.h"

//#define OS_ENABLE_DATABASE_ANALYZE // VERYURGENT

//////////////////////////////////////////////////////////////////////

namespace sqlite {

//////////////////////////////////////////////////////////////////////

class Connection : public IDbConnection
{
	typedef IDbConnection ConnectionBase;

// Construction
public:
	Connection(const String &database, shared_ptr<IDbOptions> options, shared_ptr<IDbDriver> driver);
	virtual ~Connection();

private:
	String _getPath() const;

// Operations
public:
	void connect(const String &filename);

private:
	void _check(sqlite3 *connection);
#ifdef OS_ENABLE_DATABASE_ANALYZE
	String preAnalyze(const String &sql);
	void postAnalyze(const String &sql);
#endif
	uint32 doExecute(sqlite3 *connection, const String &sql);
	void doQuery(sqlite3 *connection, const String &sql, DataTable &table);
	shared_ptr<Result> doQuery(sqlite3 *connection, const String &sql);

// IConnection interface
public:
	virtual void open();
	virtual void close();
	virtual void beginTransaction();
	virtual void commit();
	virtual void rollback();
	virtual uint32 _execute(const String &sql);
	virtual shared_ptr<IDbStatement> _prepare(const String &sql);
	virtual shared_ptr<IDbResult> _query(const String &sql);
	virtual void _parse(shared_ptr<DbSqlICommand> command, String &sql);
	virtual void analyze();
	virtual void reindex();
	virtual void vacuum();

protected:
	boost::recursive_mutex m_cs;
    sqlite3 *m_connection;
	String m_filename;
	bool m_showSql;
#ifdef OS_ENABLE_DATABASE_ANALYZE
	uint32 m_transactionId;
#endif

public:
	static const String NAME;
};

//////////////////////////////////////////////////////////////////////

} // sqlite

//////////////////////////////////////////////////////////////////////

#endif // _SQLITE_CONNECTION_H
