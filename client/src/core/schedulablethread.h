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

#ifndef _OS_CORE_SCHEDULABLETHREAD_H
#define _OS_CORE_SCHEDULABLETHREAD_H

#include "boost/thread/recursive_mutex.hpp"
#include "condition.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////

#define OS_SCHEDULABLETHREAD_DEFAULT_TIMEOUT			1000

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IJob;

//////////////////////////////////////////////////////////////////////

class CoreExport SchedulableThread : public Thread
{
	typedef Thread ThreadBase;

public:
	typedef deque<shared_ptr<IJob> >::type Jobs;

// Construction
public:
	SchedulableThread();
	virtual ~SchedulableThread();

// Attributes
public:
	// Restituisce il numero di jobs da eseguire
	uint32 getJobsCount() const;
	// Restituisce il job specificato
	shared_ptr<IJob> getJob(uint32 id) const;
	// Restituisce l'elenco dei jobs attivi
	void getJobs(Jobs &jobs) const;
	// Restituisce true se il thread ha dei job da eseguire
	bool hasJobs() const;

// Operations
public:
	// Accoda un nuovo job al thread restituendo il numero di task in attesa di essere processati
	uint32 addJob(shared_ptr<IJob> job);
	// Rimuove i job attualmente accodati (richiede la sospensione del thread se attivo)
	void transferJobs(Jobs &jobs);

	// Se il thread ha tasks da eseguire processa il successivo, altrimenti restituisce subito il controllo (restituisce true se è stato eseguito un task)
	bool pollNextJob();
	// Se il thread ha tasks da eseguire processa il successivo, altrimenti attende per "timeout" tempo un nuovo task dopodichè restituisce il controllo (restituisce true se è stato eseguito un task)
	bool executeNextJob(uint32 timeout = OS_SCHEDULABLETHREAD_DEFAULT_TIMEOUT);

private:
	void stopJobs(bool clear = false);

	shared_ptr<IJob> peekJob(uint32 timeout);
	void executeJob(shared_ptr<IJob> job);

    void reorderJob(shared_ptr<IJob> job);

// Overridables
protected:
	virtual void onJobCompleted(shared_ptr<IJob> job);
	virtual void onIdle();

// Thread overrides
public:
	virtual void stop();

// Thread interface
protected:
	virtual void run();

protected:
	Jobs m_jobs;
	Condition m_jobEvent;
	mutable boost::recursive_mutex m_jobsCS;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_SCHEDULABLETHREAD_H
