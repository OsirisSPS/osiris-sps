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
#include "objectspoll.h"

#include "datapollstats.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "entitiessnapshotmanager.h"
#include "ideeditpolleditor.h"
#include "ideviewpollviewer.h"
#include "datatree.h"
#include "objectspolloption.h"
#include "objectssigner.h"
#include "idesession.h"
#include "portalsportal.h"
#include "searchpolloptions.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsPollDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsPoll::VERSION = OS_PORTAL_OBJECT_POLL_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsPoll::ObjectsPoll() : CommentableBase(portalObjectTypePoll, VERSION)
{
	type = ptSingleChoice;
}

ObjectsPoll::~ObjectsPoll()
{

}

void ObjectsPoll::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	CommentableBase::exportXML(exporter);

	shared_ptr<XMLNode> node = exporter->getRoot();

	if(exporter->getMode() == XMLPortalExporter::emData)
	{
		node->setAttributeUint32(DBTABLES::POLLS::TYPE, type);
	}

	if(exporter->getWithStats())
	{
		shared_ptr<DataIStatistics> stats = exporter->getDatabase()->loadStatistics<DataPollStats>(DBTABLES::POLLS_STATS_TABLE, getEntityID().getString());
		if(stats != null)
			stats->exportXML(exporter);
	}
}

bool ObjectsPoll::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(CommentableBase::onSign(signer) == false)
		return false;

	signer->add(type);

	return true;
}

bool ObjectsPoll::onRead(const ObjectID &id, const DataTree &data)
{
	if(CommentableBase::onRead(id, data) == false)
		return false;

	type.read(data, DBTABLES::POLLS::TYPE);

	return true;
}

bool ObjectsPoll::onWrite(DataTree &data) const
{
	if(CommentableBase::onWrite(data) == false)
		return false;

	type.write(data, DBTABLES::POLLS::TYPE);

	return true;
}

bool ObjectsPoll::onRead(const DataTableRow &row)
{
	if(CommentableBase::onRead(row) == false)
		return false;

	type.read(row, DBTABLES::POLLS::TYPE);

	return true;
}

bool ObjectsPoll::onWrite(DbSqlValues &values) const
{
	if(CommentableBase::onWrite(values) == false)
		return false;

	type.write(values, DBTABLES::POLLS::TYPE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsPollDescriptor::ObjectsPollDescriptor()
{

}

ObjectsPollDescriptor::~ObjectsPollDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsPollDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsPoll());
}

String ObjectsPollDescriptor::getTableName()
{
	return DBTABLES::POLLS_TABLE;
}

String ObjectsPollDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_POLL_TYPENAME;
}

String ObjectsPollDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_POLL_GROUPNAME;
}

void ObjectsPollDescriptor::getAllowedChilds(ObjectsTypes &allowed_childs) const
{
	DescriptorBase::getAllowedChilds(allowed_childs);

	allowed_childs.push_back(portalObjectTypePollOption);
}

bool ObjectsPollDescriptor::hasStatistics()
{
	return true;
}

shared_ptr<ISearchOptions> ObjectsPollDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchPollOptions());
}

ide::edit::editor_ptr ObjectsPollDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::PollEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsPollDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::PollViewer(entity));
}

void ObjectsPollDescriptor::createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::createStatistics(db, object);

	shared_ptr<ObjectsPoll> poll = objects_poll_cast(object);

	shared_ptr<DataIStatistics> statistics(OS_NEW DataPollStats());
	statistics->reference = poll->getEntityID().toUTF16();
	statistics->create_ZZZ(db);
}

void ObjectsPollDescriptor::computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::computeStatistics(db, object);

	shared_ptr<ObjectsPoll> poll = objects_poll_cast(object);

	shared_ptr<DataPollStats> stats(OS_NEW DataPollStats());
	stats->reference = poll->getEntityID().toUTF16();

	String sql = String::format(_S("select count(*) from os_snapshot_users, os_polls_votes where \
								os_snapshot_users.id = os_polls_votes.author \
								and os_snapshot_users.visible=1 \
								and os_polls_votes.parent = '%S' \
								and os_polls_votes.score is not null").c_str(),
								poll->getEntityID().toUTF16().c_str());

	DataTable table;
	if(db->execute(sql, table))
		stats->poll_votes = static_cast<uint32>(*table[0][0]);

	db->updateRecord(stats);
}

void ObjectsPollDescriptor::removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::removeStatistics(db, object);

	ObjectOrEntityID id;
	if(object)
	{
		shared_ptr<ObjectsPoll> poll = objects_poll_cast(object);
		id = poll->getEntityID().toUTF16();
	}	

	db->removeProfileStatistics(DBTABLES::POLLS_STATS_TABLE, id);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
