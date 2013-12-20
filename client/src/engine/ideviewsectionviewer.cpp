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
#include "ideviewsectionviewer.h"

#include "timerperformance.h"
#include "convert.h"
#include "conversions.h"
#include "dbtables.h"
#include "entitiesentitychilds.h"
#include "entitiesentity.h"
#include "extensionscomponentviewer.h"
#include "extensionssystem.h"
#include "ideportaledit.h"
#include "ideskin.h"
#include "objectsinstance.h"
#include "objectssystem.h"
#include "objectssection.h"
#include "idesession.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace view {

//////////////////////////////////////////////////////////////////////

SectionViewer::SectionViewer(shared_ptr<EntitiesEntity> entity) : ControlBase(entity)
{

}

SectionViewer::~SectionViewer()
{

}

String SectionViewer::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("section_viewer.xsl"));
}

void SectionViewer::onLoad()
{	
	OS_TIMER_PERFORMANCE(TP, _S("SectionViewer::onLoad"));

	ControlBase::onLoad();

	shared_ptr<EntitiesEntity> entity = getEntity();
	if(entity == nullptr)
		return;

	shared_ptr<ObjectsSection> section = objects_section_cast(entity->getCurrent());
	if(section != nullptr)
	{
		ExtensionsComponentID component = section->getComponentID(getPortal());
		//shared_ptr<IExtensionsComponent> sectionComponent = section->getComponent(getPortal());
		if(component != ExtensionsComponentID::EMPTY)
		{
			shared_ptr<ExtensionsComponentViewer> componentViewer = ExtensionsSystem::instance()->createComponentViewer(component, nullptr);
			if(componentViewer != nullptr)
			{
				componentViewer->init(ExtensionsSystem::instance()->getComponent(component), entity->getEntityID(), section->data);
				//componentViewer->init(section->getPrimary(), section->data);
				getControls()->add(componentViewer);
			}	
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // view
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
