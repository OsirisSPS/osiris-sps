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
#include "htmlbutton.h"

#include "eventssource.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"
#include "htmlwriter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String HtmlButton::EVENT_ONCLICK = _S("onClick");

//////////////////////////////////////////////////////////////////////

HtmlButton::HtmlButton(const String &caption) : m_caption(OS_NEW HtmlLiteral(caption))
{
	
}

HtmlButton::~HtmlButton()
{

}

const String & HtmlButton::getCaption() const
{
	return m_caption->getHtml();
}

void HtmlButton::setCaption(const String &caption)
{
	m_caption->setHtml(caption);
}

shared_ptr<EventSource> HtmlButton::getEventClick() const
{
	return getEvents()->get(EVENT_ONCLICK);
}

void HtmlButton::renderAttributes(HtmlWriter &writer)
{
	ControlBase::renderAttributes(writer);

	if(hasID())
	{
		// Verifica che non ci sia una gestione adhoc dell'evento
		if(getAttributes()->exists(_S("onclick")) == false)
            writer.writeAttribute(_S("onclick"), getEventCommand(EVENT_ONCLICK));
	}
}

void HtmlButton::onInit()
{
	ControlBase::onInit();

	getControls()->add(m_caption);
}

void HtmlButton::onRender(HtmlWriter &writer)
{
	writer.openTag(_S("button"));
	renderAttributes(writer);
	writer.closeTag();
	writer.writeLine();
	renderChilds(writer);
	writer.closeTag(_S("button"));
	writer.writeLine();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
