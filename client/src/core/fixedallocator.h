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

#ifndef _OS_CORE_FIXEDALLOCATOR_H
#define _OS_CORE_FIXEDALLOCATOR_H

#include "base/object.h"
#include "boost/thread/mutex.hpp"
#include "pools.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport FixedAllocator : public Object
{
	typedef unordered_map<uint32, PoolAllocator<byte> *>::type Pools;

// Construction
public:
	FixedAllocator(uint32 size = 64, uint32 count = 6);	// Di default 6 pools: 64, 128, 256, 512, 1024, 2048
	virtual ~FixedAllocator();
	
// Operations
public:
	// Alloca la dimensione richiesta in un pool se disponibile, altrimenti tramite allocazione standard
	byte * allocate(uint32 size);
	// Come la "allocate" ma effettua la deallocazione automatica nella distruzione dello shared_ptr
	shared_ptr<byte> allocateShared(uint32 size);
	// Dealloca un buffer precedentemente allocato
	void deallocate(byte *data, uint32 size);

	// Rilascia eventuale memoria non utilizzata nei pools
	void releaseMemory();

private:
	uint32 roundSize(uint32 size) const;
	PoolAllocator<byte> * findPool(uint32 size) const;
	void clear();

private:
	boost::mutex m_mutex;
	uint32 m_size;
	Pools m_pools;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_FIXEDALLOCATOR_H
