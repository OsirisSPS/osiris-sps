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

#include "buffer.h"
#include "dataitem.h"
#include "datatree.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_dataitem_construction)
{
	DataItem value;
	BOOST_CHECK(value.isNull());
	BOOST_CHECK(value.getType() == DataItem::vt_null);
	BOOST_CHECK(value.isDataTree() == false);
	BOOST_CHECK(value.get<bool>() == false);
	BOOST_CHECK(static_cast<bool>(value) == false);
	BOOST_CHECK(static_cast<bool *>(value) == nullptr);
	BOOST_CHECK(value.get<byte>() == 0);
	BOOST_CHECK(static_cast<byte>(value) == 0);
	BOOST_CHECK(static_cast<byte *>(value) == nullptr);
	BOOST_CHECK(value.get<int32>() == 0);
	BOOST_CHECK(static_cast<int32>(value) == 0);
	BOOST_CHECK(static_cast<int32 *>(value) == nullptr);
	BOOST_CHECK(value.get<uint32>() == 0);
	BOOST_CHECK(static_cast<uint32>(value) == 0);
	BOOST_CHECK(static_cast<uint32 *>(value) == nullptr);
	BOOST_CHECK(value.get<double>() == 0);
	BOOST_CHECK(static_cast<double>(value) == 0);
	BOOST_CHECK(static_cast<double *>(value) == nullptr);
	BOOST_CHECK(value.get<String>().empty());
	BOOST_CHECK(static_cast<String>(value).empty());
	BOOST_CHECK(static_cast<String *>(value) == nullptr);
	BOOST_CHECK(value.get<Buffer>().empty());
//	BOOST_CHECK(static_cast<Buffer>(value).empty());
//	BOOST_CHECK(static_cast<Buffer *>(value) == nullptr);
	BOOST_CHECK(value.get<DataTree>().empty());
	BOOST_CHECK(static_cast<DataTree>(value).empty());
	BOOST_CHECK(static_cast<DataTree *>(value) == nullptr);
	BOOST_CHECK(value.get<int64>() == 0);
	BOOST_CHECK(static_cast<int64>(value) == 0);
	BOOST_CHECK(static_cast<int64 *>(value) == nullptr);
	BOOST_CHECK(value.get<uint64>() == 0);
	BOOST_CHECK(static_cast<uint64>(value) == 0);
	BOOST_CHECK(static_cast<uint64 *>(value) == nullptr);
	BOOST_CHECK(value == DataItem::EMPTY);
	BOOST_CHECK(value.cast(DataItem::vt_null) == value);
}

template <typename T>
void test_dataitem_type(DataItem::ValueType type, const T &def, const T &val)
{
	DataItem value;
	BOOST_CHECK(value.get<T>() == def);
	BOOST_CHECK(static_cast<T *>(value) == nullptr);
	value = val;
	BOOST_CHECK(value.getType() == type);
	BOOST_CHECK(value.get<T>() == val);
	BOOST_CHECK(static_cast<T *>(value) != nullptr);

	Buffer buffer;
	BOOST_CHECK(value.write(buffer));

	buffer.seekToBegin();

	DataItem valueCheck;
	BOOST_CHECK(valueCheck.read(buffer));

	BOOST_CHECK(valueCheck.getType() == type);
	BOOST_CHECK(value == valueCheck);
	BOOST_CHECK(valueCheck.get<T>() == val);
	BOOST_CHECK(static_cast<T *>(valueCheck) != nullptr);

	value.clear();
	BOOST_CHECK(value.isNull());
	BOOST_CHECK(value.getType() == DataItem::vt_null);
	BOOST_CHECK(value == DataItem::EMPTY);
}

BOOST_AUTO_TEST_CASE(test_dataitem_types)
{
	test_dataitem_type<bool>(DataItem::vt_bool, false, true);
	test_dataitem_type<byte>(DataItem::vt_byte, 0, 255);
	test_dataitem_type<int32>(DataItem::vt_int32, 0, -1200);
	test_dataitem_type<uint32>(DataItem::vt_uint32, 0, 15000);
	test_dataitem_type<double>(DataItem::vt_double, 0, 15.269);
	test_dataitem_type<String>(DataItem::vt_string, String::EMPTY, String("Foo"));
	Buffer buffer;
	buffer.write<bool>(true);
	buffer.write<byte>(255);
	buffer.write<int32>(-1200);
	buffer.write<uint32>(15000);
	buffer.write<double>(15.269);
	test_dataitem_type<Buffer>(DataItem::vt_buffer, Buffer::EMPTY, buffer);
	DataTree datatree;
	datatree.setT("alfa")->setV("value", true);
	datatree.setT("beta")->setV("value", false);
	test_dataitem_type<DataTree>(DataItem::vt_datatree, DataTree::EMPTY, datatree);
	test_dataitem_type<int64>(DataItem::vt_int64, 0, -15000);
	test_dataitem_type<uint64>(DataItem::vt_uint64, 0, 188888888);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
