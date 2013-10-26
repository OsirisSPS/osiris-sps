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

#ifndef _OS_XML_XMLWRITER_H
#define _OS_XML_XMLWRITER_H

#include "base/object.h"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IStream;
class XMLDocument;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class XMLExport XMLWriter : public Object
{
// Construction
public:
	XMLWriter(bool enableFormat = true);
	virtual ~XMLWriter();

// Attributes
public:
	bool getEnableFormat() const;
	void setEnableFormat(bool enableFormat);

	String getIndent() const;
	void setIndent(const String &indent);

// Operations
public:
	bool openFile(const String &filename);
	bool openStream(shared_ptr<IStream> stream);
	void close();

	bool writeDeclaration();
	bool writeDocument(shared_ptr<XMLDocument> document, bool declaration = true);
	bool writeNode(shared_ptr<XMLNode> node);
	bool writeOpen(shared_ptr<XMLNode> node);
	bool writeContent(shared_ptr<XMLNode> node, bool childs = true);
	bool writeClose(shared_ptr<XMLNode> node);

	void increaseIndent();
	void decreaseIndent();
	void resetIndent();

private:
	pimpl_of<XMLWriter>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_XMLWRITER_H
