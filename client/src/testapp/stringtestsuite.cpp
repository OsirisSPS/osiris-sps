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

#include "buffer.h"

#include "iostream"

#include "charset.h"

#include "filesystem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(test_string_unicode)
{
	String str1 = "alfa";
	BOOST_CHECK(str1 == String(str1.to_wide()));
	BOOST_CHECK(str1.length() == String(str1.to_wide()).to_wide().length());
	BOOST_CHECK(memcmp(str1.str().data(), String(str1.to_wide()).str().data(), str1.size()) == 0);
	BOOST_CHECK(memcmp(str1.str().c_str(), String(str1.to_wide()).str().c_str(), str1.size()) == 0);

	String str2;

	String path = "../../../src/testapp/test_unicode.txt";
	#ifdef OS_PLATFORM_MAC
        path = "/Users/osiris/osiris/osiris-sps/client/src/testapp/test_unicode.txt";
    #endif
	if(FileSystem::instance()->exists(path))
	{
		Buffer buffer;
		BOOST_CHECK(buffer.load(path));

		std::string tmp((char*)buffer.getData(), buffer.getSize());
		str2.from_utf8(tmp);

		std::cout << "O:" << str2.to_ascii() << std::endl;
		std::cout << "1:" << str2.length() << std::endl;
		std::cout << "2:" << str2.to_wide().length() << std::endl;  // Mac: 308, Linux/Win: 224
		std::cout << "3:" << String(str2.to_wide()).to_wide().length() << std::endl;
		std::cout << "4:" << String(str2.to_wide()).length() << std::endl;
		std::cout << "5:" << str2.to_wide().length() << std::endl;
		std::cout << "8:" << str2.to_ascii().length() << std::endl;
		std::cout << "9:" << str2.to_utf8().length() << std::endl;



		BOOST_CHECK(str2 == String(str2.to_wide()));
		BOOST_CHECK(str2.length() == String(str2.to_wide()).to_wide().length());
		BOOST_CHECK(memcmp(str2.str().data(), String(str2.to_wide()).str().data(), str2.size()) == 0);
		BOOST_CHECK(memcmp(str2.str().c_str(), String(str2.to_wide()).str().c_str(), str2.size()) == 0);
	}
}

BOOST_AUTO_TEST_CASE(test_string_construction)
{
	String s1(_S("s1"));
	OS_EXCEPT_IF(s1.length() != 2, "length of 's1' must be 2");
	OS_EXCEPT_IF(s1.at(0) != chLatin_s || s1.at(1) != chDigit_1, "invalid string 's1'");
	OS_EXCEPT_IF(s1.buffer_size() != (sizeof(uchar)*s1.length()), "invalid buffer size of 's1'");

	String s2 = _S("str2");
	OS_EXCEPT_IF(s2.length() != 4, "length of 'str2' must be 4");
	OS_EXCEPT_IF(s2.at(0) != chLatin_s || s2.at(1) != chLatin_t || s2.at(2) != chLatin_r || s2.at(3) != chDigit_2, "invalid string 'str2'");
	OS_EXCEPT_IF(s2.buffer_size() != (sizeof(uchar)*s2.length()), "invalid buffer size of 's1'");

	String s3;
	s3.assign(_S("str"));
	OS_EXCEPT_IF(s3.length() != 3, "length of 'str' must be 3");
	OS_EXCEPT_IF(s3.at(0) != chLatin_s || s3.at(1) != chLatin_t || s3.at(2) != chLatin_r, "invalid string 'str'");

	String s4(s3);
	OS_EXCEPT_IF(s4.length() != 3, "length of s4 must be 3");
	OS_EXCEPT_IF(s4.at(0) != chLatin_s || s4.at(1) != chLatin_t || s4.at(2) != chLatin_r, "s4 must be equal to 'str'");

	String s5;
	s5 = s4;
	OS_EXCEPT_IF(s5.length() != 3, "length of s5 must be 3");
	OS_EXCEPT_IF(s5.at(0) != chLatin_s || s5.at(1) != chLatin_t || s5.at(2) != chLatin_r, "s5 must be equal to 'str'");

	String s6;
	s6.assign(s5);
	OS_EXCEPT_IF(s6.length() != 3, "length of s6 must be 3");
	OS_EXCEPT_IF(s6.at(0) != chLatin_s || s6.at(1) != chLatin_t || s6.at(2) != chLatin_r, "s6 must be equal to 'str'");

	String s7 = s5 + s6;
	OS_EXCEPT_IF(s7.length() != 6, "length of s7 must be 6");
	OS_EXCEPT_IF((s7 == (s5 + s6)) == false, "s7 must be equal to s5 + s6");
	OS_EXCEPT_IF(s7 != (s5 + s6), "s7 must not be different than s5 + s6");

	s7 += s5;
	OS_EXCEPT_IF(s7.length() != 9, "length of s7 must be 9");

	String s8 = L"test";
	BOOST_CHECK(s8 == _S("test"));
}

BOOST_AUTO_TEST_CASE(test_string_charset)
{
	OS_EXCEPT_IF(_S(" ").at(0) != chSpace, "characted must be ' '");
	OS_EXCEPT_IF(_S("~").at(0) != chTilde, "characted must be '~'");
	OS_EXCEPT_IF(_S("+").at(0) != chPlus, "characted must be '+'");
	OS_EXCEPT_IF(_S("*").at(0) != chAsterisk, "characted must be '*'");
	OS_EXCEPT_IF(_S("?").at(0) != chQuestion, "characted must be '?'");
	OS_EXCEPT_IF(_S("|").at(0) != chPipe, "characted must be '|'");
	OS_EXCEPT_IF(_S("=").at(0) != chEqual, "characted must be '='");
	OS_EXCEPT_IF(_S(".").at(0) != chPeriod, "characted must be '.'");
	OS_EXCEPT_IF(_S("[").at(0) != chOpenSquare, "characted must be '['");
	OS_EXCEPT_IF(_S("]").at(0) != chCloseSquare, "characted must be ']'");
	OS_EXCEPT_IF(_S("^").at(0) != chCaret, "characted must be '^'");
	OS_EXCEPT_IF(_S("$").at(0) != chDollarSign, "characted must be '$'");
	OS_EXCEPT_IF(_S("(").at(0) != chOpenParen, "characted must be '('");
	OS_EXCEPT_IF(_S(")").at(0) != chCloseParen, "characted must be ')'");
	OS_EXCEPT_IF(_S("{").at(0) != chOpenCurly, "characted must be '{'");
	OS_EXCEPT_IF(_S("}").at(0) != chCloseCurly, "characted must be '}'");
	OS_EXCEPT_IF(_S("!").at(0) != chBang, "characted must be '!'");
	OS_EXCEPT_IF(_S("<").at(0) != chOpenAngle, "characted must be '<'");
	OS_EXCEPT_IF(_S(">").at(0) != chCloseAngle, "characted must be '>'");
	OS_EXCEPT_IF(_S(":").at(0) != chColon, "characted must be ':'");
	OS_EXCEPT_IF(_S("0").at(0) != chDigit_0, "characted must be '0'");
	OS_EXCEPT_IF(_S("\\").at(0) != chBackSlash, "characted must be '\\'");
	OS_EXCEPT_IF(_S("/").at(0) != chForwardSlash, "characted must be '/'");
	OS_EXCEPT_IF(_S("\n").at(0) != chLF, "characted must be '\n'");
	OS_EXCEPT_IF(_S("\r").at(0) != chCR, "characted must be '\r'");
	OS_EXCEPT_IF(_S("&").at(0) != chAmpersand, "characted must be '&'");
	OS_EXCEPT_IF(_S("#").at(0) != chPound, "characted must be '#'");
	OS_EXCEPT_IF(_S(";").at(0) != chSemiColon, "characted must be ';'");
	OS_EXCEPT_IF(_S("`").at(0) != chGrave, "characted must be '`'");
	OS_EXCEPT_IF(_S(",").at(0) != chComma, "characted must be ','");

    static const uchar Jan[] = { chLatin_J, chLatin_a, chLatin_n, chNull };
	static const uchar Feb[] = { chLatin_F, chLatin_e, chLatin_b, chNull };
	static const uchar Mar[] = { chLatin_M, chLatin_a, chLatin_r, chNull };
	static const uchar Apr[] = { chLatin_A, chLatin_p, chLatin_r, chNull };
	static const uchar May[] = { chLatin_M, chLatin_a, chLatin_y, chNull };
	static const uchar Jun[] = { chLatin_J, chLatin_u, chLatin_n, chNull };
	static const uchar Jul[] = { chLatin_J, chLatin_u, chLatin_l, chNull };
	static const uchar Aug[] = { chLatin_A, chLatin_u, chLatin_g, chNull };
	static const uchar Sep[] = { chLatin_S, chLatin_e, chLatin_p, chNull };
	static const uchar Oct[] = { chLatin_O, chLatin_c, chLatin_t, chNull };
	static const uchar Nov[] = { chLatin_N, chLatin_o, chLatin_v, chNull };
	static const uchar Dec[] = { chLatin_D, chLatin_e, chLatin_c, chNull };
	static const uchar *months[] = { Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };

	OS_EXCEPT_IF(months[0] != _S("Jan"), "string is not 'Jan'");
	OS_EXCEPT_IF(months[1] != _S("Feb"), "string is not 'Feb'");
	OS_EXCEPT_IF(months[2] != _S("Mar"), "string is not 'Mar'");
	OS_EXCEPT_IF(months[3] != _S("Apr"), "string is not 'Apr'");
	OS_EXCEPT_IF(months[4] != _S("May"), "string is not 'May'");
	OS_EXCEPT_IF(months[5] != _S("Jun"), "string is not 'Jun'");
	OS_EXCEPT_IF(months[6] != _S("Jul"), "string is not 'Jul'");
	OS_EXCEPT_IF(months[7] != _S("Aug"), "string is not 'Aug'");
	OS_EXCEPT_IF(months[8] != _S("Sep"), "string is not 'Sep'");
	OS_EXCEPT_IF(months[9] != _S("Oct"), "string is not 'Oct'");
	OS_EXCEPT_IF(months[10] != _S("Nov"), "string is not 'Nov'");
	OS_EXCEPT_IF(months[11] != _S("Dec"), "string is not 'Dec'");

	static const uchar Sun[] = { chLatin_S, chLatin_u, chLatin_n, chNull };
	static const uchar Mon[] = { chLatin_M, chLatin_o, chLatin_n, chNull };
	static const uchar Tue[] = { chLatin_T, chLatin_u, chLatin_e, chNull };
	static const uchar Wed[] = { chLatin_W, chLatin_e, chLatin_d, chNull };
	static const uchar Thu[] = { chLatin_T, chLatin_h, chLatin_u, chNull };
	static const uchar Fri[] = { chLatin_F, chLatin_r, chLatin_i, chNull };
	static const uchar Sat[] = { chLatin_S, chLatin_a, chLatin_t, chNull };
	static const uchar *days[] = { Sun, Mon, Tue, Wed, Thu, Fri, Sat };

	OS_EXCEPT_IF(days[0] != _S("Sun"), "string is not 'Sun'");
	OS_EXCEPT_IF(days[1] != _S("Mon"), "string is not 'Mon'");
	OS_EXCEPT_IF(days[2] != _S("Tue"), "string is not 'Tue'");
	OS_EXCEPT_IF(days[3] != _S("Wed"), "string is not 'Wed'");
	OS_EXCEPT_IF(days[4] != _S("Thu"), "string is not 'Thu'");
	OS_EXCEPT_IF(days[5] != _S("Fri"), "string is not 'Fri'");
	OS_EXCEPT_IF(days[6] != _S("Sat"), "string is not 'Sat'");
}

BOOST_AUTO_TEST_CASE(test_string_comparison)
{
	String str = _S("alfa");
	OS_EXCEPT_IF(str != _S("alfa"), "'alfa' must be equal to 'alfa'");
	OS_EXCEPT_IF(str == _S("ALFA"), "'alfa' must be different from 'ALFA'");
	OS_EXCEPT_IF(str.compare(_S("ALFA").str()) <= 0, "'alfa' must be > than 'ALFA'");

	str = _S("ALFA");
	OS_EXCEPT_IF(str != _S("ALFA"), "'ALFA' must be equal to 'ALFA'");
	OS_EXCEPT_IF(str.compare(_S("alfa").str()) >= 0, "'ALFA' must be < than 'alfa'");
	OS_EXCEPT_IF(str.compare(_S("ALFA").str()), "'ALFA' compared with 'ALFA' must be 0");
	OS_EXCEPT_IF(str == _S("alfa"), "'ALFA' must be different from 'alfa'");
    OS_EXCEPT_IF(str.compare_no_case(_S("alfa")) == false, "'ALFA' compared no case with 'alfa' must be true");

	str = _S("aAbB");
	OS_EXCEPT_IF(str.starts_with(_S("aa"), false), "'aAbB' doesn't start with 'aa'");
	OS_EXCEPT_IF(str.starts_with(_S("aA"), false) == false, "'aAbB' must starts with 'aA'");
	OS_EXCEPT_IF(str.starts_with(_S("aa"), true) == false, "'aAbB' must start with 'aa' (nocase)");
	OS_EXCEPT_IF(str.ends_with(_S("bb"), false), "'aAbB' doesn't end with 'bb'");
	OS_EXCEPT_IF(str.ends_with(_S("bB"), false) == false, "'aAbB' must ends with 'bB'");
	OS_EXCEPT_IF(str.ends_with(_S("bb"), true) == false, "'aAbB' must end with 'bb' (nocase)");
}

BOOST_AUTO_TEST_CASE(test_string_transform)
{
	String str = _S("alfa");

	str.to_upper();
	OS_EXCEPT_IF(str != _S("ALFA"), "uppercase of 'alfa' must be 'ALFA'");
	str.to_lower();
	OS_EXCEPT_IF(str != _S("alfa"), "lowercase of 'ALFA' must be 'alfa'");

	str.clear();
	OS_EXCEPT_IF(str.empty() == false, "a cleared string must be empty");
	str.append(_S(" a "));
	OS_EXCEPT_IF(str != _S(" a "), "str must be ' a '");

	str.trim_left();
	OS_EXCEPT_IF(str != _S("a "), "str must be 'a '");

	str.trim_right();
	OS_EXCEPT_IF(str != _S("a"), "str must be 'a'");

	str.trim(_S("a"));
	OS_EXCEPT_IF(str.empty() == false, "str must be empty");
}

BOOST_AUTO_TEST_CASE(test_string_find)
{
	String str = _S("alfa");
	size_t pos = str.find(chLatin_a);
	OS_EXCEPT_IF(pos != 0, "'a' must be the first characted in 'alfa'");

	pos = str.rfind(chLatin_a);
	OS_EXCEPT_IF(pos != 3, "'a' must be the last characted in 'alfa'");

	pos = str.find(chLatin_z);
	OS_EXCEPT_IF(pos != String::npos, "'z' must not exists in 'alfa'");
}

BOOST_AUTO_TEST_CASE(test_string_format)
{
	String str;

	String pattern = _S("str%d");
	str = String::format(pattern.c_str(), 1);
	OS_EXCEPT_IF(str != _S("str1"), "str must be 'str1'");

	pattern = _S("str%g");
	str = String::format(pattern.c_str(), 0.5);
	OS_EXCEPT_IF(str != _S("str0.5"), "str must be 'str0.5'");

	pattern = _S("str%C");
	str = String::format(pattern.c_str(), chLatin_Z);
	OS_EXCEPT_IF(str != _S("strZ"), "str must be 'strZ'");

	String test = _S("test");
	pattern = _S("str %S");
	str = String::format(pattern.c_str(), test.c_str());
	OS_EXCEPT_IF(str != _S("str test"), "str must be 'str test'");

	pattern = _S("");
	str = String::format(pattern.c_str());
	OS_EXCEPT_IF(str.empty() == false, "str must be empty");

	str = String::format(_S("%S %S %S").c_str(), _S("alfa").c_str(), _S("beta").c_str(), _S("gamma").c_str());
	OS_EXCEPT_IF(str != _S("alfa beta gamma"), "str must be 'alfa beta gamma'");

	String str1 = _S("id");
	String str2 = _S("=");
	String str3 = _S("alfabetagamma");
	str = String::format(_S("%S %S %S").c_str(), str1.c_str(), str2.c_str(), str3.c_str());
	OS_EXCEPT_IF(str != _S("id = alfabetagamma"), "str must be 'id = alfabetagamma'");
}

BOOST_AUTO_TEST_CASE(test_string_substr)
{
	String str = _S("alfabeta");

	OS_EXCEPT_IF(str.left(4) != _S("alfa"), "left(4) of 'alfabeta' must be 'alfa'");
	OS_EXCEPT_IF(str.right(4) != _S("beta"), "right(4) of 'alfabeta' must be 'beta'");
	OS_EXCEPT_IF(str.mid(2, 2) != _S("fa"), "mid(2, 2) of 'alfabeta' must be 'fa'");
	OS_EXCEPT_IF(str.mid(2) != _S("fabeta"), "mid(2) of 'alfabeta' must be 'fabeta'");
}

BOOST_AUTO_TEST_CASE(test_string_replace)
{
	String str = _S("aaa");

	str.replace(0, 3, _S("bbb").str());
	OS_EXCEPT_IF(str != _S("bbb"), "str must be 'bbb'");

	str.replace(1, 1, _S("c").str());
	OS_EXCEPT_IF(str != _S("bcb"), "str must be 'bcb'");

	str = _S("alfa and beta");
	str.replace_all(_S(" "), _S(""));
	OS_EXCEPT_IF(str != _S("alfaandbeta"), "str must be 'alfaandbeta'");

	str.assign(_S("abab"));
	str.replace_all(_S("a"), _S("b"));
	OS_EXCEPT_IF(str != _S("bbbb"), "str must be 'bbbb'");
}

BOOST_AUTO_TEST_CASE(test_string_scanf)
{
	String str1 = _S("1 2");
	int32 v1 = 0;
	int32 v2 = 0;
	OS_EXCEPT_IF(str1.scanf(_S("%d %d").c_str(), &v1, &v2) != 2, "Invalid scanf (member) return value with pattern '%d %d'");
	OS_EXCEPT_IF(v1 != 1, "v1 must be 1 with pattern '%d %d'");
	OS_EXCEPT_IF(v2 != 2, "v1 must be 2 with pattern '%d %d'");

	str1 = _S("1-2");
	v1 = 0;
	v2 = 0;
	OS_EXCEPT_IF(str1.scanf(_S("%d-%d").c_str(), &v1, &v2) != 2, "Invalid scanf (member) return value with pattern '%d-%d'");
	OS_EXCEPT_IF(v1 != 1, "v1 must be 1 with pattern '%d-%d'");
	OS_EXCEPT_IF(v2 != 2, "v1 must be 2 with pattern '%d-%d'");

	int32 v3 = 0;
	int32 v4 = 0;
	OS_EXCEPT_IF(String::sscanf(_S("3 4").c_str(), _S("%d %d").c_str(), &v3, &v4) != 2, "Invalid scanf (static) return value with pattern '%d %d'");
	OS_EXCEPT_IF(v3 != 3, "v3 must be 3 with pattern '%d %d'");
	OS_EXCEPT_IF(v4 != 4, "v4 must be 4 with pattern '%d %d'");

	v3 = 0;
	v4 = 0;
	OS_EXCEPT_IF(String::sscanf(_S("3-4").c_str(), _S("%d-%d").c_str(), &v3, &v4) != 2, "Invalid scanf (static) return value with pattern '%d-%d'");
	OS_EXCEPT_IF(v3 != 3, "v3 must be 3 with pattern '%d-%d'");
	OS_EXCEPT_IF(v4 != 4, "v4 must be 4 with pattern '%d-%d'");
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
