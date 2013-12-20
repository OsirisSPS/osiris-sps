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
#include "objectsobjectscache.h"

#include "iportaldatabase.h"
#include "objectssection.h"
#include "objectssystem.h"
#include "realtimestats.h"
#include "timerperformance.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ObjectsObjectsCache::ObjectsObjectsCache(uint32 limit) : CacheBase(limit)
{

}

ObjectsObjectsCache::~ObjectsObjectsCache()
{

}

shared_ptr<ObjectsIObject> ObjectsObjectsCache::load(const ObjectID &id, IPortalDatabase *db)
{
	if(id == ObjectsSection::getRootSection()->id)
		return ObjectsSection::getRootSection();
	else if(id == ObjectsSection::getRecursiveSection()->id)
		return ObjectsSection::getRecursiveSection();
	else if(id == ObjectsSection::getOrphanSection()->id)
		return ObjectsSection::getOrphanSection();
	else if(id == ObjectsSection::getSkippedSection()->id)
		return ObjectsSection::getSkippedSection();

	
	OS_ASSERT(db != nullptr);
	return db->loadObject(id);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
