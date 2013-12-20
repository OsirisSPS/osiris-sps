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
#include "documentationdocumentationeditor.h"

#include "convert.h"
#include "conversions.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmltextbox.h"
#include "htmlcontrols.h"
#include "htmlxslcontrol.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

extern OS_NAMESPACE_NAME::String g_componentsPath;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace documentation {

//////////////////////////////////////////////////////////////////////

DocumentationEditor::DocumentationEditor() : m_tocLayout(OS_NEW HtmlComboBox()),
										     m_tocMinTexts(OS_NEW HtmlTextBox()),
											 m_contentLevel(OS_NEW HtmlTextBox()),
											 m_subindexLevel(OS_NEW HtmlTextBox()),
											 m_subindexShowTexts(OS_NEW HtmlCheckBox())
{
	m_tocLayout->setID(_S("tocLayout"));
	m_tocMinTexts->setID(_S("tocMinTexts"));
	m_contentLevel->setID(_S("contentLevel"));
	m_subindexLevel->setID(_S("subindexLevel"));
	m_subindexShowTexts->setID(_S("subindexShowTexts"));
}

DocumentationEditor::~DocumentationEditor()
{

}

bool DocumentationEditor::createDocument()
{
	if(EditorBase::createDocument() == false)
		return false;

	String tocLayout = m_tocLayout->getValue();	
	int32 tocMinTexts = conversions::from_utf16<int32>(m_tocMinTexts->getValue());
	int32 contentLevel = conversions::from_utf16<int32>(m_contentLevel->getValue());
	int32 subindexLevel = conversions::from_utf16<int32>(m_subindexLevel->getValue());
	bool subindexShowTexts = m_subindexShowTexts->getCheck();
	
	shared_ptr<XMLNode> root = getComponentDocument()->create("documentation");
	root->setAttributeString("toc_layout", tocLayout);		
	root->setAttributeInt32("toc_mintexts", tocMinTexts);		
	root->setAttributeInt32("content_level", contentLevel);		
	root->setAttributeInt32("subindex_level", subindexLevel);		
	root->setAttributeBool("subindex_showtexts", subindexShowTexts);		

	return true;
}

void DocumentationEditor::onInit()
{
	EditorBase::onInit();
	
	m_tocLayout->addOption(_S("right"),_S("right"));
	m_tocLayout->addOption(_S("top"),_S("top"));		
}

void DocumentationEditor::onLoad()
{
	EditorBase::onLoad();

	shared_ptr<XMLNode> root = getComponentDocument()->getRoot();
	m_tocLayout->setValue(root->getAttributeString("toc_layout"));
	m_tocMinTexts->setValue(root->getAttributeString("toc_mintexts"));
	m_contentLevel->setValue(root->getAttributeString("content_level"));
	m_subindexLevel->setValue(root->getAttributeString("subindex_level"));
	m_subindexShowTexts->setCheck(root->getAttributeBool("subindex_showtexts"));

	String path = utils::makeFilePath(utils::makeFolderPath(g_componentsPath, _S("documentation")), _S("documentation_editor.xsl"));

	shared_ptr<XMLStylesheet> stylesheet(OS_NEW XMLStylesheet());
	// Inizializza le funzioni del template
	getPage()->initStylesheet(stylesheet);
	// Carica l'xsl
	if(stylesheet->parseFile(path))
	{
		// URGENT devo per forza creare un documento?
		// Se il secondo parametro della XSLControl  nullptr non crea il controllo,
		// e non posso passare una "getDocument()" perch se  una nuova istanza  nullptr pure lui...
		shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
		shared_ptr<XMLNode> root = document->create("documentation");

		shared_ptr<HtmlXSLControl> templ(OS_NEW HtmlXSLControl(stylesheet, document));
		templ->addChildParam(m_tocLayout);
		templ->addChildParam(m_tocMinTexts);
		templ->addChildParam(m_contentLevel);
		templ->addChildParam(m_subindexLevel);
		templ->addChildParam(m_subindexShowTexts);		
		getControls()->add(templ);
	}	
}

//////////////////////////////////////////////////////////////////////

} // documentation
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
