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
#include "istreamlayer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IStreamLayer::IStreamLayer()
{

}

IStreamLayer::~IStreamLayer()
{

}

uint64 IStreamLayer::available() const
{
	OS_ASSERT(size() >= position());
	return size() - position();	
}

bool IStreamLayer::seek(uint64 offset, SeekPosition from)
{
	if(m_next != nullptr)
		return m_next->seek(offset, from);

	return false;
}

uint64 IStreamLayer::position() const
{
	if(m_next != nullptr)
		return m_next->position();

	return 0;
}

uint64 IStreamLayer::size() const
{
	if(m_next != nullptr)
		return m_next->size();

	return 0;
}

bool IStreamLayer::close()
{
	if(m_next != nullptr && m_next->close() == false)
		return false;

	return true;
}

bool IStreamLayer::flush()
{
	if(m_next != nullptr && m_next->flush() == false)
		return false;

	return true;
}	

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
