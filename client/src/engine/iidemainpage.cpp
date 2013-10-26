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
#include "iidemainpage.h"

#include "engine.h"
#include "idepathway.h"
#include "portalsportalssystem.h"
#include "portalsportal.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IMainPage::IMainPage(shared_ptr<HttpSession> session) : PageBase(session)
{

}

IMainPage::~IMainPage()
{
	
}

void IMainPage::onLoad()
{
	PageBase::onLoad();

	if(getPageName().empty() == false)
	{
		setPageTitle(getText(getPageName() + ".title"));
		setPageDescription(getText(getPageName() + ".description"));
	}
}	

/*
void IMainPage::redirect(ide::main::MainPage page)
{
	PageBase::redirect(PortalsSystem::instance()->getMainLink(page));	
}
*/

void IMainPage::redirect(const std::string &url)	// Necessario per non "oscurare" il metodo redirect della base
{
	PageBase::redirect(url);	
}

bool IMainPage::isMcpModeRequired() const
{
	return true;
}

// Pathway
void IMainPage::onPathway() const
{
	PageBase::onPathway();

	if(getPageName().empty() == false)
		getPathway()->add(getText(getPageName() + _S(".title")), getPagePathway());
	//getPathway()->add(getTitle(), getPagePathway(), ObjectID::getRootID());
}

String IMainPage::getBaseTitle()
{
	return OS_OSIRIS_APPLICATION_NAME;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
