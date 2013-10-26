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

#ifndef _PORTALSSYNCJOB_H
#define _PORTALSSYNCJOB_H

#include "ibackgroundjob.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport PortalsSyncJob : public IBackgroundJob
{
	typedef IBackgroundJob JobBase;

// Construction
public:
	PortalsSyncJob(uint32 id, shared_ptr<Portal> portal);
	virtual ~PortalsSyncJob();

// Attributes
public:
	inline shared_ptr<Portal> getPortal() const;

// IJob interface
public:
	virtual JobStatus run();

// IBackgroundJob interface
public:
	virtual shared_ptr<Details> getDetails() const;

protected:
	weak_ptr<Portal> m_portal;
	double m_progressPercentage;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<Portal> PortalsSyncJob::getPortal() const { return m_portal.lock(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _PORTALSSYNCJOB_H
