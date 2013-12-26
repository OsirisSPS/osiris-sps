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
#include "portalsexporterjob.h"

#include "boost/bind.hpp"
#include "engine.h"
#include "filesystem.h"
#include "httpdirectorycallback.h"
#include "httpenumerateddirectorycallback.h"
#include "httppath.h"
#include "httpresponse.h"
#include "httpsession.h"
#include "log.h"
#include "mimemanager.h"
#include "notificationsmanager.h"
#include "options.h"
#include "portalsserializer.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "temporaryfile.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PortalsExporterJob::PortalsExporterJob(uint32 id, shared_ptr<Portal> portal, const String &format) : JobBase(true, id, portal),
																											m_format(format),
																											m_serializer(OS_NEW PortalsSerializer())
{
	OS_ASSERT(portal != null);

	m_filename = FileSystem::instance()->createValidFilename(portal->getName());
	if(m_filename.empty())
		m_filename = OS_PORTALFILE_DEFAULT_NAME;
	else
		m_filename = FileSystem::instance()->makeFilename(m_filename, getExtension());

	m_downloadDirectory = PortalsSystem::instance()->getTempDirectory()->addDirectoryCallback(boost::bind(&PortalsExporterJob::handleDownloadRequest, this, _1, _2));
	OS_ASSERT(m_downloadDirectory != null);

	m_serializer->getProgressEvent()->connect(boost::bind(&PortalsExporterJob::onProgress, this));
}

PortalsExporterJob::~PortalsExporterJob()
{
	PortalsSystem::instance()->getTempDirectory()->removeDirectory(m_downloadDirectory);
	m_downloadDirectory.reset();
}

std::string PortalsExporterJob::getExtension()
{
	if(m_format == _S("osiris"))
	{
		return OS_PORTALFILE_EXTENSION;
	}
	else if(m_format == _S("gource"))
	{
		return "txt";
	}
	else
	{
		return "";
	}
}

bool PortalsExporterJob::handleDownloadRequest(shared_ptr<HttpSession> session, const HttpPath &path)
{
	if(m_file == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	OS_ASSERT(m_file->is_open() == false);
	OS_ASSERT(m_filename.empty() == false);

	session->getResponse()->setContentDisposition(m_filename.to_utf8());
	//session->transmitFile(m_file->getFilename(), OS_PORTALFILE_MIME_TYPE);

	String mime_type;
	MimeManager::instance()->getMimeType(getExtension(), mime_type);

	session->transmitFile(m_file->getFilename(), mime_type.to_ascii());

	return true;	
}

void PortalsExporterJob::stop()
{
	m_serializer->setStopped(true);

	JobBase::stop();	
}

IJob::JobStatus PortalsExporterJob::run()
{
	shared_ptr<Portal> portal = getPortal();
	if(portal == null)
		return jobComplete;

	NotificationsManager::instance()->notify(_S("Exporting of '") + portal->getName() + _S("' started."));

	m_file.reset();

	shared_ptr<TemporaryFile> file = Options::instance()->createTemporaryFile();
	if(file == null)
	{
		OS_LOG_ERROR("Cannot create temporary file for export");
		return jobComplete;
	}

	m_serializer->setStopped(false);
	if(m_serializer->exportStream(portal, file, m_format))
	{
		file->close();
		m_file = file;
	}

	NotificationsManager::instance()->notify(_S("Exporting of '") + portal->getName() + _S("' completed."));

	return jobComplete;
}

shared_ptr<IBackgroundJob::Details> PortalsExporterJob::getDetails() const
{
	String jobName(Engine::instance()->getText("job.exporter"));

	shared_ptr<Portal> portal = getPortal();
	if(portal != null)
	{
		jobName.append(" - ");
		jobName.append(portal->getName());
	}

	shared_ptr<Details> details(OS_NEW Details(jobName.to_wide()));
	details->setPercentage(m_serializer->getProgressPercentage());
	if(m_file != null && m_downloadDirectory != null)
	{
		// TOTRANSLATE
		String msg = _S("[center][a class=\"os_button\" href=\"{@url}\"]Click here for download[/a][/center]");
		msg.replace_all(_S("{@url}"), m_downloadDirectory->getPath().getUrl());
		details->setResult(msg);
		//details->setUrl(m_downloadDirectory->getPath().getUrl().to_ascii());
	}

	return details;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
