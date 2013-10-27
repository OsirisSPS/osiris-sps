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

#ifndef _OS_UI_DOWNLOADTHREAD_H
#define _OS_UI_DOWNLOADTHREAD_H

#include "thread.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DownloadEvent;

//////////////////////////////////////////////////////////////////////

class DownloadThread : public Thread
{
// Construction
public:
	DownloadThread(wxEvtHandler *handler, const String &url, const String &filename, bool notifyDownloading = false, uint32 notifyBytes = 1024);
	virtual ~DownloadThread();

// Operations
private:
	void _fireEvent(DownloadEvent &e);

// Thread Interface
protected:
	virtual void run();

private:
	wxEvtHandler *m_handler;	// events handler
	String m_url;				// URL from where to download the file
	String m_filename;			// local file name where the downloaded file would be saved
	uint32 m_filesize;			// size of the file downloading; may be wrong also, depends on the server.
	bool m_notifyDownloading;	// Enable or disable the downloading in progress notification.
	uint32 m_notifyBytes;		// After how many number of bytes downloaded, to send the notification.
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_DOWNLOADTHREAD_H
