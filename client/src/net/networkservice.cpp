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
#include "networkservice.h"

#include "boost/bind.hpp"
#include "log.h"
#include "platformmanager.h"
#include "threadsgroup.h"
#include "workerthread.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<NetworkService>
{
// Construction
public:
	pimpl();
	virtual ~pimpl();

public:
	shared_ptr<boost::asio::io_service> service;
	shared_ptr<ConnectionsManager> connectionsManager;
	ThreadsGroup<WorkerThread> workerThreads;
	shared_ptr<boost::asio::deadline_timer> timer;
	mutable boost::recursive_mutex serviceCS;
	uint32 workerThreadsCount;
};

//////////////////////////////////////////////////////////////////////

pimpl<NetworkService>::pimpl() : workerThreadsCount(0)
{

}

pimpl<NetworkService>::~pimpl()
{

}

//////////////////////////////////////////////////////////////////////

NetworkService::NetworkService(boost::optional<uint32> workerThreadsCount)
{
	if(workerThreadsCount)
		m_impl->workerThreadsCount = workerThreadsCount.get();
	else
		m_impl->workerThreadsCount = PlatformManager::instance()->getOptimalHardwareConcurrency();
	m_impl->service = createAsioService();
	m_impl->connectionsManager.reset(OS_NEW ConnectionsManager(m_impl->service));
	m_impl->connectionsManager->addListener(boost::bind(&NetworkService::connectionsCallback, this, _1, _2));
}

NetworkService::~NetworkService()
{

}

shared_ptr<boost::asio::io_service> NetworkService::getService() const
{
	return m_impl->service;
}

shared_ptr<ConnectionsManager> NetworkService::getConnectionsManager() const
{
	return m_impl->connectionsManager;
}

uint32 NetworkService::getWorkerThreadsCount() const
{
	OS_LOCK(m_impl->serviceCS);
	return m_impl->workerThreadsCount;
}

void NetworkService::setWorkerThreadsCount(uint32 workerThreadsCount)
{
	OS_ASSERT(running() == false);

	OS_LOCK(m_impl->serviceCS);
	m_impl->workerThreadsCount = workerThreadsCount;
}

void NetworkService::handleIdle()
{
	if(onIdle())
		startTimer();
}

void NetworkService::startTimer()
{
	if(m_impl->timer == nullptr)
		return;

	m_impl->timer->expires_from_now(boost::posix_time::seconds(1));
	m_impl->timer->async_wait(boost::bind(&NetworkService::handleIdle, this));
}

void NetworkService::runService()
{
	while(stopped() == false)
	{
		try
		{
			m_impl->service->run();
		}
		catch(std::exception &e)
		{
			OS_LOG_ERROR(String(_S("Network service error: ")) + e.what());
		}
	}
}

void NetworkService::stopService()
{
	try
	{
		m_impl->service->stop();
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(_S("Service error (closing): ") + e.what());
	}

	getConnectionsManager()->disable();
}

void NetworkService::connectionsCallback(shared_ptr<IConnection> connection, ConnectionsManager::ConnectionEvent e)
{
	switch(e)
	{
	case ConnectionsManager::connectionAdded:		onConnectionAdded(connection);
													break;

	case ConnectionsManager::connectionRemoved:		onConnectionRemoved(connection);
													break;

	default:										OS_ASSERTFALSE();
													break;
	}
}

bool NetworkService::onIdle()
{
	return stopped() == false;
}

void NetworkService::onConnectionAdded(shared_ptr<IConnection> connection)
{

}

void NetworkService::onConnectionRemoved(shared_ptr<IConnection> connection)
{

}

void NetworkService::stop()
{
	stopService();

	ThreadBase::stop();
}

void NetworkService::run()
{
	m_impl->service->reset();

	m_impl->connectionsManager->enable();

	// Inizializza il timer
	m_impl->timer = createAsioObject<boost::asio::deadline_timer>(*getService());
	startTimer();

	OS_ASSERT(m_impl->workerThreads.empty());

	uint32 workerThreadsCount = getWorkerThreadsCount();
	for(uint32 i = 0; i < workerThreadsCount; i++)
	{
	    shared_ptr<WorkerThread> thread(OS_NEW WorkerThread(boost::bind(&NetworkService::runService, this)));
        // Assegna ai sotto threads la stessa priorità del thread principale
	    thread->setPriority(getPriority());
	    // Accoda e avvia il sottothread
		m_impl->workerThreads.add(thread, true);
	}

	// Avvia il servizio
	runService();

	// Attende la chiusura di tutti i threads
	m_impl->workerThreads.join();
	m_impl->workerThreads.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
