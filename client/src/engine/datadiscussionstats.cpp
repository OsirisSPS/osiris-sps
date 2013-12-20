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
#include "datadiscussionstats.h"

#include "dbbinder.h"
#include "dbtables.h"
#include "entitiesentity.h"
#include "idesession.h"
#include "iideportalpage.h"
#include "iportaldatabase.h"
#include "portalsportal.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DataDiscussionStats::DataDiscussionStats()
{
	posts = 0;
	votes = 0;
	votes_average = 0;
	pinned = false;
}

DataDiscussionStats::~DataDiscussionStats()
{

}

String DataDiscussionStats::getTableName() const
{
	return DBTABLES::DISCUSSIONS_STATS_TABLE;
}

void DataDiscussionStats::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	StatisticsBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getNodeStats();
	node->setAttributeUint32(DBTABLES::DISCUSSIONS_STATS::POSTS, posts);
	node->setAttributeUint32(DBTABLES::DISCUSSIONS_STATS::VOTES, votes);
	node->setAttributeDouble(DBTABLES::DISCUSSIONS_STATS::VOTES_AVERAGE, votes_average);
	if(last_entry->empty() == false)
	{
		shared_ptr<EntitiesEntity> lastEntryEntity = exporter->getPage()->getPortal()->getEntity(exporter->getDatabase(), last_entry);
		if(lastEntryEntity != nullptr)
		{
			shared_ptr<XMLPortalExporter> lastEntryExporter = exporter->createChild<XMLPortalExporter>(exporter->getNodeStats()->addChild(DBTABLES::DISCUSSIONS_STATS::LAST_ENTRY));
			lastEntryExporter->setWithStats(false);			// Per evitare di andare in ricorsivo se l'entry coincide con l'oggetto
			lastEntryEntity->exportXML(lastEntryExporter);

			ordered_map<std::wstring, std::wstring> params;
			params.set(_W("mode"), _W("thread"));
			String threadLink = lastEntryExporter->getPortal()->getViewObjectLink(last_entry, params);
			node->setAttributeString(_S("last_entry_href"), threadLink);
		}
	}

	if(last_entry_date->isValid())
		node->setAttributeDateTime(DBTABLES::DISCUSSIONS_STATS::LAST_ENTRY_DATE, last_entry_date);

	node->setAttributeBool(DBTABLES::DISCUSSIONS_STATS::PINNED, pinned);
}

bool DataDiscussionStats::onRead(const DataTableRow &row)
{
	if(StatisticsBase::onRead(row) == false)
		return false;

	parent.read(row, DBTABLES::DISCUSSIONS_STATS::PARENT);
	posts.read(row, DBTABLES::DISCUSSIONS_STATS::POSTS);
	votes.read(row, DBTABLES::DISCUSSIONS_STATS::VOTES);
	votes_average.read(row, DBTABLES::DISCUSSIONS_STATS::VOTES_AVERAGE);
	last_entry.read(row, DBTABLES::DISCUSSIONS_STATS::LAST_ENTRY);
	last_entry_date.read(row, DBTABLES::DISCUSSIONS_STATS::LAST_ENTRY_DATE);
	pinned.read(row, DBTABLES::DISCUSSIONS_STATS::PINNED);

	return true;
}

bool DataDiscussionStats::onWrite(DbSqlValues &values) const
{
	if(StatisticsBase::onWrite(values) == false)
		return false;

	parent.write(values, DBTABLES::DISCUSSIONS_STATS::PARENT);
	posts.write(values, DBTABLES::DISCUSSIONS_STATS::POSTS);
	votes.write(values, DBTABLES::DISCUSSIONS_STATS::VOTES);
	votes_average.write(values, DBTABLES::DISCUSSIONS_STATS::VOTES_AVERAGE);
	last_entry.write(values, DBTABLES::DISCUSSIONS_STATS::LAST_ENTRY);
	last_entry_date.write(values, DBTABLES::DISCUSSIONS_STATS::LAST_ENTRY_DATE);
	pinned.write(values, DBTABLES::DISCUSSIONS_STATS::PINNED);

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
