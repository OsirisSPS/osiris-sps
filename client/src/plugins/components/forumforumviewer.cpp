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
#include "forumforumviewer.h"

#include "conversions.h"
#include "dbtables.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "ideeditpolleditor.h"
#include "ideedittexteditor.h"
#include "idesession.h"
#include "ideskin.h"
#include "forumforumconfig.h"
#include "forumforummanager.h"
#include "forumtopicviewer.h"
#include "objectssystem.h"
#include "idesession.h"
#include "portalsportal.h"
#include "httprequest.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace forum {

//////////////////////////////////////////////////////////////////////

ForumViewer::ForumViewer(shared_ptr<EntitiesEntity> topic) : m_topic(topic)
{

}

ForumViewer::~ForumViewer()
{

}

ForumViewer::Action ForumViewer::getAction() const
{
	String action = getRequest()->getUrlParam(OS_URL_PARAM_ACTION);
	if(action.empty())
		return faNone;

	if(action == conversions::to_utf16<uint32>(faCreateTopic))
		return faCreateTopic;

	if(action == conversions::to_utf16<uint32>(faCreatePoll))
		return faCreatePoll;

	return faNone;
}

String ForumViewer::getCreateTopicLink(const EntityID &forum)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_ACTION, conversions::to_wstring<uint32>(faCreateTopic));

	return getPortal()->getViewObjectLink(forum, params);
}

String ForumViewer::getCreatePollLink(const EntityID &forum)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_ACTION, conversions::to_wstring<uint32>(faCreatePoll));

	return getPortal()->getViewObjectLink(forum, params);
}

String ForumViewer::getTemplatePath()
{
	return utils::makeFilePath(ForumManager::instance()->getPath(), _S("forum_viewer.xsl"));
}

String ForumViewer::getPageHref(uint32 offset) const
{
	String href;

	shared_ptr<EntitiesEntity> entity = getEntity(getDatabase());
	if(entity != nullptr)
	{
		ordered_map<std::wstring, std::wstring> params;
		if(offset != 0)
			params.set(OS_URL_PARAM_OFFSET, conversions::to_wstring(offset));

		href = entity->getViewLink(getPortal(), params);
	}

	return href;
}

void ForumViewer::onLoad()
{
	ViewerBase::onLoad();

	if(m_topic != nullptr)
	{
		getControls()->add(shared_ptr<IHtmlControl>(OS_NEW TopicViewer(m_topic)));
	}
	else
	{
		switch(getAction())
		{
		case faNone:			onViewForum(conversions::from_utf16<uint32>(getRequest()->getUrlParam(OS_URL_PARAM_OFFSET)));
								break;

		case faCreateTopic:		onCreateTopic();
								break;

		case faCreatePoll:		onCreatePoll();
								break;

		default:				OS_ASSERTFALSE();
								break;
		}
	}
}

void ForumViewer::onCreateTopic()
{
	ide::edit::editor_ptr topic_editor(OS_NEW ide::edit::TextEditor(nullptr, getEntity(getDatabase())));
	topic_editor->setID(_S("topicEditor"));
	// Una volta creato il topic effettua il redirect in modalit di vista
	topic_editor->setRedirectionType(ide::edit::IObjectEditor::rtView);

	getControls()->add(topic_editor);
}

void ForumViewer::onCreatePoll()
{
	ide::edit::editor_ptr poll_editor(OS_NEW ide::edit::PollEditor(nullptr, getEntity(getDatabase())));
	poll_editor->setID(_S("pollEditor"));

	getControls()->add(poll_editor);
}

void ForumViewer::onViewForum(uint32 offset)
{
	shared_ptr<EntitiesEntity> entity = getEntity(getDatabase());
	if(entity != nullptr)
	{
		shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(getDocument()->create(OS_COMFORUM_FORUM_NODE), getPage(), XMLPortalExporter::emFull));

		ForumManager::instance()->renderForum(entity, exporter);

		renderForumActions(entity, exporter);
		renderForumContent(entity, exporter, offset);
	}
}

void ForumViewer::renderForumActions(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter)
{
	OS_ASSERT(entity != nullptr);
	if(getPage()->getSessionAccount()->isPortalGuest(getDatabase()) == false)
	{
		exporter->createAction(_S("createTopic"), getCreateTopicLink(entity->getEntityID()));
		exporter->createAction(_S("createPoll"), getCreatePollLink(entity->getEntityID()));
	}
}

void ForumViewer::renderForumContent(shared_ptr<EntitiesEntity> entity, shared_ptr<XMLPortalExporter> exporter, uint32 offset)
{
	OS_ASSERT(entity != nullptr);

	ObjectsTypes types;
	types.push_back(portalObjectTypeText);
	types.push_back(portalObjectTypePoll);

	// Calcola il numero totale di discussioni
	uint32 total_discussions = entity->getChildsCount(getDatabase(), types);
	if(total_discussions > 0)
	{
		uint32 discussions_to_show = getSkin()->getPagerItems();

		// Corregge l'offset se necessario
		adjustOffset(total_discussions, discussions_to_show, offset);

		shared_ptr<DbSqlSelect> select;
		// Inizializza lo statement di base per il caricamento delle discussioni
		entity->getChildsStatement(getDatabase(), types, RangeUint32(offset, discussions_to_show), false, false, false, select);

		// Aggiunge alla select la tabella sulle statistiche dei testi
		select->tables.add(DbSqlTable(DBTABLES::DISCUSSIONS_STATS_TABLE));

		// Crea la join con la tabella delle statistiche dei testi
		select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::ENTITY, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField(DBTABLES::DISCUSSIONS_STATS::REFERENCE, DBTABLES::DISCUSSIONS_STATS_TABLE));

		// Crea l'ordinamento per il campo pinned
		DbSqlField pinned_field(DBTABLES::DISCUSSIONS_STATS::PINNED, DBTABLES::DISCUSSIONS_STATS_TABLE);
		pinned_field.setOrder(DbSqlField::foDesc);
		select->orderBy.fields.add(pinned_field);

		// Crea l'ordinamento per la data di creazione
		DbSqlField last_message_date_field(DBTABLES::DISCUSSIONS_STATS::LAST_ENTRY_DATE, DBTABLES::DISCUSSIONS_STATS_TABLE);
		last_message_date_field.setOrder(DbSqlField::foDesc);
		select->orderBy.fields.add(last_message_date_field);

		// Carica le discussioni nel range richiesto
		shared_ptr<EntitiesEntities> discussions_childs = entity->getChilds(getDatabase(), types, RangeUint32(offset, discussions_to_show), select);
		if(discussions_childs != nullptr && discussions_childs->empty() == false)
		{
			shared_ptr<XMLNode> forum_node = exporter->getRoot();
			shared_ptr<XMLNode> discussions_node = forum_node->addChild(_S("discussions"));

			for(EntitiesEntities::const_iterator i = discussions_childs->begin(); i != discussions_childs->end(); ++i)
			{
				shared_ptr<EntitiesEntity> discussion = discussions_childs->get(getDatabase(), *i);
				if(discussion != nullptr)
				{
					shared_ptr<XMLPortalExporter> discussionExporter(OS_NEW XMLPortalExporter(discussions_node->addChild(_S("discussion")), getPage(), XMLPortalExporter::emFull));
					discussion->exportXML(discussionExporter);
				}
			}

			createPager(getSkin(), forum_node, total_discussions, discussions_to_show, offset);
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // forum
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
