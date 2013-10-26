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

#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

Parameter::Parameter(MYSQL_BIND *binder)
{
	OS_ASSERT(binder != null);
	m_binder = binder;
}

Parameter::~Parameter()
{

}

void Parameter::setBool(bool value)
{
	// Per compatibilit con la versione 4 (la versione 5 supporta MYSQL_TYPE_BIT) il tipo bool viene trattato come byte
	setByte(value ? 1 : 0);
}

void Parameter::setByte(byte value)
{
	_setValue(MYSQL_TYPE_TINY, &value, sizeof(byte), false, true);
}

void Parameter::setInt32(int32 value)
{
	_setValue(MYSQL_TYPE_LONG, &value, sizeof(int32), false, false);
}

void Parameter::setUint32(uint32 value)
{
	_setValue(MYSQL_TYPE_LONG, &value, sizeof(uint32), false, true);
}

void Parameter::setDouble(double value)
{
	_setValue(MYSQL_TYPE_DOUBLE, &value, sizeof(double), false, false);
}

void Parameter::setString(const String &value)
{
	std::string str = value.to_utf8();
	_setValue(MYSQL_TYPE_STRING, str.data(), static_cast<uint32>(str.size()), false, false);
}

void Parameter::setBlob(const void *data, uint32 size)
{
	_setValue(MYSQL_TYPE_BLOB, data, size, false, false);
}

void Parameter::setNull()
{
	_setValue(MYSQL_TYPE_NULL, null, 0, true, false);
}

void Parameter::_setValue(enum_field_types type, const void *data, uint32 size, bool is_null,  bool is_unsigned)
{
	// Aggiorna il buffer al nuovo valore
	m_buffer.assign(data, size);

	m_binder->buffer_type = type;
	m_binder->buffer = m_buffer.getData();
	m_binder->buffer_length = m_buffer.getSize();
	m_binder->length = &m_binder->buffer_length;
	m_binder->is_null_value = is_null;
	m_binder->is_null = &m_binder->is_null_value;
	m_binder->is_unsigned = is_unsigned;
}
