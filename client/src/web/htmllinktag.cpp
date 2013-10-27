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
#include "htmllinktag.h"

#include "htmlwriter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlLinkTag::HtmlLinkTag()
{

}

HtmlLinkTag::~HtmlLinkTag()
{

}

void HtmlLinkTag::render(HtmlWriter &writer)
{
	writer.openTag(_S("link"));

	if(m_rel.empty() == false)
        writer.writeAttribute(_S("rel"), m_rel);
	if(m_href.empty() == false)
		writer.writeAttribute(_S("href"), m_href);
	if(m_type.empty() == false)
        writer.writeAttribute(_S("type"), m_type);
	if(m_title.empty() == false)
        writer.writeAttribute(_S("title"), m_title);
	if(m_hreflang.empty() == false)
        writer.writeAttribute(_S("hreflang"), m_hreflang);

	//writer.closeTag();
	writer.selfCloseTag(); // 0.13 RC2
}

shared_ptr<HtmlLinkTag> HtmlLinkTag::create(const String &rel, const String &href)
{
	shared_ptr<HtmlLinkTag> linkTag(OS_NEW HtmlLinkTag());
	linkTag->setRel(rel);
	linkTag->setHref(href);
	return linkTag;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
