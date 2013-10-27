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
#include "version.h"

#include "algorithms.h"
#include "boost/format.hpp"
#include "conversions.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((conversions))

//////////////////////////////////////////////////////////////////////

template<>
CoreExport String to_utf16<Version>(const Version &v)
{
	return v.toString();
}

template<>
CoreExport Version from_utf16<Version>(const String &v)
{
	Version version;
	version.fromString(v.to_ascii());
	return version;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((conversions))

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const Version Version::EMPTY;

//////////////////////////////////////////////////////////////////////

Version::Version(const uint16 &value)
{
	set(value);
}

Version::Version(uint8 minor, uint8 major)
{
	set(minor, major);
}

Version::Version(const Version &second)
{
	*this = second;
}

Version::~Version()
{

}

std::string Version::toString() const
{
	// N.B.: boost::format non digerisce gli uint8
	return boost::str(boost::format("%u.%u") % static_cast<uint32>(getMajor()) % static_cast<uint32>(getMinor()));
}

bool Version::fromString(const std::string &str)
{
	vector<std::string>::type values;
	if(algorithms::split(str, values, boost::algorithm::is_any_of(".")).size() != 2)
		return false;

	set(conversions::from_string<uint8>(values[1]), conversions::from_string<uint8>(values[0]));			
	return true;	
}

void Version::clear()
{
	m_value = OS_VERSION_NULL;
}

Version & Version::operator =(const Version &second)
{
	m_value = second.m_value;
	return *this;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
