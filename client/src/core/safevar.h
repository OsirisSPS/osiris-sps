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

#ifndef _OS_CORE_SAFEVAR_H
#define _OS_CORE_SAFEVAR_H

#include "boost/thread/shared_mutex.hpp"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
class SafeVar
{
// Construction
public:
	SafeVar();
	SafeVar(const T &v);
	~SafeVar();

// Operators
public:
	operator T() const;
	void operator=(const T &v);
	void operator=(const SafeVar<T> &v);
	
// Operations
private:
	T _get() const;
	void _set(const T &v);

private:
	T m_value;
	bool m_initialized;
	mutable boost::shared_mutex m_cs;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
SafeVar<T>::SafeVar() : m_initialized(false)
{
	
}

template <typename T>
SafeVar<T>::SafeVar(const T &v) : m_initialized(false)
{	
	_set(v);
}

template <typename T>
SafeVar<T>::~SafeVar()
{
	
}

template <typename T>
SafeVar<T>::operator T() const
{
	return _get();
}

template <typename T>
void SafeVar<T>::operator=(const T &v)
{
	_set(v);
}

template <typename T>
void SafeVar<T>::operator=(const SafeVar<T> &v)
{
	_set(v._get());	
}

template <typename T>
T SafeVar<T>::_get() const
{	
	boost::shared_lock<boost::shared_mutex> lock(m_cs);

	OS_EXCEPT_IF(m_initialized == false, "Uninitialized var");
	return m_value;
}

template <typename T>
void SafeVar<T>::_set(const T &v)
{
	boost::unique_lock<boost::shared_mutex> lock(m_cs);

	m_initialized = true;
	m_value = v;		
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_SAFEVAR_H
