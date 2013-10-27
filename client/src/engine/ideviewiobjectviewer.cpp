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
#include "ideviewiobjectviewer.h"

#include "boost/bind.hpp"
#include "conversions.h"
#include "iportaldatabase.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "idesession.h"
#include "idevoteeditor.h"
#include "objectsirevisionable.h"

#include "objectssystem.h"
#include "objectsvote.h"
#include "htmlevent.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace view {

//////////////////////////////////////////////////////////////////////

IObjectViewer::IObjectViewer(shared_ptr<EntitiesEntity> entity) : m_entity(entity)
{

}

IObjectViewer::~IObjectViewer()
{

}

/*
shared_ptr<ObjectsIRevisionable> IObjectViewer::getPrimaryObject() const
{
	return m_entity != null ? m_entity->getPrimary() : null;
}

shared_ptr<ObjectsIRevisionable> IObjectViewer::getCurrentObject() const
{
	return m_entity != null ? m_entity->getCurrent() : null;
}

const ObjectID & IObjectViewer::getPrimaryID() const
{
	if(m_entity != null)
		return m_entity->getPrimary()->id;

	return ObjectID::EMPTY;
}
*/

#ifdef OS_TODOCIP
const EntityID & IObjectViewer::getEntityID() const
{
	if(m_entity != null)
		return m_entity->getEntityID();

	return EntityID::EMPTY;
}
#else
EntityID IObjectViewer::getEntityID() const
{
	if(m_entity != null)
		return m_entity->getEntityID();

	return EntityID::EMPTY;
}
#endif

void IObjectViewer::renderVote(shared_ptr<XMLPortalExporter> exporter) const
{
	/*
	se l'oggetto  votabile deve renderizzare l'azione
	sistemare POLLS_OPTIONS_STATS

	if(hasUserVote())
		exporter->root->setAttributeString("vote", getUserVote())

	exporter->addAction("vote", ...)*/
}

void IObjectViewer::renderTags(shared_ptr<XMLPortalExporter> exporter) const
{
	if(m_entity == null)
		return;

	shared_ptr<EntitiesEntities> tags = m_entity->getChilds(getDatabase(), portalObjectTypeTag);
	if(tags->empty() == false)
	{
		shared_ptr<XMLNode> tags_node = exporter->getRoot()->addChild(ObjectsSystem::instance()->getGroupName(portalObjectTypeTag));
		for(EntitiesEntities::iterator i = tags->begin(); i != tags->end(); ++i)
		{
			shared_ptr<EntitiesEntity> tag = tags->get(getDatabase(), *i);
			if(tag != null)
			{
				shared_ptr<XMLPortalExporter> tagExporter(OS_NEW XMLPortalExporter(tags_node->addChild(ObjectsSystem::instance()->getTypeName(portalObjectTypeTag)), getPage(), XMLPortalExporter::emFull));
				tag->exportXML(tagExporter);
			}
		}
	}
}

String IObjectViewer::getPageHref(uint32 offset) const
{
	String href;
	if(m_entity != null)
	{
		ordered_map<std::wstring, std::wstring> params;
		if(offset != 0)
			params.set(OS_URL_PARAM_OFFSET, conversions::to_wstring(offset));

		href = m_entity->getViewLink(getPortal(), params);
	}

	return href;
}

void IObjectViewer::onLoad()
{
	ControlBase::onLoad();

	if( (getEntity() == null) || (getObject() == null) )
		return;

	shared_ptr<ObjectsIRevisionable> object = getEntity()->getCurrent();
	if(object->isVotable() && (getSessionAccount()->isPortalGuest(getDatabase()) == false))
	{
		m_voteEditor.reset(OS_NEW IdeVoteEditor());
		m_voteEditor->getEventUpdate()->connect(boost::bind(&IObjectViewer::onUpdateVote, this));
		getTemplate()->addChildParam(m_voteEditor, _S("vote_editor"));

		m_vote = getDatabase()->getObjectVote(getSessionAccount()->getUser(getDatabase()), object);
	}
}

void IObjectViewer::onPreRender()
{
	ControlBase::onPreRender();

	// TODO: da sistemare...adesso  stato messo qui per consentire al vote_editor di inizializzarsi nella onInit,
	// verr pulito quando i controlli riceveranno gli eventi di init e compagnia non appena vengono aggiunti ai controlli del padre
	if(m_vote != null)
	{
		OS_ASSERT(m_voteEditor != null);
		if(m_vote->isNull())
			m_voteEditor->setNull();
		else
            m_voteEditor->setValue(m_vote->score);
	}
}

void IObjectViewer::onUpdateVote()
{
	if( (getEntity() == null) || (getObject() == null) )
		return;

	LanguageResult result;
	if(m_voteEditor->isNull())
		result = getDatabase()->unvoteObject(getSessionAccount()->getUser(getDatabase()), getSessionAccount()->getPrivateKey(), getEntityID());
	else
		result = getDatabase()->voteObject(getSessionAccount()->getUser(getDatabase()), getSessionAccount()->getPrivateKey(), getEntityID(), m_voteEditor->getValue());

	if(result.empty())
		getPage()->reload();
	else
		getPage()->showError(getPage()->getText(result));
}

//////////////////////////////////////////////////////////////////////

} // view
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
