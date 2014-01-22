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
#include "upnpmanager.h"

#include "boostasio.h"
#include "boost/thread/recursive_mutex.hpp"
#include "conversions.h"
#include "log.h"
#include "miniupnp.h"

//////////////////////////////////////////////////////////////////////

#define OS_MINIUPNP_IGD_NO_FOUND				0		// NO IGD found
#define OS_MINIUPNP_IGD_FOUND					1		// A valid connected IGD has been found
#define OS_MINIUPNP_IGD_NOT_CONNECTED			2		// A valid IGD has been found but it reported as not connected
#define OS_MINIUPNP_IGD_NOT_VALID				3		// An UPnP device has been found but was not recognized as an IGD

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<UPnPManager>
{
// Construction
public:
	pimpl();
	virtual ~pimpl();

// Attributes
public:
	bool getServiceAvailable() const;

	uint32 getDiscoverTimeout() const;
	void setDiscoverTimeout(uint32 discoverTimeout);

// Operations
public:
	bool addPortMapping(uint32 port, const std::string &protocol, std::string description);
	bool removePortMapping(uint32 port, const std::string &protocol);

private:
	void ensureInitialized() const;
	void cleanup();

private:
	mutable boost::recursive_mutex m_mutex;
	uint32 m_discoverTimeout;
	mutable bool m_initialized;
	mutable bool m_serviceAvailable;
	mutable UPNPUrls *m_upnpUrls;
	mutable IGDdatas *m_igdDatas;
	mutable char m_lanIP[16];
};

//////////////////////////////////////////////////////////////////////

pimpl<UPnPManager>::pimpl() : m_discoverTimeout(OS_UPNP_DEFAULT_DISCOVER_TIMEOUT),
							  m_initialized(false),
							  m_serviceAvailable(false),
							  m_upnpUrls(nullptr),
							  m_igdDatas(nullptr)
{

}

pimpl<UPnPManager>::~pimpl()
{
	cleanup();
}

bool pimpl<UPnPManager>::getServiceAvailable() const
{
	ensureInitialized();

	boost::recursive_mutex::scoped_lock lock(m_mutex);

	return m_serviceAvailable;
}

uint32 pimpl<UPnPManager>::getDiscoverTimeout() const
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);

	return m_discoverTimeout;
}

void pimpl<UPnPManager>::setDiscoverTimeout(uint32 discoverTimeout)
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);

	m_discoverTimeout = discoverTimeout;
}

bool pimpl<UPnPManager>::addPortMapping(uint32 port, const std::string &protocol, std::string description)
{
	OS_ASSERT(port != 0);
	OS_ASSERT(protocol.empty() == false);

	ensureInitialized();

	boost::recursive_mutex::scoped_lock lock(m_mutex);

	if(m_serviceAvailable == false)
		return false;

	std::string strPort = conversions::to_string(port);

	if(description.empty())
		description = protocol + "_" + strPort;

	OS_ASSERT(m_upnpUrls != nullptr);
	OS_ASSERT(m_upnpUrls->controlURL != nullptr);
	OS_ASSERT(m_igdDatas != nullptr);
	return UPNP_AddPortMapping(m_upnpUrls->controlURL, m_igdDatas->first.servicetype, strPort.c_str(), strPort.c_str(), m_lanIP, description.c_str(), protocol.c_str(), nullptr, "0") == 0;
}

bool pimpl<UPnPManager>::removePortMapping(uint32 port, const std::string &protocol)
{
	ensureInitialized();

	boost::recursive_mutex::scoped_lock lock(m_mutex);

	if(m_serviceAvailable == false)
		return false;

	OS_ASSERT(m_upnpUrls != nullptr);
	OS_ASSERT(m_upnpUrls->controlURL != nullptr);
	OS_ASSERT(m_igdDatas != nullptr);
	return UPNP_DeletePortMapping(m_upnpUrls->controlURL, m_igdDatas->first.servicetype, conversions::to_string(port).c_str(), protocol.c_str(), nullptr) == 0;
}

void pimpl<UPnPManager>::ensureInitialized() const
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);

	if(m_initialized)
	{
		OS_ASSERT(m_upnpUrls != nullptr);
		OS_ASSERT(m_igdDatas != nullptr);
		return;
	}

	m_initialized = true;

	m_upnpUrls = static_cast<UPNPUrls *>(OS_ALLOCATE(sizeof(UPNPUrls)));
	OS_ZEROMEMORY(m_upnpUrls, sizeof(UPNPUrls));
	m_igdDatas = static_cast<IGDdatas *>(OS_ALLOCATE(sizeof(IGDdatas)));
	OS_ZEROMEMORY(m_igdDatas, sizeof(IGDdatas));
	OS_ZEROMEMORY(m_lanIP, sizeof(m_lanIP));

	int error = 0;
	UPNPDev *devices = upnpDiscover(m_discoverTimeout, nullptr, nullptr, 0, 0, &error);
	if(devices != nullptr)
	{
		switch(UPNP_GetValidIGD(devices, m_upnpUrls, m_igdDatas, m_lanIP, sizeof(m_lanIP)))
		{
		case OS_MINIUPNP_IGD_NO_FOUND:			OS_LOG_DEBUG("UPnP: no IGD found");
												break;

		case OS_MINIUPNP_IGD_FOUND:				OS_LOG_INFO("UPnP: valid IGD found");
												OS_ASSERT(m_upnpUrls->controlURL != nullptr);
												m_serviceAvailable = m_upnpUrls->controlURL != nullptr;
												break;

		case OS_MINIUPNP_IGD_NOT_CONNECTED:		OS_LOG_DEBUG("UPnP: not connected IGD found");
												break;

		case OS_MINIUPNP_IGD_NOT_VALID:			OS_LOG_DEBUG("UPnP: not valid IGD found");
												break;

		default:								OS_ASSERTFALSE();
												break;
		}

		freeUPNPDevlist(devices);
	}
}

void pimpl<UPnPManager>::cleanup()
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);

	m_serviceAvailable = false;

	if(m_upnpUrls != nullptr)
	{
		FreeUPNPUrls(m_upnpUrls);

		OS_DEALLOCATE(m_upnpUrls);
		m_upnpUrls = nullptr;
	}

	OS_DEALLOCATE(m_igdDatas);
	m_igdDatas = nullptr;

	m_initialized = false;
}

//////////////////////////////////////////////////////////////////////

UPnPManager::UPnPManager()
{

}

UPnPManager::~UPnPManager()
{

}

bool UPnPManager::getServiceAvailable() const
{
	return m_impl->getServiceAvailable();
}

uint32 UPnPManager::getDiscoverTimeout() const
{
	return m_impl->getDiscoverTimeout();
}

void UPnPManager::setDiscoverTimeout(uint32 discoverTimeout)
{
	m_impl->setDiscoverTimeout(discoverTimeout);
}

bool UPnPManager::addPortMapping(uint32 port, const std::string &protocol, const std::string &description)
{
	return m_impl->addPortMapping(port, protocol, description);
}

bool UPnPManager::removePortMapping(uint32 port, const std::string &protocol)
{
	return m_impl->removePortMapping(port, protocol);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
