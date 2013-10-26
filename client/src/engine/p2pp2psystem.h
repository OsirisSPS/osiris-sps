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

#ifndef _OS_P2P_P2PSYSTEM_H
#define _OS_P2P_P2PSYSTEM_H

#include "boost/thread/recursive_mutex.hpp"
#include "isystem.h"
#include "p2pp2p.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class CryptKeyAgreement;
class P2PServer;

//////////////////////////////////////////////////////////////////////

namespace p2p {

//////////////////////////////////////////////////////////////////////

class EngineExport P2PSystem : public DynamicSingleton<P2PSystem>,
							   public ISystem
{
// Construction
public:
	P2PSystem();
	virtual ~P2PSystem();

// Attributes
public:
	inline shared_ptr<P2PServer> getServer() const;

// Operations
public:
	Buffer generateMachineID(const Buffer &publicKey) const;
	bool validateMachineID(const Buffer &id, const Buffer &publicKey) const;

	shared_ptr<P2pMachine> addMachine(const Buffer &id, const Buffer &publicKey);
	bool isKnownMachine(const Buffer &id) const;
	shared_ptr<P2pMachine> getMachine(const Buffer &id) const;

	shared_ptr<CryptKeyAgreement> peekKey();

private:
	void updateServer();
	void stopServer();
	
	void updateOptions();

// ISystem interface
public:
	virtual String getName() const;

	virtual bool start(bool recovery);
	virtual void stop();

private:
	scoped_ptr<MachinesCache> m_machines;
	scoped_ptr<KeysCache> m_keys;
	shared_ptr<P2PServer> m_server;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<P2PServer> P2PSystem::getServer() const { return m_server; }

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_P2PSYSTEM_H
