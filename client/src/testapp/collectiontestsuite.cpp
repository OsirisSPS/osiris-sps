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

#include "collections.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
void test_collections(T &collection)
{
	BOOST_CHECK(collection.empty());

	collection.push_back(1, typename T::type_ptr(new String("alfa")));
	collection.push_back(2, typename T::type_ptr(new String("beta")));
	collection.push_back(3, typename T::type_ptr(new String("gamma")));

	BOOST_CHECK(collection.empty() == false);
	BOOST_CHECK(collection.size() == 3);

	BOOST_CHECK(collection.begin()->first == 1);
	BOOST_CHECK(*collection.begin()->second == _S("alfa"));

	BOOST_CHECK(*collection.front() == _S("alfa"));
	BOOST_CHECK(*collection.back() == _S("gamma"));

	BOOST_CHECK(collection.key_at(0) == 1);
	BOOST_CHECK(collection.key_at(1) == 2);
	BOOST_CHECK(collection.key_at(2) == 3);

	BOOST_CHECK(*collection.value_at(0) == _S("alfa"));
	BOOST_CHECK(*collection.value_at(1) == _S("beta"));
	BOOST_CHECK(*collection.value_at(2) == _S("gamma"));

	collection.pop_front();

	BOOST_CHECK(*collection.front() == _S("beta"));
	BOOST_CHECK(collection.size() == 2);

	collection.pop_back();

	BOOST_CHECK(*collection.front() == _S("beta"));
	BOOST_CHECK(collection.size() == 1);

	BOOST_CHECK(collection.begin()->first == 2);
	BOOST_CHECK(*collection.begin()->second == _S("beta"));

	collection.clear();

	BOOST_CHECK(collection.empty());
	BOOST_CHECK(collection.size() == 0);

	collection.push_back(1, typename T::type_ptr(new String("alfa")));
	collection.push_back(2, typename T::type_ptr(new String("beta")));
	collection.push_back(3, typename T::type_ptr(new String("gamma")));
	collection.push_back(4, typename T::type_ptr(new String("delta")));

	BOOST_CHECK(collection.exists(3));
	BOOST_CHECK(collection.exists(5) == false);
	BOOST_CHECK(collection.get(4) != nullptr);

	collection.push_front(0, typename T::type_ptr(new String("foo")));
	BOOST_CHECK(*collection.front() == _S("foo"));
	BOOST_CHECK(collection.key_at(0) == 0);
	BOOST_CHECK(*collection.value_at(0) == _S("foo"));

	collection.remove(0);
	BOOST_CHECK(collection.size() == 4);
	BOOST_CHECK(*collection.front() == _S("alfa"));
	BOOST_CHECK(collection.key_at(0) == 1);
	BOOST_CHECK(*collection.value_at(0) == _S("alfa"));
	BOOST_CHECK(*collection[1] == _S("alfa"));
	BOOST_CHECK(collection[0] == nullptr);
}

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_collections_smart)
{
	Collections<int, String>::smart_collection collection;
	test_collections(collection);
}

BOOST_AUTO_TEST_CASE(test_collections_shared)
{
	Collections<int, String>::shared_collection collection;
	test_collections(collection);	
}

BOOST_AUTO_TEST_CASE(test_collections_string)
{
	{
		StringCollection<std::string> s;
		BOOST_CHECK(s.empty());
		BOOST_CHECK(s.get("foo") == "");
		s.set("foo", "bar");
		BOOST_CHECK(s.size() == 1);
		BOOST_CHECK(s.get("foo") == "bar");
		std::string senc;
		s.encode(senc, ";", "=");
		BOOST_CHECK(senc == "foo=bar");
		s.clear();
		s.decode(senc, ";", "=");
		BOOST_CHECK(s.size() == 1);
		BOOST_CHECK(s.get("foo") == "bar");
		StringCollection<std::string> sc(s);
		BOOST_CHECK(sc.size() == 1);
		BOOST_CHECK(sc.get("foo") == "bar");
	}

	{
		StringCollection<std::wstring> s;
		BOOST_CHECK(s.empty());
		BOOST_CHECK(s.get(L"foo") == L"");
		s.set(L"foo", L"bar");
		BOOST_CHECK(s.size() == 1);
		BOOST_CHECK(s.get(L"foo") == L"bar");
		std::wstring senc;
		s.encode(senc, L";", L"=");
		BOOST_CHECK(senc == L"foo=bar");
		s.clear();
		s.decode(senc, L";", L"=");
		BOOST_CHECK(s.size() == 1);
		BOOST_CHECK(s.get(L"foo") == L"bar");
		StringCollection<std::wstring> sc(s);
		BOOST_CHECK(sc.size() == 1);
		BOOST_CHECK(sc.get(L"foo") == L"bar");
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
