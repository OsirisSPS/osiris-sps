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

#ifndef _OS_CORE_EVENTSSOURCE_H
#define _OS_CORE_EVENTSSOURCE_H

#include "base/object.h"
#include "boost/noncopyable.hpp"
#include "boost/thread/mutex.hpp"
#include "eventsource.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EventSource;

//////////////////////////////////////////////////////////////////////

template <typename K, typename E>
class EventsSource : public Object,
					 public boost::noncopyable
{
	typedef K Key;
	typedef E Event;

	typedef typename unordered_map<Key, shared_ptr<EventSource> >::type Sources;

// Construction
public:
	EventsSource();
	virtual ~EventsSource();

// Attributes
public:
	bool exists(const Key &key) const;
	shared_ptr<EventSource> get(const Key &key) const;

// Operations
private:
	shared_ptr<EventSource> find(const Key &key, bool create) const;

// Overridables
public:
	virtual void fire(const Key &key, Event *e);

protected:
	mutable boost::mutex m_cs;
	mutable Sources m_sources;
};

//////////////////////////////////////////////////////////////////////

template <typename K, typename E>
EventsSource<K, E>::EventsSource()
{
	
}

template <typename K, typename E>
EventsSource<K, E>::~EventsSource()
{
	
}

template <typename K, typename E>
bool EventsSource<K, E>::exists(const Key &key) const
{
	OS_LOCK(m_cs);

	return m_sources.find(key) != m_sources.end();
}

template <typename K, typename E>
shared_ptr<EventSource> EventsSource<K, E>::get(const Key &key) const
{
	return find(key, true);
}

template <typename K, typename E>
shared_ptr<EventSource> EventsSource<K, E>::find(const Key &key, bool create) const
{
	OS_LOCK(m_cs);

	typename Sources::iterator i = m_sources.find(key);
	if(i != m_sources.end())
		return i->second;	

	shared_ptr<EventSource> source;
	if(create)
	{
		source.reset(OS_NEW EventSource());
		m_sources[key] = source;
	}

	return source;
}

template <typename K, typename E>
void EventsSource<K, E>::fire(const Key &key, Event *e)
{
	shared_ptr<EventSource> source = find(key, false);
	if(source != nullptr)
		source->fire(e);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_EVENTSSOURCE_H
