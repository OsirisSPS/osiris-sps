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

#include "algorithms.h"
#include "stringutf16.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_algorithms_tokenize_next)
{
	size_t pos;

	std::string s1 = " 123 456 789  ";

	pos = 0;
	BOOST_CHECK(algorithms::tokenize_next(s1, " ", pos).empty());
	BOOST_CHECK(pos == 1);
	BOOST_CHECK(algorithms::tokenize_next(s1, " ", pos) == "123");
	BOOST_CHECK(pos == 5);
	BOOST_CHECK(algorithms::tokenize_next(s1, " ", pos) == "456");
	BOOST_CHECK(pos == 9);
	BOOST_CHECK(algorithms::tokenize_next(s1, " ", pos) == "789");
	BOOST_CHECK(pos == 13);
	BOOST_CHECK(algorithms::tokenize_next(s1, " ", pos).empty());
	BOOST_CHECK(pos == std::string::npos);

	s1 = "123aa456aa789";

	pos = 0;
	BOOST_CHECK(algorithms::tokenize_next(s1, "aa", pos) == "123");
	BOOST_CHECK(pos == 5);
	BOOST_CHECK(algorithms::tokenize_next(s1, "aa", pos) == "456");
	BOOST_CHECK(pos == 10);
	BOOST_CHECK(algorithms::tokenize_next(s1, "aa", pos) == "789");
	BOOST_CHECK(pos == std::string::npos);
	BOOST_CHECK(algorithms::tokenize_next(s1, "aa", pos).empty());
	BOOST_CHECK(pos == std::string::npos);

	pos = 0;
	BOOST_CHECK(algorithms::tokenize_next(s1, std::string("aa"), pos) == "123");
	BOOST_CHECK(pos == 5);
	BOOST_CHECK(algorithms::tokenize_next(s1, std::string("aa"), pos) == "456");
	BOOST_CHECK(pos == 10);
	BOOST_CHECK(algorithms::tokenize_next(s1, std::string("aa"), pos) == "789");
	BOOST_CHECK(pos == std::string::npos);
	BOOST_CHECK(algorithms::tokenize_next(s1, std::string("aa"), pos).empty());
	BOOST_CHECK(pos == std::string::npos);

	std::wstring s2 = L" 123 456 789  ";

	pos = 0;
	BOOST_CHECK(algorithms::tokenize_next(s2, L" ", pos).empty());
	BOOST_CHECK(pos == 1);
	BOOST_CHECK(algorithms::tokenize_next(s2, L" ", pos) == L"123");
	BOOST_CHECK(pos == 5);
	BOOST_CHECK(algorithms::tokenize_next(s2, L" ", pos) == L"456");
	BOOST_CHECK(pos == 9);
	BOOST_CHECK(algorithms::tokenize_next(s2, L" ", pos) == L"789");
	BOOST_CHECK(pos == 13);
	BOOST_CHECK(algorithms::tokenize_next(s2, L" ", pos).empty());
	BOOST_CHECK(pos == std::wstring::npos);

	s2 = L"123aa456aa789";

	pos = 0;
	BOOST_CHECK(algorithms::tokenize_next(s2, L"aa", pos) == L"123");
	BOOST_CHECK(pos == 5);
	BOOST_CHECK(algorithms::tokenize_next(s2, L"aa", pos) == L"456");
	BOOST_CHECK(pos == 10);
	BOOST_CHECK(algorithms::tokenize_next(s2, L"aa", pos) == L"789");
	BOOST_CHECK(pos == std::wstring::npos);
	BOOST_CHECK(algorithms::tokenize_next(s2, L"aa", pos).empty());
	BOOST_CHECK(pos == std::wstring::npos);

	pos = 0;
	BOOST_CHECK(algorithms::tokenize_next(s2, std::wstring(L"aa"), pos) == L"123");
	BOOST_CHECK(pos == 5);
	BOOST_CHECK(algorithms::tokenize_next(s2, std::wstring(L"aa"), pos) == L"456");
	BOOST_CHECK(pos == 10);
	BOOST_CHECK(algorithms::tokenize_next(s2, std::wstring(L"aa"), pos) == L"789");
	BOOST_CHECK(pos == std::wstring::npos);
	BOOST_CHECK(algorithms::tokenize_next(s2, std::wstring(L"aa"), pos).empty());
	BOOST_CHECK(pos == std::wstring::npos);
}

BOOST_AUTO_TEST_CASE(test_algorithms_substr)
{
	BOOST_CHECK(algorithms::left(std::string("alfa and beta"), 4) == std::string("alfa"));
	BOOST_CHECK(algorithms::left(std::wstring(L"alfa and beta"), 4) == std::wstring(L"alfa"));
	BOOST_CHECK(algorithms::left(StringUTF16("alfa and beta"), 4) == StringUTF16("alfa"));

	BOOST_CHECK(algorithms::left(std::string("alfa and beta")) == std::string("alfa and beta"));
	BOOST_CHECK(algorithms::left(std::wstring(L"alfa and beta")) == std::wstring(L"alfa and beta"));
	BOOST_CHECK(algorithms::left(StringUTF16("alfa and beta")) == StringUTF16("alfa and beta"));

	BOOST_CHECK(algorithms::right(std::string("alfa and beta"), 4) == std::string("beta"));
	BOOST_CHECK(algorithms::right(std::wstring(L"alfa and beta"), 4) == std::wstring(L"beta"));
	BOOST_CHECK(algorithms::right(StringUTF16("alfa and beta"), 4) == StringUTF16("beta"));

	BOOST_CHECK(algorithms::right(std::string("alfa and beta")) == std::string("alfa and beta"));
	BOOST_CHECK(algorithms::right(std::wstring(L"alfa and beta")) == std::wstring(L"alfa and beta"));
	BOOST_CHECK(algorithms::right(StringUTF16("alfa and beta")) == StringUTF16("alfa and beta"));

	BOOST_CHECK(algorithms::mid(std::string("alfa and beta"), 5) == std::string("and beta"));
	BOOST_CHECK(algorithms::mid(std::wstring(L"alfa and beta"), 5) == std::wstring(L"and beta"));
	BOOST_CHECK(algorithms::mid(StringUTF16("alfa and beta"), 5) == StringUTF16("and beta"));

	BOOST_CHECK(algorithms::mid(std::string("alfa and beta"), 5, 3) == std::string("and"));
	BOOST_CHECK(algorithms::mid(std::wstring(L"alfa and beta"), 5, 3) == std::wstring(L"and"));
	BOOST_CHECK(algorithms::mid(StringUTF16("alfa and beta"), 5, 3) == StringUTF16("and"));
}

BOOST_AUTO_TEST_CASE(test_algorithms_lower_upper)
{
	{
		std::string str = "alfa and beta";
		algorithms::to_lower(str);
		BOOST_CHECK(str == "alfa and beta");
		algorithms::to_upper(str);
		BOOST_CHECK(str == "ALFA AND BETA");
		algorithms::to_lower(str);
		BOOST_CHECK(str == "alfa and beta");
	}

	{
		std::wstring str = L"alfa and beta";
		algorithms::to_lower(str);
		BOOST_CHECK(str == L"alfa and beta");
		algorithms::to_upper(str);
		BOOST_CHECK(str == L"ALFA AND BETA");
		algorithms::to_lower(str);
		BOOST_CHECK(str == L"alfa and beta");
	}

	{
		StringUTF16 str = "alfa and beta";
		algorithms::to_lower(str);
		BOOST_CHECK(str == "alfa and beta");
		algorithms::to_upper(str);
		BOOST_CHECK(str == "ALFA AND BETA");
		algorithms::to_lower(str);
		BOOST_CHECK(str == "alfa and beta");
	}

	{
		BOOST_CHECK(algorithms::to_lower_copy(std::string("alfa and beta")) == "alfa and beta");
		BOOST_CHECK(algorithms::to_upper_copy(std::string("alfa and beta")) == "ALFA AND BETA");
		BOOST_CHECK(algorithms::to_upper_copy(std::string("ALFA AND BETA")) == "ALFA AND BETA");
		BOOST_CHECK(algorithms::to_lower_copy(std::string("ALFA AND BETA")) == "alfa and beta");
	}

	{
		BOOST_CHECK(algorithms::to_lower_copy(std::wstring(L"alfa and beta")) == L"alfa and beta");
		BOOST_CHECK(algorithms::to_upper_copy(std::wstring(L"alfa and beta")) == L"ALFA AND BETA");
		BOOST_CHECK(algorithms::to_upper_copy(std::wstring(L"ALFA AND BETA")) == L"ALFA AND BETA");
		BOOST_CHECK(algorithms::to_lower_copy(std::wstring(L"ALFA AND BETA")) == L"alfa and beta");
	}
}

BOOST_AUTO_TEST_CASE(test_algorithms_combine)
{
	BOOST_CHECK(algorithms::combine<std::string>("alfa", "", ";") == "alfa");
	BOOST_CHECK(algorithms::combine<std::string>("", "alfa", ";") == "alfa");
	BOOST_CHECK(algorithms::combine<std::string>("alfa", "beta", ";") == "alfa;beta");
	BOOST_CHECK(algorithms::combine<std::string>("alfa;", "beta", ";") == "alfa;;beta");
	BOOST_CHECK(algorithms::combine<std::string>("alfa;", "beta;", ";") == "alfa;;beta;");
	BOOST_CHECK(algorithms::combine<std::string>("alfa;", "beta;", " ") == "alfa; beta;");
	BOOST_CHECK(algorithms::combine<std::string>("alfa;", "beta", ";_") == "alfa;;_beta");
	BOOST_CHECK(algorithms::combine<std::string>("alfa beta", "gamma delta", " ") == "alfa beta gamma delta");
	BOOST_CHECK(algorithms::combine<std::string>("alfa beta ", " gamma delta", " ") == "alfa beta   gamma delta");
}

BOOST_AUTO_TEST_CASE(test_algorithms_compare)
{
	BOOST_CHECK(algorithms::compare_no_case(std::string("alfa"), std::string("ALFA")));
	BOOST_CHECK(algorithms::compare_no_case(std::wstring(L"alfa"), std::wstring(L"ALFA")));
	BOOST_CHECK(algorithms::compare_no_case(_U("alfa"), _U("ALFA")));
	BOOST_CHECK(algorithms::compare_no_case(std::string(" alfa"), std::string("ALFA")) == false);
	BOOST_CHECK(algorithms::compare_no_case(std::wstring(L" alfa"), std::wstring(L"ALFA")) == false);
	BOOST_CHECK(algorithms::compare_no_case(_U(" alfa"), _U("ALFA")) == false);
	BOOST_CHECK(algorithms::compare_no_case(std::string(""), std::string("")));
	BOOST_CHECK(algorithms::compare_no_case(std::wstring(L""), std::wstring(L"")));
	BOOST_CHECK(algorithms::compare_no_case(_U(""), _U("")));
	BOOST_CHECK(algorithms::compare_no_case(std::string("alfa"), std::string("beta")) == false);
	BOOST_CHECK(algorithms::compare_no_case(std::wstring(L"alfa"), std::wstring(L"beta")) == false);
	BOOST_CHECK(algorithms::compare_no_case(_U("alfa"), _U("beta")) == false);
	BOOST_CHECK(algorithms::compare_no_case(std::string("alfa"), std::string("aLfA")));
	BOOST_CHECK(algorithms::compare_no_case(std::wstring(L"alfa"), std::wstring(L"aLfA")));
	BOOST_CHECK(algorithms::compare_no_case(_U("alfa"), _U("aLfA")));
}

BOOST_AUTO_TEST_CASE(test_algorithms_trim)
{
	BOOST_CHECK(algorithms::trim_left_copy(std::string("  alfa  ")) == std::string("alfa  "));
	BOOST_CHECK(algorithms::trim_left_copy(std::wstring(L"  alfa  ")) == std::wstring(L"alfa  "));
    BOOST_CHECK(algorithms::trim_left_copy(String("  alfa  ")) == String("alfa  "));

	BOOST_CHECK(algorithms::trim_left_copy(std::string(" \t \r \n\n\n\ralfa  \n")) == std::string("alfa  \n"));
	BOOST_CHECK(algorithms::trim_left_copy(std::wstring(L" \t \r \n\n\n\ralfa  \n")) == std::wstring(L"alfa  \n"));
	BOOST_CHECK(algorithms::trim_left_copy(String(" \t \r \n\n\n\ralfa  \n")) == String("alfa  \n"));

	BOOST_CHECK(algorithms::trim_right_copy(std::string("  alfa  ")) == std::string("  alfa"));
	BOOST_CHECK(algorithms::trim_right_copy(std::wstring(L"  alfa  ")) == std::wstring(L"  alfa"));
	BOOST_CHECK(algorithms::trim_right_copy(String("  alfa  ")) == String("  alfa"));

	BOOST_CHECK(algorithms::trim_right_copy(std::string(" \t \r \n\n\n\ralfa  \n")) == std::string(" \t \r \n\n\n\ralfa"));
	BOOST_CHECK(algorithms::trim_right_copy(std::wstring(L" \t \r \n\n\n\ralfa  \n")) == std::wstring(L" \t \r \n\n\n\ralfa"));
	BOOST_CHECK(algorithms::trim_right_copy(String(" \t \r \n\n\n\ralfa  \n")) == String(" \t \r \n\n\n\ralfa"));

	BOOST_CHECK(algorithms::trim_copy(std::string("  alfa  ")) == std::string("alfa"));
	BOOST_CHECK(algorithms::trim_copy(std::wstring(L"  alfa  ")) == std::wstring(L"alfa"));
	BOOST_CHECK(algorithms::trim_copy(String("  alfa  ")) == String("alfa"));

	BOOST_CHECK(algorithms::trim_copy(std::string(" \t \r \n\n\n\ralfa  \n")) == std::string("alfa"));
	BOOST_CHECK(algorithms::trim_copy(std::wstring(L" \t \r \n\n\n\ralfa  \n")) == std::wstring(L"alfa"));
	BOOST_CHECK(algorithms::trim_copy(String(" \t \r \n\n\n\ralfa  \n")) == String("alfa"));

	BOOST_CHECK(algorithms::trim_left_copy(std::string("  alfa  "), boost::algorithm::is_any_of(";_")) == std::string("  alfa  "));
	BOOST_CHECK(algorithms::trim_left_copy(std::wstring(L"  alfa  "), boost::algorithm::is_any_of(L";_")) == std::wstring(L"  alfa  "));

	BOOST_CHECK(algorithms::trim_right_copy(std::string("  alfa  ;"), boost::algorithm::is_any_of(";_")) == std::string("  alfa  "));
	BOOST_CHECK(algorithms::trim_right_copy(std::wstring(L"  alfa  ;"), boost::algorithm::is_any_of(L";_")) == std::wstring(L"  alfa  "));

	BOOST_CHECK(algorithms::trim_copy(std::string("<>  alfa  ;"), boost::algorithm::is_any_of("<> ;")) == std::string("alfa"));
	BOOST_CHECK(algorithms::trim_copy(std::wstring(L"<>  alfa  ;"), boost::algorithm::is_any_of(L"<> ;")) == std::wstring(L"alfa"));
}

BOOST_AUTO_TEST_CASE(test_algorithms_patterns)
{
	BOOST_CHECK(algorithms::starts_with(std::string("alfa"), "al"));
	BOOST_CHECK(algorithms::starts_with(std::string("alfa"), "b") == false);
	BOOST_CHECK(algorithms::starts_with(std::string("alfa"), "AL"));
	BOOST_CHECK(algorithms::starts_with(std::string("alfa"), "AL", algorithms::case_sensitive) == false);

	BOOST_CHECK(algorithms::starts_with(std::wstring(L"alfa"), L"al"));
	BOOST_CHECK(algorithms::starts_with(std::wstring(L"alfa"), L"b") == false);
	BOOST_CHECK(algorithms::starts_with(std::wstring(L"alfa"), L"AL"));
	BOOST_CHECK(algorithms::starts_with(std::wstring(L"alfa"), L"AL", algorithms::case_sensitive) == false);

	BOOST_CHECK(algorithms::ends_with(std::string("alfa"), "a"));
	BOOST_CHECK(algorithms::ends_with(std::string("alfa"), " ") == false);
	BOOST_CHECK(algorithms::ends_with(std::string("alfa"), "fa", algorithms::case_insensitive));
	BOOST_CHECK(algorithms::ends_with(std::string("alfa"), "FA", algorithms::case_sensitive) == false);
	BOOST_CHECK(algorithms::ends_with(std::string("alfa"), "al") == false);

	BOOST_CHECK(algorithms::ends_with(std::wstring(L"alfa"), L"a"));
	BOOST_CHECK(algorithms::ends_with(std::wstring(L"alfa"), L" ") == false);
	BOOST_CHECK(algorithms::ends_with(std::wstring(L"alfa"), L"fa", algorithms::case_insensitive));
	BOOST_CHECK(algorithms::ends_with(std::wstring(L"alfa"), L"FA", algorithms::case_sensitive) == false);
	BOOST_CHECK(algorithms::ends_with(std::wstring(L"alfa"), L"al") == false);
}

BOOST_AUTO_TEST_CASE(test_algorithms_replace)
{
	BOOST_CHECK(algorithms::replace_all_copy(std::string("alfa"), "a", "b") == std::string("blfb"));
	BOOST_CHECK(algorithms::replace_all_copy(std::string("alfa"), "al", "b") == std::string("bfa"));
	BOOST_CHECK(algorithms::replace_all_copy(std::string("alfa"), "A", "b") == std::string("alfa"));
	BOOST_CHECK(algorithms::replace_all_copy(std::string("alfa"), "a", "b", algorithms::case_sensitive) == std::string("blfb"));
	BOOST_CHECK(algorithms::replace_all_copy(std::string("alfa"), "A", "b", algorithms::case_sensitive) == std::string("alfa"));
	BOOST_CHECK(algorithms::replace_all_copy(std::string("alfa"), "a", "b", algorithms::case_insensitive) == std::string("blfb"));
	BOOST_CHECK(algorithms::replace_all_copy(std::string("alfa"), "A", "b", algorithms::case_insensitive) == std::string("blfb"));

	BOOST_CHECK(algorithms::replace_first_copy(std::string("alfa"), "a", "b") == std::string("blfa"));
	BOOST_CHECK(algorithms::replace_last_copy(std::string("alfa"), "a", "b") == std::string("alfb"));
	BOOST_CHECK(algorithms::replace_first_copy(std::string("alfa"), "a", "b", algorithms::case_sensitive) == std::string("blfa"));
	BOOST_CHECK(algorithms::replace_last_copy(std::string("alfa"), "a", "b", algorithms::case_sensitive) == std::string("alfb"));
	BOOST_CHECK(algorithms::replace_first_copy(std::string("alfa"), "A", "b", algorithms::case_insensitive) == std::string("blfa"));
	BOOST_CHECK(algorithms::replace_last_copy(std::string("alfa"), "A", "b", algorithms::case_insensitive) == std::string("alfb"));
	BOOST_CHECK(algorithms::replace_first_copy(std::string("alfa"), "A", "b", algorithms::case_sensitive) == std::string("alfa"));
	BOOST_CHECK(algorithms::replace_last_copy(std::string("alfa"), "A", "b", algorithms::case_sensitive) == std::string("alfa"));

	BOOST_CHECK(algorithms::replace_all_copy(std::wstring(L"alfa"), L"a", L"b") == std::wstring(L"blfb"));
	BOOST_CHECK(algorithms::replace_all_copy(std::wstring(L"alfa"), L"A", L"b") == std::wstring(L"alfa"));
	BOOST_CHECK(algorithms::replace_all_copy(std::wstring(L"alfa"), L"a", L"b", algorithms::case_sensitive) == std::wstring(L"blfb"));
	BOOST_CHECK(algorithms::replace_all_copy(std::wstring(L"alfa"), L"A", L"b", algorithms::case_sensitive) == std::wstring(L"alfa"));
	BOOST_CHECK(algorithms::replace_all_copy(std::wstring(L"alfa"), L"a", L"b", algorithms::case_insensitive) == std::wstring(L"blfb"));
	BOOST_CHECK(algorithms::replace_all_copy(std::wstring(L"alfa"), L"A", L"b", algorithms::case_insensitive) == std::wstring(L"blfb"));
}

BOOST_AUTO_TEST_CASE(test_algorithms_erase)
{
	BOOST_CHECK(algorithms::erase_all_copy(std::string("alfa"), "a") == std::string("lf"));
	BOOST_CHECK(algorithms::erase_all_copy(std::string("alfa"), "a", algorithms::case_sensitive) == std::string("lf"));
	BOOST_CHECK(algorithms::erase_all_copy(std::string("alfa"), "a", algorithms::case_insensitive) == std::string("lf"));
	BOOST_CHECK(algorithms::erase_all_copy(std::string("alfa"), "A", algorithms::case_sensitive) == std::string("alfa"));

	BOOST_CHECK(algorithms::erase_all_copy(std::wstring(L"alfa"), L"a") == std::wstring(L"lf"));
	BOOST_CHECK(algorithms::erase_all_copy(std::wstring(L"alfa"), L"a", algorithms::case_sensitive) == std::wstring(L"lf"));
	BOOST_CHECK(algorithms::erase_all_copy(std::wstring(L"alfa"), L"a", algorithms::case_insensitive) == std::wstring(L"lf"));
	BOOST_CHECK(algorithms::erase_all_copy(std::wstring(L"alfa"), L"A", algorithms::case_sensitive) == std::wstring(L"alfa"));

	BOOST_CHECK(algorithms::erase_first_copy(std::string("alfa"), "a") == std::string("lfa"));
	BOOST_CHECK(algorithms::erase_first_copy(std::string("alfa"), "a", algorithms::case_sensitive) == std::string("lfa"));
	BOOST_CHECK(algorithms::erase_first_copy(std::string("alfa"), "a", algorithms::case_insensitive) == std::string("lfa"));
	BOOST_CHECK(algorithms::erase_first_copy(std::string("alfa"), "A", algorithms::case_sensitive) == std::string("alfa"));
	BOOST_CHECK(algorithms::erase_first_copy(std::string("alfa"), "A", algorithms::case_insensitive) == std::string("lfa"));

	BOOST_CHECK(algorithms::erase_first_copy(std::wstring(L"alfa"), L"a") == std::wstring(L"lfa"));
	BOOST_CHECK(algorithms::erase_first_copy(std::wstring(L"alfa"), L"a", algorithms::case_sensitive) == std::wstring(L"lfa"));
	BOOST_CHECK(algorithms::erase_first_copy(std::wstring(L"alfa"), L"a", algorithms::case_insensitive) == std::wstring(L"lfa"));
	BOOST_CHECK(algorithms::erase_first_copy(std::wstring(L"alfa"), L"A", algorithms::case_sensitive) == std::wstring(L"alfa"));
	BOOST_CHECK(algorithms::erase_first_copy(std::wstring(L"alfa"), L"A", algorithms::case_insensitive) == std::wstring(L"lfa"));

	BOOST_CHECK(algorithms::erase_last_copy(std::string("alfa"), "a") == std::string("alf"));
	BOOST_CHECK(algorithms::erase_last_copy(std::string("alfa"), "a", algorithms::case_sensitive) == std::string("alf"));
	BOOST_CHECK(algorithms::erase_last_copy(std::string("alfa"), "a", algorithms::case_insensitive) == std::string("alf"));
	BOOST_CHECK(algorithms::erase_last_copy(std::string("alfa"), "A", algorithms::case_sensitive) == std::string("alfa"));
	BOOST_CHECK(algorithms::erase_last_copy(std::string("alfa"), "A", algorithms::case_insensitive) == std::string("alf"));

	BOOST_CHECK(algorithms::erase_last_copy(std::wstring(L"alfa"), L"a") == std::wstring(L"alf"));
	BOOST_CHECK(algorithms::erase_last_copy(std::wstring(L"alfa"), L"a", algorithms::case_sensitive) == std::wstring(L"alf"));
	BOOST_CHECK(algorithms::erase_last_copy(std::wstring(L"alfa"), L"a", algorithms::case_insensitive) == std::wstring(L"alf"));
	BOOST_CHECK(algorithms::erase_last_copy(std::wstring(L"alfa"), L"A", algorithms::case_sensitive) == std::wstring(L"alfa"));
	BOOST_CHECK(algorithms::erase_last_copy(std::wstring(L"alfa"), L"A", algorithms::case_insensitive) == std::wstring(L"alf"));
}

BOOST_AUTO_TEST_CASE(test_algorithms_split)
{
	{
		std::string str = "alfa beta  gamma ";

		vector<std::string>::type vectorCompressed;
		BOOST_CHECK(&algorithms::split(str, vectorCompressed, true) == &vectorCompressed);
		BOOST_CHECK(vectorCompressed.size() == 4);
		BOOST_CHECK(vectorCompressed[0] == "alfa");
		BOOST_CHECK(vectorCompressed[1] == "beta");
		BOOST_CHECK(vectorCompressed[2] == "gamma");
		BOOST_CHECK(vectorCompressed[3] == "");

		vector<std::string>::type vectorUncompressed;
		BOOST_CHECK(&algorithms::split(str, vectorUncompressed, false) == &vectorUncompressed);
		BOOST_CHECK(vectorUncompressed.size() == 5);
		BOOST_CHECK(vectorUncompressed[0] == "alfa");
		BOOST_CHECK(vectorUncompressed[1] == "beta");
		BOOST_CHECK(vectorUncompressed[2] == "");
		BOOST_CHECK(vectorUncompressed[3] == "gamma");
		BOOST_CHECK(vectorUncompressed[4] == "");
	}

	{
		std::wstring str = L"alfa beta  gamma ";

		vector<std::wstring>::type vectorCompressed;
		BOOST_CHECK(&algorithms::split(str, vectorCompressed, true) == &vectorCompressed);
		BOOST_CHECK(vectorCompressed.size() == 4);
		BOOST_CHECK(vectorCompressed[0] == L"alfa");
		BOOST_CHECK(vectorCompressed[1] == L"beta");
		BOOST_CHECK(vectorCompressed[2] == L"gamma");
		BOOST_CHECK(vectorCompressed[3] == L"");

		vector<std::wstring>::type vectorUncompressed;
		BOOST_CHECK(&algorithms::split(str, vectorUncompressed, false) == &vectorUncompressed);
		BOOST_CHECK(vectorUncompressed.size() == 5);
		BOOST_CHECK(vectorUncompressed[0] == L"alfa");
		BOOST_CHECK(vectorUncompressed[1] == L"beta");
		BOOST_CHECK(vectorUncompressed[2] == L"");
		BOOST_CHECK(vectorUncompressed[3] == L"gamma");
		BOOST_CHECK(vectorUncompressed[4] == L"");
	}

	{
		std::string str = "alfa;beta,gamma delta;,omega";

		vector<std::string>::type vectorCompressed;
		BOOST_CHECK(&algorithms::split(str, vectorCompressed, boost::algorithm::is_any_of(";,"), true) == &vectorCompressed);
		BOOST_CHECK(vectorCompressed.size() == 4);
		BOOST_CHECK(vectorCompressed[0] == "alfa");
		BOOST_CHECK(vectorCompressed[1] == "beta");
		BOOST_CHECK(vectorCompressed[2] == "gamma delta");
		BOOST_CHECK(vectorCompressed[3] == "omega");

		vector<std::string>::type vectorUncompressed;
		BOOST_CHECK(&algorithms::split(str, vectorUncompressed, boost::algorithm::is_any_of(";,"), false) == &vectorUncompressed);
		BOOST_CHECK(vectorUncompressed.size() == 5);
		BOOST_CHECK(vectorUncompressed[0] == "alfa");
		BOOST_CHECK(vectorUncompressed[1] == "beta");
		BOOST_CHECK(vectorUncompressed[2] == "gamma delta");
		BOOST_CHECK(vectorUncompressed[3] == "");
		BOOST_CHECK(vectorUncompressed[4] == "omega");
	}

	{
		std::wstring str = L"alfa;beta,gamma delta;,omega";

		vector<std::wstring>::type vectorCompressed;
		BOOST_CHECK(&algorithms::split(str, vectorCompressed, boost::algorithm::is_any_of(L";,"), true) == &vectorCompressed);
		BOOST_CHECK(vectorCompressed.size() == 4);
		BOOST_CHECK(vectorCompressed[0] == L"alfa");
		BOOST_CHECK(vectorCompressed[1] == L"beta");
		BOOST_CHECK(vectorCompressed[2] == L"gamma delta");
		BOOST_CHECK(vectorCompressed[3] == L"omega");

		vector<std::wstring>::type vectorUncompressed;
		BOOST_CHECK(&algorithms::split(str, vectorUncompressed, boost::algorithm::is_any_of(L";,"), false) == &vectorUncompressed);
		BOOST_CHECK(vectorUncompressed.size() == 5);
		BOOST_CHECK(vectorUncompressed[0] == L"alfa");
		BOOST_CHECK(vectorUncompressed[1] == L"beta");
		BOOST_CHECK(vectorUncompressed[2] == L"gamma delta");
		BOOST_CHECK(vectorUncompressed[3] == L"");
		BOOST_CHECK(vectorUncompressed[4] == L"omega");
	}

	{
		std::string str = "alfa;beta,gamma delta;,omega";

		list<std::string>::type listCompressed;
		BOOST_CHECK(&algorithms::split(str, listCompressed, boost::algorithm::is_any_of(";,"), true) == &listCompressed);
		BOOST_CHECK(listCompressed.size() == 4);

		list<std::string>::type listUncompressed;
		BOOST_CHECK(&algorithms::split(str, listUncompressed, boost::algorithm::is_any_of(";,"), false) == &listUncompressed);
		BOOST_CHECK(listUncompressed.size() == 5);
	}

	{
		std::wstring str = L"alfa;beta,gamma delta;,omega";

		list<std::wstring>::type listCompressed;
		BOOST_CHECK(&algorithms::split(str, listCompressed, boost::algorithm::is_any_of(L";,"), true) == &listCompressed);
		BOOST_CHECK(listCompressed.size() == 4);

		list<std::wstring>::type listUncompressed;
		BOOST_CHECK(&algorithms::split(str, listUncompressed, boost::algorithm::is_any_of(L";,"), false) == &listUncompressed);
		BOOST_CHECK(listUncompressed.size() == 5);
	}
}

BOOST_AUTO_TEST_CASE(test_algorithms_join)
{
	{
		vector<std::string>::type vector;
		vector.push_back("alfa");
		vector.push_back("beta");
		vector.push_back("gamma");
		BOOST_CHECK(algorithms::join(vector, " ") == std::string("alfa beta gamma"));
		BOOST_CHECK(algorithms::join(vector, ";") == std::string("alfa;beta;gamma"));
		BOOST_CHECK(algorithms::join(vector, "foo") == std::string("alfafoobetafoogamma"));
		BOOST_CHECK(algorithms::join(vector, "") == std::string("alfabetagamma"));
		BOOST_CHECK(algorithms::join(vector) == std::string("alfabetagamma"));
	}

	{
		vector<std::wstring>::type vector;
		vector.push_back(L"alfa");
		vector.push_back(L"beta");
		vector.push_back(L"gamma");
		BOOST_CHECK(algorithms::join(vector, L" ") == std::wstring(L"alfa beta gamma"));
		BOOST_CHECK(algorithms::join(vector, L";") == std::wstring(L"alfa;beta;gamma"));
		BOOST_CHECK(algorithms::join(vector, L"foo") == std::wstring(L"alfafoobetafoogamma"));
		BOOST_CHECK(algorithms::join(vector, L"") == std::wstring(L"alfabetagamma"));
		BOOST_CHECK(algorithms::join(vector) == std::wstring(L"alfabetagamma"));
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
