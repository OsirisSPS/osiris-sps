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
#include "htmlpopup.h"

#include "htmlattributes.h"
#include "css.h"
#include "htmlcontrols.h"
#include "htmlpage.h"
#include "htmlscript.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlPopup::HtmlPopup() : m_type(ptPopup)
{

}

HtmlPopup::~HtmlPopup()
{

}

shared_ptr<HtmlDiv> HtmlPopup::getHeader()
{
	if(m_header == null)
	{
		m_header.reset(OS_NEW HtmlDiv());
		m_header->setCss(OS_CSS_POPUP_HEADER);
		m_header->getAttributes()->set(_S("data-os-popup"), _S("header"));

		getControls()->push_front(m_header);
	}

	return m_header;
}

shared_ptr<HtmlDiv> HtmlPopup::getBody()
{
	if(m_body == null)
	{
		m_body.reset(OS_NEW HtmlDiv());
		m_body->setCss(OS_CSS_POPUP_BODY);
		m_body->setStyle(_S("display:none"));
		m_body->getAttributes()->set(_S("data-os-popup"), _S("body"));

		getControls()->push_back(m_body);
	}

	return m_body;
}

void HtmlPopup::onPreRender()
{
	ControlBase::onPreRender();

	String type;
	switch(getType())
	{
	case ptStatic:		type = _S("static"); 
						break;

	case ptStaticClick:	type = _S("staticclick"); 
						break;

	case ptTooltip:		type = _S("tooltip"); 
						break;

	case ptPopup:		type = _S("popup"); 
						break;
	}

	getAttributes()->set(_S("data-os-otype"), _S("popup"));
	getAttributes()->set(_S("data-os-popupMode"), type);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
