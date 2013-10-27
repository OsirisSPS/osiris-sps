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

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_bool_conversions)
{
	BOOST_CHECK(conversions::from_utf16<bool>(_S("true")));
	BOOST_CHECK(conversions::to_utf16<bool>(true) == _S("true"));
	BOOST_CHECK(conversions::from_utf16<bool>(_S("false")) == false);
	BOOST_CHECK(conversions::to_utf16<bool>(false) == _S("false"));
	BOOST_CHECK(conversions::from_utf16<bool>(String::EMPTY) == false);
	BOOST_CHECK(conversions::from_utf16<bool>(_S("0")) == false);
	BOOST_CHECK(conversions::from_utf16<bool>(_S("1")) == false);
	BOOST_CHECK(conversions::from_utf16<bool>(_S("foo")) == false);
	BOOST_CHECK(conversions::from_utf16<bool>(_S("TRUE")) == false);
}

BOOST_AUTO_TEST_CASE(test_byte_conversions)
{
	BOOST_CHECK(conversions::from_utf16<byte>(_S("0")) == byte(0));
	BOOST_CHECK(conversions::to_utf16<byte>(0) == _S("0"));
	BOOST_CHECK(conversions::from_utf16<byte>(_S("255")) == byte(255));
	BOOST_CHECK(conversions::to_utf16<byte>(255) == _S("255"));
	BOOST_CHECK(conversions::from_utf16<byte>(_S("256")) == byte(0));							// Overflow
}

BOOST_AUTO_TEST_CASE(test_int16_conversions)
{
	BOOST_CHECK(conversions::from_utf16<int16>(_S("0")) == int16(0));
	BOOST_CHECK(conversions::to_utf16<int16>(0) == _S("0"));
	BOOST_CHECK(conversions::from_utf16<int16>(_S("123")) == int16(123));
	BOOST_CHECK(conversions::to_utf16<int16>(123) == _S("123"));
	BOOST_CHECK(conversions::from_utf16<int16>(_S("-123")) == int16(-123));
	BOOST_CHECK(conversions::to_utf16<int16>(-123) == _S("-123"));
	BOOST_CHECK(conversions::from_utf16<int16>(_S("32767")) == int16(32767));
	BOOST_CHECK(conversions::to_utf16<int16>(32767) == _S("32767"));
	BOOST_CHECK(conversions::from_utf16<int16>(_S("-32767")) == int16(-32767));
	BOOST_CHECK(conversions::to_utf16<int16>(-32767) == _S("-32767"));
	BOOST_CHECK(conversions::from_utf16<int16>(_S("32768")) == int16(0));						// Overflow
	BOOST_CHECK(conversions::from_utf16<int16>(_S("-32768")) == int16(-32768));
	BOOST_CHECK(conversions::to_utf16<int16>(-32768) == _S("-32768"));
	BOOST_CHECK(conversions::from_utf16<int16>(_S("-32769")) == int16(0));						// Overflow
	BOOST_CHECK(conversions::from_utf16<int16>(_S("65535")) == int16(0));						// Overflow
}

BOOST_AUTO_TEST_CASE(test_uint16_conversions)
{
	BOOST_CHECK(conversions::from_utf16<uint16>(_S("0")) == uint16(0));
	BOOST_CHECK(conversions::to_utf16<uint16>(0) == _S("0"));
	BOOST_CHECK(conversions::from_utf16<uint16>(_S("123")) == uint16(123));
	BOOST_CHECK(conversions::to_utf16<uint16>(123) == _S("123"));
	BOOST_CHECK(conversions::from_utf16<uint16>(_S("32767")) == uint16(32767));
	BOOST_CHECK(conversions::to_utf16<uint16>(32767) == _S("32767"));
	BOOST_CHECK(conversions::from_utf16<uint16>(_S("32768")) == uint16(32768));
	BOOST_CHECK(conversions::to_utf16<uint16>(32768) == _S("32768"));
	BOOST_CHECK(conversions::from_utf16<uint16>(_S("65535")) == uint16(65535));
	BOOST_CHECK(conversions::to_utf16<uint16>(65535) == _S("65535"));
	BOOST_CHECK(conversions::from_utf16<uint16>(_S("65536")) == uint16(0));						// Overflow
}

BOOST_AUTO_TEST_CASE(test_int32_conversions)
{
	BOOST_CHECK(conversions::from_utf16<int32>(_S("0")) == int32(0));
	BOOST_CHECK(conversions::to_utf16<int32>(0) == _S("0"));
	BOOST_CHECK(conversions::from_utf16<int32>(_S("123")) == int32(123));
	BOOST_CHECK(conversions::to_utf16<int32>(123) == _S("123"));
	BOOST_CHECK(conversions::from_utf16<int32>(_S("-123")) == int32(-123));
	BOOST_CHECK(conversions::to_utf16<int32>(-123) == _S("-123"));
	BOOST_CHECK(conversions::from_utf16<int32>(_S("2147483647")) == int32(2147483647));
	BOOST_CHECK(conversions::to_utf16<int32>(2147483647) == _S("2147483647"));
	BOOST_CHECK(conversions::from_utf16<int32>(_S("-2147483647")) == int32(-2147483647));
	BOOST_CHECK(conversions::to_utf16<int32>(-2147483647) == _S("-2147483647"));
	BOOST_CHECK(conversions::from_utf16<int32>(_S("2147483648")) == int32(0));					// Overflow
	BOOST_CHECK(conversions::from_utf16<int32>(_S("4294967295")) == int32(0));					// Overflow
}

BOOST_AUTO_TEST_CASE(test_uint32_conversions)
{
	BOOST_CHECK(conversions::from_utf16<uint32>(_S("0")) == uint32(0));
	BOOST_CHECK(conversions::to_utf16<uint32>(0) == _S("0"));
	BOOST_CHECK(conversions::from_utf16<uint32>(_S("123")) == uint32(123));
	BOOST_CHECK(conversions::to_utf16<uint32>(123) == _S("123"));
	BOOST_CHECK(conversions::from_utf16<uint32>(_S("2147483647")) == uint32(2147483647));
	BOOST_CHECK(conversions::to_utf16<uint32>(2147483647) == _S("2147483647"));
	BOOST_CHECK(conversions::from_utf16<uint32>(_S("2147483648")) == uint32(2147483648LL));
	BOOST_CHECK(conversions::from_utf16<uint32>(_S("4294967295")) == uint32(4294967295LL));
	BOOST_CHECK(conversions::from_utf16<uint32>(_S("4294967296")) == uint32(0));				// Overflow
}

BOOST_AUTO_TEST_CASE(test_int64_conversions)
{
	BOOST_CHECK(conversions::from_utf16<int64>(_S("0")) == int64(0));
	BOOST_CHECK(conversions::to_utf16<int64>(0) == _S("0"));
	BOOST_CHECK(conversions::from_utf16<int64>(_S("123")) == int64(123));
	BOOST_CHECK(conversions::to_utf16<int64>(123) == _S("123"));
	BOOST_CHECK(conversions::from_utf16<int64>(_S("-123")) == int64(-123));
	BOOST_CHECK(conversions::to_utf16<int64>(-123) == _S("-123"));
}

BOOST_AUTO_TEST_CASE(test_uint64_conversions)
{
	BOOST_CHECK(conversions::from_utf16<uint64>(_S("0")) == uint64(0));
	BOOST_CHECK(conversions::to_utf16<uint64>(0) == _S("0"));
	BOOST_CHECK(conversions::from_utf16<uint64>(_S("123")) == uint64(123));
	BOOST_CHECK(conversions::to_utf16<uint64>(123) == _S("123"));
}

BOOST_AUTO_TEST_CASE(test_double_conversions)
{
	BOOST_CHECK(conversions::from_utf16<double>(_S("0")) == double(0));
	BOOST_CHECK(conversions::to_utf16<double>(0) == _S("0"));
	BOOST_CHECK(conversions::from_utf16<double>(_S("123")) == double(123));
	BOOST_CHECK(conversions::to_utf16<double>(123) == _S("123"));
	BOOST_CHECK(conversions::from_utf16<double>(_S("-123")) == double(-123));
	BOOST_CHECK(conversions::to_utf16<double>(-123) == _S("-123"));
	BOOST_CHECK(conversions::from_utf16<double>(_S("123.123")) == double(123.123));
	BOOST_CHECK(conversions::to_utf16<double>(123.123) == _S("123.123"));
	BOOST_CHECK(conversions::from_utf16<double>(_S("-123.123")) == double(-123.123));
	BOOST_CHECK(conversions::to_utf16<double>(-123.123) == _S("-123.123"));
}

BOOST_AUTO_TEST_CASE(test_misc_conversions)
{
	BOOST_CHECK(conversions::from_utf16<uint32>(_S("foo")) == uint32(0));
	BOOST_CHECK(conversions::from_utf16<uint32>(_S("01")) == uint32(1));
	BOOST_CHECK(conversions::from_utf16<uint32>(_S("001")) == uint32(1));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
