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

#ifndef _OS_XML_XMLNODES_H
#define _OS_XML_XMLNODES_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLNode;

//////////////////////////////////////////////////////////////////////

class XMLExport XMLNodes : public Object
{
public:
	typedef deque<shared_ptr<XMLNode> >::type nodes;
	typedef nodes::iterator iterator;
	typedef nodes::const_iterator const_iterator;
	typedef nodes::reverse_iterator reverse_iterator;
	typedef nodes::const_reverse_iterator const_reverse_iterator;

// Construction
public:
	XMLNodes(XMLNode &node);
	virtual ~XMLNodes();

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

// Operations
public:
	shared_ptr<XMLNode> add(shared_ptr<XMLNode> node);
	shared_ptr<XMLNode> add(const String &name);

	shared_ptr<XMLNode> find(const String &name) const;
	shared_ptr<XMLNode> find(size_t index) const;

	void clear();

	void push_back(shared_ptr<XMLNode> node);
	void push_front(shared_ptr<XMLNode> node);

	void remove(const String &name);

private:
	iterator _findByName(const String &name);
	const_iterator _findByName(const String &name) const;

	shared_ptr<XMLNode> _findByIndex(size_t index) const;

	shared_ptr<XMLNode> _insert(shared_ptr<XMLNode> node, bool back);

protected:
	XMLNode &m_node;
	nodes m_nodes;
};

//////////////////////////////////////////////////////////////////////

inline XMLNode & XMLNodes::getNode() { return m_node; }
inline const XMLNode & XMLNodes::getNode() const { return m_node; }

inline bool XMLNodes::empty() const { return m_nodes.empty(); }
inline uint32 XMLNodes::size() const { return static_cast<uint32>(m_nodes.size()); }

inline XMLNodes::iterator XMLNodes::begin() { return m_nodes.begin(); }
inline XMLNodes::const_iterator XMLNodes::begin() const { return m_nodes.begin(); }
inline XMLNodes::iterator XMLNodes::end() { return m_nodes.end(); }
inline XMLNodes::const_iterator XMLNodes::end() const { return m_nodes.end(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_XML_XMLNODES_H
