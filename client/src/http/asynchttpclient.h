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

#ifndef _OS_HTTP_ASYNCHTTPCLIENT_H
#define _OS_HTTP_ASYNCHTTPCLIENT_H

#include "http.h"
#include "httpurl.h"
#include "ihttpclient.h"
#include "iconnection.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpData;
class HttpRequest;

//////////////////////////////////////////////////////////////////////

class HttpExport AsyncHttpClient : public IConnection,
								   public IHttpClient
{
	typedef IConnection ConnectionBase;
	typedef IHttpClient ClientBase;

protected:
	enum ErrorContext
	{
		errorContextConnection	= 0,
		errorContextResolve		= 1,
		errorContextHandshake	= 2,
		errorContextRequest		= 3,
		errorContextResponse	= 4,
	};

// Internal
private:
	class HttpScope;

// Construction
public:
	AsyncHttpClient(shared_ptr<ConnectionsManager> connectionsManager, shared_ptr<TCPSocket> socket, const HttpUrl &url, uint32 timeout = OS_HTTP_CONNECTION_TIMEOUT_DEFAULT);
	virtual ~AsyncHttpClient();

// Attributes
public:
	inline void setUrl(const HttpUrl &url);

// IConnection interface
public:
	virtual TCPSocket & getSocket();
	virtual void run();

// IConnection overrides
public:
	virtual shared_ptr<ConnectionScope> createScope(const boost::posix_time::time_duration &timeout = boost::posix_time::milliseconds(0));

// Overridables
protected:
	virtual boost::posix_time::time_duration getTimeout() const;

	virtual void onResponseCompleted(shared_ptr<ConnectionScope> scope);
	virtual void onResponseError(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);
	virtual void onError(ErrorContext context, const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);

// Operations
protected:
	void resolveUrl(shared_ptr<ConnectionScope> scope);
	void openConnection(boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope);
	void sendRequest(shared_ptr<ConnectionScope> scope);
	void handleResponseCompleted(shared_ptr<ConnectionScope> scope);

private:
	void connectionCallback(const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope);
	void resolveCallback(const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope);
	void handshakeCallback(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);

	void handleRequestSended(const boost::system::error_code &e, shared_ptr<HttpData> data, shared_ptr<ConnectionScope> scope);
	void handleServerResponse(const boost::system::error_code &e, size_t bytes_transferred, shared_ptr<ConnectionScope> scope);	
	void handleError(ErrorContext context, const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);

// IHttpClient interface
public:
	virtual boost::asio::ssl::context & getSSLContext();

protected:
	HttpUrl m_url;											// Url di notifica
	uint32 m_timeout;										// Timeout in millisecondi di connessione
	shared_ptr<boost::asio::ip::tcp::resolver> m_resolver;
	shared_ptr<boost::asio::ssl::context> m_sslContext;
	shared_ptr<TCPSocket> m_socket;
	shared_ptr<boost::asio::ssl::stream<TCPSocket &> > m_sslStream;
	boost::array<char, OS_HTTP_DEFAULT_BUFFER_SIZE> m_buffer;
};

//////////////////////////////////////////////////////////////////////

inline void AsyncHttpClient::setUrl(const HttpUrl &url) { m_url = url; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTTP_ASYNCHTTPCLIENT_H
