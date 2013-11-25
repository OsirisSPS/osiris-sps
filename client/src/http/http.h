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

#ifndef _OS_HTTP_HTTP_H
#define _OS_HTTP_HTTP_H

#include "boost/array.hpp"

//////////////////////////////////////////////////////////////////////

#define OS_HTTP_HEADER_LINE_TERMINATOR			"\r\n"
#define OS_HTTP_HEADER_TERMINATOR				"\r\n\r\n"
#define OS_HTTP_CONTENT_TERMINATOR				"\r\n"

#define OS_HTTP_301								"<html><head><title>Moved</title></head><body><h2>HTTP Code 301</h2><p><strong>File Moved</strong></p><p>The file you are looking for has moved.</p></body></html>"
#define OS_HTTP_302								"<html><head><title>Moved</title></head><body><h2>HTTP Code 302</h2><p><strong>File Moved</strong></p><p>The file you are looking for has moved.</p></body></html>"
#define OS_HTTP_400								"<html><head><title>Error 400 - Bad HttpRequest</title></head><body><h2>HTTP Error 400</h2><p><strong>400 Bad HttpRequest</strong></p><p>Due to malformed syntax, the request could not be understood by the server.The client should not repeat the request without modifications.</p></body></html>"
#define OS_HTTP_401								"<html><head><title>Error 401 - Unauthorized</title></head><body><h2>HTTP Error 401</h2><p><strong>401 Unauthorized</strong></p><p>You are not authorized to perform this request.</p><p>Please ensure you have supplied the correct user credentials.</p></body></html>"
#define OS_HTTP_403								"<html><head><title>Error 403 - Forbidden</title></head><body><h2>HTTP Error 403</h2><p><strong>403 Forbidden</strong></p><p>You don't have permission to access this resource.</p></body></html>"
#define OS_HTTP_404								"<html><head><title>Error 404 - File Not Found</title></head><body><h2>HTTP Error 404</h2><p><strong>404 Not Found</strong></p><p>The Web server cannot find the file or script you asked for. Please check the URL to ensure that the path is correct.</p><p>Please contact the server's administrator if this problem persists.</p></body></html>"
#define OS_HTTP_500								"<html><head><title>Error 500 - Internal Server Error</title></head><body><h2>HTTP Error 500</h2><p><strong>500 Internal Server Error</strong></p><p>The server encountered an unexpected condition which prevented it from fulfilling the request.</p></body></html>"
#define OS_HTTP_501								"<html><head><title>Error 501 - Method Not Supported</title></head><body><h2>HTTP Error 501</h2><p><strong>501 Method Not Supported</strong></p><p>The specified method is not supported.</p></body></html>"
#define OS_HTTP_DELETEOK						"<html><head><title>Delete Successful</title></head><body><p>The specified file has been deleted successfully.</p></body></html>"

#define OS_HTTP_PARAM_ACCEPT					"Accept"
#define OS_HTTP_PARAM_ACCEPT_ENCODING			"Accept-Encoding"
#define OS_HTTP_PARAM_ACCEPT_LANGUAGE			"Accept-Language"
#define OS_HTTP_PARAM_ACCEPT_CHARSET			"Accept-Charset"
#define OS_HTTP_PARAM_ALLOW						"Allow"
#define OS_HTTP_PARAM_AUTHORIZATION				"Authorization"
#define OS_HTTP_PARAM_CONNECTION				"Connection"
#define OS_HTTP_PARAM_CONTENTTYPE				"Content-Type"
#define OS_HTTP_PARAM_CONTENTBOUNDARY			"boundary"
#define OS_HTTP_PARAM_CONTENTDISPOSITION		"Content-Disposition"
#define OS_HTTP_PARAM_CONTENTLENGTH				"Content-Length"
#define OS_HTTP_PARAM_CONTENTENCODING			"Content-Encoding"
#define OS_HTTP_PARAM_COOKIE					"Cookie"
#define OS_HTTP_PARAM_DATE						"Date"
#define OS_HTTP_PARAM_EXPIRES					"Expires"
#define OS_HTTP_PARAM_HOST						"Host"
#define OS_HTTP_PARAM_IFMODIFIEDSINCE			"If-Modified-Since"
#define OS_HTTP_PARAM_LASTMODIFIED				"Last-Modified"
#define OS_HTTP_PARAM_LOCATION					"Location"
#define OS_HTTP_PARAM_WWWAUTHENTICATE			"WWW-Authenticate"
#define OS_HTTP_PARAM_SERVER					"Server"
#define OS_HTTP_PARAM_SETCOOKIE					"Set-Cookie"
#define OS_HTTP_PARAM_USERAGENT					"User-Agent"
#define OS_HTTP_PARAM_KEEPALIVE					"Keep-Alive"
#define OS_HTTP_PARAM_TRANSFERENCODING			"Transfer-Encoding"

#define OS_HTTP_CONTENTTYPE_URLENCODED			"application/x-www-form-urlencoded"
#define OS_HTTP_CONTENTTYPE_MULTIPARTDATA		"multipart/form-data"
#define OS_HTTP_CONTENTTYPE_TEXTPLAIN			"text/plain"
#define OS_HTTP_CONTENTTYPE_OCTETSTREAM			"application/octet-stream"
#define OS_HTTP_CONTENTTYPE_GZIP				"application/x-gzip"

#define OS_HTTP_CONTENTENCODING_GZIP			"gzip"
#define OS_HTTP_CONTENTENCODING_XGZIP			"x-gzip"
#define OS_HTTP_CONTENTENCODING_DEFLATE			"deflate"

#define OS_HTTP_TRANSFERENCODING_CHUNKED		"chunked"

#define OS_HTTP_DEFAULT_SESSION_DURATION		30		// Espresso in minuti

#define OS_HTTP_CONNECTION_TIMEOUT_MIN			100		// In millisecondi
#define OS_HTTP_CONNECTION_TIMEOUT_MAX			10000	// In millisecondi
#define OS_HTTP_CONNECTION_TIMEOUT_DEFAULT		OS_HTTP_CONNECTION_TIMEOUT_MAX

#define OS_HTTP_VERSION_1_0()					OS_MAKEVERSION(0, 1)
#define OS_HTTP_VERSION_1_1()					OS_MAKEVERSION(1, 1)

#define OS_HTTP_DEFAULT_BUFFER_SIZE				8192

#define OS_HTTP_PATH_SEPARATOR					"/"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// Enumerated types

enum HttpStatusCode
{
	httpStatusOK					= 200,
	httpStatusCreated				= 201,
	httpStatusAccepted				= 202,
	httpStatusNoContent				= 204,
	httpStatusMultipleChoices		= 300,
	httpStatusMovedPermanently		= 301,
	httpStatusMovedTemporarily		= 302,
	httpStatusNotModified			= 304,
	httpStatusBadRequest			= 400,
	httpStatusUnauthorized			= 401,
	httpStatusForbidden				= 403,
	httpStatusNotFound				= 404,
	httpStatusInternalServerError	= 500,
	httpStatusNotImplemented		= 501,
	httpStatusBadGateway			= 502,
	httpStatusServiceUnavailable	= 503,
};

enum HttpEncoding
{
	httpEncodingNone				= 0,
	httpEncodingUrlEncoded			= 1,		// Form data is encoded as name and value pairs (default)
	httpEncodingMultipartData		= 2,		// Form data is encoded as a message with a separate part for each control on the page
	httpEncodingPlain				= 3,		// Form data is encoded in plain text, without any control or formatting characters
};

enum HttpContentType
{
	httpContentUnknown				= 0,
	httpContentTextHtml				= 1,
};

enum HttpContentEncoding
{
	httpContentEncodingUnknown		= 0,
	httpContentEncodingNone			= 1,
	httpContentEncodingGzip			= 2,
	httpContentEncodingDeflate		= 3,
};

enum HttpTransferEncoding
{
	httpTransferEncodingUnknown		= 0,
	httpTransferEncodingNone		= 1,
	httpTransferEncodingChunked		= 2,
};

enum HttpMethod
{
	httpMethodPost					= 0,
	httpMethodGet					= 1,
	httpMethodHead					= 2,
	httpMethodPut					= 3,
	httpMethodDelete				= 4,
	httpMethodLink					= 5,
	httpMethodUnlink				= 6,
	httpMethodUnknown				= 7,
};

enum HttpAuthorization
{
	httpAuthorizationAnonymous		= 0,
	httpAuthorizationPlainText		= 1,
};

enum HttpPageMode
{
	httpPageModeFull	= 0,
	httpPageModeContent = 1,
	httpPageModeAjax	= 2,
};

// Types

typedef boost::array<char, OS_HTTP_DEFAULT_BUFFER_SIZE> HttpBuffer;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTTP_HTTP_H
