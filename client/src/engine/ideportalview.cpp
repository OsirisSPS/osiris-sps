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
#include "ideportalview.h"

#include "convert.h"
#include "conversions.h"
#include "entitiesentity.h"
#include "extensionscomponentviewer.h"
#include "extensionssystem.h"
#include "iextensionscomponent.h"
#include "ideblock.h"
#include "idehelpbox.h"
#include "idepathway.h"
#include "idesession.h"
#include "ideviewiobjectviewer.h"
#include "objectsdescriptor.h"
#include "objectsirevisionable.h"
#include "objectssystem.h"
#include "objectssection.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsoptionsshared.h"
#include "httprequest.h"
#include "htmlcontrols.h"
#include "htmlliteral.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

View::View(shared_ptr<HttpSession> session) : PageBase(session)
{

}

View::~View()
{

}

shared_ptr<IHtmlControl> View::createView(shared_ptr<EntitiesEntity> targetEntity) const
{
	if(targetEntity == null)
		return null;

	shared_ptr<IHtmlControl> viewer;

	EntityID targetID = targetEntity->getEntityID();
	
	//OS_ASSERT(targetID.empty() == false);
	
	// Se l'oggetto non  una sezione sale verso i padri alla ricerca della prima sezione che abbia un componente
	// e sia in grado di renderizzare l'oggetto di riferimento. Nel caso di una sezione segue la stessa logica
	// solo se quest'ultima non ha un componente associato

	bool checkComponent = true;
	bool root = targetID.empty();

	if( (root) || (targetEntity->getObjectType() == portalObjectTypeSection) )
	{
		shared_ptr<ObjectsSection> targetSection = objects_section_cast(targetEntity->getCurrent());
		if(targetSection != null)
		{
			shared_ptr<IExtensionsComponent> targetComponent = targetSection->getComponent(getPortal());
			checkComponent = (targetComponent == null);
			//checkComponent = targetSection->hasComponent() == false;
		}
	}

	if(checkComponent)
	{
		shared_ptr<EntitiesEntity> currentEntity = targetEntity->getParent(getDatabase());
		while(currentEntity != null)
		{
			if(currentEntity->getObjectType() == portalObjectTypeSection)
			{
				shared_ptr<ObjectsSection> section = objects_section_cast(currentEntity->getCurrent());
				if(section != null)
				{
					shared_ptr<IExtensionsComponent> sectionComponent = section->getComponent(getPortal());
					if(sectionComponent != null)
					{
						// Controlla se il componente corrente  in grado di renderizzare l'entit
						shared_ptr<ExtensionsComponentViewer> componentViewer = ExtensionsSystem::instance()->createComponentViewer(section->component, targetEntity);
						if(componentViewer != null)
						{
							componentViewer->init(ExtensionsSystem::instance()->getComponent(section->component), targetID, section->data);
							viewer = componentViewer;

							break;		// Se il componente  in grado di renderizzare l'oggetto interrompe il ciclo
						}
					}
				}
			}

			currentEntity = currentEntity->getParent(getDatabase());
		}
	}

	if(viewer == null)
	{
		shared_ptr<ObjectsIDescriptor> targetDescriptor = ObjectsSystem::instance()->getDescriptor(root ? portalObjectTypeSection : targetEntity->getObjectType());
		if(targetDescriptor != null)
			viewer = targetDescriptor->createViewControl(targetEntity);
	}

	return viewer;
}

String View::getPageName() const
{
	return _S("portal.pages.view");
}

void View::onPathway2() const
{
	return;
}

String View::getPagePathway() const
{
	return getPortal()->getLink("view");
}

EntityID View::getTargetObject() const
{
	return static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_ID)).to_ascii();
}

bool View::getShowInstance(shared_ptr<ObjectsInstance> instance) const
{
	// La pagina di view a differenza delle altre mostra le istanze in ogni circostanza se visibili
	return instance != null;
}

uint32 View::loadInstances(const EntityID &ID)
{
	uint32 instancesCount = PageBase::loadInstances(ID);
	// Controlla se l'oggetto target  la root e non  stata caricata alcuna istanza
	if(instancesCount == 0 && ID == ObjectsSystem::instance()->getRootID())
	{
		// Istanzia un pannello di benvenuto per evitare di renderizzare una pagina vuota

		shared_ptr<IdeBlock> block(OS_NEW IdeBlock(getText(_S("main.pages.view.welcome.title"))));
		//String html = _S("<div class=\"os_help\">")+parseBBCode(getText(_S("main.pages.view.welcome.content")))+_S("</div>");
		String text = getText(_S("main.pages.view.welcome.content"));
		//block->getBody()->getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(html)));
		block->getBody()->getControls()->add(shared_ptr<IdeHelpBox>(OS_NEW IdeHelpBox(text, String::EMPTY)));
		getArea(pageAreaContent)->getControls()->add(block);

	}

	return instancesCount;
}

void View::onLoad()
{
	PageBase::onLoad();

	EntityID targetID = getTargetObject();
	shared_ptr<IHtmlControl> viewer;

	shared_ptr<EntitiesEntity> targetEntity = getPortal()->getEntity(getDatabase(), targetID);

	if(targetEntity == null)
	{
		showError(getText(_S("ide.messages.unknown_object")));
	}
	else if (targetEntity->getCurrent() == null)
	{
		showError(getText(_S("ide.messages.invalid_object")));
	}
	else
	{
		// Inizializza il pathway dell'entit di riferimento
		getPathway()->add(getDatabase(), targetEntity, rmView);
		// Crea la vista dell'entità
		//if(targetID.empty() == false)
		viewer = createView(targetEntity);

		if(targetID == ObjectsSystem::instance()->getRootID())
		{
			// Home
			shared_ptr<PortalOptions> portalOptions = getPortal()->getOptions();

			//setPageTitle(portalOptions->getName());
			setPageTitle(String::EMPTY); // Only "getBaseTitle" used.
			setPageDescription(getPortal()->getOptionsShared()->getPortalDescription());
		}
		else
		{
			// Titolo
			setPageTitle(parseOml(targetEntity->getCurrent()->getTitle(), false, false, true));

			// Descrizione
			setPageDescription(parseOml(targetEntity->getCurrent()->getDescription(), false, false, true));
		}
	}

	if(viewer != null)
	{
		viewer->setID(_S("view"));
		getArea(pageAreaContent)->getControls()->add(viewer);
	}
}

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
