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

#ifndef _OS_HTTP_HTTPVIRTUALDIRECTORY_H
#define _OS_HTTP_HTTPVIRTUALDIRECTORY_H

#include "ihttpdirectory.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IHttpDirectoryHandler;

//////////////////////////////////////////////////////////////////////

class HttpExport HttpVirtualDirectory : public IHttpDirectory
{
	typedef IHttpDirectory DirectoryBase;

	typedef list<shared_ptr<IHttpDirectoryHandler> >::type Handlers;

// Construction
public:
    HttpVirtualDirectory(const String &name);
	virtual ~HttpVirtualDirectory();

// Attributes
public:
	inline const Handlers & getHandlers() const;

// Operations
public:
	// Registra un handler
	void addHandler(shared_ptr<IHttpDirectoryHandler> handler);
	// Deregistra un handler
	void removeHandler(shared_ptr<IHttpDirectoryHandler> handler);

// IDirectory interface
protected:
	virtual bool handle(shared_ptr<HttpSession> session, const HttpPath &path);

protected:
	mutable boost::recursive_mutex m_cs;
	Handlers m_handlers;
};

//////////////////////////////////////////////////////////////////////

inline const HttpVirtualDirectory::Handlers & HttpVirtualDirectory::getHandlers() const { return m_handlers; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPVIRTUALDIRECTORY_H
