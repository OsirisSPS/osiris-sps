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

#ifndef _OS_HTTP_HTTPREQUEST_H
#define _OS_HTTP_HTTPREQUEST_H

#include "boost/date_time/posix_time/posix_time.hpp"
#include "collections.h"
#include "constants.h"
#include "ihttpmethod.h"
#include "ordered_map.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpPostParam;
class HttpUrl;

//////////////////////////////////////////////////////////////////////

class HttpExport HttpRequest : public IHttpMethod
{
	typedef IHttpMethod MethodBase;

public:
	typedef Collections<std::wstring, HttpPostParam>::shared_collection HttpPostParams;

// Construction
public:
	HttpRequest(HttpMethod method = httpMethodUnknown, const std::string &rawurl = constants::empty_string);
	virtual ~HttpRequest();

// Attributes
public:
	inline HttpMethod getMethod() const;
	inline void setMethod(HttpMethod method);

	inline const std::string & getHost() const;
	inline void setHost(const std::string &host);

	inline const uint32 & getPort() const;
	inline void setPort(const uint32 &port);

	inline const std::string & getUserAgent() const;
	inline void setUserAgent(const std::string &userAgent);

	inline const std::string & getAccept() const;
	inline void setAccept(const std::string &accept);

	inline const std::string & getAcceptLanguage() const;
	inline void setAcceptLanguage(const std::string &acceptLanguage);

	inline const std::string & getAcceptEncoding() const;
	inline void setAcceptEncoding(const std::string &acceptEncoding);

	inline const std::string & getAcceptCharset() const;
	inline void setAcceptCharset(const std::string &acceptCharset);

	inline const boost::posix_time::ptime & getIfModifiedSince() const;
	inline void setIfModifiedSince(const boost::posix_time::ptime &t);

	inline const std::string & getContentType() const;
	inline void setContentType(const std::string &contentType);

	inline const std::string & getUrl() const;
	inline const std::string & getRawUrl() const;

	inline bool getSupportGzip() const;

	inline uint32 getKeepAlive() const;
	
	inline bool getCloseConnection() const;
	inline void setCloseConnection(bool closeConnection);

	inline HttpAuthorization getAuthorization() const;
	inline void setAuthorization(HttpAuthorization authorization);

	inline const ordered_map<std::wstring, std::wstring> & getUrlParams() const;
	const std::wstring & getUrlParam(const std::wstring &name) const;
	inline bool hasUrlParam(const std::wstring &name) const;

	inline const HttpPostParams & getPostParams() const;
	inline shared_ptr<HttpPostParam> getPostParam(const std::wstring &name) const;
	inline bool hasPostParam(const std::wstring &name) const;

	inline const StringCollection<std::string> & getCookies() const;

	inline const std::string & getCookie(const std::string &name) const;
	inline void setCookie(const std::string &name, const std::string &value);
	inline bool hasCookie(const std::string &name) const;

	inline bool hasModifiedSince() const;

	std::wstring getPostParamString(const std::wstring &name, const std::wstring &def = constants::empty_wstring) const;
	const Buffer * getPostParamBuffer(const std::wstring &name) const;
	void setPostParamString(const std::wstring &name, const std::wstring &value);
	void setPostParamBuffer(const std::wstring &name, const Buffer &value);

	void setUrl(const HttpUrl &url);

// Operations
private:
	void parseUrlParams();

public:
	void addPostParam(const std::wstring &name, shared_ptr<HttpPostParam> param);
	void parsePostParams();
	void parsePostParams(const std::string &params, const std::string &contentType, const std::string &contentBoundary = constants::empty_string);

// IHttpMethod interface
protected:
	virtual shared_ptr<HttpData> createHeader(shared_ptr<HttpData> content) const;
	virtual bool parseMethod(const std::string &header);

// IHttpMethod overrides
protected:
	virtual shared_ptr<HttpData> createContent() const;

	virtual bool parseParam(const std::string &paramName, const std::string &paramValue);
	virtual bool parseContent();

private:
	HttpMethod m_method;
	std::string m_url;							
	std::string m_rawurl;						
	std::string m_extra;						
	boost::posix_time::ptime m_modifiedSince;	
	std::string m_acceptLanguage;					
	std::string m_acceptEncoding;
	std::string m_accept;							
	std::string m_acceptCharset;						
	std::string m_userAgent;					
	bool m_supportGzip;							
	HttpAuthorization m_authorization;			
	std::string m_username;							
	std::string m_password;							
	uint32 m_keepAlive;
	bool m_closeConnection;
	mutable std::string m_contentBoundary;
	mutable std::string m_contentType;
	mutable std::string m_contentBoundaryTerminator;
	std::string m_host;
	uint32 m_port;
	ordered_map<std::wstring, std::wstring> m_urlParams;
	HttpPostParams m_postParams;
	StringCollection<std::string> m_cookies;
};

//////////////////////////////////////////////////////////////////////

inline HttpMethod HttpRequest::getMethod() const { return m_method; }
inline void HttpRequest::setMethod(HttpMethod method) { m_method = method; }

inline const std::string & HttpRequest::getHost() const { return m_host; }
inline void HttpRequest::setHost(const std::string &host) { m_host = host; }

inline const uint32 & HttpRequest::getPort() const { return m_port; }
inline void HttpRequest::setPort(const uint32 &port) { m_port = port; }

inline const std::string & HttpRequest::getUserAgent() const { return m_userAgent; }
inline void HttpRequest::setUserAgent(const std::string &userAgent) { m_userAgent = userAgent; }

inline const std::string & HttpRequest::getAccept() const { return m_accept; }
inline void HttpRequest::setAccept(const std::string &accept) { m_accept = accept; }

inline const std::string & HttpRequest::getAcceptLanguage() const { return m_acceptLanguage; }
inline void HttpRequest::setAcceptLanguage(const std::string &acceptLanguage) { m_acceptLanguage = acceptLanguage; }

inline const std::string & HttpRequest::getAcceptEncoding() const { return m_acceptEncoding; }
inline void HttpRequest::setAcceptEncoding(const std::string &acceptEncoding) { m_acceptEncoding = acceptEncoding; }

inline const std::string & HttpRequest::getAcceptCharset() const { return m_acceptCharset; }
inline void HttpRequest::setAcceptCharset(const std::string &acceptCharset) { m_acceptCharset = acceptCharset; }

inline const boost::posix_time::ptime & HttpRequest::getIfModifiedSince() const { return m_modifiedSince; }
inline void HttpRequest::setIfModifiedSince(const boost::posix_time::ptime &t) { m_modifiedSince = t; }

inline const std::string & HttpRequest::getContentType() const { return m_contentType; }
inline void HttpRequest::setContentType(const std::string &contentType) { m_contentType = contentType; }

inline const std::string & HttpRequest::getUrl() const { return m_url; }
inline const std::string & HttpRequest::getRawUrl() const { return m_rawurl; }

inline bool HttpRequest::getSupportGzip() const { return m_supportGzip; }

inline uint32 HttpRequest::getKeepAlive() const { return m_keepAlive; }

inline bool HttpRequest::getCloseConnection() const { return m_closeConnection; }
inline void HttpRequest::setCloseConnection(bool closeConnection) { m_closeConnection = closeConnection; }

inline HttpAuthorization HttpRequest::getAuthorization() const { return m_authorization; }
inline void HttpRequest::setAuthorization(HttpAuthorization authorization) { m_authorization = authorization; }

inline const ordered_map<std::wstring, std::wstring> & HttpRequest::getUrlParams() const { return m_urlParams; }
inline bool HttpRequest::hasUrlParam(const std::wstring &name) const { return m_urlParams.exists(name); }

inline const HttpRequest::HttpPostParams & HttpRequest::getPostParams() const { return m_postParams; }
inline shared_ptr<HttpPostParam> HttpRequest::getPostParam(const std::wstring &name) const { return m_postParams.get(name); }
inline bool HttpRequest::hasPostParam(const std::wstring &name) const { return m_postParams.exists(name); }

inline const StringCollection<std::string> & HttpRequest::getCookies() const { return m_cookies; }

inline const std::string & HttpRequest::getCookie(const std::string &name) const { return m_cookies.get(name); }
inline void HttpRequest::setCookie(const std::string &name, const std::string &value) { m_cookies.set(name, value); }
inline bool HttpRequest::hasCookie(const std::string &name) const { return m_cookies.exists(name); }

inline bool HttpRequest::hasModifiedSince() const { return m_modifiedSince.is_not_a_date_time() == false; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTTP_HTTPREQUEST_H
