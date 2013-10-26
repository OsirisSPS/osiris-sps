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

#ifndef _OS_NET_NETWORKSYSTEM_H
#define _OS_NET_NETWORKSYSTEM_H

#include "boostasio.h"
#include "pimpl_ptr.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class NetExport NetworkSystem : public StaticSingleton<NetworkSystem>
{
// Construction
public:
	NetworkSystem();
	virtual ~NetworkSystem();

// Attributes
public:
	String getLocalAddress() const;

	bool isLocalAddress(const String &address) const;
	bool isPrivateAddress(const String &address) const;

// Operations
public:
	bool dnsLookup(in_addr sin_addr, String &domainName);
	bool validateIP(const String &ip);

	bool compareAddresses(const std::string &ip, const boost::asio::ip::address &address);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_NETWORKSYSTEM_H
