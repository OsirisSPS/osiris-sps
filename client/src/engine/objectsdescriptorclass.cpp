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
#include "objectsdescriptorclass.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ObjectsIDescriptorClass::ObjectsIDescriptorClass()
{
	_register(this);
}

ObjectsIDescriptorClass::~ObjectsIDescriptorClass()
{
	_unregister(this);
}

ObjectsIDescriptorClass::descriptors_classes & ObjectsIDescriptorClass::getClasses()
{
	static descriptors_classes classes;
	return classes;
}

void ObjectsIDescriptorClass::_register(ObjectsIDescriptorClass *class_type)
{
	OS_ASSERT(class_type != nullptr);

	getClasses().push_back(class_type);
}

void ObjectsIDescriptorClass::_unregister(ObjectsIDescriptorClass *class_type)
{
	OS_ASSERT(class_type != nullptr);

	descriptors_classes &classes = getClasses();

	descriptors_classes::iterator i = std::find(classes.begin(), classes.end(), class_type);
	if(i != classes.end())
		classes.erase(i);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
