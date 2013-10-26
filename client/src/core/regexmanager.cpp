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
#include "regexmanager.h"

#include "icache.h"
#include "regex.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class RegexManager::RegexsCache : public ICache<String, boost::wregex, bool>
{
	typedef ICache<String, boost::wregex, bool> CacheBase;

// Construction
public:
	RegexsCache(uint32 limit);
	virtual ~RegexsCache();

// Interface
protected:
	virtual shared_ptr<boost::wregex> load(const String &key, bool nocase);
};

//////////////////////////////////////////////////////////////////////

RegexManager::RegexsCache::RegexsCache(uint32 limit) : CacheBase(limit)
{

}

RegexManager::RegexsCache::~RegexsCache()
{

}

shared_ptr<boost::wregex> RegexManager::RegexsCache::load(const String &key, bool nocase)
{
	return regex_create(key, nocase);
}

//////////////////////////////////////////////////////////////////////

RegexManager::RegexManager() : m_sensitivesRegexs(new RegexsCache(OS_REGEX_MANAGER_CACHE_SIZE)),
							   m_insensitivesRegexs(new RegexsCache(OS_REGEX_MANAGER_CACHE_SIZE))
{

}

RegexManager::~RegexManager()
{

}

shared_ptr<boost::wregex> RegexManager::getRegex(const String &regex, bool nocase) const
{
	if(nocase)
		return m_insensitivesRegexs->get(regex, nocase);
	
	return m_sensitivesRegexs->get(regex, nocase);
}

bool RegexManager::match(const String &str, const String &regex, bool nocase) const
{
	return regex_match(str, getRegex(regex, nocase));
}

String RegexManager::replace(const String &str, const String &regex, const String &format, bool nocase) const
{
	return regex_replace(str, getRegex(regex, nocase), format);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
