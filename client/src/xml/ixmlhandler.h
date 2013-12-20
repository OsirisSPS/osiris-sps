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

#ifndef _OS_XML_IXMLHANDLER_H
#define _OS_XML_IXMLHANDLER_H

#include "base/object.h"
#include "boostasio_fwd.h"
#include "boost/noncopyable.hpp"
#include "collections.h"
#include "pimpl_ptr.h"
#include "xmlstandards.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class HttpUrl;
class IStream;
class TCPSocket;
class XMLSchema;

//////////////////////////////////////////////////////////////////////

class XMLExport IXMLHandler : public Object,
							  public boost::noncopyable
{
	class impl;

// Construction
public:
	IXMLHandler(shared_ptr<XMLSchema> schema = nullptr);
	virtual ~IXMLHandler();

// Attributes
public:
	shared_ptr<XMLSchema> getSchema() const;
	void setSchema(shared_ptr<XMLSchema> schema);

	bool getStopParser() const;
	void setStopParser(bool stopParser);

// Operations
public:
	// Carica un xml da un buffer
	bool parseBuffer(const Buffer &buffer, const String &encoding = OS_XML_ENCODING_UTF8);
	// Carica un xml da un file
	bool parseFile(const String &filename);
	// Carica un xml da una stringa
	bool parseString(const String &str);
	// Carica un xml da una stringa utf8
	bool parseStringUTF8(const std::string &str);
	// Carica un xml da un'url
	bool parseUrl(const HttpUrl &url, const String &userAgent, shared_ptr<boost::asio::io_service> service, shared_ptr<TCPSocket> socket);
	// Carica un xml da uno stream
	bool parseStream(shared_ptr<IStream> stream, const String &encoding = OS_XML_ENCODING_UTF8);

// Attributes
public:
	inline impl * getImpl() const;

// Interface
protected:
	virtual void onStartDocument() = 0;
	virtual void onEndDocument() = 0;

	virtual void onStartElement(const String &name, const StringCollection<String> &attributes) = 0;
	virtual void onEndElement(const String &name) = 0;
	virtual void onCharacters(const String &str) = 0;

// Overridables
public:
	virtual void clear();

protected:
	virtual void onError(const String &msg);
	virtual void onWarning(const String &msg);

private:
	pimpl_ptr<impl> m_impl;
};

//////////////////////////////////////////////////////////////////////

inline IXMLHandler::impl * IXMLHandler::getImpl() const { return m_impl.get(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_IXMLHANDLER_H
