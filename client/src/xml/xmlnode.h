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

#ifndef _OS_XML_XMLNODE_H
#define _OS_XML_XMLNODE_H

#include "base/object.h"
#include "collections.h"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class DateTime;
class XMLAttribute;
class XMLAttributes;
class XMLDocument;
class XMLNodes;

//////////////////////////////////////////////////////////////////////

class XMLExport XMLNode : public Object,
						  public enable_this_ptr<XMLNode>
{
// Construction
public:
	XMLNode(const String &name = String::EMPTY, shared_ptr<XMLDocument> document = nullptr, shared_ptr<XMLNode> parent = nullptr);
	virtual ~XMLNode();

// Attributes
public:
	inline const String & getName() const;
	inline void setName(const String &name);

	inline const String & getData() const;
	inline void setData(const String &data);
	inline bool hasData() const;

	inline shared_ptr<XMLNode> getParent() const;
	inline void setParent(shared_ptr<XMLNode> parent);

	inline shared_ptr<XMLAttributes> getAttributes() const;
	inline shared_ptr<XMLNodes> getNodes() const;

	shared_ptr<XMLAttribute> getAttribute(const String &name) const;
	shared_ptr<XMLNode> getNode(const String &name) const;	
	shared_ptr<XMLNode> getNodeByIndex(size_t index) const;	

	bool hasAttributes() const;
	bool hasNodes() const;

	shared_ptr<XMLDocument> getDocument() const;
	void setDocument(shared_ptr<XMLDocument> document);

	bool getAttributeBool(const String &name) const;
	void setAttributeBool(const String &name, bool value);

	int32 getAttributeInt32(const String &name) const;
	void setAttributeInt32(const String &name, int32 value);

	uint32 getAttributeUint32(const String &name) const;
	void setAttributeUint32(const String &name, uint32 value);

	float getAttributeFloat(const String &name) const;
	void setAttributeFloat(const String &name, float value);
	
	double getAttributeDouble(const String &name) const;
	void setAttributeDouble(const String &name, double value);

	DateTime getAttributeDateTime(const String &name) const;
	void setAttributeDateTime(const String &name, const DateTime &value);

	Buffer getAttributeBuffer(const String &name) const;
	void setAttributeBuffer(const String &name, const Buffer &value);

	const String & getAttributeString(const String &name) const;
	void setAttributeString(const String &name, const String &value);

// Operations
public:
	void addData(const String &data);

	shared_ptr<XMLNode> addChild(shared_ptr<XMLNode> child);
	shared_ptr<XMLNode> addChild(const String &name);	

	void clear();

protected:
	weak_ptr<XMLDocument> m_document;			// Documento di riferimento
	weak_ptr<XMLNode> m_parent;					// Nodo padre
	shared_ptr<XMLNodes> m_nodes;				// Nodi figli
	String m_name;								// Nome del nodo
	shared_ptr<XMLAttributes> m_attributes;		// Attributi del nodo
	String m_data;								// Valore del nodo
};

//////////////////////////////////////////////////////////////////////

inline const String & XMLNode::getName() const { return m_name; }
inline void XMLNode::setName(const String &name) { m_name = name; }

inline const String & XMLNode::getData() const { return m_data; }
inline void XMLNode::setData(const String &data) { m_data = data; }
inline bool XMLNode::hasData() const { return m_data.empty() == false; }

inline shared_ptr<XMLNode> XMLNode::getParent() const { return m_parent.lock(); }
inline void XMLNode::setParent(shared_ptr<XMLNode> parent) { m_parent = parent; }

inline shared_ptr<XMLAttributes> XMLNode::getAttributes() const { return m_attributes; }
inline shared_ptr<XMLNodes> XMLNode::getNodes() const { return m_nodes; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_XMLNODE_H
