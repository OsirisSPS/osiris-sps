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
#include "idemainexport.h"

#include "conversions.h"
#include "engine.h"
#include "htmlfilebrowser.h"
#include "httprequest.h"
#include "idemainjobs.h"

#include "portalsexporterjob.h"
#include "portalsportalssystem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((ide)(main))

//////////////////////////////////////////////////////////////////////

Export::Export(shared_ptr<HttpSession> session) : PageBase(session)/*,
												  m_fileBrowser(OS_NEW HtmlFileBrowser())*/
{
	//m_fileBrowser->setID(_S("fileBrowser"));
	//m_fileBrowser->setCss(_S("os_input_full"));
}

Export::~Export()
{

}

String Export::getPageName() const
{
    return "main.pages.export";
}

void Export::onLoad()
{
	PageBase::onLoad();

	shared_ptr<Portal> portal = getPortalFromUrl();
	if(portal == null)
	{
		showError(getText(_S("ide.messages.invalid_portal")));
		return;
	}

	String format = getRequest()->getUrlParam(_W("format"));
	
	if(format == String::EMPTY) // TEMP
		format = _S("osiris");
	
	// RAZORVERYURGENT: serve?
	/*
	ObjectID user(getRequest()->getUrlParam(OS_URL_PARAM_USER));
	if(user.validateObject(true) == false)
		user.clear();
	*/

	if(format == String::EMPTY)
	{
		// Elenco formati possibili, os_actions_links
	}
	else
	{
		/*
		ObjectID user(utils::wtoa(getRequest()->getUrlParam(OS_URL_PARAM_USER)));
		if(user.validateObject(true) == false)
			user.clear();
			*/

		uint32 jobID = Engine::instance()->peekBackgroundJobID();

		shared_ptr<PortalsExporterJob> job(OS_NEW PortalsExporterJob(jobID, portal, format));

		Engine::instance()->addBackgroundJob(job);

		ordered_map<std::wstring, std::wstring> params;
		params.set(OS_PAGEJOBS_PARAM_JOB, conversions::to_wstring(jobID));
		redirect(PortalsSystem::instance()->getRedirectLink(PortalsSystem::instance()->getMainLink("jobs", params)));
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((ide)(main))

//////////////////////////////////////////////////////////////////////
