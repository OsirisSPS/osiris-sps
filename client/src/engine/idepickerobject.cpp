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
#include "idepickerobject.h"

#include "boost/bind.hpp"
#include "ideskin.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "eventssource.h"
#include "objectsiobject.h"
#include "objectsirevisionable.h"

#include "portalsportal.h"
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

const String IdePickerObject::EVENT_OPEN = _S("onOpen");
const String IdePickerObject::EVENT_SELECT = _S("onSelect");

//////////////////////////////////////////////////////////////////////

IdePickerObject::IdePickerObject()
{
	m_multipleSelection = false;

	getEvents()->get(EVENT_OPEN)->connect(boost::bind(&IdePickerObject::onOpen, this, _1));
	getEvents()->get(EVENT_SELECT)->connect(boost::bind(&IdePickerObject::onSelect, this, _1));
}

IdePickerObject::~IdePickerObject()
{

}

String IdePickerObject::getValue() const
{
	return String::EMPTY; 
}

void IdePickerObject::setValue(const String &id)
{
	//m_ids = id;
}

void IdePickerObject::onLoad()
{
	ControlBase::onLoad();


}

void IdePickerObject::onPreRender()
{
	ControlBase::onPreRender();

	shared_ptr<XMLDocument> document = getDocument();
	shared_ptr<XMLNode> root = document->create(_S("objectsPicker"));

	root->setAttributeBool(_S("multipleSelection"), m_multipleSelection);

	// I padri degli oggetti selezionati sono 'open' per forza.
	StringMap opens;
	for(StringMap::const_iterator i = m_selected.begin(); i != m_selected.end(); ++i)
	{
		shared_ptr<EntitiesEntity> objectEntity = getPage()->getPortal()->getEntity(getDatabase(), i->first.to_ascii());
		followParent(opens, objectEntity);
	}

	// Dopodich apro gli altri facoltativi
	for(StringMap::const_iterator i = m_opens.begin(); i != m_opens.end(); ++i)
	{
		opens[i->first] = true;
	}

	// Apro la root
	opens[ObjectsSystem::instance()->getRootID().toUTF16()] = true;

	exportObject(opens, getPage()->getPortal()->getEntity(getDatabase(), ObjectsSystem::instance()->getRootID()), root);
}

void IdePickerObject::followParent(StringMap& opens, shared_ptr<EntitiesEntity> entity)
{
	shared_ptr<EntitiesEntity> parent = entity->getParent(getDatabase());
	if(parent != null)
	{
		opens[parent->getEntityID().toUTF16()] = true;
		if(parent->getEntityID() != ObjectsSystem::instance()->getRootID())
			followParent(opens, parent);
	}
}

void IdePickerObject::exportObject(StringMap& opens, shared_ptr<EntitiesEntity> entity, shared_ptr<XMLNode> nodeParent)
{
	shared_ptr<XMLNode> node = nodeParent->addChild(_S("object"));
	shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(node, getPage(), XMLPortalExporter::emLite));
	shared_ptr<ObjectsIRevisionable> object = entity->getCurrent();
	if(object != null)
		object->exportXML(exporter);

	String id = entity->getEntityID().toUTF16();

	bool open = opens.find(id) != opens.end();
	bool selected = m_selected.find(id) != m_selected.end();

	shared_ptr<XMLNode> nodePicker = node->addChild(_S("picker"));

	nodePicker->setAttributeBool(_S("open"), open);
	nodePicker->setAttributeBool(_S("selected"), selected);
	nodePicker->setAttributeString(_S("open_href"), getEventCommand(EVENT_OPEN, id));
	nodePicker->setAttributeString(_S("select_href"), getEventCommand(EVENT_SELECT, id));

	if(open)
	{
		shared_ptr<XMLNode> nodeChilds = node->addChild(_S("childs"));

		// Scorre i figli e fa la stessa cosa.  URGENT
		shared_ptr<EntitiesEntities> childs = entity->getChilds(getDatabase(), portalObjectTypeUnknown);
		for(EntitiesEntities::iterator i = childs->begin(); i != childs->end(); ++i)
		{
			shared_ptr<EntitiesEntity> child = childs->get(getDatabase(), *i);

			exportObject(opens, child, nodeChilds);
		}
	}
}

void splitToStringMap(const String& str, StringMap& valueMap)
{
	StringVector tarray;
	utils::split(str, _S(";"), tarray);

	valueMap.clear();
	size_t nTokens = tarray.size();
	for(size_t iToken=0; iToken<nTokens; iToken++)
	{
		valueMap[tarray[iToken]] = true;
	}
}

String desplitFromStringMap(StringMap& valueMap)
{
	String out;
	for(StringMap::const_iterator i = valueMap.begin(); i != valueMap.end(); ++i)
	{
		if(!out.empty()) out += _S(";");
		out += i->first;
	}
	return out;
}

void IdePickerObject::onLoadViewState(const DataTree &state)
{
	ControlBase::onLoadViewState(state);

	String selectedStr = state.getV(_S("selected"), false);
	selectedStr.trim();
	splitToStringMap(selectedStr, m_selected);

	String openStr = state.getV(_S("open"), false);
	openStr.trim();
	splitToStringMap(openStr, m_opens);
}

void IdePickerObject::onSaveViewState(DataTree &state)
{
	ControlBase::onSaveViewState(state);

	String selectedStr = desplitFromStringMap(m_selected);
	state.setV(_S("selected"), selectedStr);

	String openStr = desplitFromStringMap(m_opens);
	state.setV(_S("open"), openStr);
}

String IdePickerObject::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("objects_picker.xsl"));
}

void IdePickerObject::onOpen(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	String id = htmlEvent->get(0);

	bool alreadyOpen = m_opens.find(id) != m_opens.end();
	if(alreadyOpen)
		m_opens.erase(id);
	else
		m_opens[id] = true;
}

void IdePickerObject::onSelect(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	String id = htmlEvent->get(0);

	bool alreadySelected = m_selected.find(id) != m_selected.end();
	if(alreadySelected)
		m_selected.erase(id);
	else
		m_selected[id] = true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
