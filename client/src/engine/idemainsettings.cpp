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
#include "idemainsettings.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "convert.h"
#include "conversions.h"
#include "eventssource.h"
#include "iportaldatabase.h"
#include "htmlevent.h"
#include "ipaddress.h"
#include "ideblock.h"
#include "idebutton.h"
#include "idepages.h"
#include "ideskin.h"
#include "htmlevent.h"
#include "css.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmldiv.h"
#include "htmlliteral.h"
#include "htmlspan.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltablerow.h"
#include "htmltextbox.h"
#include "htmlwriter.h"
#include "httprequest.h"
#include "idesession.h"
#include "isisendpoint.h"
#include "locked.h"
#include "options.h"
#include "platformmanager.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "xmldocument.h"
#include "xmlexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

const String Settings::EVENT_ONSAVEOPTIONS = _S("onSaveOptions");
const String Settings::EVENT_ONADDISISENDPOINT = _S("onAddIsisEndpoint");
const String Settings::EVENT_ONEDITISISENDPOINT = _S("onEditIsisEndpoint");
const String Settings::EVENT_ONREMOVEISISENDPOINT = _S("onRemoveIsisEndpoint");
const std::wstring Settings::PARAM_MESSAGE(L"message");
const std::wstring Settings::PARAM_ISISENDPOINT(L"isisendpoint");
const std::wstring Settings::PARAM_ISIS(L"isis");

//////////////////////////////////////////////////////////////////////

Settings::Settings(shared_ptr<HttpSession> session) : PageBase(session),
													  m_name(OS_NEW HtmlTextBox()),
													  m_exchangeEnabled(OS_NEW HtmlCheckBox()),
													  m_publicDefault(OS_NEW HtmlCheckBox()),
													  m_publicEnabled(OS_NEW HtmlCheckBox()),													  
													  m_loginEnabled(OS_NEW HtmlCheckBox()),
													  m_registerEnabled(OS_NEW HtmlCheckBox()),
													  m_isisName(OS_NEW HtmlTextBox()),
													  m_isisUrl(OS_NEW HtmlTextBox()),
													  m_isisEnabled(OS_NEW HtmlCheckBox()),
													  m_isisPassword(OS_NEW HtmlTextBox()),
													  m_sync(OS_NEW HtmlComboBox())
{
	getEvents()->get(EVENT_ONSAVEOPTIONS)->connect(boost::bind(&Settings::onSaveOptions, this, _1));
	getEvents()->get(EVENT_ONADDISISENDPOINT)->connect(boost::bind(&Settings::onAddIsisEndpoint, this, _1));
	getEvents()->get(EVENT_ONEDITISISENDPOINT)->connect(boost::bind(&Settings::onEditIsisEndpoint, this, _1));
	getEvents()->get(EVENT_ONREMOVEISISENDPOINT)->connect(boost::bind(&Settings::onRemoveIsisEndpoint, this, _1));	
}

Settings::~Settings()
{

}

String Settings::getEditIsisEndpointLink(const shared_ptr<Portal> &portal, uint32 endpointID) const
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(PARAM_ISISENDPOINT, conversions::to_wstring(endpointID));
	return PortalsSystem::instance()->getSettingsLink(portal, params);
}

void Settings::displayMessage()
{
	const String &message = getRequest()->getUrlParam(PARAM_MESSAGE);
	if(message.empty() == false)
	{
		switch(conversions::from_utf16<uint32>(message))
		{
		case messageIsisAdded:				showMessage(getText(_S("main.pages.settings.message.isis_endpoint_added")));
											break;

		case messageIsisEdited:				showMessage(getText(_S("main.pages.settings.message.isis_endpoint_edited")));
											break;

		case messageIsisRemoved:			showMessage(getText(_S("main.pages.settings.message.isis_endpoint_removed")));
											break;

		default:							OS_ASSERTFALSE();
											break;
		}
	}
}

bool Settings::initIsisControlValue(shared_ptr<IHtmlInput> control, const String &paramName)
{
	OS_ASSERT(control != nullptr);
	OS_ASSERT(paramName.empty() == false);

	std::wstring value = getRequest()->getUrlParam(paramName.to_wide());
	if(value.empty())
		return false;

	control->setValue(value);
	return true;
}

String Settings::getPageName() const
{
	return _S("main.pages.settings");
}

void Settings::onLoad()
{
	PageBase::onLoad();

	shared_ptr<Portal> portal = getPortalFromUrl();
	if(portal == nullptr)
	{
		showError(_S("Invalid portal"));
		return;
	}

	shared_ptr<PortalOptions> portalOptions = portal->getOptions();
	if(portalOptions == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	displayMessage();

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLExporter> pageExporter(OS_NEW XMLExporter(document->create(_S("settings"))));
	String path = getSkin()->getTemplatePath(_S("settings.xsl"));
	shared_ptr<HtmlXSLControl> pageTemplate(OS_NEW HtmlXSLControl(loadStylesheet(path), document));
	getArea(pageAreaContent)->getControls()->add(pageTemplate);

	pageExporter->createAction(_S("saveOptions"), getEventCommand(EVENT_ONSAVEOPTIONS));
	
	String editedIsisEndpoint = getRequest()->getUrlParam(PARAM_ISISENDPOINT);

	{
		Locked<const PortalOptions::IsisEndpoints>::unique isisEndpoints = portalOptions->getIsisEndpoints();
		if(isisEndpoints->empty() == false)
		{
			shared_ptr<XMLExporter> endpointsExporter = pageExporter->createChild<XMLExporter>(_S("isisEndpoints"));

			for(PortalOptions::IsisEndpoints::const_iterator i = isisEndpoints->begin(); i != isisEndpoints->end(); ++i)
			{
				shared_ptr<XMLExporter> endpointExporter = endpointsExporter->createChild<XMLExporter>(_S("isisEndpoint"));

				shared_ptr<IsisEndpoint> isisEndpoint = i->second;

				endpointExporter->setAttributeString(_S("name"), isisEndpoint->getName());
				endpointExporter->setAttributeString(_S("url"), isisEndpoint->getUrl().toString());
				endpointExporter->setAttributeBool(_S("enabled"), isisEndpoint->getEnabled());
				endpointExporter->setAttributeString(_S("password"), isisEndpoint->getPassword());
				endpointExporter->setAttributeString(_S("lastEvent"), isisEndpoint->getLastEvent());

				endpointExporter->createAction(_S("edit"), getEditIsisEndpointLink(portal, i->first));
				endpointExporter->createAction(_S("remove"), getEventCommand(EVENT_ONREMOVEISISENDPOINT, conversions::to_utf16(i->first)));

				if(editedIsisEndpoint == conversions::to_utf16(i->first))
					m_editedIsisEndpoint = isisEndpoint;
			}
		}
	}

	if(m_editedIsisEndpoint != nullptr)
		pageExporter->createAction(_S("edit"), getEventCommand(EVENT_ONEDITISISENDPOINT), OS_PAGE_SETTINGS_GROUP_ISIS);
	else
		pageExporter->createAction(_S("add"), getEventCommand(EVENT_ONADDISISENDPOINT), OS_PAGE_SETTINGS_GROUP_ISIS);

	m_name->setID(_S("name"));
	m_name->setCss(_S("os_input_full"));
	m_exchangeEnabled->setID(_S("exchangeEnabled"));
	m_publicDefault->setID(_S("publicDefault"));
	m_publicEnabled->setID(_S("publicEnabled"));
	m_loginEnabled->setID(_S("loginEnabled"));
	m_registerEnabled->setID(_S("registerEnabled"));	
	
	m_isisName->setID(_S("isisName"));
	m_isisName->setCss(_S("os_input_full"));
	m_isisUrl->setID(_S("isisUrl"));
	m_isisUrl->setCss(_S("os_input_full"));
	m_isisEnabled->setID(_S("isisEnabled"));
	m_isisPassword->setID(_S("isisPassword"));
	m_isisPassword->setCss(_S("os_input_full"));
	m_isisPassword->setPassword(true);

	m_sync->setID(_S("sync"));	
	m_sync->addOption(getText(_S("main.pages.settings.sync.none")), String::EMPTY);
	PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
	for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
	{
		shared_ptr<Portal> portal2 = *i;

		if( (portal->getPortalID() == portal2->getPortalID()) && (portal->getPovID() != portal2->getPovID()) )
			m_sync->addOption(portal2->getPovName(), portal2->getFullPovID());
	}

	pageTemplate->addChildParam(m_name);
	pageTemplate->addChildParam(m_exchangeEnabled);
	if(Options::instance()->getWebMcpPassword().empty() == false)
	{
		pageTemplate->addChildParam(m_publicDefault);
		pageTemplate->addChildParam(m_publicEnabled);
		pageTemplate->addChildParam(m_loginEnabled);
		pageTemplate->addChildParam(m_registerEnabled);		
	}
	pageTemplate->addChildParam(m_isisName);
	pageTemplate->addChildParam(m_isisUrl);
	pageTemplate->addChildParam(m_isisEnabled);
	pageTemplate->addChildParam(m_isisPassword);
	pageTemplate->addChildParam(m_sync);

	// Valori di default
	if(getPostBack() == false)
	{
		m_name->setValue(portal->getOptions()->getName());
		m_exchangeEnabled->setCheck(portal->getOptions()->getExchangeEnabled());
		m_publicDefault->setCheck(portal->getFullPovID() == Options::instance()->getWebDefaultPortal());
		m_publicEnabled->setCheck(portal->getOptions()->getPublicEnabled());
		m_loginEnabled->setCheck(portal->getOptions()->getLoginEnabled());
		m_registerEnabled->setCheck(portal->getOptions()->getRegisterEnabled());
				
		bool showIsis = (m_editedIsisEndpoint != nullptr) || conversions::from_utf16<bool>(getRequest()->getUrlParam(PARAM_ISIS));
		if(m_editedIsisEndpoint != nullptr)
		{
			m_isisName->setValue(m_editedIsisEndpoint->getName());
			m_isisUrl->setValue(m_editedIsisEndpoint->getUrl().toString());
			m_isisPassword->setValue(m_editedIsisEndpoint->getPassword());
			m_isisEnabled->setCheck(m_editedIsisEndpoint->getEnabled());
		}
		else
		{
			showIsis |= initIsisControlValue(m_isisName, "name");
			showIsis |= initIsisControlValue(m_isisUrl, "url");
			showIsis |= initIsisControlValue(m_isisPassword, "password");
			m_isisEnabled->setCheck(true);
		}

		m_sync->setValue(portal->getSync());

		if(showIsis)
			pageExporter->setAttributeBool(_S("showIsis"), true);
	}
}

void Settings::onSaveOptions(IEvent *e)
{
	shared_ptr<Portal> portal = getPortalFromUrl();
	if(portal == nullptr)
	{
		showError(_S("Invalid portal"));
		return;
	}

	portal->getOptions()->setName(m_name->getValue());
	portal->getOptions()->setExchangeEnabled(m_exchangeEnabled->getCheck());

	if(Options::instance()->getWebMcpPassword().empty() == false)
	{
		portal->getOptions()->setPublicEnabled(m_publicEnabled->getCheck());
		portal->getOptions()->setLoginEnabled(m_loginEnabled->getCheck());
		portal->getOptions()->setRegisterEnabled(m_registerEnabled->getCheck());

		if(m_publicDefault->getCheck())
			Options::instance()->setWebDefaultPortal(portal->getFullPovID());
		else if(Options::instance()->getWebDefaultPortal() == portal->getFullPovID())
			Options::instance()->setWebDefaultPortal(String::EMPTY);
	}

	portal->setSync(m_sync->getValue());

	portal->save();

	redirect(OS_IDE_PAGE_OSIRIS);
}

void Settings::onRemoveIsisEndpoint(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == nullptr)
		return;

	shared_ptr<Portal> portal = getPortalFromUrl();
	if(portal == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<PortalOptions> portalOptions = portal->getOptions();
	if(portalOptions == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	if(htmlEvent->empty())
	{
		OS_ASSERTFALSE();
		return;
	}

	if(portalOptions->removeIsisEndpoint(conversions::from_utf16<uint32>(htmlEvent->get(0))))
	{
		// Salva le opzioni a livello di portale
		portal->save();

		ordered_map<std::wstring, std::wstring> params;
		params.set(PARAM_ISIS, conversions::to_wstring(true));
		params.set(PARAM_MESSAGE, conversions::to_wstring(static_cast<uint32>(messageIsisRemoved)));
		redirect(PortalsSystem::instance()->getSettingsLink(portal, params));
	}
	else
	{
		showError(getText(_S("main.pages.settings.error.cannotRemoveIsis")));
	}
}

void Settings::onAddIsisEndpoint(IEvent *e)
{
	shared_ptr<Portal> portal = getPortalFromUrl();
	if(portal == nullptr)
		return;

	String isisUrl = algorithms::trim_copy(m_isisUrl->getValue());
	if(isisUrl.empty())
	{
		showError(getText(_S("main.pages.settings.error.invalidIsisEndpoint")));
		return;
	}

	if(portal->getOptions()->createIsisEndpoint(algorithms::trim_copy(m_isisName->getValue()), isisUrl.to_ascii(), m_isisEnabled->getCheck(), algorithms::trim_copy(m_isisPassword->getValue())))
	{
		// Salva le opzioni a livello di portale
		portal->save();

		ordered_map<std::wstring, std::wstring> params;
		params.set(PARAM_ISIS, conversions::to_wstring(true));
		params.set(PARAM_MESSAGE, conversions::to_wstring(static_cast<uint32>(messageIsisAdded)));
		redirect(PortalsSystem::instance()->getSettingsLink(portal, params));
	}
	else
	{
        showError(getText(_S("main.pages.settings.error.cannotInsertIsisEndpoint")));
	}
}

void Settings::onEditIsisEndpoint(IEvent *e)
{
	if(m_editedIsisEndpoint == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	m_editedIsisEndpoint->setName(m_isisName->getValue());
	m_editedIsisEndpoint->setUrl(m_isisUrl->getValue().to_ascii());
	m_editedIsisEndpoint->setPassword(m_isisPassword->getValue());
	m_editedIsisEndpoint->setEnabled(m_isisEnabled->getCheck());
	m_editedIsisEndpoint->setLastEvent(String::EMPTY); // 0.14

	shared_ptr<Portal> portal = getPortalFromUrl();
	if(portal == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	// Salva le opzioni a livello di portale
	portal->save();

	ordered_map<std::wstring, std::wstring> params;
	params.set(PARAM_ISIS, conversions::to_wstring(true));
	params.set(PARAM_MESSAGE, conversions::to_wstring(static_cast<uint32>(messageIsisEdited)));
	redirect(PortalsSystem::instance()->getSettingsLink(portal, params));
}

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
