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
#include "iextensionscomponent.h"

#include "xmlportalexporter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IExtensionsComponent::IExtensionsComponent()
{

}

IExtensionsComponent::~IExtensionsComponent()
{

}

void IExtensionsComponent::exportXML(const EntityID &id, shared_ptr<XMLPortalExporter> exporter)
{
	exporter->setAttributeString(_S("component_name"), getName());
}

void IExtensionsComponent::initXML(shared_ptr<XMLDocument> document)
{

}

void IExtensionsComponent::createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{

}

void IExtensionsComponent::computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{

}

void IExtensionsComponent::removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{

}

bool IExtensionsComponent::removeProfileData(shared_ptr<IPortalDatabase> db)
{
	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
