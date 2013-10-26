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

#ifndef _OS_ENGINE_PORTALSTRANSACTION_H
#define _OS_ENGINE_PORTALSTRANSACTION_H

#include "base/object.h"
#include "dbdb.h"
#include "lock.h"
#include "portalsportal.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport PortalsTransaction : public Object
{
public:
	PortalsTransaction(shared_ptr<Portal::DatabaseLockData> lockData, shared_ptr<IPortalDatabase> database);
	virtual ~PortalsTransaction();

// Attributes
public:
	inline uint32 getID() const;
	inline shared_ptr<IPortalDatabase> getDatabase() const;

// Operations
public:
	void setExclusiveAccess(bool exclusiveAccess);

private:
	uint32 m_id;
	shared_ptr<IPortalDatabase> m_database;
	shared_ptr<Portal::DatabaseLockData> m_lockData;
	scoped_ptr<boost::recursive_mutex::scoped_lock, os_delete_t> m_lock;
};

//////////////////////////////////////////////////////////////////////

inline uint32 PortalsTransaction::getID() const { return m_id; }
inline shared_ptr<IPortalDatabase> PortalsTransaction::getDatabase() const { return m_database; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_PORTALSTRANSACTION_H
