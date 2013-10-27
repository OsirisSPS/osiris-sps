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

#include "boost/date_time/posix_time/ptime.hpp"
#include "httpparser.h"
#include "httpurl.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_http_url)
{
	HttpUrl url1;
	BOOST_CHECK(url1.empty());
	BOOST_CHECK(url1.getUserName().empty());
	BOOST_CHECK(url1.getPassword().empty());
	BOOST_CHECK(url1.getHost().empty());
	BOOST_CHECK(url1.getPort() == 0);
	BOOST_CHECK(url1.getPath().empty());
	BOOST_CHECK(url1.toString().empty());
	BOOST_CHECK(url1.getSSL() == false);

	std::string url2str = "http://www.google.com/";
	HttpUrl url2(url2str);
	BOOST_CHECK(url2.empty() == false);
	BOOST_CHECK(url2.getUserName().empty());
	BOOST_CHECK(url2.getPassword().empty());
	BOOST_CHECK(url2.getHost() == "www.google.com");
	BOOST_CHECK(url2.getPort() == 80);
	BOOST_CHECK(url2.getPath() == "/");
	BOOST_CHECK(url2.toString() == url2str);
	BOOST_CHECK(url2.getSSL() == false);

	std::string url3str = "http://www.google.com";
	HttpUrl url3(url3str);
	BOOST_CHECK(url3.empty() == false);
	BOOST_CHECK(url3.getUserName().empty());
	BOOST_CHECK(url3.getPassword().empty());
	BOOST_CHECK(url3.getHost() == "www.google.com");
	BOOST_CHECK(url3.getPort() == 80);
	BOOST_CHECK(url3.getPath() == "/");
	BOOST_CHECK(url3.toString() == (url3str + "/"));
	BOOST_CHECK(url3.getSSL() == false);

	std::string url4str = "http://www.google.com/foo.php";
	HttpUrl url4(url4str);
	BOOST_CHECK(url4.empty() == false);
	BOOST_CHECK(url4.getUserName().empty());
	BOOST_CHECK(url4.getPassword().empty());
	BOOST_CHECK(url4.getHost() == "www.google.com");
	BOOST_CHECK(url4.getPort() == 80);
	BOOST_CHECK(url4.getPath() == "/foo.php");
	BOOST_CHECK(url4.toString() == url4str);
	BOOST_CHECK(url4.getSSL() == false);

	std::string url5str = "http://www.google.com:8181";
	HttpUrl url5(url5str);
	BOOST_CHECK(url5.empty() == false);
	BOOST_CHECK(url5.getUserName().empty());
	BOOST_CHECK(url5.getPassword().empty());
	BOOST_CHECK(url5.getHost() == "www.google.com");
	BOOST_CHECK(url5.getPort() == 8181);
	BOOST_CHECK(url5.getPath() == "/");
	BOOST_CHECK(url5.toString() == (url5str + "/"));
	BOOST_CHECK(url5.getSSL() == false);

	std::string url6str = "http://www.google.com:8181/foo/test.php";
	HttpUrl url6(url6str);
	BOOST_CHECK(url6.empty() == false);
	BOOST_CHECK(url6.getUserName().empty());
	BOOST_CHECK(url6.getPassword().empty());
	BOOST_CHECK(url6.getHost() == "www.google.com");
	BOOST_CHECK(url6.getPort() == 8181);
	BOOST_CHECK(url6.getPath() == "/foo/test.php");
	BOOST_CHECK(url6.toString() == url6str);
	BOOST_CHECK(url6.getSSL() == false);

	HttpUrl url7;
	BOOST_CHECK(url7.parse("foo://www.google.com") == false);
	BOOST_CHECK(url7.empty());
	BOOST_CHECK(url7.toString().empty());
	BOOST_CHECK(url7.getSSL() == false);

	std::string url8str = "http://foo@www.google.com";
	HttpUrl url8(url8str);
	BOOST_CHECK(url8.empty() == false);
	BOOST_CHECK(url8.getUserName() == "foo");
	BOOST_CHECK(url8.getPassword().empty());
	BOOST_CHECK(url8.getHost() == "www.google.com");
	BOOST_CHECK(url8.getPort() == 80);
	BOOST_CHECK(url8.getPath() == "/");
	BOOST_CHECK(url8.toString() == (url8str + "/"));
	BOOST_CHECK(url8.getSSL() == false);

	std::string url9str = "http://foo:bar@www.google.com:8181/foo/test.php";
	HttpUrl url9(url9str);
	BOOST_CHECK(url9.empty() == false);
	BOOST_CHECK(url9.getUserName() == "foo");
	BOOST_CHECK(url9.getPassword() == "bar");
	BOOST_CHECK(url9.getHost() == "www.google.com");
	BOOST_CHECK(url9.getPort() == 8181);
	BOOST_CHECK(url9.getPath() == "/foo/test.php");
	BOOST_CHECK(url9.toString() == url9str);
	BOOST_CHECK(url9.getSSL() == false);

	std::string url10str = "https://openid.claimid.com/login";		
	HttpUrl url10(url10str);
	BOOST_CHECK(url10.empty() == false);
	BOOST_CHECK(url10.getUserName().empty());
	BOOST_CHECK(url10.getPassword().empty());
	BOOST_CHECK(url10.getHost() == "openid.claimid.com");
	BOOST_CHECK(url10.getPath() == "/login");
	BOOST_CHECK(url10.getPort() == 443);
	BOOST_CHECK(url10.getSSL());

	std::string url11str = "https://openid.claimid.com:123/login";		
	HttpUrl url11(url11str);
	BOOST_CHECK(url11.empty() == false);
	BOOST_CHECK(url11.getUserName().empty());
	BOOST_CHECK(url11.getPassword().empty());
	BOOST_CHECK(url11.getHost() == "openid.claimid.com");
	BOOST_CHECK(url11.getPath() == "/login");
	BOOST_CHECK(url11.getPort() == 123);
	BOOST_CHECK(url11.getSSL());

	std::string url12str = "http://www.google.com/foo.php?alfa=beta";
	HttpUrl url12(url12str);
	BOOST_CHECK(url12.empty() == false);
	BOOST_CHECK(url12.getUserName().empty());
	BOOST_CHECK(url12.getPassword().empty());
	BOOST_CHECK(url12.getHost() == "www.google.com");
	BOOST_CHECK(url12.getPort() == 80);
	BOOST_CHECK(url12.getPath() == "/foo.php");
	BOOST_CHECK(url12.getParams() == "alfa=beta");
	BOOST_CHECK(url12.toString() == url12str);
	BOOST_CHECK(url12.getSSL() == false);

	HttpUrl url13;
	BOOST_CHECK(url13.parse(""));
	BOOST_CHECK(url13.empty());
}

BOOST_AUTO_TEST_CASE(test_http_parser)
{	
	boost::posix_time::ptime input;
	boost::posix_time::ptime output;
	std::string str;

	input = boost::posix_time::ptime(boost::gregorian::date(1982, 1, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Fri, 01 Jan 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 2, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Mon, 01 Feb 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 3, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Mon, 01 Mar 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 4, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Thu, 01 Apr 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 5, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Sat, 01 May 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 6, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Tue, 01 Jun 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 7, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Thu, 01 Jul 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 8, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Sun, 01 Aug 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 9, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Wed, 01 Sep 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 10, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Fri, 01 Oct 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 11, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Mon, 01 Nov 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1982, 12, 1), boost::posix_time::time_duration(0, 0, 0));
	str = "Wed, 01 Dec 1982 00:00:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(1980, 10, 17), boost::posix_time::time_duration(9, 30, 00));
	str = "Fri, 17 Oct 1980 09:30:00 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(2009, 3, 1), boost::posix_time::time_duration(12, 58, 32));
	str = "Sun, 01 Mar 2009 12:58:32 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(2009, 3, 2), boost::posix_time::time_duration(12, 58, 32));
	str = "Mon, 02 Mar 2009 12:58:32 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(2009, 3, 3), boost::posix_time::time_duration(12, 58, 32));
	str = "Tue, 03 Mar 2009 12:58:32 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(2009, 3, 4), boost::posix_time::time_duration(12, 58, 32));
	str = "Wed, 04 Mar 2009 12:58:32 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);	
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(2009, 3, 5), boost::posix_time::time_duration(12, 58, 32));
	str = "Thu, 05 Mar 2009 12:58:32 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);	
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(2009, 3, 6), boost::posix_time::time_duration(12, 58, 32));
	str = "Fri, 06 Mar 2009 12:58:32 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);	
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);
	input = boost::posix_time::ptime(boost::gregorian::date(2009, 3, 7), boost::posix_time::time_duration(12, 58, 32));
	str = "Sat, 07 Mar 2009 12:58:32 GMT";
	BOOST_CHECK(HttpParser::toString(input) == str);	
	output = boost::date_time::not_a_date_time;
	BOOST_CHECK(HttpParser::parseDate(str, output));
	BOOST_CHECK(input == output);

	std::string user;
	std::string password;
	BOOST_CHECK(HttpParser::parseAuthorization("Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==", user, password));	
	BOOST_CHECK(user == "Aladdin");	
	BOOST_CHECK(password == "open sesame");
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
