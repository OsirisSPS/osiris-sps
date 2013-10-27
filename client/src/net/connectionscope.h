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

#ifndef _OS_NET_CONNECTIONSCOPE_H
#define _OS_NET_CONNECTIONSCOPE_H

#include "base/object.h"
#include "boostasio.h"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class NetExport ConnectionScope : public enable_this_ptr<ConnectionScope>,
								  public Object
{
public:
	class NetExport IListener
	{
		friend class ConnectionScope;

	// Construction
	public:
		IListener();
		virtual ~IListener();

	// Interface
	protected:
		virtual void onTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope) = 0;
		virtual void onFinalize() = 0;

	private:
		void handleTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope);
	};

// Construction
public:
	ConnectionScope(shared_ptr<boost::asio::io_service> service, shared_ptr<IListener> listener);
	virtual ~ConnectionScope();

// Attributes
public:
	inline shared_ptr<IListener> getListener() const;
	inline const boost::posix_time::time_duration & getTimeout() const;

// Operations
public:
	void cancelTimeout();

	shared_ptr<ConnectionScope> extendTimeout(const boost::posix_time::time_duration &duration);
	shared_ptr<ConnectionScope> extendTimeout();	

private:
	weak_ptr<IListener> m_listener;
	boost::posix_time::time_duration m_timeout;
	shared_ptr<boost::asio::deadline_timer> m_timer;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<ConnectionScope::IListener> ConnectionScope::getListener() const { return m_listener.lock(); }
inline const boost::posix_time::time_duration & ConnectionScope::getTimeout() const { return m_timeout; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_CONNECTIONSCOPE_H
