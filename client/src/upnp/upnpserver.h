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

#ifndef _OS_UPNP_UPNPSERVER_H
#define _OS_UPNP_UPNPSERVER_H

#include "boost/thread/recursive_mutex.hpp"
#include "boost/type_traits/is_base_of.hpp"
#include "format.h"
#include "iserver.h"
#include "lock.h"
#include "log.h"
#include "notificationsmanager.h"
#include "upnpservice.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
class UPnPServer : public T
{
	typedef T ServerBase;
	OS_STATIC_ASSERT((boost::is_base_of<IServer, ServerBase>::type::value));	// Assicura la derivazione da IServer

// Construction
public:
	UPnPServer();
	template <typename P1>
	UPnPServer(P1 p1);
	template <typename P1, typename P2>
	UPnPServer(P1 p1, P2 p2);
	template <typename P1, typename P2, typename P3>
	UPnPServer(P1 p1, P2 p2, P3 p3);
	template <typename P1, typename P2, typename P3, typename P4>
	UPnPServer(P1 p1, P2 p2, P3 p3, P4 p4);
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	UPnPServer(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);
	virtual ~UPnPServer();

private:
	void construct();

// Attributes
public:
	bool getUPnPEnabled() const;
	void setUPnPEnabled(bool upnpEnabled);

	bool getUPnPMapped() const;

// Operations
private:
	void stopUPnP();

// IServer overrides
public:
	virtual bool bind(const String &bindIP = String::EMPTY, uint32 port = 0);

private:
	mutable boost::recursive_mutex m_upnpCS;
	bool m_upnpEnabled;
	shared_ptr<UPnPService> m_upnpService;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
UPnPServer<T>::UPnPServer()
{
	construct();
}

template <typename T>
template <typename P1>
UPnPServer<T>::UPnPServer(P1 p1) : ServerBase(p1)
{
	construct();
}

template <typename T>
template <typename P1, typename P2>
UPnPServer<T>::UPnPServer(P1 p1, P2 p2) : ServerBase(p1, p2)
{
	construct();
}

template <typename T>
template <typename P1, typename P2, typename P3>
UPnPServer<T>::UPnPServer(P1 p1, P2 p2, P3 p3) : ServerBase(p1, p2, p3)
{
	construct();
}

template <typename T>
template <typename P1, typename P2, typename P3, typename P4>
UPnPServer<T>::UPnPServer(P1 p1, P2 p2, P3 p3, P4 p4) : ServerBase(p1, p2, p3, p4)
{
	construct();
}

template <typename T>
template <typename P1, typename P2, typename P3, typename P4, typename P5>
UPnPServer<T>::UPnPServer(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) : ServerBase(p1, p2, p3, p4, p5)
{
	construct();
}

template <typename T>
UPnPServer<T>::~UPnPServer()
{
	stopUPnP();
}

template <typename T>
void UPnPServer<T>::construct()
{
	m_upnpEnabled = false;
}

template <typename T>
bool UPnPServer<T>::getUPnPEnabled() const
{
	OS_LOCK(m_upnpCS);
	return m_upnpEnabled;
}

template <typename T>
void UPnPServer<T>::setUPnPEnabled(bool upnpEnabled)
{
	OS_ASSERT(ServerBase::running() == false);

	OS_LOCK(m_upnpCS);
	m_upnpEnabled = upnpEnabled;
}

template <typename T>
bool UPnPServer<T>::getUPnPMapped() const
{
	OS_LOCK(m_upnpCS);
	
	if(m_upnpService == null)
		return false;

	return m_upnpService->getMapped();
}

template <typename T>
void UPnPServer<T>::stopUPnP()
{
	OS_LOCK(m_upnpCS);

	if(m_upnpService != null)
	{
		m_upnpService->removePortMapping();
		m_upnpService.reset();
	}
}

template <typename T>
bool UPnPServer<T>::bind(const String &bindIP, uint32 port)
{
	stopUPnP();

	if(ServerBase::bind(bindIP, port) == false)
		return false;

	OS_LOCK(m_upnpCS);

	if(m_upnpEnabled)
	{
		try
		{
			NotificationsManager::instance()->notify(_S("Initializing UPNP"));

			m_upnpService.reset(OS_NEW UPnPService());
			if(m_upnpService->addPortMapping(ServerBase::getEndpoint()))
				OS_LOG_INFO(boost::format("UPnP port %d mapped") % ServerBase::getEndpoint().port());
			else
				OS_LOG_WARNING(boost::format("Cannot map UPnP port %d") % ServerBase::getEndpoint().port());
		}
		catch(std::exception &e)
		{
			OS_LOG_ERROR(e.what());
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UPNP_UPNPSERVER_H
