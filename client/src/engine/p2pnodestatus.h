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

#ifndef _OS_P2P_NODESTATUS_H
#define _OS_P2P_NODESTATUS_H

#include "connectionstatus.h"
#include "ipaddress.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Portal;

//////////////////////////////////////////////////////////////////////

namespace p2p {

//////////////////////////////////////////////////////////////////////

class EngineExport NodeStatus : public ConnectionStatus
{
// Enumerated types
public:
	enum Status
	{
		statusInitializing			= 0,
		statusAttemptingConnection	= 1,
		statusHandshaking			= 2,
		statusExchanging			= 3,
		statusTerminating			= 4,
	};

// Construction
public:
	NodeStatus(bool outgoing);
	virtual ~NodeStatus();

// Attributes
public:
	bool getOutgoing() const;

	shared_ptr<Portal> getPortal() const;
	void setPortal(shared_ptr<Portal> portal);

	Status getStatus() const;
	void setStatus(Status status);

	uint32 getDownloadedBytes() const;
	uint32 getUploadedBytes() const;

	uint32 getDownloadedObjects() const;
	uint32 getUploadedObjects() const;

	String getLastEvent() const;
	void setLastEvent(const String &lastEvent);

	shared_ptr<IPAddress> getPeer() const;
	void setPeer(shared_ptr<IPAddress> peer);

// Operations
public:
	void addDownloadedBytes(uint32 amount);
	void addUploadedBytes(uint32 amount);

	void addDownloadedObject();
	void addUploadedObject();

	shared_ptr<NodeStatus> clone() const;

private:
	weak_ptr<Portal> m_portal;
	bool m_outgoing;
	Status m_status;
	uint32 m_downloadedBytes;
	uint32 m_uploadedBytes;
	uint32 m_downloadedObjects;
	uint32 m_uploadedObjects;
	String m_lastEvent;
	shared_ptr<IPAddress> m_peer;	
};

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_NODESTATUS_H
