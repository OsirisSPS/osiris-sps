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
#include "ideomleditor.h"

#include "boost/bind.hpp"
#include "conversions.h"
#include "datatree.h"
#include "eventssource.h"
#include "omlcontext.h"
#include "omlemoticon.h"
#include "omlmanager.h"
#include "idesystem.h"
#include "idepages.h"
#include "ideskin.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmlcontrols.h"
#include "htmlevent.h"
#include "htmltextarea.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "regexmanager.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IdeOMLEditor::EVENT_PREVIEW = _S("onPreview");
const String IdeOMLEditor::EVENT_HTML2OML = _S("onHtml2Oml");
const String IdeOMLEditor::EVENT_HTMLANDBBCODE2OML = _S("onHtmlAndBbcode2Oml");
const String IdeOMLEditor::EVENT_NORMAL = _S("onNormal");
const String IdeOMLEditor::EVENT_EXTENDED = _S("onExtended");

//////////////////////////////////////////////////////////////////////

IdeOMLEditor::IdeOMLEditor() : m_extended(false),
					   m_row(false),
					   m_editor(OS_NEW HtmlTextArea()),
					   m_modeLite(OS_NEW HtmlCheckBox()),
					   m_modeMode(OS_NEW HtmlComboBox()),
					   m_modeSecure(OS_NEW HtmlCheckBox()),
					   m_modeReport(OS_NEW HtmlCheckBox())
{
	m_editor->setID(_S("editor"));
	m_modeLite->setID(_S("modeLite"));
	m_modeMode->setID(_S("modeMode"));
	m_modeSecure->setID(_S("modeSecure"));
	m_modeReport->setID(_S("modeReport"));

	getTemplate()->addChildParam(m_editor, _S("text_editor"));
	getTemplate()->addChildParam(m_modeLite, _S("mode_lite"));
	getTemplate()->addChildParam(m_modeMode, _S("mode_mode"));
	getTemplate()->addChildParam(m_modeSecure, _S("mode_secure"));
	getTemplate()->addChildParam(m_modeReport, _S("mode_report"));

	getEvents()->get(EVENT_PREVIEW)->connect(boost::bind(&IdeOMLEditor::onPreview, this));
	getEvents()->get(EVENT_HTML2OML)->connect(boost::bind(&IdeOMLEditor::onHtml2Oml, this));
	getEvents()->get(EVENT_HTMLANDBBCODE2OML)->connect(boost::bind(&IdeOMLEditor::onHtmlAndBbcode2Oml, this));	
	getEvents()->get(EVENT_NORMAL)->connect(boost::bind(&IdeOMLEditor::onNormal, this));
	getEvents()->get(EVENT_EXTENDED)->connect(boost::bind(&IdeOMLEditor::onExtended, this));
}

IdeOMLEditor::~IdeOMLEditor()
{

}

const String & IdeOMLEditor::getValue() const
{
	return m_editor->getValue();
}

void IdeOMLEditor::setValue(const String &value)
{
	m_editor->setValue(optimize(value));
}

String IdeOMLEditor::optimize(const String &value) const
{
	String newValue = value;
	String base = regex_quote(IdeSystem::instance()->getLocalUrl(String::EMPTY));
	newValue = regex_replace(newValue, RegexManager::instance()->getRegex(base + _S("(.*?)\?(.*?)"), true), _S("osiris://|url|$1|$2"));
	newValue = regex_replace(newValue, RegexManager::instance()->getRegex(base + _S("(.*?)"), true), _S("osiris://|url|$1"));
	return newValue;
}

void IdeOMLEditor::onInit()
{
	ControlBase::onInit();

	m_modeLite->setCaption(getPage()->getText(_S("omleditor.mode.lite")));
	m_modeSecure->setCaption(getPage()->getText(_S("omleditor.mode.secure")));
	m_modeReport->setCaption(getPage()->getText(_S("omleditor.mode.report")));
	//m_mode->addOption(getPage()->getText(_S("omleditor.mode.normal")), conversions::to_utf16(true));
	//m_mode->addOption(getPage()->getText(_S("omleditor.mode.lite")), conversions::to_utf16(false));
	//m_mode->setValue(conversions::to_utf16(true));

	m_modeMode->addOption(getPage()->getText(_S("omleditor.mode.osiris")), conversions::to_utf16<uint32>(omlRenderModeOsiris));
	m_modeMode->addOption(getPage()->getText(_S("omleditor.mode.search")), conversions::to_utf16<uint32>(omlRenderModeSearch));
	m_modeMode->addOption(getPage()->getText(_S("omleditor.mode.external")), conversions::to_utf16<uint32>(omlRenderModeExternal));

}

void IdeOMLEditor::onLoad()
{
	ControlBase::onLoad();

	getPage()->addJavascript(getPage()->getSkin()->getResourceUrl(_S("js/omleditor.js")));

	shared_ptr<XMLDocument> document = getDocument();
	shared_ptr<XMLNode> root = document->create(_S("omlEditor"));

	root->setAttributeString(_S("areaId"), m_editor->getTarget());
}

void IdeOMLEditor::onPreRender()
{
	ControlBase::onPreRender();

	shared_ptr<XMLNode> root = getDocument()->getRoot();

	root->setAttributeString(_S("preview_href"), getEventCommand(EVENT_PREVIEW));
	root->setAttributeString(_S("html2oml_href"), getEventCommand(EVENT_HTML2OML));
	root->setAttributeString(_S("htmlandbbcode2oml_href"), getEventCommand(EVENT_HTMLANDBBCODE2OML));
	root->setAttributeString(_S("normal_href"), getEventCommand(EVENT_NORMAL));
	root->setAttributeString(_S("extended_href"), getEventCommand(EVENT_EXTENDED));
	root->setAttributeString(_S("help_href"), PortalsSystem::instance()->getMainLink("omlhelp"));

	// Emoticons
	{
		shared_ptr<XMLNode> emoticonsNode = root->addChild(_S("emoticons"));
		const OMLManager::Emoticons &emoticons = OMLManager::instance()->getEmoticons();
		for(OMLManager::Emoticons::const_iterator i = emoticons.begin(); i != emoticons.end(); ++i)
		{
			shared_ptr<OMLEmoticon> emo = *i;
			const String &code = emo->getCode();
			const String &name = emo->getName();

			shared_ptr<XMLNode> emoticonNode = emoticonsNode->addChild(_S("emoticon"));

			emoticonNode->setAttributeString(_S("code"), code);
			emoticonNode->setAttributeString(_S("name"), name);
			emoticonNode->setAttributeString(_S("href"), getPage()->getSkin()->getImageUrl(_S("emoticons/")+name+_S(".gif")));
		}

		m_editor->setRows(6);
	}
}

void IdeOMLEditor::onLoadViewState(const DataTree &state)
{
	ControlBase::onLoadViewState(state);

	m_extended = state.getV(_S("extended"), false);
}

void IdeOMLEditor::onSaveViewState(DataTree &state)
{
	ControlBase::onSaveViewState(state);

	if(m_extended)
		state.setV(_S("extended"), true);		// Salva il valore solo se diverso dal default
}

String IdeOMLEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("omleditor.xsl"));
}

void IdeOMLEditor::onPreview()
{
	String preview = getValue();
	// Controlla se  stato specificato un callback sulla preview
	if(m_previewCallback)
		// Richiama il callback di preview
		m_previewCallback(preview);

	shared_ptr<OMLContext> context = getPage()->parseOmlEx(preview, true, !m_modeLite->getCheck(), m_modeSecure->getCheck(), getRow(), (OMLRenderMode) conversions::from_utf16<uint32>(m_modeMode->getValue()));

	String html = context->getOutput();
	getDocument()->getRoot()->setAttributeString(_S("preview"), html);

	getDocument()->getRoot()->setAttributeString(_S("report"), context->getReport(preview, m_modeReport->getCheck()));
}

void IdeOMLEditor::onHtml2Oml()
{
	/*
	shared_ptr<OMLContext> context = Manager::reverseParse(getValue());

	String html = context->getOutput();
	String osml = context->getInput();
	setValue(osml);

	getDocument()->getRoot()->setAttributeString(_S("preview"), html);

	getDocument()->getRoot()->setAttributeString(_S("report"), context->getReport(getReportDetailed()));
	*/

	setValue(OMLManager::instance()->reverseParse(getValue(), "html"));

	onPreview();
}

void IdeOMLEditor::onHtmlAndBbcode2Oml()
{	
	setValue(OMLManager::instance()->reverseParse(getValue(), "bbc+html"));

	onPreview();
}

void IdeOMLEditor::onNormal()
{
	setExtended(false);
}

void IdeOMLEditor::onExtended()
{
	setExtended(true);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
