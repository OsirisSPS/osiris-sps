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

#include "algorithms.h"
#include "constants.h"
#include "conversions.h"
#include "http.h"

//////////////////////////////////////////////////////////////////////

OS_MODULE_ENTRY_POINT()

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((conversions))

//////////////////////////////////////////////////////////////////////

template<>
HttpExport std::string to_string<HttpStatusCode>(const HttpStatusCode &code)
{
 	switch(code)
    {
	case httpStatusOK:						return "200 OK";
    case httpStatusCreated:					return "201 Created";
    case httpStatusAccepted:				return "202 Accepted";
    case httpStatusNoContent:				return "204 No Content";
    case httpStatusMultipleChoices:			return "300 Multiple Choices";
    case httpStatusMovedPermanently:		return "301 Moved Permanently";
    case httpStatusMovedTemporarily:		return "302 Moved Temporarily";
    case httpStatusNotModified:				return "304 Not Modified";
    case httpStatusBadRequest:				return "400 Bad HttpRequest";
	case httpStatusUnauthorized:			return "401 Unauthorized";
    case httpStatusForbidden:				return "403 Forbidden";
    case httpStatusNotFound:				return "404 Not Found";
    case httpStatusInternalServerError:		return "500 Internal Server Error";
    case httpStatusNotImplemented:			return "501 Not Implemented";
    case httpStatusBadGateway:				return "502 Bad Gateway";
    case httpStatusServiceUnavailable:		return "503 Service Unavailable";
    }

	OS_ASSERTFALSE();
	return to_string(static_cast<uint32>(code));
}

template<>
HttpExport HttpMethod from_string<HttpMethod>(const std::string &str)
{
	if(str == "GET")
		return httpMethodGet;

	if(str == "POST")
		return httpMethodPost;

	if(str == "HEAD")
		return httpMethodHead;

	if(str == "PUT")
		return httpMethodPut;

	if(str == "LINK")
		return httpMethodLink;

	if(str == "DELETE")
		return httpMethodDelete;

	if(str == "UNLINK")
		return httpMethodUnlink;

	OS_ASSERTFALSE();
	return httpMethodUnknown;
}

template<>
HttpExport std::string to_string<HttpMethod>(const HttpMethod &method)
{
	switch(method)
	{
	case httpMethodPost:		return "POST";
	case httpMethodGet:			return "GET";
	case httpMethodHead:		return "HEAD";
	case httpMethodPut:			return "PUT";
	case httpMethodDelete:		return "DELETE";
	case httpMethodLink:		return "LINK";
	case httpMethodUnlink:		return "UNLINK";
	}

	OS_ASSERTFALSE();
	return constants::empty_string;
}

template<>
HttpExport std::string to_string<HttpEncoding>(const HttpEncoding &encoding)
{
	switch(encoding)
	{
	case httpEncodingNone:				return constants::empty_string;
	case httpEncodingUrlEncoded:		return OS_HTTP_CONTENTTYPE_URLENCODED;
	case httpEncodingMultipartData:		return OS_HTTP_CONTENTTYPE_MULTIPARTDATA;
	case httpEncodingPlain:				return OS_HTTP_CONTENTTYPE_TEXTPLAIN;
	}

	OS_ASSERTFALSE();
	return to_string(static_cast<uint32>(encoding));
}

template<>
HttpExport std::string to_string<HttpContentEncoding>(const HttpContentEncoding &encoding)
{
	switch(encoding)
	{
	case httpContentEncodingUnknown:
	case httpContentEncodingNone:
											break;

	case httpContentEncodingGzip:			return OS_HTTP_CONTENTENCODING_GZIP;

	case httpContentEncodingDeflate:		return OS_HTTP_CONTENTENCODING_DEFLATE;

	default:								OS_ASSERTFALSE();
											break;
	}

	return constants::empty_string;
}

template<>
HttpExport HttpContentEncoding from_string<HttpContentEncoding>(const std::string &str)
{
	if(str.empty())
		return httpContentEncodingNone;

	if(algorithms::compare_no_case(str, OS_HTTP_CONTENTENCODING_GZIP) || algorithms::compare_no_case(str, OS_HTTP_CONTENTENCODING_XGZIP))
		return httpContentEncodingGzip;

	if(algorithms::compare_no_case(str, OS_HTTP_CONTENTENCODING_DEFLATE))
		return httpContentEncodingDeflate;

	return httpContentEncodingUnknown;
}

template<>
HttpExport std::string to_string<HttpTransferEncoding>(const HttpTransferEncoding &encoding)
{
	switch(encoding)
	{
	case httpTransferEncodingUnknown:
	case httpTransferEncodingNone:
											break;

	case httpTransferEncodingChunked:		return OS_HTTP_TRANSFERENCODING_CHUNKED;

	default:								OS_ASSERTFALSE();
											break;
	}

	return constants::empty_string;
}

template<>
HttpExport HttpTransferEncoding from_string<HttpTransferEncoding>(const std::string &str)
{
	if(str.empty())
		return httpTransferEncodingNone;

	if(algorithms::compare_no_case(str, OS_HTTP_TRANSFERENCODING_CHUNKED))
		return httpTransferEncodingChunked;
	
	return httpTransferEncodingUnknown;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((conversions))

//////////////////////////////////////////////////////////////////////
