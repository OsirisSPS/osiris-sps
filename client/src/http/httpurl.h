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

#ifndef _OS_HTTP_HTTPURL_H
#define _OS_HTTP_HTTPURL_H

#include "boost/operators.hpp"

//////////////////////////////////////////////////////////////////////

#define OS_HTTP_PROTOCOL_SEPARATOR			"://"
#define OS_HTTP_HTTP_PROTOCOL				"http://"
#define OS_HTTP_HTTP_PORT_DEFAULT			80
#define OS_HTTP_HTTPS_PROTOCOL				"https://"
#define OS_HTTP_HTTPS_PORT_DEFAULT			443
#define OS_HTTP_PATH_DEFAULT				"/"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpExport HttpUrl : public boost::equality_comparable<HttpUrl>
{
// Construction
public:
	HttpUrl();
	HttpUrl(const std::string &url);
	HttpUrl(const HttpUrl &second);
	virtual ~HttpUrl();

// Attributes
public:
	inline const std::string & getUserName() const;
	inline void setUserName(const std::string &username);

	inline const std::string & getPassword() const;
	inline void setPassword(const std::string &password);

	inline const std::string & getHost() const;
	inline void setHost(const std::string &host);

	inline uint16 getPort() const;
	inline void setPort(uint16 port);

	inline const std::string & getPath() const;		
	inline void setPath(const std::string &path);		

	inline const std::string & getParams() const;
	inline void setParams(const std::string &params);
	
	inline bool getSSL() const;
	inline void setSSL(bool ssl);

	bool empty() const;

// Operations
public:
	void clear();
	bool parse(const std::string &url);	

	std::string toString() const;

// Operators
public:
	HttpUrl & operator=(const HttpUrl &second);
	bool operator==(const HttpUrl &second) const;

public:
	static const HttpUrl EMPTY;

private:
	std::string m_username;
	std::string m_password;
	std::string m_host;
	uint16 m_port;
	std::string m_path;		
	std::string m_params;
	bool m_ssl;
};

//////////////////////////////////////////////////////////////////////

inline const std::string & HttpUrl::getUserName() const { return m_username; }
inline void HttpUrl::setUserName(const std::string & username) { m_username = username; }

inline const std::string & HttpUrl::getPassword() const { return m_password; }
inline void HttpUrl::setPassword(const std::string &password) { m_password = password; }

inline const std::string & HttpUrl::getHost() const { return m_host; }
inline void HttpUrl::setHost(const std::string &host) { m_host = host; }

inline uint16 HttpUrl::getPort() const { return m_port; }
inline void HttpUrl::setPort(uint16 port) { m_port = port; }

inline const std::string & HttpUrl::getPath() const { return m_path; }
inline void HttpUrl::setPath(const std::string &path) { m_path = path; }

inline const std::string & HttpUrl::getParams() const { return m_params; }
inline void HttpUrl::setParams(const std::string &params) { m_params = params; }

inline bool HttpUrl::getSSL() const { return m_ssl; }
inline void HttpUrl::setSSL(bool ssl) { m_ssl = ssl; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPURL_H
