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

#ifndef _OS_HTTP_HTTPPARSER_H
#define _OS_HTTP_HTTPPARSER_H

#include "collections.h"
#include "http.h"
#include "ordered_map.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpPostParam;
class Version;

//////////////////////////////////////////////////////////////////////

class HttpExport HttpParser
{
public:
	typedef Collections<std::wstring, HttpPostParam>::shared_collection HttpPostParams;

// Attributes
public:
	static bool isSafeURL(const std::string &url);
	
	static std::string contentFromType(HttpContentType type);
	static HttpContentType contentFromString(const std::string &str);

// Operations
public:
	static bool parseAuthorization(const std::string &value, std::string &username, std::string &password);
	static bool parseContentType(const std::string &value, std::string &contentType, std::string &boundary, std::string &boundaryTerminator);
	static bool parseDate(const std::string &value, boost::posix_time::ptime &time);
	static bool parseParam(const std::string &line, std::string &param, std::string &value);
	static bool parseParamValue(const std::string &str, std::string &value, StringCollection<std::string> &attributes);
	static void loadUrlParams(const std::string &source, ordered_map<std::wstring, std::wstring> &params);
	static void saveUrlParams(const ordered_map<std::wstring, std::wstring> &source, std::string &params);
	static std::string generateLink(const std::string &url, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	static std::string generateLink(const std::string &url, const std::wstring &paramName, const std::wstring &paramValue);
	static std::string createAnchor(const std::string &url, const std::string &anchor);
	static bool parseHttpVersion(const std::string &str, Version &version);

	static std::string urlEncodeUtf8(const std::string &url, bool escapeOnly = false);
	static std::string urlDecodeUtf8(const std::string &url);	
	static std::string urlEncodeW(const std::wstring &url); // CLODOURGENT: Per me queste dovrebbero sostituire quelle sopra.
	static std::string urlEncodeA(const std::string &url); // CLODOURGENT: Per me queste dovrebbero sostituire quelle sopra.
	static std::wstring urlDecodeW(const std::string &url);	
	static std::string urlDecodeA(const std::string &url);	

	static bool decodePlainPostParams(const std::string &content, HttpPostParams &params);
	static bool decodeUrlEncodedPostParams(const std::string &content, HttpPostParams &params);
	static bool decodeMultiPartDataPostParams(const std::string &content, const std::string &boundary, HttpPostParams &params);

	static std::string toString(const boost::posix_time::ptime &time);

private:
	static bool parseDateMonth(const std::string &str, uint16 &month);
	static bool parseDateWeekDay(const std::string &value, uint16 &weekDay);
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTTP_HTTPPARSER_H
