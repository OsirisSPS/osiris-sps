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

#ifndef _OS_ENGINE_PORTALSSERIALIZER_H
#define _OS_ENGINE_PORTALSSERIALIZER_H

#include "base/object.h"
#include "boost/signals.hpp"
#include "boost/thread/mutex.hpp"
#include "dbdb.h"
#include "locked.h"

#include "pimpl_ptr.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

//#define OS_PORTALFILE_MIME_TYPE				"application/osiris"
#define OS_PORTALFILE_EXTENSION				"osiris"
#define OS_PORTALFILE_DEFAULT_NAME			"portal.osiris"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class IStream;
class Portal;
class OsirisLink;

//////////////////////////////////////////////////////////////////////

class EngineExport PortalsSerializer : public Object
{
public:
	class EngineExport IResult : public Object
	{
	// Construction
	public:
		IResult();
		virtual ~IResult();

	// Interface
	public:
		// Restituisce il link al portale di riferimento
		virtual shared_ptr<OsirisLink> getPortalLink() const = 0;
		// Restituisce il numero totale di oggetti analizzati
		virtual uint32 getTotalObjects() const = 0;
		// Restituisce il numero di oggetti importati
		virtual uint32 getImportedObjects() const = 0;
		// Restituisce il numero di oggetti saltati (già disponibili o per mancato autore)
		virtual uint32 getSkippedObjects() const = 0;
		// Restituisce il numero di oggetti corrotti
		virtual uint32 getCorruptedObjects() const = 0;
	};

	typedef boost::signal<void (double)> ProgressEvent;

// Construction
public:
	PortalsSerializer();
	virtual ~PortalsSerializer();

// Attributes
public:
	Locked<ProgressEvent>::unique getProgressEvent();
	// Restituisce un valore tra 0 e 1 della percentuale completa di esportazione/importazione
	double getProgressPercentage() const;

	bool getStopped() const;
	void setStopped(bool stopped);

// Operations
public:
	bool exportFile(shared_ptr<Portal> portal, const String &filename, const String &format = _S("osiris"));
	bool exportStream(shared_ptr<Portal> portal, shared_ptr<IStream> stream, const String &format = _S("osiris"));

	shared_ptr<IResult> importFile(shared_ptr<Portal> portal, const String &filename, const String &format = _S("osiris"));
	shared_ptr<IResult> importStream(shared_ptr<Portal> portal, shared_ptr<IStream> stream, const String &format = _S("osiris"));

	shared_ptr<IResult> parsePortalLink(const String &filename);
	shared_ptr<IResult> parsePortalLink(shared_ptr<IStream> stream);

private:
	bool exportOsirisFormat(shared_ptr<IPortalDatabase> db, shared_ptr<Portal> portal, shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback);
	bool exportGourceFormat(shared_ptr<IPortalDatabase> db, shared_ptr<Portal> portal, shared_ptr<IStream> stream, const boost::function<void (double)> &progressCallback);

// Overridables
protected:
	virtual void onProgress(double percentage);

private:
	pimpl_of<PortalsSerializer>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_PORTALSSERIALIZER_H
