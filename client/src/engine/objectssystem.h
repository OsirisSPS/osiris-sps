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

#ifndef _OBJECTS_OBJECTSSYSTEM_H
#define _OBJECTS_OBJECTSSYSTEM_H

#include "boost/signals.hpp"
#include "datadata.h"
#include "ids.h"
#include "isystem.h"
#include "objectsobjects.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsSystem : public DynamicSingleton<ObjectsSystem>,
								   public ISystem
{
	typedef boost::signal<void (shared_ptr<ObjectsIObject>)> NewObjectEvent;

// Construction
public:
	ObjectsSystem();
	virtual ~ObjectsSystem();

// Attributes
public:
	inline const EntityID & getRootID() const;
	inline const EntityID & getRecursiveID() const;
	inline const EntityID & getOrphanID() const;
	inline const EntityID & getSkippedID() const;

	inline const ObjectsDescriptors & getDescriptors() const;
	inline NewObjectEvent & getNewObjectEvent();

	shared_ptr<ObjectsIDescriptor> getDescriptor(PortalObjectType type) const;

	String getTypeName(PortalObjectType type) const;
	String getGroupName(PortalObjectType type) const;

// Operations
public:

	bool isVirtual(const EntityID & id) const;
	ObjectID getVirtualObjectID(const EntityID & id) const;

	virtual shared_ptr<ObjectsIObject> createObject(PortalObjectType type) const;
	virtual void fireNewObject(shared_ptr<ObjectsIObject> object);

// ISystem interface
public:
	virtual String getName() const;

	virtual bool start(bool recovery);
	virtual void stop();

private:
	const EntityID m_recursiveID;
	const EntityID m_orphanID;
	const EntityID m_skippedID;
	ObjectsDescriptors m_descriptors;
	NewObjectEvent m_newObjectEvent;
};

//////////////////////////////////////////////////////////////////////

inline const EntityID & ObjectsSystem::getRootID() const { return EntityID::EMPTY; }
inline const EntityID & ObjectsSystem::getRecursiveID() const { return m_recursiveID; }
inline const EntityID & ObjectsSystem::getOrphanID() const { return m_orphanID; }
inline const EntityID & ObjectsSystem::getSkippedID() const { return m_skippedID; }

//////////////////////////////////////////////////////////////////////

inline const ObjectsDescriptors & ObjectsSystem::getDescriptors() const { return m_descriptors; }
inline ObjectsSystem::NewObjectEvent & ObjectsSystem::getNewObjectEvent() { return m_newObjectEvent; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_OBJECTSSYSTEM_H
