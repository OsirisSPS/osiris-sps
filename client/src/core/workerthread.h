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

#ifndef _OS_CORE_WORKERTHREAD_H
#define _OS_CORE_WORKERTHREAD_H

#include "boost/function.hpp"
#include "enable_this_ptr.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport WorkerThread : public Thread,
								public enable_this_ptr<WorkerThread>
{
	typedef boost::function<void (shared_ptr<Thread>)> Callback;

// Construction
public:
	WorkerThread();
	WorkerThread(const Callback &callback);
	virtual ~WorkerThread();

// Attributes
public:
	inline void setCallback(const Callback &callback);

// Thread interface
protected:
	virtual void run();

private:
	Callback m_callback;
};

//////////////////////////////////////////////////////////////////////

inline void WorkerThread::setCallback(const Callback &callback) { m_callback = callback; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

// Samples

/* 
void callback1(shared_ptr<Thread> thread, int param)
{
	
}

void callback2()
{
	
}

void runThreads()
{
	shared_ptr<Thread> thread1(OS_NEW WorkerThread(boost::bind(&callback1, _1, 100)));
	thread1->start();

	shared_ptr<Thread> thread2(OS_NEW WorkerThread(boost::bind(&callback2)));
	thread2->start();	
}
*/

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_WORKERTHREAD_H
