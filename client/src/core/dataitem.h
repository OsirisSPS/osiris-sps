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

#ifndef _OS_CORE_DATAITEM_H
#define _OS_CORE_DATAITEM_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

// N.B.: non modificare mai questi valori

#define OS_DATAITEM_NULL						0
#define OS_DATAITEM_BOOL						1
#define OS_DATAITEM_BYTE						2
#define OS_DATAITEM_INT32						3
#define OS_DATAITEM_UINT32						4
#define OS_DATAITEM_DOUBLE						5
#define OS_DATAITEM_STRING						6
#define OS_DATAITEM_BUFFER						7
#define OS_DATAITEM_DATATREE					8
#define OS_DATAITEM_INT64						9
#define OS_DATAITEM_UINT64						10

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class DataTree;

//////////////////////////////////////////////////////////////////////

class CoreExport DataItem : public Object
{
	typedef Object BaseClass;

public:
	enum ValueType
	{
		vt_null			= OS_DATAITEM_NULL,
		vt_bool			= OS_DATAITEM_BOOL,
		vt_byte			= OS_DATAITEM_BYTE,
		vt_int32		= OS_DATAITEM_INT32,
		vt_uint32		= OS_DATAITEM_UINT32,
		vt_double		= OS_DATAITEM_DOUBLE,
		vt_string		= OS_DATAITEM_STRING,
		vt_buffer		= OS_DATAITEM_BUFFER,
		vt_datatree		= OS_DATAITEM_DATATREE,
		vt_int64		= OS_DATAITEM_INT64,
		vt_uint64		= OS_DATAITEM_UINT64,
	};

// Construction
public:
	DataItem();
	DataItem(const bool &v);
	DataItem(const byte &v);
	DataItem(const int32 &v);
	DataItem(const uint32 &v);
	DataItem(const int64 &v);
	DataItem(const uint64 &v);
	DataItem(const double &v);
	DataItem(const achar *v);
	DataItem(const std::string &v);
#ifndef OS_HAS_NATIVE_UCHAR
	DataItem(const wchar *v);
	DataItem(const std::wstring &v);
#endif
	DataItem(const uchar *v);
	DataItem(const String &v);
	DataItem(const Buffer &v);
	DataItem(const DataTree &v);
	DataItem(const DataItem &v);
	virtual ~DataItem();

// Attributes
public:
	inline const ValueType & getType() const;

	inline bool isNull() const;
	inline bool isDataTree() const;

	bool getBool() const;
	void setBool(const bool& value);

	byte getByte() const;
	void setByte(const byte& value);

	int32 getInt32() const;
	void setInt32(const int32& value);

	uint32 getUInt32() const;
	void setUInt32(const uint32& value);

	int64 getInt64() const;
	void setInt64(const int64& value);

	uint64 getUInt64() const;
	void setUInt64(const uint64& value);

	double getDouble() const;
	void setDouble(const double& value);

	std::string getStringAscii() const;
	void setStringAscii(const std::string& value);

	String getString() const;
	void setString(const String& value);

	Buffer getBuffer() const;
	void setBuffer(const Buffer& value);

	DataTree getDataTree() const;
	void setDataTree(const DataTree& value);


// Operations
public:
	void clear();

	bool read(Buffer &buff);
	bool write(Buffer &buff) const;

	DataItem cast(ValueType type) const;

private:
	void construct();
	void destroy();

	bool readBool(Buffer &buff);
	bool writeBool(Buffer &buff) const;

	bool readByte(Buffer &buff);
	bool writeByte(Buffer &buff) const;

	bool readInt32(Buffer &buff);
	bool writeInt32(Buffer &buff) const;

	bool readUint32(Buffer &buff);
	bool writeUint32(Buffer &buff) const;

	bool readInt64(Buffer &buff);
	bool writeInt64(Buffer &buff) const;

	bool readUint64(Buffer &buff);
	bool writeUint64(Buffer &buff) const;

	bool readDouble(Buffer &buff);
	bool writeDouble(Buffer &buff) const;

	bool readString(Buffer &buff);
	bool writeString(Buffer &buff) const;

	bool readBuffer(Buffer &buff);
	bool writeBuffer(Buffer &buff) const;

	bool readDataTree(Buffer &buff);
	bool writeDataTree(Buffer &buff) const;

	template <typename T>
	inline void allocateT(const T &value);
	template <typename T>
	inline void deallocateT();

	template <typename T>
	inline void newT();
	template <typename T>
	inline void newT(const T &value);
	template <typename T>
	inline void deleteT();

// Operators
public:
	DataItem & operator=(const bool &v);
	DataItem & operator=(const byte &v);
	DataItem & operator=(const int32 &v);
	DataItem & operator=(const uint32 &v);
	DataItem & operator=(const int64 &v);
	DataItem & operator=(const uint64 &v);
	DataItem & operator=(const double &v);
	DataItem & operator=(const achar *v);
	DataItem & operator=(const std::string &v);
#ifndef OS_HAS_NATIVE_UCHAR
	DataItem & operator=(const wchar *v);
	DataItem & operator=(const std::wstring &v);
#endif
	DataItem & operator=(const uchar *v);
	DataItem & operator=(const String &v);
	DataItem & operator=(const Buffer &v);
	DataItem & operator=(const DataTree &v);
	DataItem & operator=(const DataItem &v);

	bool operator ==(const DataItem &v) const;
	bool operator !=(const DataItem &v) const;

	operator bool *() const;
	operator bool() const;

	operator byte *() const;
	operator byte() const;

	operator int32 *() const;
	operator int32() const;

	operator uint32 *() const;
	operator uint32() const;

	operator int64 *() const;
	operator int64() const;

	operator uint64 *() const;
	operator uint64() const;

	operator double *() const;
	operator double() const;

	operator String *() const;
	operator String() const;

	operator Buffer *() const;
	operator Buffer() const;

	operator DataTree *() const;
	operator DataTree() const;

    template <typename T>
    T get() const;

public:
	static const DataItem EMPTY;

protected:
	ValueType m_type;
	void *m_value;
};

//////////////////////////////////////////////////////////////////////

inline const DataItem::ValueType & DataItem::getType() const { return m_type; }

inline bool DataItem::isNull() const { return m_type == vt_null; }
inline bool DataItem::isDataTree() const { return m_type == vt_datatree; }

//////////////////////////////////////////////////////////////////////

template <typename T>
T DataItem::get() const
{
    return static_cast<T>(*this);
}

template <>
CoreExport Buffer DataItem::get<Buffer>() const;

template <typename T>
inline void DataItem::allocateT(const T &value)
{
	m_value = OS_ALLOCATE_T(T, 1);
	*static_cast<T *>(m_value) = value;
}

template <typename T>
inline void DataItem::deallocateT()
{
	OS_DEALLOCATE_T(static_cast<T *>(m_value));
}

template <typename T>
inline void DataItem::newT()
{
	m_value = OS_NEW_T(T);	
}

template <typename T>
inline void DataItem::newT(const T &value)
{
	m_value = OS_NEW_T(T)(value);
}

template <typename T>
inline void DataItem::deleteT()
{
	OS_DELETE_T(static_cast<T *>(m_value));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_DATAITEM_H
