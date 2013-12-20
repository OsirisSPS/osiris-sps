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
#include "portalsstabilityjob.h"

#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "iportaldatabase.h"
#include "engine.h"
#include "entitiessnapshotmanager.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PortalsStabilityJob::PortalsStabilityJob(uint32 id, shared_ptr<Portal> portal) : JobBase(false, id, portal)
{

}

PortalsStabilityJob::~PortalsStabilityJob()
{

}

IJob::JobStatus PortalsStabilityJob::run()
{
	shared_ptr<Portal> portal = getPortal();
	if(portal == nullptr)
		return jobComplete;

	portal->setStabilizePending(false);

	EntitiesSnapshotManager *manager = portal->getSnapshotManager(); // URGENT
	if(manager == nullptr)
		return jobComplete;

	bool result = manager->update(get_this_ptr());
	
	IJob::JobStatus status = (result) ? jobComplete : jobIncomplete;
	

	if(status == jobComplete)
	{
		// URGENT
		// non vale la pena accodare il job con delay? è una facile a questo punto
		// k, nel caso fai la differenza dei due TimeDuration e sul risultato chiama "getTotalMilliseconds" da passare al job

		bool alive = Engine::instance()->getAliveSince(TimeDuration::minutes(60));
		if(alive)
		{
			DateTime lastOptimization = portal->getOptions()->getLastOptimizationDate();
			bool needRun = ( (lastOptimization.isValid() == false) || ( (lastOptimization + TimeDuration(30,0,0)) < DateTime::now() ) );
			if(needRun)
				portal->runJobOptimizer();								
		}
		
		// Search portals that sync with this, and launch Sync Job.
		PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
		for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
		{
			shared_ptr<Portal> portal2 = *i;
			if(portal2->getSync() == portal->getFullPovID())
				portal2->runJobSync();				
		}
		
	}
	return status;
}

shared_ptr<IBackgroundJob::Details> PortalsStabilityJob::getDetails() const
{
	String jobName(Engine::instance()->getText("job.stability"));

	shared_ptr<Portal> portal = getPortal();
	if(portal != nullptr)
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
