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

#ifndef _OS_CORE_ALLOCWRAPPER_H
#define _OS_CORE_ALLOCWRAPPER_H

#include "allocators.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, typename A = typename DefaultAllocator<T>::type>
class AllocWrapper
{
public:
	typedef A allocator;

// Construction
public:
	AllocWrapper();
	AllocWrapper(const AllocWrapper<T, A> &second);
	virtual ~AllocWrapper();
	
// Operators
public:
	void * operator new(size_t size);
	void * operator new(size_t size, const char *file);	
	void * operator new(size_t size, const char *file, int line);	
	void * operator new(size_t size, const char *file, int line, const char *function);	
	void * operator new(size_t size, void *ptr);		// Placement operator new
	void * operator new[](size_t size);
	void * operator new[](size_t size, const char *file);
	void * operator new[](size_t size, const char *file, int line);
	void * operator new[](size_t size, const char *file, int line, const char *function);

	void operator delete(void *ptr);
	void operator delete(void *ptr, const char *file);
	void operator delete(void *ptr, const char *file, int line);
	void operator delete(void *ptr, const char *file, int line, const char *function);
	void operator delete(void *ptr, void *p);			// Placement operator delete
	void operator delete[](void *ptr);
	void operator delete[](void *ptr, const char *file);
	void operator delete[](void *ptr, const char *file, int line);
	void operator delete[](void *ptr, const char *file, int line, const char *function);
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename A>
AllocWrapper<T, A>::AllocWrapper()
{
	// Nel caso di variabile statica di una classe derivata da AllocWrapper, assicura che il gestore della memoria venga preistanziato per garantire che venga distrutto in seguito a quest'ultima	
#ifdef OS_ENABLE_MEMORY_TRACKER
	MemoryTracker::instance();
#endif // OS_ENABLE_MEMORY_TRACKER
}

template <typename T, typename A>
AllocWrapper<T, A>::AllocWrapper(const AllocWrapper<T, A> &second)
{
	// Nel caso di variabile statica di una classe derivata da AllocWrapper, assicura che il gestore della memoria venga preistanziato per garantire che venga distrutto in seguito a quest'ultima	
#ifdef OS_ENABLE_MEMORY_TRACKER
	MemoryTracker::instance();
#endif // OS_ENABLE_MEMORY_TRACKER
}

template <typename T, typename A>
AllocWrapper<T, A>::~AllocWrapper()
{

}

template <typename T, typename A>
void * AllocWrapper<T, A>::operator new(size_t size)
{
	return allocator::allocate(size);
}

template <typename T, typename A>
void * AllocWrapper<T, A>::operator new(size_t size, const char *file)
{
	return allocator::allocate(size, file);
}

template <typename T, typename A>
void * AllocWrapper<T, A>::operator new(size_t size, const char *file, int line)
{
	return allocator::allocate(size, file, line);
}

template <typename T, typename A>
void * AllocWrapper<T, A>::operator new(size_t size, const char *file, int line, const char *function)
{
	return allocator::allocate(size, file, line, function);
}

template <typename T, typename A>
void * AllocWrapper<T, A>::operator new(size_t size, void *ptr)
{
	return ptr;
}

template <typename T, typename A>
void * AllocWrapper<T, A>::operator new[](size_t size)
{
	return allocator::allocate(size);
}

template <typename T, typename A>
void * AllocWrapper<T, A>::operator new[](size_t size, const char *file)
{
	return allocator::allocate(size, file);
}

template <typename T, typename A>
void * AllocWrapper<T, A>::operator new[](size_t size, const char *file, int line)
{
	return allocator::allocate(size, file, line);
}

template <typename T, typename A>
void * AllocWrapper<T, A>::operator new[](size_t size, const char *file, int line, const char *function)
{
	return allocator::allocate(size, file, line, function);
}

template <typename T, typename A>
void AllocWrapper<T, A>::operator delete(void *ptr)
{
	allocator::deallocate(static_cast<typename allocator::pointer>(ptr));
}

template <typename T, typename A>
void AllocWrapper<T, A>::operator delete(void *ptr, const char *file)
{
	allocator::deallocate(ptr);
}

template <typename T, typename A>
void AllocWrapper<T, A>::operator delete(void *ptr, const char *file, int line)
{
	allocator::deallocate(ptr);
}

template <typename T, typename A>
void AllocWrapper<T, A>::operator delete(void *ptr, const char *file, int line, const char *function)
{
	allocator::deallocate(ptr);
}

template <typename T, typename A>
void AllocWrapper<T, A>::operator delete(void *ptr, void *p)
{
	OS_ASSERT(ptr == p);
	allocator::deallocate(ptr);
}

template <typename T, typename A>
void AllocWrapper<T, A>::operator delete[](void *ptr)
{
	allocator::deallocate(ptr);
}

template <typename T, typename A>
void AllocWrapper<T, A>::operator delete[](void *ptr, const char *file)
{
	allocator::deallocate(ptr);
}

template <typename T, typename A>
void AllocWrapper<T, A>::operator delete[](void *ptr, const char *file, int line)
{
	allocator::deallocate(ptr);
}

template <typename T, typename A>
void AllocWrapper<T, A>::operator delete[](void *ptr, const char *file, int line, const char *function)
{
	allocator::deallocate(ptr);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ALLOCWRAPPER_H
