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
#include "idemainjobdetail.h"

#include "idemainjobs.h"

#include "conversions.h"
#include "convert.h"
#include "css.h"
#include "engine.h"
#include "htmlevent.h"
#include "htmlxslcontrol.h"
#include "httprequest.h"
#include "ibackgroundjob.h"
#include "ideskin.h"
#include "logmanager.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

const std::wstring JobDetail::ID = _W("id");

//////////////////////////////////////////////////////////////////////

JobDetail::JobDetail(shared_ptr<HttpSession> session) : PageBase(session)
{

}

JobDetail::~JobDetail()
{

}

String JobDetail::getUrlID() const
{
	return getRequest()->getUrlParam(ID);
}

String JobDetail::getPageName() const
{
	return _S("main.pages.jobDetail");
}

void JobDetail::onLoad()
{
	PageBase::onLoad();

	// Ottiene l'id dell'utente richiesto
	int id = conversions::from_utf16<uint32>(getUrlID());

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());	
	
	shared_ptr<XMLExporter> exporter(OS_NEW XMLExporter(document->create(_S("job"))));

	// VERYURGENT: abolire il ciclo for	

	Engine::BackgroundJobs::const_iterator i;

	shared_ptr<Engine::BackgroundJobs> jobs = Engine::instance()->getBackgroundJobs();	

	if(jobs != nullptr)
	{
		for(i = jobs->begin(); i != jobs->end(); ++i)
		{
			shared_ptr<IBackgroundJob> job = *i;
			if(job->getID() == id)
			{
				Jobs::exportJob(exporter, job, false);				
			}
		}
	}

	jobs = Engine::instance()->getLastCompletedBackgroundJobs();
	if(jobs != nullptr)
	{
		for(i = jobs->begin(); i != jobs->end(); ++i)
		{
			shared_ptr<IBackgroundJob> job = *i;
			if(job->getID() == id)
			{
				Jobs::exportJob(exporter, job, true);				
			}
		}
	}

	String path = getSkin()->getTemplatePath(_S("job_detail.xsl"));
	shared_ptr<HtmlXSLControl> pageTemplate(OS_NEW HtmlXSLControl(loadStylesheet(path), document));	
	
	// Carica il template di riferimento
	getControls()->add(pageTemplate);
	
}

void JobDetail::onPreRender()
{
	PageBase::onPreRender();

}

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
