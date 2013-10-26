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

#ifndef _DB_SQL_COLLECTION_H
#define _DB_SQL_COLLECTION_H

#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
class DbSqlCollection
{
public:
	typedef typename list<T>::type list_type;
	typedef typename list_type::iterator iterator;
	typedef typename list_type::const_iterator const_iterator;
	typedef typename list_type::reverse_iterator reverse_iterator;
	typedef typename list_type::const_reverse_iterator const_reverse_iterator;

// Construction
public:
	DbSqlCollection();
	virtual ~DbSqlCollection();

// Attributes
public:
	inline iterator begin() { return m_list.begin(); }
	inline const_iterator begin() const { return m_list.begin(); }
	inline iterator end() { return m_list.end(); }
	inline const_iterator end() const { return m_list.end(); }

	inline reverse_iterator rbegin() { return m_list.rbegin(); }
	inline const_reverse_iterator rbegin() const { return m_list.rbegin(); }
	inline reverse_iterator rend() { return m_list.rend(); }
	inline const_reverse_iterator rend() const { return m_list.rend(); }

// Operations
public:
	T & add(const T &value);
	void clear();

protected:
	list_type m_list;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
DbSqlCollection<T>::DbSqlCollection()
{

}

template <typename T>
DbSqlCollection<T>::~DbSqlCollection()
{

}

template <typename T>
T & DbSqlCollection<T>::add(const T &value)
{
	OS_ASSERT(utils::exists(m_list, value) == false);
	m_list.push_back(value);
	return m_list.back();
}

template <typename T>
void DbSqlCollection<T>::clear()
{
	m_list.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _DB_SQL_COLLECTION_H
