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

#ifndef _OS_HTTP_IHTTPCLIENT_H
#define _OS_HTTP_IHTTPCLIENT_H

#include "base/object.h"
#include "boostasio_ssl.h"
#include "constants.h"

//////////////////////////////////////////////////////////////////////

#define OS_IHTTPCLIENT_MAX_REDIRECTS		5

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpRequest;
class HttpResponse;
class HttpUrl;

//////////////////////////////////////////////////////////////////////

class HttpExport IHttpClient : public Object
{
// Construction
public:
	IHttpClient(const std::string &url = constants::empty_string);
	virtual ~IHttpClient();

// Attributes
public:
	inline shared_ptr<HttpRequest> getRequest() const;
	inline shared_ptr<HttpResponse> getResponse() const;

	inline bool getFollowRedirects() const;
	inline void setFollowRedirects(bool followRedirects);

	inline uint32 getMaxRedirects() const;
	inline void setMaxRedirects(uint32 maxRedirects);

// Operations
public:
	bool loadVerifyFile(const std::string &filename, boost::asio::ssl::context_base::verify_mode verifyMode = boost::asio::ssl::context::verify_peer | boost::asio::ssl::context::verify_client_once | boost::asio::ssl::context::verify_fail_if_no_peer_cert);

protected:
	void initRedirect(const HttpUrl &redirectUrl, shared_ptr<HttpRequest> request, shared_ptr<HttpResponse> response);

// Interface
public:
	virtual boost::asio::ssl::context & getSSLContext() = 0;

protected:
	shared_ptr<HttpRequest> m_request;
	shared_ptr<HttpResponse> m_response;
	bool m_followRedirects;
	uint32 m_maxRedirects;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<HttpRequest> IHttpClient::getRequest() const { return m_request; }
inline shared_ptr<HttpResponse> IHttpClient::getResponse() const { return m_response; }

inline bool IHttpClient::getFollowRedirects() const { return m_followRedirects; }
inline void IHttpClient::setFollowRedirects(bool followRedirects) { m_followRedirects = followRedirects; }

inline uint32 IHttpClient::getMaxRedirects() const { return m_maxRedirects; }
inline void IHttpClient::setMaxRedirects(uint32 maxRedirects) { m_maxRedirects = maxRedirects; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTTP_IHTTPCLIENT_H
