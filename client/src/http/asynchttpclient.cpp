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
#include "asynchttpclient.h"

#include "boost/bind.hpp"
#include "connectionsmanager.h"
#include "conversions.h"
#include "httpdata.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "log.h"
#include "tcpsocket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class AsyncHttpClient::HttpScope : public ConnectionScope
{
	typedef ConnectionScope ScopeBase;

// Construction
public:
	HttpScope(shared_ptr<boost::asio::io_service> service, shared_ptr<IListener> listener, uint32 redirects);
	virtual ~HttpScope();

// Attributes
public:
	inline uint32 getRedirects() const;
	inline void setRedirects(uint32 redirects);

private:
	uint32 m_redirects;
};

//////////////////////////////////////////////////////////////////////

AsyncHttpClient::HttpScope::HttpScope(shared_ptr<boost::asio::io_service> service, shared_ptr<IListener> listener, uint32 redirects) : ScopeBase(service, listener),
																																	   m_redirects(redirects)
{

}

AsyncHttpClient::HttpScope::~HttpScope()
{

}

inline uint32 AsyncHttpClient::HttpScope::getRedirects() const
{
	return m_redirects;
}

inline void AsyncHttpClient::HttpScope::setRedirects(uint32 redirects)
{
	m_redirects = redirects;
}

//////////////////////////////////////////////////////////////////////

AsyncHttpClient::AsyncHttpClient(shared_ptr<ConnectionsManager> connectionsManager, shared_ptr<TCPSocket> socket, const HttpUrl &url, uint32 timeout) : ConnectionBase(connectionsManager),
																																						ClientBase(url.getPath()),
																																						m_url(url),
																																						m_timeout(timeout),
																																						m_socket(socket)
{
	shared_ptr<boost::asio::io_service> service = connectionsManager->getService();
	OS_ASSERT(service != null);

	m_resolver = createAsioObject<boost::asio::ip::tcp::resolver>(*service);
	m_sslContext = createAsioSSLObject<boost::asio::ssl::context, boost::asio::ssl::context_base::method>(*service, boost::asio::ssl::context::sslv23);
//	m_sslSocket = createAsioSSLObject<boost::asio::ssl::stream<TCPSocket>, boost::asio::ssl::context &>(*service, *m_sslContext);
	// VERYURGENT: verificare se sotto Linux da comunque i problemi riportati nelle note di asio.h/asio_ssl.h (in tal caso creare un wrapper di allocazione/deallocazione)
	OS_ASSERT(m_socket != null);
	m_sslStream.reset(OS_NEW_T(boost::asio::ssl::stream<TCPSocket &>)(*m_socket, *m_sslContext), os_delete_t());
}

AsyncHttpClient::~AsyncHttpClient()
{
	
}

TCPSocket & AsyncHttpClient::getSocket()
{
	return m_sslStream->next_layer();
}

void AsyncHttpClient::run()
{
	resolveUrl(createScope(getTimeout()));
}

shared_ptr<ConnectionScope> AsyncHttpClient::createScope(const boost::posix_time::time_duration &timeout)
{
	shared_ptr<ConnectionScope> scope(OS_NEW HttpScope(getService(), get_this_ptr(), getFollowRedirects() ? getMaxRedirects() : 0));
	scope->extendTimeout(timeout);
	return scope;
}

boost::posix_time::time_duration AsyncHttpClient::getTimeout() const
{
	return boost::posix_time::milliseconds(m_timeout);
}

void AsyncHttpClient::onResponseCompleted(shared_ptr<ConnectionScope> scope)
{

}

void AsyncHttpClient::onResponseError(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{

}

void AsyncHttpClient::onError(ErrorContext context, const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{
	OS_ASSERT(isAsioOperationAborted(e) == false);

	OS_LOG_WARNING(_S("Http error: ") + e.message().c_str() + _S(" (url='") + m_url.toString() + _S("')"));
}

void AsyncHttpClient::resolveUrl(shared_ptr<ConnectionScope> scope)
{
	m_request->setUrl(m_url);
	if(m_url.empty())
	{
		OS_ASSERTFALSE();
		return;
	}

	boost::asio::ip::tcp::resolver::query query(m_url.getHost(), conversions::to_string(m_url.getPort()));
	m_resolver->async_resolve(query, boost::bind(&AsyncHttpClient::resolveCallback, get_this_ptr<AsyncHttpClient>(), boost::asio::placeholders::error, boost::asio::placeholders::iterator, scope));
}

void AsyncHttpClient::openConnection(boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope)
{
	OS_ASSERT(endpoint != boost::asio::ip::tcp::resolver::iterator());

	close();
	// N.B.: creare un endpoint temporaneo o *endpoint verrebbe deferenziato dopo l'incremento di ++endpoint (con assert/crash se a quel punto è in posizione finale)
	getSocket().async_connect(*endpoint, boost::bind(&AsyncHttpClient::connectionCallback, get_this_ptr<AsyncHttpClient>(), boost::asio::placeholders::error, ++boost::asio::ip::tcp::resolver::iterator(endpoint), scope->extendTimeout(getTimeout())));
}

void AsyncHttpClient::sendRequest(shared_ptr<ConnectionScope> scope)
{
	shared_ptr<HttpData> data = m_request->serialize();
	if(data == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	if(m_url.getSSL())
		boost::asio::async_write(*m_sslStream, boost::asio::buffer(data->getData(), data->getSize()), boost::asio::transfer_all(), boost::bind(&AsyncHttpClient::handleRequestSended, get_this_ptr<AsyncHttpClient>(), boost::asio::placeholders::error, data, scope->extendTimeout(getTimeout())));
	else
		boost::asio::async_write(getSocket(), boost::asio::buffer(data->getData(), data->getSize()), boost::asio::transfer_all(), boost::bind(&AsyncHttpClient::handleRequestSended, get_this_ptr<AsyncHttpClient>(), boost::asio::placeholders::error, data, scope->extendTimeout(getTimeout())));
}

void AsyncHttpClient::handleResponseCompleted(shared_ptr<ConnectionScope> scope)
{
	OS_ASSERT(m_request != null);
	OS_ASSERT(m_response != null);	

	switch(m_response->getStatusCode())
	{
	case httpStatusMovedPermanently:
	case httpStatusMovedTemporarily:	
										{
											shared_ptr<HttpScope> httpScope = boost::dynamic_pointer_cast<HttpScope>(scope);
											OS_ASSERT(httpScope != null);
											if(httpScope != null)
											{
												uint32 redirects = httpScope->getRedirects();
												if(redirects > 0)													
												{
													const std::string &rawRedirectUrl = m_response->getRawParam(OS_HTTP_PARAM_LOCATION);
													OS_ASSERT(rawRedirectUrl.empty() == false);
													HttpUrl redirectUrl;
													if((rawRedirectUrl.empty() == false) && redirectUrl.parse(rawRedirectUrl))
													{
														initRedirect(redirectUrl, m_request, m_response);	
														setUrl(redirectUrl);
														httpScope->setRedirects(redirects - 1);
														resolveUrl(httpScope);
														
														return;
													}
													else
													{
														OS_LOG_WARNING("Invalid redirect url '" + rawRedirectUrl + "'");
													}												
												}												
											}
										}
										
										break;
	}

	onResponseCompleted(scope);
}

void AsyncHttpClient::connectionCallback(const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
	{
		// Connessione fallita
		handleError(errorContextConnection, e, scope);
	}
	else if(endpoint != boost::asio::ip::tcp::resolver::iterator())
	{
		// Connessione fallita, altri endpoints disponibili
		openConnection(endpoint, scope);
	}
	else
	{
		if(m_url.getSSL())
			m_sslStream->async_handshake(boost::asio::ssl::stream_base::client, boost::bind(&AsyncHttpClient::handshakeCallback, get_this_ptr<AsyncHttpClient>(), boost::asio::placeholders::error, scope->extendTimeout(getTimeout())));
		else
			sendRequest(scope);
	}
}

void AsyncHttpClient::resolveCallback(const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
	{
		handleError(errorContextResolve, e, scope);
	}
	else
	{
		openConnection(endpoint, scope);
	}
}

void AsyncHttpClient::handshakeCallback(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
	{
		handleError(errorContextHandshake, e, scope);
	}
	else
	{
		sendRequest(scope);
	}
}

void AsyncHttpClient::handleRequestSended(const boost::system::error_code &e, shared_ptr<HttpData> data, shared_ptr<ConnectionScope> scope)
{
	scope->cancelTimeout();

	if(e)
	{
		handleError(errorContextRequest, e, scope);
	}
	else
	{
        m_response.reset(OS_NEW HttpResponse());

		if(m_url.getSSL())
			m_sslStream->async_read_some(boost::asio::buffer(m_buffer), boost::bind(&AsyncHttpClient::handleServerResponse, get_this_ptr<AsyncHttpClient>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, scope->extendTimeout(getTimeout())));
		else
			getSocket().async_read_some(boost::asio::buffer(m_buffer), boost::bind(&AsyncHttpClient::handleServerResponse, get_this_ptr<AsyncHttpClient>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, scope->extendTimeout(getTimeout())));
	}
}

void AsyncHttpClient::handleServerResponse(const boost::system::error_code &e, size_t bytes_transferred, shared_ptr<ConnectionScope> scope)
{
 	scope->cancelTimeout();

	if(e)
	{
		// VERYURGENT: andrebbe convertita in handleError(errorContextResponse, e, scope)
		onResponseError(e, scope);
	}
	else
	{
		switch(m_response->writeData(m_buffer.data(), static_cast<uint32>(bytes_transferred)))
		{
		case HttpRequest::statusWaitingHeader:
		case HttpRequest::statusWaitingContent:
													if(m_url.getSSL())
														m_sslStream->async_read_some(boost::asio::buffer(m_buffer), boost::bind(&AsyncHttpClient::handleServerResponse, get_this_ptr<AsyncHttpClient>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, scope->extendTimeout(getTimeout())));
													else
														getSocket().async_read_some(boost::asio::buffer(m_buffer), boost::bind(&AsyncHttpClient::handleServerResponse, get_this_ptr<AsyncHttpClient>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, scope->extendTimeout(getTimeout())));

													break;

		case HttpRequest::statusCompleted:			handleResponseCompleted(scope);
													break;

		case HttpRequest::statusError:				// VERYURGENT: andrebbe convertita in handleError(errorContextResponse, e, scope)
													onResponseError(boost::system::error_code(), scope);
													break;

		default:									OS_ASSERTFALSE();
													break;
		}
	}
}

void AsyncHttpClient::handleError(ErrorContext context, const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{
	if(isAsioOperationAborted(e) == false)
		onError(context, e, scope);		
}

boost::asio::ssl::context & AsyncHttpClient::getSSLContext()
{
	return *m_sslContext;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
