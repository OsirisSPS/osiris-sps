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

#ifndef _OS_P2P_EXCHANGECONTEXT_H
#define _OS_P2P_EXCHANGECONTEXT_H

#include "boost/thread/recursive_mutex.hpp"
#include "datetime.h"
#include "datadata.h"
#include "dataentry.h"
#include "dbdb.h"
#include "enable_this_ptr.h"
#include "base/object.h"
#include "objectsobjects.h"
#include "p2pp2p.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPAddress;
class IPortalDatabase;
class ObjectID;
class Portal;

//////////////////////////////////////////////////////////////////////

namespace p2p {

//////////////////////////////////////////////////////////////////////

class EngineExport ExchangeContext : public Object,
									 public enable_this_ptr<ExchangeContext>
{
	friend class ExchangeSession;

	typedef list<exchange_session_ptr>::type Sessions;

// Construction
public:
	ExchangeContext(shared_ptr<Portal> portal);
	virtual ~ExchangeContext();

// Attributes
public:
	inline shared_ptr<Portal> getPortal() const;

// Operations
public:
	// Crea una nuova sessione
	exchange_session_ptr createSession();
	// Rimuove una sessione
	void removeSession(exchange_session_ptr session);

	shared_ptr<DataEntry> getEntry(shared_ptr<IPortalDatabase> db, const ObjectID &id) const;

	bool extractEntriesByRank(shared_ptr<IPortalDatabase> db, uint32 start, uint32 count, DataEntry::DataEntryList &entries);
	bool extractEntriesByID(shared_ptr<IPortalDatabase> db, uint32 count, DataEntry::DataEntryList &entries);
/*
	bool extractPeers(shared_ptr<IPortalDatabase> db, uint32 count, data::peers_list &peers, bool validOnly);

	bool savePeer(shared_ptr<IPortalDatabase> db, shared_ptr<IPAddress> address, bool validateIfExists);
	void removePeer(shared_ptr<IPortalDatabase> db, shared_ptr<IPAddress> address);

	bool increasePeerRank(shared_ptr<IPortalDatabase> db, data::peer_ptr peer);
	bool decreasePeerRank(shared_ptr<IPortalDatabase> db, data::peer_ptr peer);

	void validatePeer(shared_ptr<IPortalDatabase> db, shared_ptr<IPAddress> address);
	void invalidatePeer(shared_ptr<IPortalDatabase> db, shared_ptr<IPAddress> address);
*/
	// Effettua la richiesta di un oggetto se non attualmente richiesto da altre sessioni e se necessario
	bool requestObject(shared_ptr<IPortalDatabase> database, exchange_session_ptr session, const ObjectID &id, const DateTime &submit_date = DateTime::EMPTY);

private:
	bool needObject(shared_ptr<IPortalDatabase> db, const ObjectID &id, const DateTime &submit_date) const;
	bool storeObject(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object);

	void decreaseEntryRank(shared_ptr<IPortalDatabase> db, const ObjectID &id);
	void increaseEntryRank(shared_ptr<IPortalDatabase> db, const ObjectID &id);

private:
	mutable boost::recursive_mutex m_dbCS;
	mutable boost::recursive_mutex m_sessionsCS;
	Sessions m_sessions;
	weak_ptr<Portal> m_portal;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<Portal> ExchangeContext::getPortal() const { return m_portal.lock(); }

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_EXCHANGECONTEXT_H
