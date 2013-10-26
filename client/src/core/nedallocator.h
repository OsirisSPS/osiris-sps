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

#ifndef _OS_CORE_NEDALLOCATOR_H
#define _OS_CORE_NEDALLOCATOR_H

#include "basicallocator.h"
#include "memorytracker.h"
#include "ned.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
class NedAllocator : public BasicAllocator<T>
{
public:
	typedef BasicAllocator<T> Base;
	typedef typename Base::size_type size_type;

	template <typename U>
	struct rebind
	{
		typedef NedAllocator<U> other;
	};

// Construction
public:
	inline explicit NedAllocator();
	inline NedAllocator(const NedAllocator<T> &second);
	template <typename U>
	inline NedAllocator(const NedAllocator<U> &second);
	virtual ~NedAllocator();

// Operations
public:
	inline static void * allocate(const size_type size, const char *file = 0, int line = 0, const char *function = 0);
	inline static void deallocate(void *ptr, const char *file = 0, int line = 0, const char *function = 0);
	inline static void * reallocate(void *ptr, const size_type size, const char *file = 0, int line = 0, const char *function = 0);
};

//////////////////////////////////////////////////////////////////////

template <typename T>
inline NedAllocator<T>::NedAllocator()
{

}

template <typename T>
inline NedAllocator<T>::NedAllocator(const NedAllocator<T> &second) : Base(second)
{

}

template <typename T>
template <typename U>
inline NedAllocator<T>::NedAllocator(const NedAllocator<U> &second) : Base(second)
{

}

template <typename T>
NedAllocator<T>::~NedAllocator()
{

}

template <typename T>
inline void * NedAllocator<T>::allocate(const size_type size, const char *file, int line, const char *function)
{
#ifdef OS_ENABLE_MEMORY_TRACKER
	shared_ptr<MemoryTracker::IScope> memoryLocker = MemoryTracker::instance()->lock();
#endif // OS_ENABLE_MEMORY_TRACKER

	void *ptr = ned_allocate(size);

#ifdef OS_ENABLE_MEMORY_TRACKER
	memoryLocker->trackAllocation(ptr, size, file, line, function);
#endif // OS_ENABLE_MEMORY_TRACKER

	return ptr;
}

template <typename T>
inline void NedAllocator<T>::deallocate(void *ptr, const char *file, int line, const char *function)
{
#ifdef OS_ENABLE_MEMORY_TRACKER
	shared_ptr<MemoryTracker::IScope> memoryLocker = MemoryTracker::instance()->lock();
	memoryLocker->trackDeallocation(ptr, file, line, function);
#endif // OS_ENABLE_MEMORY_TRACKER

	ned_deallocate(ptr);
}

template <typename T>
inline void * NedAllocator<T>::reallocate(void *ptr, const size_type size, const char *file, int line, const char *function)
{
#ifdef OS_ENABLE_MEMORY_TRACKER
	shared_ptr<MemoryTracker::IScope> memoryLocker = MemoryTracker::instance()->lock();
	void *prev = ptr;
#endif

	ptr = ned_reallocate(ptr, size);

#ifdef OS_ENABLE_MEMORY_TRACKER
	memoryLocker->trackReallocation(prev, ptr, size, file, line, function);
#endif // OS_ENABLE_MEMORY_TRACKER

	return ptr;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_NEDALLOCATOR_H
