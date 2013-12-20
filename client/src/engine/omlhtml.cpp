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
#include "omlhtml.h"

#include "css.h"
#include "entitiesentity.h"
#include "omlcontext.h"
#include "omlitem.h"
#include "idesystem.h"
#include "iideportalpage.h"
#include "ideskin.h"
#include "objectsirevisionable.h"

#include "objectsuser.h"
#include "options.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

/*
Esempio d'uso:
[html]
<object width="425" height="355">
  <param name="movie" value="http://www.youtube.com/v/DZt2nH86dTg&rel=1"></param>
  <param name="wmode" value="transparent"></param>
  <embed 
      src="http://www.youtube.com/v/DZt2nH86dTg&rel=1" 
      type="application/x-shockwave-flash" 
      wmode="transparent" 
      width="425" 
      height="355">
  </embed>
</object>
[/html]

---------------------------
In maniera simile, ci sarebbe da realizzare i tag [object], [param] e [embed]:

Dal codice di : http://www.osiris-sps.org/forums/index.php?showtopic=494&hl=flash

Diventa:

[object width="425" height="355">
  [param name="movie" value="http://www.youtube.com/v/DZt2nH86dTg&rel=1"][/param]
  [param name="wmode" value="transparent"][/param]
  [embed 
      src="http://www.youtube.com/v/DZt2nH86dTg&rel=1" 
      type="application/x-shockwave-flash" 
      wmode="transparent" 
      width="425" 
      height="355"]
  [/embed]
[/object]

(p.s. attento, c'è da aggiungere i "px")
(similmente al tag [html], genera il codice).

<object width=\"425\" height=\"355\">  <param name=\"movie\" value=\"http://www.youtube.com/v/DZt2nH86dTg&rel=1\"></param>  <param name=\"wmode\" value=\"transparent\"></param>  <embed       src=\"http://www.youtube.com/v/DZt2nH86dTg&rel=1\"       type=\"application/x-shockwave-flash\"       wmode=\"transparent\"       width=\"425\"       height=\"355\">  </embed></object>

('abc' è l'id del contesto bbcode).



<div id="abc" style="width:425px;">
	<div id="abc_wrn" class="os_oml_object_warning">
		<div class="os_oml_object_warning_title">Anonymity Warning</div>
		<span class="os_label">Type: </span><img src="/skins/osiris/images/mime/application-x-shockwave-flash.gif"><span class="os_value"> application/x-shockwave-flash</span><br/>
		<span class="os_label">Url: </span><span class="os_value">http://www.youtube.com/v/DZt2nH86dTg&rel=1</span><br/><br/>
		<script type="text/javascript">
		function osBBCodeObject_abc()
		{
			code = "<object width=\"425\" height=\"355\">  <param name=\"movie\" value=\"http://www.youtube.com/v/DZt2nH86dTg&rel=1\"></param>  <param name=\"wmode\" value=\"transparent\"></param>  <embed       src=\"http://www.youtube.com/v/DZt2nH86dTg&rel=1\"       type=\"application/x-shockwave-flash\"       wmode=\"transparent\"       width=\"425\"       height=\"355\">  </embed></object>";
			osOmlInject('abc',code);
		}
		</script>
		<a class="os_button" href="javascript:void(osBBCodeObject_abc())">Show this content</a>
	</div>
</div>

*/

OMLHtml::OMLHtml(const String& tag) : IOMLCode(tag)
{	
}

OMLHtml::~OMLHtml()
{

}

String OMLHtml::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	// Deprecated in 0.14
	return String::EMPTY;
	/*
	//String text = encodeBody(context, i->getSingleText(), false, true);
	String text = i->getSingleText();

	if(context->getMode() == omlRenderModeSearch)
	{
		return _S(" ") + text + _S(" ");
	}
	else
	{
		// 0- No, 1- Su conferma, 2- Sempre
		shared_ptr<const IPortalPage> page = context->getPortalPage();		
		uint32 allowedMode = 0;
		if(page != nullptr)
			allowedMode = page->getOption(Options::anonymity_options::html_mode);		
		
		if(allowedMode == 0)
		{
			return String::EMPTY;
		}
		else if(allowedMode == 2)
		{
			// Dump diretto
			return text;
		}
		else if(allowedMode == 1)
		{
			String code;
			String id = context->getContextNextID();
			String title = encode(context, getText(context, _S("oml.html.messages.warning.title"))); 
			String subtitle = encode(context, getText(context, _S("oml.html.messages.warning.description")));
			String confirm = encode(context, getText(context, _S("oml.html.messages.warning.confirm")));

			String warningImageUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/html_warning.png"));

			text.replace_all(_S("\""),_S("\\\""));
			text.replace_all(_S("\r"),_S("\\r"));
			text.replace_all(_S("\n"),_S("\\n"));

			code += _S("<div id=\"") + id + _S("\">");
			code += _S(" <div id=\"") + id + _S("_wrn\" class=\"os_oml_html_warning\">");
			code += _S("  <img style=\"float:left;\" src=\"") + warningImageUrl + _S("\">");
			code += _S("  <span class=\"os_oml_html_warning_title\">") + title + _S("</span>");
			code += _S("  <br/>");
			code += _S("  <span class=\"os_oml_html_warning_subtitle\">") + subtitle + _S("</span>");
			code += _S("  <br/><br/>");
			code += _S("  <script type=\"text/javascript\">\r\n");
			code += _S("  function osBBCodeHtml_") + id + _S("()\r\n");
			code += _S("  {\r\n");
			code += _S("   code = \"") + text + _S("\";\r\n");
			code += _S("   osOmlInject('") + id + _S("',code);\r\n");
			code += _S("  }\r\n");
			code += _S("  </script>\r\n");
			code += _S("  <a href=\"javascript:void(osBBCodeHtml_") + id + _S("())\">") + confirm + _S("</a>");
			code += _S(" </div>");
			code += _S("</div>");

			return code;
		}		
		else
		{
			return String::EMPTY;
		}
	}
	*/
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
