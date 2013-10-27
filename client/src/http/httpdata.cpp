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
#include "httpdata.h"

#include "algorithms.h"
#include "buffer.h"
#include "http.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

HttpData::HttpData() : m_data(OS_NEW Buffer())
{
	
}

HttpData::~HttpData()
{
	
}

const byte * HttpData::getData() const
{
	return m_data->getData();
}

uint32 HttpData::getSize() const
{
	return m_data->getSize();
}

void HttpData::writeHeaderLine(const std::string &line)
{
	OS_ASSERT(line.empty() == false && algorithms::trim_copy(line) == line);

	write(line);
	writeHeaderLineTerminator();	
}

void HttpData::writeHeaderParam(const std::string &paramName, const std::string &paramValue, bool check)
{
	if(check && paramValue.empty() == false)
	{
		OS_ASSERT(paramName.empty() == false && algorithms::trim_copy(paramName) == paramName);
		OS_ASSERT(paramValue.empty() == false && algorithms::trim_copy(paramValue) == paramValue);

		write(paramName);
		write(": ");
		write(paramValue);
		writeHeaderLineTerminator();
	}
}

void HttpData::writeHeaderLineTerminator()
{
	write(OS_HTTP_HEADER_LINE_TERMINATOR);
}

void HttpData::write(const std::string &str)
{
	write(reinterpret_cast<const byte *>(str.data()), static_cast<uint32>(str.size()));
}

void HttpData::write(const Buffer &buffer)
{
	write(buffer.getData(), buffer.getSize());
}

void HttpData::write(const byte *data, uint32 size)
{
	m_data->write(data, size);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
