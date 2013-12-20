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
#include "networksystem.h"

#include "ipaddress.h"
#include "log.h"
#include "platformmanager.h"
#include "regexmanager.h"
#include "unicode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

NetworkSystem::NetworkSystem()
{

}

NetworkSystem::~NetworkSystem()
{

}

String NetworkSystem::getLocalAddress() const
{
	char szHost[256];
    if(gethostname(szHost, sizeof(szHost)) == 0)
	{
		hostent *host = gethostbyname(szHost);
		if(host != nullptr && host->h_length == 4 && host->h_addr_list[0] != nullptr)
		{
			struct in_addr addr;
			memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));

			return inet_ntoa(addr);
		}
	}

	return String::EMPTY;
}

bool NetworkSystem::isLocalAddress(const String &address) const
{
	if(address == _S("127.0.0.1") || address == _S("localhost"))
		return true;

	return getLocalAddress() == address;
}

bool NetworkSystem::isPrivateAddress(const String &address) const
{
	struct in_addr addr;

	addr.s_addr = inet_addr(address.to_utf8().c_str());
	if(addr.s_addr != INADDR_NONE)
	{
		unsigned long haddr = ntohl(addr.s_addr);
		return ((haddr & 0xff000000) == 0x0a000000 || // 10.0.0.0/8
				(haddr & 0xff000000) == 0x7f000000 || // 127.0.0.0/8
				(haddr & 0xfff00000) == 0xac100000 || // 172.16.0.0/12
				(haddr & 0xffff0000) == 0xc0a80000);  // 192.168.0.0/16
	}

	return false;
}

bool NetworkSystem::dnsLookup(in_addr sin_addr, String &domainName)
{
    hostent *pHost = gethostbyaddr(reinterpret_cast<const char *>(&sin_addr), sizeof(sin_addr), AF_INET);
	if(pHost)
    {
        domainName = pHost->h_name;
		return true;
	}

    return false;
}

bool NetworkSystem::validateIP(const String &ip)
{
	return regex_match(ip, RegexManager::instance()->getRegex(_S("^([1-9]?\\d|1\\d\\d|2[0-4]\\d|25[0-5])\\.([1-9]?\\d|1\\d\\d|2[0-4]\\d|25[0-5])\\.([1-9]?\\d|1\\d\\d|2[0-4]\\d|25[0-5])\\.([1-9]?\\d|1\\d\\d|2[0-4]\\d|25[0-5])$")));
}

bool NetworkSystem::compareAddresses(const std::string &ip, const boost::asio::ip::address &address)
{
	try
	{
		boost::asio::ip::address second;
		if(ip.empty())
		{
			if(address.is_v4())
				second = boost::asio::ip::address_v4();
			else
				second = boost::asio::ip::address_v6();
		}
		else
		{
			second = boost::asio::ip::address::from_string(ip);
		}
		
		return second == address;
	}
	catch(std::exception &)
	{
		
	}

	return false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
