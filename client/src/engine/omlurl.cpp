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
#include "omlurl.h"

#include "convert.h"
#include "conversions.h"
#include "css.h"
#include "httpparser.h"
#include "httpurl.h"
#include "omlcontext.h"
#include "omlitem.h"
#include "osirislink.h"
#include "iidepage.h"
#include "ideskin.h"

#include "options.h"
#include "platformmanager.h"
#include "idesession.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "regexmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLUrl::OMLUrl(const String &tag) : IOMLCode(tag)
{

}

OMLUrl::~OMLUrl()
{

}

String OMLUrl::specialUrlDecode(const String &url) const
{
	/* 
	URGENT
	Qui non chiamo la HttpParser::urlDecode, perchè i link trattati da questa non
	sono standard. Ad esempio, un link ed2k:// può contenere caratteri giapponesi, mentre un url standard
	dovrebbe contenere solo caratteri ascii. 
	Il problema è che un link ed2k contenente caratteri jappo non viene mostrato a video correttamente così.
	*/
	String output = url;
	output.replace_all(_S("%20"), _S(" "));
	return output;	
}

void OMLUrl::generateBodyFromUrl(shared_ptr<OMLContext> context, const String& url, String& preBody, String& body, String& afterBody) const
{
	OS_ASSERT(context->getPage() != nullptr);

	OsirisLink osirisLink(url.to_ascii());

	if(osirisLink.isValid())
	{
		String logoUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/url_osiris_portal.png"));
		/*
		String re = _S("osiris://\\|portal\\|(.*?)\\|name=(.*?)\\|.*"); // Non  proprio il massimo... pu sbagliare a seconda della posizione dell'attributo "name".
		String rp = _S("$2");
		String out = regex_replace(url, RegexManager::instance()->getRegex(re, true), rp);
		//return _S("<span class=\"os_oml_url_osiris\">") + encodeBody(context, out, false) + _S("</span>");
		out = HttpParser::urlDecodeW(out.to_ascii());

		preBody = _S("<img style=\"vertical-align:middle\" src=\"")+ logoUrl +_S("\" alt=\"\" />"); 
		body = _S("<span class=\"os_oml_url_osiris\">") + encodeBody(context, out, false) + _S("</span>");
		*/
		preBody = _S("<img style=\"vertical-align:middle\" src=\"")+ logoUrl +_S("\" alt=\"\" />"); 
		body = _S("<span class=\"os_oml_url_osiris\">") + encodeBody(context, osirisLink.getName(), false) + _S("</span>");
	}
	else
	{
		String protocol = extractProtocol(url);
		if( protocol == _S("ed2k") )
		{
			if(url.starts_with(_S("ed2k://|server|")))
			{	
				String logoUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/url_ed2k_server.png"));
				String re = _S("ed2k://\\|server\\|(.*?)\\|(.*?)\\|/");
				String rp = _S("$1:$2");
				String out = regex_replace(url, RegexManager::instance()->getRegex(re, true), rp);
				//return _S("<span class=\"os_oml_url_ed2k_server\">") + encodeBody(context, out, false) + _S("</span>");
				out = specialUrlDecode(out);

				preBody = _S("<img style=\"vertical-align:middle\" src=\"")+ logoUrl +_S("\" alt=\"\" />");
				body = _S("<span class=\"os_oml_url_ed2k_server\">") + encodeBody(context, out, false) + _S("</span>");
				return;
			}
			else if(url.starts_with(_S("ed2k://|file|")))
			{
				String logoUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/url_ed2k_file.png"));
				String re = _S("ed2k://\\|file\\|(.*?)\\|(.*?)\\|(.*?)\\|/");
				String rp = _S("$1 ($2 bytes)");
				String out = regex_replace(url, RegexManager::instance()->getRegex(re, true), rp);

				out = specialUrlDecode(out);

				// Ci vorrebbe la callback nella regex_replace...
				{
					String sizeString = regex_replace(url, RegexManager::instance()->getRegex(re, true), _S("$2"));
					uint64 sizeBytes = conversions::from_utf16<uint64>(sizeString);
					out = out.replace_all(sizeString + _S(" bytes"), utils::formatSize(sizeBytes));				
				}

				//return _S("<span class=\"os_oml_url_ed2k_file\">") + encodeBody(context, out, false) + _S("</span>");
				preBody = _S("<img style=\"vertical-align:middle\" src=\"")+ logoUrl +_S("\" alt=\"\" />");
				body = _S("<span class=\"os_oml_url_ed2k_file\">") + encodeBody(context, out, false) + _S("</span>");
				return;
			}
			else if(url.starts_with(_S("ed2k://|serverlist|")))
			{	
				String logoUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/url_ed2k_serverlist.png"));
				String re = _S("ed2k://\\|serverlist\\|(.*?)\\|/");
				String rp = _S("$1");
				String out = regex_replace(url, RegexManager::instance()->getRegex(re, true), rp);
				//return _S("<span class=\"os_oml_url_ed2k_server\">") + encodeBody(context, out, false) + _S("</span>");
				out = specialUrlDecode(out);

				preBody = _S("<img style=\"vertical-align:middle\" src=\"")+ logoUrl +_S("\" alt=\"\" />");
				body = _S("<span class=\"os_oml_url_ed2k_serverlist\">") + encodeBody(context, out, false) + _S("</span>");
				return;
			}		
			else
			{
				body = encodeBody(context, url, false);
				return;
			}
		}
		else if( protocol == _S("ants2k") )
		{
			// ants2k://|file|<nome_File>|<Dimensioni_in_Byte>|<hash_md4_file>|/
			// ants2k://|file|[FUMETTO - ITA]Uccidere_Un_Hacker.7z|14892352|34dbe82a0325728485417292aa68b7e3|/

			String logoUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/url_ants2k_file.png"));
			String re = _S("ants2k://\\|file\\|(.*?)\\|(.*?)\\|(.*?)\\|/");
			String rp = _S("$1 ($2 bytes)");
			String out = regex_replace(url, RegexManager::instance()->getRegex(re, true), rp);

			out = specialUrlDecode(out);

			// Ci vorrebbe la callback nella regex_replace...
			{
				String sizeString = regex_replace(url, RegexManager::instance()->getRegex(re, true), _S("$2"));
				uint64 sizeBytes = conversions::from_utf16<uint64>(sizeString);
				out = out.replace_all(sizeString + _S(" bytes"), utils::formatSize(sizeBytes));				
			}
		
			//return _S("<span class=\"os_oml_url_ed2k_file\">") + encodeBody(context, out, false) + _S("</span>");
			preBody = _S("<img style=\"vertical-align:middle\" src=\"")+ logoUrl +_S("\" alt=\"\" />");
			body = _S("<span class=\"os_oml_url_ants2k_file\">") + encodeBody(context, out, false) + _S("</span>");
			return;
		}
		else if( protocol == _S("magnet") )
		{
			// osiris:type=portal&id=fddsfd&name=Osiris&user=jfjfd
			// osiris:type=url&url=& |
			// magnet:?xt=urn:bitprint:FBMCGD676KVF3MBJBCGFMCP6FU6VEJEV.RAGKQO73OA3B3HQMPTLC2QNJEWN2OSTBEK52TZY&dn=Aliprandi-copyleft.pdf
			//return encodeBody(context, url, false);

			String logoUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/url_magnet_file.png"));
		
			// Parsering da fare..
			String out = url;
			/*String re = _S("ants2k://\\|file\\|(.*?)\\|(.*?)\\|(.*?)\\|/");
			String rp = _S("$1 ($2 bytes)");
			String out = regex_replace(url, re, rp, true);

			// Ci vorrebbe la callback nella regex_replace...
			{
				String sizeString = regex_replace(url, re, _S("$2"), true);
				uint32 sizeBytes = Convert::toUint32(sizeString);
				out = out.replace_all(sizeString + _S(" bytes"), utils::formatSize(sizeBytes));				
			}
			*/
			out = HttpParser::urlDecodeW(out.to_ascii());
		
			preBody = _S("<img style=\"vertical-align:middle\" src=\"")+ logoUrl +_S("\" alt=\"\" />");
			body = _S("<span class=\"os_oml_url_magnet_file\">") + encodeBody(context, out, false) + _S("</span>");
			return;
		}
		else if( protocol == _S("stealthnet") )
		{
			// Ex. stealthnet://?hash=1F76A165384B6D391E549EACADCC2B7F01B32CC55F647E5C5DE8F976FDADB46E39988FAC05914159CEC52A994FABDA33A7A6FA55B237FFA5773FA74CD537824C&name=(eBook+-+French)+-+Linux+Magazine+France+-+No+85+-+Juillet+Aout+2006.pdf&size=21906308

			String logoUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/url_stealthnet_file.png"));
		
			std::string urlParams = url.substr(14).to_ascii();
			ordered_map<std::wstring, std::wstring> params;
			HttpParser::loadUrlParams(urlParams, params);
				
			String out = _S("");
			if(params.exists(_W("name")))
				out = params.get(_W("name"));

			if(params.exists(_W("size")))
			{
				uint64 sizeBytes = conversions::from_wstring<uint64>(params.get(_W("size")));
				out += _S(" (") + utils::formatSize(sizeBytes) + _S(")");
			}

			out = HttpParser::urlDecodeW(out.to_ascii());
		
			preBody = _S("<img style=\"vertical-align:middle\" src=\"")+ logoUrl +_S("\" alt=\"\" />");
			body = _S("<span class=\"os_oml_url_stealthnet_file\">") + encodeBody(context, out, false) + _S("</span>");
			return;
		}
		else if (url.starts_with(_S("http://localhost:23402/offsystem/v2/application/")))
		{
			// Ex. http://localhost:23402/offsystem/v2/application/x-msdownload/976709d8376e2f4228a955747c3d00be7ba14d9c/9009292/1e7cff52e10cf07fcd6819f712aee332b053f625/744ccccf5c51f44c3345d4d0ac13d0af797a4dd3/129b8be9c332d241394c9f8f94bb20353fc7c89e/031a49a560c1bf59062a8ed390596ef03ee085cb/352421fd9fd1be7164798502d2df1c7b19cb413d/osiris0.11.exe

			String logoUrl = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/url_offsystem_file.png"));

			// OffSystem V2 uses 9 parameters. See docs here: http://offsystem.sourceforge.net/off-system-introduction/urls/
			String re = _S("http://localhost:23402/offsystem/v2/application/(.*?)/(.*?)/(.*?)/(.*?)/(.*?)/(.*?)/(.*?)/(.*?)/(.*?)$");
			String rp = _S("$9");
			String out = regex_replace(url, RegexManager::instance()->getRegex(re, true), rp);
			//return _S("<span class=\"os_oml_url_ed2k_server\">") + encodeBody(context, out, false) + _S("</span>");
			out = specialUrlDecode(out);

			preBody = _S("<img style=\"vertical-align:middle\" src=\"")+ logoUrl +_S("\" alt=\"\" />");
			body = _S("<span class=\"os_oml_url_offsystem_file\">") + encodeBody(context, out, false) + _S("</span>");			
		}
		else
		{
			body = encodeBody(context, url, false);
			return;
		}
	}
}

bool OMLUrl::allowRowMode() const
{
	return true;
}

String OMLUrl::processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{	
	mapDefaultParamTo(i, _S("href"));

	String target;
	String css;
	
	String preBody;
	String body;
	String postBody;
	String url = i->getParam(_S("href"));
	bool sureLink = false;

	if(url.empty())
	{		
		url = i->getSingleText();

		if(context->getMode() == omlRenderModeSearch)
			return _S(" ") + url + _S(" ");

		generateBodyFromUrl(context, url, preBody, body, postBody);
	}
	else
	{
		body = i->getHtmlChilds(context);

		if(context->getMode() == omlRenderModeSearch)
			return _S(" ") + url + _S(" ") + body + _S(" ");
	}	

	if( (i->hasParam(_S("collect")) == false) || (i->getParam(_S("collect")) == _S("true")) )
		context->addUrl(i, url);
	
	// E' la encodeUrl che si smazza se aprire una nuova finestra, dato che a seconda del protocollo
	// di alcuni url esterni pu non doversi aprire una nuova finestra, anche se specificato [url] e non [iurl].
	OMLRenderUrlDestination omlRenderUrlDestination = (i->getTagName() == _S("url")) ? omlRenderUrlDestinationNewWindow : omlRenderUrlDestinationSame;
	url = encodeUrl(context, i, url, omlRenderUrlDestination, false);
	if(omlRenderUrlDestination == omlRenderUrlDestinationNewWindow)
		target = _S("_blank");
	else
		target = _S("_self");
	
	String html;
	if(url.empty())
		html = String::format(_S("<span class=\"%S\">%S</span>").c_str(), css.c_str(), body.c_str());
	else
		html = String::format(_S("%S<a class=\"%S\" href=\"%S\" target=\"%S\">%S</a>%S").c_str(), preBody.c_str(), css.c_str(), url.c_str(), target.c_str(), body.c_str(), postBody.c_str());

	return html;
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
