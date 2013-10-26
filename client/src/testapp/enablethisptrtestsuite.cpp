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

#include "enable_this_ptr.h"
#include "boost/type_traits.hpp"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EnableThisPtrTester : public enable_this_ptr<EnableThisPtrTester>
{
public:
	EnableThisPtrTester()
	{

	}

	EnableThisPtrTester(shared_ptr<EnableThisPtrTester> &ptr)
	{
		ptr = get_this_ptr();
		BOOST_CHECK(ptr != null);
	}
};

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_enable_this_ptr)
{
	{
		shared_ptr<EnableThisPtrTester> ptr1;
		shared_ptr<EnableThisPtrTester> ptr2(new EnableThisPtrTester(ptr1));
		BOOST_CHECK(ptr1 == ptr2);
		BOOST_CHECK(ptr1.use_count() == ptr2.use_count());
		BOOST_CHECK(ptr1->get_this_ptr() == ptr1);
		BOOST_CHECK(ptr1->get_this_ptr().use_count() == ptr1.use_count());
		shared_ptr<EnableThisPtrTester> ptr3 = ptr2->get_this_ptr();
		BOOST_CHECK(ptr2 == ptr3);
		BOOST_CHECK(ptr2.use_count() == ptr3.use_count());
		BOOST_CHECK(ptr1->get_this_ptr().use_count() == ptr1.use_count());
	}

	{
		shared_ptr<EnableThisPtrTester> ptr1;
		EnableThisPtrTester tester(ptr1);
		shared_ptr<EnableThisPtrTester> ptr2 = tester.get_this_ptr();
		BOOST_CHECK(ptr1 == ptr2);
		BOOST_CHECK(ptr1.use_count() == ptr2.use_count());

		ptr1.reset();
		ptr2.reset();
	}

	{
		EnableThisPtrTester tester;

		{
			shared_ptr<EnableThisPtrTester> ptr1 = tester.get_this_ptr();
			shared_ptr<EnableThisPtrTester> ptr2 = tester.get_this_ptr();
			BOOST_CHECK(ptr1 == ptr2);
			BOOST_CHECK(ptr1.use_count() == ptr2.use_count());

			shared_ptr<EnableThisPtrTester> ptr3(&tester, null_deleter());		// Verifica il supporto di uno shared_ptr esterno

			BOOST_CHECK(ptr1 == ptr3);
			BOOST_CHECK(ptr1.use_count() == ptr3.use_count());					// Lo shared_ptr esterno deve condividere lo stesso reference counting

			shared_ptr<EnableThisPtrTester> ptr4 = tester.get_this_ptr();
			BOOST_CHECK(ptr4 == ptr1);
			BOOST_CHECK(ptr4 == ptr3);

			BOOST_CHECK(ptr1.use_count() == ptr4.use_count());
			BOOST_CHECK(ptr3.use_count() == ptr4.use_count());
		}

		BOOST_CHECK(tester.get_this_ptr() != null);			// Verifica che dopo la distruzione di tutti gli shared_ptr il weak_ptr interno venga reinizializzato
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
