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
#include "htmltextarea.h"

#include "conversions.h"
#include "htmlattributes.h"
#include "htmlparser.h"
#include "htmlwriter.h"
#include "htmlunit.h"
#include "httprequest.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlTextArea::HtmlTextArea()
{

}

HtmlTextArea::~HtmlTextArea()
{

}

uint32 HtmlTextArea::getColumns() const
{
	return conversions::from_utf16<uint32>(getAttributes()->value_of(_S("cols")));
}

void HtmlTextArea::setColumns(const uint32 &value)
{
	getAttributes()->set(_S("cols"), conversions::to_utf16(value));
}

uint32 HtmlTextArea::getRows() const
{
	return conversions::from_utf16<uint32>(getAttributes()->value_of(_S("rows")));
}

void HtmlTextArea::setRows(const uint32 &value)
{
	getAttributes()->set(_S("rows"), conversions::to_utf16(value));
}

void HtmlTextArea::renderAttributes(HtmlWriter &writer)
{
	ControlBase::renderAttributes(writer);

	if(m_id.empty() == false)
		writer.writeAttribute(_S("name"), getTarget());
}

void HtmlTextArea::onInit()
{
	ControlBase::onInit();

	if(getPostBack() && hasID())
		m_text = getRequest()->getPostParamString(getTarget().to_wide());
}

void HtmlTextArea::onRender(HtmlWriter &writer)
{
	writer.openTag(_S("textarea"));
	renderAttributes(writer);
	writer.closeTag();
	writer.add(HtmlParser::instance()->encode(m_text));
	writer.closeTag(_S("textarea"), false);
	writer.writeLine();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
