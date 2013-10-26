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
#include "jobspool.h"

#include "lock.h"
#include "platformmanager.h"
#include "schedulablethread.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class JobsPool::JobsPoolThread : public SchedulableThread
{
	typedef SchedulableThread ThreadBase;

// Construction
public:
	JobsPoolThread(JobsPool &pool);
	virtual ~JobsPoolThread();

// SchedulableThread overrides
protected:
	virtual void onJobCompleted(shared_ptr<IJob> job);

private:
	JobsPool &m_pool;
};

//////////////////////////////////////////////////////////////////////

JobsPool::JobsPoolThread::JobsPoolThread(JobsPool &pool) : m_pool(pool)
{

}

JobsPool::JobsPoolThread::~JobsPoolThread()
{

}

void JobsPool::JobsPoolThread::onJobCompleted(shared_ptr<IJob> job)
{
	// N.B.: va richiamato prima l'evento sul pool altrimenti la base rimuoverebbe il job dalla lista ed un'eventuale pool->getJob(id) sull'evento restituirebbe null

	m_pool.onJobCompleted(job);

	ThreadBase::onJobCompleted(job);
}

//////////////////////////////////////////////////////////////////////

JobsPool::JobsPool(boost::optional<uint32> threads, TaskPriority priority) : m_priority(priority),
																			 m_pos(0)
{	
	resize(threads ? threads.get() : PlatformManager::instance()->getOptimalHardwareConcurrency());
}

JobsPool::~JobsPool()
{
	stop();
}

TaskPriority JobsPool::getPriority() const
{
	OS_LOCK(m_dataCS);
	return m_priority;
}

void JobsPool::setPriority(TaskPriority priority)
{
	OS_LOCK(m_dataCS);

	m_priority = priority;
	for(WorkerThreads::iterator i = m_threads.begin(); i != m_threads.end(); (*i)->setPriority(priority), ++i);
}

Locked<JobsPool::JobEvent>::unique JobsPool::getJobCompletedEvent() 
{ 
	return Locked<JobsPool::JobEvent>::unique(m_jobCompletedEvent, m_eventsCS); 
}

shared_ptr<IJob> JobsPool::getJob(uint32 id) const
{
	OS_LOCK(m_dataCS);

	for(WorkerThreads::const_iterator i = m_threads.begin(); i != m_threads.end(); ++i)
	{
		shared_ptr<IJob> job = (*i)->getJob(id);
		if(job != null)
			return job;
	}

	return null;
}

void JobsPool::getJobs(Jobs &jobs) const
{
	OS_LOCK(m_dataCS);

	jobs.clear();

	for(WorkerThreads::const_iterator i = m_threads.begin(); i != m_threads.end(); ++i)
	{
		Jobs threadJobs;
		(*i)->getJobs(threadJobs);
		utils::add(jobs, threadJobs);
	}
}

bool JobsPool::hasJobs() const
{
	OS_LOCK(m_dataCS);

	for(WorkerThreads::const_iterator i = m_threads.begin(); i != m_threads.end(); ++i)
	{
		if((*i)->hasJobs())
			return true;
	}

	return false;
}

uint32 JobsPool::addJob(shared_ptr<IJob> job)
{
	OS_LOCK(m_dataCS);

	if(m_threads.empty())
	{
		OS_ASSERTFALSE();
		return 0;
	}

	size_t threadsCount = m_threads.size();
	if(m_pos > (threadsCount - 1))
		m_pos = 0;

	OS_ASSERT(m_pos < threadsCount);

	// Accoda il job al thread corrente salvandone il totale
	uint32 jobsCount = m_threads[m_pos]->addJob(job);
	// Calcola il numero totale di jobs
	for(size_t i = 0; i < threadsCount; i++)
	{
		// Aggiorna il numero totale di jobs se la posizione corrente non è quella già salvata in precedenza

		if(i != m_pos)			
			jobsCount += m_threads[m_pos]->getJobsCount();
	}

	// Aggiorna la posizione corrente
	m_pos++;
	// Restituisce il numero totale di jobs
	return jobsCount;
}

bool JobsPool::pollJobs()
{
	OS_LOCK(m_dataCS);

	bool polled = false;

	size_t threadsCount = m_threads.size();
	for(size_t i = 0; i < threadsCount; i++)
	{
		if(m_threads[i]->pollNextJob())
			polled = true;
	}

	return polled;
}

bool JobsPool::resize(uint32 threads)
{
	OS_LOCK(m_dataCS);

	if(threads == 0)
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(threads == m_threads.size())
		return true;

	if(threads > m_threads.size())
	{
		while(threads > m_threads.size())
		{
			shared_ptr<JobsPoolThread> thread(OS_NEW JobsPoolThread(*this));
			m_threads.push_back(thread);
			thread->setPriority(m_priority);
			thread->start();
		}
	}
	else
	{
		JobsPoolThread::Jobs jobs;
		while(threads < m_threads.size())
		{
			shared_ptr<JobsPoolThread> thread = m_threads.front();
			// Ferma il thread corrente
			thread->stop();

			JobsPoolThread::Jobs threadJobs;
			// Salva i jobs in sospeso del thread corrente
			thread->transferJobs(jobs);
			// Accoda i jobs del thread corrente
			utils::add(threadJobs, jobs);

			// Rimuove il thread corrente
			m_threads.pop_front();
		}

		// Distribuisce i jobs non ancora processati agli altri threads

		while(jobs.empty() == false)
		{
			addJob(jobs.front());
			jobs.pop_front();
		}
	}

	return true;
}

void JobsPool::join()
{
	OS_LOCK(m_dataCS);

	for(WorkerThreads::const_iterator i = m_threads.begin(); i != m_threads.end(); (*i)->join(), ++i);
}

void JobsPool::stop()
{
	OS_LOCK(m_dataCS);

	for(WorkerThreads::const_iterator i = m_threads.begin(); i != m_threads.end(); (*i)->stop(), ++i);
}

void JobsPool::onJobCompleted(shared_ptr<IJob> job)
{
	OS_LOCK(m_eventsCS);
	m_jobCompletedEvent(job);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
