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

#ifndef _OS_HTTP_HTTPPHYSICALDIRECTORY_H
#define _OS_HTTP_HTTPPHYSICALDIRECTORY_H

#include "ihttpdirectory.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpExport HttpPhysicalDirectory : public IHttpDirectory
{
	typedef IHttpDirectory DirectoryBase;

// Construction
public:
    HttpPhysicalDirectory(const String &name, const String &physicalPath = String::EMPTY);
	virtual ~HttpPhysicalDirectory();

// Attributes
public:
	inline const String & getDefaultFile() const;
	inline void setDefaultFile(const String &defaultFile);

	const String & getPhysicalPath() const;
	void setPhysicalPath(const String &physicalPath);

// VirtualDirectory overrides
protected:
	virtual bool handle(shared_ptr<HttpSession> session, const HttpPath &path);

protected:
	String m_defaultFile;
	String m_physicalPath;	
};

//////////////////////////////////////////////////////////////////////

inline const String & HttpPhysicalDirectory::getDefaultFile() const { return m_defaultFile; }
inline void HttpPhysicalDirectory::setDefaultFile(const String &defaultFile) { m_defaultFile = defaultFile; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPPHYSICALDIRECTORY_H
