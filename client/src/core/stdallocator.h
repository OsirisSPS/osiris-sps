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

#ifndef _OS_CORE_STDALLOCATOR_H
#define _OS_CORE_STDALLOCATOR_H

#include "allocators.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, typename A = typename DefaultAllocator<T>::type>
class StdAllocator
{
public:
	typedef A Allocator;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;

	template <typename U>
	struct rebind
	{
		typedef StdAllocator<U, typename Allocator::template rebind<U>::other> other;
	};

// Construction
public:
	inline explicit StdAllocator();
	inline StdAllocator(const StdAllocator<T, A> &second);
	template <typename U, typename Z>
	inline StdAllocator(const StdAllocator<U, Z> &second);
	virtual ~StdAllocator();

// Attributes
public:
	inline const Allocator & allocator() const;

	inline pointer address(reference value) const;
	inline const_pointer address(const_reference value) const;

	inline size_type max_size() const;

	// Operations
public:
	inline pointer allocate(const size_type n, const void *ptr = 0);
	inline void deallocate(pointer ptr, const size_type n = 0);
	
	inline void construct(pointer p, const T &v);
    inline void destroy(pointer p);

public:
	inline bool operator ==(const StdAllocator<T, A> &second) const;
	inline bool operator !=(const StdAllocator<T, A> &second) const;

protected:
	Allocator m_allocator;
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename A>
inline StdAllocator<T, A>::StdAllocator()
{

}

template <typename T, typename A>
inline StdAllocator<T, A>::StdAllocator(const StdAllocator<T, A> &second) : m_allocator(second.allocator())
{

}

template <typename T, typename A>
template <typename U, typename Z>
inline StdAllocator<T, A>::StdAllocator(const StdAllocator<U, Z> &second) : m_allocator(second.allocator())
{

}

template <typename T, typename A>
StdAllocator<T, A>::~StdAllocator()
{

}

template <typename T, typename A>
inline const typename StdAllocator<T, A>::Allocator & StdAllocator<T, A>::allocator() const
{
	return m_allocator;
}

template <typename T, typename A>
inline typename StdAllocator<T, A>::pointer StdAllocator<T, A>::address(reference value) const
{
	return m_allocator.address(value);
}

template <typename T, typename A>
inline typename StdAllocator<T, A>::const_pointer StdAllocator<T, A>::address(const_reference value) const
{
	return m_allocator.address(value);
}

template <typename T, typename A>
inline typename StdAllocator<T, A>::size_type StdAllocator<T, A>::max_size() const
{
	return m_allocator.max_size();
}

template <typename T, typename A>
inline typename StdAllocator<T, A>::pointer StdAllocator<T, A>::allocate(const size_type n, const void *)
{
	register size_type size = n * sizeof(T);
	return static_cast<pointer>(m_allocator.allocate(size));
}

template <typename T, typename A>
inline void StdAllocator<T, A>::deallocate(pointer ptr, const size_type)
{
	m_allocator.deallocate(ptr);
}

template <typename T, typename A>
inline void StdAllocator<T, A>::construct(pointer p, const T &v)
{
	new (static_cast<void *>(p)) T(v);
}

template <typename T, typename A>
inline void StdAllocator<T, A>::destroy(pointer p)
{
	OS_ASSERT(p != null);
	p->~T();
}

template <typename T, typename A>
inline bool StdAllocator<T, A>::operator ==(const StdAllocator<T, A> &second) const
{
	return m_allocator == second.allocator();
}

template <typename T, typename A>
inline bool StdAllocator<T, A>::operator !=(const StdAllocator<T, A> &second) const
{
	return m_allocator != second.allocator();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_STDALLOCATOR_H
