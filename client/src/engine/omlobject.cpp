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
#include "omlobject.h"

#include "css.h"
#include "httpurl.h"
#include "omlcontext.h"
#include "omlmanager.h"
#include "omlitem.h"
#include "idesystem.h"
#include "iidepage.h"
#include "idepages.h"
#include "iideportalpage.h"
#include "ideskin.h"
#include "log.h"
#include "options.h"
#include "portalsportal.h"
#include "idesession.h"
#include "regexmanager.h"

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

OMLObject::OMLObject(const String& tag) : IOMLCode(tag)
{
}

OMLObject::~OMLObject()
{

}

String OMLObject::directRender(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	if(i->getTagName() == OMLItem::ITEM_TAG_TEXT)
	{
		return i->getParam(OMLItem::ITEM_PARAM_TEXT);
	}

	String childsHtml;
	for(OMLManager::Items::const_iterator i2 = i->getChilds().begin(); i2 != i->getChilds().end(); ++i2)
	{
		shared_ptr<OMLItem> child = *i2;
		childsHtml += directRender(child,context) + _S("\r\n");
	}

	String attributes;
	for(StringCollection<String>::iterator i3 = i->getParams().begin(); i3 != i->getParams().end(); ++i3)
	{
		String paramName = i3->first;
		String paramValue = *i3->second;

		if(!paramValue.empty())
			attributes+=_S(" ") + paramName + _S("=\"") + paramValue + _S("\"");
	}

	// Da sistemare per output XHTML
	return _S("<") + i->getTagName() + attributes + _S(">") + childsHtml + _S("</") + i->getTagName() + _S(">");
}

String OMLObject::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	String body = i->getHtmlChilds(context);
	if(context->getMode() == omlRenderModeSearch)
	{
		return _S(" ") + body + _S(" ");
	}
	else if(i->getTagName() == _S("object"))
	{
		shared_ptr<const IPortalPage> page = context->getPortalPage();
		if(page == nullptr)
			return String::EMPTY;

		String src;
		String type;
		String imageType = context->getPage()->getSkin()->getImageUrl(_S("mime/unknown.gif"));

		for(OMLManager::Items::const_iterator i2 = i->getChilds().begin(); i2 != i->getChilds().end(); ++i2)
		{
			shared_ptr<OMLItem> child = *i2;
			if( (child->getTagName() == _S("embed")) && (!child->getSkipped()) )
			{
				if(child->hasParam(_S("src")))
					src = child->getParam(_S("src"));
				if(child->hasParam(_S("type")))
				{
					type = child->getParam(_S("type"));
					String type2 = type.replace_all(_S("/"),_S("-"));
					imageType = context->getPage()->getSkin()->getImageUrl(_S("mime/") + type2 + _S(".gif"));
				}
			}
		}

		// 0- No, 1- Su conferma, 2- Sempre
		uint32 allowedMode = 0;
		String trustedDomains = String::EMPTY;
		String trustedTypes = String::EMPTY;

		if(page != nullptr)
		{
			allowedMode = static_cast<uint32>(page->getOption(Options::anonymity_options::object_mode));
			trustedDomains = static_cast<String>(page->getOption(Options::anonymity_options::object_trusted_domains));
			trustedTypes = static_cast<String>(page->getOption(Options::anonymity_options::object_trusted_types));
		}

		String imageTrusting = context->getPage()->getSkin()->getImageUrl(_S("small/add.gif"));
		String htmlTrusting = _S("<img src=\"") + imageTrusting + _S("\">");

		// Trusting links
		//bool allowTrusting = (page->getLoggedUser()->getGuestMode() == false);
		bool allowTrusting = false; // Se "false" non puoi aggiungerle. TOFIX -> Da abolire.
		String hrefTrustDomain;
		if(src.empty())
		{
			src = encode(context, getText(context, _S("oml.object.messages.warning.unknown")));
		}
		else
		{
			// Trusted checking
			HttpUrl url(src.to_ascii());
			if(regex_match(url.getHost(), RegexManager::instance()->getRegex(_S("(") + trustedDomains + _S(")"), true)))
			{
				allowedMode=2;
			}
			else if( (page != nullptr) && (allowTrusting) )
			{
				ordered_map<std::wstring, std::wstring> params;
				params.set(_W("name"), _W("domain"));
				params.set(_W("value"), encode(context, url.getHost()).to_wide());

				hrefTrustDomain = page->getPortal()->getLink(OS_IDE_PAGE_BBOBJECT, params);
			}
		}

		String hrefTrustType;
		if(type.empty())
		{
			type = encode(context, getText(context, _S("oml.object.messages.warning.unknown")));
		}
		else
		{
			if(regex_match(type, RegexManager::instance()->getRegex(_S("(") + trustedTypes + _S(")"), true)))
			{
				allowedMode=2;
			}
			else if( (page != nullptr) && (allowTrusting) )
			{
				ordered_map<std::wstring, std::wstring> params;
				params.set(_W("name"), _W("type"));
				params.set(_W("value"), encode(context, type).to_wide());

				hrefTrustType = page->getPortal()->getLink(OS_IDE_PAGE_BBOBJECT, params);
			}
		}

		if(allowedMode == 0)
		{
			return String::EMPTY;
		}
		else
		{
			// Composizione diretta senza alcuna trasformazione
			String text = directRender(i, context);

			if(allowedMode == 2)
			{
				// Dump diretto
				return text;
			}
			else if(allowedMode == 1)
			{
				// Encoding per injection javascript
				text.replace_all(_S("\""),_S("\\\""));
				text.replace_all(_S("\r"),_S("\\r"));
				text.replace_all(_S("\n"),_S("\\n"));

				String code;
				String id = context->getContextNextID();
				String title = encode(context, getText(context, _S("oml.object.messages.warning.title")));
				String confirm = encode(context, getText(context, _S("oml.object.messages.warning.confirm")));
				String labelType = encode(context, getText(context, _S("oml.object.messages.warning.type")));
				String labelUrl = encode(context, getText(context, _S("oml.object.messages.warning.url")));

				String warningImageUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/object_warning.png"));

				String width = encodeToMeasure(context, i, i->getParam(_S("width")));
				String height = encodeToMeasure(context, i, i->getParam(_S("height")));
				if(width.empty())
					width = _S("200px");
				if(height.empty())
					height = _S("50px");

				code += _S("<div id=\"") + id + _S("\" style=\"width:") + width + _S(";height:") + height + _S(";\">");
				code += _S("	<div id=\"") + id + _S("_wrn\" class=\"os_oml_object_warning\">");
				code += _S("		<img style=\"float:left;\" src=\"") + warningImageUrl + _S("\">");
				code += _S("		<div class=\"os_oml_object_warning_title\">") + title + _S("</div>");
				code += _S("		<span class=\"os_label\">") + labelType + _S(": </span><img src=\"") + imageType + _S("\"><span class=\"os_value\">") + type + _S("</span>");

				if(hrefTrustType.empty() == false)
					code += _S("<a href=\"") + hrefTrustType + _S("\">") + htmlTrusting + _S("</a>");
				code += _S("<br />");

				code += _S("		<span class=\"os_label\">") + labelUrl + _S(": </span><span class=\"os_value\">") + src + _S("</span>");

				if(hrefTrustDomain.empty() == false)
					code += _S("<a href=\"") + hrefTrustDomain + _S("\">") + htmlTrusting + _S("</a>");
				code += _S("<br /><br />");

				code += _S("		<script type=\"text/javascript\">");
				code += _S("		function osOmlObject_") + id + _S("()\r\n");
				code += _S("		{\r\n");
				code += _S("			code = \"") + text + _S("\";\r\n");
				code += _S("			osOmlInject('") + id + _S("',code);\r\n");
				code += _S("		}\r\n");
				code += _S("		</script>\r\n");
				code += _S("		<a href=\"javascript:void(osOmlObject_") + id + _S("())\">") + confirm + _S("</a>");
				code += _S("	</div>");
				code += _S("</div>");

				/*
				text.replace_all(_S("\\\""),_S("\""));
				text.replace_all(_S("\\r"),_S("\r"));
				text.replace_all(_S("\\n"),_S("\n"));
				code += encode(context,text);
				*/

				return code;
			}
			else
			{
				return String::EMPTY;
			}
		}
	}
	else if(i->getTagName() == _S("embed"))
	{
		return String::EMPTY;
	}
	else if(i->getTagName() == _S("param"))
	{
		return String::EMPTY;
	}
	else
	{
		return String::EMPTY;
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
