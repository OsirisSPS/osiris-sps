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
#include "base/exception.h"

#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

Exception::Exception(const std::string &description, const char *file, uint32 line, const char *function) : m_description(description),
																											m_file(utils::safe_string<std::string>(file)),
																											m_line(line),
																											m_function(utils::safe_string<std::string>(function))
{
	
}

Exception::Exception(const Exception &second) : ExceptionBase(second)
{
	*this = second;
}

Exception::~Exception() throw()
{

}

const char * Exception::what() const throw()
{
	return m_description.c_str();
}

void Exception::operator =(const Exception &second)
{
	ExceptionBase::operator =(second);

    m_description = second.m_description;
    m_file = second.m_file;
	m_line = second.m_line;
	m_function = second.m_function;	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
