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

#ifndef _OS_XML_XMLDOCUMENT_H
#define _OS_XML_XMLDOCUMENT_H

#include "enable_this_ptr.h"
#include "ixmlhandler.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLNode;

//////////////////////////////////////////////////////////////////////

class XMLExport XMLDocument : public IXMLHandler,
				  			  public enable_this_ptr<XMLDocument>
{
	typedef IXMLHandler HandlerBase;

// Construction
public:
	XMLDocument(shared_ptr<XMLSchema> schema = nullptr);
	virtual ~XMLDocument();

// Attributes
public:
	shared_ptr<XMLNode> getRoot() const;
	void setRoot(shared_ptr<XMLNode> root);

// Operations
public:
	// Crea un nuovo documento e restituisce il puntatore alla root
	shared_ptr<XMLNode> create(const String &root);
	
	// Salva il documento in un buffer
	bool writeBuffer(Buffer &buffer, const String &encoding = OS_XML_ENCODING_UTF8) const;
	// Salva il documento su file
	bool writeFile(const String &filename, const String &encoding = OS_XML_ENCODING_UTF8) const;
	// Salva il documento in una stringa
	bool writeString(String &str) const;
	// Salva il documento su stringa UTF8
	bool writeStringUTF8(std::string &str) const;	
	// Salva il documento su stream
	bool writeStream(shared_ptr<IStream> stream, const String &encoding = OS_XML_ENCODING_UTF8) const;
	
// IXMLHandler overrides
public:
	virtual void clear();

// IXMLHandler Interface
protected:
	virtual void onStartDocument();
	virtual void onEndDocument();

	virtual void onStartElement(const String &name, const StringCollection<String> &attributes);
	virtual void onEndElement(const String &name);
	virtual void onCharacters(const String &str);

protected:
	shared_ptr<XMLNode> m_root;

private:
	shared_ptr<XMLNode> m_current;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_XMLDOCUMENT_H
