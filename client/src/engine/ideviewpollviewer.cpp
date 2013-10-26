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
#include "ideviewpollviewer.h"

#include "boost/bind.hpp"
#include "iportaldatabase.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "idesession.h"
#include "ideskin.h"
#include "objectssystem.h"
#include "objectspoll.h"
#include "objectspolloption.h"
#include "objectspollvote.h"
#include "portalsportal.h"
#include "htmlevent.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace view {

//////////////////////////////////////////////////////////////////////

const String PollViewer::EVENT_ONVOTE = _S("onVote");
const String PollViewer::EVENT_ONUNVOTE = _S("onUnvote");

//////////////////////////////////////////////////////////////////////

PollViewer::PollViewer(shared_ptr<EntitiesEntity> entity) : ControlBase(entity)
{
	getEvents()->get(EVENT_ONVOTE)->connect(boost::bind(&PollViewer::onVote, this, _1));
	getEvents()->get(EVENT_ONUNVOTE)->connect(boost::bind(&PollViewer::onUnvote, this));
}

PollViewer::~PollViewer()
{

}

String PollViewer::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("poll_viewer.xsl"));
}

void PollViewer::onInit()
{
	ControlBase::onInit();

	if( (getEntity() == null) || (getObject() == null) )
		return;

	if(getSessionAccount()->isPortalGuest(getDatabase()) == false)
	{
		m_vote = getDatabase()->getPollVote(getSessionAccount()->getUser(getDatabase()), objects_poll_cast(getObject()));
	}
}

void PollViewer::renderObject(shared_ptr<XMLPortalExporter> exporter, shared_ptr<ObjectsIRevisionable> object)
{
	ControlBase::renderObject(exporter, object);

	if(getSessionAccount()->isPortalGuest(getDatabase()) == false)
	{
		if(m_vote != null && m_vote->isNull() == false)
		{
			exporter->createAction(_S("unvote"), getEventCommand(EVENT_ONUNVOTE));
		}
	}

	shared_ptr<ObjectsIDescriptor> poll_option_descriptor = ObjectsSystem::instance()->getDescriptor(portalObjectTypePollOption);
	shared_ptr<XMLNode> options_node;

	shared_ptr<EntitiesEntities> poll_childs = getEntity()->getChilds(getDatabase(), portalObjectTypePollOption);
	for(EntitiesEntities::iterator i = poll_childs->begin(); i != poll_childs->end(); ++i)
	{
		shared_ptr<EntitiesEntity> poll_option_entity = poll_childs->get(getDatabase(), *i);
		if(poll_option_entity != null)
		{
			shared_ptr<ObjectsPollOption> poll_option = objects_poll_option_cast(poll_option_entity->getCurrent());
			if(poll_option != null)
			{
				if(options_node == null)
				{
                    options_node = exporter->getNode(poll_option_descriptor->getGroupName());
				}

				shared_ptr<XMLNode> option_node = options_node->addChild(poll_option_descriptor->getTypeName());
				exportPollOption(option_node, poll_option, m_vote != null ? m_vote->isReferencedTo(poll_option->getEntityID()) : false);
			}
		}
	}
}

void PollViewer::exportPollOption(const shared_ptr<XMLNode> &option_node, shared_ptr<ObjectsPollOption> option, bool current)
{
	shared_ptr<XMLPortalExporter> pollOptionExporter(OS_NEW XMLPortalExporter(option_node, getPage(), XMLPortalExporter::emFull));
	option->exportXML(pollOptionExporter);
	option_node->setAttributeBool(_S("current"), current);

	shared_ptr<XMLNode> actions_node = option_node->addChild(_S("actions"));

	if(getSessionAccount()->isPortalGuest(getDatabase()) == false)
	{
		if(current)
		{
			shared_ptr<XMLNode> action_unvote = actions_node->addChild(_S("action"));
			action_unvote->setAttributeString(_S("name"), _S("unvote"));
			action_unvote->setAttributeString(_S("href"), getEventCommand(EVENT_ONUNVOTE, option->getEntityID().toUTF16()));
		}
		else
		{
			shared_ptr<XMLNode> action_vote = actions_node->addChild(_S("action"));
			action_vote->setAttributeString(_S("name"), _S("vote"));
			action_vote->setAttributeString(_S("href"), getEventCommand(EVENT_ONVOTE, option->getEntityID().toUTF16()));
		}
	}
}

void PollViewer::onVote(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	if(getSessionAccount()->isPortalGuest(getDatabase()))
		return;

	shared_ptr<EntitiesEntity> option_entity = getPortal()->getEntity(getDatabase(), htmlEvent->get(0).to_ascii());
	if(option_entity != null)
	{
		LanguageResult result = getDatabase()->votePoll(getSessionAccount()->getUser(getDatabase()), getSessionAccount()->getPrivateKey(), objects_poll_cast(getEntity()->getCurrent()), option_entity->getEntityID());
		if(result.empty())
			getPage()->reload();
		else
			getPage()->showError(getPage()->getText(result));
	}
}

void PollViewer::onUnvote()
{
	if(getSessionAccount()->isPortalGuest(getDatabase()))
		return;

	LanguageResult result = getDatabase()->unvotePoll(getSessionAccount()->getUser(getDatabase()), getSessionAccount()->getPrivateKey(), objects_poll_cast(getEntity()->getCurrent()));
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
