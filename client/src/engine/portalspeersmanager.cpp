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
#include "portalspeersmanager.h"

#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/mem_fun.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/multi_index/sequenced_index.hpp"
#include "boost/multi_index_container.hpp"
#include "datatable.h"
#include "dbsqlselect.h"
#include "dbtables.h"
#include "eventsource.h"
#include "ievent.h"
#include "ipaddress.h"
#include "iportaldatabase.h"
#include "lock.h"
#include "multiindexwrappers.h"
#include "networksystem.h"
#include "options.h"
#include "portalspeer.h"
#include "portalsportal.h"
#include "randomnumbergenerator.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<PortalsPeersManager> : public Object
{
	struct sequence {};
	struct map {};
	struct supernodes {};

	typedef multi_index_container<shared_ptr<PortalsPeer>, boost::multi_index::indexed_by<
			boost::multi_index::sequenced<boost::multi_index::tag<sequence> >,
			boost::multi_index::hashed_unique<boost::multi_index::tag<map>, BOOST_MULTI_INDEX_CONST_MEM_FUN(PortalsPeer, uint64, getID)>,
			boost::multi_index::ordered_non_unique<boost::multi_index::tag<supernodes>, BOOST_MULTI_INDEX_CONST_MEM_FUN(PortalsPeer, bool, getSupernode), std::greater<bool> > >,
			StdAllocator<shared_ptr<PortalsPeer> >
	>::type Peers;

	/*
// Events
public:
	class EventStorePeer : public IEvent
	{
		// Construction
		public:
			EventStorePeer();
			EventStorePeer(const String &ip, uint32 port, const String &origin);
			virtual ~EventStorePeer();
		
		// Attributes
		public:
			inline const String & getIP() const { return m_ip; }
			inline uint32 getPort() const { return m_port; }
			inline const String & getOrigin() const { return m_origin; }
			inline bool getCancel() const { return m_cancel; }
			inline void setCancel(const bool cancel) { m_cancel = cancel; }

		protected:
			String m_ip;
			uint32 m_port;
			String m_origin;
			bool m_cancel;
	};
	*/
	
// Construction
public:
	pimpl(Portal *portal, uint32 peersCacheSize);
	virtual ~pimpl();

// Attributes
public:
	bool empty() const;

	uint32 getPeersCacheSize() const;
	void setPeersCacheSize(uint32 size);

	shared_ptr<EventSource> getEventStorePeer() const;

// Operations
public:
	bool savePeer(const String &ip, uint32 port, bool validate, const String &origin);
	void validatePeer(shared_ptr<IPAddress> address);
	void invalidatePeer(shared_ptr<IPAddress> address);	
	void removePeer(uint64 id);

	shared_ptr<PortalsPeer> peekPeer();

	void addSupernode(shared_ptr<IPAddress> address);
	void extractSupernodes(uint32 count, list<shared_ptr<PortalsPeer> >::type &peers);

	void getPeersInfo(uint32 &activePeers, uint32 &activeSupernodes);

	DateTime getContactDateCheck() const;

	bool loadPeers(shared_ptr<IPortalDatabase> database);
	bool savePeers(shared_ptr<IPortalDatabase> database);

	void exportXML(shared_ptr<XMLNode> node) const;

private:
	bool storePeer(const String &ip, uint32 port, const String &origin, bool validate);
	void limitPeers();

	template <typename T>
	shared_ptr<PortalsPeer> peekPeer(T &view);

private:
	Portal &m_portal;
	uint32 m_peersCacheSize;
	shared_ptr<boost::recursive_mutex> m_peersCS;
	mutable boost::mutex m_cs;
	bool m_peekSupernode;
	Peers m_peers;
	//shared_ptr<EventSource> m_eventStorePeer;
};

//////////////////////////////////////////////////////////////////////

/*
pimpl<PortalsPeersManager>::EventStorePeer::EventStorePeer(const String &ip, uint32 port, const String &origin) :	m_ip(ip),
																													m_port(port),
																													m_origin(origin),
																													m_cancel(false)
{
}

pimpl<PortalsPeersManager>::EventStorePeer::~EventStorePeer()
{
}
*/

pimpl<PortalsPeersManager>::pimpl(Portal *portal, uint32 peersCacheSize) : m_portal(*portal),
																		  m_peersCacheSize(peersCacheSize),
																		  m_peersCS(OS_NEW_T(boost::recursive_mutex), os_delete_t()),
																		  m_peekSupernode(true)/*,
																		  m_eventStorePeer(OS_NEW EventSource())*/

{

}

pimpl<PortalsPeersManager>::~pimpl()
{

}

bool pimpl<PortalsPeersManager>::empty() const
{
	OS_LOCK(m_cs);
	return m_peers.empty();	
}

uint32 pimpl<PortalsPeersManager>::getPeersCacheSize() const
{
	OS_LOCK(m_cs);
	return m_peersCacheSize;
}

void pimpl<PortalsPeersManager>::setPeersCacheSize(uint32 size)
{
	OS_LOCK(m_cs);

	OS_ASSERT(size > 0);
	m_peersCacheSize = utils::max<uint32>(size, 1);
	
	limitPeers();
}

/*
shared_ptr<EventSource> pimpl<PortalsPeersManager>::getEventStorePeer() const
{
	return m_eventStorePeer;
}
*/

bool pimpl<PortalsPeersManager>::savePeer(const String &ip, uint32 port, bool validate, const String &origin)
{
	return storePeer(ip, port, origin, validate);
}

void pimpl<PortalsPeersManager>::validatePeer(shared_ptr<IPAddress> address)
{
	OS_LOCK(m_cs);

	Peers::index<map>::type &view = m_peers.get<map>();	
	Peers::index<map>::type::iterator i = view.find(PortalsPeer::makeID(address));
	if(i != view.end())
		(*i)->validate();
}

void pimpl<PortalsPeersManager>::invalidatePeer(shared_ptr<IPAddress> address)
{
	OS_LOCK(m_cs);

	Peers::index<map>::type &view = m_peers.get<map>();	
	Peers::index<map>::type::iterator i = view.find(PortalsPeer::makeID(address));
	if(i != view.end())
	{
		// Invalida il nodo e verifica se è stato raggiunto il numero massimo di invalidazioni
		if((*i)->invalidate() == false)
		{
			view.erase(i);			
		}
		else 
		{
			// Nel caso in cui un supernodo venga invalidato viene retrocesso a nodo normale
			if((*i)->getSupernode())			
				view.modify(i, boost::bind(&PortalsPeer::setSupernode, _1, false));			
		}
	}
}

void pimpl<PortalsPeersManager>::removePeer(uint64 id)
{
	OS_LOCK(m_cs);

	Peers::index<map>::type &view = m_peers.get<map>();	
	Peers::index<map>::type::iterator i = view.find(id);
	if(i != view.end())
		view.erase(i);
}

shared_ptr<PortalsPeer> pimpl<PortalsPeersManager>::peekPeer()
{
	// TOCLEAN
	// m_portal.updateIsisStatus();	// N.B.: la chiamata non deve essere sotto lock perchè potrebbe richiamare a sua volta dei metodo di PortalsPeersManager e andare in deadlock
	
	OS_LOCK(m_cs);

	// Estrae a rotazione un supernodo e un nodo normale:
	// - se non esistono supernodi vengono estratti nodi normali
	// - se viene estratto un supernodo, quest'ultimo viene comunque riaccodato in coda alla sequenza e pertanto non verrà riestratto nella successiva estrazione (per sequenza)
	// - se viene estratto un nodo normale non viene intaccato l'ordine nella lista dei supernodi

	shared_ptr<PortalsPeer> peer;
	if(m_peekSupernode)
		peer = peekPeer(m_peers.get<supernodes>());
	else  
		peer = peekPeer(m_peers.get<sequence>());

	m_peekSupernode = !m_peekSupernode;
	return peer;
}

void pimpl<PortalsPeersManager>::addSupernode(shared_ptr<IPAddress> address)
{
	OS_LOCK(m_cs);

	Peers::index<map>::type &view = m_peers.get<map>();	
	Peers::index<map>::type::iterator i = view.find(PortalsPeer::makeID(address));
	if(i != view.end())
	{
		if((*i)->getSupernode() == false)
		{
			OS_ASSERT((*i)->getRetries() == 0);
			view.modify(i, boost::bind(&PortalsPeer::setSupernode, _1, true));		
		}
	}
}

void pimpl<PortalsPeersManager>::extractSupernodes(uint32 count, list<shared_ptr<PortalsPeer> >::type &peers)
{
	OS_ASSERT(count > 0);
	OS_ASSERT(peers.empty());

	OS_LOCK(m_cs);

	// TODO: è da ottimizzare perchè i supernodi andrebbero proposti a "rotazione" (ora la rotazione è data dalla "peekPeer", ma finchè non viene richiamata vengono proposti gli stessi nodi...)

	Peers::index<supernodes>::type &view = m_peers.get<supernodes>();
	for(Peers::index<supernodes>::type::const_iterator i = view.begin(); (i != view.end()) && (static_cast<uint32>(peers.size()) < count); ++i)
	{
		// Verifica se non sono più disponibili supernodi
		if((*i)->getSupernode() == false)
			break;

		peers.push_back(*i);
	}
}

DateTime pimpl<PortalsPeersManager>::getContactDateCheck() const
{
	return DateTime::now() + TimeDuration::minutes(Options::instance()->getOption<uint32>(Options::portals_options::peers_contact_date_check));
}

void pimpl<PortalsPeersManager>::getPeersInfo(uint32 &activePeers, uint32 &activeSupernodes)
{
	OS_LOCK(m_cs);

	OS_ASSERT(m_peersCS != nullptr);
	OS_LOCK(*m_peersCS);

	activePeers = 0;
	activeSupernodes = 0;

	DateTime contactDateCheck = getContactDateCheck();

	for(Peers::iterator i = m_peers.begin(); i != m_peers.end(); ++i)
	{
		shared_ptr<PortalsPeer> peer = *i;

		DateTime lastContactDate = peer->getLastContactDate();
		if(lastContactDate.isValid())
		{
			OS_ASSERT(peer->getRetries() == 0);

			if(lastContactDate <= contactDateCheck)
			{
				activePeers++;

				if(peer->getSupernode())
					activeSupernodes++;
			}
		}
	}
}

bool pimpl<PortalsPeersManager>::loadPeers(shared_ptr<IPortalDatabase> database)
{
	OS_ASSERT(database != nullptr);

	shared_ptr<DbSqlSelect> selectStatement(OS_NEW DbSqlSelect(DBTABLES::PEERS_TABLE));
	selectStatement->limit.setCount(getPeersCacheSize());
	selectStatement->fields.add(DBTABLES::PEERS::IP);
	selectStatement->fields.add(DBTABLES::PEERS::PORT);
	
	DataTable result;
	if(database->execute(selectStatement, result) == false)
		return false;	

	for(uint32 i = 0; i < result.rows(); i++)
	{
		const DataTableRow &current = result[i];
		storePeer(*current[DBTABLES::PEERS::IP], *current[DBTABLES::PEERS::PORT], OS_PEER_ORIGIN_CACHE, false);
	}

	OS_LOCK(m_cs);
	limitPeers();

	return true;	
}

bool pimpl<PortalsPeersManager>::savePeers(shared_ptr<IPortalDatabase> database)
{
	OS_ASSERT(database != nullptr);	

	if(database->execute(String::format(_S("delete from %S").c_str(), DBTABLES::PEERS_TABLE.c_str())) == false)
		return false;

	OS_LOCK(m_cs);

	for(Peers::iterator i = m_peers.begin(); i != m_peers.end(); ++i)
	{
		shared_ptr<PortalsPeer> peer = *i;
		// 24/03/10: Osiris 0.14 salva nel db solo i nodi validi
		if(peer->getCacheable())
		{
			if(database->execute(String::format(_S("insert into %S (%S, %S) values (%S, %S)").c_str(), DBTABLES::PEERS_TABLE.c_str(), DBTABLES::PEERS::IP.c_str(), DBTABLES::PEERS::PORT.c_str(), Convert::toSQL(peer->getIP()).c_str(), Convert::toSQL(peer->getPort()).c_str())) == false)
				return false;
		}
	}

	return true;
}

void pimpl<PortalsPeersManager>::exportXML(shared_ptr<XMLNode> node) const
{
	OS_LOCK(m_cs);

	OS_ASSERT(m_peersCS != nullptr);
	OS_LOCK(*m_peersCS);

	int peers = 0;
	int activePeers = 0;
	int activeSupernodes = 0;

	DateTime contactDateCheck = getContactDateCheck();

	for(Peers::iterator i = m_peers.begin(); i != m_peers.end(); ++i)
	{
		shared_ptr<PortalsPeer> peer = *i;

		peers++;

		shared_ptr<XMLNode> nodePeer = node->addChild(_S("peer"));

		DateTime lastContactDate = peer->getLastContactDate();

		bool active = (lastContactDate.isValid() && lastContactDate <= contactDateCheck);

		nodePeer->setAttributeString("ip",peer->getIP());
		nodePeer->setAttributeUint32("port",peer->getPort());
		nodePeer->setAttributeUint32("retries",peer->getRetries());
		nodePeer->setAttributeString("origin",peer->getOrigin());
		nodePeer->setAttributeDateTime("last_contact",lastContactDate);
		nodePeer->setAttributeBool("active", active);
		nodePeer->setAttributeBool("super",peer->getSupernode());
				
		if(active)
		{
			OS_ASSERT(peer->getRetries() == 0);

			activePeers++;

			if(peer->getSupernode())
				activeSupernodes++;			
		}
	}

	node->setAttributeUint32("peers",peers);
	node->setAttributeUint32("active",activePeers);
	node->setAttributeUint32("super",activeSupernodes);
}

bool pimpl<PortalsPeersManager>::storePeer(const String &ip, uint32 port, const String &origin, bool validate)
{
	// Filtra l'indirizzo locale
	if(NetworkSystem::instance()->validateIP(ip) == false || NetworkSystem::instance()->isLocalAddress(ip)
#ifndef OS_DEBUG
		// In release filtra anche gli indirizzi privati
		|| NetworkSystem::instance()->isPrivateAddress(ip)
#endif // OS_DEBUG
	)
		return false;

	// Event
	/*
	EventStorePeer e(ip, port, origin);
	getEventStorePeer()->fire(&e);	
	if(e.getCancel())
		return false;
	*/

	boost::asio::ip::tcp::endpoint endpoint;
	try
	{
		endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip.to_ascii()), port);
	}
	catch(std::exception &)
	{
		return false;
	}
	
	OS_LOCK(m_cs);

	uint64 id = PortalsPeer::makeID(ip, port);

	Peers::index<map>::type &view = m_peers.get<map>();
	Peers::index<map>::type::const_iterator i = view.find(id);
	if(i != view.end())
	{
		if(validate)
			(*i)->validate();
	}
	else
	{
		OS_ASSERT(m_peersCS != nullptr);
		m_peers.push_back(shared_ptr<PortalsPeer>(OS_NEW PortalsPeer(m_peersCS, endpoint, origin)));
	}

	limitPeers();

	return true;
}

void pimpl<PortalsPeersManager>::limitPeers()
{
	// N.B.: questa funzione deve essere loccata a monte
	OS_ASSERT(m_cs.try_lock() == false);

	OS_ASSERT(m_peersCacheSize > 0);

	if(m_peers.size() <= m_peersCacheSize)
		return;
	
	// Tenta di rimuovere prima i nodi invalidi
	Peers::index<sequence>::type &view = m_peers.get<sequence>();
	for(Peers::index<sequence>::type::iterator i = view.begin(); i != view.end();)
	{
		Peers::index<sequence>::type::iterator y = i;
		++i;

		if((*y)->getRetries() > 0)
		{
			view.erase(y);
			if(m_peers.size() <= m_peersCacheSize)
				return;
		}
	}

	// Elimina i nodi più vecchi
	utils::pop_front_until(m_peers, m_peersCacheSize);
}

template <typename T>
shared_ptr<PortalsPeer> pimpl<PortalsPeersManager>::peekPeer(T &view)
{
	// N.B.: questa funzione deve essere loccata a monte
	OS_ASSERT(m_cs.try_lock() == false);

	typename T::iterator i = view.begin();
	if(i == view.end())
		return nullptr;
	
	// Estrae il nodo in testa
	shared_ptr<PortalsPeer> peer = *i;
			
	// Riposiziona il nodo in coda
	view.erase(i);
	view.insert(view.end(), peer);	

	return peer;
}

//////////////////////////////////////////////////////////////////////

PortalsPeersManager::PortalsPeersManager(Portal &portal, uint32 peersCacheSize) : m_impl(&portal, peersCacheSize)
{

}

PortalsPeersManager::~PortalsPeersManager()
{
		
}

bool PortalsPeersManager::empty() const
{
	return m_impl->empty();
}

uint32 PortalsPeersManager::getPeersCacheSize() const
{
	return m_impl->getPeersCacheSize();
}

void PortalsPeersManager::setPeersCacheSize(uint32 size)
{
	m_impl->setPeersCacheSize(size);
}

/*
shared_ptr<EventSource> PortalsPeersManager::getEventStorePeer() const
{
	return m_impl->getEventStorePeer();
}
*/

bool PortalsPeersManager::savePeer(shared_ptr<IPAddress> address, bool validate, const String &origin)
{
	if(address == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	return m_impl->savePeer(address->toString(), address->getPort(), validate, origin);
}

bool PortalsPeersManager::savePeer(const String &ip, uint32 port, bool validate, const String &origin)
{
	return m_impl->savePeer(ip, port, validate, origin);
}

void PortalsPeersManager::validatePeer(shared_ptr<IPAddress> address)
{
	m_impl->validatePeer(address);
}

void PortalsPeersManager::invalidatePeer(shared_ptr<IPAddress> address)
{
	m_impl->invalidatePeer(address);
}

void PortalsPeersManager::removePeer(shared_ptr<IPAddress> address)
{
	m_impl->removePeer(PortalsPeer::makeID(address));
}

void PortalsPeersManager::removePeer(uint64 id)
{
	m_impl->removePeer(id);
}

shared_ptr<PortalsPeer> PortalsPeersManager::peekPeer()
{
	return m_impl->peekPeer();
}

void PortalsPeersManager::addSupernode(shared_ptr<IPAddress> address)
{
	m_impl->addSupernode(address);
}

void PortalsPeersManager::extractSupernodes(uint32 count, list<shared_ptr<PortalsPeer> >::type &peers)
{
	m_impl->extractSupernodes(count, peers);
}

void PortalsPeersManager::getPeersInfo(uint32 &activePeers, uint32 &activeSupernodes)
{
	m_impl->getPeersInfo(activePeers, activeSupernodes);
}

DateTime PortalsPeersManager::getContactDateCheck() const
{
	return m_impl->getContactDateCheck();
}

bool PortalsPeersManager::loadPeers(shared_ptr<IPortalDatabase> database)
{
	return m_impl->loadPeers(database);
}

bool PortalsPeersManager::savePeers(shared_ptr<IPortalDatabase> database)
{
	return m_impl->savePeers(database);
}

void PortalsPeersManager::exportXML(shared_ptr<XMLNode> node) const
{
	return m_impl->exportXML(node);
}

/*
bool PortalsPeersManager::empty() const
{
	OS_LOCK(m_cs);
	return m_peers.empty();
}

uint32 PortalsPeersManager::getPeersCacheSize() const
{
	OS_LOCK(m_cs);
	return m_peersCacheSize;
}

void PortalsPeersManager::setPeersCacheSize(uint32 size)
{
	{
		OS_LOCK(m_cs);
		m_peersCacheSize = utils::max<uint32>(size, 1);
	}

	limitPeers();
}

bool PortalsPeersManager::savePeer(shared_ptr<IPAddress> address, bool validate, const String &origin)
{
	if(address == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	return savePeer(address->toString(), address->getPort(), validate, origin);
}

bool PortalsPeersManager::savePeer(const String &ip, uint32 port, bool validate, const String &origin)
{
	return storePeer(ip, port, 0, origin, true, validate);
}

void PortalsPeersManager::validatePeer(shared_ptr<IPAddress> address)
{
	OS_LOCK(m_cs);

	shared_ptr<PortalsPeer> peer = m_peers.get(PortalsPeer::makeID(address));
	if(peer != nullptr)
		peer->validate();
}

void PortalsPeersManager::invalidatePeer(shared_ptr<IPAddress> address)
{
	OS_LOCK(m_cs);

	shared_ptr<PortalsPeer> peer = m_peers.get(PortalsPeer::makeID(address));
	if(peer != nullptr)
		peer->invalidate();
}

void PortalsPeersManager::removePeer(shared_ptr<IPAddress> address)
{
	removePeer(PortalsPeer::makeID(address));
}

void PortalsPeersManager::removePeer(uint64 id)
{
	OS_LOCK(m_cs);
	m_peers.remove(id);
}

shared_ptr<PortalsPeer> PortalsPeersManager::peekPeer()
{
   getPortal()->ensureBootstrap();

   OS_LOCK(m_cs);

   if((m_peerIndex + 1) >= m_peers.size())
	   m_peerIndex = 0;

   shared_ptr<PortalsPeer> peer;
   if(m_peers.empty() == false)
   {
	   OS_ASSERT(m_peerIndex < m_peers.size());
	   peer = m_peers.value_at(m_peerIndex);
	   m_peerIndex++;
   }

   return peer;
}

void PortalsPeersManager::addSupernode(shared_ptr<IPAddress> address)
{
	OS_LOCK(m_cs);

	shared_ptr<PortalsPeer> peer = m_peers.get(PortalsPeer::makeID(address));
	if(peer != nullptr)	// Il nodo potrebbe essere nullptr in quanto rimosso per i limiti della cache
	{
		OS_ASSERT(peer->getRetries() == 0);
		peer->setSupernode(true);
	}
}

void PortalsPeersManager::extractSupernodes(uint32 count, list<shared_ptr<PortalsPeer> >::type &peers)
{
	// URGENT: da testare

	if(count == 0)
	{
		OS_ASSERTFALSE();
		return;
	}

	OS_LOCK(m_cs);

	if((m_supernodeIndex + 1) >= m_peers.size())
		m_supernodeIndex = 0;

	if(m_peers.empty())
		return;

	uint32 i = m_supernodeIndex;
	do
	{
		shared_ptr<PortalsPeer> peer = m_peers.value_at(i);
		OS_ASSERT(peer != nullptr);
		if(peer->getSupernode())
			peers.push_back(peer);

		i++;
		if(i == m_peers.size())
			i = 0;
	}
	while(i != m_supernodeIndex && count > static_cast<uint32>(peers.size()));

	m_supernodeIndex = i;
}

void PortalsPeersManager::getPeersInfo(uint32 &activePeers, uint32 &activeSupernodes)
{
	OS_LOCK(m_cs);

	OS_ASSERT(m_peersCS != nullptr);
	OS_LOCK(*m_peersCS);

	activePeers = 0;
	activeSupernodes = 0;

	DateTime contactDateCheck = DateTime::now() + TimeDuration::minutes(Options::instance()->getOption<uint32>(Options::portals_options::peers_contact_date_check));

	for(Peers::iterator i = m_peers.begin(); i != m_peers.end(); ++i)
	{
		shared_ptr<PortalsPeer> peer = i->second;

		DateTime lastContactDate = peer->getLastContactDate();
		if(lastContactDate.isValid())
		{
			OS_ASSERT(peer->getRetries() == 0);

			if(lastContactDate <= contactDateCheck)
			{
				activePeers++;

				if(peer->getSupernode())
					activeSupernodes++;
			}
		}
	}
}

bool PortalsPeersManager::loadPeers(shared_ptr<IPortalDatabase> database)
{
	OS_ASSERT(database != nullptr);

	shared_ptr<DbSqlSelect> selectStatement(OS_NEW DbSqlSelect(DBTABLES::PEERS));
	selectStatement->limit.setCount(getPeersCacheSize());
	selectStatement->fields.add(DBTABLES::PEERS::IP);
	selectStatement->fields.add(DBTABLES::PEERS::PORT);
	
	DataTable result;
	if(database->execute(selectStatement, result) == false)
		return false;

	for(uint32 i = 0; i < result.rows(); i++)
	{
		const DataTableRow &current = result[i];
		storePeer(*current[DBTABLES::PEERS::IP], *current[DBTABLES::PEERS::PORT], 0, OS_PEER_ORIGIN_CACHE, false, false);
	}

	limitPeers();

	OS_LOCK(m_cs);
	m_peerIndex = RandomNumberGenerator::instance()->generate<uint32>(0, static_cast<uint32>(m_peers.size()));

	return true;	
}

bool PortalsPeersManager::savePeers(shared_ptr<IPortalDatabase> database)
{
	OS_ASSERT(database != nullptr);

	OS_LOCK(m_cs);

	if(database->execute(String::format(_S("delete from %S").c_str(), DBTABLES::PEERS.c_str())) == false)
		return false;

	for(Peers::iterator i = m_peers.begin(); i != m_peers.end(); ++i)
	{
		shared_ptr<PortalsPeer> peer = i->second;
		// 24/03/10: Osiris 0.14 salva nel db solo i nodi validi
		if(peer->getCacheable())
		{
			if(database->execute(String::format(_S("insert into %S (%S, %S) values (%S, %S)").c_str(), DBTABLES::PEERS.c_str(), DBTABLES::PEERS::IP.c_str(), DBTABLES::PEERS::PORT.c_str(), Convert::toSQL(peer->getIP()).c_str(), Convert::toSQL(peer->getPort()).c_str())) == false)
				return false;
		}
	}

	return true;
}

bool PortalsPeersManager::storePeer(const String &ip, uint32 port, uint32 retries, const String &origin, bool checkLimit, bool validate)
{
	// Filtra l'indirizzo locale
	if(NetworkSystem::instance()->validateIP(ip) == false || NetworkSystem::instance()->isLocalAddress(ip)
#ifndef OS_DEBUG
		// In release filtra anche gli indirizzi privati
		|| NetworkSystem::instance()->isPrivateAddress(ip)
#endif // OS_DEBUG
	)
		return false;

	{
		OS_LOCK(m_cs);

		// URGENT: gestire il caso in cui venga lanciata un'eccezione per un IP non valido

		OS_ASSERT(m_peersCS != nullptr);

		shared_ptr<PortalsPeer> peer(OS_NEW PortalsPeer(getPortal(), m_peersCS, ip, port, retries, origin));
		uint64 id = peer->getID();

		shared_ptr<PortalsPeer> existingPeer = m_peers.get(id);
		if(existingPeer != nullptr)
		{
			if(validate)
				existingPeer->validate();
		}
		else
		{
			m_peers.push_back(id, peer);
		}
	}

	if(checkLimit)
		limitPeers();

	return true;
}

void PortalsPeersManager::limitPeers()
{
	OS_LOCK(m_cs);

	// URGENT: se c'è corrispondenza nella collection dei supernodi andrebbero rimossi anche li?

	OS_ASSERT(m_peersCacheSize > 0);

	// Verifica che il numero attuale di peers sia oltre la soglia
	if(m_peers.size() <= m_peersCacheSize)
		return;

	deque<uint64>::type invalidPeers;

	// Tenta di rimuovere prima i nodi invalidi
	for(Peers::iterator i = m_peers.begin(); (i != m_peers.end()) && ((m_peers.size() - invalidPeers.size()) > m_peersCacheSize); ++i)
	{
		if(i->second->getRetries() > 0)
			invalidPeers.push_back(i->first);	// TODO: temporaneo finchè non si sostituisce shared_collection con ordered_map (eliminando qui la chiave falserebbe l'iteratore altrimenti)
	}

	while(invalidPeers.empty() == false)
	{
		m_peers.remove(utils::pop_front(invalidPeers));
	}

	// Elimina i nodi più vecchi
	utils::pop_front_until(m_peers, m_peersCacheSize);
}
*/
//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
