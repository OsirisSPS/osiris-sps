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
#include "result.h"

#include "boost/format.hpp"
#include "driver.h"
#include "buffer.h"
#include "dataitem.h"
#include "platformmanager.h"
#include "realtimestats.h"
#include "sqliteutils.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

Result::Result(sqlite3 *connection, boost::recursive_mutex &cs) : m_lock(cs)
{
	OS_ASSERT(connection != nullptr);
	m_connection = connection;

	m_end = true;
	m_statement = nullptr;

	// Nota: non richiamare nessuna funzione che potrebbe potenzialmente lanciare delle eccezioni dal costruttore
}

Result::~Result()
{
	// Finalizza l'sql corrente
	_finalize();

	m_connection = nullptr;
}

void Result::execute(const String &sql)
{
	// Finalizza un'eventuale sql corrente
	_finalize();

	// Prepara il nuovo sql
	_prepare(sql, true);
}

void Result::prepare(const String &sql)
{
	// Finalizza un'eventuale sql corrente
	_finalize();

	// Prepara il nuovo sql
	_prepare(sql, false);
}

void Result::_prepare(const String &sql, bool finalize)
{
	OS_ASSERT(m_statement == nullptr);

	const void *buffer = sql.buffer();
	size_t bytes = sql.buffer_size();

	while(bytes > 0)
	{
		RealtimeStatsScopeTimer RSS(_S("Activity"), _S("Database - Activity"));

		const void *reminder = nullptr;
		int32 res = safe_sqlite_prepare(m_connection, buffer, static_cast<int32>(bytes), &m_statement, &reminder);
		OS_EXCEPT_IF(res != SQLITE_OK, sqlite_last_error(m_connection, res));
		
		RSS.stop();

		if(reminder != nullptr)
		{
			size_t offset = std::distance(static_cast<const byte *>(buffer), static_cast<const byte *>(reminder));
			OS_ASSERT(offset > 0 && offset <= bytes);

			bytes -= offset;
			buffer = reminder;					
		}
		else
		{
			bytes = 0;
			buffer = nullptr;			
		}

		_step();

		if(finalize)
			_finalize();
	}
}

sqlite_int Result::_step()
{	
	if(m_statement == nullptr)
		return SQLITE_OK;

	RealtimeStatsScopeTimer RSS(_S("Activity"), _S("Database - Activity"));

	sqlite_int res = safe_sqlite_step(m_statement);
	m_end = res != SQLITE_ROW;
	OS_EXCEPT_IF(res != SQLITE_OK && res != SQLITE_ROW && res != SQLITE_DONE, sqlite_last_error(m_connection, res));
	return res;
}

sqlite_int Result::_finalize()
{
	if(m_statement == nullptr)
		return SQLITE_OK;

	RealtimeStatsScopeTimer RSS(_S("Activity"), _S("Database - Activity"));

	sqlite_int res = safe_sqlite_finalize(m_statement);

	m_statement = nullptr;
	m_end = true;

	// _finalize viene chiamata nel distruttore pertanto non deve lanciare un'eccezione in caso di errore
	if(res != SQLITE_OK)
	{
		OS_TRACE(sqlite_last_error(m_connection, res));
		OS_ASSERTFALSE();
	}

	return res;
}

uint32 Result::columns()
{
	return static_cast<uint32>(sqlite3_column_count(m_statement));
}

String Result::columnName(uint32 index)
{
	return utils::safeString(_tsqlite3_column_name(m_statement, index));
}

bool Result::end()
{
	return m_end;
}

void Result::moveNext()
{
	OS_ASSERT(end() == false);
	if(end() == false)
		_step();
}

void Result::get(uint32 index, DataItem &value)
{
	int32 type = sqlite3_column_type(m_statement, index);
	switch(type)
	{
	case SQLITE_INTEGER:	value = static_cast<int32>(sqlite3_column_int(m_statement, index));
							break;

	case SQLITE_FLOAT:		value = sqlite3_column_double(m_statement, index);
							break;

	case SQLITE_TEXT:		value = _tsqlite3_column_text(m_statement, index);							
							break;

	case SQLITE_BLOB:		{
								uint32 size = sqlite3_column_bytes(m_statement, index);
								Buffer buffer(size);
								buffer.write(sqlite3_column_blob(m_statement, index), size);
								value = buffer;
							}

							break;

	case SQLITE_NULL:		value = DataItem::EMPTY;
							break;

	default:				OS_EXCEPTION((boost::format("unknown value type '%d'") % type).str());
							break;
	}
}
