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
#include "idemainisis.h"

#include "convert.h"
#include "conversions.h"
#include "iportaldatabase.h"
#include "idbconnection.h"
#include "entitiessnapshotmanager.h"
#include "eventssource.h"
#include "ideskin.h"
#include "idemainsettings.h"
#include "isisendpoint.h"
#include "locked.h"
#include "platformmanager.h"
#include "portalsoptionsshared.h"
#include "portalsportals.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "realtimestats.h"
#include "htmlevent.h"
#include "httprequest.h"
#include "htmlcontrols.h"
#include "htmlxslcontrol.h"
#include "portalsportal.h"
#include "realtimestats.h"
#include "xmldocument.h"
#include "xmlexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

Isis::Isis(shared_ptr<HttpSession> session) : PageBase(session)
{
}

Isis::~Isis()
{

}

String Isis::getPageName() const
{
	return _S("main.pages.isis");
}

void Isis::onLoad()
{
	RealtimeStatsScopeTimer RSS1(_S("Debug"), String::format(_S("Isis::onLoad").c_str()));

	PageBase::onLoad();

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLExporter> pageExporter(OS_NEW XMLExporter(document->create(_S("isis"))));
	String path = getSkin()->getTemplatePath(_S("isis.xsl"));
	shared_ptr<HtmlXSLControl> pageTemplate(OS_NEW HtmlXSLControl(loadStylesheet(path), document));
	getArea(pageAreaContent)->getControls()->add(pageTemplate);

	shared_ptr<XMLExporter> portalsExporter = pageExporter->createChild<XMLExporter>(_S("portals"));

	PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
	for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
	{
		shared_ptr<Portal> portal = *i;

		shared_ptr<PortalOptions> portalOptions = portal->getOptions();

		if(portalOptions != nullptr)
		{
			Locked<const PortalOptions::IsisEndpoints>::unique isisEndpoints = portalOptions->getIsisEndpoints();
			if(isisEndpoints->empty() == false)
			{
				shared_ptr<XMLExporter> portalExporter = portalsExporter->createChild<XMLExporter>(_S("portal"));

				portalExporter->setAttributeString(_S("id"), portal->getFullPovID());
				portalExporter->setAttributeString(_S("href"), portal->getLink("view"));
				portalExporter->setAttributeString(_S("name"), portalOptions->getName());
				portalExporter->setAttributeString(_S("description"), portal->getOptionsShared()->getPortalDescription());
				//portalExporter->setAttributeString(_S("type"), (portal->getType() == portalTypeAnarchic) ? _S("anarchist") : _S("monarchist"));
				//portalExporter->setAttributeString(_S("access"), (portal->getAccess() == portalAccessPublic) ? _S("public") : _S("private"));

				shared_ptr<XMLExporter> endpointsExporter = portalExporter->createChild<XMLExporter>(_S("isisEndpoints"));

				for(PortalOptions::IsisEndpoints::const_iterator i = isisEndpoints->begin(); i != isisEndpoints->end(); ++i)
				{
					shared_ptr<XMLExporter> endpointExporter = endpointsExporter->createChild<XMLExporter>(_S("isisEndpoint"));

					shared_ptr<IsisEndpoint> isisEndpoint = i->second;

					endpointExporter->setAttributeString(_S("name"), isisEndpoint->getName());
					endpointExporter->setAttributeString(_S("url"), isisEndpoint->getUrl().toString());
					endpointExporter->setAttributeBool(_S("enabled"), isisEndpoint->getEnabled());
					endpointExporter->setAttributeString(_S("password"), isisEndpoint->getPassword());
					endpointExporter->setAttributeString(_S("lastEvent"), isisEndpoint->getLastEvent());

					String editUrl;
					ordered_map<std::wstring, std::wstring> editParams;
					editParams.set(_W("isisendpoint"), conversions::to_wstring(i->first));
					editUrl = PortalsSystem::instance()->getSettingsLink(portal, editParams);

					endpointExporter->createAction(_S("edit"), editUrl);

					//endpointExporter->createAction(_S("remove"), getEventCommand(EVENT_ONREMOVEISISENDPOINT, conversions::to_utf16(i->first)));
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
