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

#include "datetime.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_datetime_construction)
{
	DateTime dt;
	BOOST_CHECK(dt.isNull());
	BOOST_CHECK(dt.isValid() == false);
	BOOST_CHECK(dt.fromString(_S("19801017053259")));
	BOOST_CHECK(dt.getYear() == 1980);
	BOOST_CHECK(dt.getMonth() == 10);
	BOOST_CHECK(dt.getDay() == 17);
	BOOST_CHECK(dt.getHours() == 5);
	BOOST_CHECK(dt.getMinutes() == 32);
	BOOST_CHECK(dt.getSeconds() == 59);
	BOOST_CHECK(dt.getDayOfWeek() == 5);
}

BOOST_AUTO_TEST_CASE(test_datetime_conversions)
{
	DateTime dt;
	BOOST_CHECK(dt.toString() == DateTime::EMPTY.toString());		// Non è String::EMPTY ma 00000000000000
	BOOST_CHECK(dt.fromString(String::EMPTY));
	BOOST_CHECK(dt == DateTime::EMPTY);
	BOOST_CHECK(dt.fromString(DateTime::EMPTY.toString()));
	BOOST_CHECK(dt == DateTime::EMPTY);

	BOOST_CHECK(dt.fromString(_S("20001102053200")));

	String str = dt.toString();

	BOOST_CHECK(str == _S("20001102053200"));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
