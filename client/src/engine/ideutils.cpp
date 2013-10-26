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
#include "ideutils.h"

#include "convert.h"
#include "iidepage.h"
#include "filesystem.h"
#include "languagefolder.h"
#include "languagemanager.h"
#include "log.h"
#include "options.h"
#include "css.h"
#include "htmlparser.h"
#include "htmlcontrols.h"
#include "htmlpage.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltablerow.h"
#include "htmltext.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {

//////////////////////////////////////////////////////////////////////

void Utils::addInputRow(shared_ptr<HtmlTable> table, shared_ptr<IPage> page, shared_ptr<IHtmlControl> ctrl, const String &key)
{
	shared_ptr<HtmlTableRow> row = table->addRow();

	shared_ptr<HtmlTableCell> left = row->addCell();
	left->setCss(OS_CSS_FORM_LEFT);
	left->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(page->getText(key))));

	shared_ptr<HtmlTableCell> right = row->addCell();
	right->setCss(OS_CSS_FORM_RIGHT);
	right->getControls()->add(ctrl);
}

shared_ptr<XMLNode> Utils::addAction(shared_ptr<XMLNode> nodeActions, shared_ptr<IPage> page, const String &suffix, const String &actionName, const String &actionHref, bool selected, bool confirm)
{
	shared_ptr<XMLNode> nodeAction = nodeActions->addChild(_S("action"));
	nodeAction->setAttributeString(_S("name"), actionName);
	nodeAction->setAttributeString(_S("href"), actionHref);
	nodeAction->setAttributeBool(_S("selected"), selected);
	if(confirm)
		nodeAction->setAttributeBool(_S("confirm"), confirm);	

	return nodeAction;
}

/*
shared_ptr<XMLNode> Utils::addAction(shared_ptr<XMLNode> nodeActions, shared_ptr<IPage> page, const String &suffix, const String &actionName, const String &actionHref, shared_ptr<XMLNode> actionContent, bool confirm)
{
	shared_ptr<XMLNode> nodeAction = addAction(nodeActions, page, suffix, actionName, actionHref, confirm);

	shared_ptr<XMLNode> nodeContent = nodeAction->addChild(_S("content"));
	nodeContent->addChild(actionContent);

	return nodeAction;
}
*/

String Utils::generateConfirmHref(shared_ptr<IPage> page, const String &href, const String &confirm)
{
	// URGENT, da verificare, credo non usata da nessuno
	String title = page->getText(confirm + _S(".title"));
	String description = page->getText(confirm + _S(".description"));

	// Presuppongo che i title & description siano già epurati/encodati da javascript-injection e altri caratteri particolari.
	String out = String::format(_S("osLinkConfirm('%S','%S','%S'").c_str(), title.c_str(), description.c_str(), href.c_str());
	return _S("javascript:void(") + out + _S(");");
}

//////////////////////////////////////////////////////////////////////

} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
