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
#include "portalsprofilescache.h"

#include "objectsuser.h"
#include "entitiessnapshotmanager.h"
#include "idesession.h"
#include "iportaldatabase.h"
#include "portalsportal.h"
#include "portalsprofile.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace portals {

//////////////////////////////////////////////////////////////////////

ProfilesCache::ProfilesCache(uint32 limit) : CacheBase(limit)
{

}

ProfilesCache::~ProfilesCache()
{

}

profile_ptr ProfilesCache::load(const ObjectID &id, IdeSession *session)
{
	/*
	shared_ptr<ObjectsUser> user = objects_user_cast(logged_user->getObject(id));
	if(user == nullptr)
		return nullptr;

	return profile_ptr(OS_NEW Profile(logged_user->getPortal(), logged_user->getDatabase(), logged_user->getSnapshotProfile(), user));
	*/
	OS_ASSERT(false);
	return nullptr;
}

profile_ptr ProfilesCache::myGet(shared_ptr<IPortalDatabase> database, const ObjectID &id)
{
	profile_ptr ptr = nullptr;
	if(exists(id))
	{
		ptr = find(id);
	}
	else
	{
		shared_ptr<ObjectsUser> user = objects_user_cast(database->getPortal()->getObject(database, id));
		if(user != nullptr)
		{
			ptr.reset(OS_NEW Profile(database->getPortal(), database, user));
		}
	}

	insert(id, ptr);
	return ptr;
}

//////////////////////////////////////////////////////////////////////

} // portals
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
