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

#include "ordered_map.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
void test_empty_ordered_map(T &c)
{
	BOOST_CHECK(c.empty());
	BOOST_CHECK(c.begin() == c.end());
	BOOST_CHECK(c.size() == 0);
	BOOST_CHECK(c == T());
	BOOST_CHECK((c != T()) == false);
}

BOOST_AUTO_TEST_CASE(test_ordered_map_construction)
{
	ordered_map<int32, int32> c;
	test_empty_ordered_map(c);
}

BOOST_AUTO_TEST_CASE(test_ordered_map_get_set)
{
	ordered_map<int32, std::string> c;
	BOOST_CHECK(c.size() == 0);
	BOOST_CHECK(c.get(0).empty());
	BOOST_CHECK(c.size() == 1);
	BOOST_CHECK(c.begin() != c.end());
	BOOST_CHECK(c.get(1).empty());
	BOOST_CHECK(c.size() == 2);
	BOOST_CHECK(c.set(1, "first"));
	BOOST_CHECK(c.size() == 2);
	BOOST_CHECK(c.get(1) == "first");
	BOOST_CHECK(c[1] == "first");
	c[1] = "foo";
	BOOST_CHECK(c.get(1) == "foo");
	BOOST_CHECK(c.size() == 2);
	c[1].clear();
	BOOST_CHECK(c.get(1).empty());
	BOOST_CHECK(c.size() == 2);	
}

BOOST_AUTO_TEST_CASE(test_ordered_map_operations)
{
	ordered_map<int32, std::string> c;
	std::pair<ordered_map<int32, std::string>::iterator, bool> result1 = c.push_back(2, "second");
	BOOST_CHECK(result1.first != c.end());
	BOOST_CHECK(result1.second);	
	std::pair<ordered_map<int32, std::string>::iterator, bool> result2 = c.push_front(2, "front");
	BOOST_CHECK(result2.first == result1.first);		// Inserimento fallito, restituisce la posizione precedente
	BOOST_CHECK(result2.second == false);
	std::pair<ordered_map<int32, std::string>::iterator, bool> result3 = c.push_back(2, "back");
	BOOST_CHECK(result3.first == result1.first);		// Inserimento fallito, restituisce la posizione precedente
	BOOST_CHECK(result3.second == false);
	BOOST_CHECK(c.find(2) != c.end());
	BOOST_CHECK(c.size() == 1);
	ordered_map<int32, std::string>::iterator pos = c.push_front(1, "first").first;
	BOOST_CHECK(pos != c.end());
	BOOST_CHECK(c.push_front(1, "front").first == pos);		// Inserimento fallito, restituisce la posizione precedente
	BOOST_CHECK(c.push_back(1, "back").first == pos);		// Inserimento fallito, restituisce la posizione precedente
	BOOST_CHECK(c.find(2) != c.end());
	BOOST_CHECK(c.size() == 2);
	BOOST_CHECK(c.find(1)->first == 1);	
	BOOST_CHECK(c.find(1)->second == "first");
	BOOST_CHECK(c.find(2)->first == 2);
	BOOST_CHECK(c.find(2)->second == "second");
	BOOST_CHECK(c.find(1) == c.begin());
	BOOST_CHECK(c.find(2) != c.end());
	BOOST_CHECK(c.find(3) == c.end());
	const ordered_map<int32, std::string> &cref = c;
	BOOST_CHECK(cref.find(1) == c.begin());
	BOOST_CHECK(cref.find(2) != c.end());
	BOOST_CHECK(cref.find(2) == (++c.begin()));
	BOOST_CHECK(cref.find(2) == (--c.end()));
	BOOST_CHECK(cref.find(3) == c.end());
	BOOST_CHECK(c.erase(1));
	BOOST_CHECK(c.size() == 1);
	BOOST_CHECK(c.find(2)->first == 2);
	BOOST_CHECK(c.find(2)->second == "second");
	BOOST_CHECK(c.begin() != c.end());
	BOOST_CHECK(c.begin()->first == 2);
	BOOST_CHECK(c.begin()->second == "second");
	BOOST_CHECK(c.erase(3) == false);
	c.clear();
	test_empty_ordered_map(c);		
}

BOOST_AUTO_TEST_CASE(test_ordered_map_operators)
{
	ordered_map<int32, std::string> c1, c2;
	BOOST_CHECK(c1.push_back(1, "1").first != c1.end());
	BOOST_CHECK(c1.push_back(2, "2").first != c1.end());
	BOOST_CHECK(c1.push_back(3, "3").first != c1.end());
	BOOST_CHECK(c1 != c2);
	BOOST_CHECK(c2.push_back(0, "0").first != c2.end());
	BOOST_CHECK(c1 != c2);
	c2 = c1;
	BOOST_CHECK(c2.size() == 3);
	BOOST_CHECK(c2.erase(0) == false);
	BOOST_CHECK(c1 == c2);	
	BOOST_CHECK(c2.erase(1));
	BOOST_CHECK(c1 != c2);	
	BOOST_CHECK(c2.push_back(1, "1").first != c2.end());
	BOOST_CHECK(c1 != c2);	
	BOOST_CHECK(c2.erase(1));
	BOOST_CHECK(c2.push_front(1, "1").first != c2.end());
	BOOST_CHECK(c1 == c2);	
	c2.clear();
	BOOST_CHECK(c1 != c2);	
	c2.append(c1);
	BOOST_CHECK(c2.size() == 3);
	BOOST_CHECK(c1 == c2);	
	c2.clear();
	BOOST_CHECK(c2.push_back(0, "0").first != c2.end());
	c2.append(c1);
	BOOST_CHECK(c2.size() == 4);
	BOOST_CHECK(c1 != c2);	
	c2.pop_front();
	BOOST_CHECK(c1 == c2);	
	BOOST_CHECK(c2.push_back(0, "0").first != c2.end());
	BOOST_CHECK(c1 != c2);	
	c2.pop_back();
	BOOST_CHECK(c1 == c2);	
}

BOOST_AUTO_TEST_CASE(test_ordered_map_erase)
{
	ordered_map<std::string, std::string> c;
	c["a"] = "1";
	c["b"] = "2";
	c["c"] = "3";
	c["d"] = "4";

	ordered_map<std::string, std::string>::iterator i = c.find("b");
	BOOST_CHECK(i != c.end());
	BOOST_CHECK(c.erase(i));
	BOOST_CHECK(c.size() == 3);

	BOOST_CHECK(c.begin()->first == "a");
	BOOST_CHECK((--c.end())->first == "d");

	ordered_map<std::string, std::string>::iterator i1 = c.find("a");
	BOOST_CHECK(i1 != c.end());

	ordered_map<std::string, std::string>::iterator i2 = c.find("d");
	BOOST_CHECK(i2 != c.end());
	
	c.erase(i1, i2);
	BOOST_CHECK(c.size() == 1);		// i2 è escluso nella erase sopra
	
	i = c.find("d");
	BOOST_CHECK(i != c.end());
	BOOST_CHECK(c.erase(i));
	test_empty_ordered_map(c);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
