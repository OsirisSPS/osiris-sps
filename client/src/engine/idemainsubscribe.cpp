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
#include "idemainsubscribe.h"

#include "boost/bind.hpp"
#include "dbdatabasessystem.h"
#include "idbdriver.h"
#include "idboptions.h"
#include "htmlevent.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmltable.h"
#include "htmltableheader.h"
#include "htmltablerow.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "htmlunit.h"
#include "httppostparam.h"
#include "httprequest.h"
#include "ideblock.h"
#include "idebutton.h"
#include "idehelpbox.h"
#include "idepages.h"
#include "osirislink.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace main {

//////////////////////////////////////////////////////////////////////

Subscribe::Subscribe(shared_ptr<HttpSession> session) : PageBase(session),
														m_portalLink(OS_NEW HtmlTextBox()),
														m_showPortalParams(OS_NEW HtmlCheckBox()),
														m_portalName(OS_NEW HtmlTextBox()),
														m_portalID(OS_NEW HtmlTextBox()),
														m_portalPov(OS_NEW HtmlTextBox()),
														m_portalPassword(OS_NEW HtmlTextBox()),														
														m_databaseDriver(OS_NEW HtmlComboBox()),
														m_table(OS_NEW HtmlTable())
{
	m_portalLink->setID(_S("portalLink"));
	m_portalLink->setCss(_S("os_input_full"));

	m_showPortalParams->setID(_S("showPortalParams"));
	m_showPortalParams->setAutoPostBack(true);

	m_portalName->setID(_S("portalName"));
	m_portalName->setCss(_S("os_input_full"));
	
	m_portalID->setID(_S("portalID"));
	m_portalID->setCss(_S("os_input_full"));

	m_portalPassword->setID(_S("portalPassword"));
	m_portalPassword->setCss(_S("os_input_full"));

	m_portalPov->setID(_S("portalPov"));
	m_portalPov->setCss(_S("os_input_full"));

	m_databaseDriver->setID(_S("databaseDriver"));
	m_databaseDriver->setAutoPostBack(true);

	m_table->setCss(_S("os_table_data"));
}

Subscribe::~Subscribe()
{

}

shared_ptr<HtmlTextBox> Subscribe::getDriverOptionControl(const String &driverName, const String &optionName) const
{
	shared_ptr<HtmlTextBox> control;

	String controlID = driverName + optionName;
	DriverOptionsControls::const_iterator i = m_driverOptionsControls.find(controlID);
	if(i == m_driverOptionsControls.end())
	{
		control.reset(OS_NEW HtmlTextBox());
		control->setID(controlID);
		m_driverOptionsControls[controlID] = control;
	}
	else
	{
		control = i->second;
	}

	OS_ASSERT(control != nullptr);
	return control;
}

bool Subscribe::initControlValue(shared_ptr<IHtmlInput> control, const String &paramName)
{
	OS_ASSERT(control != nullptr);
	OS_ASSERT(paramName.empty() == false);

	std::wstring value = getRequest()->getUrlParam(paramName.to_wide());
	if(value.empty())
		return false;

	control->setValue(value);
	return true;
}

String Subscribe::getPageName() const
{
	return _S("main.pages.subscribe");
}

void Subscribe::onInit()
{
	PageBase::onInit();

	const list<shared_ptr<IDbDriver> >::type &drivers = DatabasesSystem::instance()->getDrivers();
	for(list<shared_ptr<IDbDriver> >::type::const_iterator i = drivers.begin(); i != drivers.end(); ++i)
	{
		shared_ptr<IDbDriver> driver = *i;
		m_databaseDriver->addOption(driver->getDescription(), driver->getName());
	}

	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("main.pages.subscribe.title"))));
	getArea(pageAreaContent)->getControls()->add(block);

	block->getBody()->getControls()->add(shared_ptr<IdeHelpBox>(OS_NEW IdeHelpBox(getText(_S("main.pages.subscribe.help")), _S("59FCAB72193EB4E02AF5CFD2D45811ACCB765879"))));

	block->getBody()->getControls()->add(m_table);

	// Header opzioni portale

	shared_ptr<HtmlTableHeader> portalHeader = m_table->addRow()->addHeader();
	portalHeader->setCss(_S("os_subtitle"));
	portalHeader->setColSpan(2);
	portalHeader->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText("main.pages.subscribe.subtitle"))));

	// Link del portale

	m_rowPortalLink = m_table->addRow();

	shared_ptr<HtmlTableCell> portalLinkLeft = m_rowPortalLink->addCell();
	portalLinkLeft->setCss(_S("os_label"));
	portalLinkLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText("main.pages.subscribe.portalLink"))));
	portalLinkLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(":")));

	shared_ptr<HtmlTableCell> portalLinkRight = m_rowPortalLink->addCell();
	portalLinkRight->setCss(_S("os_value"));
	portalLinkRight->getControls()->add(m_portalLink);

	// Checkbox di visualizzazione dei parametri

	shared_ptr<HtmlTableRow> rowShowParams = m_table->addRow();

	shared_ptr<HtmlTableCell> colShowParamsLeft = rowShowParams->addCell();
	colShowParamsLeft->setCss(_S("os_label"));
	colShowParamsLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText("main.pages.subscribe.showParams"))));
	colShowParamsLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(":")));

	shared_ptr<HtmlTableCell> colShowParamsRight = rowShowParams->addCell();
	colShowParamsRight->setCss(_S("os_value"));
	colShowParamsRight->getControls()->add(m_showPortalParams);

	// Nome del portale

	m_rowName = m_table->addRow();

	shared_ptr<HtmlTableCell> nameLeft = m_rowName->addCell();
	nameLeft->setCss(_S("os_label"));
	nameLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText("main.pages.subscribe.name"))));
	nameLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(":")));

	shared_ptr<HtmlTableCell> nameRight = m_rowName->addCell();
	nameRight->setCss(_S("os_value"));
	nameRight->getControls()->add(m_portalName);

	// ID del portale

	m_rowID = m_table->addRow();

	shared_ptr<HtmlTableCell> idLeft = m_rowID->addCell();
	idLeft->setCss(_S("os_label"));
	idLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText("main.pages.subscribe.id"))));
	idLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(":")));

	shared_ptr<HtmlTableCell> idRight = m_rowID->addCell();
	idRight->setCss(_S("os_value"));
	idRight->getControls()->add(m_portalID);

	// Utente del portale

	m_rowUser = m_table->addRow();

	shared_ptr<HtmlTableCell> userLeft = m_rowUser->addCell();
	userLeft->setCss(_S("os_label"));
	userLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText("main.pages.subscribe.user"))));
	userLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(":")));

	shared_ptr<HtmlTableCell> userRight = m_rowUser->addCell();
	userRight->setCss(_S("os_value"));
	userRight->getControls()->add(m_portalPov);

	// Password del portale

	shared_ptr<HtmlTableRow> rowPassword = m_table->addRow();

	shared_ptr<HtmlTableCell> passwordLeft = rowPassword->addCell();
	passwordLeft->setCss(_S("os_label"));
	passwordLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText("main.pages.subscribe.password"))));
	passwordLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(":")));

	shared_ptr<HtmlTableCell> passwordRight = rowPassword->addCell();
	passwordRight->setCss(_S("os_value"));
	passwordRight->getControls()->add(m_portalPassword);

	// Header opzioni database

	shared_ptr<HtmlTableHeader> databaseHeader = m_table->addRow()->addHeader();
	databaseHeader->setCss(_S("os_subtitle"));
	databaseHeader->setColSpan(2);
	databaseHeader->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText("main.pages.subscribe.database.title"))));

	// Driver

	shared_ptr<HtmlTableRow> rowDriver = m_table->addRow();

	shared_ptr<HtmlTableCell> driverLeft = rowDriver->addCell();
	driverLeft->setCss(_S("os_label"));
	driverLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText("main.pages.subscribe.database.driver"))));
	driverLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(":")));

	shared_ptr<HtmlTableCell> driverRight = rowDriver->addCell();
	driverRight->setCss(_S("os_value"));
	driverRight->getControls()->add(m_databaseDriver);

	if(getPostBack() == false)
	{
		shared_ptr<IDbDriver> defaultDriver = DatabasesSystem::instance()->getDefaultDriver();
		if(defaultDriver != nullptr)
			m_databaseDriver->setValue(defaultDriver->getName());
	}
}

void Subscribe::onLoad()
{
	PageBase::onLoad();

	// Opzioni driver

	m_driver = DatabasesSystem::instance()->getDriver(m_databaseDriver->getValue().to_ascii());
	if(m_driver != nullptr)
	{
		m_driverOptions = m_driver->createOptions();
		if(m_driverOptions != nullptr)
		{
			const ordered_map<String, String> &driverParams = m_driverOptions->getParams();
			for(ordered_map<String, String>::const_iterator i = driverParams.begin(); i != driverParams.end(); ++i)
			{
				shared_ptr<HtmlTableRow> rowOption = m_table->addRow();

				shared_ptr<HtmlTableCell> optionLeft = rowOption->addCell();
				optionLeft->setCss(_S("os_label"));
				optionLeft->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(i->first)));

				shared_ptr<HtmlTableCell> optionRight = rowOption->addCell();
				optionRight->setCss(_S("os_value"));

				shared_ptr<HtmlTextBox> optionControl = getDriverOptionControl(m_driver->getName(), i->first);
				optionControl->setValue(i->second);
				optionRight->getControls()->add(optionControl);
			}
		}
	}

	// Comandi

	shared_ptr<HtmlTableCell> cellCommands = m_table->addRow()->addCell();
	cellCommands->setCss(_S("os_commands"));
	cellCommands->setColSpan(2);

	shared_ptr<IdeButton> cmdSubscribe(OS_NEW IdeButton(getText("main.pages.subscribe.action.subscribe")));
	cmdSubscribe->setID("subscribe");
	cmdSubscribe->setIsDefault(true);
	cmdSubscribe->getEventClick()->connect(boost::bind(&Subscribe::onSubscribe, this));
	cellCommands->getControls()->add(cmdSubscribe);

	shared_ptr<IdeButton> cmdCancel(OS_NEW IdeButton(getText("main.pages.subscribe.action.cancel")));
	cmdCancel->setID("cancel");
	cmdCancel->setIsDefault(false);
	cmdCancel->getEventClick()->connect(boost::bind(&Subscribe::onCancel, this));
	cellCommands->getControls()->add(cmdCancel);
}

void Subscribe::onPreRender()
{
	PageBase::onPreRender();

	if(getPostBack() == false)
	{
		bool hasUrlParams = false;
		hasUrlParams |= initControlValue(m_portalName, "name");
		hasUrlParams |= initControlValue(m_portalID, "portal");
		hasUrlParams |= initControlValue(m_portalPov, "pov");

		if(hasUrlParams)
			m_showPortalParams->setCheck(true);
	}

	bool viewParams = m_showPortalParams->getCheck();

	OS_ASSERT(m_rowPortalLink != nullptr);
	m_rowPortalLink->setVisible(!viewParams);
	OS_ASSERT(m_rowName != nullptr);
	m_rowName->setVisible(viewParams);
	OS_ASSERT(m_rowID != nullptr);
	m_rowID->setVisible(viewParams);
	OS_ASSERT(m_rowUser != nullptr);
	m_rowUser->setVisible(viewParams);
}

void Subscribe::onCancel()
{
	redirect(PortalsSystem::instance()->getMainLink(OS_IDE_PAGE_OSIRIS));
}

void Subscribe::onSubscribe()
{
	if(m_driver == nullptr)
	{
		reportError(httpStatusServiceUnavailable);
		return;
	}

	PortalID portalID = m_portalID->getValue().to_ascii();
	String portalName = m_portalName->getValue();
	PovID portalPov = m_portalPov->getValue().to_ascii();

	if(m_showPortalParams->getCheck() == false)
	{
		OsirisLink link;
		if(link.parse(m_portalLink->getValue().to_ascii()) == false || (link.getType() != OsirisLink::linkPortal))
		{
			showError(getText("main.pages.subscribe.error.invalidPortalLink"));
			return;
		}
		
		portalID = link.getPortal();
		portalName = link.getName();
		portalPov = link.getPov();
	}

	shared_ptr<Portal> currentPortal = PortalsSystem::instance()->getPortal(portalID, portalPov);
	if(currentPortal != nullptr)
	{
		redirect(currentPortal->getLink("view"));
		return;
	}

	if(m_driverOptions != nullptr)
	{
		const ordered_map<String, String> &driverParams = m_driverOptions->getParams();
		for(ordered_map<String, String>::const_iterator i = driverParams.begin(); i != driverParams.end(); ++i)
		{
			shared_ptr<IHtmlInput> optionControl = getDriverOptionControl(m_driver->getName(), i->first);
			if(optionControl != nullptr)
			{
				const String &paramValue = optionControl->getValue();
				if(paramValue.empty() == false)
					m_driverOptions->setParam(i->first, paramValue);
			}
		}
	}

	shared_ptr<PortalOptions> options(OS_NEW PortalOptions());
	options->setPortalID(portalID);
	options->setPovID(portalPov);
	options->setName(portalName);
	options->setPassword(m_portalPassword->getValue());
	OS_ASSERT(m_driver->getName() == m_databaseDriver->getValue());
	options->setDatabaseDriver(m_driver->getName());
	options->setDatabaseOptions(m_driverOptions);

	shared_ptr<Portal> portal = PortalsSystem::instance()->subscribePortal(options);
	if(portal != nullptr)
		//redirect(PortalsSystem::instance()->getAccountsLink(PortalsSystem::instance()->getPortal(portalID, portalUser)));
		redirect(PortalsSystem::instance()->getPortalLink(portal, "view"));
	else
		showError(getText("main.pages.subscribe.error.cannotCreate"));
}

//////////////////////////////////////////////////////////////////////

} // main
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
