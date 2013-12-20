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
#include "idepickerskin.h"

#include "boost/bind.hpp"
#include "ideskin.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "eventssource.h"
#include "htmlliteral.h"
#include "htmlparser.h"
#include "htmlscript.h"
#include "idesystem.h"
#include "idesession.h"
#include "ideskin.h"
#include "locked.h"
#include "objectsiobject.h"
#include "objectsirevisionable.h"

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

/*
const String IdeSkinPicker::EVENT_OPEN = _S("onOpen");
const String IdeObjectPicker::EVENT_SELECT = _S("onSelect");
*/

//////////////////////////////////////////////////////////////////////

IdePickerSkin::IdePickerSkin()
{
	setTemplateName(_S("picker_skin"));

	m_showSystem = false;
	/*
	getEvents()->get(EVENT_OPEN)->connect(boost::bind(&IdeObjectPicker::onOpen, this, _1));
	getEvents()->get(EVENT_SELECT)->connect(boost::bind(&IdeObjectPicker::onSelect, this, _1));
	*/

	//setID(_S("skinPicker"));
	//setCss(_S("os_skin_picker"));
}

IdePickerSkin::~IdePickerSkin()
{

}

void IdePickerSkin::onLoad()
{
	ControlBase::onLoad();

	// Enumera le skins disponibili
	Locked<const IdeSystem::Skins>::unique skins = IdeSystem::instance()->getAvailableSkins();
	// Verifica che ci sia almeno una skin
	if(skins->empty() == false)
	{
		if(getShowSystem())
			addSkin(nullptr);

		for(IdeSystem::Skins::const_iterator i = skins->begin(); i != skins->end(); ++i)
		{
			shared_ptr<IdeSkin> skin = i->second;
			if(skin != nullptr)
				addSkin(skin);				
		}
	}
}

void IdePickerSkin::addSkin(shared_ptr<IdeSkin> skin)
{
	// If skin == nullptr, system skin.

	String skinID = String::EMPTY;	
	String name = getPage()->getText(_S("skin.system.name"));
	String description = getPage()->getText(_S("skin.system.description"));
	if(skin != nullptr)
	{
		skinID = skin->getID().toUTF16();
		name = skin->getName();
		description = skin->getDescription();
	}
	/*
	String html = String::EMPTY;
	html += _S("<a id=\"") + skinID + _S("\" href=\"javascript:Osiris.selectorEffect('") + getTarget() + _S("','") + skinID + _S("','") + m_input->getTarget() + _S("');\"");					
	if(description != String::EMPTY)
		html += _S(" data-os-tooltip=\"") + getPage()->parseOml(description, false, false, true) + _S("\"");
	html += _S(">");
	*/
	String preview = getPage()->getSkin()->getImageUrl(_S("skin_undef.png"));

	if(skin == nullptr)
		preview = getPage()->getSkin()->getImageUrl(_S("skin_system.png"));
	else if(skin->hasPreview())
		preview = skin->getPreview();
	/*
	html += _S("<img src=\"" + HtmlParser::instance()->encode(preview) + "\">");
	html += _S("<br/>");
	html += getPage()->parseOml(name, false, false, true);
	html += _S("</a>");	
	*/

	

	//getControls()->add(shared_ptr<IHtmlControl>(OS_NEW HtmlLiteral(html)));

	shared_ptr<XMLNode> nodeSkin = m_nodeRoot->addChild(_S("skin"));
	nodeSkin->setAttributeString(_S("group"), getTarget());
	nodeSkin->setAttributeString(_S("id"), skinID);
	nodeSkin->setAttributeString(_S("key"), getKey(skinID));
	nodeSkin->setAttributeString(_S("href"), getKeyHref(skinID));
	nodeSkin->setAttributeString(_S("preview_href"), preview);
	nodeSkin->setAttributeString(_S("name"), name);
	nodeSkin->setAttributeString(_S("description"), description);
}

void IdePickerSkin::onPreRender()
{
	ControlBase::onPreRender();

	//getControls()->add(shared_ptr<IHtmlControl>(OS_NEW HtmlScript(_S("Osiris.selectorEffect('") + getTarget() + _S("','") + m_input->getValue() + _S("','") + m_input->getTarget() + _S("')"))));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
