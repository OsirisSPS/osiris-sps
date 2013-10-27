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

#ifndef _OS_CORE_EVENTCALLBACK_H
#define _OS_CORE_EVENTCALLBACK_H

#include "boost/function.hpp"
#include "ieventcallback.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport EventCallback : public IEventCallback
{	
// Construction
public:
	template <typename C>
	EventCallback(const C &callback);
	virtual ~EventCallback();

// IEventCallback interface
protected:
	virtual void invoke(IEvent *e);

protected:
	boost::function<void (IEvent *)> m_callback;
};

//////////////////////////////////////////////////////////////////////

template <typename C>
EventCallback::EventCallback(const C &callback) : m_callback(callback)
{
	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_EVENTCALLBACK_H
