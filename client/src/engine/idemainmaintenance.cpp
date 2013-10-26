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
#include "idemainmaintenance.h"

#include "boost/bind.hpp"
#include "iportaldatabase.h"
#include "idbconnection.h"
#include "ideskin.h"
#include "entitiessnapshotmanager.h"
#include "eventssource.h"
#include "htmlliteral.h"
#include "htmldiv.h"
#include "platformmanager.h"
#include "portalsportals.h"
#include "portalsportalssystem.h"
#include "portalstransaction.h"
#include "realtimestats.h"
#include "htmlevent.h"
#include "httprequest.h"
#include "htmlcontrols.h"
#include "htmlxslcontrol.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

const String Maintenance::EVENT_ONSTABILITYRESTART = _S("onStabilityRestart");
const String Maintenance::EVENT_ONSTABILITYREBUILD = _S("onStabilityRebuild");
const String Maintenance::EVENT_ONLUCENEREBUILD = _S("onLuceneRebuild");
const String Maintenance::EVENT_ONLUCENEOPTIMIZE = _S("onLuceneOptimize");
const String Maintenance::EVENT_ONDATABASEVACUUM = _S("onDatabaseVacuum");
const String Maintenance::EVENT_ONDATABASEREBUILDINDEXES = _S("onDatabaseRebuildIndexes");
const String Maintenance::EVENT_ONDATABASEANALYZE = _S("onDatabaseAnalyze");

Maintenance::Maintenance(shared_ptr<HttpSession> session) : PageBase(session)
{
	getEvents()->get(EVENT_ONSTABILITYRESTART)->connect(boost::bind(&Maintenance::onStabilityRestart, this));
	getEvents()->get(EVENT_ONSTABILITYREBUILD)->connect(boost::bind(&Maintenance::onStabilityRebuild, this));
	getEvents()->get(EVENT_ONLUCENEREBUILD)->connect(boost::bind(&Maintenance::onLuceneRebuild, this));
	getEvents()->get(EVENT_ONLUCENEOPTIMIZE)->connect(boost::bind(&Maintenance::onLuceneOptimize, this));
	getEvents()->get(EVENT_ONDATABASEVACUUM)->connect(boost::bind(&Maintenance::onDatabaseVacuum, this));
	getEvents()->get(EVENT_ONDATABASEREBUILDINDEXES)->connect(boost::bind(&Maintenance::onDatabaseRebuildIndexes, this));
	getEvents()->get(EVENT_ONDATABASEANALYZE)->connect(boost::bind(&Maintenance::onDatabaseAnalyze, this));
}

Maintenance::~Maintenance()
{

}

String Maintenance::getPageName() const
{
	return _S("main.pages.maintenance");
}

void Maintenance::onLoad()
{
	PageBase::onLoad();
	
	/*
	shared_ptr<Portal> portal = getPortalFromUrl();
	if(portal == null)
	{
		showError(_S("Invalid portal"));
		return;
	}
	*/

	if(getRequest()->hasUrlParam(_W("crash"))) // TEMP, x test crash report
	{
		int32 *pI=null;
		*pI = 5;
	}

	/*
	for(int i=0;i<100;i++)
	{
		shared_ptr<HtmlDiv> div(OS_NEW HtmlDiv());
		getArea(pageAreaContent)->getControls()->add(div);
	}
	*/


	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLNode> root = document->create(_S("maintenance"));
	String path = getSkin()->getTemplatePath(_S("maintenance.xsl"));
	shared_ptr<HtmlXSLControl> pageTemplate(OS_NEW HtmlXSLControl(loadStylesheet(path), document));
	getArea(pageAreaContent)->getControls()->add(pageTemplate);

	root->setAttributeString(_S("stability_restart_href"), getEventCommand(EVENT_ONSTABILITYRESTART));
	root->setAttributeString(_S("stability_rebuild_href"), getEventCommand(EVENT_ONSTABILITYREBUILD));
	root->setAttributeString(_S("lucene_rebuild_href"), getEventCommand(EVENT_ONLUCENEREBUILD));
	root->setAttributeString(_S("lucene_optimize_href"), getEventCommand(EVENT_ONLUCENEOPTIMIZE));
	root->setAttributeString(_S("database_vacuum_href"), getEventCommand(EVENT_ONDATABASEVACUUM));
	root->setAttributeString(_S("database_rebuild_indexes_href"), getEventCommand(EVENT_ONDATABASEREBUILDINDEXES));
	root->setAttributeString(_S("database_analyze_href"), getEventCommand(EVENT_ONDATABASEANALYZE));
}

void Maintenance::onStabilityRestart()
{
	shared_ptr<Portal> portalParam = getPortalFromUrl();

	PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
	for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
	{
		shared_ptr<Portal> portal = *i;

		if( (portalParam == null) || (portal == portalParam) )
		{
			portal->getSnapshotManager()->resetStabilityStatus(EntitiesSnapshotManager::ssNone);			
		}
	}
}

void Maintenance::onStabilityRebuild()
{
	shared_ptr<Portal> portalParam = getPortalFromUrl();

	PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
	for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
	{
		shared_ptr<Portal> portal = *i;

		if( (portalParam == null) || (portal == portalParam) )
		{
			shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(true);
			shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();
			database->beginTransaction();

			portal->getSnapshotManager()->invalidateTotal(database);
			
			database->commit();
		}
	}
}

void Maintenance::onLuceneRebuild()
{
	shared_ptr<Portal> portalParam = getPortalFromUrl();

	PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
	for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
	{
		shared_ptr<Portal> portal = *i;

		if( (portalParam == null) || (portal == portalParam) )
		{
			shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(true);
			shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();
			database->beginTransaction();

			portal->getSnapshotManager()->destroySearchIndexes(database, true);
			
			database->commit();
		}
	}
}

void Maintenance::onLuceneOptimize()
{
	shared_ptr<Portal> portalParam = getPortalFromUrl();

	PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
	for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
	{
		shared_ptr<Portal> portal = *i;

		if( (portalParam == null) || (portal == portalParam) )
		{
			portal->getSnapshotManager()->optimizeSearchIndexes();			
		}
	}
}

void Maintenance::onDatabaseVacuum()
{
	shared_ptr<Portal> portalParam = getPortalFromUrl();

	// TODO: andrebbe sotto try/catch
	PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
	for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
	{
		shared_ptr<Portal> portal = *i;

		if( (portalParam == null) || (portal == portalParam) )
		{
			shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(true);
			shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();

			database->getConnection()->vacuum();
		}
	}
}

void Maintenance::onDatabaseRebuildIndexes()
{
	shared_ptr<Portal> portalParam = getPortalFromUrl();

	// TODO: andrebbe sotto try/catch
	PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
	for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
	{
		shared_ptr<Portal> portal = *i;

		if( (portalParam == null) || (portal == portalParam) )
		{
			shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(true);
			shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();

			database->beginTransaction();
			database->getConnection()->reindex();
			database->commit();
		}
	}
}

void Maintenance::onDatabaseAnalyze()
{
	try
	{
		shared_ptr<Portal> portalParam = getPortalFromUrl();

		// TODO: andrebbe sotto try/catch
		PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
		for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
		{
			shared_ptr<Portal> portal = *i;

			if( (portalParam == null) || (portal == portalParam) )
			{
				shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(true);
				shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();

				database->beginTransaction();
				database->getConnection()->analyze();
				database->commit();
			}
		}
	}
	catch(std::exception &e)
	{
		showError(e.what());
	}
	catch(...)
	{
		showError(_S("Unknown error."));
	}
}

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
