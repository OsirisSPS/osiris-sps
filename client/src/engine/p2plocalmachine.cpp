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
#include "p2plocalmachine.h"

#include "cryptmanager.h"
#include "engine.h"
#include "buffer.h"
#include "datatree.h"
#include "notificationsmanager.h"
#include "options.h"
#include "p2pp2psystem.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

const String LocalMachine::FILENAME = _S("machine.bin");

const String LocalMachine::ID = _S("id");
const String LocalMachine::PUBLIC_KEY = _S("public_key");
const String LocalMachine::PRIVATE_KEY = _S("private_key");

//////////////////////////////////////////////////////////////////////

LocalMachine::LocalMachine()
{
	m_initialized = false;
}

LocalMachine::~LocalMachine()
{

}

bool LocalMachine::init()
{
	if(m_initialized)
		return true;

	NotificationsManager::instance()->notify(_S("Initializing local machine..."));

	OS_ASSERT(m_id.empty());

	String filePath = utils::makeFilePath(Options::instance()->getDataPath(), FILENAME);

	DataTree dt;
	if(dt.load(filePath))
	{
		m_publicKey = dt.getV(PUBLIC_KEY);
		m_privateKey = dt.getV(PRIVATE_KEY);

		if(CryptManager::instance()->rsaCheckKeys(m_privateKey, m_publicKey))
		{
			// Carica l'id della macchina dopo aver verificato la validit delle chiavi
			m_id = dt.getV(ID);

			// Verifica che l'id sia stato archiviato correttamente
			if(validate() == false)
				m_id.clear();
		}
	}

	if(m_id.empty())
	{
		m_privateKey.clear();
		m_publicKey.clear();

		if(CryptManager::instance()->rsaGenerateKeys(rsaType4096, m_privateKey, m_publicKey) == false)
			return false;

		// L'ID della macchina  dato dall'hash della chiave pubblica
		m_id = P2PSystem::instance()->generateMachineID(m_publicKey);

		dt.setV(ID, m_id);
		dt.setV(PUBLIC_KEY, m_publicKey);
		dt.setV(PRIVATE_KEY, m_privateKey);

		if(dt.save(filePath) == false)
			return false;
	}

	m_initialized = true;

	return true;
}

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
