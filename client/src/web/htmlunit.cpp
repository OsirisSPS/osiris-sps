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
#include "htmlunit.h"

#include "conversions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HtmlUnit::HtmlUnit(int32 value, Type type) : m_value(value),
											 m_type(type)
{
	
}
/*
HtmlUnit::HtmlUnit(const String &value)
{
	*this = fromString(value);
}
*/
HtmlUnit::HtmlUnit(const HtmlUnit &second)
{
	*this = second;
}

HtmlUnit::~HtmlUnit()
{

}

String HtmlUnit::toString() const
{
	String str = conversions::to_utf16(m_value);
	switch(m_type)
	{
	case utPure:		break;

	case utPercent:		str += _S("%");
						break;

	case utPixels:		str += _S("px");
						break;

	default:			OS_ASSERTFALSE();
						break;
	}

	return str;
}	

HtmlUnit HtmlUnit::fromString(String value)
{
	value.trim();
	
	if(value.ends_with(_S("%")))
	{
		value.trim_right(_S("%"));
		return HtmlUnit(conversions::from_utf16<int32>(value), utPercent);
	}

	if(value.ends_with(_S("px")))
	{
		value.trim_right(_S("px"));
		return HtmlUnit(conversions::from_utf16<int32>(value), utPixels);
	}

	return HtmlUnit(conversions::from_utf16<int32>(value), utPure);
}

HtmlUnit HtmlUnit::units(int32 value) 
{ 
	return HtmlUnit(value, HtmlUnit::utPure); 
}

HtmlUnit HtmlUnit::percent(int32 value) 
{ 
	return HtmlUnit(value, HtmlUnit::utPercent); 
}

HtmlUnit HtmlUnit::pixels(int32 value) 
{ 
	return HtmlUnit(value, HtmlUnit::utPixels); 
}

HtmlUnit::operator int32() const 
{ 
	return m_value; 
}

HtmlUnit::operator String() const 
{ 
	return toString(); 
}

HtmlUnit & HtmlUnit::operator =(const HtmlUnit &second)
{
	m_value = second.m_value;
	m_type = second.m_type;

	return *this;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
