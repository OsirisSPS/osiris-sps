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
#include "httprequest.h"

#include "algorithms.h" 
#include "buffer.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "conversions.h"
#include "cryptmanager.h"
#include "httpcontent.h"
#include "httpdata.h"
#include "httpparser.h"
#include "httppostparam.h"
#include "httpurl.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HttpRequest::HttpRequest(HttpMethod method, const std::string &rawurl) : m_method(method),
																		 m_rawurl(rawurl),
																		 m_modifiedSince(boost::date_time::not_a_date_time),
																		 m_supportGzip(false),
																		 m_authorization(httpAuthorizationAnonymous),
																		 m_keepAlive(OS_HTTP_CONNECTION_TIMEOUT_DEFAULT),
																		 m_closeConnection(false),	// default HTTP/1.1
																		 m_port(OS_HTTP_HTTP_PORT_DEFAULT)
{

}

HttpRequest::~HttpRequest()
{

}

const std::wstring & HttpRequest::getUrlParam(const std::wstring &name) const
{ 
	ordered_map<std::wstring, std::wstring>::const_iterator i = m_urlParams.find(name);

	if(i != m_urlParams.end())
		return i->second;

	return constants::empty_wstring;
}

std::wstring HttpRequest::getPostParamString(const std::wstring &name, const std::wstring &def) const
{
	shared_ptr<HttpPostParam> param = getPostParam(name);
	if(param == nullptr)
		return def;

	return param->getString();
}

const Buffer * HttpRequest::getPostParamBuffer(const std::wstring &name) const
{
	shared_ptr<HttpPostParam> param = getPostParam(name);
	if(param == nullptr)
		return nullptr;

	return param->getBuffer();
}

void HttpRequest::setPostParamString(const std::wstring &name, const std::wstring &value)
{
	shared_ptr<HttpPostParam> postParam = m_postParams.get(name);
	if(postParam == nullptr)
	{
		postParam.reset(OS_NEW HttpPostParam());
		m_postParams.push_back(name, postParam);
	}

	postParam->setName(name);
	postParam->setString(value);
}

void HttpRequest::setPostParamBuffer(const std::wstring &name, const Buffer &value)
{
	shared_ptr<HttpPostParam> postParam = m_postParams.get(name);
	if(postParam == nullptr)
	{
		postParam.reset(OS_NEW HttpPostParam());
		m_postParams.push_back(name, postParam);
	}

	postParam->setName(name);
	postParam->setBuffer(value);	
}

void HttpRequest::setUrl(const HttpUrl &url)
{
	m_rawurl = url.getPath();
	m_extra = url.getParams();
	setHost(url.getHost());
	setPort(url.getPort());
}

void HttpRequest::parseUrlParams()
{
	m_urlParams.clear();
	HttpParser::loadUrlParams(m_extra, m_urlParams);
}

void HttpRequest::addPostParam(const std::wstring &name, shared_ptr<HttpPostParam> param)
{
	OS_ASSERT(name.empty() == false);
	OS_ASSERT(param != nullptr);

	m_postParams.push_back(name, param);
}

void HttpRequest::parsePostParams()
{
    parsePostParams(m_content->toString(), m_contentType, m_contentBoundary);
}

void HttpRequest::parsePostParams(const std::string &params, const std::string &contentType, const std::string &contentBoundary)
{
    m_postParams.clear();

	if(contentType == OS_HTTP_CONTENTTYPE_TEXTPLAIN)
		HttpParser::decodePlainPostParams(params, m_postParams);
	else if(contentType == OS_HTTP_CONTENTTYPE_URLENCODED)
		HttpParser::decodeUrlEncodedPostParams(params, m_postParams);
	else if(contentType == OS_HTTP_CONTENTTYPE_MULTIPARTDATA)
		HttpParser::decodeMultiPartDataPostParams(params, contentBoundary, m_postParams);
}
 
shared_ptr<HttpData> HttpRequest::createHeader(shared_ptr<HttpData> content) const
{
	shared_ptr<HttpData> header(OS_NEW HttpData());

	std::string requestHeader;
	requestHeader.append(conversions::to_string(m_method));
	requestHeader.append(" ");
	requestHeader.append(HttpParser::urlEncodeUtf8(m_rawurl, true)); // TOTEST. Need encoded? Not m_rawurl already encoded?
	if(m_extra.empty() == false)
	{
		requestHeader.append("?");
		// Non si può qui chiamare una urlEncode su m_extra o si rischia di encodare '=' o '&'
		requestHeader.append(m_extra);
	}

	requestHeader.append(" ");
	requestHeader.append("HTTP/" + m_httpVersion.toString());
	header->writeHeaderLine(requestHeader);

	std::string hostParam = m_host;
	// N.B.: alcuni webserver non supportano host:80, pertanto usa la versione di "default" solo in questo caso (no 443)
	if(m_port != OS_HTTP_HTTP_PORT_DEFAULT)
		hostParam.append(":" + conversions::to_string(m_port));
	header->writeHeaderParam(OS_HTTP_PARAM_HOST, hostParam);

	header->writeHeaderParam(OS_HTTP_PARAM_USERAGENT, m_userAgent);
	if(m_accept.empty() == false)
		header->writeHeaderParam(OS_HTTP_PARAM_ACCEPT, m_accept);
	if(m_acceptLanguage.empty() == false)
		header->writeHeaderParam(OS_HTTP_PARAM_ACCEPT_LANGUAGE, m_acceptLanguage);
	if(m_acceptEncoding.empty() == false)
		header->writeHeaderParam(OS_HTTP_PARAM_ACCEPT_ENCODING, m_acceptEncoding);
	if(m_acceptCharset.empty() == false)
		header->writeHeaderParam(OS_HTTP_PARAM_ACCEPT_CHARSET, m_acceptCharset);
	if(hasModifiedSince())
		header->writeHeaderParam(OS_HTTP_PARAM_IFMODIFIEDSINCE, HttpParser::toString(m_modifiedSince));	

	if(m_cookies.empty() == false)
	{
		std::string values;
		m_cookies.encode(values, ";", "=");
		header->writeHeaderParam(OS_HTTP_PARAM_COOKIE, values);
	}

	if(m_contentType.empty() == false)
	{
		if(m_contentType != OS_HTTP_CONTENTTYPE_MULTIPARTDATA)
			header->writeHeaderParam(OS_HTTP_PARAM_CONTENTTYPE, m_contentType);
		else
			header->write(std::string(OS_HTTP_PARAM_CONTENTTYPE) + ": " + m_contentType + "; " + OS_HTTP_PARAM_CONTENTBOUNDARY + "=" + m_contentBoundary + OS_HTTP_HEADER_LINE_TERMINATOR);
	}

	return header;
}

bool HttpRequest::parseMethod(const std::string &header)
{
	vector<std::string>::type tokens;
	size_t count = algorithms::split(header, tokens, true).size();
	if(count == 0)
		return false;

	HttpMethod method = conversions::from_string<HttpMethod>(tokens[0]);
	if(method == httpMethodUnknown)
		return false;

	setMethod(method);

    if(count > 1)
	{
		m_rawurl = tokens[1];

		size_t question = m_rawurl.find("?");
		if(question != std::string::npos)
		{
			m_url = HttpParser::urlDecodeA(algorithms::left(m_rawurl, question)); // VERYVERYURGENT

			// Nota: l'extra non deve essere decodificato per garantire il corretto caricamento dei parametri dell'url.
			// Esempio: url?param=alfa%26beta decodificato vale url?param=alfa&beta verrebbe interpretato come param = alfa, beta = String::EMPTY,
			// mentre il valore effettivo di param è alfa&beta
			m_extra = algorithms::mid(m_rawurl, question + 1);
		}
		else
		{
			m_url = HttpParser::urlDecodeA(m_rawurl); // VERYVERYURGENT
		}

		if(count > 2)
		{
			if(HttpParser::parseHttpVersion(tokens[2], m_httpVersion) == false)
				return false;
		}
		else
		{
			m_httpVersion = OS_HTTP_VERSION_1_0();
		}

		// Nel caso di HTTP 1.0 ogni connessione viene chiusa dopo essere stata processata
		if(m_httpVersion == OS_HTTP_VERSION_1_0())
		{
			m_keepAlive = 0;
			m_closeConnection = true;
		}
	}

	return true;
}

shared_ptr<HttpData> HttpRequest::createContent() const
{	
    shared_ptr<HttpData> content = MethodBase::createContent();
    if(content != nullptr)
    {
        OS_ASSERT(m_postParams.empty());    // Se il contenuto del metodo è stato valorizzato nella base la richiesta non dovrebbe avere nulla da scrivere
        return content;
    }

	// Analizzo i parametri e determino il content-type necessario.
	// La faccio qui perchè so che createContent è chiamata prima di createHeader, ma non son certo che sia il punto giusto.
	if(m_postParams.empty() == false)
	{
		// Content-Type di default per valori di POST plain-text.
		m_contentType = OS_HTTP_CONTENTTYPE_URLENCODED;

		for(HttpPostParams::iterator i = m_postParams.begin(); i != m_postParams.end(); ++i)
		{
			shared_ptr<HttpPostParam> param = i->second;
			if(param->isBuffer())
			{
				m_contentType = OS_HTTP_CONTENTTYPE_MULTIPARTDATA;
				if(m_contentBoundary.empty())
				{
					std::string boundary = "Fw7uIxpvXnj5dzjGMrD7OB"; // TOFIX, sarebbe meglio random?
					m_contentBoundary = "--" + boundary;
					m_contentBoundaryTerminator = m_contentBoundary + "--";
				}

				break;
			}
		}
	}

	if(m_postParams.empty() == false)
	{
	    content.reset(OS_NEW HttpData());

		OS_ASSERT(getMethod() == httpMethodPost);
		
		for(HttpPostParams::iterator i = m_postParams.begin(); i != m_postParams.end(); ++i)
		{
			shared_ptr<HttpPostParam> param = i->second;
			
			bool compressData = ( (param->getFilename().empty() == false) && (param->isBuffer()) && (param->getBuffer()->getSize() > 0) );

			if(m_contentType != OS_HTTP_CONTENTTYPE_MULTIPARTDATA)
			{
				if(i != m_postParams.begin())
					content->write("&");

				content->write(HttpParser::urlEncodeW(param->getName()));
				content->write("=");
				content->write(HttpParser::urlEncodeW(param->getString()));
			}
			else
			{
				content->write("--" + m_contentBoundary + OS_HTTP_HEADER_LINE_TERMINATOR);
				content->write("Content-Disposition: form-data; name=\"" + HttpParser::urlEncodeW(param->getName()) + "\"");
				if(param->getFilename().empty() == false)
				{
					// Se ha il filename, allora setto anche il content-type. Non so se rispecchia lo standard.

					content->write("; filename=\"" + HttpParser::urlEncodeW(param->getFilename()) + "\"");
					content->writeHeaderLineTerminator();

					if(param->isBuffer())
					{						
						if(compressData)
						{							
							content->write(std::string("Content-Type: ") + OS_HTTP_CONTENTTYPE_GZIP);
						}
						else
						{
							content->write(std::string("Content-Type: ") + OS_HTTP_CONTENTTYPE_OCTETSTREAM);
						}
					}
					else
					{
						content->write(std::string("Content-Type: ") + OS_HTTP_CONTENTTYPE_TEXTPLAIN);
						content->write(std::string(";charset: utf8")); // 0.15 Added
					}

					content->writeHeaderLineTerminator();
				}
				else
				{
					content->writeHeaderLineTerminator();
				}

				content->writeHeaderLineTerminator();
				if(param->isBuffer())
				{
					const Buffer *buffer = param->getBuffer();
					OS_ASSERT(buffer != nullptr);	

					if(compressData)
					{
						// Compressione GZIP						
						Buffer compressed;
						if(CryptManager::instance()->compress(compressorTypeGzip, buffer->getData(), buffer->getSize(), compressed))
						{
							content->write(compressed);
						}
					}
					else
					{
						// Versione "application/octet-stream"						
						content->write(*buffer);						
					}
				}
				else
				{
					OS_ASSERT(param->isString());
					//content->write(param->getString()); 
					content->write(utils::wtou8(param->getString())); // 0.15
				}

				content->writeHeaderLineTerminator();
			}
		}
		
		if(m_contentType == OS_HTTP_CONTENTTYPE_MULTIPARTDATA)
			content->write("--" + m_contentBoundaryTerminator + OS_HTTP_HEADER_LINE_TERMINATOR);
	}

	return content;	
}

bool HttpRequest::parseParam(const std::string &paramName, const std::string &paramValue)
{
	if(MethodBase::parseParam(paramName, paramValue) == false)
		return false;

	if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_HOST))
	{
		m_host = paramValue;
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_USERAGENT))
	{
		m_userAgent = paramValue;
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_IFMODIFIEDSINCE))
	{
		// Handle the If-Modified-Since header

		boost::posix_time::ptime time;
		if(HttpParser::parseDate(paramValue, time))
			m_modifiedSince = time;
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_AUTHORIZATION))
	{
		// Handle the Authorization header

		std::string username;
		std::string password;
		if(HttpParser::parseAuthorization(paramValue, username, password))
		{
			m_authorization = httpAuthorizationPlainText;
			m_username = username;
			m_password = password;
		}
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_CONTENTTYPE))
	{
		HttpParser::parseContentType(paramValue, m_contentType, m_contentBoundary, m_contentBoundaryTerminator);
		if(m_contentType.empty())
			m_contentType = OS_HTTP_CONTENTTYPE_URLENCODED;	// Codifica di default
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_COOKIE))
	{
		m_cookies.decode(paramValue, ";", "=");
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_ACCEPT))
	{
		m_accept = paramValue;
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_ACCEPT_ENCODING))
	{
		m_supportGzip = paramValue.find(OS_HTTP_CONTENTENCODING_GZIP) != String::npos;
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_ACCEPT_LANGUAGE))
	{
		m_acceptLanguage = paramValue;
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_ACCEPT_CHARSET))
	{
		m_acceptCharset = paramValue;
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_KEEPALIVE))
	{
		// L'rfc dell'http 1.1 indica di non considerare il keep-alive nel caso di versione 1.0

		if(m_httpVersion > OS_HTTP_VERSION_1_0())
			m_keepAlive = utils::limit<uint32>(conversions::from_string<uint32>(paramValue), OS_HTTP_CONNECTION_TIMEOUT_MIN, OS_HTTP_CONNECTION_TIMEOUT_MAX);
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_CONNECTION))
	{
		// L'RFC dell'http 1.1 indica di non considerare il keep-alive nel caso di versione 1.0

		if(m_httpVersion > OS_HTTP_VERSION_1_0())
			m_closeConnection = algorithms::compare_no_case(paramValue, "close");
	}

	return true;
}

bool HttpRequest::parseContent()
{
	if(MethodBase::parseContent() == false)
		return false;

	parseUrlParams();
	parsePostParams();

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
