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
#include "httpproxy.h"

#include "buffer.h"
#include "conversions.h"
#include "httpcontent.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpurl.h"
#include "log.h"
#include "tcpsocket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HttpProxy::HttpProxy(shared_ptr<boost::asio::io_service> service, shared_ptr<TCPSocket> socket) : m_service(service),
																								  m_socket(socket),
																								  m_rawResponse(OS_NEW Buffer())
{
	OS_ASSERT(m_service != null);
	OS_ASSERT(m_socket != null);

	// N.B.: non usare m_socket->get_io_service() (crasha, da verificare il motivo)
	m_sslContext = createAsioSSLObject<boost::asio::ssl::context, boost::asio::ssl::context_base::method>(*service, boost::asio::ssl::context::sslv23);
}

HttpProxy::~HttpProxy()
{
	
}

const Buffer & HttpProxy::getResponse() const
{
	return *m_rawResponse;
}

bool HttpProxy::perform(shared_ptr<HttpRequest> request)
{
	if(request == null || socket == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	std::string rawHeaders = request->getRawHeaders(true);

	Buffer rawRequest;
	rawRequest.write(rawHeaders.data(), static_cast<uint32>(rawHeaders.size()));
	shared_ptr<HttpContent> content = request->getContent();
	if(content != null && content->empty() == false)
		rawRequest.write(content->getContent().getData(), content->getContent().getSize());

	return perform(request->getRawUrl(), rawRequest);
}

bool HttpProxy::perform(const HttpUrl &url, const Buffer &rawRequest)
{
	try
	{
		m_rawResponse->clear();		

		shared_ptr<boost::asio::ip::tcp::resolver> resolver = createAsioObject<boost::asio::ip::tcp::resolver>(*m_service);

		boost::asio::ip::tcp::resolver::query query(url.getHost(), conversions::to_string(url.getPort()));
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver->resolve(query);
		boost::asio::ip::tcp::resolver::iterator end;
		
		shared_ptr<boost::asio::ssl::stream<TCPSocket &> > sslSocket(OS_NEW_T(boost::asio::ssl::stream<TCPSocket &>)(*m_socket, *m_sslContext), os_delete_t());

		boost::system::error_code error = boost::asio::error::host_not_found;
		while(error && endpoint_iterator != end)
		{
			// sslSocket->lowest_layer().close();
			m_socket->close();
			m_socket->connect(*endpoint_iterator++, error);
		}
		
		if(error)
			throw boost::system::system_error(error);	

		if(url.getSSL())
			sslSocket->handshake(boost::asio::ssl::stream_base::client);

		// Invio della richiesta

		{
			uint32 size = 0;
			if(url.getSSL())
				size = static_cast<uint32>(boost::asio::write(*sslSocket, boost::asio::buffer(rawRequest.getData(), rawRequest.getSize()), boost::asio::transfer_all()));
			else
				size = static_cast<uint32>(boost::asio::write(*m_socket, boost::asio::buffer(rawRequest.getData(), rawRequest.getSize()), boost::asio::transfer_all()));
			if(size != rawRequest.getSize())
				return false;
		}

		// Lettura della risposta

		shared_ptr<HttpResponse> response(OS_NEW HttpResponse());

		{
			boost::array<char, OS_HTTP_DEFAULT_BUFFER_SIZE> buffer;
			
			bool loop = true;
			while(loop)
			{
				uint32 size = 0;
				if(url.getSSL())
					size = static_cast<uint32>(sslSocket->read_some(boost::asio::buffer(buffer)));
				else
					size = static_cast<uint32>(m_socket->read_some(boost::asio::buffer(buffer)));
				if(size == 0)
					return false;

				m_rawResponse->write(buffer.data(), static_cast<uint32>(size));
				
				switch(response->writeData(buffer.data(), static_cast<uint32>(size)))
				{
				case HttpRequest::statusWaitingHeader:
				case HttpRequest::statusWaitingContent:
															break;

				case HttpRequest::statusCompleted:			loop = false;

															break;

				case HttpRequest::statusError:				return false;													

				default:									OS_ASSERTFALSE();
															return false;													
				}
			}
		}
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
