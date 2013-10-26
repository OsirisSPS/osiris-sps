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
#include "limitedconnection.h"

#include "bandwidth.h"
#include "bandwidthmanager.h"
#include "realtimestats.h" // URGENT

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ILimitedConnection::ILimitedConnection() : m_download(OS_NEW Bandwidth()),
										   m_upload(OS_NEW Bandwidth())
{
	BandwidthManager::instance()->addSocket(this);
}

ILimitedConnection::~ILimitedConnection()
{
	BandwidthManager::instance()->removeSocket(this);
}

uint32 ILimitedConnection::limitDownload(uint32 bytes)
{
	return BandwidthManager::instance()->limitDownload(this, bytes);
}

uint32 ILimitedConnection::limitUpload(uint32 bytes)
{
	return BandwidthManager::instance()->limitUpload(this, bytes);
}

void ILimitedConnection::handleDownload(uint32 bytes)
{
	m_download->handle(bytes);

	BandwidthManager::instance()->handleDownload(this, bytes);
}

void ILimitedConnection::handleUpload(uint32 bytes)
{
	m_upload->handle(bytes);

	BandwidthManager::instance()->handleUpload(this, bytes);	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
