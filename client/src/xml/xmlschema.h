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

#ifndef _OS_XML_XMLSCHEMA_H
#define _OS_XML_XMLSCHEMA_H

#include "base/object.h"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;

//////////////////////////////////////////////////////////////////////

class XMLExport XMLSchema : public Object
{
	friend class XMLManager;

// Construction
public:
	XMLSchema();
	virtual ~XMLSchema();

// Operations
public:
	// Carica un xsd da un file
	void parseFile(const String &filename);
	// Carica un xsd da una stringa
	void parseString(const String &str);
	// Carica un xsd da un buffer
	void parseBuffer(const Buffer &buffer);

	static shared_ptr<XMLSchema> fromFile(const String &filename);
	static shared_ptr<XMLSchema> fromString(const String &str);
	static shared_ptr<XMLSchema> fromBuffer(const Buffer &buffer);

protected:
	pimpl_of<XMLSchema>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_XMLSCHEMA_H
