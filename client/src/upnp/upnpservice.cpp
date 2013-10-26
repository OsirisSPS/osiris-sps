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
#include "upnpservice.h"

#include "boost/bind.hpp"
#include "boost/thread/once.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "lock.h"
#include "miniupnp.h"
#include "upnpmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

UPnPService::UPnPService() : m_mapped(false),
							 m_port(0)
{

}

UPnPService::~UPnPService()
{
	removePortMapping();
}

bool UPnPService::addPortMapping(const boost::asio::ip::tcp::endpoint &endpoint)
{
	return addPortMapping(endpoint.port(), OS_MINIUPNP_PROTOCOL_TCP);
}


bool UPnPService::addPortMapping(const boost::asio::ip::udp::endpoint &endpoint)
{
	return addPortMapping(endpoint.port(), OS_MINIUPNP_PROTOCOL_UDP);
}

bool UPnPService::addTcpPortMapping(uint32 port)
{
	return addPortMapping(port, OS_MINIUPNP_PROTOCOL_TCP);
}

bool UPnPService::addUdpPortMapping(uint32 port)
{
	return addPortMapping(port, OS_MINIUPNP_PROTOCOL_UDP);
}

bool UPnPService::removePortMapping()
{
	if(m_mapped == false)
	{
		OS_ASSERT(m_port == 0);
		OS_ASSERT(m_protocol.empty());

		return true;
	}

	bool done = UPnPManager::instance()->removePortMapping(m_port, m_protocol);
	
	m_mapped = false;
	m_port = 0;
	m_protocol.clear();

	return done;
}

bool UPnPService::addPortMapping(uint32 port, const std::string &protocol)
{
	removePortMapping();

	if(UPnPManager::instance()->addPortMapping(port, protocol) == false)
		return false;

	m_mapped = true;
	m_port = port;
	m_protocol = protocol;
	
	return true;	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
