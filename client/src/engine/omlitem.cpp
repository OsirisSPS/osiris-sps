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
#include "omlitem.h"

#include "algorithms.h"
#include "charset.h"
#include "conversions.h"
#include "htmlparser.h"
#include "log.h"
#include "omlcontext.h"
#include "omlmanager.h"
#include "regexmanager.h"


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String OMLItem::ITEM_TAG_TEXT = _S("text");
const String OMLItem::ITEM_TAG_PARAM = _S("?");
//const String OMLItem::ITEM_TAG_NULL = _S("nullptr");
const String OMLItem::ITEM_PARAM_TEXT = _S("text");
const String OMLItem::ITEM_PARAM_DEFAULT = _S("default");

//////////////////////////////////////////////////////////////////////

OMLItem::OMLItem()
{
	m_skipped = false;
	m_postProcess = true;
	m_directClose = false;
}

OMLItem::~OMLItem()
{

}

/*
const bool OMLItem::getRequiredClose() const
{
	// Forzate qui, ma cmq sono di per s un eccezzione...
	if( (m_tagName == _S("img")) && (hasParams()) ) return false;

	if( (m_tagName == _S("abr")) ||
		(m_tagName == _S("br")) ||
		(m_tagName == _S("hr")) )
	{
		return false;
	}
	else
	{
		return true;
	}
}
*/

/*
const bool OMLItem::getNoParse() const
{
	if(!isTag()) return false;

	if( (m_tagName == _S("noparse")) || (m_tagName == _S("code")) || (m_tagName == _S("codebox")) )
		return true;

	return false;
}
*/

void OMLItem::setParam(const String &name, const String& value)
{
	String val = value;
	val.replace_all(_S("\\\""), _S("\""));
	
	m_params.set(name, val);
}

void OMLItem::removeParam(const String &name)
{
	m_params.remove(name);
}

void OMLItem::add(shared_ptr<OMLItem> i)
{
	// Qui compatto + tag "text" consecutivi. Soprattutto per fare che un
	// http://www.alfa[beta]gamma.com venga rappresentato in un'unico 'item', per far s che le regex lo individuino.
	if(i->getTagName() == OMLItem::ITEM_TAG_TEXT)
	{
		if( (m_lastAdded) && (m_lastAdded->getTagName() == OMLItem::ITEM_TAG_TEXT) )
		{
			m_lastAdded->setParam(OMLItem::ITEM_PARAM_TEXT, m_lastAdded->getParam(OMLItem::ITEM_PARAM_TEXT) + i->getParam(OMLItem::ITEM_PARAM_TEXT));
			return;
		}
	}

	i->m_parent = get_this_ptr();
	m_childs.push_back(i);
	m_lastAdded = i;
}

/*
void OMLItem::clear()
{
	setSkipped(true);
	//m_tagName = OMLItem::ITEM_TAG_NULL;
	//m_params.clear();
	//m_childs.clear();
}
*/

String OMLItem::getSingleText()
{
	// Alcuni tag dovrebbero contenere solo un figlio di testo nel body, quali il [code], il [noparse] e il [?].
	if(getChilds().size() == 1)
	{
		shared_ptr<OMLItem> text = getChilds().front();

		//Assert sotto commentato, un caso come
		//[img][center]alfa[/center][/img]
		//finisce qui con text->getTagName() == "center".
		//OS_ASSERT(text->getTagName() == OMLItem::ITEM_TAG_TEXT);
		
		if(text->getTagName() == OMLItem::ITEM_TAG_TEXT)
		{
			return text->getParam(OMLItem::ITEM_PARAM_TEXT);
		}
	}

	return String::EMPTY;
}

String OMLItem::getHtml(shared_ptr<OMLContext> context)
{
	// Chiedo al Manager di convertilo.
	return OMLManager::instance()->processItemForHtml(get_this_ptr(), context);
}

String OMLItem::getOsml(shared_ptr<OMLContext> context)
{
	return OMLManager::instance()->processItemForOsml(get_this_ptr(), context);
}

String OMLItem::getHtmlChildsPlainWithAttributes(shared_ptr<OMLContext> context)
{
	String output = getHtmlChilds(context);
	String attributes;

	for(StringCollection<String>::iterator i3 = getParams().begin(); i3 != getParams().end(); ++i3)
	{
		String paramName = i3->first;
		String paramValue = *i3->second;

		output += _S(" ") + paramValue;
	}

	return _S(" ") + output + _S(" ");
}

String OMLItem::getHtmlChilds(shared_ptr<OMLContext> context)
{
	String output;

	for(Items::const_iterator i2 = m_childs.begin(); i2 != m_childs.end(); ++i2)
	{
		shared_ptr<OMLItem> child = *i2;
		String childText = child->getHtml(context);
		output += childText;
	}

	return output;
}

String OMLItem::getOsmlChilds(shared_ptr<OMLContext> context)
{
	String output;

	for(Items::const_iterator i2 = m_childs.begin(); i2 != m_childs.end(); ++i2)
	{
		shared_ptr<OMLItem> child = *i2;
		String childText = child->getOsml(context);
		output += childText;
	}

	return output;
}

std::wstring::size_type OMLItem::extract(const std::wstring& text, const std::wstring& textLower, std::wstring::size_type posStart, std::wstring::size_type posEnd)
{
	std::wstring::size_type pos = extract2(text, textLower, posStart, posEnd);

	// Gestione del 'noparse'.
	// Gestione dei tag ad hoc 'code', 'parse', '?', e 'url'.
	// da sistemare...
	if(!m_directClose)
	{
		bool noparse = false;

		if(m_tagName == _S("noparse")) noparse = true;
		if(m_tagName == _S("code")) noparse = true;
		if(m_tagName == _S("codebox")) noparse = true;
		if(m_tagName == _S("?")) noparse = true;
		if( (m_tagName == _S("url")) && (!hasParam(OMLItem::ITEM_PARAM_DEFAULT)) && (!hasParam(_S("href")))) noparse = true;
		if( (m_tagName == _S("iurl")) && (!hasParam(OMLItem::ITEM_PARAM_DEFAULT)) && (!hasParam(_S("href")))) noparse = true;

		if(noparse)
		{
			// Cerco direttamente la chiusura, e considero come testo tutto quello tra l'apertura e la chiusura del tag.
			std::wstring::size_type posEnd = textLower.find(_W("[/") + getTagName().to_wide() + _W("]"), pos);
			if(posEnd == std::wstring::npos)
				return posEnd;

			//String insideText = text.substr(pos,posEnd-pos);
			shared_ptr<OMLItem> t = shared_ptr<OMLItem>(OS_NEW OMLItem());
			t->extract(text, textLower, pos, posEnd);
			add(t);
			pos = posEnd;
		}
	}

	return pos;
}

std::wstring::size_type OMLItem::extract2(const std::wstring& text, const std::wstring& textLower, String::size_type posStart, String::size_type posEnd)
{
	// NB. questo caso:  [code beta="alfa"gamma]
	// Qui questo parser non d errore, bens il parametro varr {'alfa'gamma}.
	// Solo se completamente circondato da doppi apici da entrambi i lati, vengono rimossi dal valore.

	// Se  un tag bbCode e non un text
	//if(text.substr(startPos,1) == _S("["))
	if(posEnd == std::wstring::npos)
	{
		// Sintassi parametri:
		// V1:	[img=myurl]text[/img]
		//		- first = myurl
		// V2:  [img alfa='beta' gamma='delta']text[/img]
		//		- alfa = beta
		//		- gamma = delta
		// v2:  [b alfa='[b]beta[/b]' beta]ciccio[/b]
		//      [b=[b]beta[/b]]ciccio[/b]
		//		[i alfa='[b f=\'Ciccio\']']gatto[/i]

		//String::size_type posA = Context::findOneOf(text, _S(" =/]"), posStart);
		std::wstring::size_type posA = OMLContext::findTokens(textLower, _W(" ;=;/];]"), posStart);
		if(posA == std::wstring::npos)
		{
			// Fatal error!
			return std::wstring::npos;
		}

		m_tagName = textLower.substr(posStart+1, posA-posStart-1).c_str();
		//m_tagName.to_lower();
		//m_text = String::EMPTY;

		/*
		if(regex_match(m_tagName, RegexManager::instance()->getRegex(_S("[0-9A-F]*"), false)) == false)
		{
			// Fatal error!
			return std::wstring::npos;
		}
		*/

		if( (textLower[posA] == chCloseSquare) || (textLower[posA] == chForwardSlash) )
		{
			// Chiusura diretta, ex. "[alfa]"
			if(textLower[posA] == chForwardSlash) posA++;
			_extractCheckClosed(textLower, posA);
			return posA+1;
		}

		std::wstring paramName;
		if(textLower[posA] == chEqual)
		{
			paramName = OMLItem::ITEM_PARAM_DEFAULT.to_wide();
		}
		std::wstring::size_type posCurrent = posA+1;
		for(;;)
		{
			std::wstring::size_type posB;

			if(paramName.empty())
			{
				posB = OMLContext::findTokens(textLower, _W("=;/];]"), posCurrent);
			}
			else
			{
				// Eccezione (Issue:10)
				if(m_tagName == _S("url"))
				{
					posB = OMLContext::findTokens(textLower, _W(" ;]"), posCurrent);
				}
				else
				{
					posB = OMLContext::findTokens(textLower, _W(" ;/];]"), posCurrent);
				}
			}

			if(posB == std::wstring::npos)
			{
				// Fatal error!
				return String::npos;
			}

			std::wstring textCurrent = text.substr(posCurrent, posB-posCurrent);
			if( (textLower[posB] == chCloseSquare) || (textLower[posB] == chForwardSlash) )
			{
				// End!
				_extractSetParam(paramName, textCurrent);

				if(text[posB] == chForwardSlash) posB++;
				_extractCheckClosed(textLower, posB);
				return posB+1;
			}
			else if(textLower[posB] == chSpace)
			{
				// Nuovo parametro
				_extractSetParam(paramName, textCurrent.c_str());
				paramName.clear();
			}
			else if(text[posB] == chEqual)
			{
				paramName = textCurrent;
			}
			posCurrent = posB+1;
		}

		return std::wstring::npos;
	}
	else
	{
		m_tagName = OMLItem::ITEM_TAG_TEXT;
		setParam(OMLItem::ITEM_PARAM_TEXT, text.substr(posStart, posEnd - posStart).c_str());

		m_directClose = true;
		return posEnd;
	}
}

void OMLItem::_extractSetParam(std::wstring name, std::wstring value)
{
	// Chiamata per settare parametri estratti dalla 'extract', diversa dalla ::setParam.
	// Ad esempio fa il trim del valore, da non fare se è arrivato da un tag [?].

	algorithms::trim(name);
	algorithms::trim(value);

	// Rimuove gli apici agli estremi, se presenti
	if( (value.length()>=2) && (value[0]==chDoubleQuote) && (value[value.length()-1]==chDoubleQuote) )
		value = value.substr(1, value.length()-2);

	if( (!name.empty()) && (!value.empty()) )
		setParam(name.c_str(), value.c_str());
}

void OMLItem::_extractCheckClosed(const std::wstring& text, std::wstring::size_type posClose)
{
	// Se il tag [img] non ha il parametro 'src', mi aspetto una chiusura, poich il suo body sar il parametro 'src'.
	if( ( (m_tagName == _S("img")) && (hasParam(_S("src"))) ) ||
		(m_tagName == _S("br")) ||
		(m_tagName == _S("hr")) )
	{
		m_directClose = true;
	}
	else
	{
		// Alcuni tag non sono chiudibili direttamente.
		// Copre anche l'eccezione (Issue:10)
		if( (m_tagName == _S("url")) )
		{			
			m_directClose = false;
		}
		else
		{
			m_directClose = ( (posClose>0) && (text[posClose-1] == chForwardSlash) );
		}
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
