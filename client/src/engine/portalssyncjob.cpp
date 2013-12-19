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
#include "portalssyncjob.h"

#include "dataentry.h"
#include "iportaldatabase.h"
#include "idbconnection.h"
#include "idbresult.h"
#include "entitiessnapshotmanager.h"
#include "log.h"
#include "notificationsmanager.h"
#include "objectsiobject.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "portalstransaction.h"
#include "realtimestats.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PortalsSyncJob::PortalsSyncJob(uint32 id, shared_ptr<Portal> portal) : JobBase(false, id),
																   m_portal(portal)
{

}

PortalsSyncJob::~PortalsSyncJob()
{

}

IJob::JobStatus PortalsSyncJob::run()
{
	shared_ptr<Portal> portal = getPortal();
	if(portal == null)
		return jobComplete;

	String povOrigin = portal->getSync();
	if(povOrigin.empty())
		return jobComplete;
	
	NotificationsManager::instance()->notify(_S("Sync of '") + portal->getPovName() + _S("' started."));

	shared_ptr<Portal> portalOrigin = PortalsSystem::instance()->getPortalByFullPov(povOrigin);
	if(portalOrigin == null)
	{
		NotificationsManager::instance()->notify(_S("Sync of '") + portal->getPovName() + _S("' failed, source portal unknown."));
		return jobComplete;
	}

	// Open Source
	shared_ptr<PortalsTransaction> transactionOrigin = portalOrigin->startTransaction(false);
	if(transactionOrigin == null)
	{
		NotificationsManager::instance()->notify(_S("Sync of '") + portal->getPovName() + _S("' failed, unable to open the source database."));
		return jobComplete;
	}

	shared_ptr<IPortalDatabase> databaseOrigin = transactionOrigin->getDatabase();
	if(databaseOrigin == null)
	{
		NotificationsManager::instance()->notify(_S("Sync of '") + portal->getPovName() + _S("' failed, unable to open the source database."));
		return jobComplete;
	}

	// Open Destination
	shared_ptr<PortalsTransaction> transactionCurrent = portal->startTransaction(false);
	if(transactionCurrent == null)
	{
		NotificationsManager::instance()->notify(_S("Sync of '") + portal->getPovName() + _S("' failed, unable to open the destination database."));
		return jobComplete;
	}

	shared_ptr<IPortalDatabase> databaseCurrent = transactionCurrent->getDatabase();
	if(databaseCurrent == null)
	{
		NotificationsManager::instance()->notify(_S("Sync of '") + portal->getPovName() + _S("' failed, unable to open the destination database."));
		return jobComplete;
	}


	// Work!
	DateTime lastSyncDate = portal->getOptions()->getLastSyncDate();

	shared_ptr<DbSqlSelect> query(OS_NEW DbSqlSelect(DBTABLES::ENTRIES_TABLE));

	query->count = true;

	DataTable result;
	if(databaseOrigin->execute(query, result) == false)
		return jobComplete;

	uint32 totalObjects = static_cast<uint32>(*result[0][0]);

	query->count = false;

	query->fields.add(DbSqlField(DBTABLES::ID));
	query->where.add(DbSqlField(DBTABLES::ENTRIES::INSERT_DATE, DBTABLES::ENTRIES_TABLE), Convert::toSQL(lastSyncDate), DbSqlCondition::cfMajor | DbSqlCondition::cfAnd);
	//query->where.add(DBTABLES::ENTRIES::TYPE, Convert::toSQL(static_cast<uint32>(portalObjectTypeUser)), DbSqlCondition::cfEqual | DbSqlCondition::cfAnd);
	shared_ptr<IDbResult> resultObjects = databaseOrigin->getConnection()->query(query);

	DataTable table;
	resultObjects->init(table);
	DataTableRow row = table.addRow();

	DateTime maxInsertDate;

	uint32 parsedObject = 0;
	while(resultObjects->end() == false)
	{
		resultObjects->bind(row);

		ObjectID objectID = static_cast<String>(*row[0]).to_ascii();
		shared_ptr<ObjectsIObject> object = portalOrigin->getObject(databaseOrigin, objectID);
		if(object == null)
		{
			OS_ASSERTFALSE();
			return jobComplete;
		}

		shared_ptr<DataEntry> entry = databaseOrigin->getEntry(object->id);
		if(entry == null)
		{
			OS_ASSERTFALSE();
			return jobComplete;
		}
				
		if( (maxInsertDate.isValid() == false) || (entry->insert_date > maxInsertDate) )
			maxInsertDate = entry->insert_date;
		
		if(databaseCurrent->needObject(objectID.getString(), object->submit_date))		
			object->store(databaseCurrent);
		
		resultObjects->moveNext();

		parsedObject++;

		m_progressPercentage = parsedObject / static_cast<double>(totalObjects);
		//setProgressPercentage(progressPercentage);
		//progressCallback(progressPercentage);

		
	}


	/*
	shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(true);
	shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();

	// DB Analysis
	RealtimeStatsScopeTimer rssAnalysis(_S("Activity"), _S("Database analysis"));
	database->beginTransaction();
	database->getConnection()->analyze();
	database->commit();
	rssAnalysis.stop();
	*/
	

	// End
	//portal->getOptions()->setLastOptimizationDate(DateTime::now());

	portal->getOptions()->setLastSyncDate(maxInsertDate);

	databaseOrigin->commit();
	databaseCurrent->commit();

	NotificationsManager::instance()->notify(_S("Sync of '") + portal->getPovName() + _S("' completed."));

	return jobComplete;
}

shared_ptr<IBackgroundJob::Details> PortalsSyncJob::getDetails() const
{
	String jobName("sync");

	shared_ptr<Portal> portal = getPortal();
	if(portal != null)
	{
		jobName.append(" - ");
		jobName.append(portal->getPovName());
	}

	shared_ptr<Details> details(OS_NEW Details(jobName.to_wide()));
	details->setPercentage(m_progressPercentage);

	return details;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
