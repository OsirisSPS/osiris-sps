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
#include "httpparser.h"

#include "algorithms.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/format.hpp"
#include "buffer.h"
#include "constants.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "httppostparam.h"
#include "platformmanager.h"
#include "version.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

bool HttpParser::isSafeURL(const std::string &url)
{
	return url.find("..") == std::string::npos;
}

std::string HttpParser::contentFromType(HttpContentType type)
{
	switch(type)
	{
	case httpContentTextHtml:		return "text/html; charset=utf-8";

	default:						OS_ASSERTFALSE();
									break;
	}

	return constants::empty_string;
}

HttpContentType HttpParser::contentFromString(const std::string &str)
{
	if(algorithms::starts_with(str, "text/html"))
		return httpContentTextHtml;

	return httpContentUnknown;
}

bool HttpParser::parseAuthorization(const std::string &value, std::string &username, std::string &password)
{
	vector<std::string>::type tokens;
	if(algorithms::split(value, tokens, boost::algorithm::is_any_of(" "), true).size() < 2)
		return false;

	if(tokens[0] != "Basic")
		return false;

	Buffer encodedAuthorization;
	if(encodedAuthorization.fromBase64(tokens[1]) == false)
		return false;

	std::string plainAuthorization;
	plainAuthorization.assign(reinterpret_cast<achar *>(encodedAuthorization.getData()), encodedAuthorization.getSize());
	size_t index = plainAuthorization.find(":");
	if(index == std::string::npos)
		return false;

	username = algorithms::left(plainAuthorization, index);
	password = algorithms::mid(plainAuthorization, index + 1);

	return true;
}

bool HttpParser::parseContentType(const std::string &value, std::string &contentType, std::string &boundary, std::string &boundaryTerminator)
{
	std::string::size_type pos = value.find(";");
	// Controlla se non sono stati specificati parametri a livello di content-type
	if(pos == std::string::npos)
	{
		contentType = value;
	}
	else
	{
		// Salva il content type
		contentType = algorithms::left(value, pos);

		StringCollection<std::string> params;
		// Estrae i parametri specificati
		params.decode(algorithms::mid(value, pos + 1), ";", "=");
		for(StringCollection<std::string>::iterator i = params.begin(); i != params.end(); ++i)
		{
			if(i->first == OS_HTTP_PARAM_CONTENTBOUNDARY)
			{
				boundary = "--" + *i->second;
				boundaryTerminator = boundary + "--";
				break;
			}
		}
	}

	return true;
}

bool HttpParser::parseDate(const std::string &value, boost::posix_time::ptime &time)
{
    time = constants::time_null;

    if(value.length() <= 5)
		return false;

	uint16 year = 0;
	uint16 month = 0;
	uint16 day = 0;
	uint16 hours = 0;
	uint16 minutes = 0;
	uint16 seconds = 0;

	// Controlla se la data è nel formato "Wed, 23 Apr 2008 07:54:52 GMT"
	if(value.at(3) == ',')
	{
		vector<std::string>::type tokens;
		if(algorithms::split(value, tokens, boost::algorithm::is_any_of(", :"), true).size() < 7)
			return false;

		// Ignora il weekday (tokens[0])

		day = conversions::from_utf16<uint16>(tokens[1]);
		month = 0;
		if(parseDateMonth(tokens[2], month) == false)
			return false;
		year = conversions::from_utf16<uint16>(tokens[3]);
		hours = conversions::from_utf16<uint16>(tokens[4]);
		minutes = conversions::from_utf16<uint16>(tokens[5]);
		seconds = conversions::from_utf16<uint16>(tokens[6]);
	}
	// Controlla se la data è nel formato "Wed Apr 23 07:54:52 2008"
	else if(value.at(3) == ' ')
	{
		vector<std::string>::type tokens;
		if(algorithms::split(value, tokens, boost::algorithm::is_any_of(", :"), true).size() < 7)
			return false;

		// Ignora il weekday (tokens[0])

		month = 0;
		if(parseDateMonth(tokens[1], month) == false)
			return false;
		day = conversions::from_utf16<uint16>(tokens[2]);
	    hours = conversions::from_utf16<uint16>(tokens[3]);
		minutes = conversions::from_utf16<uint16>(tokens[4]);
		seconds = conversions::from_utf16<uint16>(tokens[5]);
		year = conversions::from_utf16<uint16>(tokens[6]);
	}
	// La data dovrebbe essere nel formato "Wednesday, 23-Apr-08 07:54:52 GMT"
	else
	{
		vector<std::string>::type tokens;
		if(algorithms::split(value, tokens, boost::algorithm::is_any_of(", :-"), true).size() < 7)
			return false;

		// Ignora il weekday (tokens[0])

		day = conversions::from_utf16<uint16>(tokens[1]);
		month = 0;
		if(parseDateMonth(tokens[2], month) == false)
			return false;
		year = conversions::from_utf16<uint16>(tokens[3]);
		// Se l'anno è in forma abbreviata effettua la correzione
		if(tokens[3].length() == 2)
		{
			if(year < 50)
				year += 2000;
			else
				year += 1900;
		}
		hours = conversions::from_utf16<uint16>(tokens[4]);
		minutes = conversions::from_utf16<uint16>(tokens[5]);
		seconds = conversions::from_utf16<uint16>(tokens[6]);
	}

	time = utils::ptime_create(year, month, day, hours, minutes, seconds);
	return true;
}

bool HttpParser::parseParam(const std::string &line, std::string &param, std::string &value)
{
    size_t pos = line.find(":");
	if(pos == std::string::npos)
		return false;

	param = algorithms::trim_copy(algorithms::left(line, pos));
	value = algorithms::trim_copy(algorithms::mid(line, pos + 1));

	return true;
}

bool HttpParser::parseParamValue(const std::string &str, std::string &value, StringCollection<std::string> &attributes)
{
	size_t pos = str.find(";");
	if(pos == std::string::npos)
	{
		value = str;
	}
	else
	{
		value = algorithms::left(str, pos);
		attributes.decode(algorithms::mid(str, pos + 1), ";", "=");
	}

	return true;
}

void HttpParser::loadUrlParams(const std::string &source, ordered_map<std::wstring, std::wstring> &params)
{
	StringCollection<std::string> decoded;
	decoded.decode(source, "&", "=");
	for(StringCollection<std::string>::iterator i = decoded.begin(); i != decoded.end(); ++i)
		params.set(urlDecodeW(i->first), urlDecodeW(*i->second));
}

void HttpParser::saveUrlParams(const ordered_map<std::wstring, std::wstring> &source, std::string &params)
{
	ordered_map<std::string, std::string> encoded;

	for(ordered_map<std::wstring, std::wstring>::const_iterator i = source.begin(); i != source.end(); ++i)
		encoded.set(urlEncodeW(i->first), urlEncodeW(i->second));

	algorithms::encode_map<std::string>(params, encoded, "&", "=");
}

std::string HttpParser::generateLink(const std::string &url, const ordered_map<std::wstring, std::wstring> &params)
{
	std::string link = url;
	if(params.size() > 0)
	{
		std::string rawParams;
		saveUrlParams(params, rawParams);
		OS_ASSERT(rawParams.empty() == false);

		// Se non esistono dei parametri nell'url aggiunge il separatore altrimenti li concatena a quelli già esistenti
		if(link.find("?") == std::string::npos)
			link.append("?");
		else
			link.append("&");

		link.append(rawParams);
	}

	return link;
}

std::string HttpParser::generateLink(const std::string &url, const std::wstring &paramName, const std::wstring &paramValue)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(paramName, paramValue);
	return generateLink(url, params);
}

std::string HttpParser::createAnchor(const std::string &url, const std::string &anchor)
{
	return url + "#" + anchor;
}

bool HttpParser::parseHttpVersion(const std::string &str, Version &version)
{
	version.clear();
	if(algorithms::starts_with(str, "HTTP/", algorithms::case_sensitive) == false)
		return false;

	return version.fromString(algorithms::mid(str, 5));
}

std::string HttpParser::urlEncodeUtf8(const std::string &url, bool escapeOnly)
{
	std::string encoded;

	size_t length = url.length();
	for(size_t i = 0; i < length; i++)
	{
		char c = url[i];

		bool encode = (c == ' ') || (c < '0' && c != '-' && c != '.') || (c < 'A' && c > '9') || (c > 'Z' && c < 'a' && c != '_') || (c > 'z');
		if(escapeOnly)
		{
			if(c == '/')
				encode = false;
		}

		if(encode)
		{
            encoded += '%';
			encoded += static_cast<char>(conversions::intToHex((c & 0xF0) >> 4));
			encoded += static_cast<char>(conversions::intToHex(c & 0x0F));
	    }
		else
		{
			encoded += c;
		}
	}

	return encoded;
}

std::string HttpParser::urlDecodeUtf8(const std::string &url)
{
	/*
	Esempio 'estremo': Lo stesso carattere codificato in due modi diversi, più un carattere utf8 'puro'.
	In: /gratt%F2?Alfa=gratt%C3%B2&Beta=Grec%CF%88
	Out: /grattò?Alfa=grattò&Beta=Grec*  <- Attenzione: Sorgente CPP salvato in ansi... * = U+03C8 (carattere alfabeto greco)
	*/

	size_t length = url.length();

	std::string decoded;

	for(size_t i = 0; i < length; i++)
	{
		char c = url[i];
		if(c != '%')
		{
            if(c == '+')
				decoded += ' ';
			else
				decoded += c;
		}
        else
		{
            if(i < (length - 2))
			{
                int32 msb = conversions::hexToInt(url[i + 1]);
				int32 lsb = conversions::hexToInt(url[i + 2]);

				if(msb != -1 && lsb != -1)
				{
					decoded += static_cast<char>((msb << 4) + lsb);
					i += 2;
				}
				else
				{
					decoded += c;
				}
			}
			else
			{
				decoded += c;
			}
		}
	}

	return decoded;
}

std::string HttpParser::urlEncodeW(const std::wstring &url)
{
	return urlEncodeUtf8(utils::wtou8(url));		
}

std::string HttpParser::urlEncodeA(const std::string &url)
{
	return urlEncodeUtf8(url);
}

std::wstring HttpParser::urlDecodeW(const std::string &url)
{
	return utils::u8tow(urlDecodeUtf8(url));	
}

std::string HttpParser::urlDecodeA(const std::string &url)
{	
	return utils::u8toa(urlDecodeUtf8(url));	
}

bool HttpParser::decodePlainPostParams(const std::string &content, HttpPostParams &params)
{
	StringCollection<std::string> decoded;
	// Estra l'elenco dei parametri
	decoded.decode(content, "&", "=");
	for(StringCollection<std::string>::iterator i = decoded.begin(); i != decoded.end(); ++i)
	{
		shared_ptr<HttpPostParam> param(OS_NEW HttpPostParam());
		param->setName(utils::atow(i->first));
		param->setString(utils::atow(*i->second)); // We suppose that "plain" it's "ascii", not unicode.

		OS_ASSERT(param->getName().empty() == false);
		params.push_back(param->getName(), param);
	}

	return true;
}

bool HttpParser::decodeUrlEncodedPostParams(const std::string &content, HttpPostParams &params)
{
	ordered_map<std::wstring, std::wstring> decoded;
	// Effettua la decodifica dei parametri
	loadUrlParams(content, decoded);
	for(ordered_map<std::wstring, std::wstring>::iterator i = decoded.begin(); i != decoded.end(); ++i)
	{
		shared_ptr<HttpPostParam> param(OS_NEW HttpPostParam());
		param->setName(i->first);
		param->setString(i->second);

		OS_ASSERT(param->getName().empty() == false);
		params.push_back(param->getName(), param);
	}

	return true;
}

bool HttpParser::decodeMultiPartDataPostParams(const std::string &content, const std::string &boundary, HttpPostParams &params)
{
	size_t offset = boundary.length();
	// Ricerca il primo segmento
	size_t start = content.find(boundary.c_str());
	while(start != std::string::npos)
	{
		start += offset;
		// Ricerca la fine del segmento corrente
		size_t end = content.find(boundary.c_str(), start);
		if(end != std::string::npos)
		{
			// Calcola la dimensione (compresa dell'header) del segmento corrente
			size_t part_length = end - start - strlen(OS_HTTP_CONTENT_TERMINATOR);

			// Ottiene il puntatore al segmento corrente
			const char *pData = content.c_str() + start;
			// Ricerca la terminazione dell'header del segmento
			const char *pTerminator = strstr(pData, OS_HTTP_HEADER_TERMINATOR);
			if(pTerminator != nullptr)
			{
				// Calcola la dimensione dell'header
				size_t header_length = static_cast<uint32>(pTerminator - pData);
				// Calcola l'offset dei dati del segmento corrente
				size_t content_offset = header_length + strlen(OS_HTTP_HEADER_TERMINATOR);

				shared_ptr<HttpPostParam> param(OS_NEW HttpPostParam());

				// Salva l'header del segmento
				std::string header(pData, header_length);
				vector<std::string>::type header_params;
				// Carica i parametri dell'header
				algorithms::iter_split(header, header_params, boost::algorithm::first_finder(OS_HTTP_HEADER_LINE_TERMINATOR, boost::algorithm::is_equal()));
				// Scandisce tutti i parametri
				for(vector<std::string>::type::const_iterator i = header_params.begin(); i != header_params.end(); ++i)
				{
					std::string paramName;
					std::string paramValues;
					// Estrae il parametro corrente
					if(parseParam(*i, paramName, paramValues))
					{
						std::string value;
						StringCollection<std::string> attributes;
						// Estra il valore del parametro corrente
						if(parseParamValue(paramValues, value, attributes))
						{
							if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_CONTENTTYPE))
							{
								param->setContentType(value);
							}
							else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_CONTENTDISPOSITION))
							{
								param->setContentDisposition(value);

								for(StringCollection<std::string>::iterator i = attributes.begin(); i != attributes.end(); ++i)
									param->setAttribute(urlDecodeW(i->first), urlDecodeW(algorithms::trim_copy(*i->second, boost::algorithm::is_any_of("\""))));
							}
						}
					}
				}

				// Verifica che il parametro sia stato caricato correttamente
				if((param->getName().empty() == false) && (part_length > content_offset))
				{
					const achar *pParamContent = pData + content_offset;
					uint32 paramContentLength = static_cast<uint32>(part_length - content_offset);

					// TODO: "hack" per gestire files di testo: arriva text/plain ma deve essere valorizzato come buffer per lasciarne inalterato il contenuto...
					if(param->isTextPlain() && param->getFilename().empty())
					{						
						std::string value(pParamContent, paramContentLength);
						
						//param->setString(value.c_str());

						// 0.15
						// TODO: We suppose that content-charset is UTF8. Here: http://www.ietf.org/rfc/rfc2388.txt explain that we need to read with content-type above.
						// BWT, now better than 0.14.
						param->setString(utils::u8tow(value));
					}
					else
					{
						Buffer value;
						value.write(pParamContent, paramContentLength);
						param->setBuffer(value);
					}

					params.push_back(param->getName(), param);
				}
			}
		}

		start = end;
	}

	return true;
}

std::string HttpParser::toString(const boost::posix_time::ptime &time)
{
	// RFC 822

	if(time.is_not_a_date_time())
	{
		OS_ASSERTFALSE();
		return constants::empty_string;
	}

	static const char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	static const char *days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

	OS_ASSERT(time.date().day_of_week() < OS_COUNTOF(days));
	OS_ASSERT(static_cast<uint32>(time.date().month() - 1) < OS_COUNTOF(months));

	return (boost::format("%s, %02d %s %04d %02d:%02d:%02d GMT") % days[time.date().day_of_week()] % time.date().day() % months[time.date().month() - 1] % time.date().year() % time.time_of_day().hours() % time.time_of_day().minutes() % time.time_of_day().seconds()).str();
}

bool HttpParser::parseDateMonth(const std::string &str, uint16 &month)
{
	if(algorithms::compare_no_case(str, "Jan"))
        month = 1;
	else if(algorithms::compare_no_case(str, "Feb"))
        month = 2;
	else if(algorithms::compare_no_case(str, "Mar"))
        month = 3;
	else if(algorithms::compare_no_case(str, "Apr"))
        month = 4;
	else if(algorithms::compare_no_case(str, "May"))
        month = 5;
	else if(algorithms::compare_no_case(str, "Jun"))
        month = 6;
	else if(algorithms::compare_no_case(str, "Jul"))
        month = 7;
	else if(algorithms::compare_no_case(str, "Aug"))
        month = 8;
	else if(algorithms::compare_no_case(str, "Sep"))
        month = 9;
	else if(algorithms::compare_no_case(str, "Oct"))
        month = 10;
	else if(algorithms::compare_no_case(str, "Nov"))
        month = 11;
    else if(algorithms::compare_no_case(str, "Dec"))
        month = 12;
    else
        return false;

	return true;
}

bool HttpParser::parseDateWeekDay(const std::string &value, uint16 &weekDay)
{
	if(value == "Sun" || value == "Sunday")
		weekDay = 0;
    else if(value == "Mon" || value == "Monday")
        weekDay = 1;
    else if(value == "Tue" || value == "Tuesday")
	    weekDay = 2;
    else if(value == "Wed" || value == "Wednesday")
		weekDay = 3;
	else if (value == "Thu" || value == "Thursday")
		weekDay = 4;
	else if (value == "Fri" || value == "Friday")
	    weekDay = 5;
	else if (value == "Sat" || value == "Saturday")
		weekDay = 6;
	else
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
