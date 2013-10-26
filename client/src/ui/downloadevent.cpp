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
#include "downloadevent.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(DownloadEvent, wxNotifyEvent)

//////////////////////////////////////////////////////////////////////

DownloadEvent::DownloadEvent(wxEventType commandType, int id) : wxNotifyEvent(commandType, id)
{
	_construct();
}

DownloadEvent::DownloadEvent(const DownloadEvent &second) : wxNotifyEvent(second)
{
	_construct(second.m_status, second.m_url, second.m_filename, second.m_filesize, second.m_downloadedBytes);
}

DownloadEvent::~DownloadEvent()
{

}

wxEvent * DownloadEvent::Clone() const 
{ 
	return new DownloadEvent(*this); 
}

void DownloadEvent::_construct(Status status, const String &url, const String &filename, uint32 filesize, uint32 downloadedBytes)
{
	m_status = status;
	m_url = url;
	m_filename = filename;
	m_filesize = filesize;
	m_downloadedBytes = downloadedBytes;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
