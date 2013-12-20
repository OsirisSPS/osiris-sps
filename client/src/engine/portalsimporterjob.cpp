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
#include "portalsimporterjob.h"

#include "boost/bind.hpp"
#include "buffer.h"
#include "conversions.h"
#include "engine.h"
#include "httpclient.h"
#include "httpcontent.h"
#include "httpresponse.h"
#include "httpurl.h"
#include "istream.h"
#include "log.h"
#include "memfile.h"
#include "portalsserializer.h"
#include "portalsoptimizerjob.h"
#include "portalsportal.h"
#include "notificationsmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<PortalsImporterJob>
{
// Construction
public:
	pimpl(shared_ptr<Portal> portal, shared_ptr<IStream> stream);
	pimpl(shared_ptr<Portal> portal, const String &url);
	virtual ~pimpl();

// Attributes
public:
	inline shared_ptr<Portal> getPortal() const;
	inline shared_ptr<PortalsSerializer> getSerializer() const;
	inline shared_ptr<PortalsSerializer::IResult> getResult() const;
	inline String getStatus() const;

// Operations
public:
	void run();
	void stop();

private:
	weak_ptr<Portal> m_portal;
	shared_ptr<PortalsSerializer> m_serializer;	
	shared_ptr<PortalsSerializer::IResult> m_result;
	shared_ptr<IStream> m_stream;
	String m_url;
	String m_status;
};

//////////////////////////////////////////////////////////////////////

pimpl<PortalsImporterJob>::pimpl(shared_ptr<Portal> portal, shared_ptr<IStream> stream) : m_portal(portal),
																						 m_stream(stream),																						 
																						 m_serializer(OS_NEW PortalsSerializer())
{
	OS_ASSERT(portal != nullptr);
	OS_ASSERT(stream != nullptr);	
}

pimpl<PortalsImporterJob>::pimpl(shared_ptr<Portal> portal, const String &url) : m_portal(portal),
																				 m_stream(nullptr),																						 
																				 m_url(url),
																				 m_serializer(OS_NEW PortalsSerializer())
{
	OS_ASSERT(portal != nullptr);	
}

pimpl<PortalsImporterJob>::~pimpl()
{

}

inline shared_ptr<Portal> pimpl<PortalsImporterJob>::getPortal() const
{
	return m_portal.lock();
}

inline shared_ptr<PortalsSerializer> pimpl<PortalsImporterJob>::getSerializer() const
{
	return m_serializer;
}

inline shared_ptr<PortalsSerializer::IResult> pimpl<PortalsImporterJob>::getResult() const
{
	return m_result;
}

inline String pimpl<PortalsImporterJob>::getStatus() const
{
	return m_status;
}

void pimpl<PortalsImporterJob>::run()
{
	m_result.reset();

	m_status = String::EMPTY;

	if(m_stream != nullptr)
	{
		shared_ptr<Portal> portal = m_portal.lock();
		if(portal == nullptr || m_stream == nullptr)
			return;

		m_status = "importing";

		m_serializer->setStopped(false);
		m_result = m_serializer->importStream(portal, m_stream);
		m_stream->close();
		m_stream.reset();
	}
	else
	{
		m_status = "download";

		shared_ptr<boost::asio::io_service> service = createAsioService();
		shared_ptr<TCPSocket> socket = Engine::instance()->createTCPSocket(service, true, true);
		shared_ptr<HttpClient> httpClient(OS_NEW HttpClient(service, socket));

		HttpUrl url(m_url.to_ascii());
		bool result = httpClient->perform(url);

		if(result)
		{
			shared_ptr<HttpResponse> response = httpClient->getResponse();

			//shared_ptr<HttpContent> responseContent = response->getContent();

			if(response->getStatusCode() == 200)
			{
				m_status = "importing";

				shared_ptr<MemFile> content(OS_NEW MemFile(response->getContent()->getContentPtr()));
				
				shared_ptr<Portal> portal = m_portal.lock();
				if(portal == nullptr)
					return;

				m_serializer->setStopped(false);
				m_result = m_serializer->importStream(portal, content);				
			}

			m_status = "completed";
		}
		else
		{
			m_status = "failed";
		}
	}

	//Engine::instance()->addBackgroundJob(shared_ptr<PortalsOptimizerJob>(OS_NEW PortalsOptimizerJob(Engine::instance()->peekBackgroundJobID(), portal)));		
}

void pimpl<PortalsImporterJob>::stop()
{
	m_serializer->setStopped(true);
}

//////////////////////////////////////////////////////////////////////

PortalsImporterJob::PortalsImporterJob(uint32 id, shared_ptr<Portal> portal, shared_ptr<IStream> stream) : JobBase(true, id, portal),
																									     m_impl(portal, stream)
{
	m_impl->getSerializer()->getProgressEvent()->connect(boost::bind(&PortalsImporterJob::onProgress, this));	
}

PortalsImporterJob::PortalsImporterJob(uint32 id, shared_ptr<Portal> portal, const String &url) : JobBase(true, id, portal),
																									     m_impl(portal, url)
{
	m_impl->getSerializer()->getProgressEvent()->connect(boost::bind(&PortalsImporterJob::onProgress, this));	
}

PortalsImporterJob::~PortalsImporterJob()
{

}

void PortalsImporterJob::stop()
{
	m_impl->stop();

	JobBase::stop();	
}

IJob::JobStatus PortalsImporterJob::run()
{
	shared_ptr<Portal> portal = m_impl->getPortal();
	if(portal == nullptr)
		return jobComplete;

	NotificationsManager::instance()->notify(_S("Import on '") + portal->getName() + _S("' started."));

	m_impl->run();

	NotificationsManager::instance()->notify(_S("Import on '") + portal->getName() + _S("' completed."));

	return jobComplete;
}

shared_ptr<IBackgroundJob::Details> PortalsImporterJob::getDetails() const
{
	String jobName(Engine::instance()->getText("job.importer"));

	shared_ptr<Portal> portal = m_impl->getPortal();
	if(portal != nullptr)
	{
		jobName.append(" - ");
		jobName.append(portal->getName());
	}

	shared_ptr<Details> details(OS_NEW Details(jobName.to_wide()));

	String status = Engine::instance()->getText(_S("job.importer.status.") + m_impl->getStatus());
	details->setStatus(status.to_wide());

	shared_ptr<PortalsSerializer> serializer = m_impl->getSerializer();	
	if(serializer != nullptr)
		details->setPercentage(serializer->getProgressPercentage());

	shared_ptr<PortalsSerializer::IResult> result = m_impl->getResult();
	if(portal != nullptr && result != nullptr)
	{
		// TOTRANSLATE
		String msg = _S("[center][url=\"{@url}\"]Enter in portal[/url][/center][br/][br/][sub]Objects: [b]{@total}[/b], Imported: [b]{@imported}[/b], Skipped: [b]{@skipped}[/b], Invalid: [b]{@invalid}[/b][/sub]");

		msg.replace_all(_S("{@url}"), portal->getLink("view"));
		msg.replace_all(_S("{@total}"), conversions::to_utf16(result->getTotalObjects()));
		msg.replace_all(_S("{@imported}"), conversions::to_utf16(result->getImportedObjects()));
		msg.replace_all(_S("{@skipped}"), conversions::to_utf16(result->getSkippedObjects()));
		msg.replace_all(_S("{@invalid}"), conversions::to_utf16(result->getCorruptedObjects()));

		details->setResult(msg);
	}

	return details;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
