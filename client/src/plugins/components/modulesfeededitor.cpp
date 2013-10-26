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
#include "modulesfeededitor.h"

#include "convert.h"
#include "conversions.h"
#include "ideomleditor.h"
#include "idepickerbool.h"
#include "modulesfeed.h"
#include "modulesmodulesconfig.h"

#include "htmlcontrols.h"
#include "htmlcombobox.h"
#include "htmltextbox.h"
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

FeedEditor::FeedEditor() : m_url(OS_NEW HtmlTextBox()),
						   m_showTitle(OS_NEW IdePickerBool()),
						   m_showDescription(OS_NEW IdePickerBool()),
						   m_showImage(OS_NEW IdePickerBool()),
						   m_nItems(OS_NEW HtmlTextBox()),
						   m_showItemDescription(OS_NEW HtmlComboBox()),
						   m_templateModel(OS_NEW HtmlComboBox()),
						   m_templateCustom(OS_NEW IdeOMLEditor())
{
	m_url->setID(OS_MODULES_FEED_URL);
	m_url->setCss(_S("os_input_full"));
	m_showTitle->setID(OS_MODULES_FEED_SHOWTITLE);
	m_showDescription->setID(OS_MODULES_FEED_SHOWDESCRIPTION);
	m_showImage->setID(OS_MODULES_FEED_SHOWIMAGE);
	m_nItems->setID(OS_MODULES_FEED_NITEMS);
	m_showItemDescription->setID(OS_MODULES_FEED_SHOWITEMDESCRIPTION);
	m_templateModel->setID(OS_MODULES_FEED_TEMPLATEMODEL);
	m_templateCustom->setID(OS_MODULES_FEED_TEMPLATECUSTOM);
}

FeedEditor::~FeedEditor()
{

}

bool FeedEditor::createDocument()
{
	if(EditorBase::createDocument() == false)
		return false;

	String url = m_url->getValue();
	bool showTitle = m_showTitle->getCheck();
	bool showDescription = m_showDescription->getCheck();
	bool showImage = m_showImage->getCheck();
	int32 nItems = conversions::from_utf16<int32>(m_nItems->getValue());
	String showItemDescription = m_showItemDescription->getValue();
	String templateModel = m_templateModel->getValue();
	String templateCustom = m_templateCustom->getValue();

	shared_ptr<XMLNode> root = getModuleDocument()->create(OS_MODULES_FEED_ROOT);
	root->setAttributeString(OS_MODULES_FEED_URL, url);
	root->setAttributeBool(OS_MODULES_FEED_SHOWTITLE, showTitle);
	root->setAttributeBool(OS_MODULES_FEED_SHOWDESCRIPTION, showDescription);
	root->setAttributeBool(OS_MODULES_FEED_SHOWIMAGE, showImage);
	root->setAttributeInt32(OS_MODULES_FEED_NITEMS, nItems);
	root->setAttributeString(OS_MODULES_FEED_SHOWITEMDESCRIPTION, showItemDescription);
	root->setAttributeString(OS_MODULES_FEED_TEMPLATEMODEL, templateModel);
	root->setAttributeString(OS_MODULES_FEED_TEMPLATECUSTOM, templateCustom);

	return true;
}

void FeedEditor::onInit()
{
	EditorBase::onInit();

	m_url->setStyle(_S("width:100%"));
	m_nItems->setStyle(_S("width:50px"));
	m_templateModel->addOption(_S("links"),_S("links"));
	m_templateModel->addOption(_S("popup"),_S("popup"));
	m_templateModel->addOption(_S("threecolumns"),_S("threecolumns"));
	m_templateModel->addOption(_S("plainlinks"),_S("plainlinks"));
	m_templateModel->addOption(_S("custom"),_S(""));
	//m_templateModel->setAutoPostBack(true);

	m_showItemDescription->addOption(_S("no"),_S("no"));
	m_showItemDescription->addOption(_S("yes"),_S("yes"));
	m_showItemDescription->addOption(_S("popup"),_S("popup"));



	
}

void FeedEditor::onLoad()
{
	EditorBase::onLoad();


	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	m_url->setValue(root->getAttributeString(OS_MODULES_FEED_URL));
	m_showTitle->setCheck(root->getAttributeBool(OS_MODULES_FEED_SHOWTITLE));
	m_showDescription->setCheck(root->getAttributeBool(OS_MODULES_FEED_SHOWDESCRIPTION));
	m_showImage->setCheck(root->getAttributeBool(OS_MODULES_FEED_SHOWIMAGE));
	m_nItems->setValue(root->getAttributeString(OS_MODULES_FEED_NITEMS));
	m_showItemDescription->setValue(root->getAttributeString(OS_MODULES_FEED_SHOWITEMDESCRIPTION));
	m_templateModel->setValue(root->getAttributeString(OS_MODULES_FEED_TEMPLATEMODEL));
	m_templateCustom->setValue(root->getAttributeString(OS_MODULES_FEED_TEMPLATECUSTOM));


	String path = utils::makeFilePath(utils::makeFolderPath(g_componentsPath, _S("feed")), _S("feed_editor.xsl"));

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
		shared_ptr<XMLNode> root = document->create(OS_MODULES_FEED_ROOT);

		String currentTemplateModel = m_templateModel->getValue();
		root->setAttributeString(OS_MODULES_FEED_TEMPLATEMODEL, currentTemplateModel);

		shared_ptr<HtmlXSLControl> templ(OS_NEW HtmlXSLControl(stylesheet, document));
		templ->addChildParam(m_url);
		templ->addChildParam(m_showTitle);
		templ->addChildParam(m_showDescription);
		templ->addChildParam(m_showImage);
		templ->addChildParam(m_nItems);
		templ->addChildParam(m_showItemDescription);
		templ->addChildParam(m_templateModel);
		templ->addChildParam(m_templateCustom);
		getControls()->add(templ);
	}
}


//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
