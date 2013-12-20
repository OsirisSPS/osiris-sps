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
#include "xmloptions.h"

#include "dbvalue.h"
#include "option.h"
#include "lock.h"
#include "locked.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

XMLOptions::XMLOptions()
{

}

XMLOptions::~XMLOptions()
{

}

Locked<const XMLOptions::Options, boost::shared_mutex>::shared XMLOptions::getOptions() const
{
	return Locked<const Options, boost::shared_mutex>::shared(m_options, m_cs);
}

DataItem XMLOptions::getOption(const String &name) const
{
	DataItem value;
	if(getOption(name, value))
		return value;

	return DataItem::EMPTY;
}

bool XMLOptions::getOption(const String &name, DataItem &value) const
{
	boost::shared_lock<boost::shared_mutex> lock(m_cs);

	Options::const_iterator i = m_options.find(name);
	if(i == m_options.end())
		return false;

	value = i->second->getValue();
	return true;
}

bool XMLOptions::setOption(const String &name, const DataItem &value)
{
	boost::unique_lock<boost::shared_mutex> lock(m_cs);

	Options::iterator i = m_options.find(name);
	if(i != m_options.end())
		return i->second->setValue(value);		
	
	return doEnsureOption(name, value, DataItem::EMPTY, false, nullptr);
}

bool XMLOptions::exists(const String &name) const
{
	boost::shared_lock<boost::shared_mutex> lock(m_cs);
    return m_options.find(name) != m_options.end();
}

bool XMLOptions::ensureOption(const String &name, const DataItem &value, bool publicOption, shared_ptr<IOptionValidator> validator)
{
	boost::unique_lock<boost::shared_mutex> lock(m_cs);
	return doEnsureOption(name, value, value, publicOption, validator);
}

bool XMLOptions::ensureOption(const String &name, const DataItem &value, const DataItem &defaultValue, bool publicOption, shared_ptr<IOptionValidator> validator)
{
	boost::unique_lock<boost::shared_mutex> lock(m_cs);
	return doEnsureOption(name, value, defaultValue, publicOption, validator);
}

void XMLOptions::parseString(const String &str)
{
	if(str.empty())
		return;

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	if(document->parseString(str))
		parseXML(document);
}

void XMLOptions::parseXML(shared_ptr<XMLDocument> document)
{
	OS_ASSERT(document != nullptr);
	if(document != nullptr)
		parseXMLNode(document->getRoot());
}

void XMLOptions::parseXMLNode(shared_ptr<XMLNode> node)
{
	OS_ASSERT(node != nullptr);
	if(node == nullptr)
		return;

	shared_ptr<XMLNodes> optionsNodes = node->getNodes();
	for(XMLNodes::const_iterator i = optionsNodes->begin(); i != optionsNodes->end(); ++i)
	{
		shared_ptr<XMLNode> optionNode = *i;
		if(optionNode->getName() != OS_XMLOPTIONS_NODE_OPTION)
			continue;

		setOption(optionNode->getAttributeString(OS_XMLOPTIONS_ATTRIBUTE_NAME), optionNode->getAttributeString(OS_XMLOPTIONS_ATTRIBUTE_VALUE));
	}
}

String XMLOptions::toString(bool allowNull) const
{
	shared_ptr<XMLDocument> xml = toXML(allowNull);
	if(xml == nullptr)
	{
		OS_ASSERT(allowNull);
		return String::EMPTY;
	}

	String str;
	if(xml->writeString(str) == false)
		return String::EMPTY;

	return str;
}

shared_ptr<XMLDocument> XMLOptions::toXML(bool allowNull) const
{
	shared_ptr<XMLNode> node = toXMLNode(OS_XMLOPTIONS_NODE_ROOT, allowNull);
	if(node == nullptr)
	{
		OS_ASSERT(allowNull);
		return nullptr;
	}

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	document->setRoot(node);
	return document;
}

shared_ptr<XMLNode> XMLOptions::toXMLNode(const String &nodeName, bool allowNull) const
{
	boost::shared_lock<boost::shared_mutex> lock(m_cs);

	shared_ptr<XMLNode> nodeRoot(OS_NEW XMLNode(nodeName));
	for(Options::const_iterator i = m_options.begin(); i != m_options.end(); ++i)
	{
		if(i->second->isDefault())
			continue;	// Salta le opzioni al valore di default

		shared_ptr<XMLNode> optionNode(OS_NEW XMLNode(OS_XMLOPTIONS_NODE_OPTION));
		optionNode->setAttributeString(OS_XMLOPTIONS_ATTRIBUTE_NAME, i->second->getName());
		optionNode->setAttributeString(OS_XMLOPTIONS_ATTRIBUTE_VALUE, i->second->toString());

		nodeRoot->addChild(optionNode);
	}
	
	if(allowNull && nodeRoot->getNodes()->empty())
		return nullptr;	// Restituisce nullptr nel caso tutte le opzioni siano al valore di default

	return nodeRoot;
}

XMLOptions & XMLOptions::toDefault()
{
	boost::unique_lock<boost::shared_mutex> lock(m_cs);

	for(Options::iterator i = m_options.begin(); i != m_options.end(); i->second->toDefault(), ++i);

	return *this;
}

void XMLOptions::read(const DataTableRow &row, const String &field)
{
	DbValue<String> options;
	options.read(row, field);
	parseString(options);
}

void XMLOptions::write(DbSqlValues &values, const String &field) const
{
	DbValue<String> options;

	String str = toString();
	// N.B.: se le opzioni sono vuote lascia il campo a nullptr
	if(str.empty() == false)
		options = str;

	options.write(values, field);
}

bool XMLOptions::removeOption(const String &name)
{
	boost::unique_lock<boost::shared_mutex> lock(m_cs);

	Options::iterator i = m_options.find(name);
	if(i == m_options.end())
		return false;

	m_options.erase(i);
	return true;
}

uint32 XMLOptions::removePattern(const String &name, bool icase)
{
	boost::unique_lock<boost::shared_mutex> lock(m_cs);

	uint32 count = 0;
	for(Options::iterator i = m_options.begin(); i != m_options.end();)
	{
		Options::iterator y = i;
		++i;

		if(y->first.starts_with(name, icase) == false)
			continue;

		m_options.erase(y);
		count++;
	}

	return count;	
}

bool XMLOptions::doEnsureOption(const String &name, const DataItem &value, const DataItem &defaultValue, bool publicOption, shared_ptr<IOptionValidator> validator)
{
	if(name.empty())
	{
		OS_ASSERTFALSE();
		return false;
	}	

	Options::const_iterator i = m_options.find(name);
	if(i != m_options.end())
	{
		// Se il valore di default non è stato settato per l'opzioni, la tipologia deve coincidere
		OS_ASSERT(i->second->getDefaultValue().isNull() || i->second->getDefaultValue().getType() == defaultValue.getType());
		if(i->second->getDefaultValue().isNull())
			i->second->setDefaultValue(defaultValue);

		if(i->second->getValidator() == nullptr)
			i->second->setValidator(validator);

		return true;
	}

	shared_ptr<Option> option(OS_NEW Option(name, defaultValue));
	option->setValidator(validator);
	option->setValue(value);
	option->setPublic(publicOption);

	m_options[name] = option;
	
	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
