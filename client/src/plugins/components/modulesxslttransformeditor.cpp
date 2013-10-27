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
#include "modulesxslttransformeditor.h"

#include "ideomleditor.h"
#include "modulesxslttransform.h"
#include "modulesmodulesconfig.h"

#include "htmlcontrols.h"
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

XsltTransformEditor::XsltTransformEditor() : m_xml(OS_NEW IdeOMLEditor()),
											 m_xsl(OS_NEW IdeOMLEditor()),
											 m_xsd(OS_NEW IdeOMLEditor())
{
	m_xml->setID(_S("xml"));
	m_xsl->setID(_S("xsl"));
	m_xsd->setID(_S("xsd"));
}

XsltTransformEditor::~XsltTransformEditor()
{

}

bool XsltTransformEditor::createDocument()
{
	if(EditorBase::createDocument() == false)
		return false;

	String xml = m_xml->getValue();
	String xsl = m_xsl->getValue();
	String xsd = m_xsd->getValue();

	shared_ptr<XMLNode> root = getModuleDocument()->create(OS_MODULES_XSLTTRANSFORM_ROOT);
	root->setAttributeString(OS_MODULES_XSLTTRANSFORM_XML, xml);
	root->setAttributeString(OS_MODULES_XSLTTRANSFORM_XSL, xsl);
	root->setAttributeString(OS_MODULES_XSLTTRANSFORM_XSD, xsd);

	return true;
}

void XsltTransformEditor::onLoad()
{
	EditorBase::onLoad();

	if(getPostBack() == false)
	{
		shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
		if(root != null)
		{
			m_xml->setValue(root->getAttributeString(OS_MODULES_XSLTTRANSFORM_XML));
			m_xsl->setValue(root->getAttributeString(OS_MODULES_XSLTTRANSFORM_XSL));
			m_xsd->setValue(root->getAttributeString(OS_MODULES_XSLTTRANSFORM_XSD));
		}
	}

	String path = utils::makeFilePath(utils::makeFolderPath(g_componentsPath, _S("xslttransform")), _S("xslttransform_editor.xsl"));

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
		document->create(OS_MODULES_XSLTTRANSFORM_ROOT);

		shared_ptr<HtmlXSLControl> templ(OS_NEW HtmlXSLControl(stylesheet, document));
		templ->addChildParam(m_xml,_S("xml"));
		templ->addChildParam(m_xsl,_S("xsl"));
		templ->addChildParam(m_xsd,_S("xsd"));
		getControls()->add(templ);
	}
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
