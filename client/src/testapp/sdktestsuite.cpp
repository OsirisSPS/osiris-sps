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
#include "options.h"
#include "sdk.h"
#include "testapp.h"
#include <sstream>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_sdk_operations)
{
	std::string rootPath = TestApp::instance()->getRootPath().to_utf8();

	BOOST_CHECK(osiris_cleanup() == OS_SDK_ERROR_NOT_INITIALIZED);
	BOOST_CHECK(osiris_start() == OS_SDK_ERROR_NOT_INITIALIZED);
	BOOST_CHECK(osiris_stop() == OS_SDK_ERROR_NOT_INITIALIZED);

	os_uint32 optionSize = 0;
	os_uint32 optionType = OS_SDK_OPTION_TYPE_UNKNOWN;
	os_uint8 true_value = 1;
	os_uint8 false_value = 0;
	os_int8 value_int8 = 0;
	os_uint8 value_uint8 = 0;
	os_int32 value_int32 = 0;
	os_uint32 value_uint32 = 0;
	Buffer value_string;

	BOOST_CHECK(osiris_options_get(Options::log_options::enable.to_ascii().c_str(), nullptr, &optionSize, nullptr) == OS_SDK_ERROR_NOT_INITIALIZED);
	BOOST_CHECK(osiris_options_set(Options::log_options::enable.to_ascii().c_str(), &true_value, optionSize) == OS_SDK_ERROR_NOT_INITIALIZED);

	BOOST_CHECK(osiris_init(0, nullptr, rootPath.c_str()) == OS_SDK_OK);
	BOOST_CHECK(rootPath == Options::instance()->getRootPath().to_utf8());
	BOOST_CHECK(osiris_init(0, nullptr, rootPath.c_str()) == OS_SDK_ERROR_ALREADY_INITIALIZED);

	optionSize = 0;
	BOOST_CHECK(osiris_options_get(Options::log_options::enable.to_ascii().c_str(), nullptr, &optionSize, nullptr) == OS_SDK_OK);
	BOOST_CHECK(optionSize == sizeof(os_uint8));

	optionSize = 0;
	optionType = OS_SDK_OPTION_TYPE_UNKNOWN;
	BOOST_CHECK(osiris_options_get(Options::log_options::enable.to_ascii().c_str(), nullptr, &optionSize, &optionType) == OS_SDK_OK);
	BOOST_CHECK(optionSize == sizeof(os_uint8));
	BOOST_CHECK(optionType == OS_SDK_OPTION_TYPE_UINT);

	value_uint8 = 0;
	optionSize = sizeof(os_uint8);
	optionType = OS_SDK_OPTION_TYPE_UNKNOWN;
	BOOST_CHECK(osiris_options_get(Options::log_options::enable.to_ascii().c_str(), &value_uint8, &optionSize, &optionType) == OS_SDK_OK);
	BOOST_CHECK(value_uint8 == (Options::instance()->getOption<bool>(Options::log_options::enable) ? 1 : 0));
	BOOST_CHECK(optionSize == sizeof(os_uint8));
	BOOST_CHECK(optionType == OS_SDK_OPTION_TYPE_UINT);

	optionSize = sizeof(os_uint8);
	BOOST_CHECK(osiris_options_set(Options::log_options::enable.to_ascii().c_str(), &true_value, optionSize) == OS_SDK_OK);
	BOOST_CHECK(osiris_options_set(Options::log_options::enable.to_ascii().c_str(), &false_value, optionSize) == OS_SDK_OK);
	BOOST_CHECK(osiris_options_set(Options::log_options::enable.to_ascii().c_str(), &true_value, sizeof(uint32)) == OS_SDK_ERROR_INVALID_PARAM);

	value_uint8 = 0;
	optionSize = sizeof(os_uint8);
	optionType = OS_SDK_OPTION_TYPE_UNKNOWN;
	BOOST_CHECK(osiris_options_get(Options::log_options::enable.to_ascii().c_str(), &value_uint8, &optionSize, &optionType) == OS_SDK_OK);
	BOOST_CHECK(value_uint8 == 0);
	BOOST_CHECK(optionSize == sizeof(os_uint8));
	BOOST_CHECK(optionType == OS_SDK_OPTION_TYPE_UINT);

	BOOST_CHECK(osiris_options_get(Options::log_options::level.to_ascii().c_str(), nullptr, &optionSize, &optionType) == OS_SDK_OK);
	BOOST_CHECK(optionSize == sizeof(os_uint32));
	BOOST_CHECK(optionType == OS_SDK_OPTION_TYPE_UINT);

	value_uint32 = 0;
	optionSize = sizeof(os_uint32);
	BOOST_CHECK(osiris_options_get(Options::log_options::level.to_ascii().c_str(), &value_uint32, &optionSize, nullptr) == OS_SDK_OK);
	BOOST_CHECK(optionSize == sizeof(os_uint32));
	BOOST_CHECK(value_uint32 == Options::instance()->getOption<uint32>(Options::log_options::level));

	value_int8 = 0;
	optionSize = sizeof(os_int8);
	BOOST_CHECK(osiris_options_get(Options::language_options::time_offset.to_ascii().c_str(), &value_int8, &optionSize, nullptr) == OS_SDK_ERROR_BUFFER_TOO_SMALL);
	BOOST_CHECK(optionSize == sizeof(os_int32));

	value_int32 = 0;
	optionSize = sizeof(value_int32);
	optionType = OS_SDK_OPTION_TYPE_UNKNOWN;
	BOOST_CHECK(osiris_options_get(Options::language_options::time_offset.to_ascii().c_str(), &value_int32, &optionSize, &optionType) == OS_SDK_OK);
	BOOST_CHECK(value_int32 == Options::instance()->getOption<int32>(Options::language_options::time_offset));
	BOOST_CHECK(optionSize == sizeof(os_int32));
	BOOST_CHECK(optionType == OS_SDK_OPTION_TYPE_INT);

	optionSize = 0;
	optionType = OS_SDK_OPTION_TYPE_UNKNOWN;
	BOOST_CHECK(osiris_options_get(Options::db_options::default_driver.to_ascii().c_str(), nullptr, &optionSize, &optionType) == OS_SDK_OK);
	BOOST_CHECK(optionSize == Options::instance()->getOption<String>(Options::db_options::default_driver).to_utf8().size());
	BOOST_CHECK(optionType == OS_SDK_OPTION_TYPE_STRING);

    value_string.clear();
	BOOST_CHECK(value_string.reserve(optionSize));
	BOOST_CHECK(osiris_options_get(Options::db_options::default_driver.to_ascii().c_str(), value_string.getData(), &optionSize, nullptr) == OS_SDK_OK);
	BOOST_CHECK(optionSize == value_string.getSize());
	BOOST_CHECK(std::string(reinterpret_cast<std::string::const_pointer>(value_string.getData()), static_cast<std::string::size_type>(value_string.getSize())) == Options::instance()->getOption<String>(Options::db_options::default_driver).to_utf8());

	optionSize = 0;
	optionType = OS_SDK_OPTION_TYPE_UNKNOWN;
	BOOST_CHECK(osiris_options_get(Options::net_options::proxy_host.to_ascii().c_str(), nullptr, &optionSize, &optionType) == OS_SDK_OK);
	BOOST_CHECK(optionSize == Options::instance()->getOption<String>(Options::net_options::proxy_host).to_utf8().size());
	BOOST_CHECK(optionType == OS_SDK_OPTION_TYPE_STRING);

	value_string.clear();
	BOOST_CHECK(value_string.reserve(optionSize));
	BOOST_CHECK(osiris_options_get(Options::net_options::proxy_host.to_ascii().c_str(), value_string.getData(), &optionSize, nullptr) == OS_SDK_OK);
	BOOST_CHECK(optionSize == value_string.getSize());
	BOOST_CHECK(std::string(reinterpret_cast<std::string::const_pointer>(value_string.getData()), static_cast<std::string::size_type>(value_string.getSize())) == Options::instance()->getOption<String>(Options::net_options::proxy_host).to_utf8());

	value_uint32 = 0;
	optionSize = sizeof(os_uint32);
	BOOST_CHECK(OS_SDK_SUCCEEDED(osiris_options_get(Options::web_options::server_port.to_ascii().c_str(), &value_uint32, &optionSize, NULL)));

	BOOST_CHECK(osiris_cleanup() == OS_SDK_OK);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
