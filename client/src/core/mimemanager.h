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

#ifndef _OS_HTTP_MIMEMANAGER_H
#define _OS_HTTP_MIMEMANAGER_H

#include "boost/thread/recursive_mutex.hpp"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport MimeManager : public StaticSingleton<MimeManager>
{
// Construction
protected:
    MimeManager();

public:
	virtual ~MimeManager();

// Attributes
public:
	bool getMimeType(String extension, String &mimeType);
	std::string getMimeType(std::string extension);
	String getDefaultExtension(String mimeType);

// Operations
public:
	bool getCompressible(const std::string &mimeType);

private:
	bool checkDefaultMimeType(const String &extension, String &mimeType);
	bool checkSystemMimeType(const String &extension, String &mimeType);

	bool checkDefaultExtension(const String &mimeType, String &extension);
	bool checkSystemExtension(const String &mimeType, String &extension);

	String standardiseValue(const String &value);

private:
	boost::recursive_mutex m_cs;
	unordered_map<String, String>::type m_mimeTypes;		// extension -> content_type
	unordered_map<String, String>::type m_extensions;		// content_type -> extension
    unordered_map<String, String>::type m_defaultMimeTypes;

	static const String DEFAULT_MIMETYPE;
	static const String DEFAULT_EXTENSION;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTTP_MIMEMANAGER_H
