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
#include "portalsportalsdirectory.h"

#include "idepages.h"
#include "ideportalaccount.h"
#include "ideportalassistant.h"
#include "ideportaledit.h"
#include "ideportalobjectdetail.h"
#include "ideportalsearch.h"
#include "ideportaluser.h"
#include "ideportalusers.h"
#include "ideportalview.h"
#include "ideportalmessenger.h"
#include "httppath.h"
#include "httpsession.h"
#include "portalsportalssystem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace portals {

//////////////////////////////////////////////////////////////////////

PortalsDirectory::PortalsDirectory(const String &name) : DirectoryBase(name)
{

}

PortalsDirectory::~PortalsDirectory()
{

}

bool PortalsDirectory::handle(shared_ptr<HttpSession> session, const HttpPath &path)
{
	// Controlla se non  stata specificata alcuna pagina
	if(path.getTokens().size() == 0)
	{
		// Redirige il client alla pagina predefinita
		session->redirect(PortalsSystem::instance()->getMainLink(OS_IDE_PAGE_OSIRIS));
		return true;
	}
	else
	{
		ide::portal::PortalPage page_type = PortalsSystem::instance()->getPortalPage(path.getDirectory());
		// Controlla se la pagina non  una pagina standard
		if(page_type != ide::portal::ppUnknown)
		{
			shared_ptr<IPage> page;
			switch(page_type)
			{
			case ide::portal::ppAccount:	    page.reset(OS_NEW ide::portal::Account(session));
                                                break;

			case ide::portal::ppEdit:		    page.reset(OS_NEW ide::portal::Edit(session));
                                                break;

			case ide::portal::ppUser:		    page.reset(OS_NEW ide::portal::User(session));
                                                break;

			case ide::portal::ppUsers:		    page.reset(OS_NEW ide::portal::Users(session));
                                                break;

			case ide::portal::ppView:		    page.reset(OS_NEW ide::portal::View(session));
                                                break;
			
			case ide::portal::ppObjectDetail:	page.reset(OS_NEW ide::portal::ObjectDetail(session));
                                                break;

			case ide::portal::ppAssistant:		page.reset(OS_NEW ide::portal::Assistant(session));
                                                break;

			case ide::portal::ppSearch:			page.reset(OS_NEW ide::portal::Search(session));
                                                break;

			case ide::portal::ppMessenger:		page.reset(OS_NEW ide::portal::Messenger(session));
                                                break;

			default:						    OS_ASSERTFALSE();
                                                break;
			}

			return page != null ? page->transmit() : false;
		}
	}

	if(DirectoryBase::handle(session, path))
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////

} // portals
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
