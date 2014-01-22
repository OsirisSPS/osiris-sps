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

#ifndef _OS_HTTP_HTTPSOCKETPROXY_H
#define _OS_HTTP_HTTPSOCKETPROXY_H

#include "http.h"
#include "isocketproxy.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpResponse;

//////////////////////////////////////////////////////////////////////

class HttpExport HttpSocketProxy : public ISocketProxy<boost::asio::ip::tcp>
{
public:
	typedef ISocketProxy<boost::asio::ip::tcp> ProxyBase;
	typedef ProxyBase::socket_type socket_type;
	typedef ProxyBase::endpoint_type endpoint_type;
	typedef ProxyBase::callback_type callback_type;

// Construction
public:
	HttpSocketProxy(shared_ptr<socket_type> socket);
	virtual ~HttpSocketProxy();

// Attributes
public:
	inline bool getEnableHttpConnect() const;
	inline void setEnableHttpConnect(bool enableHttpConnect);

	inline const std::string & getProxyUsername() const;
	inline void setProxyUsername(const std::string &proxyUsername);

	inline const std::string & getProxyPassword() const;
	inline void setProxyPassword(const std::string &proxyPassword);

// Operations
private:
	bool initRequest();
	void handshake1(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void handshake2(const boost::system::error_code &e, size_t bytes_transferred, shared_ptr<callback_type> callback);

private:
	boost::system::error_code convertHttpErrorCode(uint32 httpErrorCode);

// ISocketProxy interface
protected:
	virtual void proxy_connect(boost::system::error_code &e) OS_OVERRIDE;
	virtual void proxy_async_connect(const boost::system::error_code &e, shared_ptr<callback_type> callback) OS_OVERRIDE;

protected:
	bool m_enableHttpConnect;
	std::string m_proxyUsername;
	std::string m_proxyPassword;
	std::string m_request;
	shared_ptr<HttpResponse> m_response;
	boost::array<char, OS_HTTP_DEFAULT_BUFFER_SIZE> m_buffer;
};

//////////////////////////////////////////////////////////////////////

inline bool HttpSocketProxy::getEnableHttpConnect() const { return m_enableHttpConnect; }
inline void HttpSocketProxy::setEnableHttpConnect(bool enableHttpConnect) { m_enableHttpConnect = enableHttpConnect; }

inline const std::string & HttpSocketProxy::getProxyUsername() const { return m_proxyUsername; }
inline void HttpSocketProxy::setProxyUsername(const std::string &proxyUsername) { m_proxyUsername = proxyUsername; }

inline const std::string & HttpSocketProxy::getProxyPassword() const { return m_proxyPassword; }
inline void HttpSocketProxy::setProxyPassword(const std::string &proxyPassword) { m_proxyPassword = proxyPassword; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTTP_HTTPSOCKETPROXY_H
