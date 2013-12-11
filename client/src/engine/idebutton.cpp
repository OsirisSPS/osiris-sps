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
#include "idebutton.h"

#include "eventssource.h"
#include "ideskin.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "htmlwriter.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IdeButton::EVENT_ONCLICK = _S("onClick");

//////////////////////////////////////////////////////////////////////

IdeButton::IdeButton(const String &caption) :	m_isDefault(false),
												m_caption(caption)
{
	
}

IdeButton::~IdeButton()
{

}

shared_ptr<EventSource> IdeButton::getEventClick() const
{
	return getEvents()->get(EVENT_ONCLICK);
}

String IdeButton::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("button.xsl"));
}

void IdeButton::onInit()
{
	ControlBase::onInit();

	shared_ptr<XMLDocument> document = getDocument();
	shared_ptr<XMLNode> root = document->create(_S("button"));

	if(getIsDefault())
		root->setAttributeBool(_S("isDefault"), true);
	if(hasID())
		root->setAttributeString(_S("id"), getTarget());
	root->setAttributeString(_S("icon_href"), m_iconHref);
	root->setAttributeString(_S("caption"), m_caption);
	root->setAttributeString(_S("href"), m_href.empty() ? getEventCommand(EVENT_ONCLICK) : m_href);
	root->setAttributeString(_S("waitMessage"), m_waitMessage);
	root->setAttributeString(_S("confirmMessage"), m_confirmMessage);		
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
