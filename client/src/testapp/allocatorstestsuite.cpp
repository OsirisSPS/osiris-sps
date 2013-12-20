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

#include "allocators.h"
#include "allocwrapper.h"
#if OS_ALLOCATOR_TYPE == OS_ALLOCATOR_TYPE_NED
    #include "nedallocator.h"
#endif
#include "sysallocator.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_allocators_macros)
{
	void *p = OS_ALLOCATE(100);
	BOOST_CHECK(p != nullptr);

	p = OS_REALLOCATE(p, 200);
	BOOST_CHECK(p != nullptr);

	OS_DEALLOCATE(p);
	p = nullptr;

	OS_DEALLOCATE(nullptr);

	p = OS_REALLOCATE(p, 300);
	BOOST_CHECK(p != nullptr);

	OS_DEALLOCATE(p);
}

template <typename T>
static void test_allocator_type()
{
	void *p = T::allocate(100);
	BOOST_CHECK(p != nullptr);

	p = T::reallocate(p, 200);
	BOOST_CHECK(p != nullptr);

	T::deallocate(p);
	p = nullptr;

	T::deallocate(nullptr);

	p = T::reallocate(p, 300);
	BOOST_CHECK(p != nullptr);

	T::deallocate(p);
}

BOOST_AUTO_TEST_CASE(test_allocators_sys)
{
	test_allocator_type<SysAllocator<byte> >();
}

#if OS_ALLOCATOR_TYPE == OS_ALLOCATOR_TYPE_NED
BOOST_AUTO_TEST_CASE(test_allocators_ned)
{
	test_allocator_type<NedAllocator<byte> >();
}
#endif

class __AllocWrapperClass__ : public AllocWrapper<__AllocWrapperClass__>
{
public:
	__AllocWrapperClass__()
	{
		instances++;	
	}

	virtual ~__AllocWrapperClass__()
	{
		OS_ASSERT(instances > 0);
		instances--;
	}

public:
	static uint32 instances;
};

uint32 __AllocWrapperClass__::instances = 0;

class __NoAllocWrapperClass__
{
public:
	__NoAllocWrapperClass__()
	{
		instances++;	
	}

	virtual ~__NoAllocWrapperClass__()
	{
		OS_ASSERT(instances > 0);
		instances--;
	}

public:
	static uint32 instances;
};

uint32 __NoAllocWrapperClass__::instances = 0;

template <typename T>
void test_allocators_wrappers_t()
{
	OS_ASSERT(T::instances == 0);

	for(uint32 i = 0; i < 10; i++)
	{
		T *p = OS_NEW_T(T);

		BOOST_CHECK(T::instances == 1);

		OS_DELETE_T(p);
	}

	BOOST_CHECK(T::instances == 0);

	T *p = OS_NEW_ARRAY_T(T, 3);

	BOOST_CHECK(T::instances == 3);

	OS_DELETE_ARRAY_T(p, 3);

	BOOST_CHECK(T::instances == 0);

	shared_array<T> a(OS_NEW_ARRAY_T(T, 9), os_delete_array_t(9));

	BOOST_CHECK(T::instances == 9);

	a.reset();

	BOOST_CHECK(T::instances == 0);
}

BOOST_AUTO_TEST_CASE(test_allocators_wrappers)
{
	for(uint32 i = 0; i < 10; i++)
	{
		__AllocWrapperClass__ *p = OS_NEW __AllocWrapperClass__();

		BOOST_CHECK(__AllocWrapperClass__::instances == 1);

		OS_DELETE p;
	}

	BOOST_CHECK(__AllocWrapperClass__::instances == 0);

	__AllocWrapperClass__ *p = OS_NEW __AllocWrapperClass__[5];

	BOOST_CHECK(__AllocWrapperClass__::instances == 5);

	OS_DELETE [] p;

	BOOST_CHECK(__AllocWrapperClass__::instances == 0);

	shared_array<__AllocWrapperClass__> a(OS_NEW __AllocWrapperClass__[7], os_delete_array());

	BOOST_CHECK(__AllocWrapperClass__::instances == 7);

	a.reset();

	BOOST_CHECK(__AllocWrapperClass__::instances == 0);

	test_allocators_wrappers_t<__AllocWrapperClass__>();	// Supporto AllocWrapper OS_NEW_T/OS_DELETE_T e OS_NEW_ARRAY_T/OS_DELETE_ARRAY_T
	test_allocators_wrappers_t<__NoAllocWrapperClass__>();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
