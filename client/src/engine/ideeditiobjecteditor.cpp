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
#include "ideeditiobjecteditor.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "constants.h"
#include "conversions.h"
#include "entitiesentity.h"
#include "iportaldatabase.h"
#include "engine.h"
#include "eventssource.h"
#include "ideomleditor.h"
#include "ideblock.h"
#include "idebutton.h"
#include "idepositioneditor.h"
#include "idesession.h"
#include "isissystem.h"
#include "objectsdescriptor.h"
#include "objectsirevisionable.h"
#include "objectssystem.h"
#include "portalsportal.h"
#include "htmlevent.h"
#include "css.h"
#include "htmlcheckbox.h"
#include "htmlcontrols.h"
#include "htmllink.h"
#include "htmlliteral.h"
#include "htmldiv.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltablerow.h"
#include "htmltext.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

const String IObjectEditor::EVENT_ONCREATEOBJECT = _S("onCreateObject");

//////////////////////////////////////////////////////////////////////

IObjectEditor::IObjectEditor(PortalObjectType type, shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) : m_enableComment(OS_NEW HtmlCheckBox()),
																												   m_comment(OS_NEW IdeOMLEditor()),
																												   m_position(OS_NEW IdePositionEditor())
{
	OS_ASSERT(type != portalObjectTypeUnknown);

	OS_EXCEPT_IF(entity != nullptr && entity->getObjectType() != type, "Invalid object type");
	m_entity = entity;

	OS_EXCEPT_IF(parent != nullptr && parent->allowChild(type) == false, "Invalid child");
	m_parent = parent;

	OS_ASSERT(IPortalDatabase::isRevisionable(type));
	m_type = type;

	m_redirectionType = rtEdit;

	m_enableComment->setID(_S("enableComment"));
	m_enableComment->setCheck(false);
	m_enableComment->setAutoPostBack(true);
	m_comment->setID(_S("comment"));

	m_position->setID(_S("position"));

	getTemplate()->addChildParam(m_enableComment, _S("enable_comment"));
	getTemplate()->addChildParam(m_comment, _S("comment"));

	// URGENT Nel componente documentazione, in alcuni casi ci vuole il positioneditor per gli oggetti testo.
	// Non dovrebbe essere a livello di 'descriptor'.
	// Certo è che ci vorrebbe un pager.. ma poi sta bene che ci sia nel "topiceditor"?
	// 14/11/2008: Aggiunto sempre il positioneditor.
	// 10/01/2008: Rimesso come prima.
	//getTemplate()->addChildParam(m_position, _S("position"));

#ifdef OS_DEBUG
	getTemplate()->addChildParam(m_position, _S("position"));
#else
	// Se l'ordinamento dell'oggetto  per posizione aggiunge il position editor
	if(ObjectsSystem::instance()->getDescriptor(m_type)->getOrderMode() == ObjectsIDescriptor::omPosition)
		getTemplate()->addChildParam(m_position, _S("position"));
#endif

	getEvents()->get(EVENT_ONCREATEOBJECT)->connect(boost::bind(&IObjectEditor::onCreateObject, this));
}

IObjectEditor::~IObjectEditor()
{

}

const std::wstring & IObjectEditor::getRedirectionParam(const std::wstring &name) const
{ 
	ordered_map<std::wstring, std::wstring>::const_iterator i = m_redirectionParams.find(name);

	if(i != m_redirectionParams.end())
		return i->second;

	return constants::empty_wstring;
}

#ifdef OS_TODOCIP
const EntityID & IObjectEditor::getParentID() const
#else
EntityID IObjectEditor::getParentID() const
#endif
{
	if(m_parent != nullptr)
		return m_parent->getEntityID();

	if(m_entity != nullptr)
		return m_entity->getParentID();

	return EntityID::EMPTY;
}

#ifdef OS_TODOCIP
const EntityID & IObjectEditor::getEntityID() const
#else
EntityID IObjectEditor::getEntityID() const
#endif
{
	/*
	if( (m_entity != nullptr) && (m_entity->getPrimary() != nullptr) )
		return m_entity->getPrimary()->id;
	*/
	if(m_entity != nullptr) 
		return m_entity->getEntityID();

	return EntityID::EMPTY;
}

void IObjectEditor::onCreateObject()
{
	shared_ptr<ObjectsIRevisionable> object = createObject();
	if(object == nullptr)
	{
		getPage()->showError(getPage()->getText(_S("object.editor.error")));
		return;
	}

	object->author = getSessionAccount()->getUserID();
	object->parent = getParentID();
		
	//object->revision = getPrimaryID();
	if(getEntityID().empty() == false)
	{
		OS_ASSERT(getCurrent());
		object->changeToRevision(getCurrent());
	}
	

	const String &comment = m_comment->getValue();
	if(comment.empty() == false)
		object->comment = algorithms::trim_copy(comment);

	double newPosition = m_position->getPosition();
	if(newPosition != 0)
		object->position = newPosition;

	LanguageResult result = object->create(getDatabase(), getSessionAccount());

	if(result.empty() == false)
	{
		getPage()->showError(getPage()->getText(result));
		return;
	}

	// Effettua il redirect all'oggetto
	getPage()->redirect(getRedirectLocation(object).to_ascii());
}

String IObjectEditor::getRedirectLocation(shared_ptr<ObjectsIRevisionable> object) const
{
	String url;
	switch(m_redirectionType)
	{
	case rtEdit:			url = getPortal()->getEditObjectLink(object->getEntityID(), m_redirectionParams);
							break;

	case rtEditParent:		url = getPortal()->getEditObjectLink(object->getParent(), m_redirectionParams);
							break;

	case rtView:			url = getPortal()->getViewObjectLink(object->getEntityID(), m_redirectionParams);
							break;

	case rtViewParent:		url = getPortal()->getViewObjectLink(object->getParent(), m_redirectionParams);
							break;

	default:				OS_ASSERTFALSE();
							break;
	}

	return url;
}

void IObjectEditor::renderActions(shared_ptr<XMLNode> nodeActions)
{
	shared_ptr<XMLNode> actionCreate = nodeActions->addChild(_S("action"));
	actionCreate->setAttributeString(_S("name"), _S("create"));
	actionCreate->setAttributeString(_S("href"), getEventCommand(EVENT_ONCREATEOBJECT));

	shared_ptr<XMLNode> actionCancel = nodeActions->addChild(_S("action"));
	actionCancel->setAttributeString(_S("name"), _S("cancel"));
	actionCancel->setAttributeString(_S("href"), _S("javascript:void(history.go(-1));"));
}

void IObjectEditor::onInit()
{
	ControlBase::onInit();

	if(IsisSystem::instance()->isInternetDateTimeAvailable() == false)
	{
		getPage()->showError(getPage()->getText(_S("ntp.errors.synchronization_failed"))); // TOFIX: Change translation id
		return;
	}

	double currentPosition = 0;
	shared_ptr<EntitiesEntity> parentEntity = (m_parent != nullptr) ? m_parent : m_entity->getParent(getDatabase());
	if((m_entity != nullptr) && (m_entity->getCurrent() != nullptr))
		currentPosition = m_entity->getCurrent()->position;
	m_position->setPosition(parentEntity, currentPosition);

	shared_ptr<XMLNode> editor_node = getDocument()->create(_S("editor"));

	String title;
	if(isRevision())
		title = _S("Edit \"") + getCurrent()->getTitle() + _S("\"");
	else
		title = _S("Create new \"") + ObjectsSystem::instance()->getDescriptor(m_type)->getTypeName() + _S("\"");

	editor_node->setAttributeString(_S("title"), title);

	renderActions(editor_node->addChild(_S("actions")));
}

void IObjectEditor::onPreRender()
{
	ControlBase::onPreRender();

	m_comment->setVisible(m_enableComment->getCheck());
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
