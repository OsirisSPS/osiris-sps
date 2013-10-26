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

OS_NAMESPACE_OPEN((conversions))

//////////////////////////////////////////////////////////////////////

template<>
wxString from_string<wxString>(const std::string &v)
{
	wxString str(v.data(), v.size());
	return str;
}

template<>
std::string to_string<wxString>(const wxString &v)
{	
	return static_cast<std::string>(v);
}

template<>
wxString from_wstring<wxString>(const std::wstring &v)
{
	wxString str(v.data(), v.size());
	return str;
}

template<>
std::wstring to_wstring<wxString>(const wxString &v)
{	
	return static_cast<std::wstring>(v);
}

template<>
wxString from_utf16<wxString>(const String &v)
{
#ifdef OS_HAS_NATIVE_UCHAR
	return from_wstring<wxString>(v.str());
#else
	return from_wstring<wxString>(v.to_wide());	
#endif
}

template<>
String to_utf16<wxString>(const wxString &v)
{
	return static_cast<std::wstring>(v);	
}

//////////////////////////////////////////////////////////////////////
	
OS_NAMESPACE_CLOSE((conversions))

//////////////////////////////////////////////////////////////////////
