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

#include "connection.h"
#include "buffer.h"
#include "dataitem.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

Result::Result(boost::recursive_mutex &cs) : m_lock(cs)
{
	m_result = nullptr;
	m_row = nullptr;
	m_lengths = nullptr;
}

Result::~Result()
{
	if(m_result != nullptr)
	{
        mysql_free_result(m_result);
		m_result = nullptr;
	}
}

void Result::prepare(Connection *connection)
{
	MYSQL_RES *result = mysql_use_result(connection->mysql());
	OS_EXCEPT_IF(result == nullptr, connection->error());

	prepare(result);
}

void Result::prepare(MYSQL_RES *result)
{
	OS_EXCEPT_IF(result == nullptr, "Invalid result");

	m_result = result;

	int32 columns = mysql_num_fields(m_result);
	for(int32 i = 0; i < columns; i++)
	{
		mysql_field_seek(m_result, i);

		MYSQL_FIELD *field = mysql_fetch_field(m_result);
		OS_EXCEPT_IF(field == nullptr, "Mysql internal error");

		// Salva il nome del campo
		m_fieldNames.push_back(field->name);
		// Salva la tipologia del campo  il flag indicante il segno (nel caso sia un campo numerico)
		m_fieldTypes.push_back(field_type(field->type, field->flags));
	}

	moveNext();
}

uint32 Result::columns()
{
	return static_cast<uint32>(m_fieldNames.size());
}

String Result::columnName(uint32 index)
{
	if(index < columns())
		return m_fieldNames[index];

	return String::EMPTY;
}

bool Result::end()
{
	return m_row == nullptr;
}

void Result::moveNext()
{
	// Si posiziona sulla riga successiva
	m_row = mysql_fetch_row(m_result);
	// Se la riga corrente  valida carica la dimensione dei campi
	m_lengths = m_row != nullptr ? mysql_fetch_lengths(m_result) : null;
}

void Result::get(uint32 index, DataItem &value)
{
	OS_ASSERT(m_row != nullptr);
	OS_ASSERT(index < columns());

	char *data = m_row[index];
	if(data == nullptr)
	{
		value = DataItem::EMPTY;
	}
	else
	{
		uint32 length = static_cast<uint32>(m_lengths[index]);
		const field_type &field = m_fieldTypes[index];

		// Seleziona il tipo di campo
		switch(field.first)
		{
		case MYSQL_TYPE_NULL:			value = DataItem::EMPTY;
										break;

		case MYSQL_TYPE_LONG:
		case MYSQL_TYPE_LONGLONG:
		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_SHORT:
		case MYSQL_TYPE_TINY:
										{
											bool is_unsigned = (field.second & UNSIGNED_FLAG) == UNSIGNED_FLAG;

											// IMPORTANTE: evitare "value = field.second ? _toUint32(data) : _toInt32(data)" perch le due funzioni nonostante
											// restituiscano tipi diversi vengono castate dal ad un unico tipo (in questo caso uint32) assegnabile al DataItem!!!

											// Controlla se il valore numerico  senza segno
											if(is_unsigned)
												value = _toUint32(data);
											else
												value = _toInt32(data);
										}

										break;

		case MYSQL_TYPE_DECIMAL:
		case MYSQL_TYPE_DOUBLE:
		case MYSQL_TYPE_FLOAT:
		case MYSQL_TYPE_NEWDECIMAL:
										value = _toDouble(data);
										break;

		case MYSQL_TYPE_BLOB:
		case MYSQL_TYPE_MEDIUM_BLOB:
		case MYSQL_TYPE_LONG_BLOB:
		case MYSQL_TYPE_TINY_BLOB:
										{
											// Mysql tratta i campi "text" come campi blog, per distinguerli bisogna testare il flag BINARY_FLAG

											bool is_binary = (field.second & BINARY_FLAG) == BINARY_FLAG;

											if(is_binary)
                                                value = _toBuffer(data, length);
											else
												value = _toString(data, length);
										}

										break;

		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_VARCHAR:
		case MYSQL_TYPE_VAR_STRING:
										value = _toString(data, length);
										break;

		case MYSQL_TYPE_TIMESTAMP:
		case MYSQL_TYPE_DATE:
		case MYSQL_TYPE_TIME:
		case MYSQL_TYPE_DATETIME:
		case MYSQL_TYPE_YEAR:
		case MYSQL_TYPE_NEWDATE:
		case MYSQL_TYPE_BIT:
		case MYSQL_TYPE_ENUM:
		case MYSQL_TYPE_SET:
		case MYSQL_TYPE_GEOMETRY:
										OS_EXCEPTION("Unsupported mysql type");
										break;

		default:						OS_EXCEPTION("Unknown mysql type");
										break;
		}
	}
}

int32 Result::_toInt32(char *data)
{
	return static_cast<int32>(atoi(data));
}

uint32 Result::_toUint32(char *data)
{
	return static_cast<uint32>(atol(data));
}

double Result::_toDouble(char *data)
{
	return static_cast<double>(atof(data));
}

String Result::_toString(char *data, unsigned long length)
{
	return std::string(data, length);
}

Buffer Result::_toBuffer(char *data, unsigned long length)
{
	Buffer buffer(length);
	buffer.write(data, length);
	return buffer;
}
