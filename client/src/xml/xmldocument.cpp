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
#include "xmldocument.h"

#include "collections.h"
#include "constants.h"
#include "xmlmanager.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

XMLDocument::XMLDocument(shared_ptr<XMLSchema> schema) : HandlerBase(schema)
{
	
}

XMLDocument::~XMLDocument()
{
	
}

shared_ptr<XMLNode> XMLDocument::getRoot() const 
{ 
	return m_root; 
}

void XMLDocument::setRoot(shared_ptr<XMLNode> root)
{
	m_root = root;
	if(m_root != null)
		m_root->setDocument(get_this_ptr());
}

shared_ptr<XMLNode> XMLDocument::create(const String &root)
{
	clear();

	if(root.empty() == false)
		m_root.reset(OS_NEW XMLNode(root, get_this_ptr()));	

	return m_root;
}

bool XMLDocument::writeBuffer(Buffer &buffer, const String &encoding) const
{
	return XMLManager::instance()->writeBuffer(*this, buffer, encoding);
}

bool XMLDocument::writeFile(const String &filename, const String &encoding) const
{
	return XMLManager::instance()->writeFile(*this, filename, encoding);	
}

bool XMLDocument::writeString(String &str) const
{
	return XMLManager::instance()->writeString(*this, str);
}

bool XMLDocument::writeStringUTF8(std::string &str) const
{
	return XMLManager::instance()->writeStringUTF8(*this, str);
}

bool XMLDocument::writeStream(shared_ptr<IStream> stream, const String &encoding) const
{
	return XMLManager::instance()->writeStream(*this, stream, encoding);
}

void XMLDocument::clear()
{
	HandlerBase::clear();

	m_root.reset();
	m_current.reset();
}

void XMLDocument::onStartDocument()
{
	clear();
}

void XMLDocument::onEndDocument()
{
	m_current.reset();
}

void XMLDocument::onStartElement(const String &name, const StringCollection<String> &attributes)
{
	if(m_root == null)
	{
		m_root.reset(OS_NEW XMLNode(name, get_this_ptr()));
		m_current = m_root;
	}
	else
	{
		m_current = m_current->addChild(name);
	}

	for(StringCollection<String>::iterator i = attributes.begin(); i != attributes.end(); ++i)
	{
		m_current->setAttributeString(i->first, *i->second);
	}	
}

void XMLDocument::onEndElement(const String &name)
{
	OS_ASSERT(m_current != null);
	m_current = m_current->getParent();
}

void XMLDocument::onCharacters(const String &str)
{
	OS_ASSERT(m_current != null);
	m_current->addData(str);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
