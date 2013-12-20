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
#include "forumforum.h"

#include "iportaldatabase.h"
#include "entitiesentity.h"
#include "entitiessnapshotmanager.h"
#include "forumforumconfig.h"
#include "forumforumstats.h"
#include "forumforumviewer.h"
#include "objectsiobject.h"
#include "objectssection.h"
#include "portalsportal.h"
#include "xmlportalexporter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace forum {

//////////////////////////////////////////////////////////////////////

Forum::Forum()
{

}

Forum::~Forum()
{

}

ExtensionsComponentID Forum::getID()
{
	return ExtensionsComponentID(OS_COMFORUM_FORUM_ID.to_ascii());
}

String Forum::getName()
{
	return _S("Forums - forum");
}

shared_ptr<ExtensionsComponentEditor> Forum::createEditor(shared_ptr<EntitiesEntity> entity)
{
	return nullptr;
}

shared_ptr<ExtensionsComponentViewer> Forum::createViewer(shared_ptr<EntitiesEntity> entity)
{
	if(entity != nullptr && entity->getObjectType() != portalObjectTypeText)
		return nullptr;	// Il componente renderizza solo se stesso o un oggetto di tipo testo

	return shared_ptr<ExtensionsComponentViewer>(OS_NEW ForumViewer(entity));
}

bool Forum::removeProfileData(shared_ptr<IPortalDatabase> db)
{
	String sql=String::format(_S("delete from %S").c_str(),OS_FORUMS_FORUM_STATS_TABLE.c_str());
	db->execute(sql);
	return true;
}

void Forum::exportXML(const EntityID &id, shared_ptr<XMLPortalExporter> exporter)
{
	ComponentBase::exportXML(id, exporter);

	if(exporter->getWithStats())
	{
		shared_ptr<DataIStatistics> stats = exporter->getDatabase()->loadStatistics<ForumStats>(OS_FORUMS_FORUM_STATS_TABLE, id.toUTF16());
		if(stats != nullptr)
			stats->exportXML(exporter);
	}
}

void Forum::createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	ComponentBase::createStatistics(db, object);

	shared_ptr<ObjectsSection> section = objects_section_cast(object);

	shared_ptr<DataIStatistics> statistics(OS_NEW ForumStats());
	statistics->reference = section->getEntityID().toUTF16();
	statistics->create_ZZZ(db);
}

void Forum::computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	ComponentBase::computeStatistics(db, object);

	shared_ptr<ObjectsSection> section = objects_section_cast(object);

	shared_ptr<ForumStats> stats(OS_NEW ForumStats());

	stats->reference = section->getEntityID().toUTF16();
	stats->topics = 0;
	stats->posts = 0;

	uint32 directTopics=0;
	uint32 directPosts=0;
	// Calcolo le statistiche sui topics figli
	{
		String sql = String::format(_S("select count(*), sum(os_discussions_stats.posts) from os_discussions_stats, os_snapshot_objects where \
								    os_snapshot_objects.parent = '%S' \
									and os_snapshot_objects.type in (%d,%d) \
									and os_snapshot_objects.visible = 1 \
									and os_discussions_stats.reference = os_snapshot_objects.entity \
									and os_discussions_stats.parent = '%S'").c_str(),
									section->getEntityID().toUTF16().c_str(),
									portalObjectTypeText,
									portalObjectTypePoll,
									section->getEntityID().toUTF16().c_str()
									);

		DataTable table;
		db->execute(sql, table);
		directTopics = *table[0][0];
		directPosts = *table[0][1];
	}

	uint32 indirectTopics=0;
	uint32 indirectPosts=0;
	// Calcolo le statistiche sui forums figli
	{
		String sql = String::format(String(_S("\
									select sum(") + OS_FORUMS_FORUM_STATS_TABLE + _S(".topics), sum(") + OS_FORUMS_FORUM_STATS_TABLE + _S(".posts) \
									from ") + OS_FORUMS_FORUM_STATS_TABLE + _S(",os_snapshot_objects,os_sections \
									where \
								    os_snapshot_objects.entity = ") + OS_FORUMS_FORUM_STATS_TABLE + _S(".reference and \
									os_snapshot_objects.current = os_sections.id and \
									os_sections.component = '%S' and \
									os_snapshot_objects.parent = '%S' and \
									os_snapshot_objects.visible = 1")).c_str(),
									getID().toUTF16().c_str(),
									section->getEntityID().toUTF16().c_str());

		DataTable table;
		db->execute(sql, table);
		indirectTopics = *table[0][0];
		indirectPosts = *table[0][1];
	}
	stats->reference = section->getEntityID().toUTF16();
	stats->topics = directTopics + indirectTopics;
	stats->posts = directPosts + indirectPosts;

	// Calcolo le statistiche riferite ai topic figli
	{
		String sql = String::format(_S("\
			select os_discussions_stats.reference, os_discussions_stats.last_entry, os_discussions_stats.last_entry_date \
			from \
			os_snapshot_objects, \
			os_discussions_stats \
			where \
			os_snapshot_objects.entity = os_discussions_stats.reference and \
			os_snapshot_objects.parent = '%S' and \
			os_snapshot_objects.type in (%d,%d) and \
			os_snapshot_objects.visible = 1 \
			order by os_discussions_stats.last_entry_date desc \
			limit 1").c_str(),
		section->getEntityID().toUTF16().c_str(),
		portalObjectTypeText,
		portalObjectTypePoll);

		DataTable table;
		db->execute(sql, table);
		if(table.rows()>0)
		{
			stats->last_topic = *table[0][0];
			stats->last_message = *table[0][1];
			stats->last_message_date = *table[0][2];
		}
	}

	// Calcolo le statistiche riferite ai forums figli
	{
		String sql = String::format(String(_S("\
			select ") + OS_FORUMS_FORUM_STATS_TABLE + _S(".last_topic, ") + OS_FORUMS_FORUM_STATS_TABLE + _S(".last_message, ") + OS_FORUMS_FORUM_STATS_TABLE + _S(".last_message_date \
			from \
			os_snapshot_objects, \
			") + OS_FORUMS_FORUM_STATS_TABLE + _S(", \
			os_sections \
			where \
			os_snapshot_objects.entity = ") + OS_FORUMS_FORUM_STATS_TABLE + _S(".reference and \
			os_snapshot_objects.current = os_sections.id and \
			os_sections.component = '%S' and \
			os_snapshot_objects.parent = '%S' and \
			os_snapshot_objects.visible = 1 \
			order by ") + OS_FORUMS_FORUM_STATS_TABLE + _S(".last_message_date desc \
			limit 1")).c_str(),
		getID().toUTF16().c_str(),
		section->getEntityID().toUTF16().c_str());

		DataTable table;
		db->execute(sql, table);
		if(table.rows()>0)
		{
			DateTime last_message_date = static_cast<String>(*table[0][2]);
			if( (stats->last_message_date->isNull()) || (last_message_date > stats->last_message_date) )
			{
				stats->last_topic = *table[0][0];
				stats->last_message = *table[0][1];
				stats->last_message_date = *table[0][2];
			}
		}
	}

	db->updateRecord(stats);
}

void Forum::removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	ComponentBase::removeStatistics(db, object);

	ObjectOrEntityID id;
	if(object)
	{
		shared_ptr<ObjectsSection> section = objects_section_cast(object);
		id = section->getEntityID().toUTF16();
	}	

	db->removeProfileStatistics(OS_FORUMS_FORUM_STATS_TABLE, id);	
}

//////////////////////////////////////////////////////////////////////

} // forum
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
