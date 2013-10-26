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

#ifndef _OS_CORE_TASKTHREAD_H
#define _OS_CORE_TASKTHREAD_H

#include "condition.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////

#define OS_TASK_THREAD_DEFAULT_DELAY			100

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport TaskThread : public Thread
{
	typedef Thread ThreadBase;

// Construction
public:
	TaskThread(uint32 delay = OS_TASK_THREAD_DEFAULT_DELAY);
	virtual ~TaskThread();

// IThread overrides
public:
	virtual void stop();

// IThread interface
protected:
	virtual void run();

// Interface
protected:
	// Restituisce true se è stato eseguito un task
	virtual bool executeTask() = 0;

protected:
	uint32 m_delay;
	Condition m_delayCondition;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_TASKTHREAD_H
