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
#include "htmlparser.h"

#include "boost/assign/list_of.hpp"
#include "charset.h"
#include "conversions.h"
#include "unicode.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlParser::HtmlParser()
{
	unordered_map<String, uchar>::type entities = boost::assign::map_list_of(_S("#39"), 146)	// TODO: la codifica XHTML correttà	è "apos" ma su Internet Explorer non funziona...
																			 (_S("quot"), 34)
																			 (_S("amp"), 38)
																			 (_S("lt"), 60)
																			 (_S("gt"), 62)
																			 (_S("AElig"), 198)
																			 (_S("Aacute"), 193)
																			 (_S("Acirc"), 194)
																			 (_S("Agrave"), 192)
																			 (_S("Aring"), 197)
																			 (_S("Atilde"), 195)
																			 (_S("Auml"), 196)
																			 (_S("Ccedil"), 199)
																			 (_S("ETH"), 208)
																			 (_S("Eacute"), 201)
																			 (_S("Ecirc"), 202)
																			 (_S("Egrave"), 200)
																			 (_S("Euml"), 203)
																			 (_S("Iacute"), 205)
																			 (_S("Icirc"), 206)
																			 (_S("Igrave"), 204)
																			 (_S("Iuml"), 207)
																			 (_S("Ntilde"), 209)
																			 (_S("Oacute"), 211)
																			 (_S("Ocirc"), 212)
																			 (_S("Ograve"), 210)
																			 (_S("Oslash"), 216)
																			 (_S("Otilde"), 213)
																			 (_S("Ouml"), 214)
																			 (_S("THORN"), 222)
																			 (_S("Uacute"), 218)
																			 (_S("Ucirc"), 219)
																			 (_S("Ugrave"), 217)
																			 (_S("Uuml"), 220)
																			 (_S("Yacute"), 221)
																			 (_S("aacute"), 225)
																			 (_S("acirc"), 226)
																			 (_S("acute"), 180)
																			 (_S("aelig"), 230)
																			 (_S("agrave"), 224)
																			 (_S("aring"), 229)
																			 (_S("atilde"), 227)
																			 (_S("auml"), 228)
																			 (_S("brvbar"), 166)
																			 (_S("ccedil"), 231)
																			 (_S("cedil"), 184)
																			 (_S("cent"), 162)
																			 (_S("copy"), 169)
																			 (_S("curren"), 164)
																			 (_S("deg"), 176)
																			 (_S("divide"), 247)
																			 (_S("eacute"), 233)
																			 (_S("ecirc"), 234)
																			 (_S("egrave"), 232)
																			 (_S("eth"), 240)
																			 (_S("euml"), 235)
																			 (_S("frac12"), 189)
																			 (_S("frac14"), 188)
																			 (_S("frac34"), 190)
																			 (_S("iacute"), 237)
																			 (_S("icirc"), 238)
																			 (_S("iexcl"), 161)
																			 (_S("igrave"), 236)
																			 (_S("iquest"), 191)
																			 (_S("iuml"), 239)
																			 (_S("laquo"), 171)
																			 (_S("macr"), 175)
																			 (_S("micro"), 181)
																			 (_S("middot"), 183)
																			 (_S("nbsp"), 160)
																			 (_S("not"), 172)
																			 (_S("ntilde"), 241)
																			 (_S("oacute"), 243)
																			 (_S("ocirc"), 244)
																			 (_S("ograve"), 242)
																			 (_S("ordf"), 170)
																			 (_S("ordm"), 186)
																			 (_S("oslash"), 248)
																			 (_S("otilde"), 245)
																			 (_S("ouml"), 246)
																			 (_S("para"), 182)
																			 (_S("plusmn"), 177)
																			 (_S("pound"), 163)
																			 (_S("raquo"), 187)
																			 (_S("reg"), 174)
																			 (_S("sect"), 167)
																			 (_S("shy"), 173)
																			 (_S("sup1"), 185)
																			 (_S("sup2"), 178)
																			 (_S("sup3"), 179)
																			 (_S("szlig"), 223)
																			 (_S("thorn"), 254)
																			 (_S("times"), 215)
																			 (_S("uacute"), 250)
																			 (_S("ucirc"), 251)
																			 (_S("ugrave"), 249)
																			 (_S("uml"), 168)
																			 (_S("uuml"), 252)
																			 (_S("yacute"), 253)
																			 (_S("yen"), 165)
																			 (_S("yuml"), 255 );
																			 // iso8859-1 only for now	( "OElig", 338 ),
																			 // ditto			( "oelig", 339 ),

	for(unordered_map<String, uchar>::type::const_iterator i = entities.begin(); i != entities.end(); ++i)
	{
		m_entitiesNames[i->first] = i->second;
		m_entitiesCodes[i->second] = i->first;
	}
}

HtmlParser::~HtmlParser()
{

}

String HtmlParser::encode(const String &text)
{
	std::wstring source = text.to_wide();
	std::wstring encoded;
	
	size_t length = source.length();
	for(size_t i = 0; i < length; i++)
	{
		uchar c = source.at(i);
		unordered_map<uchar, String>::type::const_iterator e = m_entitiesCodes.find(c);
		if(e != m_entitiesCodes.end())
		{
			encoded.append(_W("&"));
			encoded.append(e->second.to_wide());
			encoded.append(_W(";"));
		}
		else
		{
			if(c > 128)
			{
				encoded.append(_W("&#"));
				encoded.append(String::format(_S("%d").c_str(), c).to_wide());
				encoded.append(_W(";"));
			}
			else
			{
				encoded += c;
			}
		}
	}

	/*
	String encoded;

	size_t length = text.length();
	for(size_t i = 0; i < length; i++)
	{
		uchar c = text.at(i);
		unordered_map<uchar, String>::type::const_iterator e = m_entitiesCodes.find(c);
		if(e != m_entitiesCodes.end())
		{
			encoded.append(_S("&"));
			encoded.append(e->second);
			encoded.append(_S(";"));
		}
		else
		{
			if(c > 128)
			{
				encoded.append(_S("&#"));
				encoded.append(String::format(_S("%d").c_str(), c));
				encoded.append(_S(";"));
			}
			else
			{
				encoded += c;
			}
		}
	}
	*/

	return encoded;
}

String HtmlParser::encodeEx(const String &text)
{
	String encoded = encode(text);

	// Queste encode le ho aggiunte io e non fanno parte dello standard di encoding di html.
	// Secondo me non sono sbagliate e risolvono una carenza dello standard di encoding,
	// che ignora i carriage return.
	encoded = encoded.replace_all(_S("\r\n"),_S("\n"));
	encoded = encoded.replace_all(_S("\n"),_S("<br />"));

	encoded = encoded.replace_all(_S("'"),_S("&apos;"));

	return encoded;
}

String HtmlParser::decode(const String &html)
{
	String decoded = html;

    // We need a const_iterator version of s.end() - otherwise the
    // find() and find_if() templates don't work...

	String::const_iterator amp = decoded.begin(), s_end = decoded.end();
    while((amp = find(amp, s_end, chAmpersand)) != s_end)
	{
		uint32 val = 0;
		String::const_iterator end, p = amp + 1;
		if(p != s_end && *p == chPound)
		{
			p++;
			if((p != s_end) && (*p == chLatin_x || *p == chLatin_X))
			{
				// hex
				p++;
				end = find_if(p, s_end, utils::notxdigit);

				String hexStr = decoded.substr(p - decoded.begin(), end - p);
				hexStr.scanf(_S("%x").c_str(), &val);
			}
			else
			{
				// number
				end = find_if(p, s_end, utils::notdigit);
				val = conversions::from_utf16<uint32>(decoded.substr(p - decoded.begin(), end - p));
			}
		}
		else
		{
			end = find_if(p, s_end, utils::notalnum);
			String code = decoded.substr(p - decoded.begin(), end - p);
			unordered_map<String, uchar>::type::const_iterator i;
			i = m_entitiesNames.find(code);
			if(i != m_entitiesNames.end())
				val = i->second;
		}

		if(end < s_end && *end == chSemiColon)
			end++;

		if(val)
		{
			String::size_type amp_pos = amp - decoded.begin();
			decoded.replace(amp_pos, end - amp, 1u, uchar(val));	// URGENT: cast a uchar?
			s_end = decoded.end();
			// We've modified the String, so the iterators are no longer
			// valid...
			amp = decoded.begin() + amp_pos + 1;
		}
		else
		{
			amp = end;
		}
    }

	return decoded;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
