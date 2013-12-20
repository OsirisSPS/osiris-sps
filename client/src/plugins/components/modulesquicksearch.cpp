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
#include "modulesquicksearch.h"

#include "modulesquicksearcheditor.h"
#include "modulesquicksearchviewer.h"
#include "modulesmodulesconfig.h"
#include "xmlattributes.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

QuickSearch::QuickSearch()
{

}

QuickSearch::~QuickSearch()
{

}

// Verifica che il documento esista, e sia conforme (cioè ad esempio ne fa un'upgrade se è una vecchia versione).
void QuickSearch::initXML(shared_ptr<XMLDocument> document)
{
	shared_ptr<XMLNode> root = document->getRoot();
	if(root == nullptr)
		root = document->create(OS_MODULES_LANGUAGESWITCHER_ROOT);

	if(root->getAttributes()->exists(OS_MODULES_LANGUAGESWITCHER_STYLE) == false)
		root->setAttributeString(OS_MODULES_LANGUAGESWITCHER_STYLE, _S("base"));	
}

ExtensionsModuleID QuickSearch::getID()
{
	return ExtensionsModuleID(OS_MODULES_QUICKSEARCH_ID.to_ascii());
}

String QuickSearch::getName()
{
	return _S("Quick search");
}

shared_ptr<ExtensionsModuleEditor> QuickSearch::createEditor()
{
	return shared_ptr<ExtensionsModuleEditor>(OS_NEW QuickSearchEditor());
}

shared_ptr<ExtensionsModuleViewer> QuickSearch::createViewer()
{
	return shared_ptr<ExtensionsModuleViewer>(OS_NEW QuickSearchViewer());
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
