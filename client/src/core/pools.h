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

#ifndef _OS_CORE_POOLS_H
#define _OS_CORE_POOLS_H

#include "allocators.h"
#include "boost/pool/object_pool.hpp"
#include "boost/pool/singleton_pool.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename A>
struct BoostPoolAllocator : public A
{
	typedef A Allocator;

	inline static char * malloc(const typename Allocator::size_type size);
	inline static void free(char * const ptr);
};

//////////////////////////////////////////////////////////////////////

template <typename A>
inline char * BoostPoolAllocator<A>::malloc(const typename Allocator::size_type size)
{
	return reinterpret_cast<char *>(Allocator::allocate(size));
}

template <typename A>
inline void BoostPoolAllocator<A>::free(char * const ptr)
{
	Allocator::deallocate(reinterpret_cast<typename Allocator::pointer>(ptr));
}

//////////////////////////////////////////////////////////////////////

// Implementazione thread safe

template <typename T, unsigned S = sizeof(T), typename A = BoostPoolAllocator<typename DefaultAllocator<T>::type>, typename P = boost::singleton_pool<T, S, A> >
class SingletonPoolAllocator : public BasicAllocator<T>
{
public:
	typedef BasicAllocator<T> Base;
	typedef typename Base::size_type size_type;
	typedef typename Base::pointer pointer;
	typedef A Allocator;
	typedef P Pool;
	
	template <typename U>
	struct rebind
	{
		typedef SingletonPoolAllocator<U, S, typename Allocator::template rebind<U>::other, Pool> other;
	};

// Construction
public:
	inline explicit SingletonPoolAllocator();
	inline SingletonPoolAllocator(const SingletonPoolAllocator<T> &second);
	template <typename W, unsigned X, typename Y, typename Z>
	inline SingletonPoolAllocator(const SingletonPoolAllocator<W, X, Y, Z> &second);
	virtual ~SingletonPoolAllocator();

// Operations
public:
	inline static pointer allocate();
	inline static void deallocate(pointer ptr);
	inline static void deallocate(pointer ptr, const size_type n);

	// Rilascia la memoria non utilizzata (nel caso di pool ad esempio), restitusce true se è stata deallocata della memoria
	inline static bool release_memory();
	// Rilascia tutta la memoria allocata invalidando eventuali puntatori allocati in precedenza, restitusce true se è stata deallocata della memoria
	inline static bool purge_memory();
};

//////////////////////////////////////////////////////////////////////

template <typename T, unsigned S, typename A, typename P>
inline SingletonPoolAllocator<T, S, A, P>::SingletonPoolAllocator()
{

}

template <typename T, unsigned S, typename A, typename P>
inline SingletonPoolAllocator<T, S, A, P>::SingletonPoolAllocator(const SingletonPoolAllocator<T> &second) : Base(second)
{

}

template <typename T, unsigned S, typename A, typename P>
template <typename W, unsigned X, typename Y, typename Z>
inline SingletonPoolAllocator<T, S, A, P>::SingletonPoolAllocator(const SingletonPoolAllocator<W, X, Y, Z> &second) : Base(second)
{

}

template <typename T, unsigned S, typename A, typename P>
SingletonPoolAllocator<T, S, A, P>::~SingletonPoolAllocator()
{

}

template <typename T, unsigned S, typename A, typename P>
inline typename SingletonPoolAllocator<T, S, A, P>::pointer SingletonPoolAllocator<T, S, A, P>::allocate()
{
	return static_cast<pointer>(Pool::malloc());
}

template <typename T, unsigned S, typename A, typename P>
inline void SingletonPoolAllocator<T, S, A, P>::deallocate(pointer ptr)
{
	Pool::free(ptr);
}

template <typename T, unsigned S, typename A, typename P>
inline void SingletonPoolAllocator<T, S, A, P>::deallocate(pointer ptr, const size_type n)
{
	Pool::free(ptr, n);
}

template <typename T, unsigned S, typename A, typename P>
inline bool SingletonPoolAllocator<T, S, A, P>::release_memory()
{
	return Pool::release_memory();
}

template <typename T, unsigned S, typename A, typename P>
inline bool SingletonPoolAllocator<T, S, A, P>::purge_memory()
{
	return Pool::purge_memory();
}

//////////////////////////////////////////////////////////////////////

// Implementazione non thread safe

template <typename T, typename A = BoostPoolAllocator<typename DefaultAllocator<T>::type>, typename P = boost::pool<A> >
class PoolAllocator : public BasicAllocator<T>
{
public:
	typedef BasicAllocator<T> Base;
	typedef typename Base::pointer pointer;
	typedef typename Base::size_type size_type;
	typedef A Allocator;
	typedef P Pool;

	template <typename U>
	struct rebind
	{
		typedef PoolAllocator<U, typename Allocator::template rebind<U>::other, Pool> other;
	};

// Construction
public:
	inline explicit PoolAllocator(size_type size);
	inline PoolAllocator(const PoolAllocator<T, A, P> &second);
	template <typename X, typename Y, typename Z>
	inline PoolAllocator(const PoolAllocator<X, Y, Z> &second);
	virtual ~PoolAllocator();

// Operations
public:
	inline pointer allocate();

	inline void deallocate(pointer ptr);
	inline void deallocate(pointer ptr, const size_type n);

	// Rilascia la memoria non utilizzata (nel caso di pool ad esempio), restitusce true se è stata deallocata della memoria
	inline bool release_memory();
	// Rilascia tutta la memoria allocata invalidando eventuali puntatori allocati in precedenza, restitusce true se è stata deallocata della memoria
	inline bool purge_memory();

protected:
	Pool m_pool;
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename A, typename P>
inline PoolAllocator<T, A, P>::PoolAllocator(size_type size) : m_pool(size)
{

}

template <typename T, typename A, typename P>
inline PoolAllocator<T, A, P>::PoolAllocator(const PoolAllocator<T, A, P> &second) : Base(second),
																					 m_pool(second.m_pool)
{

}

template <typename T, typename A, typename P>
template <typename X, typename Y, typename Z>
inline PoolAllocator<T, A, P>::PoolAllocator(const PoolAllocator<X, Y, Z> &second) : Base(second),
																					 m_pool(second.m_pool)
{

}

template <typename T, typename A, typename P>
PoolAllocator<T, A, P>::~PoolAllocator()
{

}

template <typename T, typename A, typename P>
inline typename PoolAllocator<T, A, P>::pointer PoolAllocator<T, A, P>::allocate()
{
	return static_cast<pointer>(m_pool.malloc());
}

template <typename T, typename A, typename P>
inline void PoolAllocator<T, A, P>::deallocate(pointer ptr)
{
	m_pool.free(ptr);
}

template <typename T, typename A, typename P>
inline void PoolAllocator<T, A, P>::deallocate(pointer ptr, const size_type n)
{
	m_pool.free(ptr, n);
}

template <typename T, typename A, typename P>
inline bool PoolAllocator<T, A, P>::release_memory()
{
	return m_pool.release_memory();
}

template <typename T, typename A, typename P>
inline bool PoolAllocator<T, A, P>::purge_memory()
{
	return m_pool.purge_memory();
}

//////////////////////////////////////////////////////////////////////

// Implementazione non thread safe

template <typename T, typename A = BoostPoolAllocator<typename DefaultAllocator<T>::type>, typename P = boost::object_pool<T, A> >
class ObjectPoolAllocator : public BasicAllocator<T>
{
public:
	typedef BasicAllocator<T> Base;
	typedef typename Base::pointer pointer;
	typedef typename Base::size_type size_type;
	typedef A Allocator;
	typedef P Pool;

	template <typename U>
	struct rebind
	{
		typedef ObjectPoolAllocator<U, typename Allocator::template rebind<U>::other, Pool> other;
	};

// Construction
public:
	inline explicit ObjectPoolAllocator();
	inline ObjectPoolAllocator(const ObjectPoolAllocator<T, A, P> &second);
	template <typename X, typename Y, typename Z>
	inline ObjectPoolAllocator(const ObjectPoolAllocator<X, Y, Z> &second);
	virtual ~ObjectPoolAllocator();

// Operations
public:
	inline pointer allocate();

	inline void deallocate(pointer ptr);
	inline void deallocate(pointer ptr, const size_type n);

	// Rilascia la memoria non utilizzata (nel caso di pool ad esempio), restitusce true se è stata deallocata della memoria
	inline bool release_memory();
	// Rilascia tutta la memoria allocata invalidando eventuali puntatori allocati in precedenza, restitusce true se è stata deallocata della memoria
	inline bool purge_memory();

protected:
	Pool m_pool;
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename A, typename P>
inline ObjectPoolAllocator<T, A, P>::ObjectPoolAllocator()
{

}

template <typename T, typename A, typename P>
inline ObjectPoolAllocator<T, A, P>::ObjectPoolAllocator(const ObjectPoolAllocator<T, A, P> &second) : Base(second),
																									   m_pool(second.m_pool)
{

}

template <typename T, typename A, typename P>
template <typename X, typename Y, typename Z>
inline ObjectPoolAllocator<T, A, P>::ObjectPoolAllocator(const ObjectPoolAllocator<X, Y, Z> &second) : Base(second),
																									   m_pool(second.m_pool)
{

}

template <typename T, typename A, typename P>
ObjectPoolAllocator<T, A, P>::~ObjectPoolAllocator()
{

}

template <typename T, typename A, typename P>
inline typename ObjectPoolAllocator<T, A, P>::pointer ObjectPoolAllocator<T, A, P>::allocate()
{
	return m_pool.malloc();
}

template <typename T, typename A, typename P>
inline void ObjectPoolAllocator<T, A, P>::deallocate(pointer ptr)
{
	m_pool.free(ptr);
}

template <typename T, typename A, typename P>
inline void ObjectPoolAllocator<T, A, P>::deallocate(pointer ptr, const size_type n)
{
	m_pool.free(ptr, n);
}

template <typename T, typename A, typename P>
inline bool ObjectPoolAllocator<T, A, P>::release_memory()
{
	return m_pool.release_memory();
}

template <typename T, typename A, typename P>
inline bool ObjectPoolAllocator<T, A, P>::purge_memory()
{
	return m_pool.purge_memory();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_POOLS_H
