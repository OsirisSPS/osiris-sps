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

#ifndef _OS_CORE_PIMPL_H
#define _OS_CORE_PIMPL_H

#include "boost/noncopyable.hpp"
#include "noninstantiable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, typename D = os_delete_t>
class pimpl_ptr : public boost::noncopyable
{
	typedef D deleter;

public:
	pimpl_ptr() : m_ptr(OS_NEW_T(T))
    {

	}

	template <typename P1>
	pimpl_ptr(P1 p1) : m_ptr(OS_NEW_T(T)(p1))
    {

	}

	template <typename P1, typename P2>
	pimpl_ptr(P1 p1, P2 p2) : m_ptr(OS_NEW_T(T)(p1, p2))
    {

	}

	template <typename P1, typename P2, typename P3>
	pimpl_ptr(P1 p1, P2 p2, P3 p3) : m_ptr(OS_NEW_T(T)(p1, p2, p3))
    {

	}

	template <typename P1, typename P2, typename P3, typename P4>
	pimpl_ptr(P1 p1, P2 p2, P3 p3, P4 p4) : m_ptr(OS_NEW_T(T)(p1, p2, p3, p4))
    {

	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	pimpl_ptr(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) : m_ptr(OS_NEW_T(T)(p1, p2, p3, p4, p5))
    {

	}

    ~pimpl_ptr()
    {
       clear();
    }

    T * operator->()
    {
        return m_ptr;
    }

    T & operator*()
    {
        return *m_ptr;
    }

    const T * operator->() const
    {
        return m_ptr;
    }

    const T & operator*() const
    {
        return *m_ptr;
    }

	T * get() const
    {
		return m_ptr;
    }

	void reset(T *ptr)
	{
		clear();
		m_ptr = ptr;
	}

private:
	void clear()
	{
		deleter()(m_ptr);
		m_ptr = nullptr;
	}

private:
	T *m_ptr;
};

//////////////////////////////////////////////////////////////////////

template <typename X>
class pimpl;

template <typename T>
struct pimpl_of : public noninstantiable
{
	typedef pimpl_ptr<pimpl<T> > type;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_PIMPL_H
