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

#ifndef _OS_CORE_EVENTSOURCE_H
#define _OS_CORE_EVENTSOURCE_H

#include "base/object.h"
#include "boost/noncopyable.hpp"
#include "boost/thread/mutex.hpp"
#include "eventcallback.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IEvent;
class IEventCallback;

//////////////////////////////////////////////////////////////////////

class CoreExport EventSource : public Object,
							   public boost::noncopyable,
							   public enable_this_ptr<EventSource>
{
	typedef list<shared_ptr<IEventCallback> >::type Callbacks;

// Construction
public:
	EventSource();
	virtual ~EventSource();

// Attributes
public:
	bool empty() const;

// Operations
public:
	// Connette un segnale all'evento
	shared_ptr<IEventCallback> connect(shared_ptr<IEventCallback> callback);
	template <typename T>
	shared_ptr<IEventCallback> connect(const T &callback);

	// Disconnette un segnale dall'evento
	void disconnect(shared_ptr<IEventCallback> callback);

	// Rimuove tutti i segnali agganciati all'evento
	void clear();

// Overridables
public:
	virtual void fire(IEvent *e = nullptr);

private:
	mutable boost::mutex m_cs;
	Callbacks m_callbacks;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
shared_ptr<IEventCallback> EventSource::connect(const T &callback)
{
	return connect(shared_ptr<IEventCallback>(OS_NEW EventCallback(callback)));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_EVENTSOURCE_H
