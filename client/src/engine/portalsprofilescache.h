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

#ifndef _PORTALS_PROFILESCACHE_H
#define _PORTALS_PROFILESCACHE_H

#include "icache.h"
#include "dbdb.h"
#include "entitiesentities.h"
#include "objectsobjects.h"

#include "options.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class IdeSession;
class ObjectID;

//////////////////////////////////////////////////////////////////////

namespace portals {

//////////////////////////////////////////////////////////////////////

class EngineExport ProfilesCache : public ICache<ObjectID, Profile, IdeSession *>
{
	typedef ICache<ObjectID, Profile, IdeSession *> CacheBase;

// Construction
public:
	ProfilesCache(uint32 limit = OS_DEFAULT_CACHESIZE);
	virtual ~ProfilesCache();

// Operations
public:
	profile_ptr myGet(shared_ptr<IPortalDatabase> database, const ObjectID &id);

// ICache interface
protected:
	virtual profile_ptr load(const ObjectID &id, IdeSession *session);
};

//////////////////////////////////////////////////////////////////////

} // portals
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _PORTALS_PROFILESCACHE_H
