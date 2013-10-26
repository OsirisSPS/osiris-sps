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
#include "mimemanager.h"

#include "boost/assign/list_of.hpp"
#include "lock.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String MimeManager::DEFAULT_MIMETYPE = _S("text/plain");
const String MimeManager::DEFAULT_EXTENSION = _S("");

//////////////////////////////////////////////////////////////////////

MimeManager::MimeManager()
{
	/*
    m_defaultMimeTypes = boost::assign::map_list_of(_S("application/osiris"), _S("osiris"))
												   (_S("application/gnutar"), _S("tgz"))
												   (_S("application/javascript"), _S("js"))
												   (_S("application/rss+xml"), _S("rdf"))
                                                   (_S("application/pdf"), _S("pdf"))
                                                   (_S("application/x-bzip"), _S("bz"))
                                                   (_S("application/x-bzip2"), _S("bz2"))
                                                   (_S("application/x-gzip"), _S("gz"))
												   (_S("application/x-javascript"), _S("js"))
                                                   (_S("application/x-macbinary"), _S("bin"))
                                                   (_S("application/x-shockwave-flash"), _S("swf"))
                                                   (_S("application/x-tar"), _S("tar"))
                                                   (_S("application/zip"), _S("zip"))
                                                   (_S("audio/midi"), _S("mid"))
                                                   (_S("audio/mpeg"), _S("mpg"))
                                                   (_S("audio/mpeg3"), _S("mp3"))
                                                   (_S("audio/wav"), _S("wav"))
                                                   (_S("image/bmp"), _S("bmp"))
                                                   (_S("image/gif"), _S("gif"))
                                                   (_S("image/jpeg"), _S("jpg"))
                                                   (_S("image/png"), _S("png"))
                                                   (_S("image/tiff"), _S("tif"))
                                                   (_S("image/x-icon"), _S("ico"))
                                                   (_S("text/css"), _S("css"))
                                                   (_S("text/html"), _S("htm"))
												   (_S("text/javascript"), _S("js"))
                                                   (_S("text/plain"), _S("txt"))
                                                   (_S("text/richtext"), _S("rtf"))
                                                   (_S("text/xml"), _S("xml"))
                                                   (_S("video/avi"), _S("avi"))
                                                   (_S("video/mpeg"), _S("mpeg"))
                                                   (_S("video/quicktime"), _S("mov"));*/
	m_defaultMimeTypes[_S("application/osiris")] = _S("osiris");
	m_defaultMimeTypes[_S("application/gnutar")] = _S("tgz");
	m_defaultMimeTypes[_S("application/javascript")] = _S("js");
	m_defaultMimeTypes[_S("application/rss+xml")] = _S("rdf");
	m_defaultMimeTypes[_S("application/pdf")] = _S("pdf");
	m_defaultMimeTypes[_S("application/x-bzip")] = _S("bz");
	m_defaultMimeTypes[_S("application/x-bzip2")] = _S("bz2");
	m_defaultMimeTypes[_S("application/x-gzip")] = _S("gz");
	m_defaultMimeTypes[_S("application/x-javascript")] = _S("js");
	m_defaultMimeTypes[_S("application/x-macbinary")] = _S("bin");
	m_defaultMimeTypes[_S("application/x-shockwave-flash")] = _S("swf");
	m_defaultMimeTypes[_S("application/x-tar")] = _S("tar");
	m_defaultMimeTypes[_S("application/zip")] = _S("zip");
	m_defaultMimeTypes[_S("audio/midi")] = _S("mid");
	m_defaultMimeTypes[_S("audio/mpeg")] = _S("mpg");
	m_defaultMimeTypes[_S("audio/mpeg3")] = _S("mp3");
	m_defaultMimeTypes[_S("audio/wav")] = _S("wav");
	m_defaultMimeTypes[_S("image/bmp")] = _S("bmp");
	m_defaultMimeTypes[_S("image/gif")] = _S("gif");
	m_defaultMimeTypes[_S("image/jpeg")] = _S("jpg");
	m_defaultMimeTypes[_S("image/png")] = _S("png");
	m_defaultMimeTypes[_S("image/tiff")] = _S("tif");
	m_defaultMimeTypes[_S("image/x-icon")] = _S("ico");
	m_defaultMimeTypes[_S("text/css")] = _S("css");	
	m_defaultMimeTypes[_S("text/html")] = _S("htm");
	m_defaultMimeTypes[_S("text/javascript")] = _S("js");
	m_defaultMimeTypes[_S("text/plain")] = _S("txt");
	m_defaultMimeTypes[_S("text/richtext")] = _S("rtf");
	m_defaultMimeTypes[_S("text/xml")] = _S("xml");
	m_defaultMimeTypes[_S("video/avi")] = _S("avi");
	m_defaultMimeTypes[_S("video/mpeg")] = _S("mpeg");
	m_defaultMimeTypes[_S("video/quicktime")] = _S("mov");
}

MimeManager::~MimeManager()
{

}

std::string MimeManager::getMimeType(std::string extension)
{
	String mimeType;
	if(getMimeType(extension, mimeType))
		return mimeType.to_ascii();
	else
		return "";
}

bool MimeManager::getMimeType(String extension, String &mimeType)
{
	OS_LOCK(m_cs);

	bool match = false;

	extension = standardiseValue(extension);

	unordered_map<String, String>::type::iterator i = m_mimeTypes.find(extension);
	if(i != m_mimeTypes.end())
	{
		mimeType = i->second;
		match = true;
	}

	if(match == false)
	{
		if(checkDefaultMimeType(extension, mimeType))
		{
			m_mimeTypes[extension] = mimeType;
			match = true;
		}
	}

	if(match == false)
	{
		if(checkSystemMimeType(extension, mimeType))
		{
			m_mimeTypes[extension] = mimeType;
			match = true;
		}
	}

	if(match == false)
	{
		mimeType = DEFAULT_MIMETYPE;
		return false;
	}

	return true;
}

String MimeManager::getDefaultExtension(String mimeType)
{
	OS_LOCK(m_cs);

	mimeType = standardiseValue(mimeType);

	String extension;
	bool match = false;

	unordered_map<String, String>::type::iterator i = m_extensions.find(mimeType);
	if(i != m_extensions.end())
	{
		extension = i->second;
		match = true;
	}

	if(match == false)
	{
		if(checkDefaultExtension(mimeType, extension))
		{
			m_extensions[mimeType] = extension;
			match = true;
		}
	}

	if(match == false)
	{
		if(checkSystemExtension(mimeType, extension))
		{
			m_extensions[mimeType] = extension;
			match = true;
		}
	}

	if(match == false)
		extension = DEFAULT_EXTENSION;

	return extension;
}

bool MimeManager::getCompressible(const std::string &mimeType)
{
	if( 
		(mimeType == "application/x-javascript") ||
		(mimeType == "application/javascript") ||
		(mimeType == "application/ecmascript") ||
		(mimeType == "application/rss+xml") ||
		(mimeType == "application/x-www-form-urlencoded") ||
		(mimeType == "multipart/form-data") ||
		(mimeType == "text/css") ||
		(mimeType == "text/html") ||
		(mimeType == "text/javascript") ||
		(mimeType == "text/plain") ||
		(mimeType == "text/richtext") ||
		(mimeType == "text/xml") )
		return true;
	else
		return false;
}

bool MimeManager::checkDefaultMimeType(const String &extension, String &mimeType)
{
	OS_ASSERT(extension == standardiseValue(extension));

	for(unordered_map<String, String>::type::const_iterator i = m_defaultMimeTypes.begin(); i != m_defaultMimeTypes.end(); ++i)
	{
		if(extension == i->second)
		{
			OS_ASSERT(i->first == standardiseValue(i->first));
			mimeType = i->first;

			return true;
		}
	}

	return false;
}

bool MimeManager::checkSystemMimeType(const String &extension, String &mimeType)
{
	if(PlatformManager::instance()->contentTypeFromExtension(extension, mimeType) == false)
		return false;

	mimeType = standardiseValue(mimeType);
	return true;
}

bool MimeManager::checkDefaultExtension(const String &mimeType, String &extension)
{
	OS_ASSERT(mimeType == standardiseValue(mimeType));

	for(unordered_map<String, String>::type::const_iterator i = m_defaultMimeTypes.begin(); i != m_defaultMimeTypes.end(); ++i)
	{
		if(mimeType == i->first)
		{
			OS_ASSERT(i->second == standardiseValue(i->second));
			extension = i->second;

			return true;
		}
	}

	return false;
}

bool MimeManager::checkSystemExtension(const String &mimeType, String &extension)
{
	if(PlatformManager::instance()->extensionFromContentType(mimeType, extension) == false)
		return false;

	extension = standardiseValue(extension);
	return true;
}

String MimeManager::standardiseValue(const String &value)
{
	String standardised = value;
	standardised.to_lower();
	standardised.trim(_S("."));
	return standardised;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
