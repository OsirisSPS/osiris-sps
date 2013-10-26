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

#include "lock.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/recursive_mutex.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static bool get_set_true(bool &value)
{
	value = true;
	return true;
}

BOOST_AUTO_TEST_CASE(test_macros_verify)
{
	bool value = false;
	OS_VERIFY(get_set_true(value));
	BOOST_CHECK(value);
}

BOOST_AUTO_TEST_CASE(test_macros_assert)
{
	bool value = false;
	OS_ASSERT(get_set_true(value));
#ifdef OS_DEBUG
	BOOST_CHECK(value);
#else
	BOOST_CHECK(value == false);
#endif
}

BOOST_AUTO_TEST_CASE(test_macros_make_values)
{
	uint8 vuint8 = OS_MAKE_UINT8(5, 2);
	BOOST_CHECK(vuint8 == 37);
	BOOST_CHECK(OS_UINT8_LO(vuint8) == 5);
	BOOST_CHECK(OS_UINT8_HI(vuint8) == 2);

	uint16 vuint16 = OS_MAKE_UINT16(5, 2);
	BOOST_CHECK(vuint16 == 517);
	BOOST_CHECK(OS_UINT16_LO(vuint16) == 5);
	BOOST_CHECK(OS_UINT16_HI(vuint16) == 2);
	
	uint32 vuint32 = OS_MAKE_UINT32(5, 2);
	BOOST_CHECK(vuint32 == 131077);
	BOOST_CHECK(OS_UINT32_LO(vuint32) == 5);
	BOOST_CHECK(OS_UINT32_HI(vuint32) == 2);

	uint64 vuint64 = OS_MAKE_UINT64(5, 2);
	BOOST_CHECK(vuint64 == 8589934597LL);
	BOOST_CHECK(OS_UINT64_LO(vuint64) == 5);
	BOOST_CHECK(OS_UINT64_HI(vuint64) == 2);
}

BOOST_AUTO_TEST_CASE(test_macros_utils)
{
	byte buffer[5];
	BOOST_CHECK(OS_COUNTOF(buffer) == 5);

	uint32 value = 5;
	OS_ZEROMEMORY(&value, sizeof(uint32));
	BOOST_CHECK(value == 0);
}

BOOST_AUTO_TEST_CASE(test_macros_lock)
{
	{
		boost::mutex m;

		{
			BOOST_CHECK(m.try_lock());
			m.unlock();
			OS_LOCK(m);
			BOOST_CHECK(m.try_lock() == false);
		}

		{
			BOOST_CHECK(m.try_lock());
			m.unlock();
			OS_LOCK(m);
			BOOST_CHECK(m.try_lock() == false);
		}
	}

	{
		boost::recursive_mutex m;

		{
			BOOST_CHECK(m.try_lock());
			OS_LOCK(m);
			BOOST_CHECK(m.try_lock());

			m.unlock();
			m.unlock();			
		}

		{
			BOOST_CHECK(m.try_lock());
			OS_LOCK(m);
			BOOST_CHECK(m.try_lock());

			m.unlock();
			m.unlock();
		}
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
