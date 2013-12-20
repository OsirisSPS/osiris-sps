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
#include "ihttpdirectory.h"

#include "cryptmanager.h"
#include "buffer.h"
#include "lock.h"
#include "http.h"
#include "httppath.h"
#include "httpsessionstate.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IHttpDirectory::IHttpDirectory(const String &name) : m_name(name),
													 m_active(true),
													 m_restriction(drNone)
{
	
}

IHttpDirectory::~IHttpDirectory()
{

}

shared_ptr<IHttpDirectory> IHttpDirectory::getDirectory(const String &name) const
{
	OS_LOCK(m_cs);
	return m_directories.get(name);
}

HttpPath IHttpDirectory::getPath() const
{
	OS_LOCK(m_cs);

	String path;

	shared_ptr<const IHttpDirectory> directory = get_this_ptr();
	while(directory != nullptr)
	{
		String name = directory->getName();

		OS_ASSERT((name.empty() == false) || (directory->getParent() == nullptr));
		if(name.empty() == false)
			path.insert(0, (OS_HTTP_PATH_SEPARATOR + name).c_str());
		
		directory = directory->getParent();			
	}	

	return path;
}

bool IHttpDirectory::addDirectory(shared_ptr<IHttpDirectory> directory)
{
	OS_LOCK(m_cs);

	if(directory == nullptr)
		return false;

	OS_EXCEPT_IF(hasDirectory(directory->getName()), "Duplicated directory");

	directory->m_parent = get_this_ptr();
	m_directories.push_back(directory->getName(), directory);

	return true;
}

bool IHttpDirectory::removeDirectory(shared_ptr<IHttpDirectory> directory)
{
	if(directory == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	return removeDirectory(directory->getName());
}

bool IHttpDirectory::removeDirectory(const String &name)
{
	OS_LOCK(m_cs);

	shared_ptr<IHttpDirectory> directory = m_directories.get(name);
	if(directory == nullptr)
		return false;
	
	directory->m_parent.reset();
	m_directories.remove(name);

	return true;
}

bool IHttpDirectory::isAccessible(shared_ptr<HttpSession> session)
{
	return true;
}

String IHttpDirectory::getSessionCookie(shared_ptr<HttpSession> session) const
{
	return OS_HTTP_DIRECTORY_SESSION_COOKIE;
}

shared_ptr<HttpSessionState> IHttpDirectory::createSessionState(shared_ptr<HttpSession> session, const TimeDuration &duration) const
{
	shared_ptr<IHttpDirectory> parent = getParent();
	if(parent != nullptr)
		return parent->createSessionState(session, duration);

	return shared_ptr<HttpSessionState>(OS_NEW HttpSessionState(CryptManager::instance()->randomSHA().toHex(), duration));
}

bool IHttpDirectory::process(shared_ptr<HttpSession> session, const HttpPath &path)
{
	if(isActive() == false)
		return false;

	if(isAccessible(session) == false)
		return false;

	if(handle(session, path))
		return true;

	shared_ptr<IHttpDirectory> directory = getDirectory(path.getDirectory());
	if(directory != nullptr && directory->process(session, path.getTarget()))
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
