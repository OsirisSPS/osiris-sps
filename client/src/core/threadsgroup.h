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

#ifndef _OS_CORE_THREADSGROUP_H
#define _OS_CORE_THREADSGROUP_H

#include "boost/thread/recursive_mutex.hpp"
#include "lock.h"
#include "threads.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, typename C = typename deque<shared_ptr<T> >::type>
class ThreadsGroup
{
	typedef C Threads;

// Construction
public:
	ThreadsGroup(TaskPriority priority = taskPriorityDefault);
	virtual ~ThreadsGroup();

// Attributes
public:
	shared_ptr<T> at(const typename C::size_type &index) const;

	bool empty() const;
	typename C::size_type size() const;

	TaskPriority getPriority() const;
	void setPriority(TaskPriority priority);

// Operations
public:
	// Accoda un thread all'elenco
	void add(shared_ptr<T> thread, bool run);
	// Avvia tutti i threads
	void start();
	// Ferma tutti i threads
	void stop();
	// Attende tutti i threads
	void join();
	// Resetta tutti i threads della lista
	void clear();

protected:
	mutable boost::recursive_mutex m_cs;
	TaskPriority m_priority;
	Threads m_threads;
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename C>
ThreadsGroup<T, C>::ThreadsGroup(TaskPriority priority) : m_priority(priority)
{

}

template <typename T, typename C>
ThreadsGroup<T, C>::~ThreadsGroup()
{

}

template <typename T, typename C>
shared_ptr<T> ThreadsGroup<T, C>::at(const typename C::size_type &index) const
{
	OS_LOCK(m_cs);

	OS_ASSERT(index < m_threads.size());
	return m_threads[index];
}

template <typename T, typename C>
bool ThreadsGroup<T, C>::empty() const
{
	OS_LOCK(m_cs);
	return m_threads.empty();
}

template <typename T, typename C>
typename C::size_type ThreadsGroup<T, C>::size() const
{
	OS_LOCK(m_cs);
	return m_threads.size();
}

template <typename T, typename C>
TaskPriority ThreadsGroup<T, C>::getPriority() const
{
	OS_LOCK(m_cs);
	return m_priority;
}

template <typename T, typename C>
void ThreadsGroup<T, C>::setPriority(TaskPriority priority)
{
	OS_LOCK(m_cs);

	m_priority = priority;
	for(typename Threads::iterator i = m_threads.begin(); i != m_threads.end(); (*i)->setPriority(priority), ++i);
}

template <typename T, typename C>
void ThreadsGroup<T, C>::add(shared_ptr<T> thread, bool run)
{
	if(thread == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	{
		OS_LOCK(m_cs);

		thread->setPriority(m_priority);
		m_threads.push_back(thread);
	}

	if(run)
		thread->start();
}

template <typename T, typename C>
void ThreadsGroup<T, C>::start()
{
	OS_LOCK(m_cs);
	for(typename Threads::const_iterator i = m_threads.begin(); i != m_threads.end(); (*i)->start(), ++i);
}

template <typename T, typename C>
void ThreadsGroup<T, C>::stop()
{
	OS_LOCK(m_cs);
	for(typename Threads::const_iterator i = m_threads.begin(); i != m_threads.end(); (*i)->stop(), ++i);
}

template <typename T, typename C>
void ThreadsGroup<T, C>::join()
{
	OS_LOCK(m_cs);
	for(typename Threads::const_iterator i = m_threads.begin(); i != m_threads.end(); (*i)->join(), ++i);
}

template <typename T, typename C>
void ThreadsGroup<T, C>::clear()
{
	OS_LOCK(m_cs);
	m_threads.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_THREADSGROUP_H
