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
#include "regex.h"

#include "boostregex.h"
#include "charset.h"
#include "log.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

shared_ptr<boost::wregex> regex_create(const String &regex, bool nocase)
{
	try
	{
		return shared_ptr<boost::wregex>(OS_NEW_T(boost::wregex)(regex.to_wide(), nocase ? boost::regex_constants::icase : boost::regex_constants::normal), os_delete_t());
	}
	catch(std::exception &e)
	{
		OS_LOG_DEBUG(String::format(_S("re:'%S'").c_str(), regex.c_str()));
		OS_LOG_ERROR(e.what());		
	}

	return nullptr;
}

bool regex_match(const String &str, shared_ptr<boost::wregex> regex)
{
	if(regex == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	return regex_match(str, *regex);
}

bool regex_match(const String &str, const boost::wregex &regex)
{
	try
	{
		return boost::regex_match(str.to_wide(), regex);
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(_S("regex_match error: ") + e.what());		
	}

	return false;
}

String regex_replace(const String &str, shared_ptr<boost::wregex> regex, const String &format)
{
	if(regex == nullptr)
	{
		OS_ASSERTFALSE();
		return String::EMPTY;
	}

	return regex_replace(str, *regex, format);
}

String regex_replace(const String &str, const boost::wregex &regex, const String &format)
{
	try
	{
		return boost::regex_replace(str.to_wide(), regex, format.to_wide());
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(_S("regex_replace error: ") + e.what());		
	}

	return str;
}

String regex_quote(const String &str)
{
	String result;

	size_t length = str.length();
	for(size_t i = 0; i < length; i++)
	{
		// URGENTSTRING: fixme
		uchar c = str.str()[i];
		switch(c)
		{
			case chPeriod:
			case chBackSlash:
			case chPlus:
			case chAsterisk:
			case chQuestion:
			case chOpenSquare:
			case chCaret:
			case chCloseSquare:
			case chDollarSign:
			case chOpenParen:
			case chCloseParen:
			case chOpenCurly:
			case chCloseCurly:
			case chEqual:
			case chBang:
			case chCloseAngle:
			case chOpenAngle:
			case chPipe:
			case chColon:
									result += chBackSlash;
									result += c;
									break;

			// URGENT: corretto?
			case chNull:			result += chBackSlash;
									result += chDigit_0;
									break;

			default:				result += c;
									break;
		}
	}

	return result;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
