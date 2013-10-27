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
#include "omlpopup.h"

#include "omlcontext.h"
#include "omlitem.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLPopup::OMLPopup(const String& tag) : IOMLCode(tag)
{	
}

OMLPopup::~OMLPopup()
{

}

String OMLPopup::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	mapDefaultParamTo(i, _S("body"));

	String childsHtml = i->getHtmlChilds(context);
	String mode = i->getParam(_S("mode"));
	if(mode.empty())
		mode = _S("popup");
	
	if( (mode != _S("tooltip")) && (mode != _S("static")) && (mode != _S("staticclick")) && (mode != _S("popup")) && (mode != _S("popupclick")) && (mode != _S("window")) && (mode != _S("dialog")))
	{
		context->addWarning(String::format(_S("%S doesn't support the mode '%S'.").c_str(), context->getFriendlyName(i).c_str(), mode.c_str()));
		return String::EMPTY; 
	}
	else
	{
		String bodyHtml = encodeOML(context, i->getParam(_S("body")));

		if(context->getMode() == omlRenderModeSearch)
			return childsHtml + _S(" ") + bodyHtml;
		else
		{
			String out = _S("");
			out += _S("<div class=\"os_oml_popup_main\" data-os-otype=\"popup\" data-os-popupMode=\"") + mode + _S("\">");
			out += _S("<div class=\"os_oml_popup_header\" data-os-popup=\"header\">");
			out += childsHtml;
			out += _S("</div>");
			out += _S("<div class=\"os_oml_popup_body\" style=\"display:none;\" data-os-popup=\"body\"");
			if(i->getParam(_S("title")).empty() == false)
				out += _S(" title=\"") + encodeOML(context, i->getParam(_S("title"))) + _S("\"");
			out += _S(">");
			out += bodyHtml;
			out += _S("</div>");
			out += _S("</div>");

			return out;
		}
	}
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
