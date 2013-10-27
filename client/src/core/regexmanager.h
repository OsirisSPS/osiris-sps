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

#ifndef _OS_CORE_REGEXMANAGER_H
#define _OS_CORE_REGEXMANAGER_H

#include "regex.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

#define OS_REGEX_MANAGER_CACHE_SIZE				1000

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport RegexManager : public StaticSingleton<RegexManager>
{
	class RegexsCache;

// Construction
protected:
	RegexManager();

public:
	virtual ~RegexManager();

// Attributes
public:
	shared_ptr<boost::wregex> getRegex(const String &regex, bool nocase = true) const;

// Operation
public:
	bool match(const String &str, const String &regex, bool nocase = true) const;
	String replace(const String &str, const String &regex, const String &format, bool nocase = true) const;

private:
	scoped_ptr<RegexsCache> m_sensitivesRegexs;
	scoped_ptr<RegexsCache> m_insensitivesRegexs;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_REGEXMANAGER_H
