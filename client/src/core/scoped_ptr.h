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

#ifndef _OS_CORE_SCOPED_PTR_H
#define _OS_CORE_SCOPED_PTR_H

#include "boost/noncopyable.hpp"
#include "boost/checked_delete.hpp"
#include "safe_bool.h"
#include <utility>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, typename D = boost::checked_deleter<T> > 
class scoped_ptr : public boost::noncopyable,
				   public safe_bool<scoped_ptr<T, D> > 
{
	typedef D deleter;

// Construction
public:
    explicit scoped_ptr(T *ptr = 0);
    explicit scoped_ptr(std::auto_ptr<T> ptr);
    ~scoped_ptr();    

// Attributes
public:
	inline T * get() const;

// Operations
public:
	void reset(T *ptr = 0);
	void swap(scoped_ptr<T, D> &second);

// safe_bool interface
public:
    bool boolean_test() const;

// Operators
public:
    inline T & operator *() const;
    inline T * operator ->() const;
    inline bool operator !() const;

private:
	// noncomparable
    void operator ==(const scoped_ptr<T, D> &) const;
    void operator !=(const scoped_ptr<T, D> &) const;

private:
    T *m_ptr;
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename D> 
scoped_ptr<T, D>::scoped_ptr(T *ptr) : m_ptr(ptr)
{

}

template <typename T, typename D> 
scoped_ptr<T, D>::scoped_ptr(std::auto_ptr<T> ptr) : m_ptr(ptr.release())
{

}

template <typename T, typename D> 
scoped_ptr<T, D>::~scoped_ptr()
{
	deleter()(m_ptr);
}

template <typename T, typename D> 
inline T * scoped_ptr<T, D>::get() const
{
    return m_ptr;
}

template <typename T, typename D> 
void scoped_ptr<T, D>::reset(T *ptr)
{
    OS_ASSERT(ptr == 0 || ptr != m_ptr);
    scoped_ptr<T, D>(ptr).swap(*this);
}

template <typename T, typename D> 
void scoped_ptr<T, D>::swap(scoped_ptr<T, D> &second)
{
	std::swap(m_ptr, second.m_ptr);
}

template <typename T, typename D> 
bool scoped_ptr<T, D>::boolean_test() const 
{
	return m_ptr != 0;
}

template <typename T, typename D> 
inline T & scoped_ptr<T, D>::operator *() const
{
    OS_ASSERT(m_ptr != 0);
    return *m_ptr;
}

template <typename T, typename D> 
inline T * scoped_ptr<T, D>::operator ->() const
{
    OS_ASSERT(m_ptr != 0);
    return m_ptr;
}        

template <typename T, typename D> 
inline bool scoped_ptr<T, D>::operator !() const
{
    return m_ptr == 0;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_SCOPED_PTR_H
