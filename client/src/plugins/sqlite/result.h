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

#ifndef _SQLITE_RESULT_H
#define _SQLITE_RESULT_H

#include "boost/thread/recursive_mutex.hpp"
#include "idbresult.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

namespace sqlite {

//////////////////////////////////////////////////////////////////////

class Result : public IDbResult
{
	typedef IDbResult ResultBase;

// Construction
public:
	Result(sqlite3 *connection, boost::recursive_mutex &cs);
	virtual ~Result();

// Operations
public:
	void execute(const String &sql);
	void executeNext();

	void prepare(const String &sql);

private:
	void _prepare(const String &sql, bool finalize);
	sqlite_int _step();
	sqlite_int _finalize();

// IResult interface
public:
	virtual uint32 columns();
	virtual String columnName(uint32 index);

	virtual bool end();

	virtual void moveNext();

	virtual void get(uint32 index, DataItem &value);

protected:
	boost::recursive_mutex::scoped_lock m_lock;
	bool m_end;
	sqlite3 *m_connection;
	sqlite3_stmt *m_statement;
};

//////////////////////////////////////////////////////////////////////

} // sqlite

//////////////////////////////////////////////////////////////////////

#endif // _SQLITE_RESULT_H
