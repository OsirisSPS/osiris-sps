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
#include "omlanchor.h"

#include "httpparser.h"
#include "omlcontext.h"
#include "omlitem.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLAnchor::OMLAnchor(const String& tag) : IOMLCode(tag)
{	
}

OMLAnchor::~OMLAnchor()
{

}

String OMLAnchor::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	mapDefaultParamTo(i, _S("name"));

	String body = i->getHtmlChilds(context);
	String anchor = i->getParam(_S("name"));
	if( (anchor.empty()) && (!body.empty()) )
	{
		anchor = body;
		body = String::EMPTY;
	}	
	
	if(context->getMode() == omlRenderModeSearch)
	{
		return _S(" ") + anchor + _S(" ") + body + _S(" ");
	}
	else
	{
		anchor += _S("_") + context->getContextID();
		anchor = HttpParser::urlEncodeW(anchor.to_wide());
		String css = OS_CSS_OML_ANCHOR;
		//return String::format(_S("<a id=\"%S\" name=\"%S\">%S</a>").c_str(), anchor.c_str(), anchor.c_str(), body.c_str());
		return String::format(_S("<div id=\"%S\">%S</div>").c_str(), anchor.c_str(), body.c_str());
	}	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
