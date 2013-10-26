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
#include "htmlparagraph.h"

#include "htmlwriter.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlParagraph::HtmlParagraph(const String &caption) : m_caption(OS_NEW HtmlLiteral(caption))
{

}

HtmlParagraph::~HtmlParagraph()
{

}

const String & HtmlParagraph::getCaption() const
{
	return m_caption->getHtml();
}

void HtmlParagraph::setCaption(const String &caption)
{
	m_caption->setHtml(caption);
}

void HtmlParagraph::onInit()
{
	ControlBase::onInit();

	getControls()->add(m_caption);
}

void HtmlParagraph::onRender(HtmlWriter &writer)
{
	writer.openTag(_S("p"));
	renderAttributes(writer);
	writer.closeTag();
	writer.writeLine();
	renderChilds(writer);
	writer.closeTag(_S("p"));
	writer.writeLine();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
