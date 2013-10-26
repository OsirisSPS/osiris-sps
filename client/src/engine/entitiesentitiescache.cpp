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
#include "entitiesentitiescache.h"

#include "objectsirevisionable.h"
#include "entitiesentity.h"
#include "entitiessnapshotmanager.h"
#include "iportaldatabase.h"
#include "portalsportal.h"
#include "realtimestats.h"
#include "timerperformance.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

EntitiesEntitiesCache::EntitiesEntitiesCache(uint32 limit) : CacheBase(limit)
{

}

EntitiesEntitiesCache::~EntitiesEntitiesCache()
{

}

shared_ptr<EntitiesEntity> EntitiesEntitiesCache::load(const EntityID &id, IPortalDatabase *db)
{
	// URGENT
	/*
	OS_ASSERT(snapshotProfile != null);

	shared_ptr<ObjectsIRevisionable> object = objects_revisionable_cast(snapshotProfile->getPortal()->getObject(id));
	if(object == null)
		return null;

	// Se l'id specificato  di una revisione, deve cmq essere trattato come se l'entit non esistesse.
	// Questo perch concettualmente il fatto che l'ID di un'entit sia l'ID della entry primaria  solo una comodit nostra.
	if(object->isPrimary() == false)
		return null;

	return entity_ptr(OS_NEW Entity(snapshotProfile->get_this_ptr(), object));
	*/
	OS_ASSERT(false);
	return null;
}

shared_ptr<EntitiesEntity> EntitiesEntitiesCache::myGet(const shared_ptr<IPortalDatabase> database, const EntityID &id)
{
	/*
	entity_ptr ptr = find(id);
	if(ptr == null)
	{
		//ptr = load(key, source);
		shared_ptr<ObjectsIRevisionable> object = objects_revisionable_cast(profile->getPortal()->getObject(id, database));
		if(object == null)
			return null;

		// Se l'id specificato  di una revisione, deve cmq essere trattato come se l'entit non esistesse.
		// Questo perch concettualmente il fatto che l'ID di un'entit sia l'ID della entry primaria  solo una comodit nostra.
		if(object->isPrimary() == false)
			return null;

		ptr.reset(OS_NEW Entity(database, profile, object));
	}
	if(ptr != null)
		insert(id, ptr);
	return ptr;
	*/

	shared_ptr<EntitiesEntity> ptr = null;
	if(exists(id))
	{
		ptr = find(id);
	}
	else
	{
		ptr.reset(OS_NEW EntitiesEntity());
		if(ptr->load(database, id) == false)
			ptr = null;
		/*
		shared_ptr<ObjectsIRevisionable> object = objects_revisionable_cast(database->getPortal()->getObject(database, id.toUTF16()));
		if(object != null)
		{
			OS_ASSERT(object->isEntity());

			// Se l'id specificato  di una revisione, deve cmq essere trattato come se l'entit non esistesse.
			// Questo perch concettualmente il fatto che l'ID di un'entit sia l'ID della entry primaria  solo una comodit nostra.
			ptr.reset(OS_NEW EntitiesEntity());
			if(ptr->load(database, object) == false)
				ptr = null;			
		}
		*/
	}

	insert(id, ptr);
	return ptr;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
