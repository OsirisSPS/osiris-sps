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
#include "idemainoptions.h"

#include "boost/bind.hpp"
#include "collections.h"
#include "conversions.h"
#include "htmlevent.h"
#include "htmlattributes.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmlcontrols.h"
#include "htmldiv.h"
#include "htmlliteral.h"
#include "htmlspan.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "ideblock.h"
#include "idebutton.h"
#include "idepickerbool.h"
#include "idepickerculture.h"
#include "idepickernumber.h"
#include "idehelpbox.h"
#include "idepages.h"
#include "ideskin.h"
#include "idepickerskin.h"
#include "option.h"
#include "options.h"
#include "portalsportalssystem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((ide)(main))

//////////////////////////////////////////////////////////////////////

typedef OS_NAMESPACE_NAME::Options GlobalOptions;

//////////////////////////////////////////////////////////////////////

Options::Options(shared_ptr<HttpSession> session) : PageBase(session),
													m_skinPicker(OS_NEW IdePickerSkin()),
													m_culturePicker1(OS_NEW IdePickerCulture()),
													m_culturePicker2(OS_NEW IdePickerCulture())
{	
}

Options::~Options()
{

}

String Options::getControlIDFromOptionName(String optionName) const
{
	optionName.remove(".");
	optionName.remove("_");
	OS_ASSERT(optionName.empty() == false);

	return optionName;
}

// 07/2011: Copiata da quella sotto, non ho trovato modo di non passare gli ultimi 2 parametri boost:function.
shared_ptr<IHtmlControl> Options::createControl(const String &optionName, shared_ptr<IHtmlControl> optionControl)
{
    OS_ASSERT(optionControl != null);
        
	String title = getText("options." + optionName + ".title");
	String desc = getText("options." + optionName + ".description");

	shared_ptr<HtmlDiv> div(OS_NEW HtmlDiv());
	div->setCss("os_option");
	shared_ptr<HtmlDiv> divInfo(OS_NEW HtmlDiv());	
	divInfo->setCss("os_option_name");
	div->getControls()->add(divInfo);

	shared_ptr<HtmlDiv> divInfoTitle(OS_NEW HtmlDiv());
	divInfoTitle->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(title)));
	divInfoTitle->setCss("os_title");
	divInfo->getControls()->add(divInfoTitle);

	shared_ptr<HtmlDiv> divInfoDesc(OS_NEW HtmlDiv());
	divInfoDesc->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(desc)));
	divInfoDesc->setCss("os_description");
	divInfo->getControls()->add(divInfoDesc);

	shared_ptr<HtmlDiv> divField(OS_NEW HtmlDiv());
	divField->setCss("os_option_value");
	div->getControls()->add(divField);

	optionControl->setID(getControlIDFromOptionName(optionName));
	divField->getControls()->add(optionControl);

	shared_ptr<HtmlDiv> divClear(OS_NEW HtmlDiv());
	divClear->setCss("os_clear");
	div->getControls()->add(divClear);
	
	return div;
}

shared_ptr<IHtmlControl> Options::createControl(const String &optionName, shared_ptr<IHtmlControl> optionControl, const boost::function<void()> &optionReader, const boost::function<void()> &optionWriter)
{
    OS_ASSERT(optionControl != null);
    OS_ASSERT(optionReader.empty() == false);
    OS_ASSERT(optionWriter.empty() == false);

    if(getPostBack() == false)
        optionReader();

	String title = getText("options." + optionName + ".title");
	String desc = getText("options." + optionName + ".description");

	shared_ptr<HtmlDiv> div(OS_NEW HtmlDiv());
	div->setCss("os_option");
	shared_ptr<HtmlDiv> divInfo(OS_NEW HtmlDiv());	
	divInfo->setCss("os_option_name");
	div->getControls()->add(divInfo);

	shared_ptr<HtmlDiv> divInfoTitle(OS_NEW HtmlDiv());
	//divInfoTitle->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(title + _S("(") + optionName + _S(")"))));
	divInfoTitle->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(title)));
	divInfoTitle->getAttributes()->set(_S("data-os-tooltip"), _S("Code: ") + optionName);
	divInfoTitle->setCss("os_title");
	divInfo->getControls()->add(divInfoTitle);

	shared_ptr<HtmlDiv> divInfoDesc(OS_NEW HtmlDiv());
	divInfoDesc->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(desc)));
	divInfoDesc->setCss("os_description");
	divInfo->getControls()->add(divInfoDesc);

	shared_ptr<HtmlDiv> divField(OS_NEW HtmlDiv());
	divField->setCss("os_option_value");
	div->getControls()->add(divField);

	optionControl->setID(getControlIDFromOptionName(optionName));
	divField->getControls()->add(optionControl);

	shared_ptr<HtmlDiv> divClear(OS_NEW HtmlDiv());
	divClear->setCss("os_clear");
	div->getControls()->add(divClear);

	OS_ASSERT(GlobalOptions::instance()->exists(optionName));
	OS_ASSERT(m_setters.find(optionName) == m_setters.end());
	
	m_setters[optionName] = optionWriter;

	return div;
}

shared_ptr<IHtmlControl> Options::createControlBool(const String &optionName)
{
	shared_ptr<IdePickerBool> optionControl(OS_NEW IdePickerBool());

    boost::function<void()> optionReader = boost::bind(&IdePickerBool::setCheck, optionControl, boost::bind(&GlobalOptions::getOption<bool>, GlobalOptions::instance(), optionName));
    boost::function<void()> optionWriter = boost::bind(&GlobalOptions::setOption<bool>, GlobalOptions::instance(), optionName, boost::bind(&IdePickerBool::getCheck, optionControl));

	return createControl(optionName, optionControl, optionReader, optionWriter);
}

shared_ptr<IHtmlControl> Options::createControlEnum(const String &optionName, const StringCollection<String> &values)
{
	shared_ptr<HtmlComboBox> optionControl(OS_NEW HtmlComboBox());

	OS_ASSERT(values.empty() == false);
	for(StringCollection<String>::iterator i = values.begin(); i != values.end(); ++i)
		optionControl->addOption(i->first, *i->second);

    boost::function<void()> optionReader = boost::bind(&HtmlComboBox::setValue, optionControl, boost::bind(&GlobalOptions::getOption<String>, GlobalOptions::instance(), optionName));
    boost::function<void()> optionWriter = boost::bind(&GlobalOptions::setOption<String>, GlobalOptions::instance(), optionName, boost::bind(&HtmlComboBox::getValue, optionControl));

	return createControl(optionName, optionControl, optionReader, optionWriter);
}

shared_ptr<IHtmlControl> Options::createControlNumber(const String &optionName)
{
	shared_ptr<IdePickerNumber> optionControl(OS_NEW IdePickerNumber());
	optionControl->setCss("os_input_full");

    boost::function<void()> optionReader = boost::bind(&IdePickerNumber::setValue, optionControl, boost::bind(&GlobalOptions::getOption<int>, GlobalOptions::instance(), optionName));
    boost::function<void()> optionWriter = boost::bind(&GlobalOptions::setOption<int>, GlobalOptions::instance(), optionName, boost::bind(&IdePickerNumber::getValue, optionControl));

	return createControl(optionName, optionControl, optionReader, optionWriter);
}

shared_ptr<IHtmlControl> Options::createControlString(const String &optionName)
{
	shared_ptr<HtmlTextBox> optionControl(OS_NEW HtmlTextBox());
	optionControl->setCss("os_input_full");

    boost::function<void()> optionReader = boost::bind(&HtmlTextBox::setValue, optionControl, boost::bind(&GlobalOptions::getOption<String>, GlobalOptions::instance(), optionName));
    boost::function<void()> optionWriter = boost::bind(&GlobalOptions::setOption<String>, GlobalOptions::instance(), optionName, boost::bind(&HtmlTextBox::getValue, optionControl));

	return createControl(optionName, optionControl, optionReader, optionWriter);
}

shared_ptr<IHtmlControl> Options::createPage(shared_ptr<IHtmlControl> tab, const String &name, uint32 &pageIndex)
{
	OS_ASSERT(tab != null);
	OS_ASSERT(name.empty() == false);

	shared_ptr<HtmlDiv> tabPageHeader(OS_NEW HtmlDiv());
	shared_ptr<HtmlSpan> tabPageCounter(OS_NEW HtmlSpan());
	tabPageCounter->setUID(name + "_counter");
	tabPageCounter->setCss("os_options_counter");
	//tabPageCounter->setCss("os_pagecounter");
	tabPageHeader->setUID(name + "_header");
	tabPageHeader->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(getText("main.pages.options." + name))));
	tabPageHeader->getControls()->add(tabPageCounter);
	tabPageHeader->getAttributes()->set("data-os-tabType", "header");
	tabPageHeader->getAttributes()->set("data-os-tabPage", conversions::to_utf16(pageIndex));
	tab->getControls()->add(tabPageHeader);

	shared_ptr<HtmlDiv> tabPageBody(OS_NEW HtmlDiv());
	tabPageBody->setUID(name + "_body");
	tabPageBody->getAttributes()->set("data-os-optionPage", name);
	tabPageBody->getAttributes()->set("data-os-tabType", "body");
	tabPageBody->getAttributes()->set("data-os-tabPage", conversions::to_utf16(pageIndex));
	tab->getControls()->add(tabPageBody);

	pageIndex++;

	return tabPageBody;
}

shared_ptr<IHtmlControl> Options::createPageGroup(shared_ptr<IHtmlControl> tabPage, const String &name)
{
	OS_ASSERT(tabPage != null);
	OS_ASSERT(name.empty() == false);

	shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText("main.pages.options." + name)));
	block->setType(blockTypeMinimal);
	tabPage->getControls()->add(block);
	return block->getBody();
}

/*
shared_ptr<IHtmlControl> Options::createSeparator()
{
	shared_ptr<IHtmlControl> div(OS_NEW HtmlDiv());
	div->setCss("os_separator");
	return div;
}
*/

/*
void Options::onCancel()
{
	redirect(PortalsSystem::instance()->getMainLink(OS_IDE_PAGE_OSIRIS));
}
*/

void Options::onSave()
{
	/*
	bool needReload = false;

	if(GlobalOptions::instance()->getOption<String>(GlobalOptions::ide_options::skin_id) != m_skinPicker->getValue())
		needReload = true();
	*/

	for(Setters::const_iterator i = m_setters.begin(); i != m_setters.end(); i->second(), ++i);


	GlobalOptions::instance()->setOption(GlobalOptions::ide_options::skin_id, m_skinPicker->getValue());
	GlobalOptions::instance()->setOption(GlobalOptions::language_options::id, m_culturePicker1->getValue());
	GlobalOptions::instance()->setOption(GlobalOptions::language_options::alternative, m_culturePicker2->getValue());

	GlobalOptions::instance()->save();
	GlobalOptions::instance()->update();

	//showMessage(getText("main.pages.options.options_updated"));

	redirect(PortalsSystem::instance()->getMainLink(OS_IDE_PAGE_OSIRIS));
}

String Options::getPageName() const
{
	return _S("main.pages.options");
}

void Options::onInit()
{
	PageBase::onInit();

	m_block.reset(OS_NEW IdeBlock(getText("main.pages.options.title")));
	getArea(pageAreaContent)->getControls()->add(m_block);

	shared_ptr<HtmlDiv> divActions(OS_NEW HtmlDiv());
	divActions->setCss("os_commands_right");
	m_block->getBody()->getControls()->add(divActions);

	shared_ptr<HtmlDiv> filter(OS_NEW HtmlDiv());	
	filter->setUID("filterbox");
	shared_ptr<HtmlSpan> filterTitle(OS_NEW HtmlSpan(getText("main.pages.options.filter") + _S(": ")));
	shared_ptr<HtmlTextBox> filterInput(OS_NEW HtmlTextBox());
	filterInput->setUID("filterinput");
	filter->getControls()->add(filterTitle);
	//filter->getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(_S("<br>"))));
	filter->getControls()->add(filterInput);
	//getArea(pageAreaContent)->getControls()->add(filter);	
	divActions->getControls()->add(filter);	

	shared_ptr<IdeButton> cmdSave(OS_NEW IdeButton());
	cmdSave->setID("save");
	cmdSave->setIconHref(getSkin()->getImageUrl(_S("/icons/16x16/save.png")));
	cmdSave->setCaption(getText("main.pages.options.actions.save"));
	cmdSave->getEventClick()->connect(boost::bind(&Options::onSave, this));
	divActions->getControls()->add(cmdSave);

	/*
	shared_ptr<IdeButton> cmdCancel(OS_NEW IdeButton());
	cmdCancel->setID("cancel");
	cmdCancel->setCaption(getText("main.pages.options.actions.cancel"));
	cmdCancel->getEventClick()->connect(boost::bind(&Options::onCancel, this));
	divActions->getControls()->add(cmdCancel);
	*/

	uint32 pageIndex = 0;

	//m_block->getBody()->getControls()->add(shared_ptr<IdeHelpBox>(OS_NEW IdeHelpBox(getText("main.pages.options.help"), "")));

	shared_ptr<HtmlDiv> tab(OS_NEW HtmlDiv());
	
	tab->setStyle("display:none;");
	tab->setUID(_S("options_tab"));
	tab->getAttributes()->set("data-os-otype", "tab");
	tab->getAttributes()->set("data-os-layout", "left");
	tab->getAttributes()->set("data-os-storage", "main.options.tab");
	//getArea(pageAreaContent)->getControls()->add(tab);
	m_block->getBody()->getControls()->add(tab);	

	shared_ptr<IHtmlControl> pageGeneral = createPage(tab, "general", pageIndex);
	pageGeneral->getControls()->add(createControlBool("p2p.enable"));
	pageGeneral->getControls()->add(createControlNumber("p2p.server.port"));
	pageGeneral->getControls()->add(createControlBool("p2p.server.enable_upnp"));
	pageGeneral->getControls()->add(createControlBool("isis.enable"));
	pageGeneral->getControls()->add(createControlNumber("web.server.port"));
	pageGeneral->getControls()->add(createControlBool("web.server.enable_upnp"));
	pageGeneral->getControls()->add(createControlBool("ui.start_minimized"));
	pageGeneral->getControls()->add(createControlBool("ui.exit_confirm"));
	pageGeneral->getControls()->add(createControlBool("ui.tray.minimize"));
	pageGeneral->getControls()->add(createControlBool("ui.tray.always"));
		
		
	
		
	
	shared_ptr<IHtmlControl> pageLookAndFeel = createPage(tab, "look_and_feel", pageIndex);
	pageLookAndFeel->getControls()->add(createControl("ide.skin.id", m_skinPicker));
	if(getPostBack() == false)
		m_skinPicker->setValue(GlobalOptions::instance()->getOption<String>(GlobalOptions::ide_options::skin_id).to_ascii());
	pageLookAndFeel->getControls()->add(createControlNumber("ide.objects.hot_state_threshold"));
	pageLookAndFeel->getControls()->add(createControlNumber("ide.pager.items"));
	pageLookAndFeel->getControls()->add(createControlNumber("ide.pager.pages"));
	
	//pageLookAndFeel->getControls()->add(createControlString("ide.skin.id"));
	pageLookAndFeel->getControls()->add(createControlString("url.protocols.find_link"));	
	pageLookAndFeel->getControls()->add(createControlString("url.protocols.collect"));	

	shared_ptr<IHtmlControl> groupIrc = createPageGroup(pageLookAndFeel, "irc");
	groupIrc->getControls()->add(createControlBool("irc.server.automatic_connection"));
	groupIrc->getControls()->add(createControlString("irc.server.address"));
	groupIrc->getControls()->add(createControlNumber("irc.server.port"));
	groupIrc->getControls()->add(createControlNumber("irc.server.port_ssl"));
	groupIrc->getControls()->add(createControlBool("irc.server.enable_ssl"));
	groupIrc->getControls()->add(createControlNumber("irc.server.timeout"));
	groupIrc->getControls()->add(createControlString("irc.nickname"));
	groupIrc->getControls()->add(createControlString("irc.alternate_nickname"));
	groupIrc->getControls()->add(createControlString("irc.username"));
	groupIrc->getControls()->add(createControlString("irc.email"));
	groupIrc->getControls()->add(createControlString("irc.default_channel"));
	groupIrc->getControls()->add(createControlNumber("irc.sounds.mode"));

	shared_ptr<IHtmlControl> pageInternationalization = createPage(tab, "internationalization", pageIndex);
	
	StringCollection<String> languageTimeOffsetEnum;
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 10000000)), "10000000");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -43200)), "-43200");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -39600)), "-39600");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -36000)), "-36000");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -34200)), "-34200");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -32400)), "-32400");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -28800)), "-28800");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -25200)), "-25200");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -21600)), "-21600");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -18000)), "-18000");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -16200)), "-16200");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -14400)), "-14400");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -12600)), "-12600");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -10800)), "-10800");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -7200)), "-7200");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), -3600)), "-3600");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 0)), "0");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 3600)), "3600");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 7200)), "7200");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 10800)), "10800");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 12600)), "12600");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 14400)), "14400");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 16200)), "16200");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 18000)), "18000");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 19800)), "19800");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 20700)), "20700");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 21600)), "21600");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 23400)), "23400");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 25200)), "25200");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 28800)), "28800");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 31500)), "31500");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 32400)), "32400");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 34200)), "34200");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 36000)), "36000");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 37800)), "37800");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 39600)), "39600");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 41400)), "41400");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 43200)), "43200");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 45900)), "45900");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 46800)), "46800");
	languageTimeOffsetEnum.set(getText(String::format(_S("timeTable_GMT_%d").c_str(), 50400)), "50400");		
	pageInternationalization->getControls()->add(createControlEnum("language.time_offset", languageTimeOffsetEnum));

	StringCollection<String> languageTimeDSTEnum;
	languageTimeDSTEnum.set(getText(String::format(_S("timeTable_DST_%d").c_str(), 10000000)), "10000000");
	languageTimeDSTEnum.set(getText(String::format(_S("timeTable_DST_%d").c_str(), -3600)), "-3600");
	languageTimeDSTEnum.set(getText(String::format(_S("timeTable_DST_%d").c_str(), 0)), "0");
	languageTimeDSTEnum.set(getText(String::format(_S("timeTable_DST_%d").c_str(), 3600)), "3600");
	pageInternationalization->getControls()->add(createControlEnum("language.time_dst", languageTimeDSTEnum));
	
	pageInternationalization->getControls()->add(createControl("language.id", m_culturePicker1));
	if(getPostBack() == false)
		m_culturePicker1->setValue(GlobalOptions::instance()->getDefaultLanguage());	
	pageInternationalization->getControls()->add(createControl("language.alternative", m_culturePicker2));
	if(getPostBack() == false)
		m_culturePicker2->setValue(GlobalOptions::instance()->getOption<String>(GlobalOptions::language_options::alternative));	
	
	

	// ------------------------------------------------

	shared_ptr<IHtmlControl> pagePrivacy = createPage(tab, "privacy", pageIndex);
	pagePrivacy->getControls()->add(createControlBool("privacy.allow_external_images"));
	pagePrivacy->getControls()->add(createControlBool("privacy.allow_external_urls"));
	pagePrivacy->getControls()->add(createControlString("url.protocols.no_confirm"));
	pagePrivacy->getControls()->add(createControlString("url.protocols.no_redirect"));
	pagePrivacy->getControls()->add(createControlString("web.redirect_url"));
	StringCollection<String> objectModeEnum;
	objectModeEnum.set(getText("options.anonymity.object_mode.no"), "0");
	objectModeEnum.set(getText("options.anonymity.object_mode.confirm"), "1");
	objectModeEnum.set(getText("options.anonymity.object_mode.yes"), "2");
	pagePrivacy->getControls()->add(createControlEnum("anonymity.object_mode", objectModeEnum));
	/*
	pagePrivacy->getControls()->add(createControlString("anonymity.object_trusted_domains"));
	pagePrivacy->getControls()->add(createControlString("anonymity.object_trusted_types"));
	*/

	shared_ptr<IHtmlControl> pageNetwork = createPage(tab, "network", pageIndex);	
	pageNetwork->getControls()->add(createControlNumber("net.max_download"));
	pageNetwork->getControls()->add(createControlNumber("net.max_upload"));
	pageNetwork->getControls()->add(createControlNumber("p2p.max_incoming_connections"));
	pageNetwork->getControls()->add(createControlNumber("p2p.max_outgoing_connections"));	

	shared_ptr<IHtmlControl> groupNetworkDHT = createPageGroup(pageNetwork, "dht");
	pageNetwork->getControls()->add(createControlNumber("plugins.dht.port"));
	pageNetwork->getControls()->add(createControlString("plugins.dht.bootstrap_routers"));
	pageNetwork->getControls()->add(createControlNumber("plugins.dht.nodes_cache_size"));
	
	shared_ptr<IHtmlControl> groupNetworkProxy = createPageGroup(pageNetwork, "proxy");
	StringCollection<String> proxyTypeEnum;
	proxyTypeEnum.set(getText("options.net.proxy.type.none"), conversions::to_utf16(OS_PROXY_TYPE_NONE));
	proxyTypeEnum.set(getText("options.net.proxy.type.http"), conversions::to_utf16(OS_PROXY_TYPE_HTTP));
	groupNetworkProxy->getControls()->add(createControlEnum("net.proxy.type", proxyTypeEnum));
	groupNetworkProxy->getControls()->add(createControlString("net.proxy.host"));
	groupNetworkProxy->getControls()->add(createControlNumber("net.proxy.port"));
	groupNetworkProxy->getControls()->add(createControlString("net.proxy.username"));
	groupNetworkProxy->getControls()->add(createControlString("net.proxy.password"));
	groupNetworkProxy->getControls()->add(createControlBool("net.proxy.restrict_to_unsafe_connections"));

	shared_ptr<IHtmlControl> groupNetworkIsis = createPageGroup(pageNetwork, "isis");
	groupNetworkIsis->getControls()->add(createControlString("system.isis"));
	groupNetworkIsis->getControls()->add(createControlBool("system.isis.enable_ssl"));
	groupNetworkIsis->getControls()->add(createControlNumber("system.isis.port"));
	groupNetworkIsis->getControls()->add(createControlNumber("system.isis.port_ssl"));

	shared_ptr<IHtmlControl> pageAdvanced = createPage(tab, "advanced", pageIndex);

	pageAdvanced->getControls()->add(shared_ptr<IdeHelpBox>(OS_NEW IdeHelpBox(getText("main.pages.options.advanced.warning"), "")));

	pageAdvanced->getControls()->add(createControlString("system.instance_name"));	

	shared_ptr<IHtmlControl> groupLogging = createPageGroup(pageAdvanced, "logging");
	groupLogging->getControls()->add(createControlBool("log.enable"));
	groupLogging->getControls()->add(createControlBool("log.serialize"));
	groupLogging->getControls()->add(createControlBool("ui.tray.balloon"));
	StringCollection<String> logLevelEnum;
	logLevelEnum.set(getText("options.log.level.debug"), "0");
	logLevelEnum.set(getText("options.log.level.notice"), "1");
	logLevelEnum.set(getText("options.log.level.info"), "2");
	logLevelEnum.set(getText("options.log.level.warning"), "3");
	logLevelEnum.set(getText("options.log.level.error"), "4");
	groupLogging->getControls()->add(createControlEnum("log.level", logLevelEnum));
	
	shared_ptr<IHtmlControl> groupWebAccess = createPageGroup(pageAdvanced,"web_access");
	groupWebAccess->getControls()->add(createControlBool("web.enable"));
	groupWebAccess->getControls()->add(createControlBool("web.allow_remote_connections"));	
	groupWebAccess->getControls()->add(createControlString("web.server.bind_ip"));
	groupWebAccess->getControls()->add(createControlString("web.browser"));
	groupWebAccess->getControls()->add(createControlNumber("web.session.duration"));
	groupWebAccess->getControls()->add(createControlString("web.mcp_password"));
	
	shared_ptr<IHtmlControl> groupNetworkP2P = createPageGroup(pageAdvanced, "network_p2p");
	groupNetworkP2P->getControls()->add(createControlString("p2p.server.bind_ip"));
	groupNetworkP2P->getControls()->add(createControlNumber("p2p.bootstrap.request_delay"));
	groupNetworkP2P->getControls()->add(createControlNumber("p2p.bootstrap.notify_delay"));
	groupNetworkP2P->getControls()->add(createControlNumber("net.connection_timeout"));

	shared_ptr<IHtmlControl> groupTuning = createPageGroup(pageAdvanced, "tuning");
	groupTuning->getControls()->add(createControlNumber("web.cache_size"));
	StringCollection<String> searchLevelEnum;
	searchLevelEnum.set(getText("options.portals.search_level.none"), "0");
	searchLevelEnum.set(getText("options.portals.search_level.quick"), "1");
	searchLevelEnum.set(getText("options.portals.search_level.accurate"), "2");
	groupTuning->getControls()->add(createControlEnum("portals.search_level", searchLevelEnum));
	groupTuning->getControls()->add(createControlNumber("portals.search_limit"));
	groupTuning->getControls()->add(createControlNumber("portals.objects_step"));
	groupTuning->getControls()->add(createControlNumber("portals.stability_delta"));

	shared_ptr<IHtmlControl> groupTuningSqlite = createPageGroup(groupTuning, "sqlite");
	groupTuningSqlite->getControls()->add(createControlNumber("plugins.sqlite.cache_size"));
	groupTuningSqlite->getControls()->add(createControlBool("plugins.sqlite.enable_asyncio"));
	groupTuningSqlite->getControls()->add(createControlBool("plugins.sqlite.enable_memorymanager"));
	groupTuningSqlite->getControls()->add(createControlString("plugins.sqlite.synchronous"));
	groupTuningSqlite->getControls()->add(createControlNumber("plugins.sqlite.temp_store"));

	shared_ptr<IHtmlControl> miscPage;

	Locked<const XMLOptions::Options, boost::shared_mutex>::shared options = GlobalOptions::instance()->getOptions();
	for(XMLOptions::Options::const_iterator i = options->begin(); i != options->end(); ++i)
	{
		shared_ptr<Option> option = i->second;
		OS_ASSERT(option != null);

		if(m_setters.find(i->first) != m_setters.end())
			continue;

		if(option->getPublic() == true) // I don't want option like "ui.window.position" ...
		{		
			shared_ptr<IHtmlControl> optionControl;
			switch(option->getType())
			{
			case DataItem::vt_null:		//OS_ASSERTFALSE();		// L'opzione è stata creata senza essere precedentemente registrata. Commentato: Non necessariamente c'è.
										break;

			case DataItem::vt_bool:		optionControl = createControlBool(i->first);
										break;

			case DataItem::vt_byte:
			case DataItem::vt_int32:
			case DataItem::vt_uint32:
			case DataItem::vt_double:
			case DataItem::vt_int64:
			case DataItem::vt_uint64:
										optionControl = createControlNumber(i->first);
										break;

			case DataItem::vt_string:	optionControl = createControlString(i->first);
										break;

			default:					OS_ASSERTFALSE();
										break;
			}

			if(optionControl != null)
			{
				if(miscPage == null)
				{
					miscPage = createPage(tab, "misc", pageIndex);
					miscPage->getControls()->add(shared_ptr<IdeHelpBox>(OS_NEW IdeHelpBox(getText("main.pages.options.advanced.warning"), "")));
				}

				/*
				if(miscPage->getControls()->size() > 0)
					miscPage->getControls()->add(createSeparator());
				*/

				miscPage->getControls()->add(optionControl);
			}
		}
	}	
}

void Options::onLoad()
{
	PageBase::onLoad();

	addCss(getSkin()->getResourceUrl(_S("css/main/options.less")));
	addJavascript(getSkin()->getResourceUrl(_S("js/main/options.js")));

	//OS_ASSERT(m_block != null);

	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((ide)(main))

//////////////////////////////////////////////////////////////////////
