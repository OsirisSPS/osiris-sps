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
#include "conversions.h"

#include "boost/lexical_cast.hpp"
#include "charset.h"
#include "constants.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((conversions))

//////////////////////////////////////////////////////////////////////

template <typename T, typename S>
inline S number_to_string(const T &v)
{
	try
    {
		return boost::lexical_cast<S>(v);
    }
	catch(boost::bad_lexical_cast &)
    {

    }

	return S();
}

template <typename T, typename S>
inline T string_to_number(const S &v)
{
	try
    {
		if(v.empty())
			return 0;

		return boost::lexical_cast<T>(v);
    }
	catch(boost::bad_lexical_cast &)
    {

    }

	return 0;
}

//////////////////////////////////////////////////////////////////////

template <typename T>
inline static String number_to_string(const T &v)
{
	return number_to_string<T, std::wstring>(v);
}

template <typename T>
inline static T string_to_number(const String &v)
{
	return string_to_number<T, std::wstring>(v.to_wide());
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport std::string to_string<bool>(const bool &v)
{
	// N.B.: non modificare MAI queste funzioni visto che vengono richiamate
	// per generare le firme digitali degli oggetti
	return v ? "true" : "false";
}

template<>
CoreExport std::wstring to_wstring<bool>(const bool &v)
{
	// N.B.: non modificare MAI queste funzioni visto che vengono richiamate
	// per generare le firme digitali degli oggetti
	return v ? L"true" : L"false";
}

template<>
CoreExport String to_utf16<bool>(const bool &v)
{
	// N.B.: non modificare MAI queste funzioni visto che vengono richiamate
	// per generare le firme digitali degli oggetti
	return v ? _S("true") : _S("false");
}

template<>
CoreExport bool from_string<bool>(const std::string &v)
{
	return v == "true";
}

template<>
CoreExport bool from_wstring<bool>(const std::wstring &v)
{
	return v == L"true";
}

template<>
CoreExport bool from_utf16<bool>(const String &v)
{
	return v == _S("true");
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport std::string to_string<byte>(const byte &v)
{
	return number_to_string<byte, std::string>(v);
}

template<>
CoreExport std::wstring to_wstring<byte>(const byte &v)
{
	return number_to_string<byte, std::wstring>(v);
}

template<>
CoreExport String to_utf16<byte>(const byte &v)
{
	return number_to_string<byte>(v);
}

template<>
CoreExport byte from_string<byte>(const std::string &v)
{
	// TODO: fixme
	return static_cast<byte>(string_to_number<uint16, std::string>(v));
}

template<>
CoreExport byte from_wstring<byte>(const std::wstring &v)
{
	// TODO: fixme
	return static_cast<byte>(string_to_number<uint16, std::wstring>(v));
}

template<>
CoreExport byte from_utf16<byte>(const String &v)
{
	// TODO: fixme
	return static_cast<byte>(string_to_number<uint16>(v));
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport std::string to_string<int16>(const int16 &v)
{
	return number_to_string<int16, std::string>(v);
}

template<>
CoreExport std::wstring to_wstring<int16>(const int16 &v)
{
	return number_to_string<int16, std::wstring>(v);
}

template<>
CoreExport String to_utf16<int16>(const int16 &v)
{
	return number_to_string<int16>(v);
}

template<>
CoreExport int16 from_string<int16>(const std::string &v)
{
	return string_to_number<int16, std::string>(v);
}

template<>
CoreExport int16 from_wstring<int16>(const std::wstring &v)
{
	return string_to_number<int16, std::wstring>(v);
}

template<>
CoreExport int16 from_utf16<int16>(const String &v)
{
	return string_to_number<int16>(v);
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport std::string to_string<uint16>(const uint16 &v)
{
	return number_to_string<uint16, std::string>(v);
}

template<>
CoreExport std::wstring to_wstring<uint16>(const uint16 &v)
{
	return number_to_string<uint16, std::wstring>(v);
}

template<>
CoreExport String to_utf16<uint16>(const uint16 &v)
{
	return number_to_string<uint16>(v);
}

template<>
CoreExport uint16 from_string<uint16>(const std::string &v)
{
	return string_to_number<uint16, std::string>(v);
}

template<>
CoreExport uint16 from_wstring<uint16>(const std::wstring &v)
{
	return string_to_number<uint16, std::wstring>(v);
}

template<>
CoreExport uint16 from_utf16<uint16>(const String &v)
{
	return string_to_number<uint16>(v);
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport std::string to_string<int32>(const int32 &v)
{
	return number_to_string<int32, std::string>(v);
}

template<>
CoreExport std::wstring to_wstring<int32>(const int32 &v)
{
	return number_to_string<int32, std::wstring>(v);
}

template<>
CoreExport String to_utf16<int32>(const int32 &v)
{
	return number_to_string<int32>(v);
}

template<>
CoreExport int32 from_string<int32>(const std::string &v)
{
	return string_to_number<int32, std::string>(v);
}

template<>
CoreExport int32 from_wstring<int32>(const std::wstring &v)
{
	return string_to_number<int32, std::wstring>(v);
}

template<>
CoreExport int32 from_utf16<int32>(const String &v)
{
	return string_to_number<int32>(v);
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport std::string to_string<uint32>(const uint32 &v)
{
	return number_to_string<uint32, std::string>(v);
}

template<>
CoreExport std::wstring to_wstring<uint32>(const uint32 &v)
{
	return number_to_string<uint32, std::wstring>(v);
}

template<>
CoreExport String to_utf16<uint32>(const uint32 &v)
{
	return number_to_string<uint32>(v);
}

template<>
CoreExport uint32 from_string<uint32>(const std::string &v)
{
	return string_to_number<uint32, std::string>(v);
}

template<>
CoreExport uint32 from_wstring<uint32>(const std::wstring &v)
{
	return string_to_number<uint32, std::wstring>(v);
}

template<>
CoreExport uint32 from_utf16<uint32>(const String &v)
{
	return string_to_number<uint32>(v);
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport std::string to_string<int64>(const int64 &v)
{
	return number_to_string<int64, std::string>(v);
}

template<>
CoreExport std::wstring to_wstring<int64>(const int64 &v)
{
	return number_to_string<int64, std::wstring>(v);
}

template<>
CoreExport String to_utf16<int64>(const int64 &v)
{
	return number_to_string<int64>(v);
}

template<>
CoreExport int64 from_string<int64>(const std::string &v)
{
	return string_to_number<int64, std::string>(v);
}

template<>
CoreExport int64 from_wstring<int64>(const std::wstring &v)
{
	return string_to_number<int64, std::wstring>(v);
}

template<>
CoreExport int64 from_utf16<int64>(const String &v)
{
	return string_to_number<int64>(v);
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport std::string to_string<uint64>(const uint64 &v)
{
	return number_to_string<uint64, std::string>(v);
}

template<>
CoreExport std::wstring to_wstring<uint64>(const uint64 &v)
{
	return number_to_string<uint64, std::wstring>(v);
}

template<>
CoreExport String to_utf16<uint64>(const uint64 &v)
{
	return number_to_string<uint64>(v);
}

template<>
CoreExport uint64 from_string<uint64>(const std::string &v)
{
	return string_to_number<uint64, std::string>(v);
}

template<>
CoreExport uint64 from_wstring<uint64>(const std::wstring &v)
{
	return string_to_number<uint64, std::wstring>(v);
}

template<>
CoreExport uint64 from_utf16<uint64>(const String &v)
{
	return string_to_number<uint64>(v);
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport std::string to_string<float>(const float &v)
{
	return number_to_string<float, std::string>(v);
}

template<>
CoreExport std::wstring to_wstring<float>(const float &v)
{
	return number_to_string<float, std::wstring>(v);
}

template<>
CoreExport String to_utf16<float>(const float &v)
{
	return number_to_string<float>(v);
}

template<>
CoreExport float from_string<float>(const std::string &v)
{
	return string_to_number<float, std::string>(v);
	//return atof(conversions::to_string(v).c_str()); // CLODOFIX
}

template<>
CoreExport float from_wstring<float>(const std::wstring &v)
{
	return string_to_number<float, std::wstring>(v);
	//return atof(conversions::to_string(v).c_str()); // CLODOFIX
}

template<>
CoreExport float from_utf16<float>(const String &v)
{
	return string_to_number<float>(v);
	//return atof(conversions::to_string(v).c_str()); // CLODOFIX
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport std::string to_string<double>(const double &v)
{
	return number_to_string<double, std::string>(v);
}

template<>
CoreExport std::wstring to_wstring<double>(const double &v)
{
	return number_to_string<double, std::wstring>(v);
}

template<>
CoreExport String to_utf16<double>(const double &v)
{
	return number_to_string<double>(v);
}

template<>
CoreExport double from_string<double>(const std::string &v)
{
	return string_to_number<double, std::string>(v);
	//return atof(conversions::to_string(v).c_str()); // CLODOFIX
}

template<>
CoreExport double from_wstring<double>(const std::wstring &v)
{
	return string_to_number<double, std::wstring>(v);
	//return atof(conversions::to_string(v).c_str()); // CLODOFIX
}

template<>
CoreExport double from_utf16<double>(const String &v)
{
	return string_to_number<double>(v);
	//return atof(conversions::to_string(v).c_str()); // CLODOFIX
}

//////////////////////////////////////////////////////////////////////

template <>
CoreExport std::string to_utf8<String>(const String &v)
{
	return v.to_utf8();
}

template <>
CoreExport std::string to_utf8<std::wstring>(const std::wstring &v)
{
	String str(v);
	return str.to_utf8();
}

template <>
CoreExport String from_utf8<String>(const std::string &v)
{
	String str;
	str.from_utf8(v);
	return str;
}

template <>
CoreExport std::wstring from_utf8<std::wstring>(const std::string &v)
{
	String str;
	str.from_utf8(v);
	return str.to_wide();
}

//////////////////////////////////////////////////////////////////////

uchar intToHex(int32 v)
{
	/*
    OS_ASSERT(v >= 0 && v <= 15);
	if(v <= 9)
        return static_cast<uchar>(v + _S('0'));
	else
        return static_cast<uchar>(v - 10 + _S('A'));
	*/

    OS_ASSERT(v >= 0 && v <= 15);
	if(v <= 9)
        return static_cast<uchar>(v + chDigit_0);
	else
        return static_cast<uchar>(v - 10 + chLatin_A);
}

int32 hexToInt(uchar v)
{
	/*
    // character is in range of 0-9 subtract ascii value of '0' to get integer
	if((v >= _S('0')) && (v <= _S('9')))
		return v - _S('0');

	// caracter is in range of a-z or A-Z subtract ascii value of 'a' and add 10 to get it converted into int
	if((v >= _S('a')) && (v <= _S('f')))
        return v - _S('a') + 10;
	else if((v >= _S('A')) && (v <= _S('F')))
        return v - _S('A') + 10;
	*/

    // character is in range of 0-9 subtract ascii value of '0' to get integer
	if((v >= chDigit_0) && (v <= chDigit_9))
		return v - chDigit_0;

	// caracter is in range of a-z or A-Z subtract ascii value of 'a' and add 10 to get it converted into int
	if((v >= chLatin_a) && (v <= chLatin_f))
        return v - chLatin_a + 10;
	else if((v >= chLatin_A) && (v <= chLatin_F))
        return v - chLatin_A + 10;

	// caracter is not a Hex Digit
	return -1;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((conversions))

//////////////////////////////////////////////////////////////////////
