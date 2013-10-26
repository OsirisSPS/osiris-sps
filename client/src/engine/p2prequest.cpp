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
#include "p2prequest.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

Request::Request(RequestType type) : m_type(type)
{

}

Request::~Request()
{

}

bool Request::objectExists(const String &id) const
{
	OS_ASSERT(m_type == rtObjects);
	return m_objects.find(id) != m_objects.end();
}

bool Request::hasObjects() const
{
	OS_ASSERT(m_type == rtObjects);
	return m_objects.empty() == false;
}

void Request::addObject(const String &id)
{
	OS_ASSERT(m_type == rtObjects);
	m_objects.insert(id);
}

void Request::removeObject(const String &id)
{
	OS_ASSERT(m_type == rtObjects);
	m_objects.erase(id);
}

void Request::getObjects(StringList &objects) const
{
	OS_ASSERT(m_type == rtObjects);

	for(StringSet::const_iterator i = m_objects.begin(); i != m_objects.end(); objects.push_back(*i), ++i);
}

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
