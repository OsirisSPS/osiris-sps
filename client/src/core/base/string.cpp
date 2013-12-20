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
#include "base/string.h"

#include "iculayer.h"
#include <stdarg.h>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String String::EMPTY;
const String String::DEFAULT_TRIM = _S(" \t\r\n");

//////////////////////////////////////////////////////////////////////

inline static void import_raw_char(std::ustring &str, const achar *data, icu_size len = -1)
{
	str = icu_asciitou(data, len);
}

inline static void import_raw_char(std::ustring &str, const std::string &data)
{
	import_raw_char(str, data.data(), static_cast<icu_size>(data.size()));
}

//////////////////////////////////////////////////////////////////////

String::String(const String &first, const String &second)
{
    m_str.reserve(first.length() + second.length());

	m_str.append(first.m_str);
	m_str.append(second.m_str);
}

String::String(const char_type *first, const String &second)
{
	if(first != nullptr)
	{
		m_str.reserve(std::char_traits<char_type>::length(first) + second.length());
		m_str.append(first);
		m_str.append(second.m_str);
	}
	else
	{
		m_str = second.m_str;
	}
}

String::String(const String &first, const char_type *second)
{
	if(second != nullptr)
	{
		m_str.reserve(first.length() + std::char_traits<char_type>::length(second));
		m_str.append(first.m_str);
		m_str.append(second);
	}
	else
	{
		m_str = first.m_str;
	}
}

String::String(char_type first, const String &second)
{
	m_str.reserve(1 + second.length());

	m_str.append(1, first);
	m_str.append(second.m_str);
}

String::String(const String &first, char_type second)
{
	m_str.reserve(first.length() + 1);

	m_str.append(first.m_str);
	m_str.append(1, second);
}

String::String(const achar *str)
{	
	//m_str = icu_utf8tou(str); 
	import_raw_char(m_str, str);
}

String::String(const std::string &str)
{
	//m_str = icu_utf8tou(str);
	import_raw_char(m_str, str);
}

#ifndef OS_HAS_NATIVE_UCHAR
String::String(const wchar *str)
{
	m_str = icu_wtou(str);
}

String::String(const std::wstring &str)
{
	m_str = icu_wtou(str);
}

#endif // OS_HAS_NATIVE_UCHAR

String::~String()
{

}

String & String::operator =(const achar *str)
{
	//m_str = icu_utf8tou(str);
	import_raw_char(m_str, str);

	return *this;
}

String & String::operator =(const std::string &str)
{
	//m_str = icu_utf8tou(str);
	import_raw_char(m_str, str);

	return *this;
}

#ifndef OS_HAS_NATIVE_UCHAR
String & String::operator =(const wchar *str)
{
	m_str = icu_wtou(str);
	return *this;
}

String & String::operator =(const std::wstring &str)
{
	m_str = icu_wtou(str);
	return *this;
}
#endif // OS_HAS_NATIVE_UCHAR

bool String::compare_no_case(const String &second) const
{
//	return os_strcmpi(m_str.c_str(), second.c_str()) == 0;
	return icu_compare_no_case(str(), second.str());
}

bool String::starts_with(const String &pattern, bool noCase) const
{
    size_t thisLen = m_str.length();
    size_t patternLen = pattern.length();
    if (thisLen < patternLen || patternLen == 0)
        return false;

    String startOfThis = m_str.substr(0, patternLen);
    if(noCase)
		return startOfThis.compare_no_case(pattern);

    return startOfThis == pattern;
}

bool String::ends_with(const String &pattern, bool noCase) const
{
    size_t thisLen = m_str.length();
    size_t patternLen = pattern.length();
    if(thisLen < patternLen || patternLen == 0)
        return false;

    String endOfThis = m_str.substr(thisLen - patternLen, patternLen);
    if(noCase)
		return endOfThis.compare_no_case(pattern);

    return endOfThis == pattern;
}

String & String::replace_all(const String &old_value, const String &new_value)
{
	size_t index = 0;
	size_t oldLen = old_value.length();
	size_t newLen = new_value.length();

	while((index = m_str.find(old_value.c_str(), index)) != npos)
	{
		m_str.replace(index, oldLen, new_value.c_str());
		index += newLen;
	}

	return *this;
}

String & String::remove(const String &value)
{
	replace_all(value, EMPTY);

	return *this;
}

String String::left(const size_type &count) const
{
	return m_str.substr(0, count);
}

String String::mid(const size_type &begin, const size_type &count) const
{
	return m_str.substr(begin, count);
}

String String::right(const size_type &count) const
{
	size_type begin = 0;
	if(count < m_str.length())
        begin = m_str.length() - count;

	return m_str.substr(begin, count);
}

String & String::trim(const String &delims)
{
	trim_left(delims);
	trim_right(delims);

	return *this;
}

String & String::trim_left(const String &delims)
{
	m_str.erase(0, m_str.find_first_not_of(delims.m_str));
	return *this;
}

String & String::trim_right(const String &delims)
{
	m_str.erase(m_str.find_last_not_of(delims.m_str) + 1);
	return *this;
}

String & String::to_lower()
{
//	std::transform(m_str.begin(), m_str.end(), m_str.begin(), tolower);
	m_str = icu_to_lower(m_str);
	return *this;
}

String & String::to_upper()
{
//	std::transform(m_str.begin(), m_str.end(), m_str.begin(), toupper);
	m_str = icu_to_upper(m_str);
	return *this;
}

String String::find_next(const String &str, size_t &start) const
{
	String result;
	if(start != npos)
	{
		size_t token = find(str, start);
		if(token != npos)
		{
			size_t len = token - start;
			if(len > 0)
			{
				result = mid(start, len);
				start += len + 1;
			}
		}
		else
		{
			result = mid(start);
			start = npos;
		}
	}

	return result;
}

String::size_type String::scanf(const uchar *pattern, ...) const
{
	va_list args;
	va_start(args, pattern);

	size_type count = icu_scanf(c_str(), pattern, args);

	va_end(args);

	return count;
}

String String::format(const uchar *pattern, ...)
{
	va_list args;
	va_start(args, pattern);

	String str = icu_format_string(pattern, args);

	va_end(args);

	return str;
}

String::size_type String::sscanf(const String &str, const uchar *pattern, ...)
{
	va_list args;
	va_start(args, pattern);

	size_type count = icu_scanf(str.c_str(), pattern, args);

	va_end(args);

	return count;
}

String & String::from_ascii(const achar *str)
{
	m_str = icu_asciitou(str);
	return *this;
}

String & String::from_ascii(const std::string &str)
{
	m_str = icu_asciitou(str);
	return *this;
}

std::string String::to_ascii() const
{
	return icu_utoascii(m_str);
}

String & String::from_utf8(const achar *str)
{
	m_str = icu_utf8tou(str);
	return *this;
}

String & String::from_utf8(const std::string &str)
{
	m_str = icu_utf8tou(str);
	return *this;
}

std::string String::to_utf8() const
{
	return icu_utoutf8(m_str);
}

String & String::from_wide(const wchar *str)
{
	m_str = icu_wtou(str);
	return *this;
}

String & String::from_wide(const std::wstring &str)
{
	m_str = icu_wtou(str);
	return *this;
}

std::wstring String::to_wide() const
{
	return icu_utow(m_str);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
