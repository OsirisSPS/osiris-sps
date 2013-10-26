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
#include "objectscalendarevent.h"

#include "dbbinder.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "entitiessnapshotmanager.h"
#include "ideeditcalendareventeditor.h"
#include "ideviewcalendareventviewer.h"
#include "datatree.h"
#include "objectssigner.h"
#include "searchcalendareventoptions.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_DESCRIPTOR(OS_NAMESPACE_NAME::ObjectsCalendarEventDescriptor)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

uint32 ObjectsCalendarEvent::VERSION = OS_PORTAL_OBJECT_CALENDAREVENT_VERSION;

//////////////////////////////////////////////////////////////////////

ObjectsCalendarEvent::ObjectsCalendarEvent() : CommentableBase(portalObjectTypeCalendarEvent, VERSION)
{
	repeat_mode = cmRepeatNone;
}

ObjectsCalendarEvent::~ObjectsCalendarEvent()
{

}

ObjectsCalendarRepeatMode ObjectsCalendarEvent::getRepeatMode() const
{
	switch(*repeat_mode)
	{
	case cmRepeatNone:
	case cmRepeatWeek:
	case cmRepeatMonth:
	case cmRepeatYear:
							return static_cast<ObjectsCalendarRepeatMode>(*repeat_mode);

	default:				OS_ASSERTFALSE();
							break;
	}

	return cmRepeatNone;
}

void ObjectsCalendarEvent::setRepeatMode(ObjectsCalendarRepeatMode mode)
{
	repeat_mode = static_cast<uint32>(mode);
}

bool ObjectsCalendarEvent::validate(shared_ptr<IPortalDatabase> database) const
{
	if(CommentableBase::validate(database) == false)
		return false;

	if(start_date->isValid() == false)
		return false;

	if(end_date->isValid() && end_date <= start_date)
		return false;

	switch(*repeat_mode)
	{
	case cmRepeatNone:
	case cmRepeatWeek:
	case cmRepeatMonth:
	case cmRepeatYear:
							break;

	default:				return false;
	}

	if(expiration_date->isValid() && expiration_date <= start_date)
		return false;

	return true;
}

void ObjectsCalendarEvent::exportXML(shared_ptr<XMLPortalExporter> exporter)
{
	CommentableBase::exportXML(exporter);

	if(exporter->getMode() != XMLPortalExporter::emLite)
	{
		shared_ptr<XMLNode> node = exporter->getRoot();
        node->setAttributeDateTime(DBTABLES::CALENDAR_EVENTS::START_DATE, start_date);
		node->setAttributeDateTime(DBTABLES::CALENDAR_EVENTS::END_DATE, end_date);
		node->setAttributeUint32(DBTABLES::CALENDAR_EVENTS::REPEAT_MODE, repeat_mode);
		node->setAttributeDateTime(DBTABLES::CALENDAR_EVENTS::EXPIRATION_DATE, expiration_date);
	}
}

bool ObjectsCalendarEvent::onSign(const shared_ptr<ObjectsSigner> &signer) const
{
	if(CommentableBase::onSign(signer) == false)
		return false;

	signer->add(start_date);
	signer->add(end_date);
	signer->add(repeat_mode);
	signer->add(expiration_date);

	return true;
}

bool ObjectsCalendarEvent::onRead(const ObjectID &id, const DataTree &data)
{
	if(CommentableBase::onRead(id, data) == false)
		return false;

	start_date.read(data, DBTABLES::CALENDAR_EVENTS::START_DATE);
	end_date.read(data, DBTABLES::CALENDAR_EVENTS::END_DATE);
	repeat_mode.read(data, DBTABLES::CALENDAR_EVENTS::REPEAT_MODE);
	expiration_date.read(data, DBTABLES::CALENDAR_EVENTS::EXPIRATION_DATE);

	return true;
}

bool ObjectsCalendarEvent::onWrite(DataTree &data) const
{
	if(CommentableBase::onWrite(data) == false)
		return false;

	start_date.write(data, DBTABLES::CALENDAR_EVENTS::START_DATE);
	end_date.write(data, DBTABLES::CALENDAR_EVENTS::END_DATE);
	repeat_mode.write(data, DBTABLES::CALENDAR_EVENTS::REPEAT_MODE);
	expiration_date.write(data, DBTABLES::CALENDAR_EVENTS::EXPIRATION_DATE);

	return true;
}

bool ObjectsCalendarEvent::onRead(const DataTableRow &row)
{
	if(CommentableBase::onRead(row) == false)
		return false;

	start_date.read(row, DBTABLES::CALENDAR_EVENTS::START_DATE);
	end_date.read(row, DBTABLES::CALENDAR_EVENTS::END_DATE);
	repeat_mode.read(row, DBTABLES::CALENDAR_EVENTS::REPEAT_MODE);
	expiration_date.read(row, DBTABLES::CALENDAR_EVENTS::EXPIRATION_DATE);

	return true;
}

bool ObjectsCalendarEvent::onWrite(DbSqlValues &values) const
{
	if(CommentableBase::onWrite(values) == false)
		return false;

	start_date.write(values, DBTABLES::CALENDAR_EVENTS::START_DATE);
	end_date.write(values, DBTABLES::CALENDAR_EVENTS::END_DATE);
	repeat_mode.write(values, DBTABLES::CALENDAR_EVENTS::REPEAT_MODE);
	expiration_date.write(values, DBTABLES::CALENDAR_EVENTS::EXPIRATION_DATE);

	return true;
}

//////////////////////////////////////////////////////////////////////

ObjectsCalendarEventDescriptor::ObjectsCalendarEventDescriptor()
{

}

ObjectsCalendarEventDescriptor::~ObjectsCalendarEventDescriptor()
{

}

shared_ptr<ObjectsIObject> ObjectsCalendarEventDescriptor::createObject()
{
	return shared_ptr<ObjectsIObject>(OS_NEW ObjectsCalendarEvent());
}

String ObjectsCalendarEventDescriptor::getTableName()
{
	return DBTABLES::CALENDAR_EVENTS_TABLE;
}

String ObjectsCalendarEventDescriptor::getTypeName()
{
	return OS_PORTAL_OBJECT_CALENDAREVENT_TYPENAME;
}

String ObjectsCalendarEventDescriptor::getGroupName()
{
	return OS_PORTAL_OBJECT_CALENDAREVENT_GROUPNAME;
}

bool ObjectsCalendarEventDescriptor::hasStatistics()
{
	return false;
}

shared_ptr<ISearchOptions> ObjectsCalendarEventDescriptor::getSearchOptions()
{
	return shared_ptr<ISearchOptions>(OS_NEW SearchCalendarEventOptions());
}

ide::edit::editor_ptr ObjectsCalendarEventDescriptor::createEditControl(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent)
{
	return ide::edit::editor_ptr(OS_NEW ide::edit::CalendarEventEditor(entity, parent));
}

ide::view::viewer_ptr ObjectsCalendarEventDescriptor::createViewControl(shared_ptr<EntitiesEntity> entity)
{
	return ide::view::viewer_ptr(OS_NEW ide::view::CalendarEventViewer(entity));
}
/*
void CalendarEventDescriptor::createStatistics(shared_ptr<IPortalDatabase> db, entities::snapshot_profile_ptr profile, shared_ptr<ObjectsIObject> object)
{
	shared_ptr<DataIStatistics> statistics(OS_NEW data::CalendarEventStats());
	statistics->create(db, profile, object);
}

void CalendarEventDescriptor::computeStatistics(shared_ptr<IPortalDatabase> db, entities::snapshot_profile_ptr profile, shared_ptr<ObjectsIObject> object)
{
	data::calendar_event_stats_ptr stats(OS_NEW data::CalendarEventStats());
	stats->profile = profile->getID();
	stats->reference = object->id;
	stats->posts = db->getStableObjectsCount(profile->getID(), object->id, otPost);
	db->computeVotesStats(profile, object, stats->votes, stats->votes_average);
	db->computeLastMessageStats(profile, object, stats->last_message, stats->last_message_date);

	db->updateRecord(stats);
}

void CalendarEventDescriptor::removeStatistics(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	removeProfileStatistics(db, DBTABLES::CALENDAR_EVENTS_STATS, object);
}
*/

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
