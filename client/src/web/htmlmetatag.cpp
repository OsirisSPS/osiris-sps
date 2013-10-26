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
#include "htmlmetatag.h"

#include "htmlwriter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlMetaTag::HtmlMetaTag()
{

}

HtmlMetaTag::~HtmlMetaTag()
{

}

void HtmlMetaTag::render(HtmlWriter &writer)
{
	writer.openTag(_S("meta"));

	if(m_name.empty() == false)
        writer.writeAttribute(_S("name"), m_name);
	if(m_httpEquiv.empty() == false)
		writer.writeAttribute(_S("http-equiv"), m_httpEquiv);
	if(m_content.empty() == false)
        writer.writeAttribute(_S("content"), m_content);
	if(m_lang.empty() == false)
        writer.writeAttribute(_S("lang"), m_lang);
	if(m_url.empty() == false)
        writer.writeAttribute(_S("url"), m_url);

	//writer.closeTag();
	writer.selfCloseTag(); // 0.13 RC2
}

shared_ptr<HtmlMetaTag> HtmlMetaTag::createByName(const String &name, const String &content)
{
	shared_ptr<HtmlMetaTag> metaTag(OS_NEW HtmlMetaTag());
	metaTag->setName(name);
	metaTag->setContent(content);
	return metaTag;
}

shared_ptr<HtmlMetaTag> HtmlMetaTag::createByHttpEquiv(const String &httpEquiv, const String &content)
{
	shared_ptr<HtmlMetaTag> metaTag(OS_NEW HtmlMetaTag());
	metaTag->setHttpEquiv(httpEquiv);
	metaTag->setContent(content);
	return metaTag;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
