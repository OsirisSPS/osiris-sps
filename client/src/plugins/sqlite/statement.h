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

#ifndef _SQLITE_STATEMENT_H
#define _SQLITE_STATEMENT_H

#include "boost/thread/recursive_mutex.hpp"
#include "idbstatement.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

namespace sqlite {

//////////////////////////////////////////////////////////////////////

class Statement : public IDbStatement
{
// Construction
public:
	Statement(boost::recursive_mutex &cs);
	virtual ~Statement();

// Operations
public:
	void prepare(sqlite3 *connection, const String &sql);

// IStatement interface
public:
	virtual uint32 neededParameters();
	virtual void close();

protected:
	virtual shared_ptr<IDbParameter> createParameter(uint32 index);
	virtual void run();

private:
	sqlite3 *m_connection;
	sqlite3_stmt *m_statement;
	boost::recursive_mutex::scoped_lock m_lock;
};

//////////////////////////////////////////////////////////////////////

} // sqlite

//////////////////////////////////////////////////////////////////////

#endif // _SQLITE_STATEMENT_H
