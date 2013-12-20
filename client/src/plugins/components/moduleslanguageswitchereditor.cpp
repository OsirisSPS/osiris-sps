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
#include "moduleslanguageswitchereditor.h"

#include "convert.h"
#include "conversions.h"
#include "ideomleditor.h"
#include "moduleslanguageswitcher.h"
#include "modulesmodulesconfig.h"

#include "htmlcontrols.h"
#include "htmlcheckbox.h"
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

LanguageSwitcherEditor::LanguageSwitcherEditor() : m_style(OS_NEW HtmlComboBox()),
												   m_cultures(OS_NEW HtmlTextBox())
{
	m_style->setID(OS_MODULES_LANGUAGESWITCHER_STYLE);
	m_cultures->setID(OS_MODULES_LANGUAGESWITCHER_CULTURES);
}

LanguageSwitcherEditor::~LanguageSwitcherEditor()
{

}

bool LanguageSwitcherEditor::createDocument()
{
	if(EditorBase::createDocument() == false)
		return false;

	String style = m_style->getValue();
	String cultures = m_cultures->getValue();
	
	shared_ptr<XMLNode> root = getModuleDocument()->create(OS_MODULES_LANGUAGESWITCHER_ROOT);
	root->setAttributeString(OS_MODULES_LANGUAGESWITCHER_STYLE, style);	
	root->setAttributeString(OS_MODULES_LANGUAGESWITCHER_CULTURES, cultures);	

	return true;
}

void LanguageSwitcherEditor::onInit()
{
	EditorBase::onInit();
	
	m_style->addOption(_S("base"),_S("base"));
	m_style->addOption(_S("full"),_S("full"));		
}

void LanguageSwitcherEditor::onLoad()
{
	EditorBase::onLoad();

	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	m_style->setValue(root->getAttributeString(OS_MODULES_LANGUAGESWITCHER_STYLE));
	m_cultures->setValue(root->getAttributeString(OS_MODULES_LANGUAGESWITCHER_CULTURES));

	String path = utils::makeFilePath(utils::makeFolderPath(g_componentsPath, _S("languageswitcher")), _S("languageswitcher_editor.xsl"));

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
		shared_ptr<XMLNode> root = document->create(OS_MODULES_LANGUAGESWITCHER_ROOT);

		shared_ptr<HtmlXSLControl> templ(OS_NEW HtmlXSLControl(stylesheet, document));
		templ->addChildParam(m_style);
		templ->addChildParam(m_cultures);
		getControls()->add(templ);
	}
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
