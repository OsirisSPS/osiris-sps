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
#include "languageresult.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

LanguageResult::LanguageResult()
{

}

LanguageResult::LanguageResult(const char *id) : m_id(id)
{

}

LanguageResult::LanguageResult(const std::string &id) : m_id(id)
{
	
}

LanguageResult::LanguageResult(const std::string &id, const StringCollection<String> &params)
{
	m_id = id;
	m_params = params;
}

LanguageResult::LanguageResult(const std::string &id, const std::string &key1, const DataItem &val1)	
{
	m_id = id;
	m_params.set(key1, val1);
}

LanguageResult::LanguageResult(const std::string &id, const std::string &key1, const DataItem &val1, const std::string &key2, const DataItem &val2)
{
	m_id = id;
	m_params.set(key1, val1);
	m_params.set(key2, val2);
}

LanguageResult::LanguageResult(const std::string &id, const std::string &key1, const DataItem &val1, const std::string &key2, const DataItem &val2, const std::string &key3, const DataItem &val3)
{
	m_id = id;
	m_params.set(key1, val1);
	m_params.set(key2, val2);
	m_params.set(key3, val3);
}

LanguageResult::~LanguageResult()
{
}

void LanguageResult::setNoTranslate(const std::string& text)
{
	m_id = std::string("~") + text;
}


void LanguageResult::setParam(const std::string &key, const DataItem &val)
{
	m_params.set(key, val);
}

void LanguageResult::clear()
{
	m_id.clear();
	m_params.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
