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
#include "portalstransaction.h"

#include "boost/detail/atomic_count.hpp"
#include "iportaldatabase.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static boost::detail::atomic_count g_transactionsCounter(0);

//////////////////////////////////////////////////////////////////////

PortalsTransaction::PortalsTransaction(shared_ptr<Portal::DatabaseLockData> lockData, shared_ptr<IPortalDatabase> database) : m_lockData(lockData),
																															m_database(database),
																															m_id(static_cast<uint32>(g_transactionsCounter))
{
	++g_transactionsCounter;
}

PortalsTransaction::~PortalsTransaction()
{
	if(m_database->inTransaction())
	{
		OS_ASSERTFALSE(); // Rollback di una transazione, è voluto?
		m_database->rollback();
	}

	setExclusiveAccess(false);
}

void PortalsTransaction::setExclusiveAccess(bool exclusiveAccess)
{
	if(exclusiveAccess)
	{
		if(m_lock == nullptr)
		{
			m_lockData->addExclusiveRequest();
			m_lock.reset(OS_NEW_T(boost::recursive_mutex::scoped_lock)(m_lockData->getDatabaseExclusiveLock()));
		}
	}
	else
	{
		if(m_lock)
		{
			m_lockData->removeExclusiveRequest();
			m_lock.reset();
		}
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
