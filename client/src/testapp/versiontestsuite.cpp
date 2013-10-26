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

#include "version.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_version)
{
	Version v;
	BOOST_CHECK(v.empty());
	BOOST_CHECK(v == Version::EMPTY);
	BOOST_CHECK(v.get() == 0);
	BOOST_CHECK(v.getMinor() == 0);
	BOOST_CHECK(v.getMajor() == 0);
	BOOST_CHECK(v.toString() == "0.0");
	BOOST_CHECK(v.fromString("foo") == false);
	BOOST_CHECK(v.fromString("5.12"));
	BOOST_CHECK(v.getMinor() == 12);
	BOOST_CHECK(v.getMajor() == 5);
	BOOST_CHECK(v.toString() == "5.12");
	BOOST_CHECK(v.get() == OS_MAKE_UINT16(12, 5));
	BOOST_CHECK(OS_UINT16_LO(v.get()) == 12);	
	BOOST_CHECK(OS_UINT16_HI(v.get()) == 5);	
	BOOST_CHECK(v.empty() == false);
	BOOST_CHECK(v != Version::EMPTY);
	BOOST_CHECK(v == Version(12, 5));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
