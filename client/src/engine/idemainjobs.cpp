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
#include "idemainjobs.h"

#include "boost/bind.hpp"
#include "engine.h"
#include "eventssource.h"
#include "htmlcheckbox.h"
#include "htmlevent.h"
#include "htmlcontrols.h"
#include "htmlxslcontrol.h"
#include "httprequest.h"
#include "ibackgroundjob.h"
#include "ideskin.h"
#include "xmldocument.h"
#include "xmlexporter.h"

//////////////////////////////////////////////////////////////////////

#define OS_JOBSPAGE_EVENT_ONCLEARCOMPLETEDJOBS		"onClearCompletedJobs"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((ide)(main))

//////////////////////////////////////////////////////////////////////

Jobs::Jobs(shared_ptr<HttpSession> session) : PageBase(session),
											  m_showAll(OS_NEW HtmlCheckBox())
{
	getEvents()->get(OS_JOBSPAGE_EVENT_ONCLEARCOMPLETEDJOBS)->connect(boost::bind(&Jobs::onClearCompletedJobs, this));
}

Jobs::~Jobs()
{

}

void Jobs::exportJobs(shared_ptr<XMLExporter> exporter, const Engine::BackgroundJobs &jobs, bool completed)
{
	OS_ASSERT(exporter != null);

	bool showAll = m_showAll->getCheck();

	for(Engine::BackgroundJobs::const_iterator i = jobs.begin(); i != jobs.end(); ++i)
	{
		shared_ptr<IBackgroundJob> job = *i;
		if(job == null)
		{
			OS_ASSERTFALSE();
			continue;
		}

		if(showAll || job->getVisible())
			exportJob(exporter->createChild<XMLExporter>("job"), job, completed);
	}
}

bool Jobs::exportJob(shared_ptr<XMLExporter> exporter, shared_ptr<IBackgroundJob> job, bool completed)
{
	OS_ASSERT(exporter != null);
	OS_ASSERT(job != null);

	shared_ptr<IBackgroundJob::Details> jobDetails = job->getDetails();
	if(jobDetails == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	exporter->setAttributeUint32("id", job->getID());
	exporter->setAttributeString("name", jobDetails->getName());
	exporter->setAttributeString("status", jobDetails->getStatus());

	exporter->setAttributeDateTime("creationTime", DateTime(job->getCreationTime()));

	exporter->setAttributeDateTime("lastProgressTime", DateTime(job->getLastProgressTime()));
	
	exporter->setAttributeBool("running", job->running());
	exporter->setAttributeBool("completed", completed);
	boost::optional<double> jobPercentage;
	if(job->getStatus() == IJob::jobComplete)
		jobPercentage = OS_IBACKGROUNDJOB_PERCENTAGE_MAX;
	else if(job->running() == false)
		jobPercentage = -1;
	else
		jobPercentage = jobDetails->getPercentage();
	if(jobPercentage != boost::none)
		exporter->setAttributeDouble("percentage", jobPercentage.get());		
	String result = jobDetails->getResult();
	if(result.empty() == false)
		exporter->setAttributeString("result", result);		
	
	return true;
}

void Jobs::onClearCompletedJobs()
{
	Engine::instance()->clearCompletedBackgroundJobs();
	reload();
}

String Jobs::getPageName() const
{
    return "main.pages.jobs";
}

void Jobs::onInit()
{
	PageBase::onInit();

	m_showAll->setID("showAll");
	m_showAll->setAutoPostBack(true);
	m_showAll->setCaption(getText("main.pages.jobs.show_all"));

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	m_pageTemplate.reset(OS_NEW HtmlXSLControl(loadStylesheet(getSkin()->getTemplatePath(_S("jobs.xsl"))), document));
	getArea(pageAreaContent)->getControls()->add(m_pageTemplate);

	m_pageTemplate->addChildParam(m_showAll);
}

void Jobs::onPreRender()
{
	PageBase::onPreRender();
	
	shared_ptr<XMLExporter> exporter(OS_NEW XMLExporter(m_pageTemplate->getDocument()->create(_S("jobs"))));
	exporter->createAction(_S("clearCompletedJobs"), getEventCommand(OS_JOBSPAGE_EVENT_ONCLEARCOMPLETEDJOBS));

	// Highlight job
	if(getRequest()->hasUrlParam(_W("job")))
		exporter->setAttributeString(_S("highlight"), getRequest()->getUrlParam(_W("job")));

	shared_ptr<Engine::BackgroundJobs> jobs = Engine::instance()->getBackgroundJobs();
	if(jobs != null)
		exportJobs(exporter, *jobs, false);

	jobs = Engine::instance()->getLastCompletedBackgroundJobs();
	if(jobs != null)
		exportJobs(exporter, *jobs, true);	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((ide)(main))

//////////////////////////////////////////////////////////////////////
