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
#include "ihttpdirectoryregexhandler.h"

#include "httppath.h"
#include "regex.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IHttpDirectoryRegexHandler::IHttpDirectoryRegexHandler()
{

}

IHttpDirectoryRegexHandler::~IHttpDirectoryRegexHandler()
{

}

bool IHttpDirectoryRegexHandler::addRegex(const String &regex, bool nocase)
{
	shared_ptr<boost::wregex> re = regex_create(regex, nocase);
	if(re == null)
		return false;

	m_regexps.push_back(re);
	return true;	
}

bool IHttpDirectoryRegexHandler::addRegexFile(const String &fileExtension)
{
	OS_ASSERT(fileExtension.empty() == false);
	OS_ASSERT(fileExtension.starts_with(".") == false);

	return addRegex(_S(".*\\.") + fileExtension);
}

bool IHttpDirectoryRegexHandler::matchRequest(const String &url) const
{
	for(Regexps::const_iterator i = m_regexps.begin(); i != m_regexps.end(); ++i)
	{
		shared_ptr<boost::wregex> re = *i;
		if(regex_match(url, *re))
			return true;
	}

	return false;	
}

bool IHttpDirectoryRegexHandler::handleRequest(shared_ptr<HttpSession> session, const HttpPath &path)
{
	if(matchRequest(path.getUrl()) == false)
		return false;

	return HandlerBase::handleRequest(session, path);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
