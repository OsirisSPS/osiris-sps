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
#include "httppostparam.h"

#include "constants.h"
#include "http.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const std::wstring HttpPostParam::NAME(L"name");
const std::wstring HttpPostParam::FILENAME(L"filename");

//////////////////////////////////////////////////////////////////////

HttpPostParam::HttpPostParam()
{

}

HttpPostParam::~HttpPostParam()
{

}

bool HttpPostParam::isTextPlain() const
{
	// Nota: se non specificato content-type si presume sia testo
	return m_contentType.empty() || (m_contentType == OS_HTTP_CONTENTTYPE_TEXTPLAIN);
}

const std::wstring & HttpPostParam::getString() const
{
	const std::wstring *value = boost::get<const std::wstring>(&m_value);
	if(value != null)
		return *value;

	return constants::empty_wstring;
}

void HttpPostParam::setString(const std::wstring &value)
{
	m_value = value;
}

const Buffer * HttpPostParam::getBuffer() const
{
	return boost::get<const Buffer>(&m_value);
}

void HttpPostParam::setBuffer(const Buffer &value)
{
	m_value = value;
}

const std::wstring & HttpPostParam::getName() const
{
	return getAttribute(NAME);
}

void HttpPostParam::setName(const std::wstring &name)
{
	setAttribute(NAME, name);
}

const std::wstring & HttpPostParam::getFilename() const
{
	return getAttribute(FILENAME);
}

void HttpPostParam::setFilename(const std::wstring &filename)
{
	setAttribute(FILENAME, filename);
}

void HttpPostParam::clear()
{
	m_value = boost::none;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
