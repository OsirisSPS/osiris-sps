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
#include "omlcontext.h"

#include "algorithms.h"
#include "charset.h"
#include "conversions.h"
#include "htmlparser.h"
#include "iomlcode.h"
#include "omlmanager.h"
#include "omlitem.h"
#include "iidepage.h"
#include "iideportalpage.h"
#include "ideskin.h"
#include "log.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OMLContext::OMLContext(shared_ptr<IPage> page, bool enableWarning, bool full, bool secure, bool row, OMLRenderMode mode, const String &refID, const String &fullUrl)
{
	//OS_ASSERT(!( (plain == false) && (page == null) ));

	static uint32 currentID = 0;
	currentID++;
	m_id = currentID;
	m_subid = 0;

	m_page = page;
	m_enableWarning = enableWarning;

	m_recursiveCounter = 0;
	m_recursiveLimit = 10;

	m_full = full;
	m_secure = secure;
	m_row = row;
	m_mode = mode;
	m_refID = refID;
	m_fullUrl = fullUrl;

	m_secureCheck = true;

	m_showMore = false;
}

OMLContext::~OMLContext()
{

}

const String OMLContext::getContextID() const
{
	return String::format(_S("%u").c_str(), m_id);
}

const String OMLContext::getContextNextID()
{
	m_subid++;
	return String::format(_S("%u_%u").c_str(), m_id, m_subid);
}

shared_ptr<IPage> OMLContext::getPage() const
{
	return m_page;
}

shared_ptr<IPortalPage> OMLContext::getPortalPage() const
{
	return boost::dynamic_pointer_cast<IPortalPage>(getPage());
}

void OMLContext::parseOml(const String& bbText)
{	
	try
	{
		m_root = shared_ptr<OMLItem>(OS_NEW OMLItem());
		generateItems(m_root, bbText.to_wide());

		m_output = m_root->getHtmlChilds(get_this_ptr());
		
		if( (!getRow()) && (getMode() == omlRenderModeOsiris) )
		{
			m_output = _S("<div class='os_oml'>") + m_output + _S("</div>");

			m_output += _S("<div class=\"os_oml_footer\">");
			if( (getFull() == false) && (getShowMore() == true) && (getFullUrl().empty() == false) )
			{
				m_output += _S("<div class=\"os_oml_actions\">");
				m_output += _S("<a href=\"") + getFullUrl() + _S("\"><img src=\"") + getPage()->getSkin()->getResourceUrl(_S("images/oml/show_more.png")) + _S("\" title=\"") + getPage()->getText(_S("oml.show_more")) + _S("\" />") + getPage()->getText(_S("oml.show_more")) + _S("</a>");
				m_output += _S("</div>");
			}
			else if( (getFull() == true) )
			{	
				String scriptUrls;
				String objScriptId = _S("document.osmlUrls_") + getContextID();

				int iUrl = 0;
				for(OMLContext::Urls::const_iterator i = getUrls().begin(); i != getUrls().end(); ++i)
				{
					scriptUrls += objScriptId + _S(".a[") + conversions::to_utf16(iUrl) + _S("] = \"") + *i + _S("\";\r\n");
					iUrl++;
				}

				//if(iUrl !=0) // 0.14
				if(iUrl > 1)
				{
					m_output += _S("<div class=\"os_oml_actions\">");

					
					m_output += _S("<script type=\"text/javascript\">\r\n");
					m_output += objScriptId + _S(" = new Object();\r\n");
					m_output += objScriptId + _S(".id = \"") + getContextID() + _S("\";\r\n");
					m_output += objScriptId + _S(".a = new Array();\r\n");
					m_output += objScriptId + _S(".n = 0;\r\n");
					m_output += objScriptId + _S(".iv = false;\r\n");
					m_output += scriptUrls;					
					m_output += _S("</script>");
					m_output += _S("<a href=\"javascript:osOmlUrlsLaunchAll(") + objScriptId + _S(")\"><img src=\"") + getPage()->getSkin()->getResourceUrl(_S("images/oml/urls_launch.png")) + _S("\" title=\"") + getPage()->getText(_S("oml.urls_launch")) + _S("\" /></a>");
					m_output += _S("<a href=\"javascript:osOmlUrlsPopup(") + objScriptId + _S(")\"> <img src=\"") + getPage()->getSkin()->getResourceUrl(_S("images/oml/urls_popup.png")) + _S("\" title=\"") + getPage()->getText(_S("oml.urls_popup")) + _S("\" /></a>");

					m_output += _S("</div>");
				}
			}
			m_output += _S("</div>");
		}
	}
	catch(std::exception &e)
	{
		addWarning(e.what());		
    }
    catch(...) // TOFIXHIGH, don't work?
    {
		addWarning(_S("Internal error."));		
    }
}

void OMLContext::parseCss(const String& cssText, bool encode)
{
	m_output = IOMLCode::encodeToCss(get_this_ptr(), null, cssText);
	if(encode)
		m_output = HtmlParser::instance()->encode(m_output);
}

void OMLContext::generateItems(shared_ptr<OMLItem> root, const std::wstring& text)
{
	bool stopWithError = OSMLCODES_OPTIONS_STOPWITHERROR;
	
#ifdef OSMLCODES_OPTIONS_NEWENGINE
	std::wstring textLower = text;
	//textLower.to_lower();
	algorithms::to_lower(textLower);

	String::size_type curPos = 0;
	shared_ptr<OMLItem> current = root;
	for(;;)
	{
		std::wstring::size_type posStart = findNotEscaped(textLower, _W("["),curPos);
		std::wstring::size_type posEnd = posStart;
		if(posStart != std::wstring::npos)
		{
			// Tag trovato

			// Creo un BBCode del testo prima
			if(posStart != curPos)
			{
				shared_ptr<OMLItem> t = shared_ptr<OMLItem>(OS_NEW OMLItem());
				posEnd = t->extract(text, textLower, curPos, posStart);
				if(posEnd == std::wstring::npos)
				{
					addWarningSyntax();
					if(stopWithError) break;
				}
				current->add(t);
				curPos = posStart;
			}

			bool process = false;

			// ho [a] o [a=12,23] o [a style="center"] o [/a]
			// se inizia per '[/'  una chiusura
			if(textLower.substr(posStart+1,1) == _W("/"))
			{
				//  una chiusura
				posEnd = findNotEscaped(textLower, _W("]"), posStart+1);
				if(posEnd != String::npos)
				{
					String tagName = textLower.substr(posStart+2,posEnd-posStart-2).c_str();

					// Caso speciale ad hoc: se  un [/list], e il corrente  un [*], allora chiudo il precedente.
					if( (current != null) && (current->getTagName() == _S("*")) && (tagName == _S("list")) )
					{
						current = closeItem(current);
					}

					if(tagName.empty())
					{
						// errore, tag vuoto.
						addWarning(_S("Empty tag."));
						if(stopWithError) break;
					}
					else if( (current == NULL) || (current->getTagName() != tagName) )
					{
						// errore, ex. "alfa [a] beta [/b]";
						addWarning(String::format(_S("Tag [%S] not opened.").c_str(), tagName.c_str()));
						if(stopWithError) break;
					}
					else if(!OMLManager::instance()->isKnownTag(tagName))
					{
						addWarning(String::format(_S("Tag [%S] unknown.").c_str(), tagName.c_str()));
						if(stopWithError) break;
					}
					else
					{
						current->setEndPos(posStart);
						current = closeItem(current);

						process = true;
					}
					posEnd++;
				}
				else
				{
					// errore, ex. "alfa [a] beta [/b gamma";
					addWarningSyntax();
					if(stopWithError) break;
				}
			}
			else
			{
				//  un'apertura
				shared_ptr<OMLItem> t = shared_ptr<OMLItem>(OS_NEW OMLItem());
				posEnd = t->extract(text, textLower, posStart);
				if(posEnd == std::wstring::npos)
				{
					addWarningSyntax();
					if(stopWithError) break;
				}
				else if(!OMLManager::instance()->isKnownTag(t->getTagName()))
				{
					addWarning(String::format(_S("Tag [%S] unknown.").c_str(), t->getTagName().c_str()));
					if(stopWithError) break;
				}
				else
				{
					// Caso speciale ad hoc: se  un [*], e il corrente  un [*], allora chiudo il precedente.
					if( (t->getTagName() == _S("*")) && (current != null) && (current->getTagName() == _S("*")) )
					{
						current = closeItem(current);
					}

					// Il tag diventa figlio del corrente
					current->add(t);

					if(t->getDirectClose())
					{
						closeItem(t);
					}
					else
					{
						current = t;
					}

					process = true;
				}
			}

			if( (OSMLCODES_OPTIONS_TAGERRORASTEXT) && (!process) ) // Dumpa i tag errati come testo, versione 0.11
			{				
				shared_ptr<OMLItem> t = shared_ptr<OMLItem>(OS_NEW OMLItem());
				// 0.11				
				/*
				t->extract(text, textLower, curPos, posEnd);
				t->setPostProcess(false); // This to avoid recursive problem in this example: [alfa beta="http://www.google.it" /]
				current->add(t);
				curPos = posEnd;
				*/
				// 0.14

				t->extract(text, textLower, curPos, curPos+1);				
				current->add(t);
				curPos = curPos+1;				
			}
			else
			{
				curPos = posEnd;
			}
		}
		else
		{
			// Fine, ulteriori tag non trovati.
			if(curPos != std::wstring::npos)
			{
				if(curPos != textLower.length())
				{
					shared_ptr<OMLItem> t = shared_ptr<OMLItem>(OS_NEW OMLItem());
					t->extract(text, textLower, curPos, text.length());
					current->add(t);
				}
			}
			else
			{
				// Se finisce qui, c' stato un errore di sintassi lanciato gi dalla extract.
			}
			break;
		}
	}

	while(current != root)
	{
		addWarning(String::format(_S("Tag [%S] not closed.").c_str(), current->getTagName().c_str()));
		current = closeItem(current);
	}
#else
	String::size_type curPos = 0;
	shared_ptr<OMLItem> current = root;
	for(;;)
	{
		String::size_type posStart = findNotEscaped(text, '[',curPos);
		String::size_type posEnd = posStart;
		if(posStart != String::npos)
		{
			// Tag trovato

			// Creo un BBCode del testo prima
			if(posStart != curPos)
			{
				shared_ptr<OMLItem> t = shared_ptr<OMLItem>(OS_NEW OMLItem());
				posEnd = t->extract(text, curPos, posStart);
				if(posEnd == String::npos)
				{
					addWarningSyntax();
					if(stopWithError) break;
				}
				current->add(t);
			}

			// ho [a] o [a=12,23] o [a style="center"] o [/a]
			// se inizia per '[/'  una chiusura
			if(text.substr(posStart+1,1) == _S("/"))
			{
				//  una chiusura
				posEnd = findNotEscaped(text, ']',posStart+1);
				if(posEnd != String::npos)
				{
					String tagName = text.substr(posStart+2,posEnd-posStart-2);
					tagName.to_lower();

					// Caso speciale ad hoc: se  un [/list], e il corrente  un [*], allora chiudo il precedente.
					if( (current != null) && (current->getTagName() == _S("*")) && (tagName == _S("list")) )
					{
						current = closeItem(current);
					}

					if(tagName.empty())
					{
						// errore, tag vuoto.
						addWarning(_S("Empty tag."));
						if(stopWithError) break;
					}
					else if( (current == NULL) || (current->getTagName() != tagName) )
					{
						// errore, ex. "alfa [a] beta [/b]";
						addWarning(String::format(_S("Tag [%S] not opened.").c_str(), tagName.c_str()));
						if(stopWithError) break;
					}
					else
					{
						current->setEndPos(posStart);
						current = closeItem(current);
					}
					posEnd++;
				}
				else
				{
					// errore, ex. "alfa [a] beta [/b gamma";
					addWarningSyntax();
					if(stopWithError) break;
				}
			}
			else
			{
				//  un'apertura
				shared_ptr<OMLItem> t = shared_ptr<OMLItem>(OS_NEW OMLItem());
				posEnd = t->extract(text, posStart);
				if(posEnd == String::npos)
				{
					addWarningSyntax();
					if(stopWithError) break;
				}

				// Caso speciale ad hoc: se  un [*], e il corrente  un [*], allora chiudo il precedente.
				if( (t->getTagName() == _S("*")) && (current != null) && (current->getTagName() == _S("*")) )
				{
					current = closeItem(current);
				}

				// Il tag diventa figlio del corrente
				current->add(t);


				if(t->getDirectClose())
				{
					closeItem(t);
				}
				else
				{
					current = t;

					// Caso speciale ad hoc: se  un [url] e non ha l'href nei parametri, tratto il body in 'noparse'.
				}
			}

			curPos = posEnd;
		}
		else
		{
			// Fine, ulteriori tag non trovati.
			if(curPos != String::npos)
			{
				if(curPos != text.length())
				{
					shared_ptr<OMLItem> t = shared_ptr<OMLItem>(OS_NEW OMLItem());
					t->extract(text, curPos, text.length());
					current->add(t);
				}
			}
			else
			{
				// Se finisce qui, c' stato un errore di sintassi lanciato gi dalla extract.
			}
			break;
		}
	}

	while(current != root)
	{
		addWarning(String::format(_S("Tag [%S] not closed.").c_str(), current->getTagName().c_str()));
		current = closeItem(current);
	}
#endif
}

// Il parser dell'item  concluso, eseguo dei post-processing e restituisco il padre.
shared_ptr<OMLItem> OMLContext::closeItem(shared_ptr<OMLItem> i)
{
	// Conversione dei tag di parametri in parametri
	for(OMLManager::Items::const_iterator i2 = i->getChilds().begin(); i2 != i->getChilds().end(); ++i2)
	{
		shared_ptr<OMLItem> child = *i2;
		if(child->getTagName() == OMLItem::ITEM_TAG_PARAM)
		{
			String paramValue = child->getSingleText();
			String paramName = child->getParam(OMLItem::ITEM_PARAM_DEFAULT);
			if(paramName.empty()) paramName = OMLItem::ITEM_PARAM_DEFAULT;

			// Inverto l'escaping effettuato dalla Manager::TransformNoParse
			paramValue.replace_all(_S("\\["), _S("["));

			i->setParam(paramName, paramValue);
			child->setSkipped(true); 		}
	}

	return i->getParent();
}

void OMLContext::addUrl(shared_ptr<OMLItem> i, const String &url)
{
	// TODO: Evitare duplicati.
	OMLRenderUrlDestination omlRenderUrlDestination = omlRenderUrlDestinationSame;
	String encodedUrl = IOMLCode::encodeUrl(get_this_ptr(), i, url, omlRenderUrlDestination, true);
	if(encodedUrl.empty() == false)
		m_urls.push_back(url);			
}

String OMLContext::getFriendlyName(shared_ptr<OMLItem> i) const
{
	if(i->getTagName() == OMLItem::ITEM_TAG_TEXT)
	{
		String text = i->getParam(OMLItem::ITEM_PARAM_TEXT);
		return _S("Text \"") + text + _S("\"");
	}
	else
	{
		return _S("Tag [") + i->getTagName() + _S("]");
	}
}

void OMLContext::addWarning(const String &msg)
{
	if(!m_enableWarning) return;

	m_warnings.push_back(msg);

	OS_LOG_WARNING(_S("OML: ") + msg);
}

void OMLContext::addWarningSyntax()
{
	if(!m_enableWarning) return;
	addWarning(_S("Syntax error."));
}

void OMLContext::addWarningCantHaveParams(shared_ptr<OMLItem> i)
{
	if(!m_enableWarning) return;
	addWarning(String::format(_S("Tag [%S] cannot have params. Will be ignored.").c_str(), i->getTagName().c_str()));
}

void OMLContext::addWarningParamRequired(shared_ptr<OMLItem> i)
{
	if(!m_enableWarning) return;
	addWarning(String::format(_S("Tag [%S] require a param.").c_str(), i->getTagName().c_str()));
}

void OMLContext::addWarningMustBeInside(shared_ptr<OMLItem> i, const String &parentTag)
{
	if(!m_enableWarning) return;
	addWarning(String::format(_S("%S must be inside of a tag [%S]").c_str(), getFriendlyName(i).c_str(),parentTag.c_str()));
}

void OMLContext::addWarningCantBeHere(shared_ptr<OMLItem> i, shared_ptr<OMLItem> ref)
{
	if(!m_enableWarning) return;
	addWarning(String::format(_S("%S cannot be inside of %S").c_str(), getFriendlyName(i).c_str(), getFriendlyName(ref).c_str()));
}

void OMLContext::addWarningInvalidUrl(shared_ptr<OMLItem> i, const String &url)
{
	if(!m_enableWarning) return;
	addWarning(String::format(_S("Invalid url '%S' in %S").c_str(), url.c_str(), getFriendlyName(i).c_str()));
}

void OMLContext::addWarningInvalidParam(shared_ptr<OMLItem> i, const String &name)
{
	if(!m_enableWarning) return;
	addWarning(String::format(_S("Invalid param '%S' in %S").c_str(), name.c_str(), getFriendlyName(i).c_str()));
}

void OMLContext::addWarningRowModeNotAllowed(shared_ptr<OMLItem> i)
{
	if(!m_enableWarning) return;
	addWarning(String::format(_S("%S not allowed in row mode.").c_str(), getFriendlyName(i).c_str()));
}

bool OMLContext::incrementAndCheckRecursive()
{
	m_recursiveCounter++;
	if(m_recursiveCounter>m_recursiveLimit)
	{
		addWarning(_S("Recursive limit reached. Please check your code."));
		return false;
	}
	else
		return true;
}

void OMLContext::decrementRecursive()
{
	m_recursiveCounter--;
	OS_ASSERT(m_recursiveCounter>=0);
}

bool OMLContext::checkPortalPageAvailable(shared_ptr<OMLItem> i)
{
	if(getPortalPage() == null)
	{
		addWarning(String::format(_S("%S need to be used inside a portal.").c_str(), getFriendlyName(i).c_str()));
		return false;
	}
	else
	{
		return true;
	}
}

String OMLContext::generateReportTree(shared_ptr<OMLItem> i, uint32 depth) const
{
	String out;

	String skipped;
	if(i->getSkipped())
		skipped = _S("[color=red] (skipped)[/color]");

	if(i->getTagName() == OMLItem::ITEM_TAG_TEXT)
	{
		String text = i->getParam(OMLItem::ITEM_PARAM_TEXT);
		text.replace_all(_S("\n"), _S("\\n"));
		out += _S("Text") + skipped + _S(": \"[i][noparse]") + text + _S("[/noparse][/i]\"[br /]");
	}
	else
	{
		if(!i->getTagName().empty())
			out += _S("Tag") + skipped + _S(": [b]") + i->getTagName() + _S("[/b][br /]");

		if(!i->getParams().empty())
		{
			out += _S("[list=a]");
			for(StringCollection<String>::iterator i3 = i->getParams().begin(); i3 != i->getParams().end(); ++i3)
			{
				String paramName = i3->first;
				String paramValue = *i3->second;

				out += _S("[li]");
				out += _S("Param: [b][noparse]") + paramName + _S("[/noparse][/b]");
				//out += String::format(_S("Param name:[b]%S[/b], value: [b]%S[/b]").c_str(), paramName.c_str(), paramValue.c_str());
				out += _S(" Value: [i][noparse]") + paramValue + _S("[/noparse][/i]");
				//out += generateReportTree(*i3, depth+1);
				out += _S("[/li]");
			}
			out += _S("[/list]");
		}

		if(!i->getChilds().empty())
		{
			out += _S("[list=1]");
			for(OMLManager::Items::const_iterator i2 = i->getChilds().begin(); i2 != i->getChilds().end(); ++i2)
			{
				shared_ptr<OMLItem> child = *i2;

				out += _S("[li]");
				out += generateReportTree(child, depth+1);
				out += _S("[/li]");
			}
			out += _S("[/list]");
		}
	}

	return out;
}

String OMLContext::getReport(const String& text, bool detailed) const
{
	String out;
	if(!getWarnings().empty())
	{
		out += _S("[list]");
		for(OMLContext::Warnings::const_iterator i = getWarnings().begin(); i != getWarnings().end(); ++i)
		{
			String msg = *i;
			out += _S("[li][noparse]") + msg + _S("[/noparse][/li]");
		}
		out += _S("[/list]");
	}
	if( (detailed) && (m_root != null) )
	{
		// Tree degli items
		out = out + _S("[whisper=\"Item tree\"]") + generateReportTree(m_root, 0) + _S("[/whisper]");

		// Versione Plain
		out = out + _S("[whisper=\"Texts for search indexes\"][noparse]") + OMLManager::instance()->parse(text.c_str(), getPage(), getFull(), getSecure(), getRow(), omlRenderModeSearch, String::EMPTY, String::EMPTY) + _S("[/noparse][/whisper]");
	}
	if( (detailed) )
	{
		if(!getUrls().empty())
		{
			out += _S("[whisper=\"Urls detected\"]");
			out += _S("[list]");
			for(OMLContext::Urls::const_iterator i = getUrls().begin(); i != getUrls().end(); ++i)
			{
				String msg = *i;
				out += _S("[li][noparse]") + msg + _S("[/noparse][/li]");
			}
			out += _S("[/list]");
			out += _S("[/whisper]");
		}
	}
	return out;
}
/*
String::size_type OMLContext::findNotEscaped(const std::wstring& text, const uchar &what, String::size_type posStart)
{
	String::size_type posCurrent = posStart;
	for(;;)
	{
		String::size_type posA = text.find(what, posCurrent);
		if(posA == String::npos)
			return posA;
		if( (posA>0) && (text[posA-1] == chBackSlash) )
		{
			posCurrent = posA+1;
		}
		else
		{
			return posA;
		}
	}
}
*/
std::wstring::size_type OMLContext::findNotEscaped(const std::wstring& text, const std::wstring& what, String::size_type posStart)
{
	String::size_type posCurrent = posStart;
	for(;;)
	{
		String::size_type posA = text.find(what, posCurrent);
		if(posA == String::npos)
			return posA;
		if( (posA>0) && (text[posA-1] == chBackSlash) )
		{
			posCurrent = posA+1;
		}
		else
		{
			return posA;
		}
	}
}

std::wstring::size_type OMLContext::findToken(const std::wstring& text, const std::wstring& what, String::size_type posStart)
{
	// Ricerca l'occorenza di 'what', a patto che non sia escapizzato e che non sia in un blocco tra apici.
	String::size_type posCurrent = posStart;
	for(;;)
	{
		String::size_type posS = text.find(what, posCurrent);
		if(posS == String::npos)
			return String::npos;

		if( (posS>0) && (text[posS-1] == chBackSlash) )
		{
			posCurrent = posS+1;
		}
		else
		{
			String::size_type posApix = findNotEscaped(text, _W("\""), posCurrent);
			if( (posApix != String::npos) && (posApix<posS) )
			{
				String::size_type posApixEnd = findNotEscaped(text, _W("\""), posApix+1);
				if(posApixEnd == String::npos)
					return String::npos;
				posCurrent=posApixEnd+1;
			}
			else
			{
				return posS;
			}
		}
	}
}

std::wstring::size_type OMLContext::findTokens(const std::wstring& text, const std::wstring& tokens, String::size_type posStart)
{
	// Ricerca la prima occorrenza di una delle stringhe in 'tokens', saltando per le presenze tra doppi apici.
	vector<std::wstring>::type tarray;
	algorithms::split(tokens, tarray, boost::algorithm::is_any_of(";"));

	size_t nTokens = tarray.size();
	String::size_type posMin = String::npos;
	for(size_t iToken=0; iToken<nTokens; iToken++)
	{
		String::size_type posCurrent = posStart;
		String::size_type posChar;
		for(;;)
		{
			posChar = findNotEscaped(text, tarray[iToken], posCurrent);
			if(posChar == String::npos)
				break;
			String::size_type posApix = findNotEscaped(text, _W("\""), posCurrent);
			if( (posApix != String::npos) && (posApix<posChar) )
			{
				String::size_type posApixEnd = findNotEscaped(text, _W("\""), posApix+1);
				if(posApixEnd == String::npos)
				{
					posChar = String::npos;
					break;
				}
				posCurrent=posApixEnd+1;
			}
			else
			{
				break;
			}
		}

		if(posChar != String::npos)
		{
			if( (posMin == String::npos) || (posChar<posMin) )
			{
				posMin = posChar;
			}
		}
	}

	return posMin;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
