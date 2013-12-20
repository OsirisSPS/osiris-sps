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

#include "osiriscommon.h"
#include "osirislink.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static void test_empty_shell_link(OsirisLink &link)
{
	BOOST_CHECK(link.empty());
	BOOST_CHECK(link.toString().empty());
	BOOST_CHECK(link.getType() == OsirisLink::linkInvalid);
	//BOOST_CHECK(link.getLink() == nullptr);
}

BOOST_AUTO_TEST_CASE(test_shell_link_common)
{
	OsirisLink link;
	test_empty_shell_link(link);	
	BOOST_CHECK(link.parse("http://foo.bar") == false);
	test_empty_shell_link(link);
	BOOST_CHECK(link.parse("osiris://|foo|") == false);
	test_empty_shell_link(link);
	BOOST_CHECK(link.parse("osiris://|portal|") == false);
	test_empty_shell_link(link);
	BOOST_CHECK(link.parse("osiris://|isis|") == false);
	test_empty_shell_link(link);
	BOOST_CHECK(link.parse("osiris://portal|010000017D017268CEBCDB28B8BACBD7462758405A4D1234|") == false);
	test_empty_shell_link(link);	
	String zz = _S("AA8D92B265396A4AD7BA3C5567D9FF860448123");
	BOOST_CHECK(link.parse("osiris://|portal|11000001AA8D92B265396A4AD7BA3C5567D9FF860448123|") == false);	// Lunghezza hash non corretta
	BOOST_CHECK(link.parse("osiris://|portal|000000027D017268CEBCDB28B8BACBD7462758405A4D1234|") == false);	// Tipologia ID != portale	
	BOOST_CHECK(link.parse("osiris://|isis|000000027D017268CEBCDB28B8BACBD7462758405A4D1234|") == false);	// Tipologia ID != portale	
}

BOOST_AUTO_TEST_CASE(test_shell_link_portal)
{
	{
		std::string strO = "osiris://|portal|010000017D017268CEBCDB28B8BACBD7462758405A4D1234|name=Sample%20Title|description=Sample%20Description|";
		std::string strN = "osiris:?type=portal&portal=010000017D017268CEBCDB28B8BACBD7462758405A4D1234&name=Sample%20Title&description=Sample%20Description";
		
		OsirisLink linkO;
		BOOST_CHECK(linkO.parse(strO));
		BOOST_CHECK(linkO.empty() == false);		
		BOOST_CHECK(linkO.getType() == OsirisLink::linkPortal);
		BOOST_CHECK(linkO.getPortal().getString() == "010000017D017268CEBCDB28B8BACBD7462758405A4D1234");
		BOOST_CHECK(linkO.getName() == _S("Sample Title"));
		BOOST_CHECK(linkO.getDescription() == _S("Sample Description"));
		BOOST_CHECK(linkO.getPov().empty());
		BOOST_CHECK(linkO.getParams().size() == 4);

		OsirisLink linkN;
		BOOST_CHECK(linkN.parse(strN));
		BOOST_CHECK(linkN.empty() == false);		
		BOOST_CHECK(linkN.getType() == OsirisLink::linkPortal);
		BOOST_CHECK(linkN.getPortal().getString() == "010000017D017268CEBCDB28B8BACBD7462758405A4D1234");
		BOOST_CHECK(linkN.getName() == _S("Sample Title"));
		BOOST_CHECK(linkN.getDescription() == _S("Sample Description"));
		BOOST_CHECK(linkN.getPov().empty());
		BOOST_CHECK(linkN.getParams().size() == 4);


		OsirisLink link2;
		link2.setParam("type", _W("portal"));
		link2.setParam("portal", _W("010000017D017268CEBCDB28B8BACBD7462758405A4D1234"));
		link2.setParam("name", _W("Sample Title"));
		link2.setParam("description", _W("Sample Description"));

#ifdef OS_OSIRIS_VERSION_RAZOR
		BOOST_CHECK(linkO.generate() == strN);
		BOOST_CHECK(linkN.generate() == strN);
#else
		BOOST_CHECK(linkO.generate() == strO);
		BOOST_CHECK(linkN.generate() == strO);
#endif
	}	
	
}

BOOST_AUTO_TEST_CASE(test_shell_link_isis)
{

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
