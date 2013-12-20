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
#include "xmlnodes.h"

#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

XMLNodes::XMLNodes(XMLNode &node) : m_node(node)
{

}

XMLNodes::~XMLNodes()
{

}

shared_ptr<XMLNode> XMLNodes::add(shared_ptr<XMLNode> node)
{
	return _insert(node, true);
}

shared_ptr<XMLNode> XMLNodes::add(const String &name)
{
	shared_ptr<XMLNode> node(OS_NEW XMLNode(name));
	return _insert(node, true);
}

shared_ptr<XMLNode> XMLNodes::find(const String &name) const
{
	const_iterator i = _findByName(name);
	if(i != m_nodes.end())
		return *i;

	return nullptr;
}

shared_ptr<XMLNode> XMLNodes::find(size_t index) const
{
	return _findByIndex(index);
}

void XMLNodes::clear()
{
	m_nodes.clear();
}

void XMLNodes::push_back(shared_ptr<XMLNode> node)
{
	_insert(node, true);
}

void XMLNodes::push_front(shared_ptr<XMLNode> node)
{
	_insert(node, false);
}

void XMLNodes::remove(const String &name)
{
	iterator i = _findByName(name);
	if(i != m_nodes.end())
		m_nodes.erase(i);
}

XMLNodes::iterator XMLNodes::_findByName(const String &name)
{
	for(iterator i = m_nodes.begin(); i != m_nodes.end(); ++i)
	{
		if((*i)->getName() == name)
			return i;
	}

	return m_nodes.end();
}

XMLNodes::const_iterator XMLNodes::_findByName(const String &name) const
{
	for(const_iterator i = m_nodes.begin(); i != m_nodes.end(); ++i)
	{
		if((*i)->getName() == name)
			return i;
	}

	return m_nodes.end();
}

shared_ptr<XMLNode> XMLNodes::_findByIndex(size_t index) const
{
	if(index < size())
		return m_nodes[index];

	return nullptr;
}

shared_ptr<XMLNode> XMLNodes::_insert(shared_ptr<XMLNode> node, bool back)
{
	if(node == nullptr)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	OS_ASSERT(node->getName().empty() == false);

	node->setDocument(getNode().getDocument());

	OS_EXCEPT_IF(node->getParent() != nullptr, "Node already attached");
	node->setParent(getNode().get_this_ptr());

	if(back)
        m_nodes.push_back(node);
	else
		m_nodes.push_front(node);

	return node;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
