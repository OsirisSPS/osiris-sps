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
#include "modulesxslttransformviewer.h"

#include "ideviewiobjectviewer.h"
#include "modulesxslttransform.h"
#include "modulesmodulesconfig.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "idesession.h"
#include "htmlliteral.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlschema.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

XsltTransformViewer::XsltTransformViewer()
{

}

XsltTransformViewer::~XsltTransformViewer()
{

}

void XsltTransformViewer::onLoad()
{
	ViewerBase::onLoad();

	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	if(root != null)
	{
		String xmlStr = root->getAttributeString(OS_MODULES_XSLTTRANSFORM_XML);
		String xslStr = root->getAttributeString(OS_MODULES_XSLTTRANSFORM_XSL);
		String xsdStr = root->getAttributeString(OS_MODULES_XSLTTRANSFORM_XSD);

		shared_ptr<XMLDocument> document;

		if(xsdStr.empty())
		{
			document.reset(OS_NEW XMLDocument());
		}
		else
		{
			shared_ptr<XMLSchema> xsd(OS_NEW XMLSchema());
			xsd->parseString(xsdStr);

			document.reset(OS_NEW XMLDocument(xsd));
		}

		if(document->parseString(xmlStr))
		{
			shared_ptr<XMLStylesheet> transformer(OS_NEW XMLStylesheet());
			// Inizializza le funzioni del template
			getPage()->initStylesheet(transformer);

			if(transformer->parseString(xslStr))
			{
				String output;
				if(transformer->applyToString(document, output))
				{
					getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(getPage()->parseOml(output, true, false, false, omlRenderModeOsiris, getInstance().getString()))));
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
