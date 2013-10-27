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
#include "ijob.h"

#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/function.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "condition.h"
#include "lock.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<IJob>
{
// Internal
private:
	class JobRunner
	{
	// Construction
	public:
		JobRunner(pimpl<IJob> *job, IJob::JobStatus status);
		~JobRunner();

	// Operators
	public:
		IJob::JobStatus operator()(boost::function<IJob::JobStatus ()> runCallback);

	private:
		pimpl<IJob> *m_job;
		IJob::JobStatus m_status;
	};

// Enumerated types
public:
	enum InternalStatus
	{
		isReady			= 0,		// Il job è pronto per l'esecuzione
		isRunning		= 1,		// Il job è in esecuzione
		isStopped		= 2,		// Il job è in fase di arresto
	};

// Construction
public:
	pimpl(uint32 id, TaskPriority priority);
	virtual ~pimpl();

// Attributes
public:
	uint32 getID() const;

	TaskPriority getPriority() const;
	void setPriority(TaskPriority priority);

	bool getRunnable(uint32 *timeout) const;

	boost::posix_time::ptime getDelay() const;

	InternalStatus getInternalStatus() const;
	IJob::JobStatus getJobStatus() const;

private:
	void setInternalStatus(InternalStatus status);
	void setJobStatus(IJob::JobStatus status);

public:
	void applyDelay(uint32 delay);

	IJob::JobStatus start(boost::function<IJob::JobStatus ()> runCallback);
	void stop();

protected:
	void cleanup(IJob::JobStatus status);

public:
	Condition m_runEvent;
	mutable boost::recursive_mutex m_cs;
	uint32 m_id;
	TaskPriority m_priority;
	boost::posix_time::ptime m_delay;
	InternalStatus m_internalStatus;
	IJob::JobStatus m_jobStatus;
};

//////////////////////////////////////////////////////////////////////

pimpl<IJob>::JobRunner::JobRunner(pimpl<IJob> *job, IJob::JobStatus status) : m_job(job),
																			  m_status(status)
{
	OS_ASSERT(job != null);
}

pimpl<IJob>::JobRunner::~JobRunner()
{
	m_job->cleanup(m_status);
}

IJob::JobStatus pimpl<IJob>::JobRunner::operator()(boost::function<IJob::JobStatus ()> runCallback)
{
	m_status = runCallback();
	return m_status;
}

//////////////////////////////////////////////////////////////////////

pimpl<IJob>::pimpl(uint32 id, TaskPriority priority) : m_id(id),
													   m_priority(priority),
													   m_delay(boost::date_time::not_a_date_time),
													   m_internalStatus(isReady),
													   m_jobStatus(IJob::jobIncomplete)
{

}

pimpl<IJob>::~pimpl()
{

}

uint32 pimpl<IJob>::getID() const
{
	OS_LOCK(m_cs);
	return m_id;
}

TaskPriority pimpl<IJob>::getPriority() const
{
	OS_LOCK(m_cs);
	return m_priority;
}

void pimpl<IJob>::setPriority(TaskPriority priority)
{
	OS_LOCK(m_cs);
	m_priority = priority;
}

bool pimpl<IJob>::getRunnable(uint32 *timeout) const
{
    OS_LOCK(m_cs);

	if(timeout != null)
		*timeout = 0;

	if(m_delay.is_not_a_date_time())
		return true;

	boost::posix_time::ptime now = IJob::clock();

	if(now >= m_delay)
		return true;

	if(timeout != null)
	{
		OS_ASSERT((m_delay - now).total_milliseconds() == static_cast<uint64>(static_cast<uint32>((m_delay - now).total_milliseconds())));		// Non dovrebbe esserci perdita di dati
		*timeout = static_cast<uint32>((m_delay - now).total_milliseconds());
	}

	return false;
}

boost::posix_time::ptime pimpl<IJob>::getDelay() const
{
    OS_LOCK(m_cs);
    return m_delay;
}

pimpl<IJob>::InternalStatus pimpl<IJob>::getInternalStatus() const
{
	OS_LOCK(m_cs);
	return m_internalStatus;
}

IJob::JobStatus pimpl<IJob>::getJobStatus() const
{
	OS_LOCK(m_cs);
	return m_jobStatus;
}

void pimpl<IJob>::setInternalStatus(InternalStatus status)
{
	OS_LOCK(m_cs);
	m_internalStatus = status;
}

void pimpl<IJob>::setJobStatus(IJob::JobStatus status)
{
	OS_LOCK(m_cs);
	m_jobStatus = status;
}

void pimpl<IJob>::applyDelay(uint32 delay)
{
    OS_LOCK(m_cs);

    if(delay == 0)
        m_delay = boost::posix_time::ptime(boost::date_time::not_a_date_time);
    else
        m_delay = IJob::clock() + boost::posix_time::millisec(delay);
}

IJob::JobStatus pimpl<IJob>::start(boost::function<IJob::JobStatus ()> runCallback)
{
	IJob::JobStatus currentStatus = IJob::jobComplete;	// In caso di eccezione non gestita il job verrà portato a stato completo

	{
		OS_LOCK(m_cs);

		if(m_internalStatus != isReady)
			return m_jobStatus;

		// Porta il job in stato di esecuzione
		m_internalStatus = isRunning;
		// Evita un'ulteriore lock da JobRunner per determinare lo stato corrente
		currentStatus = m_jobStatus;
	}

	m_runEvent.create();
	// N.B.: il callback protrebbe lanciare un'eccezione non gestita, pertanto è necessario assicurare che la funzione di "cleanup" sia comunque richiamata
	return JobRunner(this, currentStatus)(runCallback);
}

void pimpl<IJob>::stop()
{
	{
		OS_LOCK(m_cs);

		if(m_internalStatus != isRunning)
			return;

		m_internalStatus = isStopped;
	}

	m_runEvent.wait();
}

void pimpl<IJob>::cleanup(IJob::JobStatus status)
{
	m_runEvent.reset();

	{
		OS_LOCK(m_cs);

		m_jobStatus = status;
		m_internalStatus = isReady;
	}
}

//////////////////////////////////////////////////////////////////////

IJob::IJob(uint32 id, TaskPriority priority) : m_impl(id, priority)
{

}

IJob::~IJob()
{
	stop();
}

uint32 IJob::getID() const
{
	return m_impl->getID();
}

TaskPriority IJob::getPriority() const
{
	return m_impl->getPriority();
}

void IJob::setPriority(TaskPriority priority)
{
	m_impl->setPriority(priority);
}

bool IJob::getRunnable(uint32 *timeout) const
{
    return m_impl->getRunnable(timeout);
}

boost::posix_time::ptime IJob::getDelay() const
{
    return m_impl->getDelay();
}

IJob::JobStatus IJob::getStatus() const
{
	return m_impl->getJobStatus();
}

bool IJob::ready() const
{
	return m_impl->getInternalStatus() == pimpl<IJob>::isReady;
}

bool IJob::running() const
{
	return m_impl->getInternalStatus() == pimpl<IJob>::isRunning;
}

bool IJob::stopped() const
{
	return m_impl->getInternalStatus() == pimpl<IJob>::isStopped;
}

void IJob::applyDelay(uint32 delay)
{
    m_impl->applyDelay(delay);
}

void IJob::resetDelay()
{
    applyDelay(0);
}

boost::posix_time::ptime IJob::clock()
{
	return utils::utc_time();
}

IJob::JobStatus IJob::start()
{
	return m_impl->start(boost::bind(&IJob::run, this));
}

void IJob::stop()
{
	return m_impl->stop();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
