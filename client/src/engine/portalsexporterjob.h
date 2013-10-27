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

#ifndef _OS_ENGINE_PORTALEXPORTERJOB_H
#define _OS_ENGINE_PORTALEXPORTERJOB_H

#include "iportalbackgroundjob.h"

#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpDirectoryCallback;
class HttpPath;
class HttpSession;
class Portal;
class PortalsSerializer;
class TemporaryFile;

//////////////////////////////////////////////////////////////////////

class EngineExport PortalsExporterJob : public IPortalBackgroundJob
{
	typedef IPortalBackgroundJob JobBase;

// Construction
public:
	PortalsExporterJob(uint32 id, shared_ptr<Portal> portal, const String &format = _S("osiris"));
	virtual ~PortalsExporterJob();

// Attributes
public:

// Operations
private:
	bool handleDownloadRequest(shared_ptr<HttpSession> session, const HttpPath &path);

	std::string getExtension();

// IJob overrides
public:
	virtual void stop();

// IJob interface
public:
	virtual JobStatus run();

// IBackgroundJob interface
public:
	virtual shared_ptr<Details> getDetails() const;

protected:
	String m_format;
	shared_ptr<PortalsSerializer> m_serializer;
	shared_ptr<TemporaryFile> m_file;
	shared_ptr<HttpDirectoryCallback> m_downloadDirectory;
	String m_filename;	
};

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_PORTALEXPORTERJOB_H
