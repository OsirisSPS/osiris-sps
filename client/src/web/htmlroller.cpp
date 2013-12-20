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
#include "htmlroller.h"

#include "conversions.h"
#include "css.h"
#include "htmlattributes.h"
#include "htmlcontrols.h"
#include "htmldiv.h"
#include "htmlpage.h"
#include "htmlscript.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlRoller::HtmlRoller()
{
	m_allowClose = true;
	m_startClose = false;
	m_cssTable = OS_CSS_ROLLER_TABLE;
	m_cssHeaderOpen = OS_CSS_ROLLER_HEADEROPEN;
	m_cssHeaderOpenHover = OS_CSS_ROLLER_HEADEROPENOVER;
	m_cssHeaderClose = OS_CSS_ROLLER_HEADERCLOSE;
	m_cssHeaderCloseHover = OS_CSS_ROLLER_HEADERCLOSEHOVER;
}

HtmlRoller::~HtmlRoller()
{

}

shared_ptr<HtmlDiv> HtmlRoller::getHeader()
{
	if(m_header == nullptr)
	{
		m_header.reset(OS_NEW HtmlDiv());
		m_header->getAttributes()->set(_S("data-os-roller"), _S("header"));

		getControls()->push_front(m_header);
	}

	return m_header;
}

shared_ptr<HtmlDiv> HtmlRoller::getBody()
{
	if(m_body == nullptr)
	{
		m_body.reset(OS_NEW HtmlDiv());
		m_body->setCss(OS_CSS_ROLLER_BODY);
		m_body->getAttributes()->set(_S("data-os-roller"), _S("body"));

		getControls()->push_back(m_body);
	}

	return m_body;
}

void HtmlRoller::onPreRender()
{
	ControlBase::onPreRender();

	getAttributes()->set(_S("data-os-otype"), _S("popup"));
	getAttributes()->set(_S("data-os-allowClose"), conversions::to_utf16(m_allowClose));
	getAttributes()->set(_S("data-os-startClose"), conversions::to_utf16(m_startClose));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
