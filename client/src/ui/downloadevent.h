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

#ifndef _OS_UI_DOWNLOADEVENT_H
#define _OS_UI_DOWNLOADEVENT_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DownloadEvent : public wxNotifyEvent
{
	DECLARE_DYNAMIC_CLASS(DownloadEvent);

// Enumerated types
public:
	enum Status
	{
		dsNone			= 0,	// Download non inizializzato
		dsFailed		= 1,	// Download fallito
		dsStarting		= 2,	// Download in fase di avvio
		dsCompleted		= 3,	// Download completato
		dsDownloading	= 4,	// Download in corso
		dsCanceled		= 5,	// Download annullato
	};

// Construction
public:
	DownloadEvent(wxEventType commandType = wxEVT_NULL, int id = 0);
	DownloadEvent(const DownloadEvent &second);
	virtual ~DownloadEvent();

private:
	void _construct(Status status = dsNone, const String &url = String::EMPTY, const String &filename = String::EMPTY, uint32 filesize = 0, uint32 downloadedBytes = 0);

// Attributes
public:
	inline Status getStatus() const;
	inline void setStatus(Status status);

	inline const String & getUrl() const;
	inline void setUrl(const String &url);

	inline const String & getFilename() const;
	inline void setFilename(const String &filename);

	inline uint32 getFilesize() const;
	inline void setFilesize(uint32 filesize);

	inline uint32 getDownloadedBytes() const;
	inline void setDownloadedBytes(uint32 downloadedBytes);

// wxEvent interface
public:
    virtual wxEvent * Clone() const;

protected:
	Status m_status;				// Stato del download
	String m_url;					// Url del file da scaricare
	String m_filename;				// Path locale del file
	uint32 m_filesize;				// Dimensione totale del file da scaricare
	uint32 m_downloadedBytes;		// Numero di bytes scaricati
};

//////////////////////////////////////////////////////////////////////

inline DownloadEvent::Status DownloadEvent::getStatus() const { return m_status; }
inline void DownloadEvent::setStatus(Status status) { m_status = status; }

inline const String & DownloadEvent::getUrl() const { return m_url; }
inline void DownloadEvent::setUrl(const String &url) { m_url = url; }

inline const String & DownloadEvent::getFilename() const { return m_filename; }
inline void DownloadEvent::setFilename(const String &filename) { m_filename = filename; }

inline uint32 DownloadEvent::getFilesize() const { return m_filesize; }
inline void DownloadEvent::setFilesize(uint32 filesize) { m_filesize = filesize; }

inline uint32 DownloadEvent::getDownloadedBytes() const { return m_downloadedBytes; }
inline void DownloadEvent::setDownloadedBytes(uint32 downloadedBytes) { m_downloadedBytes = downloadedBytes; }

//////////////////////////////////////////////////////////////////////

typedef void (wxEvtHandler::*DownloadEventFunction)(DownloadEvent&);

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_DOWNLOADEVENT_H
