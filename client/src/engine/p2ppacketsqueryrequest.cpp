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
#include "p2ppacketsqueryrequest.h"

#include "datatree.h"
#include "ipaddress.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

const String QueryRequest::PEERS = _S("peers");

//////////////////////////////////////////////////////////////////////

QueryRequest::QueryRequest() : PacketBase(ptQueryRequest)
{

}

QueryRequest::~QueryRequest()
{

}

void QueryRequest::getPeers(list<shared_ptr<IPAddress> >::type &addresses) const
{
	DataTree *pPeers = _getPeers();
	if(pPeers != nullptr)
	{
		for(DataTree::iterator i = pPeers->begin(); i != pPeers->end(); ++i)
		{
			shared_ptr<IPAddress> address(OS_NEW IPAddress());
			if(address->setEndpoint(i->first, static_cast<uint32>(*i->second)))
				addresses.push_back(address);
		}
	}
}

void QueryRequest::addPeer(const String &ip, uint32 port)
{
	_createPeers();
	_getPeers()->setV(ip, port);
}

DataTree * QueryRequest::_getPeers() const
{
	return getData()->getTP(PEERS);
}

void QueryRequest::_createPeers()
{
	if(getData()->exists(PEERS) == false)
		getData()->setT(PEERS);
}

bool QueryRequest::create()
{
	return true;
}

bool QueryRequest::parse()
{
	return true;
}

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
