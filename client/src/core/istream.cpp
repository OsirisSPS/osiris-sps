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
#include "istream.h"

#include "boost/scope_exit.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const uint64 IStream::npos = static_cast<uint64>(-1);

//////////////////////////////////////////////////////////////////////

IStream::IStream() 
{

}

IStream::~IStream() 
{
	
}

uint64 IStream::available() const
{
	OS_ASSERT(size() >= position());
	return size() - position();
}

bool IStream::seekToBegin()
{
	return seek(0, seekBegin);
}

bool IStream::seekToEnd()
{
	return seek(0, seekEnd);
}

uint32 IStream::peek(void *v, uint32 size) const
{
	const IStream &stream = *this;
	uint64 offset = position();	

	BOOST_SCOPE_EXIT((&stream)(offset))
    {
        OS_EXCEPT_IF(stream.seek(offset, seekBegin) == false, "Stream seek error");
    } 
	BOOST_SCOPE_EXIT_END

	return read(v, size);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
