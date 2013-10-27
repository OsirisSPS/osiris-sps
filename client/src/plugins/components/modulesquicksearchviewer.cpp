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
#include "modulesquicksearchviewer.h"

#include "htmlevent.h"
#include "htmlliteral.h"
#include "idepages.h"
#include "ideviewiobjectviewer.h"
#include "languageculture.h"
#include "languagefolder.h"
#include "languagemanager.h"
#include "log.h"
#include "modulesquicksearch.h"
#include "modulesmodulesconfig.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "platformmanager.h"
#include "portalsaccount.h"
#include "idesession.h"
#include "portalsportal.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlschema.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

extern OS_NAMESPACE_NAME::String g_componentsPath;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {

//////////////////////////////////////////////////////////////////////

namespace modules {

//////////////////////////////////////////////////////////////////////

QuickSearchViewer::QuickSearchViewer()
{

}

QuickSearchViewer::~QuickSearchViewer()
{

}

void QuickSearchViewer::onLoad()
{
	ViewerBase::onLoad();

	String controlID = getInstance().toUTF16();
	if(controlID.empty())
		controlID = _S("preview"); // Se il modulo è in preview, non ha un'ID di istanza. Ma un ID mi serve x forza per gli eventi.
	setID(controlID);

	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	OS_ASSERT(root != null);
		
	if(root != null)
	{
		String style = root->getAttributeString("style");

		String search_href = getPage()->getPortal()->getLink(OS_IDE_PAGE_SEARCH);
		
		shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
		shared_ptr<XMLStylesheet> stylesheet(OS_NEW XMLStylesheet());
		// Inizializza le funzioni del template
		getPage()->initStylesheet(stylesheet);

		String path = utils::makeFilePath(utils::makeFolderPath(g_componentsPath, _S("quicksearch")), _S("quicksearch_viewer.xsl"));

		// Carica l'xsl
		if(stylesheet->parseFile(path))
		{
			// URGENT devo per forza creare un documento?
			// Se il secondo parametro della XSLControl  null non crea il controllo,
			// e non posso passare una "getDocument()" perch se  una nuova istanza  null pure lui...
			shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
			document->create(OS_MODULES_QUICKSEARCH_ROOT);

			shared_ptr<XMLNode> root = document->getRoot();
			root->setAttributeString(OS_MODULES_QUICKSEARCH_SEARCHHREF, search_href);

			root->setAttributeString("style", style);

			shared_ptr<HtmlXSLControl> templ(OS_NEW HtmlXSLControl(stylesheet, document));			
			getControls()->add(templ);
		}
		else
		{
			// Template non valido.
		}			
	}	
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
