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
#include "omlcode.h"

#include "omlcontext.h"
#include "omlitem.h"
#include "idesystem.h"
#include "ideskin.h"
#include "objectsirevisionable.h"

#include "objectsuser.h"
#include "entitiesentity.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLCode::OMLCode(const String& tag) : IOMLCode(tag)
{	
}

OMLCode::~OMLCode()
{

}

String OMLCode::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	mapDefaultParamTo(i, _S("title"));

	String title = encodeOML(context, i->getParam(_S("title")));

	bool startClose = false;
	if( (i->hasParam("close")) && (i->getParam("close") == "true") )
	{
		startClose = true;

		if(title.empty())
			title = encode(context, getText(context, _S("oml.code.click_to_open")));
	}
	
	//String text = encodeBody(context, i->getSingleText(), false, true);
	String text = i->getSingleText();

	if(context->getMode() == omlRenderModeSearch)
	{
		return _S(" ") + title + _S(" ") + text + _S(" ");
	}
	else
	{
		String cssBody;
		if(i->getTagName() == _S("codebox"))
		{
			cssBody = OS_CSS_OML_CODEMAINBOX;
		}
		else
		{
			cssBody = OS_CSS_OML_CODEMAIN;
		}
		
		//String html = _S("<div class=\"") + OS_CSS_OML_CODETOP + _S("\">") + title + _S("</div><div class=\"") + cssBody + _S("\">") + text + _S("</div>");
		String html = String::EMPTY;

		if(context->getMode() == omlRenderModeOsiris)
		{
			if(startClose == true)
				html += _S("<div data-os-otype=\"popup\" data-os-popupMode=\"staticclick\"");

			if( (title.empty() == false) || (startClose) )
			{
				html += _S("<div class=\"") + OS_CSS_OML_CODETOP + _S("\"");		
				if(startClose)
					html += _S(" data-os-popup=\"header\"");
				html += _S(">");
				html += title;
				// 0.13
				//String textSelect = encode(context, getText(context, _S("oml.code.select")));
				//html += _S("<a class=\"") + _S("os_oml_code_select") + _S("\" href=\"#\" onclick=\"javascript:osOmlSelectAll(this);return false;\">") + textSelect + _S("</a>");		
				html += _S("</div>");						
			}

			// 0.13
			//html += _S("<code class=\"") + cssBody + _S("\">") + text + _S("</code>");

			String params = _S("brush:");

			if(i->hasParam("language"))
				params += encodeToAttribute(context, i->getParam("language"));
			else
				params += _S("plain");
			
			if(i->hasParam("gutter"))
				if(i->getParam("gutter") == _S("false"))
					params += _S(";gutter:false");
				else
					params += _S(";gutter:true");

			if(i->hasParam("firstLine"))
				params += _S(";first-line:") + encodeToAttribute(context,i->getParam("firstLine"));

			if(i->hasParam("highlight"))
				params += _S(";highlight:[") + encodeToAttribute(context,i->getParam("highlight")) + _S("]");
			
			html += _S("<div class=\"") + cssBody + _S("\"");
			if(startClose)
				html += _S(" data-os-popup=\"body\"");
			html += _S(">");			
			html += _S("<pre class=\"") + params + _S("\">"); // Read comment about SyntaxHighlight in libraries.hs
			//html += _S("<pre class=\"") + params + _S("\" data-os-syntaxhighlighter=\"") + params + _S("\">");
			//html += _S("<![CDATA[");
			//html += encode(context, text);
			html += encodeBody(context, text, false, false, false);
			//html += _S("]]>");
			html += _S("</pre>");
			html += _S("</div>");

			if( (context->getMode() == omlRenderModeOsiris) && (startClose == true) )
				html += _S("</div>");
		}
		else if(context->getMode() == omlRenderModeExternal)
		{
			text = encodeBody(context, text, false, true);
			html += _S("<code>") + text + _S("</code>");
		}
		return html;
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
