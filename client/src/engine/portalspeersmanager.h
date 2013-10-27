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

#ifndef _OS_ENGINE_PORTALPEERSMANAGER_H
#define _OS_ENGINE_PORTALPEERSMANAGER_H

#include "base/object.h"
#include "datetime.h"
#include "eventsource.h"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPAddress;
class IPortalDatabase;
class Portal;
class PortalsPeer;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport PortalsPeersManager : public Object
{
// Construction
public:
	PortalsPeersManager(Portal &portal, uint32 peersCacheSize);
	virtual ~PortalsPeersManager();

// Attributes
public:
	bool empty() const;

	uint32 getPeersCacheSize() const;
	void setPeersCacheSize(uint32 size);

	/*
	shared_ptr<EventSource> getEventStorePeer() const;
	*/

// Operations
public:
	bool savePeer(shared_ptr<IPAddress> address, bool validate, const String &origin);
	bool savePeer(const String &ip, uint32 port, bool validate, const String &origin);
	void validatePeer(shared_ptr<IPAddress> address);
	void invalidatePeer(shared_ptr<IPAddress> address);
	void removePeer(shared_ptr<IPAddress> address);
	void removePeer(uint64 id);

	shared_ptr<PortalsPeer> peekPeer();

	void addSupernode(shared_ptr<IPAddress> address);
	void extractSupernodes(uint32 count, list<shared_ptr<PortalsPeer> >::type &peers);

	void getPeersInfo(uint32 &activePeers, uint32 &activeSupernodes);

	DateTime getContactDateCheck() const;

	bool loadPeers(shared_ptr<IPortalDatabase> database);
	bool savePeers(shared_ptr<IPortalDatabase> database);

	void exportXML(shared_ptr<XMLNode> node) const;

private:
	pimpl_of<PortalsPeersManager>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_PORTALPEERSMANAGER_H
