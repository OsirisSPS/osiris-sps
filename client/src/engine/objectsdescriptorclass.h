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

#ifndef _ENGINE_OBJECTSDESCRIPTORCLASS_H
#define _ENGINE_OBJECTSDESCRIPTORCLASS_H

#include "objectsobjects.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsIDescriptorClass
{
public:
	typedef list<ObjectsIDescriptorClass *>::type descriptors_classes;

// Construction
public:
	ObjectsIDescriptorClass();
	virtual ~ObjectsIDescriptorClass();

// Attributes
public:
	static descriptors_classes & getClasses();

// Operations
private:
	static void _register(ObjectsIDescriptorClass *class_type);
	static void _unregister(ObjectsIDescriptorClass *class_type);

// Interface
public:
	virtual shared_ptr<ObjectsIDescriptor> create() = 0;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
class ObjectsDescriptorClass : public ObjectsIDescriptorClass
{
// Construction
public:
	ObjectsDescriptorClass();
	virtual ~ObjectsDescriptorClass();

// IDescriptorClass interface
public:
	virtual shared_ptr<ObjectsIDescriptor> create();
};

//////////////////////////////////////////////////////////////////////

template <typename T>
ObjectsDescriptorClass<T>::ObjectsDescriptorClass()
{

}

template <typename T>
ObjectsDescriptorClass<T>::~ObjectsDescriptorClass()
{

}

template <typename T>
shared_ptr<ObjectsIDescriptor> ObjectsDescriptorClass<T>::create()
{
	return shared_ptr<ObjectsIDescriptor>(T::create());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_OBJECTSDESCRIPTORCLASS_H
