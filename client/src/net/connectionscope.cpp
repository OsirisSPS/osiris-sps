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
#include "connectionscope.h"

#include "boost/bind.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ConnectionScope::IListener::IListener()
{

}

ConnectionScope::IListener::~IListener()
{

}

void ConnectionScope::IListener::handleTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{
	// Verifica che l'operazione non sia stata annullata
	if(isAsioOperationAborted(e))
		return;

	// Richiama l'evento di timeout
	onTimeout(e, scope);
}

//////////////////////////////////////////////////////////////////////

ConnectionScope::ConnectionScope(shared_ptr<boost::asio::io_service> service, shared_ptr<IListener> listener) : m_listener(listener)
{
	OS_ASSERT(service != nullptr);
	m_timer = createAsioObject<boost::asio::deadline_timer>(*service);

	// N.B.: non è possibile qui chiamare una extendTimeout a causa della get_this_ptr che fa internamente		
}

ConnectionScope::~ConnectionScope()
{
	cancelTimeout();

	shared_ptr<IListener> listener = getListener();
	if(listener != nullptr)
		listener->onFinalize();
}

void ConnectionScope::cancelTimeout()
{
	boost::system::error_code error;
	m_timer->cancel(error);
}

shared_ptr<ConnectionScope> ConnectionScope::extendTimeout(const boost::posix_time::time_duration &timeout)
{
	cancelTimeout();

	shared_ptr<ConnectionScope> scope = get_this_ptr();

	m_timeout = timeout;
	if(m_timeout.total_milliseconds() > 0)
	{
		shared_ptr<IListener> listener = getListener();
		OS_ASSERT(listener != nullptr);
		if(listener != nullptr)
		{
			m_timer->expires_from_now(m_timeout);
			// N.B.: il callback deve essere effettuato sul listener e non sullo scope altrimenti si rischia che venga richiamato dopo che lo scope sia stato distrutto
			m_timer->async_wait(boost::bind(&IListener::handleTimeout, listener, boost::asio::placeholders::error, scope));
		}
	}

	return scope;
}

shared_ptr<ConnectionScope> ConnectionScope::extendTimeout()
{
	OS_ASSERT(m_timeout.total_milliseconds() > 0);
	return extendTimeout(m_timeout);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
