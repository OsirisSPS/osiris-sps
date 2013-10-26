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

#include "boost/bind.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "condition.h"
#include "platformmanager.h"
#include "threadsgroup.h"
#include "workerthread.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static void worker_thread_proc1(shared_ptr<Thread> thread, Condition *c)
{
	BOOST_CHECK(thread->running());
	BOOST_CHECK(thread->stopped() == false);
	BOOST_CHECK(PlatformManager::instance()->compareThreadsID(thread->getID(), PlatformManager::instance()->getCurrentThreadID()));
	
	c->reset();

	while(thread->stopped() == false)
		thread->yield();

	BOOST_CHECK(thread->running() == false);
	BOOST_CHECK(thread->stopped());
}

BOOST_AUTO_TEST_CASE(test_worker_thread1)
{
	Condition c;

	shared_ptr<WorkerThread> thread(new WorkerThread(boost::bind(worker_thread_proc1, _1, &c)));

	BOOST_CHECK(thread->running() == false);
	thread->stop();
	BOOST_CHECK(thread->running() == false);

	thread->start();
	c.wait();
	thread->stop();
}

static void worker_thread_proc2(shared_ptr<Thread> thread, Condition *c)
{
	c->reset();

	while(thread->stopped() == false)
		thread->yield();
}

BOOST_AUTO_TEST_CASE(test_worker_thread2)
{
	Condition c;

	shared_ptr<WorkerThread> thread(new WorkerThread(boost::bind(worker_thread_proc2, _1, &c)));
	thread->join();

	thread->start();
	c.wait();
	thread->stop();

	thread->join();
}

static void threads_group_proc1(shared_ptr<Thread> thread, bool signal, Condition *c)
{
	if(signal)
	{
		c->reset();
		return;
	}

	while(thread->stopped() == false)
	{
		thread->yield();		
	}
}

BOOST_AUTO_TEST_CASE(test_threads_group1)
{
	Condition c;

	uint32 size = 20;

	ThreadsGroup<WorkerThread> threads;
	BOOST_CHECK(threads.empty());
	BOOST_CHECK(threads.size() == 0);
	for(uint32 i = 0; i < size; i++)
	{
		threads.add(shared_ptr<WorkerThread>(new WorkerThread(boost::bind(threads_group_proc1, _1, i == (size / 2), &c))), false);
	}
	BOOST_CHECK(threads.empty() == false);
	BOOST_CHECK(threads.size() == size);

	threads.start();
	c.wait();
	threads.stop();

	BOOST_CHECK(threads.empty() == false);
	BOOST_CHECK(threads.size() == size);
	
	threads.clear();

	BOOST_CHECK(threads.empty());
	BOOST_CHECK(threads.size() == 0);
}

static void worker_threads_proc2(shared_ptr<Thread> self, Condition *c, shared_ptr<Thread> prev, bool run)
{
	c->wait();

	if(prev != null)
	{
		prev->getID();		
		prev->stop();
		prev->getPriority();
	}

	if(run == false)
		return;
	
	while(self->stopped() == false)
	{
		self->yield();		
	}
}

BOOST_AUTO_TEST_CASE(test_threads_group2)
{
	Condition c;

	uint32 size = 20;

	ThreadsGroup<WorkerThread> threads;
	for(uint32 i = 0; i < size; i++)
	{
		threads.add(shared_ptr<WorkerThread>(new WorkerThread(boost::bind(worker_threads_proc2, _1, &c, i == 0 ? null : threads.at(i - 1), i < (size - 1)))), false);
	}

	threads.start();
	c.reset();
	threads.join();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
