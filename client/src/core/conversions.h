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

#ifndef _OS_CORE_CONVERSIONS_H
#define _OS_CORE_CONVERSIONS_H

#include "noninstantiable.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((conversions))

//////////////////////////////////////////////////////////////////////

template <typename T>
std::string to_string(const T &v);

template <typename T>
std::wstring to_wstring(const T &v);

template <typename T>
std::string to_utf8(const T &v);

template <typename T>
String to_utf16(const T &v);

//////////////////////////////////////////////////////////////////////

template <typename T>
T from_string(const std::string &v);

template <typename T>
T from_wstring(const std::wstring &v);

template <typename T>
T from_utf8(const std::string &v);

template <typename T>
T from_utf16(const String &v);

//////////////////////////////////////////////////////////////////////

template <typename T>
inline T from_string(const std::string &v, const T &defaultValue)
{
	if(v.empty())
		return defaultValue;

	return from_string<T>(v);
}

template <typename T>
inline T from_wstring(const std::wstring &v, const T &defaultValue)
{
	if(v.empty())
		return defaultValue;

	return from_wstring<T>(v);
}

template <typename T>
inline T from_utf16(const String &v, const T &defaultValue)
{
	if(v.empty())
		return defaultValue;

	return from_utf16<T>(v);
}

//////////////////////////////////////////////////////////////////////

template <typename T>
T toEnumeratedType(uint32 v);

template <typename T>
uint32 fromEnumeratedType(T v) { return static_cast<uint32>(v); }

//////////////////////////////////////////////////////////////////////

CoreExport uchar intToHex(int32 v);
CoreExport int32 hexToInt(uchar v);

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((conversions))

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
struct convert : public noninstantiable
{
	inline static T from(const std::string &value)
	{
		return conversions::from_string<T>(value);
	}

	inline static T from(const std::wstring &value)
	{
		return conversions::from_wstring<T>(value);
	}

	inline static T from(const String &value)
	{
		return conversions::from_utf16<T>(value);
	}
};

//////////////////////////////////////////////////////////////////////

template <>
struct convert<std::string> : public noninstantiable
{
	template <typename V>
	inline static std::string from(const V &value)
	{
		return conversions::to_string(value);
	}
};

template <>
struct convert<std::wstring> : public noninstantiable
{
	template <typename V>
	inline static std::wstring from(const V &value)
	{
		return conversions::to_wstring(value);
	}
};

template <>
struct convert<String> : public noninstantiable
{
	template <typename V>
	inline static String from(const V &value)
	{
		return conversions::to_utf16(value);
	}
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_CONVERSIONS_H
