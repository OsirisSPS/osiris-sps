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
#include "idepickerculture.h"

#include "boost/bind.hpp"
#include "convert.h"
#include "conversions.h"
#include "constants.h"
#include "eventssource.h"
#include "idesession.h"
#include "ideskin.h"
#include "ievent.h"
#include "languageculture.h"
#include "languagemanager.h"
#include "objectsuser.h"
#include "options.h"
#include "htmlcheckbox.h"
#include "htmlcontrols.h"
#include "htmlevent.h"
#include "htmlspan.h"
#include "htmltextbox.h"
#include "htmlxslcontrol.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IdePickerCulture::EVENT_ONSELECT = _S("onSelect");

//////////////////////////////////////////////////////////////////////

IdePickerCulture::IdePickerCulture() : m_input(OS_NEW HtmlTextBox()),
						     m_showAll(OS_NEW HtmlCheckBox())
{
	m_showSystem = false;

	m_showAll->setID(_S("showAll"));
	m_input->setID(_S("hidden"));
	m_input->setHidden(true);

	getEvents()->get(EVENT_ONSELECT)->connect(boost::bind(&IdePickerCulture::onSelect, this, _1));	
}

IdePickerCulture::~IdePickerCulture()
{

}

String IdePickerCulture::getValue() const
{
	return m_input->getValue(); 
}

void IdePickerCulture::setValue(const String &id)
{
	m_input->setValue(id);
}

void IdePickerCulture::addCulture(shared_ptr<XMLNode> node, shared_ptr<LanguageCulture> culture, const String &current, uint32 level)
{
	shared_ptr<XMLNode> node_culture = node->addChild(_S("culture"));

	String id = culture->getID();
	node_culture->setAttributeString(_S("id"), id);
	if(id == current)
		node_culture->setAttributeBool(_S("current"), true);
	String parent = String::EMPTY;
	if (culture->getParent() != null)
		parent = culture->getParent()->getID();
	node_culture->setAttributeString(_S("parent"), parent);
	node_culture->setAttributeString(_S("LCID"), conversions::to_utf16(culture->getLCID()));
	node_culture->setAttributeString(_S("iso_name"), culture->getIsoName());
	node_culture->setAttributeString(_S("english_name"), culture->getEnglishName());
	node_culture->setAttributeString(_S("native_name"), culture->getNativeName());
	node_culture->setAttributeBool(_S("is_neutral"), culture->isNeutralCulture());
	node_culture->setAttributeString(_S("default_not_neutral"), culture->getDefaultNotNeutral());
	node_culture->setAttributeBool(_S("translation_native"), culture->getTranslationAvaible());
	node_culture->setAttributeBool(_S("translation_derived"), culture->getTranslationDerivedAvaible());
	node_culture->setAttributeUint32(_S("level"),level);
	
	//int32 timeOffset = getLoggedUser()->getUser()->time_offset;
	int32 timeOffset = getPage()->getTimeOffset();
	int32 timeDST = getPage()->getTimeDST();

	node_culture->setAttributeString(_S("sample_long_datetime"), culture->formatDate(DateTime::now(), DateTime::dpLongDateTimePattern, timeOffset, timeDST));
	node_culture->setAttributeString(_S("sample_short_datetime"), culture->formatDate(DateTime::now(), DateTime::dpShortDateTimePattern, timeOffset, timeDST));
	
	node_culture->setAttributeString(_S("href"), getEventCommand(IdePickerCulture::EVENT_ONSELECT, id));
	
	shared_ptr<XMLNode> node_cultures = node_culture->addChild(_S("cultures"));
	const LanguageCulture::Cultures &cultures = culture->getCultures();
	for(LanguageCulture::Cultures::const_iterator i = cultures.begin(); i != cultures.end(); ++i)
	{
		shared_ptr<LanguageCulture> subCulture = i->second;

		bool show = true;
		if(m_showAll->getCheck() == false)
		{
			if(subCulture->getTranslationDerivedAvaible())
				show = true;
			else
				show = subCulture->isParentOf(current);
		}
		
		if(show)		
			addCulture(node_cultures, subCulture, current, level+1);
	}
}

void IdePickerCulture::onLoad()
{
	ControlBase::onLoad();	

	getControls()->add(m_input);
	
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	m_nodeRoot = document->create(_S("root"));
	
	String path = getSkin()->getTemplatePath(_S("culturepicker.xsl"));
	shared_ptr<HtmlXSLControl> templ(OS_NEW HtmlXSLControl(getPage()->loadStylesheet(path), document));
	
	// Carica il template di riferimento
	getControls()->add(templ);	
	
	m_showAll->setCaption(getPage()->getText(_S("cultures.show_all")));
	m_showAll->setAutoPostBack(true);
	templ->addChildParam(m_showAll, _S("showAll"));
}

void IdePickerCulture::onPreRender()
{
	ControlBase::onPreRender();

	String current = getValue();

	m_nodeRoot->setAttributeBool(_S("showAll"), m_showAll->getCheck());
	m_nodeRoot->setAttributeBool(_S("showSystem"), m_showSystem);

	m_nodeRoot->setAttributeString(_S("current"), current);
	m_nodeRoot->setAttributeString(_S("system"), Options::instance()->getDefaultLanguage());
	m_nodeRoot->setAttributeString(_S("system_href"), getEventCommand(IdePickerCulture::EVENT_ONSELECT, _S("sys")));

	addCulture(m_nodeRoot, LanguageManager::instance()->getInvariant(), current, 0);
}

shared_ptr<EventSource> IdePickerCulture::getEventSelect() const
{
	return getEvents()->get(EVENT_ONSELECT);
}

void IdePickerCulture::onSelect(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);

	String val = htmlEvent->get(0).to_ascii();

	if(val == _S("sys"))
		val = String::EMPTY;

	setValue(val);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
