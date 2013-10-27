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

#include "charset.h"
#include "stringutf16.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_stringutf16_construction)
{
	StringUTF16 str1;
	BOOST_CHECK(str1.empty());
	BOOST_CHECK(str1.size() == 0);
	StringUTF16 str2((achar *)null);
	BOOST_CHECK(str2.empty());
	BOOST_CHECK(str2.size() == 0);
	StringUTF16 str3(std::string(""));
	BOOST_CHECK(str3.empty());
	BOOST_CHECK(str3.size() == 0);
	StringUTF16 str4((uchar *)null);
	BOOST_CHECK(str4.empty());
	BOOST_CHECK(str4.size() == 0);
	StringUTF16 str5((wchar *)null);
	BOOST_CHECK(str5.empty());
	BOOST_CHECK(str5.size() == 0);
	StringUTF16 str6(std::wstring(L""));
	BOOST_CHECK(str6.empty());
	BOOST_CHECK(str6.size() == 0);
	StringUTF16 str7("osiris");
	BOOST_CHECK(str7.empty() == false);
	BOOST_CHECK(str7.size() == 6);
	StringUTF16 str8(std::string("osiris"));
	BOOST_CHECK(str8.empty() == false);
	BOOST_CHECK(str8.size() == 6);
	StringUTF16 str9("osiris", 6);
	BOOST_CHECK(str9.empty() == false);
	BOOST_CHECK(str9.size() == 6);
	StringUTF16 str10(L"osiris");
	BOOST_CHECK(str10.empty() == false);
	BOOST_CHECK(str10.size() == 6);
	StringUTF16 str11(std::wstring(L"osiris"));
	BOOST_CHECK(str11.empty() == false);
	BOOST_CHECK(str11.size() == 6);
	StringUTF16 str12(L"osiris", 6);
	BOOST_CHECK(str12.empty() == false);
	BOOST_CHECK(str12.size() == 6);
	StringUTF16 str13("osiris", 0);
	BOOST_CHECK(str13.empty());
	BOOST_CHECK(str13.size() == 0);
	StringUTF16 str14("osiris", 1);
	BOOST_CHECK(str14.empty() == false);
	BOOST_CHECK(str14.size() == 1);
	StringUTF16 str15(L"osiris", 0);
	BOOST_CHECK(str15.empty());
	BOOST_CHECK(str15.size() == 0);
	StringUTF16 str16(L"osiris", 1);	
	BOOST_CHECK(str16.empty() == false);
	BOOST_CHECK(str16.size() == 1);	
}

BOOST_AUTO_TEST_CASE(test_stringutf16_buffer)
{
	StringUTF16 str("alfa");
	BOOST_CHECK(str.c_str()[4] == 0);	// c_str deve garantire la restituzione di un buffer terminato da zero

	byte buffer[] = { 'a', 0, 'l', 0, 'f', 0, 'a', 0, 0, 0 };
	OS_STATIC_ASSERT(sizeof(buffer) == 10);		// ("alfa" + carattere terminatore) * 2 bytes
	BOOST_CHECK(memcmp(str.c_str(), buffer, sizeof(buffer)) == 0);
}

BOOST_AUTO_TEST_CASE(test_stringutf16_insertion)
{
	StringUTF16 alfa("alfa");
	StringUTF16 beta("beta");
	StringUTF16 gamma("gamma");

	StringUTF16 str;
	str.append(alfa);
	BOOST_CHECK(str.size() == 4);	
	BOOST_CHECK(str.length() == 4);	
	BOOST_CHECK(str == _U("alfa"));	
	str.append(beta.c_str());
	BOOST_CHECK(str.size() == 8);
	BOOST_CHECK(str.length() == 8);	
	BOOST_CHECK(str == _U("alfabeta"));	
	str.append(gamma.c_str(), gamma.length());
	BOOST_CHECK(str.size() == 13);
	BOOST_CHECK(str.length() == 13);	
	BOOST_CHECK(str == _U("alfabetagamma"));	
	str.append(chPlus);
	str.push_back(chQuestion);
	BOOST_CHECK(str.length() == 15);	
	BOOST_CHECK(str == _U("alfabetagamma+?"));	
}

BOOST_AUTO_TEST_CASE(test_stringutf16_assignment)
{
	StringUTF16 str1;
	str1.assign(_U("alfa"));
	BOOST_CHECK(str1.size() == 4);	
	BOOST_CHECK(str1.length() == 4);	
	BOOST_CHECK(str1.empty() == false);	
	BOOST_CHECK(str1 == _U("alfa"));	

	StringUTF16 str2;
	str2.assign(str1.c_str(), str1.length());
	BOOST_CHECK(str2.size() == 4);	
	BOOST_CHECK(str2.length() == 4);	
	BOOST_CHECK(str2.empty() == false);	
	BOOST_CHECK(str2 == _U("alfa"));	

	StringUTF16 str3;
	str3.assign(str2);
	BOOST_CHECK(str3.size() == 4);	
	BOOST_CHECK(str3.length() == 4);	
	BOOST_CHECK(str3.empty() == false);	
	BOOST_CHECK(str3 == _U("alfa"));	

	StringUTF16 str4;
	str4.assign(str3, 0);
	BOOST_CHECK(str4.size() == 4);	
	BOOST_CHECK(str4.length() == 4);	
	BOOST_CHECK(str4.empty() == false);	
	BOOST_CHECK(str4 == _U("alfa"));	

	StringUTF16 str5;
	str5.assign(str4, 1);
	BOOST_CHECK(str5.size() == 3);	
	BOOST_CHECK(str5.length() == 3);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("lfa"));	
	str5.clear();
	str5.assign(str4, 2);
	BOOST_CHECK(str5.size() == 2);	
	BOOST_CHECK(str5.length() == 2);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("fa"));	
	str5.clear();
	str5.assign(str4, 3);
	BOOST_CHECK(str5.size() == 1);	
	BOOST_CHECK(str5.length() == 1);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("a"));	
	str5.clear();
	str5.assign(str4, 4);
	BOOST_CHECK(str5.size() == 0);	
	BOOST_CHECK(str5.length() == 0);	
	BOOST_CHECK(str5.empty());	
	BOOST_CHECK(str5 == _U(""));	
	str5.clear();
	str5.assign(str4, 0, 4);
	BOOST_CHECK(str5.size() == 4);	
	BOOST_CHECK(str5.length() == 4);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("alfa"));	
	str5.clear();
	str5.assign(str4, 0, 3);
	BOOST_CHECK(str5.size() == 3);	
	BOOST_CHECK(str5.length() == 3);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("alf"));	
	str5.clear();
	str5.assign(str4, 0, 2);
	BOOST_CHECK(str5.size() == 2);	
	BOOST_CHECK(str5.length() == 2);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("al"));	
	str5.clear();
	str5.assign(str4, 0, 1);
	BOOST_CHECK(str5.size() == 1);	
	BOOST_CHECK(str5.length() == 1);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("a"));	
	str5.clear();
	str5.assign(str4, 0, 0);
	BOOST_CHECK(str5.size() == 0);	
	BOOST_CHECK(str5.length() == 0);	
	BOOST_CHECK(str5.empty());	
	BOOST_CHECK(str5 == _U(""));	
	str5.clear();
	str5.assign(str4, 0, 5);
	BOOST_CHECK(str5.size() == 4);	
	BOOST_CHECK(str5.length() == 4);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("alfa"));	
	str5.clear();
	str5.assign(str4, 1, 4);
	BOOST_CHECK(str5.size() == 3);	
	BOOST_CHECK(str5.length() == 3);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("lfa"));	
	str5.clear();
	str5.assign(str4.c_str());
	BOOST_CHECK(str5.size() == 4);	
	BOOST_CHECK(str5.length() == 4);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("alfa"));	
	str5.clear();
	str5.assign(str4.c_str(), 1);
	BOOST_CHECK(str5.size() == 1);	
	BOOST_CHECK(str5.length() == 1);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("a"));	
	str5.clear();
	str5.assign(str4.c_str(), 2);
	BOOST_CHECK(str5.size() == 2);	
	BOOST_CHECK(str5.length() == 2);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("al"));	
	str5.clear();
	str5.assign(str4.c_str(), 3);
	BOOST_CHECK(str5.size() == 3);	
	BOOST_CHECK(str5.length() == 3);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("alf"));	
	str5.clear();
	str5.assign(str4.c_str(), 4);
	BOOST_CHECK(str5.size() == 4);	
	BOOST_CHECK(str5.length() == 4);	
	BOOST_CHECK(str5.empty() == false);	
	BOOST_CHECK(str5 == _U("alfa"));	
	str5.clear();
	str5.assign(str4.c_str(), 0);
	BOOST_CHECK(str5.size() == 0);	
	BOOST_CHECK(str5.length() == 0);	
	BOOST_CHECK(str5.empty());	
	BOOST_CHECK(str5 == _U(""));	
}

BOOST_AUTO_TEST_CASE(test_stringutf16_compare)
{
	StringUTF16 alfa("alfa");
	StringUTF16 beta("beta");

	BOOST_CHECK(alfa.compare(alfa) == 0);	
	BOOST_CHECK(alfa.compare(beta) < 0);	
	BOOST_CHECK(beta.compare(alfa) > 0);		
	
	BOOST_CHECK(alfa.compare(alfa.c_str()) == 0);	
	BOOST_CHECK(alfa.compare(beta.c_str()) < 0);	
	BOOST_CHECK(beta.compare(alfa.c_str()) > 0);		

	BOOST_CHECK(alfa.compare(alfa.c_str(), alfa.length()) == 0);	
	BOOST_CHECK(alfa.compare(beta.c_str(), beta.length()) < 0);	
	BOOST_CHECK(beta.compare(alfa.c_str(), alfa.length()) > 0);		
}

BOOST_AUTO_TEST_CASE(test_stringutf16_insert)
{
	StringUTF16 str1;
	StringUTF16 str2("gamma");
	
	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(0, str2) == StringUTF16("gammaalfabeta"));

	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(0, str2.c_str()) == StringUTF16("gammaalfabeta"));

	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(4, str2) == StringUTF16("alfagammabeta"));

	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(4, str2.c_str()) == StringUTF16("alfagammabeta"));

	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(8, str2) == StringUTF16("alfabetagamma"));

	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(8, str2.c_str()) == StringUTF16("alfabetagamma"));

	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(0, str2, 2) == StringUTF16("mmaalfabeta"));

	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(0, str2, 2, 1) == StringUTF16("malfabeta"));

	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(0, str2.c_str(), 1) == StringUTF16("galfabeta"));

	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(0, str2, 2, 0) == StringUTF16("alfabeta"));

	str1.assign("alfabeta");
	BOOST_CHECK(str1.insert(0, str2.c_str(), 0) == StringUTF16("alfabeta"));
}

BOOST_AUTO_TEST_CASE(test_stringutf16_swap)
{
	StringUTF16 str1("alfa");	
	StringUTF16 str2("beta");	

	str1.swap(str2);
	BOOST_CHECK(str1 == StringUTF16("beta"));
	BOOST_CHECK(str2 == StringUTF16("alfa"));

	str2.swap(str1);
	BOOST_CHECK(str1 == StringUTF16("alfa"));
	BOOST_CHECK(str2 == StringUTF16("beta"));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
