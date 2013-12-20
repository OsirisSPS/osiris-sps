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
#include "portalsmaindirectory.h"

#include "portalsportalssystem.h"
#include "idemainaccounts.h"
#include "idemainexport.h"
#include "idemainimport.h"
#include "idemainjobdetail.h"
#include "idemainjobs.h"
#include "idemainmaintenance.h"
#include "idemainoptions.h"
#include "idepages.h"
#include "idemainresources.h"
#include "idemainsettings.h"
#include "idemainsubscribe.h"
#include "idemainisis.h"
#include "httppath.h"
#include "httpsession.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((portals))

//////////////////////////////////////////////////////////////////////

MainDirectory::MainDirectory(const String &name) : DirectoryBase(name)
{

}

MainDirectory::~MainDirectory()
{

}

bool MainDirectory::handle(shared_ptr<HttpSession> session, const HttpPath &path)
{
	if(path.getTokens().size() == 0)
	{
		// Redirige il client alla pagina predefinita
		session->redirect(PortalsSystem::instance()->getMainLink(OS_IDE_PAGE_OSIRIS));
		return true;
	}
	else
	{
		ide::main::MainPage page_type = PortalsSystem::instance()->getMainPage(path.getDirectory());
		// Controlla se la pagina non è una pagina standard
		if(page_type != ide::main::mpUnknown)
		{
			shared_ptr<IPage> page;
			switch(page_type)
			{
			case ide::main::mpAccounts:			page.reset(OS_NEW ide::main::Accounts(session));
												break;

			case ide::main::mpSettings:			page.reset(OS_NEW ide::main::Settings(session));
												break;

			case ide::main::mpResources:		page.reset(OS_NEW ide::main::Resources(session));
												break;

			case ide::main::mpMaintenance:		page.reset(OS_NEW ide::main::Maintenance(session));
												break;

			case ide::main::mpSubscribe:		page.reset(OS_NEW ide::main::Subscribe(session));
												break;

			case ide::main::mpIsis:				page.reset(OS_NEW ide::main::Isis(session));
												break;

			case ide::main::mpJobDetail:		page.reset(OS_NEW ide::main::JobDetail(session));
												break;

            case ide::main::mpJobs:				page.reset(OS_NEW ide::main::Jobs(session));
												break;

            case ide::main::mpImport:			page.reset(OS_NEW ide::main::Import(session));
												break;

			case ide::main::mpExport:			page.reset(OS_NEW ide::main::Export(session));
												break;
			
			case ide::main::mpOptions:			page.reset(OS_NEW ide::main::Options(session));
												break;

			default:							OS_ASSERTFALSE();
												break;
			}

			return page != nullptr ? page->transmit() : false;
		}
	}

	if(DirectoryBase::handle(session, path))
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((portals))

//////////////////////////////////////////////////////////////////////
