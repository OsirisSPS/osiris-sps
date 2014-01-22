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
#include "netutils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((utils))

//////////////////////////////////////////////////////////////////////

uint16 networkToHost16(uint16 v)
{
	return ntohs(v);
}

uint16 hostToNetwork16(uint16 v)
{
	return htons(v);
}

uint32 networkToHost32(uint32 v)
{
	return ntohl(v);
}

uint32 hostToNetwork32(uint32 v)
{
	return htonl(v);
}

uint64 networkToHost64(uint64 v)
{
	union { uint32 v32[2]; uint64 v64; } u;
	u.v64 = v;

	return OS_MAKE_UINT64(ntohl(u.v32[1]), ntohl(u.v32[0]));
}

uint64 hostToNetwork64(uint64 v)
{
	union { uint32 v32[2]; uint64 v64; } u;
	u.v32[0] = htonl(OS_UINT64_HI(v));
	u.v32[1] = htonl(OS_UINT64_LO(v));

	return u.v64;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((utils))

//////////////////////////////////////////////////////////////////////
