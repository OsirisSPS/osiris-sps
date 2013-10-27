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
#include "xmlmanager.h"

#include "buffer.h"
#include "httpclient.h"
#include "httpcontent.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpurl.h"
#include "istream.h"
#include "log.h"
#include "xmldetails.h"
#include "xmldocument.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

XMLManager::XMLManager()
{

}

XMLManager::~XMLManager()
{

}

bool XMLManager::parseBuffer(const Buffer &buffer, IXMLHandler *handler, shared_ptr<XMLSchema> schema, const String &encoding)
{
	xerces::MemBufInputSource inputSource(reinterpret_cast<const XMLByte *>(buffer.getData()), static_cast<XMLSize_t>(buffer.getSize()), static_cast<const XMLCh *>(null), false);
	// N.B.: non settare una stringa vuota
	if(encoding.empty() == false)
		inputSource.setEncoding(encoding.c_str());

	return xerces::parseSource(inputSource, handler, schema != null ? schema->m_impl->getSource() : null);
}

bool XMLManager::parseFile(const String &filename, IXMLHandler *handler, shared_ptr<XMLSchema> schema)
{
	xerces::LocalFileInputSource inputSource(filename.c_str());
	return xerces::parseSource(inputSource, handler, schema != null ? schema->m_impl->getSource() : null);
}

bool XMLManager::parseString(const String &str, IXMLHandler *handler, shared_ptr<XMLSchema> schema)
{
	xerces::MemBufInputSource inputSource(reinterpret_cast<const XMLByte *>(str.buffer()), static_cast<XMLSize_t>(str.buffer_size()), "", false);
	inputSource.setEncoding(String(OS_XML_ENCODING_UTF16).c_str());

	return xerces::parseSource(inputSource, handler, schema != null ? schema->m_impl->getSource() : null);
}

bool XMLManager::parseStringUTF8(const std::string &str, IXMLHandler *handler, shared_ptr<XMLSchema> schema)
{
	xerces::MemBufInputSource inputSource(reinterpret_cast<const XMLByte *>(str.data()), static_cast<XMLSize_t>(str.size()), static_cast<const XMLCh *>(null), false);
	inputSource.setEncoding(String(OS_XML_ENCODING_UTF8).c_str());

	return xerces::parseSource(inputSource, handler, schema != null ? schema->m_impl->getSource() : null);
}

bool XMLManager::parseStream(shared_ptr<IStream> stream, IXMLHandler *handler, shared_ptr<XMLSchema> schema, const String &encoding)
{
	if(stream == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	XMLStreamInputSource inputSource(stream);
	// N.B.: non settare una stringa vuota
	if(encoding.empty() == false)
		inputSource.setEncoding(encoding.c_str());

	return xerces::parseSource(inputSource, handler, schema != null ? schema->m_impl->getSource() : null);
}

bool XMLManager::parseUrl(const HttpUrl &url, IXMLHandler *handler, const String &userAgent, shared_ptr<boost::asio::io_service> service, shared_ptr<TCPSocket> socket, shared_ptr<XMLSchema> schema)
{
	OS_ASSERT(url.empty() == false);

	if(service == null || socket == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<HttpClient> client(OS_NEW HttpClient(service, socket));
	client->getRequest()->setUserAgent(userAgent.to_ascii());	
	if(client->perform(url) == false)
		return false;

	if(client->getResponse()->getStatusCode() != httpStatusOK)
		return false;

	// String::EMPTY come encoding per forzare il caricamento dell'encoding dall'header xml
	return parseBuffer(client->getResponse()->getContent()->getContent(), handler, schema, String::EMPTY);	
}

bool XMLManager::writeBuffer(const XMLDocument &document, Buffer &buffer, const String &encoding) const
{
	buffer.clear();

	scoped_ptr<xerces::MemBufFormatTarget> target(OS_XERCES_NEW xerces::MemBufFormatTarget());
	if(xerces::exportToTarget(document, target.get(), encoding) == false)
		return false;

	uint32 length = static_cast<uint32>(target->getLen());
	return buffer.write(target->getRawBuffer(), length) == length;
}

bool XMLManager::writeFile(const XMLDocument &document, const String &filename, const String &encoding) const
{
	scoped_ptr<xerces::LocalFileFormatTarget> target;
	try
	{
		target.reset(OS_XERCES_NEW xerces::LocalFileFormatTarget(filename.c_str()));
	}
	catch(const xerces::XMLException &e)
	{
		OS_LOG_ERROR(e.getMessage());
		return false;
	}

	return xerces::exportToTarget(document, target.get(), encoding);
}

bool XMLManager::writeString(const XMLDocument &document, String &str) const
{
	return xerces::exportToString(document, str);
}

bool XMLManager::writeStringUTF8(const XMLDocument &document, std::string &str) const
{
	str.clear();

	Buffer buffer;
	if(writeBuffer(document, buffer, OS_XML_ENCODING_UTF8) == false)
		return false;

	if(buffer.empty() == false)
		str.assign(reinterpret_cast<std::string::const_pointer>(buffer.getData()), buffer.getSize());

	return true;
}

bool XMLManager::writeStream(const XMLDocument &document, shared_ptr<IStream> stream, const String &encoding) const
{
	if(stream == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	scoped_ptr<XMLStreamFormatTarget> target(OS_XERCES_NEW XMLStreamFormatTarget(stream));
	return xerces::exportToTarget(document, target.get(), encoding);
}

bool XMLManager::validateBuffer(const Buffer &buffer, shared_ptr<XMLSchema> schema) const
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(schema));
	return document->parseBuffer(buffer);
}

bool XMLManager::validateFile(const String &filename, shared_ptr<XMLSchema> schema) const
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(schema));
	return document->parseFile(filename);
}

bool XMLManager::validateString(const String &str, shared_ptr<XMLSchema> schema) const
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(schema));
	return document->parseString(str);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
