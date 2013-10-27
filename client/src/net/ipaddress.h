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

#ifndef _OS_NET_IPADDRESS_H
#define _OS_NET_IPADDRESS_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class NetExport IPAddress : public Object
{
    typedef Object BaseClass;

// Construction
public:
	IPAddress();
	IPAddress(uint8 a, uint8 b, uint8 c, uint8 d, uint32 port);
	IPAddress(uint32 ip, uint32 port);
	IPAddress(const IPAddress &second);
	virtual ~IPAddress();

private:
	void _construct(uint32 ip, uint32 port);

// Attributes
public:
	inline uint32 getIP() const;
	inline void setIP(uint32 ip);

	inline uint32 getPort() const;
	inline void setPort(uint32 port);

	bool isLocalAddress() const;
	bool isPrivateAddress() const;
	bool isAny() const;

	bool setEndpoint(const String &ip, uint32 port);

// Operations
public:
	sockaddr_in toNative() const;

	String toString() const;
	bool fromString(const String &ip);

	void clear();

// Operators
public:
	IPAddress & operator=(const IPAddress &second);

    bool operator<=(const IPAddress &second) const;
	bool operator<(const IPAddress &second) const;

	bool operator!=(const IPAddress &second) const;
	bool operator==(const IPAddress &second) const;

private:
	uint32 m_ip;
	uint32 m_port;

public:
	static const IPAddress ANY;

	static const uint32 any_addr;
	static const uint32 any_port;
};

//////////////////////////////////////////////////////////////////////

inline uint32 IPAddress::getIP() const { return m_ip; }
inline void IPAddress::setIP(uint32 ip) { m_ip = ip; }

inline uint32 IPAddress::getPort() const { return m_port; }
inline void IPAddress::setPort(uint32 port) { m_port = port; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_IPADDRESS_H
