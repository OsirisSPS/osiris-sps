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
#include "eventsource.h"

#include "ieventcallback.h"
#include "lock.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

EventSource::EventSource()
{

}

EventSource::~EventSource()
{
	clear();
}

bool EventSource::empty() const 
{ 
	OS_LOCK(m_cs);
	return m_callbacks.empty(); 
}

shared_ptr<IEventCallback> EventSource::connect(shared_ptr<IEventCallback> callback)
{
	OS_ASSERT(callback != nullptr);
	if(callback != nullptr)
	{
		OS_LOCK(m_cs);

		OS_ASSERT(utils::exists(m_callbacks, callback) == false);
		m_callbacks.push_back(callback);
		callback->attach(get_this_ptr());
	}

	return callback;
}

void EventSource::disconnect(shared_ptr<IEventCallback> callback)
{
	OS_LOCK(m_cs);
	utils::remove(m_callbacks, callback);
}

void EventSource::clear()
{
	OS_LOCK(m_cs);
	m_callbacks.clear();
}

void EventSource::fire(IEvent *e)
{
	OS_LOCK(m_cs);
	for(Callbacks::const_iterator i = m_callbacks.begin(); i != m_callbacks.end(); (*i)->invoke(e), ++i);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
