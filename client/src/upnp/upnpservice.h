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

#ifndef _OS_UPNP_UPNPSERVICE_H
#define _OS_UPNP_UPNPSERVICE_H

#include "base/object.h"
#include "boostasio.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class UPnPExport UPnPService : public Object,
							   public boost::noncopyable
{	
	typedef Object BaseClass;

// Construction
public:
	UPnPService();
	virtual ~UPnPService();

// Attributes
public:
	inline bool getMapped() const;

// Operations
public:
	bool addPortMapping(const boost::asio::ip::tcp::endpoint &endpoint);
	bool addPortMapping(const boost::asio::ip::udp::endpoint &endpoint);

	bool addTcpPortMapping(uint32 port);
	bool addUdpPortMapping(uint32 port);

	bool removePortMapping();

private:
	bool addPortMapping(uint32 port, const std::string &protocol);	

private:
	bool m_mapped;
	uint32 m_port;
	std::string m_protocol;
};

//////////////////////////////////////////////////////////////////////

inline bool UPnPService::getMapped() const { return m_mapped; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UPNP_UPNPSERVICE_H
