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

#ifndef _OS_CORE_CONSTANTS_H
#define _OS_CORE_CONSTANTS_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((constants))

//////////////////////////////////////////////////////////////////////

extern CoreExport const uint32 npos;
extern CoreExport const uint32 nsize;

extern CoreExport const bool bool_true;
extern CoreExport const bool bool_false;

extern CoreExport const int32 int32_zero;
extern CoreExport const uint32 uint32_zero;
extern CoreExport const double double_zero;

extern CoreExport const boost::posix_time::ptime time_null;

extern CoreExport const std::string empty_string;
extern CoreExport const std::wstring empty_wstring;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((constants))

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_CONSTANTS_H
