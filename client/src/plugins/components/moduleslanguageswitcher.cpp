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
#include "moduleslanguageswitcher.h"

#include "moduleslanguageswitchereditor.h"
#include "moduleslanguageswitcherviewer.h"
#include "modulesmodulesconfig.h"
#include "xmlattributes.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

LanguageSwitcher::LanguageSwitcher()
{

}

LanguageSwitcher::~LanguageSwitcher()
{

}

// Verifica che il documento esista, e sia conforme (cioè ad esempio ne fa un'upgrade se è una vecchia versione).
void LanguageSwitcher::initXML(shared_ptr<XMLDocument> document)
{
	shared_ptr<XMLNode> root = document->getRoot();
	if(root == null)
		root = document->create(OS_MODULES_LANGUAGESWITCHER_ROOT);

	if(root->getAttributes()->exists(OS_MODULES_LANGUAGESWITCHER_STYLE) == false)
		root->setAttributeString(OS_MODULES_LANGUAGESWITCHER_STYLE, _S("base"));
	if(root->getAttributes()->exists(OS_MODULES_LANGUAGESWITCHER_CULTURES) == false)
		root->setAttributeString(OS_MODULES_LANGUAGESWITCHER_CULTURES, _S("it-IT;en-US"));
}

ExtensionsModuleID LanguageSwitcher::getID()
{
	return ExtensionsModuleID(OS_MODULES_LANGUAGESWITCHER_ID.to_ascii());
}

String LanguageSwitcher::getName()
{
	return _S("Language Switcher module");
}

shared_ptr<ExtensionsModuleEditor> LanguageSwitcher::createEditor()
{
	return shared_ptr<ExtensionsModuleEditor>(OS_NEW LanguageSwitcherEditor());
}

shared_ptr<ExtensionsModuleViewer> LanguageSwitcher::createViewer()
{
	return shared_ptr<ExtensionsModuleViewer>(OS_NEW LanguageSwitcherViewer());
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
