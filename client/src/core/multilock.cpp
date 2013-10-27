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
#include "multilock.h"

#include "boost/bind.hpp"
#include "boost/thread/locks.hpp"
#include "boost/thread/recursive_mutex.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<MultiLock>
{
	typedef boost::unique_lock<boost::recursive_mutex> Lock;
	typedef vector<shared_ptr<Lock> >::type Locks;

// Construction
public:
	#define OS_MULTILOCK_PIMPL_CTOR_DECL(z, n, _) pimpl(BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), boost::recursive_mutex *m));
	BOOST_PP_REPEAT(OS_MULTILOCK_ARITY, OS_MULTILOCK_PIMPL_CTOR_DECL, _)
	#undef OS_MULTILOCK_PIMPL_CTOR_DECL	
	~pimpl();

// Attributes
public:
	bool owns_lock() const;

// Operations
public:
	void lock();
	void unlock();

private:
	void add(boost::recursive_mutex *m);

private:
    Locks m_locks;
};

//////////////////////////////////////////////////////////////////////

#define OS_MULTILOCK_PIMPL_ADD(count, index, param) add(param ## index);
#define OS_MULTILOCK_PIMPL_CTOR_IMPL(z, n, _) \
pimpl<MultiLock>::pimpl(BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), boost::recursive_mutex *m)) \
{ \
	BOOST_PP_REPEAT(BOOST_PP_INC(n), OS_MULTILOCK_PIMPL_ADD, m) \
}
BOOST_PP_REPEAT(OS_MULTILOCK_ARITY, OS_MULTILOCK_PIMPL_CTOR_IMPL, _)
#undef OS_MULTILOCK_PIMPL_CTOR_IMPL
#undef OS_MULTILOCK_PIMPL_ADD

pimpl<MultiLock>::~pimpl()
{

}

bool pimpl<MultiLock>::owns_lock() const
{
	OS_ASSERT(m_locks.empty() || (std::find_if(m_locks.begin() + 1, m_locks.end(), boost::bind(&Lock::owns_lock, _1) != m_locks.front()->owns_lock()) == m_locks.end()));		// Lo stato delle lock deve essere allineato
	return m_locks.empty() ? false : m_locks.front()->owns_lock();
}

void pimpl<MultiLock>::lock()
{
	OS_ASSERT(m_locks.empty() || (std::find_if(m_locks.begin() + 1, m_locks.end(), boost::bind(&Lock::owns_lock, _1) != m_locks.front()->owns_lock()) == m_locks.end()));		// Lo stato delle lock deve essere allineato
	for(Locks::const_iterator i = m_locks.begin(); i != m_locks.end(); (*i)->lock(), ++i);	
}

void pimpl<MultiLock>::unlock()
{
	OS_ASSERT(m_locks.empty() || (std::find_if(m_locks.begin() + 1, m_locks.end(), boost::bind(&Lock::owns_lock, _1) != m_locks.front()->owns_lock()) == m_locks.end()));		// Lo stato delle lock deve essere allineato
	for(Locks::const_iterator i = m_locks.begin(); i != m_locks.end(); (*i)->unlock(), ++i);	
}

void pimpl<MultiLock>::add(boost::recursive_mutex *m)
{
	OS_ASSERT(m != null);
	OS_ASSERT(std::find_if(m_locks.begin(), m_locks.end(), boost::bind(&Lock::mutex, _1) == m) == m_locks.end());
	m_locks.push_back(shared_ptr<Lock>(new Lock(*m, boost::defer_lock)));
}

//////////////////////////////////////////////////////////////////////

#define OS_MULTILOCK_CTOR_IMPL(z, n, _) \
MultiLock::MultiLock(BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), boost::recursive_mutex &m), bool initially_locked) : m_impl(BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), &m)) \
{ \
	if(initially_locked) \
		lock(); \
}
BOOST_PP_REPEAT(OS_MULTILOCK_ARITY, OS_MULTILOCK_CTOR_IMPL, _)
#undef OS_MULTILOCK_CTOR_IMPL

MultiLock::~MultiLock()
{
	// Non richiamare la unlock o si rischia che venga lanciata un'eccezione nel caso in cui non sia stato effettuato 
	// un precedente lock (un'eventuale unlock viene chiamata in automatico nel distruttore delle varie lock)	
}

bool MultiLock::owns_lock() const
{
    return m_impl->owns_lock();
}

void MultiLock::lock()
{
	return m_impl->lock();
}

void MultiLock::unlock()
{
	return m_impl->unlock();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
