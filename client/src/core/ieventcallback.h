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

#ifndef _OS_CORE_IEVENTCALLBACK_H
#define _OS_CORE_IEVENTCALLBACK_H

#include "base/object.h"
#include "boost/noncopyable.hpp"
#include "boost/thread/mutex.hpp"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EventSource;
class IEvent;

//////////////////////////////////////////////////////////////////////

class CoreExport IEventCallback : public Object,
								  public boost::noncopyable,
								  public enable_this_ptr<IEventCallback>
{
	typedef list<weak_ptr<EventSource> >::type Sources;

	friend class EventSource;

// Construction
public:
	IEventCallback();
	virtual ~IEventCallback();

// Operations
public:
	void disconnect();

private:
	void attach(shared_ptr<EventSource> source);

// Interface
public:
	virtual void invoke(IEvent *e) = 0;

private:
	mutable boost::mutex m_cs;
	Sources m_sources;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_IEVENTCALLBACK_H
