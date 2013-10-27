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
#include "omlemail.h"

#include "omlcontext.h"
#include "omlitem.h"
#include "iidepage.h"

#include "options.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLEMail::OMLEMail(const String& tag) : IOMLCode(tag)
{		
}

OMLEMail::~OMLEMail()
{
}

String OMLEMail::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	String css = OS_CSS_OML_EMAIL;
	
	mapDefaultParamTo(i, _S("href"));
	
	String body;
	String href = i->getParam(_S("href"));

	if(href.empty())
	{
		href = i->getSingleText();
		body = encodeBody(context, i->getSingleText(), false);
	}
	else
	{
		body = i->getHtmlChilds(context);
	}

	if(context->getMode() == omlRenderModeSearch)
		return href;

	// Check se sono abilitati i link esterni
	if( (context->getSecure()) || (!bool(context->getPage()->getOption(Options::privacy_options::allow_external_urls))) )
	{
		context->setSecureCheck(false);
		href = String::EMPTY;
	}
	
	String html;
	if(href.empty())
		html = String::format(_S("<span class=\"%S\">%S</span>").c_str(), css.c_str(), body.c_str());
	else
		html = String::format(_S("<a class=\"%S\" target=\"_new\" href=\"mailto:%S\">%S</a>").c_str(), css.c_str(), href.c_str(), body.c_str());

	return html;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
