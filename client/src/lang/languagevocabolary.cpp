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
#include "languagevocabolary.h"

#include "constants.h"
#include "languagemanager.h"
#include "xmlattribute.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String LanguageVocabolary::KEY = _S("key");
const String LanguageVocabolary::NAME = _S("name");
const String LanguageVocabolary::VALUE = _S("value");

//////////////////////////////////////////////////////////////////////

LanguageVocabolary::LanguageVocabolary()
{
	
}

LanguageVocabolary::~LanguageVocabolary()
{

}

const String & LanguageVocabolary::getText(const String &name)
{
	load();

	Items::const_iterator i = m_items.find(name);
	if(i != m_items.end())
		return i->second;

	return String::EMPTY;
}

void LanguageVocabolary::setText(const String &name, const String &value)
{
	m_items[name] = value;
}

void LanguageVocabolary::add(const String &filename)
{
	OS_ASSERT(filename.empty() == false);
	m_files.push_back(filename);
}

void LanguageVocabolary::clear()
{
	m_items.clear();
}

bool LanguageVocabolary::load()
{
	while(m_files.empty() == false)
	{
		String filename = utils::pop_front(m_files);

		shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
		if(document->parseFile(filename) == false)
			return false;

		if(load(document) == false)
			return false;
	}

	return true;
}

bool LanguageVocabolary::load(shared_ptr<XMLDocument> document)
{
	shared_ptr<XMLNode> root = document->getRoot();
	if(root == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<XMLNodes> childs = root->getNodes();
	if(childs == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	for(XMLNodes::const_iterator i = childs->begin(); i != childs->end(); parse(*i), ++i);

	return true;
}

void LanguageVocabolary::parse(shared_ptr<XMLNode> node)
{
	OS_ASSERT(node != null);

	if(node->getName() == KEY)
	{
		shared_ptr<XMLAttribute> name = node->getAttribute(NAME);
		shared_ptr<XMLAttribute> value = node->getAttribute(VALUE);

		if(name != null && value != null)
			setText(name->getValue(), value->getValue());
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
