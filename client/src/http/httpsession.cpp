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
#include "httpsession.h"

#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "buffer.h"
#include "connectionsmanager.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "file.h"
#include "filesystem.h"
#include "httpparser.h"
#include "httppath.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "ihttpserver.h"
#include "log.h"
#include "mimemanager.h"
#include "networksystem.h"
#include "platformmanager.h"
#include "realtimestats.h"
#include "tcpsocket.h"
#include "unicode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HttpSession::HttpSession(shared_ptr<IHttpServer> server, shared_ptr<TCPSocket> socket, uint32 timeout) : ConnectionBase(server->getConnectionsManager()),
																										 m_server(server),																		  																										 
																										 m_socket(socket)																										 
{
	OS_ASSERT(m_socket != null);
	initSession(timeout);	
}

HttpSession::~HttpSession()
{

}

shared_ptr<HttpBuffer> HttpSession::getRequestBuffer() const
{
	return ensureBuffer(m_requestBuffer);
}

shared_ptr<HttpBuffer> HttpSession::getResponseBuffer() const
{
	return ensureBuffer(m_responseBuffer);
}

TCPSocket & HttpSession::getSocket()
{
	return *m_socket;
}

void HttpSession::run()
{
	readRequest(createScope(boost::posix_time::milliseconds(m_timeout)), false);
}

void HttpSession::redirect(const std::string &url)
{
	OS_ASSERT(getProcessed() == false);

    // Form the body of the response
	std::string body = OS_HTTP_302;
	uint32 length = static_cast<uint32>(body.size());

    // Form the header of the response
	m_response->setStatusCode(httpStatusMovedTemporarily);
	m_response->setDateNow();
	m_response->setServer(getServer()->getName());
	m_response->setAllowMethods();
	m_response->setLocation(url);
	m_response->setContentLength(length);
	m_response->setContentType(httpContentTextHtml);

	m_response->writeHeader(getRequest());
	m_response->write(body);

	handleResponseProcessed(httpStatusMovedPermanently, length);
}

void HttpSession::reportStatusCode(HttpStatusCode status, const std::string &body)
{
	OS_ASSERT(getProcessed() == false);

	const std::string *responseBody = &body;

    std::string defaultBody;
	if(body.empty())
	{
		switch(status)
		{
		case httpStatusBadRequest:				defaultBody = OS_HTTP_400;
												break;

		case httpStatusForbidden:				defaultBody = OS_HTTP_403;
												break;

		case httpStatusNotFound:				defaultBody = OS_HTTP_404;
												break;

		case httpStatusInternalServerError:		defaultBody = OS_HTTP_500;
												break;

		case httpStatusNotImplemented:			defaultBody = OS_HTTP_501;
												break;
		}

		responseBody = &defaultBody;
	}

	OS_ASSERT(responseBody != null);

	uint32 length = static_cast<uint32>(responseBody->size());

    m_response->setStatusCode(status);
    m_response->setDateNow();
	m_response->setServer(getServer()->getName());
    m_response->setAllowMethods();
    m_response->setContentLength(length);
    m_response->setContentType(httpContentTextHtml);

	m_response->writeHeader(getRequest());
	m_response->write(*responseBody);

    handleResponseProcessed(status, length);
}

void HttpSession::transmitHtml(const std::string &html, HttpStatusCode code)
{
	OS_ASSERT(getProcessed() == false);

    m_response->setStatusCode(code);
    m_response->setDateNow();
    m_response->setServer(getServer()->getName());
	m_response->setAllowMethods();

	const void *data = html.data();
	uint32 size = static_cast<uint32>(html.size());
	scoped_ptr<Buffer> compressedHtml;
	
	if( (size != 0) && (m_request->getSupportGzip()) )
	{
		if(MimeManager::instance()->getCompressible(m_request->getContentType()))		
		{
			compressedHtml.reset(OS_NEW Buffer());
			// Comprime l'html della pagina
			if(CryptManager::instance()->compress(compressorTypeGzip, data, size, *compressedHtml.get()))
			{
				// Verifica che la dimensione della versione compressia sia effettivamente inferiore a quella originale
				if(compressedHtml->getSize() < size)
				{
					m_response->setContentEncoding(OS_HTTP_CONTENTENCODING_GZIP);

					data = compressedHtml->getData();
					size = compressedHtml->getSize();
				}
			}
		}
	}	

    m_response->setContentLength(size);
    m_response->setContentType(httpContentTextHtml);

	// Serializza l'header
   	m_response->writeHeader(getRequest());
	// Serializza il body (eventualmente compresso)
	if(size != 0)
		m_response->write(data, size);

    handleResponseProcessed(code, size);
}

void HttpSession::transmitFile(const String &filename, const std::string &mimeType)
{
	OS_ASSERT(getProcessed() == false);

	shared_ptr<File> file(OS_NEW File());
	// Apre il file da trasmettere
	if(file->open(filename, File::ofRead) == false)
	{
		reportStatusCode(httpStatusNotFound);
		return;
	}
	
	// Ottiene la data di ultima modifica del file
	boost::posix_time::ptime timeLastModify(boost::date_time::not_a_date_time);
	file->getStats(null, &timeLastModify);

	// Ottiene la data del server
	boost::posix_time::ptime timeCurrent = utils::system_time();
	if(timeLastModify.is_not_a_date_time() || timeLastModify > timeCurrent)
		// Assicura che la data di ultima modifica del file non sia antecedente a quella del server
		timeLastModify = timeCurrent;

	OS_ASSERT(timeLastModify.is_not_a_date_time() == false);

	// Tronca i millisecondi per il confronto con la data della richiesta
	timeLastModify = utils::ptime_truncate_milliseconds(timeLastModify);

	OS_ASSERT(utils::ptime_milliseconds(timeLastModify) == 0);	// I millisecondi non devono essere esclusi

	// Controlla, nel caso di GET, se la data lato client coincide con quella del server
	if(m_request->getMethod() == httpMethodGet && m_request->getIfModifiedSince() == timeLastModify)
	{
		m_response->setStatusCode(httpStatusNotModified);
		m_response->setDate(timeCurrent);
		m_response->setServer(getServer()->getName());
		m_response->setAllowMethods();
		m_response->setLastModified(timeLastModify);
		m_response->setContentLength(0);
		m_response->setContentType(httpContentTextHtml);
		
		m_response->writeHeader(getRequest());
		// No body is sent for a 304 status

		handleResponseProcessed(httpStatusNotModified, 0);
	}
	else
	{
		uint32 fileSize = static_cast<uint32>(file->size());
		String fileExtension = FileSystem::instance()->getExtension(filename, true);

		String fileMimeType = mimeType;
		if(fileMimeType.empty())
			MimeManager::instance()->getMimeType(fileExtension, fileMimeType);

		// TOFIX
		// Hack: MimeManager have a map mime->extension, but here we have two extension (css,less) with the same mime-type. 
		if(fileExtension == _S("less"))
			fileMimeType = _S("text/css");

		//Form the header of the response
		m_response->setStatusCode(httpStatusOK);
		m_response->setDate(timeCurrent);
		m_response->setServer(getServer()->getName());
		m_response->setAllowMethods();
		if(timeLastModify.is_not_a_date_time() == false)
			m_response->setLastModified(timeLastModify);
		m_response->setContentLength(fileSize);
		m_response->setContentType(fileMimeType.to_ascii());

		// Send the header
		m_response->writeHeader(getRequest());

		if(m_request->getMethod() != httpMethodHead)
			m_response->setResponseStream(file);

		handleResponseProcessed(httpStatusOK, fileSize);
	}
}

void HttpSession::transmitStream(shared_ptr<IStream> stream, const String &file_name, const String &mime_type, const boost::posix_time::ptime &date)
{
	OS_ASSERT(getProcessed() == false);

	OS_ASSERT(stream != null);
	uint32 size = static_cast<uint32>(stream->size());

	if(m_request->getMethod() == httpMethodGet && utils::ptime_is_valid(date) && m_request->hasModifiedSince() && date == m_request->getIfModifiedSince())
    {
		//Form the header
		m_response->setStatusCode(httpStatusNotModified);
		m_response->setDateNow();
		m_response->setServer(getServer()->getName());
		m_response->setAllowMethods();
		m_response->setLastModified(date);
		m_response->setContentLength(0);
		m_response->setContentType(mime_type.to_ascii());
		m_response->setContentDisposition(file_name.to_ascii(), size);

		m_response->writeHeader(getRequest());
		//No body is sent for a 304 status

		handleResponseProcessed(httpStatusNotModified, 0);
    }
	else
	{
		m_response->setStatusCode(httpStatusOK);
		m_response->setDateNow();
		m_response->setServer(getServer()->getName());
		m_response->setAllowMethods();
		if(utils::ptime_is_valid(date))
			m_response->setLastModified(date);
		m_response->setContentLength(size);
		m_response->setContentType(mime_type.to_ascii());
		m_response->setContentDisposition(file_name.to_ascii(), size);

		// Send the header
		m_response->writeHeader(getRequest());

		// Send back the file contents (if not a HEAD request)
		if(m_request->getMethod() != httpMethodHead)
			m_response->setResponseStream(stream);
		
		handleResponseProcessed(httpStatusOK, size);
	}
}

void HttpSession::transmitBuffer(const Buffer &buffer, const String &file_name, const String &mime_type)
{
	transmitBuffer(buffer, file_name, mime_type, constants::time_null);
}

void HttpSession::transmitBuffer(const Buffer &buffer, const String &file_name, const String &mime_type, const boost::posix_time::ptime &date)
{
	transmitBufferPtr(buffer.getData(), buffer.getSize(), file_name, mime_type, date);
}

void HttpSession::transmitBufferPtr(const void *buffer, uint32 size, const String &file_name, const String &mime_type, const boost::posix_time::ptime &date)
{
	OS_ASSERT(getProcessed() == false);

	if(m_request->getMethod() == httpMethodGet && utils::ptime_is_valid(date) && m_request->hasModifiedSince() && date == m_request->getIfModifiedSince())
    {
		//Form the header
		m_response->setStatusCode(httpStatusNotModified);
		m_response->setDateNow();
		m_response->setServer(getServer()->getName());
		m_response->setAllowMethods();
		m_response->setLastModified(date);
		m_response->setContentLength(0);
		m_response->setContentType(mime_type.to_ascii());
		if(file_name.empty() == false)
			m_response->setContentDisposition(file_name.to_ascii(), size);

		m_response->writeHeader(getRequest());
		//No body is sent for a 304 status

		handleResponseProcessed(httpStatusNotModified, 0);
    }
	else
	{
		m_response->setStatusCode(httpStatusOK);
		m_response->setDateNow();
		m_response->setServer(getServer()->getName());
		m_response->setAllowMethods();
		if(utils::ptime_is_valid(date))
			m_response->setLastModified(date);
		m_response->setContentLength(size);
		m_response->setContentType(mime_type.to_ascii());
		if(file_name.empty() == false)
			m_response->setContentDisposition(file_name.to_ascii(), size);

		// Send the header
		m_response->writeHeader(getRequest());

		// Send back the file contents (if not a HEAD request)
		if(m_request->getMethod() != httpMethodHead)
			m_response->write(buffer, size);
		
		handleResponseProcessed(httpStatusOK, size);
	}
}

void HttpSession::handleRead(const boost::system::error_code &e, size_t bytes_transferred, shared_ptr<ConnectionScope> scope, bool keepAlive)
{
	scope->cancelTimeout();

	if(e)
	{
		// TODO: corretto? logga l'errore solo nel caso in cui non derivi da una sessione di keep-alive...
		// Clodo 28/12/2013: ?
		//if(keepAlive == false)
			handleError(e);

		return;
	}

	shared_ptr<HttpBuffer> requestBuffer = getRequestBuffer();
	if(requestBuffer == null)
	{
		handleError("invalid request buffer");
		return;
	}

	switch(m_request->writeData(requestBuffer->data(), static_cast<uint32>(bytes_transferred)))
	{
	case HttpRequest::statusWaitingHeader:
	case HttpRequest::statusWaitingContent:
												getSocket().async_read_some(boost::asio::buffer(*requestBuffer), boost::bind(&HttpSession::handleRead, get_this_ptr<HttpSession>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, scope->extendTimeout(), false));
												break;

	case HttpRequest::statusCompleted:			handleRequestCompleted(scope);
												break;

	case HttpRequest::statusError:				handleError(_S("invalid request"));
												break;

	default:									handleError(_S("internal error"));
												break;
	}
}

void HttpSession::handleResponseBufferCompleted(const boost::system::error_code &e, size_t bytes_transferred, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
	{
		handleError(e);
		return;
	}

	shared_ptr<IStream> responseStream = m_response->getResponseStream();
	if(responseStream != null)
	{
		if(responseStream->seekToBegin())				
			sendResponseStream(responseStream, scope);
		else
			handleError("stream seek error");			
	}
	else
	{
		handleResponseCompleted(scope);			
	}
}

void HttpSession::handleWriteResponseStream(const boost::system::error_code &e, size_t bytes_transferred, shared_ptr<ConnectionScope> scope, shared_ptr<IStream> stream)
{
	scope->cancelTimeout();

	if(e)
	{
		handleError(e);
		return;
	}

	sendResponseStream(stream, scope);
}

void HttpSession::handleError(const boost::system::error_code &e)
{
	if(isAsioOperationAborted(e) == false)
		handleError(e.message().c_str());
}

void HttpSession::handleError(const String &error)
{
	String description("HttpSession error");
	if(error.empty() == false)
	{
		description.append(": ");
		description.append(error);
	}
		
	OS_LOG_ERROR(description);	
}

void HttpSession::handleRequestCompleted(shared_ptr<ConnectionScope> scope)
{
	OS_LOG_NOTICE(boost::format("WebServer: Request, url: %s") % m_request->getRawUrl());	

	// Processa la richiesta
	processRequest();
	// Invia la risposta
	sendResponse(scope);	
}

void HttpSession::handleResponseCompleted(shared_ptr<ConnectionScope> scope)
{
	if(m_request->getCloseConnection() == false)
	{
		initSession(m_request->getKeepAlive());
		readRequest(scope->extendTimeout(boost::posix_time::milliseconds(m_timeout)), true);
	}
}

void HttpSession::handleResponseProcessed(HttpStatusCode code, uint32 length)
{
	m_processed = true;

	OS_LOG_NOTICE(boost::format("WebServer: %s, length: %d, url: %s") % conversions::to_string(code) % length % m_request->getRawUrl());	
}

void HttpSession::processRequest()
{
	try
	{
		if(m_request->getAuthorization() == httpAuthorizationPlainText)
		{
			reportStatusCode(httpStatusNotImplemented);
		}
		else
		{
			switch(m_request->getMethod())
			{
			case httpMethodGet:
			case httpMethodHead:
			case httpMethodPost:
										processRequest(m_request->getUrl());
										break;

			default:					reportStatusCode(httpStatusNotImplemented);
										break;
			}
		}
	}	
	catch(std::exception &e)
	{
		OS_LOG_ERROR(_S("HttpSession error: ") + e.what());
		reportStatusCode(httpStatusInternalServerError);
	}
	catch(...)
	{
		OS_LOG_ERROR("Unknown HttpSession error");
		reportStatusCode(httpStatusInternalServerError);
	}
}

void HttpSession::processRequest(const String &url)
{
	OS_TRACE("\r\nprocessRequest : " + url.to_ascii()); // COOKIEURGENT

	HttpPath path(url);

	shared_ptr<IHttpServer> server = getServer();
	OS_ASSERT(server != null);

	// Carica lo stato della sessione
	m_state = server->getSessionState(get_this_ptr<HttpSession>(), path);

	// Processa la richiesta
	RealtimeStatsScopeTimer RSS(_S("Activity"), _S("Web request"));
	if(server->processRequest(get_this_ptr<HttpSession>(), path) == false)
		reportStatusCode(httpStatusNotFound);
}

void HttpSession::initSession(uint32 timeout)
{
	m_state.reset();
	m_request.reset(OS_NEW HttpRequest());
	m_response.reset(OS_NEW HttpResponse());
	m_timeout = utils::limit<uint32>(timeout, OS_HTTP_CONNECTION_TIMEOUT_MIN, OS_HTTP_CONNECTION_TIMEOUT_MAX);	
	m_processed = false;
}

void HttpSession::readRequest(shared_ptr<ConnectionScope> scope, bool keepAlive)
{
	shared_ptr<HttpBuffer> requestBuffer = getRequestBuffer();
	if(requestBuffer == null)
	{
		handleError("invalid request buffer");
		return;
	}

	getSocket().async_read_some(boost::asio::buffer(*requestBuffer), boost::bind(&HttpSession::handleRead, get_this_ptr<HttpSession>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, scope, keepAlive));
}

void HttpSession::sendResponse(shared_ptr<ConnectionScope> scope)
{
	const Buffer &responseBuffer = m_response->getResponseBuffer();
	if(responseBuffer.empty() == false)
	{
		boost::asio::async_write(getSocket(), boost::asio::buffer(responseBuffer.getData(), responseBuffer.getSize()), boost::asio::transfer_all(), boost::bind(&HttpSession::handleResponseBufferCompleted, get_this_ptr<HttpSession>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, scope->extendTimeout()));		
	}
	else
	{
		OS_ASSERT(m_response->getResponseStream() == null);
		handleResponseCompleted(scope);			
	}
}

void HttpSession::sendResponseStream(shared_ptr<IStream> stream, shared_ptr<ConnectionScope> scope)
{
	OS_ASSERT(stream != null);
	OS_ASSERT(scope != null);

	shared_ptr<HttpBuffer> responseBuffer = getResponseBuffer();
	if(responseBuffer == null)
	{
		handleError("invalid request buffer");
		return;
	}

	uint32 size = stream->read(responseBuffer->data(), OS_HTTP_DEFAULT_BUFFER_SIZE);
	if(size > 0)
		boost::asio::async_write(getSocket(), boost::asio::buffer(responseBuffer->data(), size), boost::asio::transfer_all(), boost::bind(&HttpSession::handleWriteResponseStream, get_this_ptr<HttpSession>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, scope->extendTimeout(), stream));		
	else
		handleResponseCompleted(scope);
}

shared_ptr<HttpBuffer> HttpSession::ensureBuffer(shared_ptr<HttpBuffer> &buffer) const
{
	if(buffer == null)
	{
		shared_ptr<IHttpServer> server = getServer();
		if(server != null)
			buffer = getServer()->peekHttpBuffer();
	}
	
	return buffer;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
