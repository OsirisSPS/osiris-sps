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

#ifndef _OS_XML_XMLMANAGER_H
#define _OS_XML_XMLMANAGER_H

#include "boostasio.h"
#include "singleton.h"
#include "xmlstandards.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class HttpUrl;
class IStream;
class IXMLHandler;
class TCPSocket;
class XMLDocument;
class XMLSchema;

//////////////////////////////////////////////////////////////////////

class XMLExport XMLManager : public StaticSingleton<XMLManager>
{
// Construction
protected:
	XMLManager();

public:
	virtual ~XMLManager();

// Operations
public:
	// Carica la stringa specificata
	bool parseBuffer(const Buffer &buffer, IXMLHandler *handler, shared_ptr<XMLSchema> schema = null, const String &encoding = OS_XML_ENCODING_UTF8);
	// Carica il file specificato
	bool parseFile(const String &filename, IXMLHandler *handler, shared_ptr<XMLSchema> schema = null);
	// Carica la stringa specificata
	bool parseString(const String &str, IXMLHandler *handler, shared_ptr<XMLSchema> schema = null);
	// Carica una stringa utf8
	bool parseStringUTF8(const std::string &str, IXMLHandler *handler, shared_ptr<XMLSchema> schema = null);
	// Carica lo stream specificato
	bool parseStream(shared_ptr<IStream> stream, IXMLHandler *handler, shared_ptr<XMLSchema> schema = null, const String &encoding = OS_XML_ENCODING_UTF8);
	// Carica l'url specificato
	bool parseUrl(const HttpUrl &url, IXMLHandler *handler, const String &userAgent, shared_ptr<boost::asio::io_service> service, shared_ptr<TCPSocket> socket, shared_ptr<XMLSchema> schema = null);

	// Salva il documento su un buffer
	bool writeBuffer(const XMLDocument &document, Buffer &buffer, const String &encoding = OS_XML_ENCODING_UTF8) const;
	// Salva il documento su file
	bool writeFile(const XMLDocument &document, const String &filename, const String &encoding = OS_XML_ENCODING_UTF8) const;
	// Salva il documento su stringa
	bool writeString(const XMLDocument &document, String &str) const;
	// Salva il documento su stringa UTF8
	bool writeStringUTF8(const XMLDocument &document, std::string &str) const;	
	// Salva il documento su stream
	bool writeStream(const XMLDocument &document, shared_ptr<IStream> stream, const String &encoding = OS_XML_ENCODING_UTF8) const;	
	
	// Restituisce true se il buffer specificato è un xml valido
	bool validateBuffer(const Buffer &buffer, shared_ptr<XMLSchema> schema = null) const;
	// Restituisce true se il file specificato è un xml valido
	bool validateFile(const String &filename, shared_ptr<XMLSchema> schema = null) const;
	// Restituisce true se la stringa specificata è un xml valido
	bool validateString(const String &str, shared_ptr<XMLSchema> schema = null) const;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_XMLMANAGER_H
