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

#ifndef _OS_NET_BANDWIDTHMANAGER_H
#define _OS_NET_BANDWIDTHMANAGER_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

#define OS_NET_UNLIMITED_BANDWIDTH			0
#define OS_NET_BANDWIDTH_CLOCK				1000

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Bandwidth;
class ILimitedConnection;

//////////////////////////////////////////////////////////////////////

class NetExport BandwidthManager : public StaticSingleton<BandwidthManager>
{
	typedef unordered_set<ILimitedConnection *>::type Sockets;

// Internal
private:
	class NetExport Interval : public Object
	{
	// Construction
	public:
		Interval();
		virtual ~Interval();

	// Attributes
	public:
		bool isElapsed() const;

		uint32 getDownloadedBytes() const;
		uint32 getUploadedBytes() const;

	// Operations
	public:
		void handleDownload(uint32 bytes);
		void handleUpload(uint32 bytes);

	private:
		mutable boost::recursive_mutex m_cs;
		uint32 m_downloaded;		// Bytes scaricati nell'intervallo corrente
		uint32 m_uploaded;			// Bytes inviati nell'intervallo corrente
		double m_clock;				// Intervallo corrente
	};

	typedef shared_ptr<Interval> interval_ptr;

// Construction
protected:
	BandwidthManager();

public:
	~BandwidthManager();

// Attributes
public:
	uint32 getMaxDownload() const;
	void setMaxDownload(uint32 bytes);

	uint32 getMaxUpload() const;
	void setMaxUpload(uint32 bytes);

	interval_ptr getInterval() const;

// Operations
public:
	void addSocket(ILimitedConnection *socket);
	void removeSocket(ILimitedConnection *socket);

public:
	uint32 limitDownload(ILimitedConnection *socket, uint32 requiredDownload) const;
	uint32 limitUpload(ILimitedConnection *socket, uint32 requiredUpload) const;

	void handleDownload(ILimitedConnection *socket, uint32 bytes);
	void handleUpload(ILimitedConnection *socket, uint32 bytes);

private:
	uint32 _adjustBandwidth(uint32 maxBytes, uint32 usedBytes, uint32 requiredBytes, shared_ptr<Bandwidth> bandwidth) const;

private:
	mutable boost::recursive_mutex m_cs;
	Sockets m_sockets;
	uint32 m_maxUpload;						// Limite di upload
	uint32 m_maxDownload;					// Limite di download
	uint32 m_totalDownloaded;				// Numero totale di bytes scaricati
	uint32 m_totalUploaded;					// Numero totale di bytes inviati
	uint32 m_downloadSpeed;					// Bytes scaricati al secondo
	uint32 m_uploadSpeed;					// Bytes inviati al secondo
	mutable interval_ptr m_interval;		// Intervallo corrente
	mutable interval_ptr m_prevInterval;	// Intervallo corrente
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_BANDWIDTHMANAGER_H
