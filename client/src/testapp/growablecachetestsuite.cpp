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

#include "growablecache.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_unlimited_growable_cache)
{
	GrowableCache<String> cache;
	BOOST_CHECK(cache.empty());
	BOOST_CHECK(cache.size() == 0);

	cache.peekEntry();
	BOOST_CHECK(cache.empty() == false);
	BOOST_CHECK(cache.size() == 1);

	cache.clear();
	BOOST_CHECK(cache.empty());
	BOOST_CHECK(cache.size() == 0);

	cache.peekEntry();
	cache.peekEntry();
	cache.peekEntry();
	BOOST_CHECK(cache.empty() == false);
	BOOST_CHECK(cache.size() == 1);

	shared_ptr<String> str1 = cache.peekEntry();
	BOOST_CHECK(cache.empty() == false);
	BOOST_CHECK(cache.size() == 1);

	str1 = cache.peekEntry();	// prima viene richiamato peekEntry e solo in seguito il "vecchio" str1 viene rilasciato, pertanto qui la cache cresce a 2
	BOOST_CHECK(cache.empty() == false);
	BOOST_CHECK(cache.size() == 2);

	shared_ptr<String> str2 = cache.peekEntry();
	BOOST_CHECK(cache.empty() == false);
	BOOST_CHECK(cache.size() == 2);
	BOOST_CHECK(str1 != str2);

	String *raw_str1 = str1.get();
	str1.reset();

	shared_ptr<String> str3 = cache.peekEntry();
	BOOST_CHECK(cache.size() == 2);
	BOOST_CHECK(str2 != str3);
	BOOST_CHECK(str3.get() == raw_str1);

	str1 = cache.peekEntry();
	BOOST_CHECK(cache.size() == 3);
	BOOST_CHECK(str1 != str2);
	BOOST_CHECK(str1 != str3);
	BOOST_CHECK(str2 != str3);

	str1.reset();
	String *raw_str2 = str2.get();
	str2.reset();

	shared_ptr<String> str4 = cache.peekEntry();
	BOOST_CHECK(cache.size() == 3);
	BOOST_CHECK(str3 != str4);
	BOOST_CHECK(str4.get() == raw_str2);
}

BOOST_AUTO_TEST_CASE(test_limited_growable_cache)
{
	{
		GrowableCache<String> cache1(static_cast<uint32>(1));
		BOOST_CHECK(cache1.empty());
		cache1.peekEntry();
		cache1.peekEntry();
		cache1.peekEntry();
		BOOST_CHECK(cache1.empty() == false);
		BOOST_CHECK(cache1.size() == 1);

		shared_ptr<String> str1 = cache1.peekEntry();
		shared_ptr<String> str2 = cache1.peekEntry();
		shared_ptr<String> str3 = cache1.peekEntry();
		BOOST_CHECK(cache1.empty() == false);
		BOOST_CHECK(cache1.size() == 3);

		str1.reset();
		str2.reset();
		shared_ptr<String> str4 = cache1.peekEntry();
		BOOST_CHECK(cache1.size() == 2);

		str3.reset();
		str4.reset();

		shared_ptr<String> str5 = cache1.peekEntry();
		BOOST_CHECK(cache1.size() == 1);
	}

	{
		GrowableCache<String> cache2(static_cast<uint32>(2));
		BOOST_CHECK(cache2.empty());
		cache2.peekEntry();
		cache2.peekEntry();
		cache2.peekEntry();
		BOOST_CHECK(cache2.empty() == false);
		BOOST_CHECK(cache2.size() == 1);

		shared_ptr<String> str1 = cache2.peekEntry();
		shared_ptr<String> str2 = cache2.peekEntry();
		shared_ptr<String> str3 = cache2.peekEntry();
		BOOST_CHECK(cache2.empty() == false);
		BOOST_CHECK(cache2.size() == 3);

		str1.reset();
		str2.reset();
		shared_ptr<String> str4 = cache2.peekEntry();
		BOOST_CHECK(cache2.size() == 2);

		str3.reset();
		str4.reset();

		shared_ptr<String> str5 = cache2.peekEntry();
		BOOST_CHECK(cache2.size() == 2);
	}

	{
		GrowableCache<String> cache3(static_cast<uint32>(3));
		BOOST_CHECK(cache3.empty());
		cache3.peekEntry();
		cache3.peekEntry();
		cache3.peekEntry();
		BOOST_CHECK(cache3.empty() == false);
		BOOST_CHECK(cache3.size() == 1);

		shared_ptr<String> str1 = cache3.peekEntry();
		shared_ptr<String> str2 = cache3.peekEntry();
		shared_ptr<String> str3 = cache3.peekEntry();
		BOOST_CHECK(cache3.empty() == false);
		BOOST_CHECK(cache3.size() == 3);

		str1.reset();
		str2.reset();
		shared_ptr<String> str4 = cache3.peekEntry();
		BOOST_CHECK(cache3.size() == 3);

		str3.reset();
		str4.reset();

		shared_ptr<String> str5 = cache3.peekEntry();
		BOOST_CHECK(cache3.size() == 3);
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
