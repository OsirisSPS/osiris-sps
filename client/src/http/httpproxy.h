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

#ifndef _OS_HTTP_HTTPPROXY_H
#define _OS_HTTP_HTTPPROXY_H

#include "base/object.h"
#include "boostasio_ssl.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class HttpRequest;
class HttpUrl;
class TCPSocket;

//////////////////////////////////////////////////////////////////////

class HttpExport HttpProxy : public Object
{
// Construction
public:
    HttpProxy(shared_ptr<boost::asio::io_service> service, shared_ptr<TCPSocket> socket);
	virtual ~HttpProxy();

// Attributes
public:
	const Buffer & getResponse() const;

// Operations
public:
	bool perform(shared_ptr<HttpRequest> request);

private:
	bool perform(const HttpUrl &url, const Buffer &rawRequest);

private:
	shared_ptr<boost::asio::io_service> m_service;
	scoped_ptr<Buffer> m_rawResponse;
	shared_ptr<TCPSocket> m_socket;
	shared_ptr<boost::asio::ssl::context> m_sslContext;	
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTTP_HTTPPROXY_H
