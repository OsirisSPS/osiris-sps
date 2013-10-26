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
#include "mainaddonsjob.h"

#include "boost/bind.hpp"
#include "buffer.h"
#include "engine.h"
#include "extensionsextension.h"
#include "extensionssystem.h"
#include "filesystem.h"
#include "httpclient.h"
#include "httpcontent.h"
#include "httpresponse.h"
#include "httpurl.h"
#include "idesystem.h"
#include "ihttpserver.h"
#include "istream.h"
#include "log.h"
#include "options.h"
#include "notificationsmanager.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

/*
template <>
class pimpl<MainAddonsJob>
{
// Construction
public:
	pimpl();
	virtual ~pimpl();

// Attributes
public:


// Operations
public:
	void run();
	void stop();
};

//////////////////////////////////////////////////////////////////////

pimpl<MainAddonsJob>::pimpl()
{
}

pimpl<MainAddonsJob>::~pimpl()
{

}

void pimpl<MainAddonsJob>::run()
{
}

void pimpl<MainAddonsJob>::stop()
{
}
*/

//////////////////////////////////////////////////////////////////////

class HttpClientJob : public HttpClient
{
// Construction
public:
	HttpClientJob(shared_ptr<boost::asio::io_service> service, shared_ptr<TCPSocket> socket, shared_ptr<IJob> job);
	virtual ~HttpClientJob();

	virtual void onResponseProgress(uint32 value, uint32 total);

protected:
	shared_ptr<MainAddonsJob> m_job;
};

HttpClientJob::HttpClientJob(shared_ptr<boost::asio::io_service> service, shared_ptr<TCPSocket> socket, shared_ptr<IJob> job) : HttpClient(service, socket)
{
	m_job = boost::dynamic_pointer_cast<MainAddonsJob>(job);
}

HttpClientJob::~HttpClientJob()
{
}

void HttpClientJob::onResponseProgress(uint32 value, uint32 total)
{
	if(total != 0)
		m_job->setProgressPercentage(double(value) / double(total));
	else
		m_job->setProgressPercentage(0);
}

//////////////////////////////////////////////////////////////////////

MainAddonsJob::MainAddonsJob(uint32 id, const String& action, const std::string &extensionID, const String &downloadUrl) : JobBase(true, id),
																														 m_action(action),
																														 m_extensionID(extensionID),
																														 m_url(downloadUrl)																											 
{
	//??m_impl->getSerializer()->getProgressEvent()->connect(boost::bind(&PortalImporterJob::onProgress, this));
	m_progressPercentage = 0;
}

MainAddonsJob::~MainAddonsJob()
{

}

void MainAddonsJob::stop()
{
	//m_impl->stop();

	JobBase::stop();
}

IJob::JobStatus MainAddonsJob::run()
{
	shared_ptr<ExtensionsExtension> existsExtension = boost::dynamic_pointer_cast<ExtensionsExtension>(ExtensionsSystem::instance()->getExtension(getExtensionID()));
	if(existsExtension)
		m_name = existsExtension->getName();
	else
		m_name = getExtensionID().getString();

	if( (getAction() == _S("install")) || (getAction() == _S("upgrade")) )
	{
		std::string tempPath = utils::makeFilePath(Options::instance()->getTempPath(), getExtensionID().getString() + _S(".osiris")).to_ascii();
		
		if(FileSystem::instance()->exists(tempPath))
			FileSystem::instance()->remove(tempPath);

		//NotificationsManager::instance()->notify(_S("Download extension: ") + getUrl().to_wide());

		m_progressPercentage = 0;

		shared_ptr<boost::asio::io_service> service = createAsioService();
		shared_ptr<TCPSocket> socket = Engine::instance()->createTCPSocket(service, true, true);
		shared_ptr<HttpClientJob> httpClient(OS_NEW HttpClientJob(service, socket, get_this_ptr()));

		HttpUrl url(getUrl().to_ascii());
		bool result = httpClient->perform(url);

		m_progressPercentage = 0;

		if(result)
		{
			shared_ptr<HttpResponse> response = httpClient->getResponse();

			shared_ptr<HttpContent> responseContent = response->getContent();

			if(response->getStatusCode() == 200)
			{
				const Buffer &bufferResponse = responseContent->getContent();
				bufferResponse.save(tempPath);

				//NotificationsManager::instance()->notify(_S("Download completed."));

				boost::unique_lock<boost::shared_mutex> lock(IdeSystem::instance()->getServer()->getRequestMutex());

				ExtensionsSystem::instance()->install(getExtensionID(), tempPath, true);

				if(existsExtension)
					m_name = existsExtension->getName();				
			}
			else
				result = false;
		}

		/*
		if(result == false)
			NotificationsManager::instance()->notify(_S("Download failed."));
		*/

		if(FileSystem::instance()->exists(tempPath))
			FileSystem::instance()->remove(tempPath);		
	}

	if(getAction() == _S("uninstall"))
	{
		boost::unique_lock<boost::shared_mutex> lock(IdeSystem::instance()->getServer()->getRequestMutex());
		ExtensionsSystem::instance()->uninstall(getExtensionID());
	}

	if(getAction() == _S("activate"))
	{
		boost::unique_lock<boost::shared_mutex> lock(IdeSystem::instance()->getServer()->getRequestMutex());
		shared_ptr<ExtensionsExtension> extension = boost::dynamic_pointer_cast<ExtensionsExtension>(ExtensionsSystem::instance()->getExtension(getExtensionID()));
		if(extension)
			extension->activate();
	}

	if(getAction() == _S("deactivate"))
	{
		boost::unique_lock<boost::shared_mutex> lock(IdeSystem::instance()->getServer()->getRequestMutex());
		shared_ptr<ExtensionsExtension> extension = boost::dynamic_pointer_cast<ExtensionsExtension>(ExtensionsSystem::instance()->getExtension(getExtensionID()));
		if(extension)
			extension->deactivate();
	}

	/*
	shared_ptr<Portal> portal = m_impl->getPortal();
	if(portal == null)
		return jobComplete;

	NotificationsManager::instance()->notify(_S("Import on '") + portal->getName() + _S("' started."));

	m_impl->run();

	NotificationsManager::instance()->notify(_S("Import on '") + portal->getName() + _S("' completed."));
	*/

	return jobComplete;
}

shared_ptr<IBackgroundJob::Details> MainAddonsJob::getDetails() const
{
	String jobName(Engine::instance()->getText("job.addons"));

	String status = Engine::instance()->getText(_S("job.addons.actions.") + m_action);
	if(m_progressPercentage != 0)
		status = Engine::instance()->getText(_S("job.addons.actions.download"));
	
	jobName += _S(" - ") + status + _S(" - ") + m_name;

	shared_ptr<Details> details(OS_NEW Details(jobName.to_wide()));
	details->setStatus(status.to_wide());

	if(m_progressPercentage != 0)
		details->setPercentage(m_progressPercentage);

	/*
	shared_ptr<PortalsSerializer> serializer = m_impl->getSerializer();
	if(serializer != null)
		details->setPercentage(serializer->getProgressPercentage());

	shared_ptr<PortalsSerializer::IResult> result = m_impl->getResult();
	if(portal != null && result != null)
		details->setUrl(portal->getLink(ide::portal::ppView));
	*/

	return details;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
