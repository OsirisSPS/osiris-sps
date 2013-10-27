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
#include "omltab.h"

#include "omlcontext.h"
#include "omlitem.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLTab::OMLTab(const String& tag) : IOMLCode(tag)
{	
}

OMLTab::~OMLTab()
{

}

String OMLTab::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	/*
	if(context->checkRowModeNotAllowed(i))
		return String::EMPTY;
	
	String childsHtml = i->getHtmlChilds(context);

	String tooltipHtml = encodeOML(context, i->getParam());

	if(context->getMode() == _S("plain"))
		return childsHtml + _S(" ") + tooltipHtml;
	else
		return String::format(_S("<div data-os-otype=\"popup\" data-os-popupMode=\"tooltip\"><div data-os-popup=\"header\">%S</div><div data-os-popup=\"body\">%S</div></div>").c_str(), childsHtml.c_str(), tooltipHtml.c_str());
	*/
	return String::EMPTY;
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
