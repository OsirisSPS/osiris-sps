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

#include "boostasio.h"
#include "networksystem.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const IPAddress IPAddress::ANY(0, 0);

const uint32 IPAddress::any_addr = 0;
const uint32 IPAddress::any_port = 0;

//////////////////////////////////////////////////////////////////////

IPAddress::IPAddress()
{
	_construct(any_addr, any_port);
}

IPAddress::IPAddress(uint8 a, uint8 b, uint8 c, uint8 d, uint32 port)
{
	_construct(((a << 24) + (b << 16) + (c << 8) + d), port);
}

IPAddress::IPAddress(uint32 ip, uint32 port)
{
	_construct(ip, port);
}

IPAddress::IPAddress(const IPAddress &second) : BaseClass(second)
{
	_construct(second.m_ip, second.m_port);
}

IPAddress::~IPAddress()
{

}

void IPAddress::_construct(uint32 ip, uint32 port)
{
	m_ip = ip;
	m_port = port;
}

bool IPAddress::isLocalAddress() const
{
	return NetworkSystem::instance()->isLocalAddress(toString());
}

bool IPAddress::isPrivateAddress() const
{
	return NetworkSystem::instance()->isPrivateAddress(toString());
}

bool IPAddress::isAny() const
{
	return *this == ANY;
}

bool IPAddress::setEndpoint(const String &ip, uint32 port)
{
	clear();

	if(fromString(ip) == false)
		return false;

	setPort(port);

	return true;
}

sockaddr_in IPAddress::toNative() const
{
	sockaddr_in in;
	OS_ZEROMEMORY(&in, sizeof(in));

	if(m_ip == any_addr)
		in.sin_addr.s_addr = INADDR_ANY;
	else
		in.sin_addr.s_addr = htonl(m_ip);

	if(m_port == any_port)
		in.sin_port = 0;
	else
		in.sin_port = htons(m_port);

	in.sin_family = AF_INET;
	return in;
}


String IPAddress::toString() const
{
	in_addr in;
	in.s_addr = htonl(m_ip);
	return inet_ntoa(in);
}

bool IPAddress::fromString(const String &ip)
{
	try
	{
		m_ip = boost::asio::ip::address::from_string(ip.to_ascii()).to_v4().to_ulong();
		return true;
	}
	catch(std::exception &)
	{

	}

	return false;
}

void IPAddress::clear()
{
	*this = ANY;
}

IPAddress & IPAddress::operator=(const IPAddress &second)
{
	m_ip = second.m_ip;
	m_port = second.m_port;

	return *this;
}

bool IPAddress::operator <=(const IPAddress &second) const
{
    if(m_ip == second.m_ip)
		return m_port <= second.m_port;

	return m_ip <= second.m_ip;
}

bool IPAddress::operator<(const IPAddress &second) const
{
	if(m_ip == second.m_ip)
		return m_port < second.m_port;

	return m_ip < second.m_ip;
}

bool IPAddress::operator!=(const IPAddress &second) const
{
	return m_ip != second.m_ip || m_port != second.m_port;
}

bool IPAddress::operator==(const IPAddress &second) const
{
	return m_ip == second.m_ip && m_port == second.m_port;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
