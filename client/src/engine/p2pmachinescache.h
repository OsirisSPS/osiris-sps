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

#ifndef _OS_P2P_MACHINESCACHE_H
#define _OS_P2P_MACHINESCACHE_H

#include "icache.h"
#include "options.h"
#include "p2pp2p.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;

//////////////////////////////////////////////////////////////////////

namespace p2p {

//////////////////////////////////////////////////////////////////////

class EngineExport MachinesCache : public ICache<String, P2pMachine, int32>
{
	typedef ICache<String, P2pMachine, int> CacheBase;

// Construction
public:
	MachinesCache(uint32 limit = OS_DEFAULT_CACHESIZE);
	virtual ~MachinesCache();

// Operations
public:
	shared_ptr<P2pMachine> addMachine(const Buffer &id, const Buffer &publicKey);

// ICache interface
protected:
	virtual shared_ptr<P2pMachine> load(const String &id, int32);
};

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_MACHINESCACHE_H
