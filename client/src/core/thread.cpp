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
#include "thread.h"

#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "boostthread.h"
#include "lock.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<Thread> : public Object
{
// Internal
public:
	enum InternalStatus
	{
		isReady	  = 0,		// Il thread è pronto per essere eseguito
		isRunning = 1,		// Il thread è in esecuzione
		isStopped = 2,		// Il thread sta per essere terminato
	};

// Construction
public:
	pimpl();
	virtual ~pimpl();

// Attributes
public:
	bool ready() const;
	bool running() const;
	bool stopped() const;

	boost::any getID() const;

	TaskPriority getPriority() const;
	void setPriority(TaskPriority priority);

// Operations
public:
	bool start(boost::function<void ()> runCallback);
	void stop();

	void join();
	static void join(const boost::any &id, shared_ptr<boost::thread> thread);

	bool enterThread();
	void leaveThread();

private:
	mutable boost::recursive_mutex m_cs;
	boost::any m_id;
	TaskPriority m_priority;
	InternalStatus m_status;
	shared_ptr<boost::thread> m_thread;
};

//////////////////////////////////////////////////////////////////////

pimpl<Thread>::pimpl() : m_priority(taskPriorityDefault),
						 m_status(isReady)
{

}

pimpl<Thread>::~pimpl()
{

}

bool pimpl<Thread>::ready() const
{
	OS_LOCK(m_cs);
	return m_status == isReady;
}

bool pimpl<Thread>::running() const
{
	OS_LOCK(m_cs);
	return m_status == isRunning;
}

bool pimpl<Thread>::stopped() const
{
	OS_LOCK(m_cs);
	return m_status == isStopped;
}

boost::any pimpl<Thread>::getID() const
{
	OS_LOCK(m_cs);
	return m_id;
}

TaskPriority pimpl<Thread>::getPriority() const
{
	OS_LOCK(m_cs);
	return m_priority;
}

void pimpl<Thread>::setPriority(TaskPriority priority)
{
	OS_LOCK(m_cs);

	OS_ASSERT(m_status != isRunning);  // Per portabilità la priorità del thread può essere cambiata solo in fase di esecuzione dal thread stesso
	m_priority = priority;
}

bool pimpl<Thread>::start(boost::function<void ()> runCallback)
{
	OS_LOCK(m_cs);

	if(m_status != isReady)
		return false;

	m_status = isRunning;
	m_thread.reset(OS_NEW_T(boost::thread)(runCallback), os_delete_t());

	return true;
}

void pimpl<Thread>::stop()
{
	boost::any id;
	shared_ptr<boost::thread> thread;

	{
		OS_LOCK(m_cs);

		if(m_status != isRunning)
			return;

		m_status = isStopped;

		OS_ASSERT(m_thread != null);

		id = m_id;
		thread = m_thread;
	}

	join(id, thread);
}

void pimpl<Thread>::join()
{
	boost::any id;
	shared_ptr<boost::thread> thread;

	{
		OS_LOCK(m_cs);

		id = m_id;
		thread = m_thread;
	}

	join(id, thread);
}

void pimpl<Thread>::join(const boost::any &id, shared_ptr<boost::thread> thread)
{
	if(thread == null)
		return;

	// N.B.: la terminazione di un thread deve essere effettuata da un thread esterno altrimenti la join non terminerebbe mai...
	// Esendo legale la join di un thread non in esecuzione va verificato che l'id specificato sia valido
	OS_ASSERT(id.empty() || (PlatformManager::instance()->compareThreadsID(id, PlatformManager::instance()->getCurrentThreadID()) == false));

	thread->join();
}

bool pimpl<Thread>::enterThread()
{
	OS_LOCK(m_cs);

	// Salva l'id del thread corrente
	m_id = PlatformManager::instance()->getCurrentThreadID();
	// Controlla se la priorità del thread non è quella di default
	if(m_priority != taskPriorityDefault)
	{
		// Aggiorna la priorità del thread prima di richiamare l'implementazione
		if(PlatformManager::instance()->setThreadPriority(m_id, m_priority) == false)
		{
			//OS_ASSERTFALSE();
		}
	}

	return true;
}

void pimpl<Thread>::leaveThread()
{
	OS_LOCK(m_cs);

	m_id = boost::any();
	m_status = isReady;
}

//////////////////////////////////////////////////////////////////////

Thread::Thread()
{

}

Thread::~Thread()
{
	// La stop dovrebbe essere chiamata dall'esterno per consentire la chiamata di evenutali funzioni virtuali
	OS_ASSERT(running() == false);
}

bool Thread::ready() const
{
	return m_impl->ready();
}

bool Thread::running() const
{
	return m_impl->running();
}

bool Thread::stopped() const
{
	return m_impl->stopped();
}

boost::any Thread::getID() const
{
	return m_impl->getID();
}

TaskPriority Thread::getPriority() const
{
	return m_impl->getPriority();
}

void Thread::setPriority(TaskPriority priority)
{
	m_impl->setPriority(priority);
}

void Thread::join()
{
	m_impl->join();
}

void Thread::sleep(uint32 milliseconds)
{
	OS_ASSERT(PlatformManager::instance()->compareThreadsID(getID(), PlatformManager::instance()->getCurrentThreadID()));
	PlatformManager::instance()->sleep(milliseconds);
}

void Thread::yield()
{
	OS_ASSERT(PlatformManager::instance()->compareThreadsID(getID(), PlatformManager::instance()->getCurrentThreadID()));
	PlatformManager::instance()->yield();
}

void Thread::runCallback()
{
	if(onEnter())
	{
		// Esegue il thread
		run();
	}

	onLeave();
}

bool Thread::start()
{
	return m_impl->start(boost::bind(&Thread::runCallback, this));
}

void Thread::stop()
{
	m_impl->stop();
}

bool Thread::onEnter()
{
	return m_impl->enterThread();
}

void Thread::onLeave()
{
	m_impl->leaveThread();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
