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

#ifndef _OS_CORE_LOCKED_H
#define _OS_CORE_LOCKED_H

#include "boost/thread/locks.hpp"
#include "lock.h"
#include "noninstantiable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, typename M, typename L>
class LockPtr
{
public:
	typedef T value_type;
	typedef M mutex_type;
	typedef L lock_type;

// Construction
public:
	LockPtr(T &value, M &mutex);
	LockPtr(T *value, M &mutex);
	LockPtr(const LockPtr &second);
	~LockPtr();

// Attributes
public:
	inline bool empty() const;
	inline T * get() const;

// Operators
public:
	inline T & operator *() const;
	inline T * operator ->() const;

	LockPtr & operator=(const LockPtr &second);

private:
	scoped_ptr<L, os_delete_t> m_lock;
	T *m_ptr;
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename M, typename L>
LockPtr<T, M, L>::LockPtr(T &value, M &mutex) : m_lock(OS_NEW_T(L)(mutex)),	// Richiama prima il costruttore della lock per acquisire il mutex
											    m_ptr(&value)
{
	OS_ASSERT(m_lock->owns_lock());
}

template <typename T, typename M, typename L>
LockPtr<T, M, L>::LockPtr(T *value, M &mutex) : m_lock(OS_NEW_T(L)(mutex)),	// Richiama prima il costruttore della lock per acquisire il mutex
											    m_ptr(value)
{
	OS_ASSERT(m_lock->owns_lock());
}

template <typename T, typename M, typename L>
LockPtr<T, M, L>::LockPtr(const LockPtr<T, M, L> &second) : m_lock(OS_NEW_T(L)(*second.m_lock->mutex())),		// Richiama prima il costruttore della lock per acquisire il mutex
															m_ptr(second.m_ptr)
{
	OS_ASSERT(m_lock->owns_lock());
}

template <typename T, typename M, typename L>
LockPtr<T, M, L>::~LockPtr()
{

}

template <typename T, typename M, typename L>
inline bool LockPtr<T, M, L>::empty() const
{
	return m_ptr == null;
}

template <typename T, typename M, typename L>
inline T * LockPtr<T, M, L>::get() const
{
	return m_ptr;
}

template <typename T, typename M, typename L>
inline T & LockPtr<T, M, L>::operator *() const
{
	OS_ASSERT(m_ptr != null);
	return *m_ptr;
}

template <typename T, typename M, typename L>
inline T * LockPtr<T, M, L>::operator ->() const
{
	return m_ptr;
}

template <typename T, typename M, typename L>
LockPtr<T, M, L> & LockPtr<T, M, L>::operator=(const LockPtr &second)
{
    // Richiama prima il costruttore della lock per acquisire il mutex
    m_lock.reset(OS_NEW_T(L)(*second.m_lock->mutex()));
	m_ptr = second.m_ptr;

    return *this;
}

//////////////////////////////////////////////////////////////////////

template <typename T, typename M = boost::recursive_mutex>
struct Locked : public noninstantiable
{
	typedef LockPtr<T, M, boost::unique_lock<M> > unique;
	typedef LockPtr<T, M, boost::shared_lock<M> > shared;	
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_LOCKED_H
