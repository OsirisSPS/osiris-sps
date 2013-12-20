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
#include "p2pnodestatus.h"

#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

NodeStatus::NodeStatus(bool outgoing) : m_outgoing(outgoing),
										m_status(statusInitializing),
									    m_downloadedBytes(0),
									    m_uploadedBytes(0),
									    m_downloadedObjects(0),
									    m_uploadedObjects(0)						   
{

}

NodeStatus::~NodeStatus()
{

}

bool NodeStatus::getOutgoing() const
{
	OS_LOCK(m_cs);
	return m_outgoing;
}

shared_ptr<Portal> NodeStatus::getPortal() const
{
	OS_LOCK(m_cs);
	return m_portal.lock();
}

void NodeStatus::setPortal(shared_ptr<Portal> portal)
{
	OS_LOCK(m_cs);

	m_portal = portal;
	m_modifiedFlag = true;
}

NodeStatus::Status NodeStatus::getStatus() const
{
	OS_LOCK(m_cs);
	return m_status;
}

void NodeStatus::setStatus(Status status)
{
	OS_LOCK(m_cs);

	if(m_status != status)
	{
		m_status = status;
		m_modifiedFlag = true;
	}
}

uint32 NodeStatus::getDownloadedBytes() const
{
	OS_LOCK(m_cs);
	return m_downloadedBytes;
}

uint32 NodeStatus::getUploadedBytes() const
{
	OS_LOCK(m_cs);
	return m_uploadedBytes;
}

uint32 NodeStatus::getDownloadedObjects() const
{
	OS_LOCK(m_cs);
	return m_downloadedObjects;
}

uint32 NodeStatus::getUploadedObjects() const
{
	OS_LOCK(m_cs);
	return m_uploadedObjects;
}

String NodeStatus::getLastEvent() const
{
	OS_LOCK(m_cs);
	return m_lastEvent;
}

void NodeStatus::setLastEvent(const String &lastEvent)
{
	OS_LOCK(m_cs);

	m_lastEvent = lastEvent;
	m_modifiedFlag = true;
}

shared_ptr<IPAddress> NodeStatus::getPeer() const
{
	OS_LOCK(m_cs);
	return m_peer;
}

void NodeStatus::setPeer(shared_ptr<IPAddress> peer)
{
	OS_LOCK(m_cs);

	m_peer = peer;
	m_modifiedFlag = true;
}

void NodeStatus::addDownloadedBytes(uint32 amount)
{
	OS_LOCK(m_cs);

	m_downloadedBytes += amount;
	m_modifiedFlag = true;
}

void NodeStatus::addUploadedBytes(uint32 amount)
{
	OS_LOCK(m_cs);

	m_uploadedBytes += amount;
	m_modifiedFlag = true;
}

void NodeStatus::addDownloadedObject()
{
	OS_LOCK(m_cs);

	m_downloadedObjects++;
	m_modifiedFlag = true;
}

void NodeStatus::addUploadedObject()
{
	OS_LOCK(m_cs);

	m_uploadedObjects++;
	m_modifiedFlag = true;
}

shared_ptr<NodeStatus> NodeStatus::clone() const
{
	OS_LOCK(m_cs);

	shared_ptr<NodeStatus> connectionStatus(OS_NEW NodeStatus(m_outgoing));
	connectionStatus->m_status = m_status;
	connectionStatus->m_portal = m_portal;
	connectionStatus->m_downloadedBytes = m_downloadedBytes;
	connectionStatus->m_uploadedBytes = m_uploadedBytes;
	connectionStatus->m_downloadedObjects = m_downloadedObjects;
	connectionStatus->m_uploadedObjects = m_uploadedObjects;
	connectionStatus->m_lastEvent = m_lastEvent;
	if(m_peer != nullptr)
		connectionStatus->m_peer.reset(OS_NEW IPAddress(*m_peer));

	return connectionStatus;
}

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
