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

#ifndef _OS_CORE_JOBCALLBACK_H
#define _OS_CORE_JOBCALLBACK_H

#include "boost/function.hpp"
#include "ijob.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T = boost::function<void ()>, typename J = IJob>
class JobCallback : public J
{
	typedef J JobBase;

public:
	typedef boost::function<void ()> Callback;
	
// Construction
public:
	JobCallback(const Callback &callback, uint32 id, TaskPriority priority = taskPriorityDefault);
	template <typename P1>
	JobCallback(P1 p1, const Callback &callback, uint32 id, TaskPriority priority = taskPriorityDefault);
	template <typename P1, typename P2>
	JobCallback(P1 p1, P2 p2, const Callback &callback, uint32 id, TaskPriority priority = taskPriorityDefault);
	template <typename P1, typename P2, typename P3>
	JobCallback(P1 p1, P2 p2, P3 p3, const Callback &callback, uint32 id, TaskPriority priority = taskPriorityDefault);
	virtual ~JobCallback();

// IJob interface
protected:
	virtual typename JobBase::JobStatus run();

private:
	Callback m_callback;
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename J>
JobCallback<T, J>::JobCallback(const Callback &callback, uint32 id, TaskPriority priority) : JobBase(id, priority),
																							 m_callback(callback)
{

}

template <typename T, typename J>
template <typename P1>
JobCallback<T, J>::JobCallback(P1 p1, const Callback &callback, uint32 id, TaskPriority priority) : JobBase(p1, id, priority),
																								    m_callback(callback)
{

}

template <typename T, typename J>
template <typename P1, typename P2>
JobCallback<T, J>::JobCallback(P1 p1, P2 p2, const Callback &callback, uint32 id, TaskPriority priority) : JobBase(p1, p2, id, priority),
																										   m_callback(callback)
{

}

template <typename T, typename J>
template <typename P1, typename P2, typename P3>
JobCallback<T, J>::JobCallback(P1 p1, P2 p2, P3 p3, const Callback &callback, uint32 id, TaskPriority priority) : JobBase(p1, p2, p3, id, priority),
																												  m_callback(callback)
{

}

template <typename T, typename J>
JobCallback<T, J>::~JobCallback()
{

}

template <typename T, typename J>
typename JobCallback<T, J>::JobBase::JobStatus JobCallback<T, J>::run()
{
    m_callback();
    return JobBase::jobComplete;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_JOBCALLBACK_H
