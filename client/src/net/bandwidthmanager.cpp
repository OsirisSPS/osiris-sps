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
#include "bandwidthmanager.h"

#include "boost/bind.hpp"
#include "limitedconnection.h"
#include "lock.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BandwidthManager::Interval::Interval()
{
	m_downloaded = 0;
	m_uploaded = 0;
	m_clock = PlatformManager::instance()->tickCount();
}

BandwidthManager::Interval::~Interval()
{

}

bool BandwidthManager::Interval::isElapsed() const
{
	OS_LOCK(m_cs);
	return (PlatformManager::instance()->tickCount() - m_clock) > OS_NET_BANDWIDTH_CLOCK;
}

uint32 BandwidthManager::Interval::getDownloadedBytes() const
{
	OS_LOCK(m_cs);
	return m_downloaded;
}

uint32 BandwidthManager::Interval::getUploadedBytes() const
{
	OS_LOCK(m_cs);
	return m_uploaded;
}

void BandwidthManager::Interval::handleDownload(uint32 bytes)
{
	OS_LOCK(m_cs);
	m_downloaded += bytes;
}

void BandwidthManager::Interval::handleUpload(uint32 bytes)
{
	OS_LOCK(m_cs);
	m_uploaded += bytes;
}

//////////////////////////////////////////////////////////////////////

BandwidthManager::BandwidthManager()
{
	m_maxUpload = OS_NET_UNLIMITED_BANDWIDTH;
	m_maxDownload = OS_NET_UNLIMITED_BANDWIDTH;

	m_totalDownloaded = 0;
	m_totalUploaded = 0;

	m_downloadSpeed = 0;
	m_uploadSpeed = 0;
}

BandwidthManager::~BandwidthManager()
{

}

uint32 BandwidthManager::getMaxDownload() const
{
	OS_LOCK(m_cs);

	return m_maxDownload;
}

void BandwidthManager::setMaxDownload(uint32 bytes)
{
	OS_LOCK(m_cs);

	m_maxDownload = bytes;
}

uint32 BandwidthManager::getMaxUpload() const
{
	OS_LOCK(m_cs);

	return m_maxUpload;
}

void BandwidthManager::setMaxUpload(uint32 bytes)
{
	OS_LOCK(m_cs);

	m_maxUpload = bytes;
}

BandwidthManager::interval_ptr BandwidthManager::getInterval() const
{
	OS_LOCK(m_cs);

	if(m_interval == nullptr)
	{
		m_interval.reset(OS_NEW Interval());
	}
	else if(m_interval->isElapsed())
	{
		m_prevInterval = m_interval;
		m_interval.reset(OS_NEW Interval());
	}

	return m_interval;
}

void BandwidthManager::addSocket(ILimitedConnection *socket)
{
	OS_ASSERT(socket != nullptr);
	OS_LOCK(m_cs);
	
	OS_ASSERT(m_sockets.find(socket) == m_sockets.end());
	m_sockets.insert(socket);
}

void BandwidthManager::removeSocket(ILimitedConnection *socket)
{
	OS_ASSERT(socket != nullptr);
	OS_LOCK(m_cs);

	OS_ASSERT(m_sockets.find(socket) != m_sockets.end());
	m_sockets.erase(socket);
}

uint32 BandwidthManager::limitDownload(ILimitedConnection *socket, uint32 requiredDownload) const
{
	OS_ASSERT(socket != nullptr);
	OS_LOCK(m_cs);

	if(m_maxDownload != OS_NET_UNLIMITED_BANDWIDTH)
	{
		// Corregge il numero di bytes da scaricare
		requiredDownload = _adjustBandwidth(m_maxDownload, getInterval()->getDownloadedBytes(), requiredDownload, socket->getDownload());
	}

	return requiredDownload;
}

uint32 BandwidthManager::limitUpload(ILimitedConnection *socket, uint32 requiredUpload) const
{
	OS_ASSERT(socket != nullptr);
	OS_LOCK(m_cs);

	if(m_maxUpload != OS_NET_UNLIMITED_BANDWIDTH)
	{
		// Corregge il numero di bytes da inviare
		requiredUpload = _adjustBandwidth(m_maxUpload, getInterval()->getUploadedBytes(), requiredUpload, socket->getUpload());
	}

	return requiredUpload;
}

void BandwidthManager::handleDownload(ILimitedConnection *socket, uint32 bytes)
{
	OS_ASSERT(socket != nullptr);
	OS_LOCK(m_cs);

	// Aggiorna il numero di bytes scaricati nell'intervallo corrente
	getInterval()->handleDownload(bytes);
	// Aggiorna il numero totale di bytes scaricati
	m_totalDownloaded += bytes;
}

void BandwidthManager::handleUpload(ILimitedConnection *socket, uint32 bytes)
{
	OS_ASSERT(socket != nullptr);
	OS_LOCK(m_cs);

	// Aggiorna il numero di bytes inviati nell'intervallo corrente
	getInterval()->handleUpload(bytes);
	// Aggiorna il numero totale di bytes inviati
	m_totalUploaded += bytes;
}

uint32 BandwidthManager::_adjustBandwidth(uint32 maxBytes, uint32 usedBytes, uint32 requiredBytes, shared_ptr<Bandwidth> bandwidth) const
{
	// TODO: fixme

	/*
	OS_ASSERT(usedBytes <= maxBytes);		// Il numero di bytes usati non dovrebbe mai superare il consentito
	if(usedBytes >= maxBytes)
	{
		OS_ASSERTFALSE();
		return 0
	}
	uint32 availableBytes = maxBytes - usedBytes;
	OS_ASSERT(availableBytes > 0);			// Per la condizione in testa

	uint32 count = static_cast<uint32>(m_sockets.size());
	*/

	return requiredBytes;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
