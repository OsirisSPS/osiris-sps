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

#ifndef _OS_P2P_PACKETS_QUERYREQUEST_H
#define _OS_P2P_PACKETS_QUERYREQUEST_H

#include "p2ppacketsipacket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPAddress;

//////////////////////////////////////////////////////////////////////

namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

class EngineExport QueryRequest : public IPacket
{
	typedef IPacket PacketBase;

// Construction
public:
	QueryRequest();
	virtual ~QueryRequest();

// Attributes
public:
	void getPeers(list<shared_ptr<IPAddress> >::type &addresses) const;
	void addPeer(const String &ip, uint32 port);

private:
	DataTree * _getPeers() const;
	void _createPeers();

// Packet interface
public:
	virtual bool create();

protected:
	virtual bool parse();

private:
	static const String PEERS;
};

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_PACKETS_QUERYREQUEST_H
