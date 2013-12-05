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

#ifndef _COMPATIBILITY_MANAGER_H
#define _COMPATIBILITY_MANAGER_H

//////////////////////////////////////////////////////////////////////

#include "ideide.h"
#include "portalsportals.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class IdeSession;

//////////////////////////////////////////////////////////////////////

class EngineExport CompatibilityManager : public StaticSingleton<CompatibilityManager>
{
// Construction
public:
	CompatibilityManager();
	virtual ~CompatibilityManager();

// Operations
public:
	bool razorUpgrade(const String &folder);

	bool razorPortalDatabaseUpgrade(const shared_ptr<IPortalDatabase> &database);

	void resigner(const shared_ptr<IPortalDatabase> &db, shared_ptr<IdeSession> session);

protected:
	void migrateAccounts(const shared_ptr<IPortalDatabase> &database);
	bool upgradeContentOML(const shared_ptr<IPortalDatabase> &database, const String &tableName, const String& fieldName, bool xmlEncoded);
	void renameIDS(const shared_ptr<IPortalDatabase> &database);

public:
	String m_DebugLog;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _COMPATIBILITY_MANAGER_H
