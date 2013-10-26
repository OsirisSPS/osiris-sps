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
#include "cachesystem.h"

#include "archivesmanager.h"
#include "cryptmanager.h"
#include "engine.h"
#include "log.h"
#include "buffer.h"
#include "filesystem.h"
#include "path.h"
#include "options.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SYSTEM(OS_NAMESPACE_NAME::CacheSystem)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

CacheSystem::Item::Item()
{
}

CacheSystem::Item::~Item()
{
}

//////////////////////////////////////////////////////////////////////

CacheSystem::CacheSystem()
{
	
}

CacheSystem::~CacheSystem()
{
}

String CacheSystem::getName() const
{
	return _S("cache");
}

bool CacheSystem::start(bool recovery)
{
	return true;
}

void CacheSystem::stop()
{	
}

bool CacheSystem::check(const String& key)
{
	OS_LOCK(m_cs);

	shared_ptr<Item> item = m_items[key];
	if(item == null)
		return false;

	if(item->expiration != DateTime::EMPTY)
	{
		if(item->expiration <= DateTime::now())
		{
			remove(key);
			return false;			
		}
	}

	return true;
}

bool CacheSystem::exists(const String& key)
{
	return check(key);
}

DataItem CacheSystem::get(const String& key)
{
	OS_LOCK(m_cs);

	if(check(key) == false)
		return DataItem();

	shared_ptr<Item> item = m_items[key];
	if(item != null)
		return item->item;
	else
	{
		OS_ASSERT(false);
		return DataItem();
	}
}

bool CacheSystem::remove(const String& key)
{
	OS_LOCK(m_cs);

	shared_ptr<Item> item = m_items[key];
	if(item != null)
	{
		m_currentSize -= item->size;
		m_items.erase(key);
		return true;
	}
	else
		return false;
}

void CacheSystem::set(const String& key, const DataItem &value, uint32 expiration)
{
	OS_LOCK(m_cs);

	Buffer buff;
	value.write(buff);
	m_currentSize += buff.getSize();

	shared_ptr<Item> item(OS_NEW CacheSystem::Item());
	item->item = value;
	item->size = buff.getSize();
	if(expiration == 0)
		item->expiration == DateTime::EMPTY;
	else
		item->expiration = DateTime::now() + TimeDuration(0, 0, expiration);
	m_items[key] = item;

	// TODO: Clean logic still missing.	
	// Will be better to add items to top of a list, and browse from bottom for removing.

	if(m_currentSize > m_maxSize)
	{
		// TODO
		// First lap, remove items expired
		
		if(m_currentSize > m_maxSize)
		{
			// Second lap, remove until reach size.
		}
	}


}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
