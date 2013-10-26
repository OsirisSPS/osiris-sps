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

#include "ipaddress.h"
#include "networksystem.h"
#include "ntp.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_net_ipaddress)
{
	static const String fakeIP = _S("?xx__foo__xx?");

	IPAddress address;

	BOOST_CHECK(address.getIP() == IPAddress::any_addr);
	BOOST_CHECK(address.getPort() == IPAddress::any_port);
	BOOST_CHECK(address.isAny());
	BOOST_CHECK(address.isLocalAddress() == false);
	BOOST_CHECK(address.isPrivateAddress() == false);

	BOOST_CHECK(address.fromString(fakeIP) == false);
	BOOST_CHECK(address.getIP() == IPAddress::any_addr);
	BOOST_CHECK(address.getPort() == IPAddress::any_port);
	BOOST_CHECK(address.isAny());
	BOOST_CHECK(address.isLocalAddress() == false);
	BOOST_CHECK(address.isPrivateAddress() == false);

	BOOST_CHECK(address.fromString("127.0.0.1"));
	BOOST_CHECK(address.getIP() == 2130706433);
	BOOST_CHECK(address.getPort() == IPAddress::any_port);
	BOOST_CHECK(address.isAny() == false);
	BOOST_CHECK(address.isLocalAddress());
	BOOST_CHECK(address.isPrivateAddress());
	BOOST_CHECK(address == IPAddress(2130706433, IPAddress::any_port));
	BOOST_CHECK(address == IPAddress(127, 0, 0, 1, IPAddress::any_port));

	BOOST_CHECK(address.fromString("192.168.5.10"));
	BOOST_CHECK(address.getIP() == 3232236810);
	BOOST_CHECK(address.getPort() == IPAddress::any_port);
	BOOST_CHECK(address.isAny() == false);
	BOOST_CHECK(address.isPrivateAddress());
	BOOST_CHECK(address == IPAddress(3232236810, IPAddress::any_port));
	BOOST_CHECK(address == IPAddress(192, 168, 5, 10, IPAddress::any_port));

	address.clear();

	BOOST_CHECK(address.getIP() == IPAddress::any_addr);
	BOOST_CHECK(address.getPort() == IPAddress::any_port);
	BOOST_CHECK(address.isAny());
	BOOST_CHECK(address.isLocalAddress() == false);
	BOOST_CHECK(address.isPrivateAddress() == false);

	BOOST_CHECK(address.setEndpoint("192.168.5.11", 55));
	BOOST_CHECK(address.getIP() == 3232236811);
	BOOST_CHECK(address.getPort() == 55);
	BOOST_CHECK(address.isAny() == false);
	BOOST_CHECK(address.isPrivateAddress());
	BOOST_CHECK(address == IPAddress(3232236811, 55));
	BOOST_CHECK(address == IPAddress(192, 168, 5, 11, 55));

	BOOST_CHECK(address.setEndpoint(fakeIP, 100) == false);
	BOOST_CHECK(address.getIP() == 0);
	BOOST_CHECK(address.getPort() == 0);
	BOOST_CHECK(address.isAny());
	BOOST_CHECK(address.isLocalAddress() == false);
	BOOST_CHECK(address.isPrivateAddress() == false);
	BOOST_CHECK(address == IPAddress());

    String localAddress = NetworkSystem::instance()->getLocalAddress();
	BOOST_CHECK(address.setEndpoint(localAddress, 250));
	BOOST_CHECK(address.getIP() != IPAddress::any_addr);
	BOOST_CHECK(address.toString() == localAddress);
	BOOST_CHECK(address.getPort() == 250);
	BOOST_CHECK(address.isAny() == false);
	BOOST_CHECK(address.isLocalAddress());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
