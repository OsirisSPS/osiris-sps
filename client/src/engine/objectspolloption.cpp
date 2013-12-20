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
#include "objectspolloption.h"

#include "datapolloptionstats.h"
#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "entitiessnapshotmanager.h"
#include "ideeditpolloptioneditor.h"
#include "ideviewpolloptionviewer.h"
#include "datatree.h"
#include "objectssigner.h"
#include "idesession.h"
#include "portalsportal.h"
#include "searchpolloptionoptions.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsPollOptionDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsPollOption::VERSION = OS_PORTAL_OBJECT_POLL_OPTION_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsPollOption::ObjectsPollOption() : RevisionableBase(portalObjectTypePollOption, VERSION)
{

}

ObjectsPollOption::~ObjectsPollOption()
{

}

bool ObjectsPollOption::requireParent() const
{
	return true;
}

bool ObjectsPollOption::validate(shared_ptr<IPortalDatabase> database) const
{
	if(RevisionableBase::validate(database) == false)
		return false;

	if(title->empty())
		return false;

    return true;
}

const String & ObjectsPollOption::getTitle() const
{
	return title;
}

void ObjectsPollOption::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	RevisionableBase::exportXML(exporter);

	if(exporter->getMode() != XMLPortalExporter::emLite)
		exporter->getRoot()->setAttributeString(DBTABLES::POLLS_OPTIONS::TITLE, title);

	if(exporter->getWithStats())
	{
		shared_ptr<DataIStatistics> stats = exporter->getDatabase()->loadStatistics<DataPollOptionStats>(DBTABLES::POLLS_OPTIONS_STATS_TABLE, getEntityID().getString());
		if(stats != nullptr)
			stats->exportXML(exporter);
	}
}

bool ObjectsPollOption::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(RevisionableBase::onSign(signer) == false)
		return false;

	signer->add(title);

	return true;
}

bool ObjectsPollOption::onRead(const ObjectID &id, const DataTree &data)
{
	if(RevisionableBase::onRead(id, data) == false)
		return false;

	title.read(data, DBTABLES::POLLS_OPTIONS::TITLE);

	return true;
}

bool ObjectsPollOption::onWrite(DataTree &data) const
{
	if(RevisionableBase::onWrite(data) == false)
		return false;

	title.write(data, DBTABLES::POLLS_OPTIONS::TITLE);

	return true;
}

bool ObjectsPollOption::onRead(const DataTableRow &row)
{
	if(RevisionableBase::onRead(row) == false)
		return false;

	title.read(row, DBTABLES::POLLS_OPTIONS::TITLE);

	return true;
}

bool ObjectsPollOption::onWrite(DbSqlValues &values) const
{
	if(RevisionableBase::onWrite(values) == false)
		return false;

	title.write(values, DBTABLES::POLLS_OPTIONS::TITLE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsPollOptionDescriptor::ObjectsPollOptionDescriptor()
{

}

ObjectsPollOptionDescriptor::~ObjectsPollOptionDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsPollOptionDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsPollOption());
}

String ObjectsPollOptionDescriptor::getTableName()
{
	return DBTABLES::POLLS_OPTIONS_TABLE;
}

String ObjectsPollOptionDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_POLLOPTION_TYPENAME;
}

String ObjectsPollOptionDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_POLLOPTION_GROUPNAME;
}

bool ObjectsPollOptionDescriptor::hasStatistics()
{
	return true;
}

shared_ptr<ISearchOptions> ObjectsPollOptionDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchPollOptionOptions());
}

ide::edit::editor_ptr ObjectsPollOptionDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::PollOptionEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsPollOptionDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::PollOptionViewer(entity));
}

void ObjectsPollOptionDescriptor::createStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::createStatistics(db, object);

	shared_ptr<ObjectsPollOption> poll_option = objects_poll_option_cast(object);

	shared_ptr<DataIStatistics> statistics(OS_NEW DataPollOptionStats());
	statistics->reference = poll_option->getEntityID().toUTF16();
	statistics->create_ZZZ(db);
}

void ObjectsPollOptionDescriptor::computeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::computeStatistics(db, object);

	shared_ptr<ObjectsPollOption> poll_option = objects_poll_option_cast(object);
	OS_EXCEPT_IF(poll_option == nullptr, "Invalid poll option");

	shared_ptr<DataPollOptionStats> stats(OS_NEW DataPollOptionStats());
	stats->reference = poll_option->getEntityID().toUTF16();

	String sql = String::format(_S("select count(*) from os_snapshot_users, os_polls_votes where \
								os_snapshot_users.id = os_polls_votes.author \
								and os_snapshot_users.visible=1 \
								and os_polls_votes.parent = '%S' \
								and os_polls_votes.reference = '%S' \
								and os_polls_votes.score is not null").c_str(),
								poll_option->getParent().toUTF16().c_str(),
								poll_option->getEntityID().toUTF16().c_str());

	DataTable table;
	if(db->execute(sql, table))
		stats->votes = static_cast<uint32>(*table[0][0]);

	db->updateRecord(stats);
}

void ObjectsPollOptionDescriptor::removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	DescriptorBase::removeStatistics(db, object);
	
	ObjectOrEntityID id;
	if(object)
	{
		shared_ptr<ObjectsPollOption> poll_option = objects_poll_option_cast(object);
		id = poll_option->getEntityID().toUTF16();
	}	

	db->removeProfileStatistics(DBTABLES::POLLS_OPTIONS_STATS_TABLE, id);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
