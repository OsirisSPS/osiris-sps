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
#include "xmlnode.h"

#include "buffer.h"
#include "constants.h"
#include "conversions.h"
#include "datetime.h"
#include "utils.h"
#include "xmlattribute.h"
#include "xmlattributes.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

XMLNode::XMLNode(const String &name, shared_ptr<XMLDocument> document, shared_ptr<XMLNode> parent) : m_name(name),
		 																						     m_document(document),
																									 m_parent(parent),
																									 m_attributes(OS_NEW XMLAttributes(*this)),
																									 m_nodes(OS_NEW XMLNodes(*this))
{

}

XMLNode::~XMLNode()
{
	clear();
}

shared_ptr<XMLDocument> XMLNode::getDocument() const
{
	return m_document.lock();
}

shared_ptr<XMLAttribute> XMLNode::getAttribute(const String &name) const
{
	return m_attributes->find(name);
}

shared_ptr<XMLNode> XMLNode::getNode(const String &name) const
{
	return m_nodes->find(name);
}

shared_ptr<XMLNode> XMLNode::getNodeByIndex(size_t index) const
{
	return m_nodes->find(index);
}

bool XMLNode::hasAttributes() const
{
	return m_attributes->empty() == false;
}

bool XMLNode::hasNodes() const
{
	return m_nodes->empty() == false;
}

void XMLNode::setDocument(shared_ptr<XMLDocument> document)
{
	m_document = document;
	for(XMLNodes::const_iterator i = m_nodes->begin(); i != m_nodes->end(); ++i)
		(*i)->setDocument(document);
}

bool XMLNode::getAttributeBool(const String &name) const
{
	return conversions::from_utf16<bool>(m_attributes->get(name));
}

void XMLNode::setAttributeBool(const String &name, bool value)
{
	m_attributes->set(name, conversions::to_utf16(value));
}

int32 XMLNode::getAttributeInt32(const String &name) const
{
	return conversions::from_utf16<int32>(m_attributes->get(name));
}

void XMLNode::setAttributeInt32(const String &name, int32 value)
{
	m_attributes->set(name, conversions::to_utf16(value));
}

uint32 XMLNode::getAttributeUint32(const String &name) const
{
	return conversions::from_utf16<uint32>(m_attributes->get(name));
}

void XMLNode::setAttributeUint32(const String &name, uint32 value)
{
	m_attributes->set(name, conversions::to_utf16(value));
}

float XMLNode::getAttributeFloat(const String &name) const
{
	return conversions::from_utf16<float>(m_attributes->get(name));
}

void XMLNode::setAttributeFloat(const String &name, float value)
{
	m_attributes->set(name, conversions::to_utf16(value));
}

double XMLNode::getAttributeDouble(const String &name) const
{
	return conversions::from_utf16<double>(m_attributes->get(name));
}

void XMLNode::setAttributeDouble(const String &name, double value)
{
	m_attributes->set(name, conversions::to_utf16(value));
}

DateTime XMLNode::getAttributeDateTime(const String &name) const
{
	DateTime dt;
	if(dt.fromXML(m_attributes->get(name)))
		return dt;

	return DateTime::EMPTY;
}

void XMLNode::setAttributeDateTime(const String &name, const DateTime &value)
{
	m_attributes->set(name, value.toXML());
}

Buffer XMLNode::getAttributeBuffer(const String &name) const
{
	Buffer buffer;	
	if(buffer.fromBase64(m_attributes->get(name).to_ascii()))
		return buffer;

	return Buffer::EMPTY;
}

void XMLNode::setAttributeBuffer(const String &name, const Buffer &value)
{
	m_attributes->set(name, value.toBase64());
}

const String & XMLNode::getAttributeString(const String &name) const
{
	return m_attributes->get(name);
}

void XMLNode::setAttributeString(const String &name, const String &value)
{
	m_attributes->set(name, value);
}

void XMLNode::addData(const String &data)
{
	m_data += data;
}

shared_ptr<XMLNode> XMLNode::addChild(shared_ptr<XMLNode> child)
{
	return m_nodes->add(child);
}

shared_ptr<XMLNode> XMLNode::addChild(const String &name)
{
	return m_nodes->add(name);
}

void XMLNode::clear()
{
	m_attributes->clear();
	m_nodes->clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
