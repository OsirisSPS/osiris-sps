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
#include "ixmlhandler.h"

#include "log.h"
#include "xmldetails.h"
#include "xmlmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IXMLHandler::IXMLHandler(shared_ptr<XMLSchema> schema) : m_impl(this, schema)
{

}

IXMLHandler::~IXMLHandler()
{

}

shared_ptr<XMLSchema> IXMLHandler::getSchema() const
{
	return m_impl->getSchema();
}

void IXMLHandler::setSchema(shared_ptr<XMLSchema> schema)
{
	m_impl->setSchema(schema);
}

bool IXMLHandler::getStopParser() const
{
	return m_impl->getStopParser();
}

void IXMLHandler::setStopParser(bool stopParser)
{
	m_impl->setStopParser(stopParser);
}

bool IXMLHandler::parseBuffer(const Buffer &buffer, const String &encoding)
{
	clear();
	return XMLManager::instance()->parseBuffer(buffer, this, getSchema(), encoding);
}

bool IXMLHandler::parseFile(const String &filename)
{
	clear();
	return XMLManager::instance()->parseFile(filename, this, getSchema());
}

bool IXMLHandler::parseString(const String &str)
{
	clear();
	return XMLManager::instance()->parseString(str, this, getSchema());
}

bool IXMLHandler::parseStringUTF8(const std::string &str)
{
	clear();
	return XMLManager::instance()->parseStringUTF8(str, this, getSchema());
}

bool IXMLHandler::parseUrl(const HttpUrl &url, const String &userAgent, shared_ptr<boost::asio::io_service> service, shared_ptr<TCPSocket> socket)
{
	clear();
	return XMLManager::instance()->parseUrl(url, this, userAgent, service, socket, getSchema());
}

bool IXMLHandler::parseStream(shared_ptr<IStream> stream, const String &encoding)
{
	clear();
	return XMLManager::instance()->parseStream(stream, this, getSchema(), encoding);
}

void IXMLHandler::clear()
{

}

void IXMLHandler::onError(const String &msg)
{
	OS_EXCEPTION(msg.to_utf8());
}

void IXMLHandler::onWarning(const String &msg)
{
	OS_LOG_WARNING(msg.to_utf8());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
