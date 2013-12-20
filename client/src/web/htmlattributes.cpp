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
#include "htmlattributes.h"

#include "htmlattribute.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlAttributes::HtmlAttributes(IHtmlControl &control) : m_control(control)
{

}

HtmlAttributes::~HtmlAttributes()
{

}

bool HtmlAttributes::exists(const String &name) const
{
	return _findByName(name) != m_attributes.end();
}

void HtmlAttributes::clear()
{
	m_attributes.clear();
}

shared_ptr<HtmlAttribute> HtmlAttributes::get(const String &name) const
{
	iterator i = _findByName(name);
	if(i != m_attributes.end())
		return *i;

	return nullptr;
}

shared_ptr<HtmlAttribute> HtmlAttributes::get(size_t index) const
{
	return _findByIndex(index);
}

shared_ptr<HtmlAttribute> HtmlAttributes::set(const String &name, const String &value)
{
	shared_ptr<HtmlAttribute> attribute = get(name);
	if(attribute != nullptr)
	{
		attribute->setValue(value);
	}
	else
	{
		attribute.reset(OS_NEW HtmlAttribute(getControl(), name, value));
		m_attributes.push_back(attribute);
	}

	return attribute;
}

void HtmlAttributes::remove(const String &name)
{
	iterator i = _findByName(name);
	if(i != m_attributes.end())
		m_attributes.erase(i);
}

const String & HtmlAttributes::value_of(const String &name)
{
	shared_ptr<HtmlAttribute> attribute = get(name);
	if(attribute != nullptr)
		return attribute->getValue();

	return String::EMPTY;
}

HtmlAttributes::iterator HtmlAttributes::_findByName(const String &name) const
{
	for(iterator i = m_attributes.begin(); i != m_attributes.end(); ++i)
	{
		if((*i)->getName() == name)
			return i;
	}

	return m_attributes.end();
}

shared_ptr<HtmlAttribute> HtmlAttributes::_findByIndex(size_t index) const
{
	if(index < size())
		return m_attributes[index];

	return nullptr;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
