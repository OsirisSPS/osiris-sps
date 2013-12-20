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
#include "extensionscomponentcontrol.h"

#include "objectsinstance.h"
#include "portalsportal.h"
#include "xmldocument.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ExtensionsComponentControl::ExtensionsComponentControl() : m_componentDocument(OS_NEW XMLDocument())
{

}

ExtensionsComponentControl::~ExtensionsComponentControl()
{

}

shared_ptr<EntitiesEntity> ExtensionsComponentControl::getEntity(const shared_ptr<IPortalDatabase> &database) const
{
	return getPortal()->getEntity(database, m_entityID);
}

bool ExtensionsComponentControl::init(shared_ptr<IExtensionsComponent> component, const EntityID &section, const String &xml)
{
	m_component = component;

	m_entityID = section;
	
	if(xml.empty() == false)
	{
		if(getComponentDocument()->parseString(xml) == false)
			return false;
	}

	if(component != nullptr)
		component->initXML(getComponentDocument());

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
