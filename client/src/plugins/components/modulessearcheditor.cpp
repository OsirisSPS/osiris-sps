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
#include "modulessearcheditor.h"

#include "convert.h"
#include "conversions.h"
#include "htmlcontrols.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmltextbox.h"
#include "htmlxslcontrol.h"
#include "idepickerbool.h"
#include "ideomleditor.h"
#include "idesearchglobalparams.h"
#include "modulessearch.h"
#include "modulesmodulesconfig.h"

#include "searchquery.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

extern OS_NAMESPACE_NAME::String g_componentsPath;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {

//////////////////////////////////////////////////////////////////////

namespace modules {

//////////////////////////////////////////////////////////////////////

SearchEditor::SearchEditor() :	m_query(OS_NEW SearchQuery()),
								m_globalParams(OS_NEW IdeSearchGlobalParams(m_query)),
								m_showParams(OS_NEW IdePickerBool()),
								m_directRun(OS_NEW IdePickerBool()),
								m_allowRss(OS_NEW IdePickerBool()),
								m_rssDescription(OS_NEW HtmlTextBox()),
								m_rssBody(OS_NEW HtmlComboBox())
{
	m_globalParams->setID(_S("globalParams"));
	m_showParams->setID(_S("showParams"));
	m_directRun->setID(_S("directRun"));
	m_allowRss->setID(_S("allowRss"));
	m_rssDescription->setID(_S("rssDescription"));
	m_rssBody->setID(_S("rssBody"));	
}

SearchEditor::~SearchEditor()
{

}

void setAttribute(shared_ptr<XMLNode> node, shared_ptr<IdePickerBool> &child)
{
	if(child == null || child->hasID() == false)
	{
		OS_ASSERTFALSE();
		return;
	}

	node->setAttributeBool(child->getID(), child->getCheck());
}

void setAttribute(shared_ptr<XMLNode> node, shared_ptr<HtmlComboBox> &child)
{
	if(child == null || child->hasID() == false)
	{
		OS_ASSERTFALSE();
		return;
	}

	node->setAttributeString(child->getID(), child->getValue());
}

void setAttribute(shared_ptr<XMLNode> node, shared_ptr<HtmlTextBox> &child)
{
	if(child == null || child->hasID() == false)
	{
		OS_ASSERTFALSE();
		return;
	}

	node->setAttributeString(child->getID(), child->getValue());
}


bool SearchEditor::createDocument()
{
	if(EditorBase::createDocument() == false)
		return false;
	
	shared_ptr<XMLNode> root = getModuleDocument()->create(OS_MODULES_SEARCH_ROOT);
	
	m_globalParams->fillQuery();
	m_query->writeXml(root);
	setAttribute(root, m_showParams);
	setAttribute(root, m_directRun);
	setAttribute(root, m_allowRss);
	setAttribute(root, m_rssDescription);
	setAttribute(root, m_rssBody);

	return true;
}

void SearchEditor::onInit()
{
	EditorBase::onInit();

	m_rssBody->addOption(getPage()->getText(_S("plugins.components.modules.search.rssBody.full")), _S("full"));
	m_rssBody->addOption(getPage()->getText(_S("plugins.components.modules.search.rssBody.link")), _S("link"));
	m_rssBody->addOption(getPage()->getText(_S("plugins.components.modules.search.rssBody.none")), _S("none"));

	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();

	if(root != null)
	{
		m_query->readXml(root);

		m_showParams->setCheck(root->getAttributeBool(m_showParams->getID()));
		m_directRun->setCheck(root->getAttributeBool(m_directRun->getID()));
		m_allowRss->setCheck(root->getAttributeBool(m_allowRss->getID()));	
		m_rssDescription->setValue(root->getAttributeString(m_rssDescription->getID()));	
		m_rssBody->setValue(root->getAttributeString(m_rssBody->getID()));	
	}
}

void SearchEditor::onLoad()
{
	EditorBase::onLoad();
	
	String path = utils::makeFilePath(utils::makeFolderPath(g_componentsPath, _S("search")), _S("search_editor.xsl"));

	shared_ptr<XMLStylesheet> stylesheet(OS_NEW XMLStylesheet());
	// Inizializza le funzioni del template
	getPage()->initStylesheet(stylesheet);
	// Carica l'xsl
	if(stylesheet->parseFile(path))
	{
		// URGENT devo per forza creare un documento?
		// Se il secondo parametro della XSLControl  null non crea il controllo,
		// e non posso passare una "getDocument()" perch se  una nuova istanza  null pure lui...
		shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
		shared_ptr<XMLNode> root = document->create(OS_MODULES_SEARCH_ROOT);

		shared_ptr<HtmlXSLControl> templ(OS_NEW HtmlXSLControl(stylesheet, document));
		
		templ->addChildParam(m_globalParams);

		templ->addChildParam(m_showParams);
		templ->addChildParam(m_directRun);
		templ->addChildParam(m_allowRss);
		templ->addChildParam(m_rssDescription);
		templ->addChildParam(m_rssBody);

		getControls()->add(templ);
	}
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
