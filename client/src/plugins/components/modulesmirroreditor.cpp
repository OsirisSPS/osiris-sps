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
#include "modulesmirroreditor.h"

#include "conversions.h"
#include "htmlcombobox.h"
#include "ideomleditor.h"
#include "modulesmirror.h"
#include "modulesmodulesconfig.h"

#include "htmlcontrols.h"
#include "htmltextbox.h"
#include "xmldocument.h"
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

MirrorEditor::MirrorEditor() : m_objectID(OS_NEW HtmlTextBox()),
							   m_renderMode(OS_NEW HtmlComboBox())
{
	m_objectID->setID("objectID");
	m_objectID->setSize(50);

	m_renderMode->setID("renderMode");	
}

MirrorEditor::~MirrorEditor()
{

}

bool MirrorEditor::createDocument()
{
	if(EditorBase::createDocument() == false)
		return false;

	EntityID id(m_objectID->getValue().to_ascii());
	/*
	if(Mirror::isValidMirrorObject(id) == false)
	{
		getPage()->showError(_S("Invalid object id"));
		return false;
	}
	*/

	shared_ptr<XMLNode> root = getModuleDocument()->create(OS_MODULES_MIRROR_ROOT);
	root->setAttributeString(OS_MODULES_MIRROR_OBJECT_ID, id.toUTF16());
	Mirror::setRenderMode(root, conversions::from_utf16<uint32>(m_renderMode->getValue()));

	return true;
}

void MirrorEditor::onInit()
{
	EditorBase::onInit();

	std::string renderModeDefault = conversions::to_string<uint32>(OS_MODULES_MIRROR_RENDER_MODE_DEFAULT);
	std::string renderModeContent = conversions::to_string<uint32>(OS_MODULES_MIRROR_RENDER_MODE_CONTENT);

	m_renderMode->addOption(getPage()->getText(OS_MODULES_MIRROR_ID + ".render_mode." + renderModeDefault), renderModeDefault);
	m_renderMode->addOption(getPage()->getText(OS_MODULES_MIRROR_ID + ".render_mode." + renderModeContent), renderModeContent);
}

void MirrorEditor::onLoad()
{
	EditorBase::onLoad();

	if(getPostBack() == false)
	{
		shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
		if(root != null)
		{
			m_objectID->setValue(root->getAttributeString(OS_MODULES_MIRROR_OBJECT_ID));
			m_renderMode->setValue(conversions::to_utf16(Mirror::getRenderMode(root)));
		}
	}

	String path = utils::makeFilePath(utils::makeFolderPath(g_componentsPath, _S("mirror")), _S("mirror_editor.xsl"));

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
		shared_ptr<XMLNode> root = document->create(OS_MODULES_MIRROR_ROOT);

		shared_ptr<HtmlXSLControl> templ(OS_NEW HtmlXSLControl(stylesheet, document));
		templ->addChildParam(m_objectID);
		templ->addChildParam(m_renderMode);
		getControls()->add(templ);
	}
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
