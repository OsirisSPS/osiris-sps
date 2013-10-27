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

#ifndef _OS_CORE_ALLOCATORS_H
#define _OS_CORE_ALLOCATORS_H

#include "typetraits.h"

//////////////////////////////////////////////////////////////////////

// OS_ALLOCATE(n):		da utilizzare per una gestione "raw" della memoria. Restituisce n bytes da deallocare tramite OS_DEALLOCATE e riallocare tramite OS_REALLOCATE
// OS_ALLOCATE_T(T, n):	da utilizzare per allocare tipi POD (int, float, double, ecc...). Restituisce n elementi di tipo T da deallocare tramite OS_DEALLOCATE_T e riallocare tramite OS_REALLOCATE_T
// OS_NEW:				da utilizzare per classi/strutture derivate da AllocWrapper, deallocare tramite OS_DELETE. Supporta l'allocazione di array tramite OS_NEW <Type>[n] che vanno deallocati tramite OS_DELETE [] <ptr>
// OS_NEW_T:			da utilizzare per classi/strutture non derivate da AllocWrapper, deallocare tramite OS_DELETE_T. Per passare parametri al costruttore usare: OS_NEW_T(<type>)(p1, p2, ...)
// OS_NEW_ARRAY_T:		da utilizzare per allocare array di classi/strutture non derivate da AllocWrapper, deallocare tramite OS_DELETE_ARRAY_T

//////////////////////////////////////////////////////////////////////

// Wrappers alle funzioni di gestione della memoria per tracciare in debug i files, linee e funzioni in cui vengono invocate

#ifdef OS_DEBUG

	#define OS_ALLOCATE(n)						OS_NAMESPACE_NAME::allocate(n, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)
	#define OS_DEALLOCATE(p)					OS_NAMESPACE_NAME::deallocate(p, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)
	#define OS_REALLOCATE(p, n)					OS_NAMESPACE_NAME::reallocate(p, n, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)
	#define OS_ALLOCATE_T(T, n)					static_cast<T *>(OS_NAMESPACE_NAME::allocate(sizeof(T) * (n), OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION))
	#define OS_DEALLOCATE_T(p)					OS_DEALLOCATE(p)
	#define OS_REALLOCATE_T(p, T, n)			static_cast<T *>(OS_NAMESPACE_NAME::reallocate(p, sizeof(T) * (n), OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION))	

	#define OS_NEW								new (OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)
	#define OS_DELETE							delete

	#define OS_NEW_T(T)							new (OS_NAMESPACE_NAME::allocate(sizeof(T), OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)) T
	#define OS_NEW_ARRAY_T(T, n)				OS_NAMESPACE_NAME::construct_n(static_cast<T *>(OS_NAMESPACE_NAME::allocate(sizeof(T) * (n), OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION)), n) 
	
	#define OS_DELETE_T(p)						OS_NAMESPACE_NAME::destruct(p)
	#define OS_DELETE_ARRAY_T(p, n)				OS_NAMESPACE_NAME::destruct_n(p, n)

#else

	#define OS_ALLOCATE(n)						OS_NAMESPACE_NAME::allocate(n)
	#define OS_DEALLOCATE(p)					OS_NAMESPACE_NAME::deallocate(p)
	#define OS_REALLOCATE(p, n)					OS_NAMESPACE_NAME::reallocate(p, n)
	#define OS_ALLOCATE_T(T, n)					static_cast<T *>(OS_NAMESPACE_NAME::allocate(sizeof(T) * (n)))
	#define OS_DEALLOCATE_T(p)					OS_DEALLOCATE(p)
	#define OS_REALLOCATE_T(p, T, n)			static_cast<T *>(OS_NAMESPACE_NAME::reallocate(p, sizeof(T) * (n)))

	#define OS_NEW								new
	#define OS_DELETE							delete

	#define OS_NEW_T(T)							new (OS_NAMESPACE_NAME::allocate(sizeof(T))) T
	#define OS_NEW_ARRAY_T(T, n)				OS_NAMESPACE_NAME::construct_n(static_cast<T *>(OS_NAMESPACE_NAME::allocate(sizeof(T) * (n))), n) 

	#define OS_DELETE_T(p)						OS_NAMESPACE_NAME::destruct(p)
	#define OS_DELETE_ARRAY_T(p, n)				OS_NAMESPACE_NAME::destruct_n(p, n)

#endif // OS_DEBUG

// OS_PLACEMENT_NEW non ha una corrispettiva OS_PLACEMENT_DELETE (avviene solitamente in modo manuale o tramite funzioni di appoggio)
#define OS_PLACEMENT_NEW(p, T)					new (p) T

//////////////////////////////////////////////////////////////////////

// Sotto windows usa come allocatore di default nedmalloc, altrimenti la versione di sistema

#define OS_ALLOCATOR_TYPE_SYS		0
#define OS_ALLOCATOR_TYPE_NED		1

#if OS_PLATFORM == OS_PLATFORM_WIN
	#include "nedallocator.h"
	#define OS_DEFAULT_ALLOCATOR NedAllocator
	#define OS_ALLOCATOR_TYPE OS_ALLOCATOR_TYPE_NED
#else
	#include "sysallocator.h"
	#define OS_DEFAULT_ALLOCATOR SysAllocator
	#define OS_ALLOCATOR_TYPE OS_ALLOCATOR_TYPE_SYS
#endif

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T = byte>
struct DefaultAllocator
{
	typedef OS_DEFAULT_ALLOCATOR<T> type;
};

//////////////////////////////////////////////////////////////////////

// Allocazione generica (usa l'allocatore di default)

inline void * allocate(std::size_t size, const char *file = 0, int line = 0, const char *function = 0)
{
	return DefaultAllocator<>::type::allocate(size, file, line, function);
}

// Deallocazione generica (usa l'allocatore di default)

inline void deallocate(void *ptr, const char *file = 0, int line = 0, const char *function = 0)
{
	DefaultAllocator<>::type::deallocate(ptr, file, line, function);
}

// Riallocazione generica (usa l'allocatore di default)

inline void * reallocate(void *ptr, std::size_t size, const char *file = 0, int line = 0, const char *function = 0)
{
	return DefaultAllocator<>::type::reallocate(ptr, size, file, line, function);
}

//////////////////////////////////////////////////////////////////////

// Richiama i costruttori di un array di oggetti tramite new, senza utilizzare new[] (che richiede memoria extra e pertanto non indicata per un allocatore custom)
template <typename T>
inline T * construct_n(T *p, std::size_t n)
{
	for(std::size_t i = 0; i < n; ++i)
	{
		new ((void *)(p + i)) T();
	}

	return p;
}

template <typename T>
inline void destruct(T *p) 
{
	if(p) 
	{ 
		(p)->~T(); 
		deallocate(p); 
	}
}

template <typename T>
inline void destruct_n(T *p, std::size_t n) 
{
	for(std::size_t i = 0; i < n; ++i) 
	{ 
		(p)[i].~T(); 
	} 
	
	deallocate(p);
}

//////////////////////////////////////////////////////////////////////

// N.B.: OS_DELETE_T e OS_DELETE_ARRAY_T non necessitano di una versione "checked" in quanto invocano direttamente ~T() e quindi producono errore di compilazione in caso di tipo incompleto

template <typename T>
inline void os_checked_delete(T *x)
{
    OS_STATIC_ASSERT(is_complete<T>::value);
    OS_DELETE x;
}

template <typename T> 
inline void os_checked_delete_array(T *x)
{
    OS_STATIC_ASSERT(is_complete<T>::value);
    OS_DELETE [] x;
}

//////////////////////////////////////////////////////////////////////

// Supporto shared_array OS_ALLOCATE
// es.: shared_array<byte> f(OS_ALLOCATE(n), os_deallocate());

struct os_deallocate
{
	template <typename T>
	inline void operator()(T *p) const 
	{
		OS_DEALLOCATE(p);
	}
};

// Supporto shared_array OS_ALLOCATE_T
// es.: shared_array<int> f(OS_ALLOCATE_T(int, n), os_deallocate_t());

struct os_deallocate_t
{
	template <typename T>
	inline void operator()(T *p) const 
	{
		OS_DEALLOCATE_T(p);
	}
};

// Supporto shared_ptr OS_NEW 
// es.: shared_ptr<Foo> f(OS_NEW Foo(), os_delete())
// N.B.: è equivalente a "shared_ptr<Foo> f(OS_NEW Foo())" in quanto OS_DELETE è una define di "delete"

struct os_delete
{
	template <typename T>
	inline void operator()(T *p) const 
	{
		os_checked_delete(p);
	}
};

// Supporto shared_array OS_NEW[n]
// es.: shared_array<Foo> f(OS_NEW Foo[n], os_delete_array())

struct os_delete_array
{
	template <typename T>
	inline void operator()(T *p) const 
	{
		os_checked_delete_array(p);
	}
};

// Supporto shared_ptr OS_NEW_T
// es.: shared_ptr<Foo> f(OS_NEW_T(Foo), os_delete_t())

struct os_delete_t
{
	template <typename T>
	inline void operator()(T *p) const 
	{
		OS_DELETE_T(p);
	}
};

// Supporto shared_array OS_NEW_ARRAY_T
// es.: shared_array<Foo> f(OS_NEW_ARRAY_T(Foo, n), os_delete_array_t(n))

struct os_delete_array_t
{
	inline os_delete_array_t(std::size_t count) : m_count(count)
	{

	}

	template <typename T>
	inline void operator()(T *p) const 
	{
		OS_DELETE_ARRAY_T(p, m_count);
	}

	std::size_t m_count;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ALLOCATORS_H
