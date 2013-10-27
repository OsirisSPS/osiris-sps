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
#include "httpphysicaldirectory.h"

#include "filesystem.h"
#include "httppath.h"
#include "httpsession.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HttpPhysicalDirectory::HttpPhysicalDirectory(const String &name, const String &physicalPath) : DirectoryBase(name)
{
	setPhysicalPath(physicalPath);
}

HttpPhysicalDirectory::~HttpPhysicalDirectory()
{

}

const String & HttpPhysicalDirectory::getPhysicalPath() const
{
	return m_physicalPath;
}

void HttpPhysicalDirectory::setPhysicalPath(const String &physicalPath)
{
	m_physicalPath = utils::standardisePath(physicalPath);
}

bool HttpPhysicalDirectory::handle(shared_ptr<HttpSession> session, const HttpPath &path)
{
	if(m_physicalPath.empty())
		return false;

	String targetPath = m_physicalPath + path.getUrl();
	if(FileSystem::instance()->isDirectory(targetPath))
	{
		if(m_defaultFile.empty())
			return false;

		session->redirect(utils::makeUrlFilePath(getPath().getUrl(), m_defaultFile).to_ascii());
		return true;
	}
	
	if(FileSystem::instance()->fileExists(targetPath))
	{
		session->transmitFile(targetPath);
		return true;
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
