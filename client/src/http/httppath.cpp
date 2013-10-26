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
#include "httppath.h"

#include "constants.h"
#include "http.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HttpPath::HttpPath(const String &url)
{
	_parse(url);
}

HttpPath::HttpPath(const HttpPath &second)
{
	*this = second;
}

HttpPath::~HttpPath()
{
	_clear();
}

const String & HttpPath::getDirectory() const
{
	return m_tokens.size() > 0 ? m_tokens[0] : String::EMPTY;
}

const String & HttpPath::getToken(uint32 index) const
{
	return index < m_tokens.size() ? m_tokens[index] : String::EMPTY;
}

bool HttpPath::isSafe() const
{
	return m_url.find(_S("..")) == String::npos;
}

String HttpPath::getRelativePath(uint32 index) const
{
	return utils::desplit(m_tokens, OS_HTTP_PATH_SEPARATOR, index);
}

String HttpPath::getTarget() const
{
	return getRelativePath(1);
}

HttpPath & HttpPath::operator =(const HttpPath &second)
{
	_clear();

	m_url = second.m_url;
	m_tokens = second.m_tokens;

	return *this;
}

void HttpPath::set(const String &url)
{
	_parse(url);
}

void HttpPath::_clear()
{
	m_url.clear();
	m_tokens.clear();
}

void HttpPath::_parse(const String &url)
{
	_clear();

	m_url = utils::standardisePath(url).trim_right(OS_HTTP_PATH_SEPARATOR);
	utils::split(m_url, OS_HTTP_PATH_SEPARATOR, m_tokens, true);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
