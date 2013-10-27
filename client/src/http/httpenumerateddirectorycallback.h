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

#ifndef _OS_HTTP_HTTPENUMERATEDDIRECTORYCALLBACK_H
#define _OS_HTTP_HTTPENUMERATEDDIRECTORYCALLBACK_H

#include "boost/function.hpp"
#include "ihttpdirectory.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpDirectoryCallback;

//////////////////////////////////////////////////////////////////////

class HttpExport HttpEnumeratedDirectoryCallback : public IHttpDirectory
{
	typedef IHttpDirectory DirectoryBase;

	typedef boost::function<bool (shared_ptr<HttpSession>, const HttpPath &)> Callback;

// Construction
public:
    HttpEnumeratedDirectoryCallback(const String &name);
	virtual ~HttpEnumeratedDirectoryCallback();

// IDirectory interface
protected:
	virtual bool handle(shared_ptr<HttpSession> session, const HttpPath &path);	

// Operations
public:
	shared_ptr<HttpDirectoryCallback> addDirectoryCallback(const Callback &callback);

private:
	boost::recursive_mutex m_callbackCS;
	uint32 m_callbackID;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPENUMERATEDDIRECTORYCALLBACK_H
