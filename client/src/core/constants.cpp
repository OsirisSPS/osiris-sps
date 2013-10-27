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
#include "constants.h"

#include "boost/date_time/posix_time/posix_time.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((constants))

//////////////////////////////////////////////////////////////////////

const uint32 npos = (uint32) -1;
const uint32 nsize = (uint32) -1;

const bool bool_true = true;
const bool bool_false = false;

const int32 int32_zero = 0;
const uint32 uint32_zero = 0;
const double double_zero = 0;

const boost::posix_time::ptime time_null(boost::date_time::not_a_date_time);

const std::string empty_string;
const std::wstring empty_wstring;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((constants))

//////////////////////////////////////////////////////////////////////
