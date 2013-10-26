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
#include "portalsoptimizerjob.h"

#include "iportaldatabase.h"
#include "idbconnection.h"
#include "engine.h"
#include "entitiessnapshotmanager.h"
#include "log.h"
#include "notificationsmanager.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalstransaction.h"
#include "realtimestats.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PortalsOptimizerJob::PortalsOptimizerJob(uint32 id, shared_ptr<Portal> portal) : JobBase(false, id, portal)
{

}

PortalsOptimizerJob::~PortalsOptimizerJob()
{

}

IJob::JobStatus PortalsOptimizerJob::run()
{
	shared_ptr<Portal> portal = getPortal();
	if(portal == null)
		return jobComplete;

	NotificationsManager::instance()->notify(_S("Optimization of '") + portal->getPovName() + _S("' started."));
		
	// Lucene
	RealtimeStatsScopeTimer rssLucene(_S("Activity"), _S("Lucene optimizer"));
	portal->getSnapshotManager()->optimizeSearchIndexes();
	rssLucene.stop();
	
	shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(true);
	shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();

	// DB Analysis
	RealtimeStatsScopeTimer rssAnalysis(_S("Activity"), _S("Database analysis"));
	database->beginTransaction();
	database->getConnection()->analyze();
	database->commit();
	rssAnalysis.stop();
	

	// End
	portal->getOptions()->setLastOptimizationDate(DateTime::now());

	NotificationsManager::instance()->notify(_S("Optimization of '") + portal->getPovName() + _S("' completed."));

	return jobComplete;
}

shared_ptr<IBackgroundJob::Details> PortalsOptimizerJob::getDetails() const
{
	String jobName(Engine::instance()->getText("job.optimizer"));

	shared_ptr<Portal> portal = getPortal();
	if(portal != null)
	{
		jobName.append(" - ");
		jobName.append(portal->getPovName());
	}

	shared_ptr<Details> details(OS_NEW Details(jobName.to_wide()));

	return details;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
