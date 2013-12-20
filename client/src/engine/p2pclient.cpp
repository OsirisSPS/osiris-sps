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
#include "p2pclient.h"

#include "engine.h"
#include "p2pserver.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

P2PClient::P2PClient(shared_ptr<IPAddress> address, shared_ptr<P2PServer> server, uint32 timeout, const String &origin) : ConnectionBase(server, Engine::instance()->createTCPSocket(server->getService(), true, false), timeout, true, origin),
																														  m_address(address)
{
	OS_ASSERT(address != nullptr);
}

P2PClient::~P2PClient()
{

}

void P2PClient::run()
{
	openConnection(m_address, createScope(boost::posix_time::milliseconds(m_timeout)));	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
