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
#include "dataitem.h"

#include "buffer.h"
#include "conversions.h"
#include "datatree.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const DataItem DataItem::EMPTY;

//////////////////////////////////////////////////////////////////////

DataItem::DataItem()
{
	construct();
}

DataItem::DataItem(const bool &v)
{
	construct();
	*this = v;
}

DataItem::DataItem(const byte &v)
{
	construct();
	*this = v;
}

DataItem::DataItem(const int32 &v)
{
	construct();
	*this = v;
}

DataItem::DataItem(const uint32 &v)
{
	construct();
	*this = v;
}

DataItem::DataItem(const int64 &v)
{
	construct();
	*this = v;
}

DataItem::DataItem(const uint64 &v)
{
	construct();
	*this = v;
}

DataItem::DataItem(const double &v)
{
	construct();
	*this = v;
}

DataItem::DataItem(const achar *v)
{
	construct();
	*this = String(v);
}

DataItem::DataItem(const std::string &v)
{
	construct();
	*this = String(v);
}

#ifndef OS_HAS_NATIVE_UCHAR
DataItem::DataItem(const wchar *v)
{
	construct();
	*this = String(v);
}

DataItem::DataItem(const std::wstring &v)
{
	construct();
	*this = String(v);
}
#endif

DataItem::DataItem(const uchar *v)
{
	construct();
	*this = String(v);
}

DataItem::DataItem(const String &v)
{
	construct();
	*this = v;
}

DataItem::DataItem(const Buffer &v)
{
	construct();
	*this = v;
}

DataItem::DataItem(const DataTree &v)
{
	construct();
	*this = v;
}

DataItem::DataItem(const DataItem &v) : BaseClass(v)
{
	construct();
	*this = v;
}

DataItem::~DataItem()
{
	destroy();
}

void DataItem::clear()
{
	destroy();
}

bool DataItem::getBool() const
{ 
	return *this; 
}

void DataItem::setBool(const bool& value)
{
	*this = value;
}

byte DataItem::getByte() const 
{ 
	return *this; 
}

void DataItem::setByte(const byte& value)
{
	*this = value;
}

int32 DataItem::getInt32() const 
{ 
	return *this; 
}

void DataItem::setInt32(const int32& value)
{
	*this = value;
}

uint32 DataItem::getUInt32() const 
{ 
	return *this; 
}

void DataItem::setUInt32(const uint32& value)
{
	*this = value;
}

int64 DataItem::getInt64() const 
{ 
	return *this; 
}

void DataItem::setInt64(const int64& value)
{
	*this = value;
}

uint64 DataItem::getUInt64() const 
{ 
	return *this; 
}

void DataItem::setUInt64(const uint64& value)
{
	*this = value;
}

double DataItem::getDouble() const 
{ 
	return *this; 
}

void DataItem::setDouble(const double& value)
{
	*this = value;
}

std::string DataItem::getStringAscii() const
{
	return getString().to_ascii();
}

void DataItem::setStringAscii(const std::string& value)
{
	setString(String(value));
}

String DataItem::getString() const 
{ 
	return *this; 
}

void DataItem::setString(const String& value)
{
	*this = value;
}

Buffer DataItem::getBuffer() const 
{ 
	return *this; 
}

void DataItem::setBuffer(const Buffer& value)
{
	*this = value;
}

DataTree DataItem::getDataTree() const 
{ 
	return *this; 
}

void DataItem::setDataTree(const DataTree& value)
{
	*this = value;
}

bool DataItem::read(Buffer &buff)
{
	destroy();

	byte type = 0;
	if(buff.read(&type, sizeof(byte)) == false)
		return false;

	switch(static_cast<ValueType>(type))
	{
	case vt_null:		break;

	case vt_bool:		return readBool(buff);

	case vt_byte:		return readByte(buff);

	case vt_int32:		return readInt32(buff);

	case vt_uint32:		return readUint32(buff);

	case vt_double:		return readDouble(buff);

	case vt_string:		return readString(buff);

	case vt_buffer:		return readBuffer(buff);

	case vt_datatree:	return readDataTree(buff);

	case vt_int64:		return readInt64(buff);

	case vt_uint64:		return readUint64(buff);

	default:			OS_ASSERTFALSE();
						return false;
	}

	return true;
}

bool DataItem::write(Buffer &buff) const
{
	byte type = static_cast<byte>(m_type);
	if(buff.write(&type, sizeof(byte)) == false)
		return false;

	switch(m_type)
	{
	case vt_null:		break;

	case vt_bool:		return writeBool(buff);

	case vt_byte:		return writeByte(buff);

	case vt_int32:		return writeInt32(buff);

	case vt_uint32:		return writeUint32(buff);

	case vt_double:		return writeDouble(buff);

	case vt_string:		return writeString(buff);

	case vt_buffer:		return writeBuffer(buff);

	case vt_datatree:	return writeDataTree(buff);

	case vt_int64:		return writeInt64(buff);

	case vt_uint64:		return writeUint64(buff);

	default:			OS_ASSERTFALSE();
						return false;
	}

	return true;
}

DataItem DataItem::cast(ValueType type) const
{
	if(m_type == type)
		return *this;

	switch(type)
	{
	case vt_null:		break;

	case vt_bool:		return static_cast<bool>(*this);

	case vt_byte:		return static_cast<byte>(*this);

	case vt_int32:		return static_cast<int32>(*this);

	case vt_uint32:		return static_cast<uint32>(*this);

	case vt_double:		return static_cast<double>(*this);

	case vt_string:		return static_cast<String>(*this);

	case vt_buffer:		{
                            Buffer *buffer = *this;
                            return *buffer;
                        }

	case vt_datatree:	return static_cast<DataTree>(*this);

	case vt_int64:		return static_cast<int64>(*this);

	case vt_uint64:		return static_cast<uint64>(*this);

	default:			OS_ASSERTFALSE();
						return false;
	}

	return EMPTY;
}

void DataItem::construct()
{
	m_type = vt_null;
	m_value = null;
}

void DataItem::destroy()
{
	switch(getType())
	{
	case vt_null:		break;

	case vt_bool:		deallocateT<bool>();
						break;

	case vt_byte:		deallocateT<byte>();
						break;

	case vt_int32:		deallocateT<int32>();
						break;

	case vt_uint32:		deallocateT<uint32>();
						break;

	case vt_double:		deallocateT<double>();
						break;

	case vt_string:		deleteT<String>();
						break;

	case vt_buffer:		deleteT<Buffer>();
						break;

	case vt_datatree:	deleteT<DataTree>();
						break;

	case vt_int64:		deallocateT<int64>();
						break;

	case vt_uint64:		deallocateT<uint64>();
						break;

	default:			OS_EXCEPTION("Invalid type");
	}

	m_type = vt_null;
	m_value = null;
}

bool DataItem::readBool(Buffer &buff)
{
	allocateT<bool>(false);
	m_type = vt_bool;
	return buff.read(m_value, sizeof(bool)) == sizeof(bool);
}

bool DataItem::writeBool(Buffer &buff) const
{
	return buff.write(m_value, sizeof(bool)) == sizeof(bool);
}

bool DataItem::readByte(Buffer &buff)
{
	allocateT<byte>(0);
	m_type = vt_byte;
	return buff.read(m_value, sizeof(byte)) == sizeof(byte);
}

bool DataItem::writeByte(Buffer &buff) const
{
	return buff.write(m_value, sizeof(byte)) == sizeof(byte);
}

bool DataItem::readInt32(Buffer &buff)
{
	allocateT<int32>(0);
	m_type = vt_int32;
	return buff.read(m_value, sizeof(int32)) == sizeof(int32);
}

bool DataItem::writeInt32(Buffer &buff) const
{
	return buff.write(m_value, sizeof(int32)) == sizeof(int32);
}

bool DataItem::readUint32(Buffer &buff)
{
	allocateT<uint32>(0);
	m_type = vt_uint32;
	return buff.read(m_value, sizeof(uint32)) == sizeof(uint32);
}

bool DataItem::writeUint32(Buffer &buff) const
{
	return buff.write(m_value, sizeof(uint32)) == sizeof(uint32);
}

bool DataItem::readInt64(Buffer &buff)
{
	allocateT<int64>(0);
	m_type = vt_int64;
	return buff.read(m_value, sizeof(int64)) == sizeof(int64);
}

bool DataItem::writeInt64(Buffer &buff) const
{
	return buff.write(m_value, sizeof(int64)) == sizeof(int64);
}

bool DataItem::readUint64(Buffer &buff)
{
	allocateT<uint64>(0);
	m_type = vt_uint64;
	return buff.read(m_value, sizeof(uint64)) == sizeof(uint64);
}

bool DataItem::writeUint64(Buffer &buff) const
{
	return buff.write(m_value, sizeof(uint64)) == sizeof(uint64);
}

bool DataItem::readDouble(Buffer &buff)
{
	allocateT<double>(0);
	m_type = vt_double;
	return buff.read(m_value, sizeof(double)) == sizeof(double);
}

bool DataItem::writeDouble(Buffer &buff) const
{
	return buff.write(m_value, sizeof(double)) == sizeof(double);
}

bool DataItem::readString(Buffer &buff)
{
	newT<String>();
	m_type = vt_string;
	return buff.readString(*static_cast<String *>(m_value));
}

bool DataItem::writeString(Buffer &buff) const
{
	return buff.writeString(*static_cast<String *>(m_value));
}

bool DataItem::readBuffer(Buffer &buff)
{
	newT<Buffer>();
	m_type = vt_buffer;
	return buff.readBuffer(*static_cast<Buffer *>(m_value));
}

bool DataItem::writeBuffer(Buffer &buff) const
{
	return buff.writeBuffer(*static_cast<Buffer *>(m_value));
}

bool DataItem::readDataTree(Buffer &buff)
{
	newT<DataTree>();
	m_type = vt_datatree;
	return static_cast<DataTree *>(m_value)->readItems(buff);
}

bool DataItem::writeDataTree(Buffer &buff) const
{
	return static_cast<DataTree *>(m_value)->writeItems(buff);
}

DataItem & DataItem::operator=(const bool &v)
{
	destroy();

	allocateT<bool>(v);
	m_type = vt_bool;

	return *this;
}

DataItem & DataItem::operator=(const byte &v)
{
	destroy();

	allocateT<byte>(v);
	m_type = vt_byte;

	return *this;
}

DataItem & DataItem::operator=(const int32 &v)
{
	destroy();

	allocateT<int32>(v);
	m_type = vt_int32;

	return *this;
}

DataItem & DataItem::operator=(const uint32 &v)
{
	destroy();

	allocateT<uint32>(v);
	m_type = vt_uint32;

	return *this;
}

DataItem & DataItem::operator=(const int64 &v)
{
	destroy();

	allocateT<int64>(v);
	m_type = vt_int64;

	return *this;
}

DataItem & DataItem::operator=(const uint64 &v)
{
	destroy();

	allocateT<uint64>(v);
	m_type = vt_uint64;

	return *this;
}

DataItem & DataItem::operator=(const double &v)
{
	destroy();

	allocateT<double>(v);
	m_type = vt_double;

	return *this;
}

DataItem & DataItem::operator=(const achar *v)
{
	return operator =(String(v));
}

DataItem & DataItem::operator=(const std::string &v)
{
	return operator =(String(v));
}

#ifndef OS_HAS_NATIVE_UCHAR
DataItem & DataItem::operator=(const wchar *v)
{
	return operator =(String(v));
}

DataItem & DataItem::operator=(const std::wstring &v)
{
	return operator =(String(v));
}
#endif

DataItem & DataItem::operator=(const uchar *v)
{
	return operator =(String(v));
}

DataItem & DataItem::operator=(const String &v)
{
	destroy();

	newT<String>(v);
	m_type = vt_string;

	return *this;
}

DataItem & DataItem::operator=(const Buffer &v)
{
	destroy();

	newT<Buffer>(v);
	m_type = vt_buffer;

	return *this;
}

DataItem & DataItem::operator=(const DataTree &v)
{
	destroy();

	newT<DataTree>(v);
	m_type = vt_datatree;

	return *this;
}

DataItem & DataItem::operator=(const DataItem &v)
{
	destroy();

	switch(v.getType())
	{
	case vt_null:		break;

	case vt_bool:		*this = *(static_cast<bool *>(v));
						break;

	case vt_byte:		*this = *(static_cast<byte *>(v));
						break;

	case vt_int32:		*this = *(static_cast<int32 *>(v));
						break;

	case vt_uint32:		*this = *(static_cast<uint32 *>(v));
						break;

	case vt_double:		*this = *(static_cast<double *>(v));
						break;

	case vt_string:		*this = *(static_cast<String *>(v));
						break;

	case vt_buffer:		*this = *(static_cast<Buffer *>(v));
						break;

	case vt_datatree:	*this = *(static_cast<DataTree *>(v));
						break;

	case vt_int64:		*this = *(static_cast<int64 *>(v));
						break;

	case vt_uint64:		*this = *(static_cast<uint64 *>(v));
						break;

	default:			OS_EXCEPTION("Invalid type");
	}

	return *this;
}

bool DataItem::operator ==(const DataItem &v) const
{
	if(m_type != v.m_type)
		return false;

	switch(m_type)
	{
	case vt_null:		return true;

	case vt_bool:		return (*static_cast<bool *>(m_value)) == (*static_cast<bool *>(v.m_value));

	case vt_byte:		return (*static_cast<byte *>(m_value)) == (*static_cast<byte *>(v.m_value));

	case vt_int32:		return (*static_cast<int32 *>(m_value)) == (*static_cast<int32 *>(v.m_value));

	case vt_uint32:		return (*static_cast<uint32 *>(m_value)) == (*static_cast<uint32 *>(v.m_value));

	case vt_double:		return (*static_cast<double *>(m_value)) == (*static_cast<double *>(v.m_value));

	case vt_string:		return (*static_cast<String *>(m_value)) == (*static_cast<String *>(v.m_value));

	case vt_buffer:		return (*static_cast<Buffer *>(m_value)) == (*static_cast<Buffer *>(v.m_value));

	case vt_datatree:	return (*static_cast<DataTree *>(m_value)) == (*static_cast<DataTree *>(v.m_value));

	case vt_int64:		return (*static_cast<int64 *>(m_value)) == (*static_cast<int64 *>(v.m_value));

	case vt_uint64:		return (*static_cast<uint64 *>(m_value)) == (*static_cast<uint64 *>(v.m_value));

	default:			OS_EXCEPTION("Invalid type");
	}

	return false;
}

bool DataItem::operator !=(const DataItem &v) const
{
	return (*this == v) == false;
}

DataItem::operator bool *() const
{
	return m_type == vt_bool ? static_cast<bool *>(m_value) : null;
}

DataItem::operator bool() const
{
	switch(m_type)
	{
	case vt_null:		return false;

	case vt_bool:		return *(static_cast<bool *>(m_value));

	case vt_byte:		return *(static_cast<byte *>(m_value)) != 0;

	case vt_int32:		return *(static_cast<int32 *>(m_value)) != 0;

	case vt_uint32:		return *(static_cast<uint32 *>(m_value)) != 0;

	case vt_double:		return utils::equal(*(static_cast<double *>(m_value)), 0) == false;

	case vt_string:		return conversions::from_utf16<bool>(*(static_cast<String *>(m_value)));

	case vt_buffer:		return static_cast<Buffer *>(m_value)->empty() == false;

	case vt_datatree:	return static_cast<DataTree *>(m_value)->empty() == false;

	case vt_int64:		return *(static_cast<int64 *>(m_value)) != 0;

	case vt_uint64:		return *(static_cast<uint64 *>(m_value)) != 0;

	default:			OS_EXCEPTION("Invalid type");
	}

	return false;
}

DataItem::operator byte *() const
{
	return m_type == vt_byte ? static_cast<byte *>(m_value) : null;
}

DataItem::operator byte() const
{
	switch(m_type)
	{
	case vt_null:		return 0;

	case vt_bool:		return *(static_cast<bool *>(m_value)) == true ? 1 : 0;

	case vt_byte:		return *(static_cast<byte *>(m_value));

	case vt_int32:		return static_cast<byte>(*(static_cast<int32 *>(m_value)));

	case vt_uint32:		return static_cast<byte>(*(static_cast<uint32 *>(m_value)));

	case vt_double:		return static_cast<byte>(*(static_cast<double *>(m_value)));

	case vt_string:		return conversions::from_utf16<byte>(*(static_cast<String *>(m_value)));

	case vt_int64:		return static_cast<byte>(*(static_cast<int64 *>(m_value)));

	case vt_uint64:		return static_cast<byte>(*(static_cast<uint64 *>(m_value)));

	default:			OS_ASSERTFALSE();
						break;
	}

	return 0;
}

DataItem::operator int32 *() const
{
	return m_type == vt_int32 ? static_cast<int32 *>(m_value) : null;
}

DataItem::operator int32() const
{
	switch(m_type)
	{
	case vt_null:		return 0;

	case vt_bool:		return *(static_cast<bool *>(m_value)) == true ? 1 : 0;

	case vt_byte:		return static_cast<int32>(*(static_cast<byte *>(m_value)));

	case vt_int32:		return *(static_cast<int32 *>(m_value));

	case vt_uint32:		return static_cast<int32>(*(static_cast<uint32 *>(m_value)));

	case vt_double:		return static_cast<int32>(*(static_cast<double *>(m_value)));

	case vt_string:		return conversions::from_utf16<int32>(*(static_cast<String *>(m_value)));

	case vt_int64:		return static_cast<int32>(*(static_cast<int64 *>(m_value)));

	case vt_uint64:		return static_cast<int32>(*(static_cast<uint64 *>(m_value)));

	default:			OS_ASSERTFALSE();
						break;
	}

	return 0;
}

DataItem::operator uint32 *() const
{
	return m_type == vt_uint32 ? static_cast<uint32 *>(m_value) : null;
}

DataItem::operator uint32() const
{
	switch(m_type)
	{
	case vt_null:		return 0;

	case vt_bool:		return *(static_cast<bool *>(m_value)) == true ? 1 : 0;

	case vt_byte:		return static_cast<uint32>(*(static_cast<byte *>(m_value)));

	case vt_int32:		return static_cast<uint32>(*(static_cast<int32 *>(m_value)));

	case vt_uint32:		return *(static_cast<uint32 *>(m_value));

	case vt_double:		return static_cast<uint32>(*(static_cast<double *>(m_value)));

	case vt_string:		return conversions::from_utf16<uint32>(*(static_cast<String *>(m_value)));

	case vt_int64:		return static_cast<uint32>(*(static_cast<int64 *>(m_value)));

	case vt_uint64:		return static_cast<uint32>(*(static_cast<uint64 *>(m_value)));

	default:			OS_ASSERTFALSE();
						break;
	}

	return 0;
}

DataItem::operator int64 *() const
{
	return m_type == vt_int64 ? static_cast<int64 *>(m_value) : null;
}

DataItem::operator int64() const
{
	switch(m_type)
	{
	case vt_null:		return 0;

	case vt_bool:		return *(static_cast<bool *>(m_value)) == true ? 1 : 0;

	case vt_byte:		return static_cast<int64>(*(static_cast<byte *>(m_value)));

	case vt_int32:		return static_cast<int64>(*(static_cast<int32 *>(m_value)));

	case vt_uint32:		return static_cast<int64>(*(static_cast<uint32 *>(m_value)));

	case vt_double:		return static_cast<int64>(*(static_cast<double *>(m_value)));

	case vt_string:		return conversions::from_utf16<int64>(*(static_cast<String *>(m_value)));

	case vt_int64:		return *(static_cast<int64 *>(m_value));

	case vt_uint64:		return static_cast<int64>(*(static_cast<uint64 *>(m_value)));

	default:			OS_ASSERTFALSE();
						break;
	}

	return 0;
}

DataItem::operator uint64 *() const
{
	return m_type == vt_uint64 ? static_cast<uint64 *>(m_value) : null;
}

DataItem::operator uint64() const
{
	switch(m_type)
	{
	case vt_null:		return 0;

	case vt_bool:		return *(static_cast<bool *>(m_value)) == true ? 1 : 0;

	case vt_byte:		return static_cast<uint64>(*(static_cast<byte *>(m_value)));

	case vt_int32:		return static_cast<uint64>(*(static_cast<int32 *>(m_value)));

	case vt_uint32:		return static_cast<uint64>(*(static_cast<uint32 *>(m_value)));

	case vt_double:		return static_cast<uint64>(*(static_cast<double *>(m_value)));

	case vt_string:		return conversions::from_utf16<uint64>(*(static_cast<String *>(m_value)));

	case vt_int64:		return static_cast<uint64>(*(static_cast<int64 *>(m_value)));

	case vt_uint64:		return *(static_cast<uint64 *>(m_value));

	default:			OS_ASSERTFALSE();
						break;
	}

	return 0;
}

DataItem::operator double *() const
{
	return m_type == vt_double ? static_cast<double *>(m_value) : null;
}

DataItem::operator double() const
{
	switch(m_type)
	{
	case vt_null:		return 0;

	case vt_bool:		return *(static_cast<bool *>(m_value)) == true ? 1 : 0;

	case vt_byte:		return static_cast<double>(*(static_cast<byte *>(m_value)));

	case vt_int32:		return static_cast<double>(*(static_cast<int32 *>(m_value)));

	case vt_uint32:		return static_cast<double>(*(static_cast<uint32 *>(m_value)));

	case vt_double:		return *(static_cast<double *>(m_value));

	case vt_string:		return conversions::from_utf16<double>(*(static_cast<String *>(m_value)));

	case vt_int64:		return static_cast<double>(*(static_cast<int64 *>(m_value)));

	case vt_uint64:		return static_cast<double>(*(static_cast<uint64 *>(m_value)));

	default:			OS_ASSERTFALSE();
						break;
	}

	return 0;
}

DataItem::operator String *() const
{
	return m_type == vt_string ? static_cast<String *>(m_value) : null;
}

DataItem::operator String() const
{
	switch(m_type)
	{
	case vt_null:		return String::EMPTY;

	case vt_bool:		return conversions::to_utf16(*(static_cast<bool *>(m_value)));

	case vt_byte:		return conversions::to_utf16(*(static_cast<byte *>(m_value)));

	case vt_int32:		return conversions::to_utf16(*(static_cast<int32 *>(m_value)));

	case vt_uint32:		return conversions::to_utf16(*(static_cast<uint32 *>(m_value)));

	case vt_double:		return conversions::to_utf16(*(static_cast<double *>(m_value)));

	case vt_string:		return *static_cast<String *>(m_value);

	case vt_int64:		return conversions::to_utf16(*(static_cast<int64 *>(m_value)));

	case vt_uint64:		return conversions::to_utf16(*(static_cast<uint64 *>(m_value)));

	default:			OS_ASSERTFALSE();
						break;
	}

	return String::EMPTY;
}

DataItem::operator Buffer *() const
{
	return m_type == vt_buffer ? static_cast<Buffer *>(m_value) : null;
}

DataItem::operator Buffer() const
{
	switch(m_type)
	{
	case vt_null:		return Buffer::EMPTY;

	case vt_buffer:		return *static_cast<Buffer *>(m_value);

	case vt_string:		{
							// Nota la stringa deve essere serializzata in chiaro per cui non richiamare il metodo writeString (serializzerebbe anche la dimensione)

							String *str = static_cast<String *>(m_value);
							Buffer tmp;
							tmp.write(str->buffer(), static_cast<uint32>(str->buffer_size()));
							return tmp;
						}

	default:			OS_ASSERTFALSE();
						break;
	}

	return Buffer::EMPTY;
}

DataItem::operator DataTree *() const
{
	return m_type == vt_datatree ? static_cast<DataTree *>(m_value) : null;
}

DataItem::operator DataTree() const
{
	switch(m_type)
	{
	case vt_null:		return DataTree::EMPTY;

	case vt_datatree:	return *static_cast<DataTree *>(m_value);

	default:			OS_ASSERTFALSE();
						break;
	}

	return DataTree::EMPTY;
}

template <>
CoreExport Buffer DataItem::get<Buffer>() const
{
    // Specializza il cast al buffer visto potrebbe essere invocato sia il costruttore di copia,
    // sia il costrutture che prende come parametro il "grow size" del buffer invocando "operator uint32()"

    return operator Buffer();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
