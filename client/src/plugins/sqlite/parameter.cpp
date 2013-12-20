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
#include "parameter.h"

#include "boost/format.hpp"
#include "sqliteutils.h"

//////////////////////////////////////////////////////////////////////

Parameter::Parameter(sqlite3_stmt *statement, uint32 index)
{
	OS_ASSERT(statement != nullptr);
	m_statement = statement;

	m_index = index;
}

Parameter::~Parameter()
{

}

void Parameter::setBool(bool value)
{
	// Sqlite non supporta il tipo bool, pertanto viene trattato come int32
	setInt32(value ? 1 : 0);
}

void Parameter::setByte(byte value)
{
	// Sqlite non supporta il tipo byte, pertanto viene trattato come int32
	setInt32(value);
}

void Parameter::setInt32(int32 value)
{
	_verify(sqlite3_bind_int(m_statement, m_index, value));
}

void Parameter::setUint32(uint32 value)
{
	_verify(sqlite3_bind_int(m_statement, m_index, value));
}

void Parameter::setDouble(double value)
{
	_verify(sqlite3_bind_double(m_statement, m_index, value));
}

void Parameter::setString(const String &value)
{
	_verify(sqlite3_bind_text16(m_statement, m_index, value.buffer(), static_cast<int32>(value.buffer_size()), SQLITE_TRANSIENT));
}

void Parameter::setBlob(const void *data, uint32 size)
{
	_verify(sqlite3_bind_blob(m_statement, m_index, data, size, SQLITE_TRANSIENT));
}

void Parameter::setNull()
{
	_verify(sqlite3_bind_null(m_statement, m_index));
}

void Parameter::_verify(int32 result)
{
	OS_EXCEPT_IF(result != SQLITE_OK, (boost::format("Cannot bind param %d, details: %s") % m_index % sqlite_error_code(result)).str());
}
