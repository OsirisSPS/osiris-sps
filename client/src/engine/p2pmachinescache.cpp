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
#include "p2pmachinescache.h"

#include "p2pmachine.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

MachinesCache::MachinesCache(uint32 limit) : CacheBase(limit)
{

}

MachinesCache::~MachinesCache()
{

}

shared_ptr<P2pMachine> MachinesCache::addMachine(const Buffer &id, const Buffer &publicKey)
{
	OS_LOCK(m_cs);

	String machineKey = id.toHex();

	shared_ptr<P2pMachine> machine = get(machineKey, 0);
	if(machine != null)
		return machine;

	machine.reset(OS_NEW P2pMachine());
	machine->setID(id);
	machine->setPublicKey(publicKey);
	
	if(machine->store() == false)
		return null;

	// Salva la macchina nella cache
	insert(machineKey, machine);
	return machine;
}

shared_ptr<P2pMachine> MachinesCache::load(const String &id, int32)
{
	shared_ptr<P2pMachine> machine(OS_NEW P2pMachine());
	if(machine->load(id))
		return machine;

	return null;
}

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
