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
#include "dhtthread.h"

#include "algorithms.h"
#include "buffer.h"
#include "cryptmanager.h"
#include "conversions.h"
#include "dht.h"
#include "dhtmanager.h"
#include "dhtportalsearch.h"
#include "filesystem.h"
#include "log.h"
#include "options.h"
#include "osiriscommon.h"
#include "p2pp2psystem.h"
#include "p2pserver.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

#define OS_DHT_NODES_CACHE_FILE				"nodes.xml"
#define OS_DHT_NODES_CACHE_FILE_NODES		"nodes"
#define OS_DHT_NODES_CACHE_FILE_NODE		"node"
#define OS_DHT_NODES_CACHE_FILE_IP			"ip"
#define OS_DHT_NODES_CACHE_FILE_PORT		"port"

//////////////////////////////////////////////////////////////////////

#if OS_PLATFORM == OS_PLATFORM_WIN
    #define OS_EINTR    WSAEINTR
    #define OS_EINVAL   WSAEINVAL
    #define OS_EFAULT   WSAEFAULT
#else
    #define OS_EINTR    EINTR
    #define OS_EINVAL   EINVAL
    #define OS_EFAULT   EFAULT
#endif

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

DHTThread::DHTThread(const String &rootPath, boost::asio::ip::udp::socket::native_type socket) : m_rootPath(rootPath),
																								 m_socket(socket),
																								 m_portal(0)
{
	
}

DHTThread::~DHTThread()
{

}

DHTThread::DHTStatus DHTThread::getStatus(int *total) const
{
	int good = 0;
	int dubious = 0;
	int incoming = 0;
    dht_nodes(AF_INET, &good, &dubious, nullptr, &incoming);

	if(total != nullptr)
		*total = good + dubious + incoming;

    if((good < 4) || (good + dubious <= 8))
        return dhtStatusBroken;
    else if(good < 40)
        return dhtStatusPoor;
    else if(incoming < 8)
        return dhtStatusFirewalled;

	return dhtStatusGood;
}

shared_ptr<IDHTSearch> DHTThread::peekNextSearch()
{
	uint32 portals = PortalsSystem::instance()->getPortalsCount();
	if(portals == 0)
		return nullptr;

	if(m_portal >= portals)
		m_portal = 0;

	shared_ptr<Portal> portal = PortalsSystem::instance()->getPortalByIndex(m_portal);
	m_portal++;

	if(portal == nullptr)
		return nullptr;

#ifdef OS_OSIRIS_VERSION_RAZOR
  std::string id = generateSearchID(portal->getNetworkID());
#else
	std::string id = generateSearchID(portal->getPortalID());
#endif
	if(id.empty())
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	if(m_searches.find(id) != m_searches.end())
		return nullptr;		// Ricerca già in corso

	shared_ptr<IDHTSearch> search(OS_NEW DHTPortalSearch(portal));
	if(search->assignID(id) == false)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	OS_LOG_DEBUG("Starting DHT search on portal '" + portal->getName() + "', search id = '" + id + "'");

	shared_ptr<P2PServer> server = p2p::P2PSystem::instance()->getServer();
	uint32 announcePort = 0;
	if(server != nullptr && server->getBindPort(announcePort))
		search->setAnnouncePort(announcePort);

	m_searches[id] = search;
	return search;
}

void DHTThread::loadNodesCache()
{
	String filename = utils::makeFilePath(m_rootPath, OS_DHT_NODES_CACHE_FILE);

	if(FileSystem::instance()->fileExists(filename) == false)
		return;

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	if(document->parseFile(filename) == false)
		return;

	shared_ptr<XMLNodes> nodes = document->getRoot()->getNodes();
	for(XMLNodes::const_iterator i = nodes->begin(); i != nodes->end(); ++i)
	{
		shared_ptr<XMLNode> node = *i;
		if(node->getName() != OS_DHT_NODES_CACHE_FILE_NODE)
			continue;

		m_nodes[node->getAttributeString(OS_DHT_NODES_CACHE_FILE_IP).to_ascii()] = static_cast<uint16>(node->getAttributeUint32(OS_DHT_NODES_CACHE_FILE_PORT));
	}
}

void DHTThread::loadRouters()
{
	vector<std::string>::type routers;
	algorithms::split(Options::instance()->getOption<String>(OS_DHT_OPTION_BOOTSTRAP_ROUTERS).to_ascii(), routers, boost::algorithm::is_any_of(";"));
	for(vector<std::string>::type::const_iterator i = routers.begin(); i != routers.end(); ++i)
	{
		const std::string &router = *i;
		size_t pos = router.find(":");
		if(pos != std::string::npos)
			m_nodes[algorithms::left(router, pos)] = conversions::from_string<uint16>(algorithms::mid(router, pos + 1));
		else
			OS_LOG_WARNING("Invalid DHT router '" + router + "'");
	}
}

void DHTThread::bootstrap()
{
	if(m_nodes.empty())
		return;

	Nodes::iterator i = m_nodes.begin();
	bootstrap(i->first, i->second);
	m_nodes.erase(i);
}

bool DHTThread::bootstrap(const std::string &host, uint16 port)
{
    struct addrinfo hints, *info;
	memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;

    int rc = getaddrinfo(host.c_str(), conversions::to_string(port).c_str(), &hints, &info);
    if(rc != 0)
        return false;

	bool done = false;

    struct addrinfo *current = info;
	while(current != nullptr)
	{
        dht_ping_node(current->ai_addr, static_cast<int>(current->ai_addrlen));
        current = current->ai_next;
    }

    freeaddrinfo(info);

	return done;
}

void DHTThread::dhtEventCallback(void *closure, int e, unsigned char *id, void *data, size_t data_len)
{
	switch(e)
	{
	case DHT_EVENT_NONE:			String();
									break;

	case DHT_EVENT_VALUES:			reinterpret_cast<DHTThread *>(closure)->processResults(id, data, static_cast<uint32>(data_len));
									break;

	case DHT_EVENT_VALUES6:			String();
									break;

	case DHT_EVENT_SEARCH_DONE:		reinterpret_cast<DHTThread *>(closure)->removeSearch(id);
									break;

	case DHT_EVENT_SEARCH_DONE6:	String();
									break;
	}
}

std::string DHTThread::generateSearchID(const std::string &id)
{
	// N.B: non modificare mai la conversione da ID a hash DHT.
	// Per generare la chiave DHT si considera tutto l'ID (in binario, compreso di header) e se ne calcola l'hash SHA

	if(id.empty())
	{
		OS_ASSERTFALSE();
		return constants::empty_string;
	}

	Buffer buffer;
	if(buffer.fromHex(id) == false)
		return constants::empty_string;

	return CryptManager::instance()->SHA(buffer.getData(), buffer.getSize()).toHex();
}

std::string DHTThread::getSearchID(unsigned char *id)
{
	Buffer buffer;
	if(buffer.assign(id, CryptManager::SHA_DIGESTSIZE) != CryptManager::SHA_DIGESTSIZE)
		return constants::empty_string;

	return buffer.toHex();
}

bool DHTThread::processResults(unsigned char *id, void *data, uint32 size)
{
	std::string searchID = getSearchID(id);

	OS_LOG_DEBUG("Found result(s) for DHT search '" + searchID + "'");

	Searches::iterator i = m_searches.find(searchID);
	if(i == m_searches.end())
		return false;

	Buffer results;
	if(results.assign(data, size) != size)
		return false;

	while(results.isReadable())
	{
		// I risultati sono codificati nella forma: <IP_1> (4 bytes) - <PORT_1> (2 bytes), <IP_2> - <PORT_2>, ...

		uint32 ip = 0;
		if(results.read(&ip, sizeof(uint32)) != sizeof(uint32))
		{
			OS_ASSERTFALSE();
			return false;
		}
		ip = ntohl(ip);

		uint16 port = 0;
		if(results.read(&port, sizeof(uint16)) != sizeof(uint16))
		{
			OS_ASSERTFALSE();
			return false;
		}

		port = ntohs(port);
		i->second->processResult(ip, port);
	}

	return true;
}

bool DHTThread::removeSearch(unsigned char *id)
{
	std::string searchID = getSearchID(id);

	Searches::iterator i = m_searches.find(searchID);
	if(i == m_searches.end())
	{
		OS_ASSERTFALSE();
		return false;
	}

	OS_LOG_DEBUG("DHT search '" + searchID + "' completed");

	m_searches.erase(i);
	return true;
}

void DHTThread::run()
{
	loadNodesCache();

	OS_LOG_DEBUG("DHT thread running");

#ifdef OS_DEBUG
	bool dumping = true;
#else
	bool dumping = false;
#endif
	bool routersLoaded = false;

	time_t tosleep = 0;
	while(stopped() == false)
	{
		if(tosleep > 5)
			tosleep = 5;

        struct timeval tv;
        fd_set readfds;
        tv.tv_sec = static_cast<long>(tosleep);
        tv.tv_usec = rand() % 1000000;

        FD_ZERO(&readfds);
        if(m_socket >= 0)
            FD_SET(m_socket, &readfds);

		int rc = select(static_cast<int>(m_socket)+1, &readfds, NULL, NULL, &tv);

        if(stopped())
            break;

        rc = dht_periodic(rc > 0, &tosleep, dhtEventCallback, this);
        if(rc < 0)
		{
            if(errno == OS_EINTR)
			{
				tosleep = 0;
                continue;
            }
			else
			{
                if(rc == OS_EINVAL || rc == OS_EFAULT)
				{
					OS_LOG_CRITICAL("Fatal DHT error");
					break;
				}

                tosleep = 1;
            }
        }

		DHTStatus status = getStatus();
		if(status == dhtStatusBroken)
		{
			if(m_nodes.empty())
			{
				if(routersLoaded == false)
				{
					routersLoaded = true;
					loadRouters();
				}
			}

			bootstrap();
		}
		else
		{
			if(status >= dhtStatusPoor)
			{
				shared_ptr<IDHTSearch> search = peekNextSearch();
				if(search != nullptr)
				{
					OS_ASSERT(search->getID()->getSize() == CryptManager::SHA_DIGESTSIZE);
					// This is how you trigger a search for a torrent hash.  If port (the second argument) is non-zero, it also performs an announce. Since peers expire announced data after 30 minutes, it's a good idea to reannounce every 28 minutes or so.
					dht_search(search->getID()->getData(), search->getAnnouncePort(), AF_INET, dhtEventCallback, this);
				}
			}
		}

        if(dumping)
		{
            dht_dump_tables(stdout);
//			dumping = false;
        }
    }
}

void DHTThread::onLeave()
{
	ThreadBase::onLeave();

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLNode> root = document->create(OS_DHT_NODES_CACHE_FILE_NODES);

	int total = 0;
	DHTStatus status = getStatus(&total);
	if(status >= dhtStatusPoor)
	{
		scoped_array<sockaddr_in, os_deallocate_t> sins(OS_ALLOCATE_T(sockaddr_in, total));

		int total_ipv6 = 0;
		int n = dht_get_nodes(sins.get(), &total, nullptr, &total_ipv6);
		OS_ASSERT(n == total);
		int cacheLimit = static_cast<int>(Options::instance()->getOption<uint32>(OS_DHT_OPTION_NODES_CACHE_SIZE));
		if(total > cacheLimit)
			total = cacheLimit;

		for(int i = 0; i < total; ++i)
		{
			shared_ptr<XMLNode> node = root->addChild(OS_DHT_NODES_CACHE_FILE_NODE);
			node->setAttributeString(OS_DHT_NODES_CACHE_FILE_IP, inet_ntoa(sins[i].sin_addr));
			node->setAttributeUint32(OS_DHT_NODES_CACHE_FILE_PORT, ntohs(sins[i].sin_port));
		}
	}

	if(FileSystem::instance()->ensureDirectory(m_rootPath))
	{
		String filename = utils::makeFilePath(m_rootPath, OS_DHT_NODES_CACHE_FILE);
		if(document->writeFile(filename) == false)
			OS_LOG_ERROR("Error saving dht cache file '" + filename + "'");
	}
	else
	{
		OS_LOG_ERROR("Cannot ensure directory '" + m_rootPath + "'");
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
