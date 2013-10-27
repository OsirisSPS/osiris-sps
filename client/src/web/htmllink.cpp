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
#include "htmllink.h"

#include "htmlcontrols.h"
#include "htmlparser.h"
#include "htmltext.h"
#include "htmlwriter.h"
#include "httpparser.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlLink::HtmlLink(const String &caption, const String &href) : m_caption(OS_NEW HtmlText(caption)),
															    m_href(href)
{
		
}

HtmlLink::~HtmlLink()
{

}

const String & HtmlLink::getCaption() const 
{
	return m_caption->getText(); 
}

void HtmlLink::setCaption(const String &caption) 
{ 
	m_caption->setText(caption); 
}

void HtmlLink::renderAttributes(HtmlWriter &writer)
{
	ControlBase::renderAttributes(writer);

	if(m_href.empty() == false)
		//writer.writeAttribute(_S("href"), HttpParser::urlEncode(m_href));
		writer.writeAttribute(_S("href"), m_href);
}

void HtmlLink::onInit()
{
	ControlBase::onInit();

	getControls()->add(m_caption);
}

void HtmlLink::onRender(HtmlWriter &writer)
{
	writer.openTag(_S("a"));
	renderAttributes(writer);
	writer.closeTag();
	writer.writeLine();
	renderChilds(writer);
	writer.closeTag(_S("a"));
	writer.writeLine();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
