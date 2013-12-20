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
#include "connectionstatusguard.h"

#include "connectionstatus.h"
#include "iconnection.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ConnectionStatusGuard::ConnectionStatusGuard(shared_ptr<IConnection> connection, shared_ptr<ConnectionStatus> status) : m_connection(connection),
																														m_status(status)
{
	OS_ASSERT(connection != nullptr);
	OS_ASSERT(status != nullptr);
}

ConnectionStatusGuard::~ConnectionStatusGuard()
{
	shared_ptr<IConnection> connection = m_connection.lock();
	// Controlla se la connessione è ancora valida e se lo stato è modificato
	if(connection != nullptr && m_status->resetModifiedFlag())
		connection->fireStateChanged();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
