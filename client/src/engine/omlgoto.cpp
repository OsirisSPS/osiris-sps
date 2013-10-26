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
#include "omlgoto.h"

#include "httpparser.h"
#include "omlcontext.h"
#include "omlitem.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLGoto::OMLGoto(const String& tag) : IOMLCode(tag)
{	
}

OMLGoto::~OMLGoto()
{

}

String OMLGoto::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{		
	mapDefaultParamTo(i, _S("address"));

	String css = OS_CSS_OML_GOTO;
	String body = i->getHtmlChilds(context);
	String address = i->getParam(_S("address"));
	if(address.empty())
	{
		address = body;
	}

	//String url = _S("#") + HttpParser::urlEncode(address.to_ascii()) + _S("_") + context->getContextID();
	String url = address + _S("_") + context->getContextID();

	url = HttpParser::urlEncodeW(url.to_wide());
	
	String html;
	//html = String::format(_S("<a class=\"%S\" href=\"%S\">%S</a>").c_str(), css.c_str(), url.c_str(), body.c_str());
	html = String::format(_S("<a href=\"#\" class=\"%S\" onclick=\"Osiris.scrollTo('%S');return false;\">%S</a>").c_str(), css.c_str(), url.c_str(), body.c_str());

	return html;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
