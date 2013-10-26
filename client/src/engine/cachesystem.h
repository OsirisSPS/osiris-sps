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

#ifndef _ENGINE_CACHESYSTEM_H
#define _ENGINE_CACHESYSTEM_H

#include "boost/thread/recursive_mutex.hpp"
#include "dataitem.h"
#include "datetime.h"
#include "isystem.h"
#include "ordered_map.h"
#include "singleton.h"
#include "uniqueid.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport CacheSystem : public DynamicSingleton<CacheSystem>,
								 public ISystem
{
private:
	class Item : public Object
	{
	public:

		Item();
		~Item();

		DataItem item;
		uint32 size;
		DateTime expiration;
	};

	typedef unordered_map<String, shared_ptr<Item> >::type Items;

// Construction
public:
	CacheSystem();
	virtual ~CacheSystem();

// Attributes
public:
	
	
// Operations
public:	
	bool exists(const String& key);
	DataItem get(const String& key);
	bool remove(const String& key);
	void set(const String& key, const DataItem &value, uint32 expiration=0);
	
private:
	
	bool check(const String& key);
	
// ISystem interface
public:
	virtual String getName() const;

	virtual bool start(bool recovery);
	virtual void stop();

protected:
	Items m_items;
	uint32 m_currentSize;
	uint32 m_maxSize;

	mutable boost::recursive_mutex m_cs;	
};

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_CACHESYSTEM_H
