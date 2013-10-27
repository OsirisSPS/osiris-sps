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
#include "iomlcode.h"

#include "algorithms.h"
#include "conversions.h"
#include "entitiesentity.h"
#include "httpparser.h"
#include "httpurl.h"
#include "htmlparser.h"
#include "omlcontext.h"
#include "omlemoticon.h"
#include "omlitem.h"
#include "omlmanager.h"
#include "omlurl.h"
#include "osirislink.h"
#include "idesystem.h"
#include "iidepage.h"
#include "iideportalpage.h"
#include "ideskin.h"
#include "log.h"
#include "objectsiobject.h"
#include "objectsirevisionable.h"

#include "options.h"
#include "idesession.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "regexmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IOMLCode::IOMLCode(const String& tag)
{
	m_tag = tag;
}

IOMLCode::~IOMLCode()
{

}

String IOMLCode::extractProtocol(const String &url)
{
	// Ex. "http://www.google.it";

	String::size_type posDot = url.find(_S(":"));
	if(posDot != String::npos)
	{
		String protocol = url.left(posDot).to_lower();
		return protocol;
	}

	return String::EMPTY;
}

void IOMLCode::mapDefaultParamTo(shared_ptr<OMLItem> i, const String &paramName) const
{
	if( (i->hasParam(OMLItem::ITEM_PARAM_DEFAULT)) && (!i->hasParam(paramName)) )
	{
		i->setParam(paramName, i->getParam(OMLItem::ITEM_PARAM_DEFAULT));
		i->removeParam(OMLItem::ITEM_PARAM_DEFAULT);
	}
}

void IOMLCode::removeTag(shared_ptr<OMLItem> i, const String &tag, const bool recursive) const
{
	for(OMLManager::Items::const_iterator i2 = i->getChilds().begin(); i2 != i->getChilds().end(); ++i2)
	{
		shared_ptr<OMLItem> child = *i2;
		if(child->getTagName() == tag)
		{
			child->setSkipped(true);
		}
		else
		{
			if(recursive)
				removeTag(child, tag, true);
		}
	}
}

bool IOMLCode::allowedParentTags(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String &tags) const
{
	StringVector tarray;
	utils::split(tags,_S(";"), tarray);

	bool found = false;
	String parentTag = i->getParent()->getTagName();
	for(uint32 a1=0; a1<tarray.size(); a1++)
	{
		if(parentTag == tarray[a1])
		{
			found = true;
			break;
		}
	}

	if(!found)
		context->addWarningMustBeInside(i, tags);

	return found;
}

void IOMLCode::allowedChildsTags(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String &tags) const
{
	StringVector tarray;
	utils::split(tags,_S(";"), tarray);

	// Se il tag [br] non  abilitato, allora rimuovo gli \n e i testi vuoti.
	bool removeEmptyTexts = true;
	for(uint32 a1=0; a1<tarray.size(); a1++)
	{
		if(tarray[a1] == _S("br"))
		{
			removeEmptyTexts = false;
			break;
		}
	}

	for(OMLManager::Items::const_iterator i2 = i->getChilds().begin(); i2 != i->getChilds().end(); ++i2)
	{
		shared_ptr<OMLItem> child = *i2;
		bool found = false;
		for(uint32 a=0; a<tarray.size(); a++)
		{
			if(child->getTagName() == tarray[a])
			{
				found = true;
				break;
			}
		}

		if( (removeEmptyTexts) && (child->getTagName() == OMLItem::ITEM_TAG_TEXT) )
		{
			String text = child->getParam(OMLItem::ITEM_PARAM_TEXT);
			text.replace_all(_S("\n"), String::EMPTY);
			text.trim(_S(" "));
			if(text.empty())
				child->setSkipped(true);
		}

		if( (!found) && (!child->getSkipped()) )
		{
			context->addWarningCantBeHere(child, i);
			child->setSkipped(true);
		}
	}
}

void IOMLCode::removeTextSpacer2(shared_ptr<OMLItem> i, const bool recursive, const bool alsoBR) const
{
	for(OMLManager::Items::const_iterator i2 = i->getChilds().begin(); i2 != i->getChilds().end(); ++i2)
	{
		shared_ptr<OMLItem> child = *i2;
		if(child->getTagName() == OMLItem::ITEM_TAG_TEXT)
		{
			String text = child->getParam(OMLItem::ITEM_PARAM_TEXT);
			if(alsoBR)
				text.replace_all(_S("\n"), String::EMPTY);
			text.trim(_S(" "));
			if(text.empty())
				child->setSkipped(true);
		}
		else
		{
			if(recursive)
				removeTextSpacer2(child, true, alsoBR);
		}
	}
}

void IOMLCode::removeBR(shared_ptr<OMLItem> i) const
{
	// Tolgo i '\n' dai valori dei parametri
	for(StringCollection<String>::iterator i3 = i->getParams().begin(); i3 != i->getParams().end(); ++i3)
	{
		String paramName = i3->first;
		String &paramValue = *i3->second;
		paramValue.replace_all(_S("\n"), String::EMPTY);
	}

	for(OMLManager::Items::const_iterator i2 = i->getChilds().begin(); i2 != i->getChilds().end(); ++i2)
	{
		shared_ptr<OMLItem> child = *i2;
		if(child->getTagName() == OMLItem::ITEM_TAG_TEXT)
		{
			String text = child->getParam(OMLItem::ITEM_PARAM_TEXT);
			text.replace_all(_S("\n"), String::EMPTY);
			child->setParam(OMLItem::ITEM_PARAM_TEXT, text);
		}
		else
		{
			removeBR(child);
		}
	}
}

void IOMLCode::removeChilds(shared_ptr<OMLItem> i) const
{
	for(OMLManager::Items::const_iterator i2 = i->getChilds().begin(); i2 != i->getChilds().end(); ++i2)
	{
		shared_ptr<OMLItem> child = *i2;
		child->setSkipped(true);
	}
}

String IOMLCode::getText(shared_ptr<OMLContext> context, const String &id)
{
	return context->getPage()->getText(id);
}

// Pulisce l'url, o restituisce un url vuoto se è invalido.
String IOMLCode::cleanUrl(shared_ptr<OMLContext> context, const String& url)
{
	// TODO: Sarebbe il caso di mettere una regex di convalida

	// Tolgo gli acapo dagli url
	String out = url;
	out.trim_left(_S("'"));
	out.trim_left(_S("\""));
	out.trim_right(_S("'"));
	out.trim_right(_S("\""));
	out.replace_all(_S("\n"), String::EMPTY);
	out.replace_all(_S("\r"), String::EMPTY);

	return out;
}

String IOMLCode::encode(shared_ptr<OMLContext> context, const String& text)
{
	if(context->getMode() == omlRenderModeSearch)
		return text;

	String out = HtmlParser::instance()->encode(text);

	// Eventuali ulteriori encoding in pi rispetto agli standard.

	// Converto il carattere ' per passare argomenti a funzioni javascript client-side
	//out.replace_all(_S("'"),_S("&#39;"));

	return out;
}

String IOMLCode::encodeToJavascriptString(shared_ptr<OMLContext> context, const String& text)
{
	// da sistemare-verificare, soprattutto l'uso di "
	String out = text;

	out.replace_all(_S("'"), _S("\\'"));

	return encode(context, out);
}

String IOMLCode::encodeToMeasure(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& text)
{
	String out = text;
	out.trim();
	out = encodeToCss(context, i, out);
	if(conversions::to_utf16(conversions::from_utf16<int32>(out)) == out)
	{
		out += _S("px");
	}
	return out;
}

String IOMLCode::encodeToCss(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& text)
{
	OS_ASSERT(context != null);

	if(text.empty())
		return String::EMPTY;

	std::wstring out = text.to_wide();

	// Qui cerco le parentesi '(' e ')'.
	// Se le trovo e sono di un "url(..)" trasformo l'url, altrimenti
	// le tolgo.
	// Basato su IPB 2.1, che rimuove radicalmente le parentesi nei parametri, evitando chiamate a expression(), url() etc..
	// In realtà per ora qui rimuovo solo '(', non ')'. E' da sistemare.

	std::wstring::size_type posCurrent = 0;
	for(;;)
	{
		std::wstring::size_type posS = out.find(_W("("), posCurrent);

		if(posS == String::npos)
			break;

		bool transform = true;

		if(posS>3)
		{
			std::wstring prefix = out.substr(posS-3,4);
			algorithms::to_lower(prefix);
			if(prefix == _W("url("))
			{
				String::size_type posE = out.find(_W(")"), posS);
				if(posE != String::npos)
				{
					std::wstring url = out.substr(posS+1,posE-posS-1);
					url = encodeResourceUrl(context, i, url).to_wide();
					out = out.substr(0,posS+1) + url + out.substr(posE);
					posCurrent = posS + url.length() + 1;
					transform = false;
				}
			}
			else if(prefix == _W("rgb("))
			{
				// VERYURGENT, da testare con
				// [div style="color:rgb(255, 255, 0);"]prova[/div]
				// La 0.12 lo toglieva
				transform = false;
				posCurrent = posS + 3;
			}
		}

		if(transform)
		{
			// Lo tolgo.
			out = out.substr(0,posS) + out.substr(posS+1);
		}
	}
	return out;

	/*
	0.11
	String::size_type posCurrent = 0;
	for(;;)
	{
		String::size_type posS = out.find(_S("url("), posCurrent);
		if(posS == String::npos)
			break;

		String::size_type posE = out.find(_S(")"), posS);
		if(posE == String::npos)
			break;

		String url = out.substr(posS+4,posE-posS-4);
		url = encodeImageUrl(context, i, url);
		out = out.substr(0,posS) + _S("url(") + url + _S(")") + out.substr(posE+1);
		posCurrent += url.length() + 1;
	}
	return out;
	*/
}

String IOMLCode::encodeToAttribute(shared_ptr<OMLContext> context, const String& text)
{
	return encode(context, text);
}

String IOMLCode::encodeToUrlGet(shared_ptr<OMLContext> context, const String& text)
{
	return HttpParser::urlEncodeW(text.to_wide()); // to_ascii() maybe wrong...
}

String IOMLCode::encodeBody(shared_ptr<OMLContext> context, const String& text, bool postProcess, bool preserveStartLineSpaces, bool convertCR)
{
	String value = text;

	if ( (postProcess) && (context->getMode() != omlRenderModeSearch) && (!context->getRow()) )
	{
		// Detect degli url
		{
			String protocolsFindLinkOption = context->getPage()->getOption(Options::url_options::protocols_find_link);
			//String protocolsFindLinkOption = Options::instance()->getOption(Options::url_options::protocols_find_link);
			//String UrlMatch = _S("(")+getProtocolsFindLinkOption()+_S(")://(\\w*:\\w*@)?[-\\w.]+(:\\d+)?(/([\\w/_.]*(\\?\\S+)?)?)?");
			//String UrlMatch = _S("((")+getProtocolsFindLinkOption()+_S(")(\\W+\\S+[^).,:;?\\]\\} \\r\\n$]+))");
			//String UrlMatch = _S("(")+protocolsFindLinkOption+_S(")://(\\w*:\\w*@)?[-\\w.]+(:\\d+)?(/([\\w/_.]*(\\?\\S+)?)?)?");

			// 0.11:
			String UrlMatch = _S("(") + protocolsFindLinkOption + _S(")://\\S+");

			// La versione sotto sarebbe meglio, ma non funziona, ad esempio con il link seguente:
			// skype:?chat&blob=iCnRW4EXR0H_funqp8i2FpbcobXxBfFkllPp0s2NMRd_sbDEOEr5Je-RzNUi
			//String UrlMatch = _S("(")+protocolsFindLinkOption+_S("):\\S+");

			String UrlReplace = _S("[url]$0[/url]");
			//if(regex_match(value, RegexManager::instance()->getRegex(UrlMatch, true)))
			value = regex_replace(value, RegexManager::instance()->getRegex(UrlMatch, true), UrlReplace);
			
			//String MailMatch = _S("\\b((([A-Za-z0-9$_.+%=-])|%[A-Fa-f0-9]{2})+@(([A-Za-z0-9$_.+!*,;/?:%&=-])|%[A-Fa-f0-9]{2})+\\.[a-zA-Z0-9]{1,4})");
			String MailMatch = _S("(\\w+\\.)*\\w+@(\\w+\\.)+[A-Za-z]+");
			String MailReplace = _S("[email]$0[/email]");
			//if(regex_match(value, RegexManager::instance()->getRegex(MailMatch, true)))
			value = regex_replace(value, RegexManager::instance()->getRegex(MailMatch, true), MailReplace);			
		}
		if(value != text)
		{
			// Sono stati inseriti tag, ricomputo...
			return encodeOML(context, value);
		}
	}

	// Risolvo gli escaping
	value.replace_all(_S("\\["), _S("["));
	//value.replace_all(_S("\\]"), _S("]"));

	// Encoding html
	value = encode(context, value);
	
	if(postProcess)
	{
		// Emoticons
		if( (context->getMode() != omlRenderModeSearch) && (context->getPage() != null) )
		{
			String valueLower = value;
			valueLower.to_lower();

			//const StringMap &emoticons = IdeSystem::instance()->getEmoticons();
			//for(StringMap::const_iterator i = emoticons.begin(); i != emoticons.end(); ++i)
			const OMLManager::Emoticons &emoticons = OMLManager::instance()->getEmoticons();
			for(OMLManager::Emoticons::const_iterator i = emoticons.begin(); i != emoticons.end(); ++i)
			{
				//const String &code = i->first;
				shared_ptr<OMLEmoticon> emo = *i;
				const String &code = emo->getCode();

				// Ottimizzazione, se non lo trovo con una 'find' secca sull'equivalente in lower-case,
				//  inutile cercarlo via regex.
				if(valueLower.find(emo->getCode()) != String::npos)
				{
					//const String &name = i->second;
					const String &name = emo->getName();
					String title = getText(context, _S("emoticons.") + name);

					String url = context->getPage()->getSkin()->getImageUrl(_S("emoticons/")+name+_S(".gif"));
					String out = String::format(_S(" <img src=\"%S\" alt=\"%S\" title=\"%S\"> ").c_str(), url.c_str(), code.c_str(), title.c_str());

					// Se prima o dopo non c' niente, o c' un carattere :
					String codeRegexQuote = regex_quote(code);
					// RegEx presa da Invision
					// $txt = preg_replace( "!(?<=[^\w&;/])$code(?=.\W|\W.|\W$)!ei", "\$this->convert_emoticon('$code', '$image')", $txt );
					//String re = _S("!(?<=[^\\w&;/])") + codeRegexQuote + _S("(?=.\\W|\\W.|\\W$)!ei");
					//String re = _S("( ")+codeRegexQuote+_S(" |^")+codeRegexQuote+_S(" | ")+codeRegexQuote+_S("$|^")+codeRegexQuote+_S("$)");

					// Non sono riuscito a far e il detect dei limiti della stringa, per cui aggiungo due spazi prima e trimmo dopo.
					value = _S(" ") + value + _S(" ");

					//String charsEmoBoundary = _S("\\s\\n\\r:),;");
					//String re = String::format(_S("(?<=[%S])%S(?=[%S])").c_str(), charsEmoBoundary.c_str(), codeRegexQuote.c_str(), charsEmoBoundary.c_str());
					String re = String::format(_S("(?<=[\\s])%S(?=[\\s])").c_str(), codeRegexQuote.c_str());

					// URGENT: da ottimizzare con una
					// shared_ptr<boost::wregex> re = regex_create(regex, nocase);
					value = regex_replace(value, RegexManager::instance()->getRegex(re, true), out);

					// Tolgo i due spazi messi sopra
					value = value.substr(1,value.length()-2);
				}
			}
		}
	}
	
	// Encoding dei \n
	if ( (context->getMode() == omlRenderModeSearch) || (context->getRow()) )
	{
		value.replace_all(_S("\n"), _S(" "));
	}
	else
	{
		if(preserveStartLineSpaces)
		{
			for(;;)
			{
				String::size_type posCR = value.find(_S("\n"));
				if(posCR == String::npos)
					break;

				String::size_type posFirstNoSpace = value.find_first_not_of(_S(" "), posCR+1);
				if(posFirstNoSpace == String::npos)
					posFirstNoSpace = value.length();
				String startLineText = value.substr(posCR+1, posFirstNoSpace - (posCR+1));
				startLineText.replace_all(_S(" "),_S("&nbsp;"));
				value = value.substr(0,posCR) + _S("<br>") + startLineText + value.substr(posFirstNoSpace);
			}
		}
		else if(convertCR)
		{
			value.replace_all(_S("\n"), _S("<br />"));
		}
	}

	// Ulteriore encoding finale di sicurezza XSS, trasformo i '\' in '&#x5C;'.
	value.replace_all(_S("\\"),_S("&#x5C;"));

	return value;
}

String IOMLCode::encodeResourceUrl(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& href)
{
	String entityID;
	return encodeResourceUrlEx(context,i,href,entityID);
}

// Se è un url di un'immagine interna, valorizza in objectID l'oggetto a cui si riferisce.
String IOMLCode::encodeResourceUrlEx(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& href, String& entityID)
{
	if(context->getMode() == omlRenderModeSearch) // 0.15
		return href;

	String src = cleanUrl(context, href);

	OsirisLink osirisLink(src.to_ascii());
	if(osirisLink.isValid())
	{
		if(osirisLink.getType() == OsirisLink::linkResource)
		{
			src = context->getPage()->getSkin()->getResourceUrl(osirisLink.getParam("path"));
			if(src.empty())
			{
				src = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/img_missing.gif"));
			}
			if(context->getMode() == omlRenderModeExternal)
				src = IdeSystem::instance()->getLocalUrl(src);
		}
		else if(osirisLink.getType() == OsirisLink::linkFile)
		{
			shared_ptr<const IPortalPage> page = context->getPortalPage();
			if(page != null)
			{
				String id = osirisLink.getParam("id");
				shared_ptr<EntitiesEntity> entity = page->getPortal()->getEntity(page->getDatabase(), EntityID(id.to_ascii()));

				shared_ptr<ObjectsIObject> current = entity ? entity->getCurrent() : null;

				if( (current != null) && (current->getObjectType() == portalObjectTypeFile) )
				{
					src = page->getPortal()->getFileLink(current->id);					
					entityID = id.to_ascii();
				}
				else
				{
					src = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/img_unknown.gif"));					
				}

				if(context->getMode() == omlRenderModeExternal)
					src = IdeSystem::instance()->getLocalUrl(src);
			}
		}
	}
	else
	{
		String protocol = extractProtocol(src);
		if( (protocol == _S("http")) || (protocol == _S("https")) )
		{
			if(context->getMode() == omlRenderModeExternal)
			{			
			}
			else
			{
				// Check se sono abilitati i link esterni
				if((context->getSecure()) || (!bool(context->getPage()->getOption(Options::privacy_options::allow_external_images))))
				{
					context->setSecureCheck(false);
					src = context->getPage()->getSkin()->getResourceUrl(_S("images/oml/img_notallowed.gif"));				
				}
			}
		}		
		else if(protocol == String::EMPTY)
		{
			// Osiris 0.13
			// If protocol can't be determined, it's a relative url. Relative url are accepted.
			if(context->getMode() == omlRenderModeExternal)
				src = IdeSystem::instance()->getLocalUrl(src);

			return src;
		}
		else
		{
			context->addWarningInvalidUrl(i, src);
			src = String::EMPTY;
		}
	}
	return src;
}

//String IOMLCode::encodeUrl(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& href, OMLRenderUrlDestination& destination, bool onlyExternalWithoutConfirm)
String IOMLCode::encodeUrl(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& href, OMLRenderUrlDestination destination, bool onlyExternalWithoutConfirm)
{
	if(context->getMode() == omlRenderModeSearch) // 0.15
		return href;	

	shared_ptr<const IPortalPage> portalPage = context->getPortalPage();

	bool redirect = false;
	bool confirm = false;
	//String confirmMessage;
	String out = cleanUrl(context, href);
	
	OsirisLink osirisLink(out.to_ascii());
	if(osirisLink.isValid())	
	{
		if(osirisLink.getType() == OsirisLink::linkPortal)
		{
			destination = omlRenderUrlDestinationSame;
		}
		else if(osirisLink.getType() == OsirisLink::linkIsis)
		{
			destination = omlRenderUrlDestinationSame;
		}
		else if(osirisLink.getType() == OsirisLink::linkUrl)
		{
			if(onlyExternalWithoutConfirm)
				return String::EMPTY;

			if(context->checkPortalPageAvailable(i) == false)
				return String::EMPTY;
						
			// Internal page url
			String urlPage = osirisLink.getParam("url");

			if(urlPage.substr(0,1) != _S("/"))
				urlPage = _S("/") + urlPage;
						
			if( (urlPage.substr(0,9) == _S("/portals/")) )
			{		
				if(urlPage.find(_S("?")) == String::npos)
					urlPage += _S("?");
				else
					urlPage += _S("&");
					
				std::string pov;
				if(osirisLink.hasParam("portal"))
					pov = osirisLink.getPortal().getString();
				else
					pov = portalPage->getPortal()->getPortalID().getString();
								
				// Portal param maybe a Portal or a POV.
				if(pov.length() == 40) // If is a Portal, convert to POV.
				{
					PortalID portalID(pov);
					if(portalID == portalPage->getPortal()->getPortalID())
						pov = Portal::generatePovID(portalID, portalPage->getPortal()->getPovID());
					else						
					{
						shared_ptr<Portal> externalPortal = PortalsSystem::instance()->getFirstPortal(portalID);

						if(externalPortal)
							pov = Portal::generatePovID(portalID, externalPortal->getPovID());							
						else
						{
							// We don't have the requested portal.
							return String::EMPTY;
						}
					}
				}
				else
				{
					shared_ptr<Portal> externalPortal = PortalsSystem::instance()->getPortalByFullPov(pov);
					if(externalPortal == null)
					{
						// We don't have the requested portal.
						return String::EMPTY;
					}
				}				
					
				urlPage += _S("portal=") + pov;				
					
				/*
				// Normally the "portal" param is mandatory
				PortalID portalID = portalPage->getPortal()->getPortalID();
				if(osirisLink.hasParam("portal"))
					portalID = osirisLink.getPortal();

				// Normally the "user" param is absent. Because we want to avoid that changing POV will break urls.				
				urlPage += _S("&user=");
				if(osirisLink.hasParam("user"))
					urlPage += osirisLink.getUser().getString();
				else
				{
					// If the link is internal to the current portal, use the current user.
					if(portalID == portalPage->getPortal()->getPortalID())
						urlPage += portalPage->getPortal()->getUserID().getString();
					else
					{
						shared_ptr<Portal> externalPortal = PortalsSystem::instance()->getFirstPortal(portalID);
						if(externalPortal)
							urlPage += externalPortal->getUserID().getString();
						else
						{
							// We don't have the requested portal.
							return String::EMPTY;
						}
					}
				}
				*/
				
				out = urlPage;
				//out = String::format(_S("%s").c_str(), url.toString());
			}
			else
			{
				// Main url
				//out = String::format(_S("%S").c_str(), urlPage.c_str());
				out = urlPage;
			}
		}
		else if(osirisLink.getType() == OsirisLink::linkFile)
		{
			if(onlyExternalWithoutConfirm)
				return String::EMPTY;

			if(context->checkPortalPageAvailable(i) == false)
				return String::EMPTY;
						
			// Internal file url			
			out = portalPage->getPortal()->getFileLink(ObjectID(String(osirisLink.getParam("id")).to_ascii()));
		}
		else
			context->addWarningInvalidUrl(i, out);

		if(context->getMode() == omlRenderModeExternal)
			out = IdeSystem::instance()->getLocalUrl(out);
		
		/*
		StringVector params;
		utils::split(out,_S("|"), params);
		if(params.size()<2)
		{
			context->addWarningInvalidUrl(i, out);
			return String::EMPTY;
		}

		if(params[1] == _S("portal"))
		{
			destination = omlRenderUrlDestinationSame;
		}
		else if(params[1] == _S("isis"))
		{
			destination = omlRenderUrlDestinationSame;
		}
		else if(params[1] == _S("url"))
		{
			if(onlyExternalWithoutConfirm)
				return String::EMPTY;
			
			if(params.size()<3)
			{
				context->addWarningInvalidUrl(i, out);
				return String::EMPTY;
			}

			// Internal page url
			String urlPage = params[2];
			String urlParams = (params.size() == 3) ? String::EMPTY : params[3];

			if(urlPage.substr(0,1) != _S("/"))
				urlPage = _S("/") + urlPage;

			if( (urlPage.substr(0,9) == _S("/portals/")) )
			{				
				// Portal url
				if(urlParams.find(OS_URL_PARAM_PORTAL + String(_S("="))) == String::npos)
				{
					if(context->checkPortalPageAvailable(i) == false)
						return String::EMPTY;

					if(!urlParams.empty())
						urlParams += _S("&");

					//urlParams += String::format(_S("%S=%S").c_str(), OS_URL_PARAM_PORTAL.c_str(),portalPage->getLoggedUser()->getPortal()->getID().toString().c_str());
					urlParams.append(OS_URL_PARAM_PORTAL);
					urlParams.append("=");
					urlParams.append(portalPage->getLoggedUser()->getPortal()->getPortalID().getString());
					urlParams.append("&");
					urlParams.append(OS_URL_PARAM_USER);
					urlParams.append("=");
					urlParams.append(portalPage->getLoggedUser()->getPortal()->getUserID().getString());
				}
				out = String::format(_S("%S?%S").c_str(), urlPage.c_str(),urlParams.c_str());
			}
			else
			{
				// Main url
				out = String::format(_S("%S?%S").c_str(), urlPage.c_str(),urlParams.c_str());
			}
		}
		else if(params[1] == _S("file"))
		{
			if(onlyExternalWithoutConfirm)
				return String::EMPTY;

			if(context->checkPortalPageAvailable(i) == false)
				return String::EMPTY;

			if(params.size()<3)
			{
				context->addWarningInvalidUrl(i, out);
				return String::EMPTY;
			}

			// Internal file url
			String oId = params[2];
			out = portalPage->getLoggedUser()->getPortal()->getFileLink(ObjectID(oId.to_ascii()));
		}

		if(context->getMode() == omlRenderModeExternal)
			out = IdeSystem::instance()->getLocalUrl(out);
		*/
	}
	else
	{
		String protocol = extractProtocol(out);
		if(protocol == String::EMPTY)
		{
			// Osiris 0.13
			// If protocol can't be determined, it's a relative url. Relative url are accepted.		

			if(context->getMode() == omlRenderModeExternal)
				out = IdeSystem::instance()->getLocalUrl(out);
		}
		else
		{
			if(context->getMode() == omlRenderModeExternal)
			{
				redirect = false;
				confirm = false;
			}
			else
			{			
				// Se non sono abilitati i link esterni
				if( (context->getSecure()) || (!bool(context->getPage()->getOption(Options::privacy_options::allow_external_urls))) )
				{
					context->setSecureCheck(false);
					return String::EMPTY;
				}
				else
				{
					if(onlyExternalWithoutConfirm)
					{
						String protocolsCollectOption = context->getPage()->getOption(Options::url_options::protocols_collect);
						if(protocolsCollectOption.empty())
						{
							return String::EMPTY;
						}
						else if(protocolsCollectOption == _S("*"))
						{
							redirect = false;
							confirm = false;
						}
						else if(regex_match(protocol, RegexManager::instance()->getRegex(_S("\\b(") + protocolsCollectOption + _S(")"), true)))
						{
							redirect = false;
							confirm = false;
						}
						else if(regex_match(out, RegexManager::instance()->getRegex(_S("\\b(") + protocolsCollectOption + _S(")\\b"), true)))
						{
							redirect = false;
							confirm = false;
						}
						else
						{
							return String::EMPTY;
						}
					}
					else
					{
						// Se il protocollo richiede una redirect
						// 0.12: Viene testato non solo il protocollo, ma anche l'url. Per ora, per supportare OffSystem.
						// Solo l'inizio dell'url, per evitare conflitti.
						String protocolsNoRedirectOption = context->getPage()->getOption(Options::url_options::protocols_no_redirect);
						if(protocolsNoRedirectOption.empty())
						{
							redirect = true;
						}
						else if(protocolsNoRedirectOption == _S("*"))
						{
							redirect = false;
							destination = omlRenderUrlDestinationSame;
						}
						else if(regex_match(protocol, RegexManager::instance()->getRegex(_S("\\b(") + protocolsNoRedirectOption + _S(")\\b"), true)))
						{
							redirect = false;
							destination = omlRenderUrlDestinationSame;
						}
						else if(regex_match(out, RegexManager::instance()->getRegex(_S("\\b(") + protocolsNoRedirectOption + _S(")"), true)))
						{
							redirect = false;
							destination = omlRenderUrlDestinationSame;
						}
						else
						{
							redirect = true;
						}

						// Se il protocollo richiede una conferma
						// 0.12: Viene testato non solo il protocollo, ma anche l'url. Per ora, per supportare OffSystem.
						// Solo l'inizio dell'url, per evitare conflitti.
						String protocolsNoConfirmOption = context->getPage()->getOption(Options::url_options::protocols_no_confirm);
						if(protocolsNoConfirmOption.empty())
						{
							confirm = true;
						}
						else if(protocolsNoConfirmOption == _S("*"))
						{
							confirm = false;
						}
						else if(regex_match(protocol, RegexManager::instance()->getRegex(_S("\\b(") + protocolsNoConfirmOption + _S(")"), true)))
						{
							confirm = false;
						}
						else if(regex_match(out, RegexManager::instance()->getRegex(_S("\\b(") + protocolsNoConfirmOption + _S(")\\b"), true)))
						{
							confirm = false;
						}
						else
						{
							confirm = true;
						}

					}
				}
			}
		}
	}

	// Encoding html
	//out = HttpParser::urlEncode(out);
	out = encode(context, out);

	// Basato su IPB 2.1, converto i '\' in '#092'
	// Questo perchè in un url se scrivo "\0040" viene interpretato quel carattere unicode.
	out.replace_all(_S("\\"), _S("#092"));

	if( (redirect) || (confirm) )
	{
		// Added 0.12
		out = String(context->getPage()->getOption(Options::web_options::redirect_url)).replace_all(_S("#url#"), out);

		out = PortalsSystem::instance()->getExternalLink(out.to_ascii(), confirm);
	}
	/*
	confirmMessage = encodeJavascriptString(confirmMessage);

	if((inNewWindow) || (!confirmMessage.empty()))
		out = String::format(_S("javascript:osRedirect('%S','%S',%d);").c_str(), out.c_str(),confirmMessage.c_str(),inNewWindow);
	*/

	return out;
}

String IOMLCode::encodeOML(shared_ptr<OMLContext> context, const String& text)
{
	String html = _S("{oml_error}");

	if(context->incrementAndCheckRecursive())
	{
		shared_ptr<OMLItem> root = shared_ptr<OMLItem>(OS_NEW OMLItem());
		context->generateItems(root, text.to_wide());
		html = root->getHtmlChilds(context);
	}

	context->decrementRecursive();
	
	return html;
}

bool IOMLCode::allowRowMode() const
{
	return false;
}

String IOMLCode::processOsml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const
{
	String params;
	for(StringCollection<String>::iterator i3 = i->getParams().begin(); i3 != i->getParams().end(); ++i3)
	{
		String paramName = i3->first;
		String paramValue = *i3->second;

		params += _S(" ") + paramName + _S("=\"") + paramValue + _S("\"");
	}

	String childs = i->getOsmlChilds(context);

	String output = _S("[")+getTag()+params;

	if(childs.empty())
	{
		output += _S(" /]");
	}
	else
	{
		output += _S("]") + childs + _S("[/")+getTag()+_S("]");
	}
	return output;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
