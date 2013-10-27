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

#ifndef _DB_SQL_SELECT_H
#define _DB_SQL_SELECT_H

#include "dbsqlcollection.h"
#include "dbsqlfield.h"
#include "dbsqlgroupby.h"
#include "dbsqlicommand.h"
#include "dbsqllimit.h"
#include "dbsqlorderby.h"
#include "dbsqltable.h"
#include "dbsqlwhere.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DatabaseExport DbSqlSelect : public DbSqlICommand
{
	typedef DbSqlICommand CommandBase;

// Enumerated types
public:
	enum ConnectionType		// Relazione di dipendenza per le select composte
	{
		ctNone			= 0,
		ctUnion			= 1,
		ctUnionAll		= 2,
		ctIntersect		= 3,
		ctExcept		= 4,
	};

// Construction
public:
	DbSqlSelect(const String &table = String::EMPTY);
	virtual ~DbSqlSelect();

// Attributes
public:
	inline ConnectionType getConnectionType() const;
	inline void setConnectionType(ConnectionType type);
	inline shared_ptr<DbSqlSelect> getConnection() const;
	inline bool isCompound() const;

// Operations
public:
	void createConnection(shared_ptr<DbSqlSelect> select, ConnectionType type);

private:
	void _init();

// IStatement interface
public:
	virtual void reset();

public:
	bool distinct;
	bool count;

	DbSqlWhere where;
	DbSqlGroupBy groupBy;
	DbSqlOrderBy orderBy;

	DbSqlFields fields;
	DbSqlTables tables;

	DbSqlLimit limit;

protected:
	ConnectionType m_connectionType;
	shared_ptr<DbSqlSelect> m_connection;
};

//////////////////////////////////////////////////////////////////////

inline DbSqlSelect::ConnectionType DbSqlSelect::getConnectionType() const { return m_connectionType; }
inline void DbSqlSelect::setConnectionType(ConnectionType type) { m_connectionType = type; }
inline shared_ptr<DbSqlSelect> DbSqlSelect::getConnection() const { return m_connection; }
inline bool DbSqlSelect::isCompound() const { return m_connection != null; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_SQL_SELECT_H
