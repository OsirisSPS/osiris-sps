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
#include "xmlschema.h"

#include "xmldetails.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

XMLSchema::XMLSchema()
{

}

XMLSchema::~XMLSchema()
{

}

void XMLSchema::parseFile(const String &filename)
{
	m_impl->parseFile(filename);
}

void XMLSchema::parseString(const String &str)
{
	m_impl->parseString(str);
}

void XMLSchema::parseBuffer(const Buffer &buffer)
{
	m_impl->parseBuffer(buffer);
}

shared_ptr<XMLSchema> XMLSchema::fromFile(const String &filename)
{
	shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
	schema->parseFile(filename);
	return schema;
}

shared_ptr<XMLSchema> XMLSchema::fromString(const String &str)
{
	shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
	schema->parseString(str);
	return schema;
}

shared_ptr<XMLSchema> XMLSchema::fromBuffer(const Buffer &buffer)
{
	shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
	schema->parseBuffer(buffer);
	return schema;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
