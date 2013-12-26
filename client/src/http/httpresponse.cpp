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
#include "httpresponse.h"

#include "boost/format.hpp"
#include "buffer.h"
#include "constants.h"
#include "conversions.h"
#include "datetime.h"
#include "httpcookie.h"
#include "httpparser.h"
#include "httprequest.h"
#include "memfile.h"
#include "platformmanager.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HttpResponse::HttpResponse() : m_statusCode(0),
							   m_responseBuffer(OS_NEW Buffer())
{

}

HttpResponse::~HttpResponse()
{

}

void HttpResponse::setContentLength(uint32 length)
{
	m_params.set(OS_HTTP_PARAM_CONTENTLENGTH, conversions::to_string(length));
}

std::string HttpResponse::getContentType() const
{
	if(m_params.exists(OS_HTTP_PARAM_CONTENTTYPE))
		return m_params.get(OS_HTTP_PARAM_CONTENTTYPE);
	else
		return "";
}

void HttpResponse::setContentType(const std::string &type)
{
	m_params.set(OS_HTTP_PARAM_CONTENTTYPE, type);
}

void HttpResponse::setContentType(HttpContentType type)
{
	setContentType(HttpParser::contentFromType(type));
}

void HttpResponse::setContentEncoding(const std::string &encoding)
{
	m_params.set(OS_HTTP_PARAM_CONTENTENCODING, encoding);
}

void HttpResponse::setContentDisposition(const std::string &filename, boost::optional<uint32> size)
{
	std::string value;
	value.append((boost::format("attachment; filename=\"%s\"") % filename).str());
	if(size)
		value.append((boost::format("; size=%u") % size.get()).str());
		
	m_params.set(OS_HTTP_PARAM_CONTENTDISPOSITION, value);
}

void HttpResponse::setAllowMethods()
{
	m_params.set(OS_HTTP_PARAM_ALLOW, "GET, HEAD, DELETE");
}

void HttpResponse::setDate(const boost::posix_time::ptime &dt)
{
	m_params.set(OS_HTTP_PARAM_DATE, HttpParser::toString(dt));
}

void HttpResponse::setDateNow()
{
	setDate(boost::posix_time::second_clock::universal_time());
}

void HttpResponse::setLastModified(const boost::posix_time::ptime &dt)
{
	m_params.set(OS_HTTP_PARAM_LASTMODIFIED, HttpParser::toString(dt));
}

void HttpResponse::setExpires(const boost::posix_time::ptime &dt)
{
	m_params.set(OS_HTTP_PARAM_EXPIRES, HttpParser::toString(dt));
}

void HttpResponse::setWWWBasicAuthenticate(const std::string &realm)
{
	m_params.set(OS_HTTP_PARAM_WWWAUTHENTICATE, "Basic realm=" + realm);
}

void HttpResponse::setLocation(const std::string &location)
{
	m_params.set(OS_HTTP_PARAM_LOCATION, location);
}

void HttpResponse::setServer(const std::string &server)
{
	m_params.set(OS_HTTP_PARAM_SERVER, server);
}

void HttpResponse::setCookie(const std::string &name, const std::string &value)
{
	m_cookies[name] = shared_ptr<HttpCookie>(OS_NEW HttpCookie(name, value, HttpCookie::MAX_EXPIRES_DATE, constants::empty_string, "/", false));
}

bool HttpResponse::removeCookie(const std::string &name)
{
	DateTime expiration = DateTime::now() - TimeDuration::seconds(1);
	shared_ptr<HttpCookie> cookie(OS_NEW HttpCookie(name, constants::empty_string, expiration.toPosixTime(), constants::empty_string, "/", false));
	m_cookies[name] = cookie;
	return true;
}

std::string HttpResponse::_createHeader(shared_ptr<HttpRequest> request)
{
	OS_ASSERT(request != null);

	std::string header;

	_addStatus(header);
	_addParams(header);
	_addCookies(header);

	//if(request->getCloseConnection())
	if(true) // VERYVERYURGENT
		header += "Connection: close\r\n";
	else
		header += "Connection: keep-alive\r\n";

	if(header.empty() == false)
		header += "\r\n";

	return header;
}

void HttpResponse::_addStatus(std::string &str)
{
	str += (boost::format("HTTP/1.1 %s\r\n") % conversions::to_string(m_statusCode)).str();
}

void HttpResponse::_addParams(std::string &str)
{
	for(StringCollection<std::string>::iterator i = m_params.begin(); i != m_params.end(); ++i)
	{
		const std::string &name = i->first;
		const std::string &value = *i->second;

		str.append(name);
		str.append(": ");
		str.append(value);
		str.append("\r\n");		
	}
}

void HttpResponse::_addCookies(std::string &str)
{
	for(HttpCookies::iterator i = m_cookies.begin(); i != m_cookies.end(); ++i)
	{
		const std::string &name = i->first;
		const shared_ptr<HttpCookie> &cookie = i->second;

		str += std::string(OS_HTTP_PARAM_SETCOOKIE) + ": " + name + "=" + cookie->getValue() + "; ";

		if(utils::ptime_is_valid(cookie->getExpires()))
			str += "expires=" + HttpParser::toString(cookie->getExpires()) + "; ";
		if(cookie->getDomain().empty() == false)
			str += "domain=" + cookie->getDomain() + "; ";
		if(cookie->getPath().empty() == false)
			str += "path=" + cookie->getPath() + "; ";
		if(cookie->getSecure())
			str += "secure";

		str += "\r\n";
	}
}

const Buffer & HttpResponse::getResponseBuffer() const
{
	OS_ASSERT(m_responseBuffer != null);
	return *m_responseBuffer;
}

shared_ptr<IStream> HttpResponse::getResponseStream() const
{
	return m_responseStream;
}

void HttpResponse::setResponseStream(shared_ptr<IStream> responseStream)
{
	m_responseStream = responseStream;
}

bool HttpResponse::writeHeader(shared_ptr<HttpRequest> request)
{
	return write(_createHeader(request));
}

bool HttpResponse::write(const std::string &str)
{
	return write(str.c_str(), static_cast<uint32>(str.length()));
}

bool HttpResponse::write(const void *data, uint32 size)
{
	OS_ASSERT(m_responseBuffer != null);
	return m_responseBuffer->write(data, size) == size;
}

shared_ptr<HttpData> HttpResponse::createHeader(shared_ptr<HttpData> content) const
{
	// URGENT: fixme
	return null;
}

bool HttpResponse::parseMethod(const std::string &header)
{
	vector<std::string>::type tokens;
	if(algorithms::split(header, tokens, true).size() < 2)
		return false;

	if(HttpParser::parseHttpVersion(tokens[0], m_httpVersion) == false)
		return false;

	m_statusCode = conversions::from_string<uint32>(tokens[1]);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
