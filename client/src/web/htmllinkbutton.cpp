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
#include "htmllinkbutton.h"

#include "eventssource.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"
#include "htmlwriter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String HtmlLinkButton::EVENT_ONCLICK = _S("onClick");

//////////////////////////////////////////////////////////////////////

HtmlLinkButton::HtmlLinkButton(const String &caption) : m_caption(OS_NEW HtmlLiteral(caption))
{

}

HtmlLinkButton::~HtmlLinkButton()
{

}

const String & HtmlLinkButton::getCaption() const 
{
	return m_caption->getHtml(); 
}

void HtmlLinkButton::setCaption(const String &caption) 
{ 
	m_caption->setHtml(caption); 
}

void HtmlLinkButton::renderAttributes(HtmlWriter &writer)
{
	ControlBase::renderAttributes(writer);

	if(hasID())
		writer.writeAttribute(_S("href"), getEventCommand(EVENT_ONCLICK));
}

shared_ptr<EventSource> HtmlLinkButton::getEventClick() const
{
	return getEvents()->get(EVENT_ONCLICK);
}

void HtmlLinkButton::onInit()
{
	ControlBase::onInit();

	getControls()->add(m_caption);
}

void HtmlLinkButton::onRender(HtmlWriter &writer)
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
