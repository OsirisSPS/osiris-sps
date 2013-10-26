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

#ifndef _OS_CORE_ICULAYER_H
#define _OS_CORE_ICULAYER_H

#include <stdio.h>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

typedef int32 icu_size;

//////////////////////////////////////////////////////////////////////

CoreExport std::ustring icu_asciitou(const achar *str, icu_size length = -1);
CoreExport std::ustring icu_asciitou(const std::string &str);
CoreExport std::string icu_utoascii(const std::ustring &str);
CoreExport std::ustring icu_utf8tou(const achar *str, icu_size length = -1);
CoreExport std::ustring icu_utf8tou(const std::string &str);
CoreExport std::string icu_utoutf8(const std::ustring &str);
CoreExport std::ustring icu_wtou(const wchar *str, icu_size length = -1);
CoreExport std::ustring icu_wtou(const std::wstring &str);
CoreExport std::wstring icu_utow(const std::ustring &str);

CoreExport bool icu_compare_no_case(const std::ustring &first, const std::ustring &second);
CoreExport std::ustring icu_to_lower(const std::ustring &str);
CoreExport std::ustring icu_to_upper(const std::ustring &str);
CoreExport std::ustring icu_format_string(const uchar *pattern, va_list args);
CoreExport icu_size icu_scanf(const uchar *str, const uchar *pattern, va_list args);

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ICULAYER_H
