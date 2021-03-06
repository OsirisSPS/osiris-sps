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
#include "httpenumerateddirectorycallback.h"

#include "conversions.h"
#include "httpdirectorycallback.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HttpEnumeratedDirectoryCallback::HttpEnumeratedDirectoryCallback(const String &name) : DirectoryBase(name),
																					   m_callbackID(0)
{

}

HttpEnumeratedDirectoryCallback::~HttpEnumeratedDirectoryCallback()
{

}

bool HttpEnumeratedDirectoryCallback::handle(shared_ptr<HttpSession> session, const HttpPath &path)
{
	return false;
}

shared_ptr<HttpDirectoryCallback> HttpEnumeratedDirectoryCallback::addDirectoryCallback(const Callback &callback)
{
	OS_LOCK(m_callbackCS);

	shared_ptr<HttpDirectoryCallback> directory(OS_NEW HttpDirectoryCallback(conversions::to_utf16(m_callbackID++), callback));
	OS_ASSERT(hasDirectory(directory->getName()) == false);
	if(addDirectory(directory) == false)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	return directory;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
