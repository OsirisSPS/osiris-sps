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
#include "modulessearch.h"

#include "modulessearcheditor.h"
#include "modulessearchviewer.h"
#include "modulesmodulesconfig.h"
#include "xmlattributes.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

Search::Search()
{

}

Search::~Search()
{

}

// Verifica che il documento esista, e sia conforme (cioè ad esempio ne fa un'upgrade se è una vecchia versione).
void Search::initXML(shared_ptr<XMLDocument> document)
{
	shared_ptr<XMLNode> root = document->getRoot();
	if(root == nullptr)
		root = document->create(OS_MODULES_SEARCH_ROOT);

	if(root->getAttributes()->exists(_S("showParams")) == false)
		root->setAttributeBool(_S("showParams"), false);	
	if(root->getAttributes()->exists(_S("directRun")) == false)
		root->setAttributeBool(_S("directRun"), true);	
	if(root->getAttributes()->exists(_S("allowRss")) == false)
		root->setAttributeBool(_S("allowRss"), true);	
	if(root->getAttributes()->exists(_S("rssBody")) == false)
		root->setAttributeString(_S("rssBody"), "full");	
}

ExtensionsModuleID Search::getID()
{
	return ExtensionsModuleID(OS_MODULES_SEARCH_ID.to_ascii());
}

String Search::getName()
{
	return _S("Search module");
}

shared_ptr<ExtensionsModuleEditor> Search::createEditor()
{
	return shared_ptr<ExtensionsModuleEditor>(OS_NEW SearchEditor());
}

shared_ptr<ExtensionsModuleViewer> Search::createViewer()
{
	return shared_ptr<ExtensionsModuleViewer>(OS_NEW SearchViewer());
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
