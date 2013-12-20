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
#include "datatree.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_datatree_construction)
{
	DataTree dt;
	BOOST_CHECK(dt.empty());
	BOOST_CHECK(dt.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_datatree_serialization)
{
	DataTree dt1;
	
	Buffer buffer;
	BOOST_CHECK(dt1.write(buffer, false, nullptr));
	BOOST_CHECK(buffer.empty() == false);

	buffer.seekToBegin();

	DataTree dt2;
	BOOST_CHECK(dt2.read(buffer));

	BOOST_CHECK(dt1 == dt2);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
