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
#include "schedulablethread.h"

#include "ijob.h"
#include "lock.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OS_STATIC_ASSERT(taskPriorityIdle < taskPriorityCritical);		// L'ordinamento dei jobs prevede questa condizione, vedere SchedulableThread::addJob

//////////////////////////////////////////////////////////////////////

SchedulableThread::SchedulableThread()
{

}

SchedulableThread::~SchedulableThread()
{
	stopJobs(true);
}

uint32 SchedulableThread::getJobsCount() const
{
	OS_LOCK(m_jobsCS);
	return static_cast<uint32>(m_jobs.size());
}

void SchedulableThread::getJobs(Jobs &jobs) const
{
	OS_LOCK(m_jobsCS);
	utils::copy(jobs, m_jobs);
}

shared_ptr<IJob> SchedulableThread::getJob(uint32 id) const
{
	OS_LOCK(m_jobsCS);

	for(Jobs::const_iterator i = m_jobs.begin(); i != m_jobs.end(); ++i)
	{
		if((*i)->getID() == id)
			return *i;
	}

	return null;
}

bool SchedulableThread::hasJobs() const
{
	OS_LOCK(m_jobsCS);
	return m_jobs.empty() == false;
}

uint32 SchedulableThread::addJob(shared_ptr<IJob> job)
{
	OS_LOCK(m_jobsCS);

	OS_ASSERT(job != null);
	if(job != null)
	{
		// N.B.: a parità di priorità il nuovo job deve essere successivo a quelli accodati in precedenza

		Jobs::iterator i = m_jobs.begin();		
		while(i != m_jobs.end())
		{
			if(job->getPriority() > (*i)->getPriority())
			{
				i = m_jobs.insert(i, job);
				break;
			}

			++i;
		}		

		if(i == m_jobs.end())
			m_jobs.push_back(job);

		OS_ASSERT(utils::exists(m_jobs, job));

		// Segnala l'evento di un nuovo job
		m_jobEvent.signal();
	}

	return static_cast<uint32>(m_jobs.size());
}

void SchedulableThread::transferJobs(Jobs &jobs)
{
	// N.B.: è necessario che il thread sia fermo per trasferire con sicurezza tutti i jobs correnti (vedi peekJob), quindi
	// se il thread è attivo viene fermato ed in seguito riavviato dopo aver copiato i jobs

	OS_LOCK(m_jobsCS);

	jobs.clear();

	// Salva lo stato del thread
	bool restore = running();
	if(restore)
		stop();	// Ferma il thread se attivo

	// Copia tutti i jobs attivi
	while(m_jobs.empty() == false)
	{
		jobs.push_front(m_jobs.back());
		m_jobs.pop_back();
	}

	if(restore)
		start();	// Riavvia il thread se necessario
}

bool SchedulableThread::pollNextJob()
{
	// Esegue il task successivo senza timeout
	return executeNextJob(0);
}

bool SchedulableThread::executeNextJob(uint32 timeout)
{
	// Attende un nuovo job da eseguire
	shared_ptr<IJob> job = peekJob(timeout);
	if(job == null)
		return false;

	// Esegue il job
	executeJob(job);

	return true;
}

void SchedulableThread::stopJobs(bool clear)
{
	OS_LOCK(m_jobsCS);

	for(Jobs::const_iterator i = m_jobs.begin(); i != m_jobs.end(); (*i)->stop(), ++i);

	if(clear)
		m_jobs.clear();
}

shared_ptr<IJob> SchedulableThread::peekJob(uint32 timeout)
{
	if(timeout != 0)
	{
		{
			OS_LOCK(m_jobsCS);			

			for(Jobs::const_iterator i = m_jobs.begin(); i != m_jobs.end() && timeout != 0; ++i)
			{
				shared_ptr<IJob> job = *i;
				
				uint32 jobTimeout = 0;
				if(job->getRunnable(&jobTimeout))
					return job;
				else
					timeout = utils::min(timeout, jobTimeout);				
			}
		}

		// N.B.: non loccare qui m_jobsCS altrimenti non è possibile accodare dall'esterno un nuovo job durante la wait

		if(timeout != 0)
		{
			m_jobEvent.wait(timeout);
			// Verifica che il thread non sia stato fermato durante l'attesa (unica condizione per cui m_jobEvent.isValid() possa essere "false")
			if(m_jobEvent.isValid() == false)
				return null;
		}
	}

	{
		// E' necessario scorrere nuovamente i job perchè durante la wait precedente potrebbe essere stato accodato un job con priorità maggiore

		OS_LOCK(m_jobsCS);

		for(Jobs::const_iterator i = m_jobs.begin(); i != m_jobs.end(); ++i)
		{
			shared_ptr<IJob> job = *i;
			if(job->getRunnable())
				return job;
		}
	}

    return null;
}

void SchedulableThread::executeJob(shared_ptr<IJob> job)
{
	if(job == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	// Esegue il job
	switch(job->start())
	{
	case IJob::jobIncomplete:	// Riordina il job perchè la priorità potrebbe essere cambiata durante l'esecuzione e
								// per garantire che a parità di priorità sia riposizionata in ultima posizione
								reorderJob(job);
								break;

	case IJob::jobComplete:		// Richiama l'evento di job completato
								onJobCompleted(job);
								break;

	default:					OS_ASSERTFALSE();
								break;
	}
}

void SchedulableThread::reorderJob(shared_ptr<IJob> job)
{
	// N.B.: la lock deve essere nello stesso scope di "addJob" altrimenti si rischia che dall'esterno una test su "hasJobs"
	// possa restituire un valore errato in fase di riordinamento del job
	OS_LOCK(m_jobsCS);

    OS_ASSERT(utils::exists(m_jobs, job));
    utils::remove(m_jobs, job);
    
    addJob(job);
}

void SchedulableThread::onJobCompleted(shared_ptr<IJob> job)
{
    OS_LOCK(m_jobsCS);

	OS_ASSERT(utils::exists(m_jobs, job));
    utils::remove(m_jobs, job);
}

void SchedulableThread::onIdle()
{

}

void SchedulableThread::stop()
{
	// N.B.: i job del thread non devono essere qui cancellati ma solo fermati a seguito, ad esempio,
	// ad un riavvio del thread che in caso contrario si perderebbe i job accodati in precedenza
	stopJobs(false);

	// N.B.: se si modifica il criterio con cui l'evento viene resettato, ricontrollare il metodo "peekJob"
	m_jobEvent.reset();

	ThreadBase::stop();
}

void SchedulableThread::run()
{	
	// Assicura che l'evento sia inizializzato
	m_jobEvent.create();

	while(stopped() == false)
	{
		// Attende ed esegue il job successivo
		if(executeNextJob() == false)
		{
			// Se non è stato eseguito alcun job richiama l'evento di idle
			onIdle();
		}
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
