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

#ifndef _OS_HTTP_IHTTPMETHOD_H
#define _OS_HTTP_IHTTPMETHOD_H

#include "base/object.h"
#include "boost/noncopyable.hpp"
#include "http.h"
#include "version.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class HttpContent;
class HttpData;

//////////////////////////////////////////////////////////////////////

class HttpExport IHttpMethod : public Object,
							   public boost::noncopyable
{
// Enumerated types
public:
	enum MethodStatus
	{
		statusWaitingHeader		= 0,		// In attesa del trasferimento dell'header
		statusWaitingContent	= 1,		// In attesa del trasferimento del content
		statusCompleted			= 2,		// Caricata
		statusError				= 3,		// In errore
	};

	typedef unordered_multimap<std::string, std::string>::type MethodParams;

// Construction
public:
	IHttpMethod();
	virtual ~IHttpMethod();

// Attributes
public:
	inline shared_ptr<HttpContent> getContent() const;
	inline MethodStatus getStatus() const;

	inline const std::string & getRawMethod() const;
	inline const MethodParams & getRawParams() const;

	const std::string & getRawParam(const std::string &param) const;
	
	std::string getRawHeaders(bool terminator) const;
	
// Operations
public:
	shared_ptr<HttpData> serialize() const;
	MethodStatus writeData(const void *data, uint32 size);

private:
	void changeStatus(MethodStatus status);

// Interface
protected:
	// Esporta l'header HTTP
	virtual shared_ptr<HttpData> createHeader(shared_ptr<HttpData> content) const = 0;

	// Effettua il parsing dell'interstazione del metodo HTTP (la prima riga dell'header)
	virtual bool parseMethod(const std::string &header) = 0;

// Overridables
protected:
	// Esporta il contenuto HTTP
	virtual shared_ptr<HttpData> createContent() const;

	// Effettua il parsing di un header HTTP
	virtual bool parseHeaders(const std::string &headers);
	// Effettua il parsing di un parametro HTTP
	virtual bool parseParam(const std::string &paramName, const std::string &paramValue);
	// Effettua il parsing del contenuto del metodo
	virtual bool parseContent();

protected:
	Version m_httpVersion;
	shared_ptr<HttpContent> m_content;
	std::string m_rawMethod;
	MethodParams m_rawParams;

private:
	MethodStatus m_status;
	scoped_ptr<Buffer> m_buffer;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<HttpContent> IHttpMethod::getContent() const { return m_content; }
inline IHttpMethod::MethodStatus IHttpMethod::getStatus() const { return m_status; }

inline const std::string & IHttpMethod::getRawMethod() const { return m_rawMethod; }
inline const IHttpMethod::MethodParams & IHttpMethod::getRawParams() const { return m_rawParams; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTTP_IHTTPMETHOD_H
