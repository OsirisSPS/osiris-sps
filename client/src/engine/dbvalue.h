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

#ifndef _DB_VALUE_H
#define _DB_VALUE_H

#include "datetime.h"
#include "dbsqlvalues.h"
#include "buffer.h"
#include "datatable.h"
#include "datatree.h"
#include "ids.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
const T & get_default();

template <typename T>
String export_to_xml(const T &value);

template <typename T>
T import_from_xml(const String &value);

template <typename T>
inline T from_data_item(const DataItem &value)
{
	return value.get<T>();
}

template <typename T>
inline DataItem to_data_item(const T &value)
{
	return value;
}

template <>
inline DateTime from_data_item<DateTime>(const DataItem &value)
{
	return static_cast<String>(value);
}

template <>
inline DataItem to_data_item<DateTime>(const DateTime &value)
{
	return value.toString();
}


#ifndef OS_DEBUGID
template <>
inline UniqueID from_data_item<UniqueID>(const DataItem &value)
{
	return static_cast<String>(value).to_ascii();
}

template <>
inline DataItem to_data_item<UniqueID>(const UniqueID &value)
{
	return value.toUTF16();
}
#else


template <>
inline ObjectID from_data_item<ObjectID>(const DataItem &value)
{
	return static_cast<String>(value).to_ascii();
}

template <>
inline DataItem to_data_item<ObjectID>(const ObjectID &value)
{
	return value.toUTF16();
}

template <>
inline EntityID from_data_item<EntityID>(const DataItem &value)
{
	return static_cast<String>(value).to_ascii();
}

template <>
inline DataItem to_data_item<EntityID>(const EntityID &value)
{
	return value.toUTF16();
}
/*

*/

template <>
inline PovID from_data_item<PovID>(const DataItem &value)
{
	return static_cast<String>(value).to_ascii();
}

template <>
inline DataItem to_data_item<PovID>(const PovID &value)
{
	return value.toUTF16();
}

template <>
inline ExtensionsComponentID from_data_item<ExtensionsComponentID>(const DataItem &value)
{
	return static_cast<String>(value).to_ascii();
}

template <>
inline DataItem to_data_item<ExtensionsComponentID>(const ExtensionsComponentID &value)
{
	return value.toUTF16();
}

template <>
inline ExtensionsModuleID from_data_item<ExtensionsModuleID>(const DataItem &value)
{
	return static_cast<String>(value).to_ascii();
}

template <>
inline DataItem to_data_item<ExtensionsModuleID>(const ExtensionsModuleID &value)
{
	return value.toUTF16();
}

#endif

//////////////////////////////////////////////////////////////////////

template <typename T>
class DbValue
{
// Construction
public:
	DbValue();
	DbValue(const T &value);
	DbValue(const DbValue<T> &value);
	DbValue(const DataItem &value);
	template <typename I>
	DbValue(const I &value);
	~DbValue();

// Attributes
public:
	inline bool isNull() const;
	inline void setNull();

	const T & get() const;
	const T * get_ptr() const;

	void set(const T &value);

	T & value() const;

	String toXML() const;
	void fromXML(const String &value);

// Operations
public:
	void read(const DataTableRow &row, const String &field);
	void read(const DataTree &data, const String &field);

	void write(DbSqlValues &values, const String &field) const;
	void write(DataTree &data, const String &field) const;

	void reset();

private:
	void _copy(const DbValue<T> &value);
	void _copy(const DataItem &value);

// Operators
public:
	const T * operator->() const;
	const T & operator*() const;

	operator const T &() const;

	DbValue & operator=(const T &value);
	DbValue & operator=(const DbValue<T> &value);
	DbValue & operator=(const DataItem &value);

	template <typename Y>
	bool operator==(const Y &value) const;
	template <typename Y>
	bool operator!=(const Y &value) const;

	template <typename Y>
	bool operator<(const Y &value) const;
	template <typename Y>
	bool operator<=(const Y &value) const;

	template <typename Y>
	bool operator>(const Y &value) const;
	template <typename Y>
	bool operator>=(const Y &value) const;

protected:
	mutable T *m_value;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
inline bool DbValue<T>::isNull() const { return m_value == null; }

template <typename T>
inline void DbValue<T>::setNull() { reset(); }

//////////////////////////////////////////////////////////////////////

template <typename T>
DbValue<T>::DbValue() : m_value(null)
{

}

template <typename T>
DbValue<T>::DbValue(const T &value) : m_value(null)
{
	set(value);
}

template <typename T>
DbValue<T>::DbValue(const DbValue<T> &value) : m_value(null)
{
	_copy(value);
}

template <typename T>
DbValue<T>::DbValue(const DataItem &value) : m_value(null)
{
	_copy(value);
}

template <typename T>
template <typename I>
DbValue<T>::DbValue(const I &value) : m_value(null)
{
	set(static_cast<T>(value));
}

template <typename T>
DbValue<T>::~DbValue()
{
	reset();
}

template <typename T>
const T & DbValue<T>::get() const
{
	if(m_value == null)
		return get_default<T>();

	return *m_value;
}

template <typename T>
const T * DbValue<T>::get_ptr() const
{
	return &get();
}

template <typename T>
void DbValue<T>::set(const T &value)
{
	if(m_value == null)
		m_value = OS_NEW_T(T)(value);
	else
		*m_value = value;
}

template <typename T>
T & DbValue<T>::value() const
{
	if(m_value == null)
		m_value = OS_NEW_T(T);

	return *m_value;
}

template <typename T>
String DbValue<T>::toXML() const
{
	return export_to_xml<T>(get());
}

template <typename T>
void DbValue<T>::fromXML(const String &value)
{
	set(import_from_xml<T>(value));
}

template <typename T>
const T * DbValue<T>::operator->() const
{
	return get_ptr();
}

template <typename T>
const T & DbValue<T>::operator*() const
{
	return get();
}

template <typename T>
DbValue<T>::operator const T &() const
{
	return get();
}

template <typename T>
DbValue<T> & DbValue<T>::operator=(const T &value)
{
	set(value);
	return *this;
}

template <typename T>
DbValue<T> & DbValue<T>::operator=(const DbValue<T> &value)
{
	_copy(value);
	return *this;
}

template <typename T>
DbValue<T> & DbValue<T>::operator=(const DataItem &value)
{
	_copy(value);
	return *this;
}

template <typename T>
template <typename Y>
bool DbValue<T>::operator==(const Y &value) const
{
	return get() == value;
}

template <typename T>
template <typename Y>
bool DbValue<T>::operator!=(const Y &value) const
{
	return get() != value;
}

template <typename T>
template <typename Y>
bool DbValue<T>::operator<(const Y &value) const
{
	return get() < value;
}

template <typename T>
template <typename Y>
bool DbValue<T>::operator<=(const Y &value) const
{
	return get() <= value;
}

template <typename T>
template <typename Y>
bool DbValue<T>::operator>(const Y &value) const
{
	return get() > value;
}

template <typename T>
template <typename Y>
bool DbValue<T>::operator>=(const Y &value) const
{
	return get() >= value;
}

template <typename T>
void DbValue<T>::read(const DataTableRow &row, const String &field)
{
	DataItem *value = row[field];
	// OS_EXCEPT_IF(value == null, String::format(_S("'%S': invalid row value").c_str(), field.c_str()));
	if(value == null)
		setNull();		// TODO: corretto?
	else
        _copy(*value);
}

template <typename T>
void DbValue<T>::read(const DataTree &data, const String &field)
{
	DataItem *value = data.getVP(field);
	//OS_EXCEPT_IF(value == null, String::format(_S("'%S': invalid item value").c_str(), field.c_str()));
	if(value == null)
		setNull();		// TODO: corretto?
	else
		_copy(*value);
}

template <typename T>
void DbValue<T>::write(DbSqlValues &values, const String &field) const
{
	if(isNull())
		values.set(field, DataItem::EMPTY);
	else
		values.set(field, to_data_item(get()));
}

template <typename T>
void DbValue<T>::write(DataTree &data, const String &field) const
{
	if(isNull())
		data.setV(field, DataItem::EMPTY);
	else
		data.setV(field, to_data_item(get()));
}

template <typename T>
void DbValue<T>::reset()
{
	OS_DELETE_T(m_value);
	m_value = null;	
}

template <typename T>
void DbValue<T>::_copy(const DbValue<T> &value)
{
	if(value.isNull())
		setNull();
	else
        set(value.get());
}

template <typename T>
void DbValue<T>::_copy(const DataItem &value)
{
	if(value.isNull())
		setNull();
	else
        set(from_data_item<T>(value));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_VALUE_H
