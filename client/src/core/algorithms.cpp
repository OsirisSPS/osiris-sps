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
#include "algorithms.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((algorithms))

//////////////////////////////////////////////////////////////////////

template<>
CoreExport void to_lower<String>(String &v)
{
    v.to_lower();
}

template<>
CoreExport void to_upper<String>(String &v)
{
    v.to_upper();
}

template <>
CoreExport void trim_left<String>(String &v)
{
    v.trim_left();
}

template <>
CoreExport void trim_right<String>(String &v)
{
    v.trim_right();
}

template <>
CoreExport void trim<String>(String &v)
{
    v.trim();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((algorithms))

//////////////////////////////////////////////////////////////////////
