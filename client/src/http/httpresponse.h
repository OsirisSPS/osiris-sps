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

#ifndef _OS_HTTP_HTTPRESPONSE_H
#define _OS_HTTP_HTTPRESPONSE_H

#include "boost/optional.hpp"
#include "collections.h"
#include "http.h"
#include "ihttpmethod.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpCookie;
class HttpRequest;
class IStream;

//////////////////////////////////////////////////////////////////////

class HttpExport HttpResponse : public IHttpMethod
{
	typedef IHttpMethod MethodBase;

public:
	typedef map<std::string, shared_ptr<HttpCookie> >::type HttpCookies;

// Construction
public:
	HttpResponse();
	virtual ~HttpResponse();

// Attributes
public:
	inline const HttpCookies & getCookies() const;

	inline uint32 getStatusCode() const;
	inline void setStatusCode(uint32 statusCode);

	void setCookie(const std::string &name, const std::string &value);
	bool removeCookie(const std::string &name);

	void setContentLength(uint32 length);
	std::string getContentType() const;
    void setContentType(const std::string &type);
	void setContentType(HttpContentType type);	
	void setContentEncoding(const std::string &encoding);
	void setContentDisposition(const std::string &filename, boost::optional<uint32> size = boost::none);
    void setDate(const boost::posix_time::ptime &dt);
	void setDateNow();
	void setLastModified(const boost::posix_time::ptime &dt);
	void setWWWBasicAuthenticate(const std::string &realm);
	void setExpires(const boost::posix_time::ptime &dt);
	void setLocation(const std::string &location);
	void setServer(const std::string &server);
	void setAllowMethods();

	const Buffer & getResponseBuffer() const;

	shared_ptr<IStream> getResponseStream() const;
	void setResponseStream(shared_ptr<IStream> responseStream);	

// Operations
public:
	bool writeHeader(shared_ptr<HttpRequest> request);
	bool write(const std::string &str);
	bool write(const void *data, uint32 size);

private:
	// Restituisce l'header codificato in utf8
	std::string _createHeader(shared_ptr<HttpRequest> request);

	void _addStatus(std::string &str);
	void _addParams(std::string &str);
	void _addCookies(std::string &str);

// IHttpMethod interface
protected:
	virtual shared_ptr<HttpData> createHeader(shared_ptr<HttpData> content) const;
	virtual bool parseMethod(const std::string &header);

protected:
	uint32 m_statusCode;
	StringCollection<std::string> m_params;
	HttpCookies m_cookies;
	scoped_ptr<Buffer> m_responseBuffer;
	shared_ptr<IStream> m_responseStream;
};

//////////////////////////////////////////////////////////////////////

inline const HttpResponse::HttpCookies & HttpResponse::getCookies() const { return m_cookies; }

inline uint32 HttpResponse::getStatusCode() const { return m_statusCode; }
inline void HttpResponse::setStatusCode(uint32 statusCode) { m_statusCode = statusCode; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPRESPONSE_H
