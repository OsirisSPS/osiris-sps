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
#include "objectssystem.h"

#include "objectsdescriptor.h"
#include "objectsdescriptorclass.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SYSTEM(OS_NAMESPACE_NAME::ObjectsSystem)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

/*
ObjectsSystem::ObjectsSystem() : m_recursiveID("030200020000000000000000000000000000000000000001"),
								 m_orphanID("030200020000000000000000000000000000000000000002"),
								 m_skippedID("030200020000000000000000000000000000000000000003")
*/
ObjectsSystem::ObjectsSystem() : m_recursiveID("0000000000000000000000000000000000000001"),
								 m_orphanID("0000000000000000000000000000000000000002"),
								 m_skippedID("0000000000000000000000000000000000000003")
{
	
}

ObjectsSystem::~ObjectsSystem()
{
	
}

shared_ptr<ObjectsIDescriptor> ObjectsSystem::getDescriptor(PortalObjectType type) const
{
	if(type == portalObjectTypeUnknown)
		return nullptr;

	ObjectsDescriptors::const_iterator i = m_descriptors.find(type);
	OS_EXCEPT_IF(i == m_descriptors.end(), "Invalid descriptor");
	return i->second;
}

String ObjectsSystem::getTypeName(PortalObjectType type) const
{
	shared_ptr<ObjectsIDescriptor> descriptor = getDescriptor(type);
	if(descriptor != nullptr)
		return descriptor->getTypeName();
	else
		return OS_PORTAL_OBJECT_UNKNOWN_TYPENAME;	
}

String ObjectsSystem::getGroupName(PortalObjectType type) const
{
	return getDescriptor(type)->getGroupName();
}

bool ObjectsSystem::isVirtual(const EntityID & id) const
{
	return( (id == getRootID()) ||
		    (id == m_recursiveID) ||
		    (id == m_orphanID) ||
		    (id == m_skippedID) );		
}

ObjectID ObjectsSystem::getVirtualObjectID(const EntityID & id) const
{
	return "03020002" + id.getString();
}

shared_ptr<ObjectsIObject> ObjectsSystem::createObject(PortalObjectType type) const
{
	shared_ptr<ObjectsIDescriptor> descriptor = getDescriptor(type);
	return descriptor != nullptr ? descriptor->createObject() : nullptr;
}

void ObjectsSystem::fireNewObject(shared_ptr<ObjectsIObject> object)
{
	OS_ASSERT(object != nullptr);
	m_newObjectEvent(object);
}

String ObjectsSystem::getName() const
{
	return _S("objects");
}

bool ObjectsSystem::start(bool recovery)
{	
	ObjectsIDescriptorClass::descriptors_classes &classes = ObjectsIDescriptorClass::getClasses();
	for(ObjectsIDescriptorClass::descriptors_classes::iterator i = classes.begin(); i != classes.end(); ++i)
	{
		shared_ptr<ObjectsIDescriptor> descriptor = (*i)->create();
		OS_EXCEPT_IF(descriptor == nullptr, "Cannot create descriptor class");
		OS_EXCEPT_IF(m_descriptors.find(descriptor->getType()) != m_descriptors.end(), "Duplicated descriptor class");

		m_descriptors[descriptor->getType()] = descriptor;
	}

	return true;
}

void ObjectsSystem::stop()
{
	m_descriptors.clear();
}	

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
