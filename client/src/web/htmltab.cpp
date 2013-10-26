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
#include "htmltab.h"

#include "css.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmldiv.h"
#include "htmlpage.h"
#include "htmlscript.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlTab::HtmlTab()
{
	m_type = ttTop;
	m_cssTable = OS_CSS_TAB_TABLE;
	m_cssLink = OS_CSS_TAB_LINK;
	m_cssLinkHover = OS_CSS_TAB_LINKHOVER;
}

HtmlTab::~HtmlTab()
{

}

void HtmlTab::addPage(shared_ptr<IHtmlControl> page, const String &title)
{
	if(page == null)
		return;

	if(page->hasCss() == false)
		page->setCss(OS_CSS_TAB_PAGE);

	page->getAttributes()->set(_S("pageTitle"), title);
	getControls()->add(page);
}

void HtmlTab::onPreRender()
{
	ControlBase::onPreRender();

	String type;
	switch(getType())
	{
	case ttLeft:	type = _S("left");
					break;

	case ttTop:		type = _S("top");
					break;

	case ttRight:	type = _S("right");
					break;

	case ttBottom:	type = _S("bottom");
					break;
	}

	shared_ptr<IHtmlControl> script(OS_NEW HtmlScript(String::format(_S("osTab_Create('%S','%S','%S','%S','%S');").c_str(), getTarget().c_str(), type.c_str(), m_cssTable.c_str(), m_cssLink.c_str(), m_cssLinkHover.c_str())));
	getControls()->add(script);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
