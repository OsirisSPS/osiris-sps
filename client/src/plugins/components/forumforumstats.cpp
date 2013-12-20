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
#include "forumforumstats.h"

#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "iideportalpage.h"
#include "entitiesentity.h"
#include "portalsportal.h"
#include "portalsportal.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace forum {

//////////////////////////////////////////////////////////////////////

ForumStats::ForumStats()
{

}

ForumStats::~ForumStats()
{

}

String ForumStats::getTableName() const
{
	return OS_FORUMS_FORUM_STATS_TABLE;
}

void ForumStats::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	StatisticsBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getNodeStats();
	node->setAttributeUint32(OS_FORUMS_FORUM_STATS_TOPICS, topics);
	node->setAttributeUint32(OS_FORUMS_FORUM_STATS_POSTS, posts);
	if(last_topic->empty() == false)
	{
		shared_ptr<EntitiesEntity> lastTopicEntity = exporter->getPage()->getPortal()->getEntity(exporter->getDatabase(), last_topic);
		if(lastTopicEntity != nullptr)
		{
			shared_ptr<XMLPortalExporter> lastTopicExporter = exporter->createChild<XMLPortalExporter>(exporter->getNodeStats()->addChild(OS_FORUMS_FORUM_STATS_LAST_TOPIC));
			lastTopicExporter->setWithStats(false);		// Per evitare di andare in ricorsivo
			lastTopicEntity->exportXML(lastTopicExporter);
		}
	}

	if(last_message->empty() == false)
	{
		shared_ptr<EntitiesEntity> lastMessageEntity = exporter->getPage()->getPortal()->getEntity(exporter->getDatabase(), last_message);
		if(lastMessageEntity != nullptr)
		{
			shared_ptr<XMLPortalExporter> lastMessageExporter = exporter->createChild<XMLPortalExporter>(exporter->getNodeStats()->addChild(OS_FORUMS_FORUM_STATS_LAST_MESSAGE));
			lastMessageExporter->setWithStats(false);		// Per evitare di andare in ricorsivo
			lastMessageEntity->exportXML(lastMessageExporter);

			ordered_map<std::wstring, std::wstring> params;
			params.set(_W("mode"), _W("thread"));
			String threadLink = lastMessageExporter->getPortal()->getViewObjectLink(last_message, params);
			node->setAttributeString(_S("last_entry_href"), threadLink);
		}
	}

	if(last_message_date->isValid())
		node->setAttributeDateTime(OS_FORUMS_FORUM_STATS_LAST_MESSAGE_DATE, last_message_date);
}

bool ForumStats::onRead(const DataTableRow &row)
{
	if(StatisticsBase::onRead(row) == false)
		return false;

	topics.read(row, OS_FORUMS_FORUM_STATS_TOPICS);
	posts.read(row, OS_FORUMS_FORUM_STATS_POSTS);
	last_topic.read(row, OS_FORUMS_FORUM_STATS_LAST_TOPIC);
	last_message.read(row, OS_FORUMS_FORUM_STATS_LAST_MESSAGE);
	last_message_date.read(row, OS_FORUMS_FORUM_STATS_LAST_MESSAGE_DATE);

	return true;
}

bool ForumStats::onWrite(DbSqlValues &values) const
{
	if(StatisticsBase::onWrite(values) == false)
		return false;

	topics.write(values, OS_FORUMS_FORUM_STATS_TOPICS);
	posts.write(values, OS_FORUMS_FORUM_STATS_POSTS);
	last_topic.write(values, OS_FORUMS_FORUM_STATS_LAST_TOPIC);
	last_message.write(values, OS_FORUMS_FORUM_STATS_LAST_MESSAGE);
	last_message_date.write(values, OS_FORUMS_FORUM_STATS_LAST_MESSAGE_DATE);

	return true;
}

//////////////////////////////////////////////////////////////////////

} // forum
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
