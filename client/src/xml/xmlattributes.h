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

#ifndef _OS_XML_XMLATTRIBUTES_H
#define _OS_XML_XMLATTRIBUTES_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class DateTime;
class XMLAttribute;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class XMLExport XMLAttributes : public Object
{
public:
	typedef deque<shared_ptr<XMLAttribute> >::type attributes;
	typedef attributes::iterator iterator;
	typedef attributes::const_iterator const_iterator;
	typedef attributes::reverse_iterator reverse_iterator;
	typedef attributes::const_reverse_iterator const_reverse_iterator;

// Construction
public:
	XMLAttributes(XMLNode &node);
	virtual ~XMLAttributes();

// Attributes
public:
	inline XMLNode & getNode();
	inline const XMLNode & getNode() const;

	inline bool empty() const;
	inline uint32 size() const;

	inline iterator begin();
	inline const_iterator begin() const;
	inline iterator end();
	inline const_iterator end() const;

	bool exists(const String &name) const;

	const String & get(const String &name) const;
	void set(const String &name, const String &value);

// Operations
public:
	shared_ptr<XMLAttribute> find(const String &name) const;
	shared_ptr<XMLAttribute> find(size_t index) const;

	void clear();
	void remove(const String &name);

private:
	iterator _findByName(const String &name) const;
	shared_ptr<XMLAttribute> _findByIndex(size_t index) const;

protected:
	XMLNode &m_node;
	mutable attributes m_attributes;
};

//////////////////////////////////////////////////////////////////////

inline XMLNode & XMLAttributes::getNode() { return m_node; }
inline const XMLNode & XMLAttributes::getNode() const { return m_node; }

inline bool XMLAttributes::empty() const { return m_attributes.empty(); }
inline uint32 XMLAttributes::size() const { return static_cast<uint32>(m_attributes.size()); }

inline XMLAttributes::iterator XMLAttributes::begin() { return m_attributes.begin(); }
inline XMLAttributes::const_iterator XMLAttributes::begin() const { return m_attributes.begin(); }
inline XMLAttributes::iterator XMLAttributes::end() { return m_attributes.end(); }
inline XMLAttributes::const_iterator XMLAttributes::end() const { return m_attributes.end(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_XMLATTRIBUTES_H
