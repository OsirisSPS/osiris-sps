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
#include "ihttpmethod.h"

#include "boost/tokenizer.hpp"
#include "buffer.h"
#include "constants.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "httpcontent.h"
#include "httpdata.h"
#include "httpparser.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IHttpMethod::IHttpMethod() : m_httpVersion(OS_HTTP_VERSION_1_1()),
							 m_content(OS_NEW HttpContent()),
							 m_status(statusWaitingHeader),
							 m_buffer(OS_NEW Buffer())
{

}

IHttpMethod::~IHttpMethod()
{

}

const std::string & IHttpMethod::getRawParam(const std::string &param) const
{
	MethodParams::const_iterator i = m_rawParams.find(param);
	if(i != m_rawParams.end())
		return i->second;

	return constants::empty_string;
}

std::string IHttpMethod::getRawHeaders(bool terminator) const
{
	OS_ASSERT(m_rawMethod.empty() == false);

	std::string headers;
	headers.append(m_rawMethod);
	for(MethodParams::const_iterator i = m_rawParams.begin(); i != m_rawParams.end(); ++i)
	{
		headers.append(OS_HTTP_HEADER_LINE_TERMINATOR);
		headers.append(i->first);
		headers.append(": ");
		headers.append(i->second);		
	}

	if(terminator)
		headers.append(OS_HTTP_HEADER_TERMINATOR);

	return headers;
}

shared_ptr<HttpData> IHttpMethod::serialize() const
{
    shared_ptr<HttpData> content = createContent();

	shared_ptr<HttpData> header = createHeader(content);
	if(header == null)
	{
	    OS_ASSERTFALSE();
		return null;
	}

	// TODO: aggiungere gli headers comuni (valorizzare anche gli attributi relativi al content es.: length, type, ecc...)

    if(content != null)
        header->writeHeaderParam(OS_HTTP_PARAM_CONTENTLENGTH, conversions::to_string(content->getSize()));

	header->writeHeaderLineTerminator();

	if(content != null)
        header->write(content->getData(), content->getSize());

	return header;
}

IHttpMethod::MethodStatus IHttpMethod::writeData(const void *data, uint32 size)
{
	static const std::string httpHeaderTerminator(OS_HTTP_HEADER_TERMINATOR);

	switch(m_status)
	{
	case statusWaitingHeader:	{
									m_buffer->write(data, size);
									if(m_buffer->empty() == false)
									{
										// Ricerca il terminatore di fine header
										const char *terminator = reinterpret_cast<const char *>(m_buffer->find(reinterpret_cast<const byte *>(httpHeaderTerminator.c_str()), static_cast<uint32>(httpHeaderTerminator.size())));
										if(terminator != null)
										{
											char *pHeaders = reinterpret_cast<char *>(m_buffer->getData());
											// Calcola la dimensione dell'header
											uint32 headerSize = static_cast<uint32>(terminator - pHeaders);
											pHeaders[headerSize] = 0;

											// Processa la richiesta
											if(parseHeaders(pHeaders) == false)
											{
												changeStatus(statusError);
												break;
											}

											// Calcola l'offset dell'header HTTP
											uint32 headerOffset = static_cast<uint32>(headerSize + httpHeaderTerminator.size());
											// Calcola la dimensione del contenuto già eventualmente ricevuta
											uint32 availableContent = m_buffer->getSize() - headerOffset;
											const byte *contentData = m_buffer->getData() + headerOffset;

											// N.B.: richiamare comunque il metodo "process" per inizializzare lo stato di m_content
											m_content->process(contentData, availableContent);

											if(m_content->getValid() == false)
												changeStatus(statusError);
											else if(m_content->getCompleted())
												changeStatus(statusCompleted);
											else
												changeStatus(statusWaitingContent);
										}
									}
								}

								break;

	case statusWaitingContent:	{
									m_content->process(reinterpret_cast<const byte *>(data), size);

									if(m_content->getValid() == false)
										changeStatus(statusError);
									else if(m_content->getCompleted())
										changeStatus(statusCompleted);
								}

								break;

	default:					OS_ASSERTFALSE();
								changeStatus(statusError);

								break;
	}

	return m_status;
}

void IHttpMethod::changeStatus(MethodStatus status)
{
	if(m_status != status)
	{
		m_status = status;
		if(m_status == statusCompleted)
		{
			if(parseContent() == false)
				m_status = statusError;			
		}
	}
}

shared_ptr<HttpData> IHttpMethod::createContent() const
{
    if(m_content->empty())
        return null;

    shared_ptr<HttpData> content(OS_NEW HttpData());
    // Scrive il contenuto del metodo (se valorizzato)
    content->write(m_content->getContent());

    return content;
}

bool IHttpMethod::parseHeaders(const std::string &headers)
{
	vector<std::string>::type tokens;
//	if(algorithms::iter_split(headers, tokens, boost::algorithm::first_finder(OS_HTTP_HEADER_LINE_TERMINATOR, boost::algorithm::is_equal())).size() == 0)
	if(algorithms::split(headers, tokens, boost::algorithm::is_any_of(OS_HTTP_HEADER_LINE_TERMINATOR)).size() == 0)
		return false;

	for(vector<std::string>::type::const_iterator i = tokens.begin(); i != tokens.end(); ++i)
	{
		if((*i).empty())
		{
			OS_ASSERTFALSE();
			continue;
		}

		// Verifica se il parametro corrente è l'intestazione del metodo
		if(i == tokens.begin())
		{
			m_rawMethod = *i;	

			if(parseMethod(*i) == false)
				return false;
		}
		else
		{
			std::string paramName;
			std::string paramValue;

			if(HttpParser::parseParam(*i, paramName, paramValue) == false)
				return false;

			OS_ASSERT(paramName.empty() == false);
			OS_ASSERT(paramValue.empty() == false);
			m_rawParams.insert(std::make_pair(paramName, paramValue));

			if(parseParam(paramName, paramValue) == false)
				return false;
		}
	}

	return true;
}

bool IHttpMethod::parseParam(const std::string &paramName, const std::string &paramValue)
{
	if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_CONTENTLENGTH))
	{
		m_content->setContentLength(conversions::from_string<uint32>(paramValue));
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_CONTENTENCODING))
	{
		m_content->setContentEncoding(conversions::from_string<HttpContentEncoding>(paramValue));
	}
	else if(algorithms::compare_no_case(paramName, OS_HTTP_PARAM_TRANSFERENCODING))
	{
		m_content->setTransferEncoding(conversions::from_string<HttpTransferEncoding>(paramValue));
	}

	return true;
}

bool IHttpMethod::parseContent()
{
	// Una volta processato l'header svuota il buffer temporaneo
	m_buffer->clear();

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
