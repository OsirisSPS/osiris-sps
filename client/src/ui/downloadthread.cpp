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
#include "downloadthread.h"

#include "conversions.h"
#include "downloadevent.h"
#include "uiapi.h"
#include "uistandards.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DownloadThread::DownloadThread(wxEvtHandler *handler, const String &url, const String &filename, bool notifyDownloading, uint32 notifyBytes)
{
    m_handler = handler;
	m_url = url;
	m_filename = filename;
	m_filesize = 0;
	m_notifyDownloading = notifyDownloading;
	m_notifyBytes = notifyBytes;
}

DownloadThread::~DownloadThread()
{

}

void DownloadThread::_fireEvent(DownloadEvent &e)
{
	if(m_handler != nullptr)
		//m_handler->ProcessEvent(event);
		wxPostEvent(m_handler, e);
}

void DownloadThread::run()
{
	DownloadEvent e(wxEVT_DOWNLOAD);
	e.setUrl(m_url);
	e.setFilename(m_filename);
	wxURL Url(conversions::from_utf16<wxString>(m_url));
	Url.GetProtocol().SetTimeout(100);
	if(Url.GetError() == wxURL_NOERR)
	{
		wxInputStream *pIn_Stream = Url.GetInputStream();
		if(pIn_Stream != nullptr)
		{
			m_filesize = static_cast<uint32>(pIn_Stream->GetSize());
			if(m_filesize != 0xFFFFFFFF)
				e.setFilesize(m_filesize);

			e.setStatus(DownloadEvent::dsStarting);
			_fireEvent(e);

		    byte buffer[OS_NET_DEFAULT_BUFFER_SIZE];
			uint32 bytesread = 0;

			wxFile file;
			wxInt64 nCount = 0;
			file.Create(conversions::from_utf16<wxString>(m_filename), true);
			while(running() && (bytesread = (int)(pIn_Stream->Read(buffer, OS_NET_DEFAULT_BUFFER_SIZE)).LastRead()) > 0)
			{
				file.Write(buffer, bytesread);
				nCount += bytesread;
				if(m_notifyDownloading && (nCount % m_notifyBytes) == 0 && nCount >= m_notifyBytes)
				{
					e.setStatus(DownloadEvent::dsDownloading);
					e.setDownloadedBytes(nCount);
					_fireEvent(e);
				}

				yield();
			}
			file.Close();
			delete pIn_Stream;

			if(stopped())
			{
				e.setStatus(DownloadEvent::dsCanceled);
				_fireEvent(e);
			}
			else
			{
				e.setStatus(DownloadEvent::dsCompleted);
				_fireEvent(e);
			}
		}
		else
		{
			e.setStatus(DownloadEvent::dsFailed);
			_fireEvent(e);
		}
	}
	else
	{
		e.setStatus(DownloadEvent::dsFailed);
		_fireEvent(e);
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
