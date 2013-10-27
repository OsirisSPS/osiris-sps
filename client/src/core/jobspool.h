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

#ifndef _OS_CORE_JOBSPOOL_H
#define _OS_CORE_JOBSPOOL_H

#include "base/object.h"
#include "boost/signals.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "enable_this_ptr.h"
#include "locked.h"
#include "threads.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IJob;

//////////////////////////////////////////////////////////////////////

class CoreExport JobsPool : public enable_this_ptr<JobsPool>,
							public Object
{
	class JobsPoolThread;
	typedef deque<shared_ptr<JobsPoolThread> >::type WorkerThreads;	

public:
	typedef deque<shared_ptr<IJob> >::type Jobs;
	typedef boost::signal<void(shared_ptr<IJob>)> JobEvent;	

// Construction
public:
	JobsPool(boost::optional<uint32> threads = boost::none, TaskPriority priority = taskPriorityDefault);
	virtual ~JobsPool();

// Attributes
public:
	TaskPriority getPriority() const;
	void setPriority(TaskPriority priority);

	Locked<JobEvent>::unique getJobCompletedEvent();

	shared_ptr<IJob> getJob(uint32 id) const;
	void getJobs(Jobs &jobs) const;
	bool hasJobs() const;

// Operations
public:
	// Accoda un nuovo job restituendo il totale corrente di tutto il pool
	uint32 addJob(shared_ptr<IJob> job);
	// Effettua il poll di tutti i threads nel pool restituendo true se almeno uno dei thread ha terminato un job
	bool pollJobs();	
	// Ridimensiona il pool di threads
	bool resize(uint32 threads);	
	
	void join();
	void stop();	

// Overridables
protected:
	virtual void onJobCompleted(shared_ptr<IJob> job);

private:
	TaskPriority m_priority;
	size_t m_pos;
	WorkerThreads m_threads;
	mutable boost::recursive_mutex m_dataCS;
	boost::recursive_mutex m_eventsCS;
	JobEvent m_jobCompletedEvent;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_JOBSPOOL_H
