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
#include "portalspeer.h"

#include "ipaddress.h"
#include "lock.h"
#include "osiriscommon.h"
#include "portalsportal.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PortalsPeer::PortalsPeer(shared_ptr<boost::recursive_mutex> mutex, const boost::asio::ip::tcp::endpoint &endpoint, const String &origin) : m_mutex(mutex),
																																		 m_endpoint(endpoint),
																																		 m_origin(origin),
																																		 m_retries(0),
																																		 m_supernode(false)
{

}

PortalsPeer::~PortalsPeer()
{
		
}

uint64 PortalsPeer::getID() const
{
	OS_LOCK(*m_mutex);
	return makeID(m_endpoint);	
}

String PortalsPeer::getIP() const
{
	return m_endpoint.address().to_string().c_str();
}

uint32 PortalsPeer::getPort() const
{
	return m_endpoint.port();
}

boost::asio::ip::tcp::endpoint PortalsPeer::getEndpoint() const
{
	OS_LOCK(*m_mutex);
	return m_endpoint;
}

uint16 PortalsPeer::getRetries() const
{
	OS_LOCK(*m_mutex);
	return m_retries;
}

String PortalsPeer::getOrigin() const
{
	OS_LOCK(*m_mutex);
	return m_origin;
}

DateTime PortalsPeer::getLastContactDate() const
{
	OS_LOCK(*m_mutex);
	return m_lastContactDate;
}

bool PortalsPeer::getSupernode() const
{
	OS_LOCK(*m_mutex);
	return m_supernode;
}

bool PortalsPeer::getCacheable() const
{
	OS_LOCK(*m_mutex);

	if(m_lastContactDate.isNull())
		return false;

	if(m_retries > 0)
		return false;

	return m_supernode;
}

void PortalsPeer::setSupernode(bool supernode)
{
	OS_LOCK(*m_mutex);
	m_supernode = supernode;
}

void PortalsPeer::validate()
{
	OS_LOCK(*m_mutex);
	
	m_retries = 0;
	m_lastContactDate = DateTime::now();
}

bool PortalsPeer::invalidate()
{
	OS_LOCK(*m_mutex);

	m_lastContactDate.clear();
	return ((++m_retries) < OS_PEER_MAX_RETRIES);
}

uint64 PortalsPeer::makeID(const String &ip, uint32 port)
{
	try
	{
		return makeID(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip.to_ascii()), port));
	}
	catch(std::exception &)
	{
		OS_ASSERTFALSE();
	}

	return 0;
}

uint64 PortalsPeer::makeID(shared_ptr<IPAddress> address)
{
	if(address == null)
	{
		OS_ASSERTFALSE();
		return 0;
	}

	return makeID(address->toString(), address->getPort());
}

uint64 PortalsPeer::makeID(const boost::asio::ip::tcp::endpoint &endpoint)
{
	OS_ASSERT(endpoint.address().is_v4());
	return OS_MAKE_UINT64(endpoint.port(), endpoint.address().to_v4().to_ulong());
}

bool PortalsPeer::operator ==(const PortalsPeer &second) const
{
	//return (getIP() == second.getIP() && getPort() == second.getPort());
	return getEndpoint() == second.getEndpoint();
}

bool PortalsPeer::operator <(const PortalsPeer &second) const
{
	return getEndpoint() < second.getEndpoint();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
