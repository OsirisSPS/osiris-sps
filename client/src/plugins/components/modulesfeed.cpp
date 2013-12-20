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
#include "modulesfeed.h"

#include "modulesfeededitor.h"
#include "modulesfeedviewer.h"
#include "modulesmodulesconfig.h"
#include "xmlattributes.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

Feed::Feed()
{

}

Feed::~Feed()
{

}

// Verifica che il documento esista, e sia conforme (cioè ad esempio ne fa un'upgrade se è una vecchia versione).
void Feed::initXML(shared_ptr<XMLDocument> document)
{
	shared_ptr<XMLNode> root = document->getRoot();
	if(root == nullptr)
		root = document->create(OS_MODULES_FEED_ROOT);

	if(root->getAttributes()->exists(OS_MODULES_FEED_URL) == false)
		root->setAttributeString(OS_MODULES_FEED_URL, _S("http://www.osiris-sps.org/permalink.php?rss_news"));
	if(root->getAttributes()->exists(OS_MODULES_FEED_SHOWTITLE) == false)
		root->setAttributeBool(OS_MODULES_FEED_SHOWTITLE, true);
	if(root->getAttributes()->exists(OS_MODULES_FEED_SHOWDESCRIPTION) == false)
		root->setAttributeBool(OS_MODULES_FEED_SHOWDESCRIPTION, true);
	if(root->getAttributes()->exists(OS_MODULES_FEED_SHOWIMAGE) == false)
		root->setAttributeBool(OS_MODULES_FEED_SHOWIMAGE, true);
	if(root->getAttributes()->exists(OS_MODULES_FEED_NITEMS) == false)
		root->setAttributeUint32(OS_MODULES_FEED_NITEMS, 10);
	if(root->getAttributes()->exists(OS_MODULES_FEED_SHOWITEMDESCRIPTION) == false)
		root->setAttributeString(OS_MODULES_FEED_SHOWITEMDESCRIPTION, _S("popup"));
	if(root->getAttributes()->exists(OS_MODULES_FEED_TEMPLATEMODEL) == false)
		root->setAttributeString(OS_MODULES_FEED_TEMPLATEMODEL, _S("links"));
	if(root->getAttributes()->exists(OS_MODULES_FEED_TEMPLATECUSTOM) == false)
		root->setAttributeString(OS_MODULES_FEED_TEMPLATECUSTOM, String::EMPTY);
}

ExtensionsModuleID Feed::getID()
{
	return ExtensionsModuleID(OS_MODULES_FEED_ID.to_ascii());
}

String Feed::getName()
{
	return _S("Feed module");
}

shared_ptr<ExtensionsModuleEditor> Feed::createEditor()
{
	return shared_ptr<ExtensionsModuleEditor>(OS_NEW FeedEditor());
}

shared_ptr<ExtensionsModuleViewer> Feed::createViewer()
{
	return shared_ptr<ExtensionsModuleViewer>(OS_NEW FeedViewer());
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
