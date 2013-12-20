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
#include "fixedallocator.h"

#include "boost/bind.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

FixedAllocator::FixedAllocator(uint32 size, uint32 count)
{
	OS_ASSERT(size > 0);
	OS_ASSERT(size % 2 == 0);
	OS_ASSERT(count > 0);

	m_size = size;
	for(uint32 i = 0; i < count; i++)
	{
		uint32 pool = size << i;		
		OS_ASSERT(m_pools.find(pool) == m_pools.end());
		m_pools[pool] = OS_NEW_T(PoolAllocator<byte>)(pool);
	}
}

FixedAllocator::~FixedAllocator()
{
	clear();
}

byte * FixedAllocator::allocate(uint32 size)
{
	OS_ASSERT(size > 0);

	boost::mutex::scoped_lock lock(m_mutex);
	
	PoolAllocator<byte> *pool = findPool(roundSize(size));
	if(pool != nullptr)
		return pool->allocate();
	else
		return static_cast<byte *>(OS_ALLOCATE(size));
}

shared_ptr<byte> FixedAllocator::allocateShared(uint32 size)
{
	byte *data = allocate(size);
	if(data == nullptr)
		return nullptr;

	return shared_ptr<byte>(data, boost::bind(&FixedAllocator::deallocate, this, _1, size));
}

void FixedAllocator::deallocate(byte *data, uint32 size)
{
	OS_ASSERT(data != nullptr);
	OS_ASSERT(size > 0);

	boost::mutex::scoped_lock lock(m_mutex);

	PoolAllocator<byte> *pool = findPool(roundSize(size));
	if(pool != nullptr)
		pool->deallocate(data);
	else
		OS_DEALLOCATE(data);
}

void FixedAllocator::releaseMemory()
{
	boost::mutex::scoped_lock lock(m_mutex);

	for(Pools::const_iterator i = m_pools.begin(); i != m_pools.end(); i->second->release_memory(), ++i);
}

uint32 FixedAllocator::roundSize(uint32 size) const
{
	// N.B.: questa funzione deve essere loccata a monte

	register uint32 rounded = m_size;
	while(size > rounded)
	{
		rounded = rounded << 1;
	}

	return rounded;
}

PoolAllocator<byte> * FixedAllocator::findPool(uint32 size) const
{
	// N.B.: questa funzione deve essere loccata a monte

	Pools::const_iterator i = m_pools.find(size);
	if(i != m_pools.end())
		return i->second;

	return nullptr;
}

void FixedAllocator::clear()
{
	boost::mutex::scoped_lock lock(m_mutex);

	for(Pools::const_iterator i = m_pools.begin(); i != m_pools.end(); OS_DELETE_T(i->second), ++i);
	m_pools.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
