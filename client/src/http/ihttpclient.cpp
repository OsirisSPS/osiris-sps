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
#include "ihttpclient.h"

#include "httprequest.h"
#include "httpresponse.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IHttpClient::IHttpClient(const std::string &url) : m_request(OS_NEW HttpRequest(httpMethodGet, url)),
												   m_response(OS_NEW HttpResponse()),
												   m_followRedirects(true),
												   m_maxRedirects(OS_IHTTPCLIENT_MAX_REDIRECTS)
{

}

IHttpClient::~IHttpClient()
{

}

bool IHttpClient::loadVerifyFile(const std::string &filename, boost::asio::ssl::context_base::verify_mode verifyMode)
{
	boost::system::error_code error;
	getSSLContext().load_verify_file(filename, error);
	if(error)
		return false;

	getSSLContext().set_verify_mode(verifyMode, error);
	if(error)
		return false;
	
	return true;
}

void IHttpClient::initRedirect(const HttpUrl &redirectUrl, shared_ptr<HttpRequest> request, shared_ptr<HttpResponse> response)
{
	if(request == null || response == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	OS_ASSERT(response->getStatusCode() == httpStatusMovedPermanently || response->getStatusCode() == httpStatusMovedTemporarily);

	// VERYURGENT: deve caricare da m_response eventuali Set-Cookie e aggiornare i cookies in m_request (rimuovendo quelli non più necessari in base a redirectUrl)
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
