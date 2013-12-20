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
#include "idepickercomponent.h"

#include "boost/bind.hpp"
#include "ideskin.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "eventssource.h"
#include "htmlliteral.h"
#include "htmlparser.h"
#include "htmlscript.h"
#include "extensionssystem.h"
#include "idesystem.h"
#include "ideskin.h"
#include "iextensionscomponent.h"
#include "locked.h"
#include "objectsiobject.h"
#include "objectsirevisionable.h"

#include "idesession.h"
#include "htmlevent.h"
#include "htmlcontrols.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "objectssystem.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

IdePickerComponent::IdePickerComponent()
{
	setTemplateName(_S("picker_component"));
		
	//setCss(_S("os_skin_picker"));
}

IdePickerComponent::~IdePickerComponent()
{

}

void IdePickerComponent::onLoad()
{
	ControlBase::onLoad();
		
	addComponent(nullptr);
	const ExtensionsSystem::Components &components = ExtensionsSystem::instance()->getComponents();
	for(ExtensionsSystem::Components::const_iterator i = components.begin(); i != components.end(); ++i)
	{
		shared_ptr<IExtensionsComponent> component = i->second;
		addComponent(component);
		
	}
}

void IdePickerComponent::addComponent(shared_ptr<IExtensionsComponent> component)
{
	// If skin == nullptr, system skin.

	String id;
	String name;
	String description;
	if(component == nullptr)
	{
		name = _S("section.components.none");
	}
	else
	{
		id = component->getID().toUTF16();
		name = component->getName();		
	}	
	
	shared_ptr<XMLNode> nodeSkin = m_nodeRoot->addChild(_S("component"));
	nodeSkin->setAttributeString(_S("group"), getTarget());
	nodeSkin->setAttributeString(_S("id"), id);
	nodeSkin->setAttributeString(_S("key"), getKey(id));
	nodeSkin->setAttributeString(_S("href"), getKeyHref(id));
	nodeSkin->setAttributeString(_S("name"), name);	
}

void IdePickerComponent::onPreRender()
{
	ControlBase::onPreRender();

	//getControls()->add(shared_ptr<IHtmlControl>(OS_NEW HtmlScript(_S("Osiris.selectorEffect('") + getTarget() + _S("','") + m_input->getValue() + _S("','") + m_input->getTarget() + _S("')"))));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
