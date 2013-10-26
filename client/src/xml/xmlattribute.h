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

#ifndef _OS_XML_XMLATTRIBUTE_H
#define _OS_XML_XMLATTRIBUTE_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLNode;

//////////////////////////////////////////////////////////////////////

class XMLExport XMLAttribute : public Object
{
// Construction
public:
	XMLAttribute(XMLNode &node, const String &name, const String &value);
	virtual ~XMLAttribute();

// Attributes
public:
	inline XMLNode & getNode();
	inline const XMLNode & getNode() const;

	inline const String & getName() const;
	inline void setName(const String &name);

	inline const String & getValue() const;
	inline void setValue(const String &value);

protected:
	XMLNode &m_node;		// Nodo di riferimento
	String m_name;			// Nome dell'attributo
	String m_value;			// Valore dell'attributo
};

//////////////////////////////////////////////////////////////////////

inline XMLNode & XMLAttribute::getNode() { return m_node; }
inline const XMLNode & XMLAttribute::getNode() const { return m_node; }

inline const String & XMLAttribute::getName() const { return m_name; }
inline void XMLAttribute::setName(const String &name) { m_name = name; }

inline const String & XMLAttribute::getValue() const { return m_value; }
inline void XMLAttribute::setValue(const String &value) { m_value = value; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_XMLATTRIBUTE_H
