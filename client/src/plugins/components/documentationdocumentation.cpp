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
#include "documentationdocumentation.h"

#include "documentationdocumentationconfig.h"
#include "documentationdocumentationviewer.h"
#include "documentationdocumentationeditor.h"
#include "entitiesentity.h"
#include "xmlattributes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace documentation {

//////////////////////////////////////////////////////////////////////

Documentation::Documentation()
{

}

Documentation::~Documentation()
{

}

// Verifica che il documento esista, e sia conforme (cioè ad esempio ne fa un'upgrade se è una vecchia versione).
void Documentation::initXML(shared_ptr<XMLDocument> document)
{
	shared_ptr<XMLNode> root = document->getRoot();
	if(root == null)
		root = document->create("documentation");

	if(root->getAttributes()->exists("toc_layout") == false)	
		root->setAttributeString("toc_layout", "right");		
	if(root->getAttributes()->exists("toc_mintexts") == false)	
		root->setAttributeInt32("toc_mintexts", 3);		
	if(root->getAttributes()->exists("content_level") == false)	
		root->setAttributeInt32("content_level", 1);		
	if(root->getAttributes()->exists("subindex_level") == false)	
		root->setAttributeInt32("subindex_level", -1);		
	if(root->getAttributes()->exists("subindex_showtexts") == false)	
		root->setAttributeBool("subindex_showtexts", true);		
}

ExtensionsComponentID Documentation::getID()
{
	return ExtensionsComponentID(OS_COMDOCUMENTATION_DOCUMENTATION_ID.to_ascii());
}

String Documentation::getName()
{
	return _S("Documentation");
}

shared_ptr<ExtensionsComponentEditor> Documentation::createEditor(shared_ptr<EntitiesEntity> entity)
{
	return shared_ptr<ExtensionsComponentEditor>(OS_NEW DocumentationEditor());
}

shared_ptr<ExtensionsComponentViewer> Documentation::createViewer(shared_ptr<EntitiesEntity> entity)
{
	if(entity != null && entity->getObjectType() != portalObjectTypeSection)
		return null;	// Il componente renderizza solo se stesso o un oggetto di tipo sezione

	return shared_ptr<ExtensionsComponentViewer>(OS_NEW DocumentationViewer(entity));
}

//////////////////////////////////////////////////////////////////////

} // documentation
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
