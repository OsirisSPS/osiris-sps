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

#ifndef _OS_HTTP_HTTPSESSION_H
#define _OS_HTTP_HTTPSESSION_H

#include "constants.h"
#include "buffer.h"
#include "datetime.h"
#include "enable_this_ptr.h"
#include "http.h"
#include "iconnection.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IHttpServer;
class IStream;
class HttpRequest;
class HttpResponse;
class HttpSessionState;

//////////////////////////////////////////////////////////////////////

class HttpExport HttpSession : public IConnection
{
	typedef IConnection ConnectionBase;

// Construction
public:
	HttpSession(shared_ptr<IHttpServer> server, shared_ptr<TCPSocket> socket, uint32 timeout = OS_HTTP_CONNECTION_TIMEOUT_DEFAULT);
	virtual ~HttpSession();

// Attributes
public:
	inline shared_ptr<IHttpServer> getServer() const;

	inline shared_ptr<HttpRequest> getRequest() const;
	inline shared_ptr<HttpResponse> getResponse() const;
	inline shared_ptr<HttpSessionState> getState() const;

	inline bool getProcessed() const;

protected:
	shared_ptr<HttpBuffer> getRequestBuffer() const;
	shared_ptr<HttpBuffer> getResponseBuffer() const;

// IConnection interface
public:
	virtual TCPSocket & getSocket();
	virtual void run();	

// Operations
public:
	void redirect(const std::string &url);
	
	void reportStatusCode(HttpStatusCode status, const std::string &body = constants::empty_string);
		
	void transmitHtml(const std::string &html, HttpStatusCode code);
	void transmitFile(const String &filename, const std::string &mimeType = constants::empty_string);
	void transmitStream(shared_ptr<IStream> stream, const String &file_name, const String &mime_type, const boost::posix_time::ptime &date = constants::time_null);	
	void transmitBuffer(const Buffer &buffer, const String &file_name, const String &mime_type);	
	void transmitBuffer(const Buffer &buffer, const String &file_name, const String &mime_type, const boost::posix_time::ptime &date);	
	void transmitBufferPtr(const void *buffer, uint32 size, const String &file_name, const String &mime_type, const boost::posix_time::ptime &date = constants::time_null);	
	

private:
	void handleRead(const boost::system::error_code &e, size_t bytes_transferred, shared_ptr<ConnectionScope> scope, bool keepAlive);
	void handleResponseBufferCompleted(const boost::system::error_code &e, size_t bytes_transferred, shared_ptr<ConnectionScope> scope);
	void handleWriteResponseStream(const boost::system::error_code &e, size_t bytes_transferred, shared_ptr<ConnectionScope> scope, shared_ptr<IStream> stream);
	void handleError(const boost::system::error_code &e);
	void handleError(const String &error);
	void handleRequestCompleted(shared_ptr<ConnectionScope> scope);
	void handleResponseCompleted(shared_ptr<ConnectionScope> scope);
	void handleResponseProcessed(HttpStatusCode code, uint32 length);

	void processRequest();
	void processRequest(const String &url);	
	void initSession(uint32 timeout);
	void readRequest(shared_ptr<ConnectionScope> scope, bool keepAlive);
	void sendResponse(shared_ptr<ConnectionScope> scope);
	void sendResponseStream(shared_ptr<IStream> stream, shared_ptr<ConnectionScope> scope);
	
	shared_ptr<HttpBuffer> ensureBuffer(shared_ptr<HttpBuffer> &buffer) const;

private:
	mutable shared_ptr<HttpBuffer> m_requestBuffer;		// N.B.: sono necessari due buffers distinti per richiesta e risposta per il supporto all'HTTP pipeline
	mutable shared_ptr<HttpBuffer> m_responseBuffer;
	shared_ptr<HttpSessionState> m_state;
	weak_ptr<IHttpServer> m_server;
	shared_ptr<HttpRequest> m_request;
	shared_ptr<HttpResponse> m_response;
	uint32 m_timeout;		
	shared_ptr<TCPSocket> m_socket;	
	bool m_processed;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IHttpServer> HttpSession::getServer() const { return m_server.lock(); }

inline shared_ptr<HttpRequest> HttpSession::getRequest() const { return m_request; }
inline shared_ptr<HttpResponse> HttpSession::getResponse() const { return m_response; }
inline shared_ptr<HttpSessionState> HttpSession::getState() const { return m_state; }

inline bool HttpSession::getProcessed() const { return m_processed; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPSESSION_H
