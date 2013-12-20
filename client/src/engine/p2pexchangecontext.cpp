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
#include "p2pexchangecontext.h"

#include "convert.h"
#include "dataentry.h"
#include "datapeer.h"
#include "iportaldatabase.h"
#include "dbtables.h"
#include "dbsqlselect.h"
#include "ipaddress.h"
#include "objectsiobject.h"
#include "p2pexchangesession.h"
#include "portalsportal.h"
#include "lock.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

ExchangeContext::ExchangeContext(shared_ptr<Portal> portal) : m_portal(portal)
{

}

ExchangeContext::~ExchangeContext()
{

}

exchange_session_ptr ExchangeContext::createSession()
{
	OS_LOCK(m_sessionsCS);

	exchange_session_ptr session(OS_NEW ExchangeSession(get_this_ptr()));
	m_sessions.push_back(session);

	return session;
}

void ExchangeContext::removeSession(exchange_session_ptr session)
{
	OS_LOCK(m_sessionsCS);

	utils::remove(m_sessions, session);
}

shared_ptr<DataEntry> ExchangeContext::getEntry(shared_ptr<IPortalDatabase> db, const ObjectID &id) const
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	return db->getEntry(id);
}

bool ExchangeContext::extractEntriesByRank(shared_ptr<IPortalDatabase> db, uint32 start, uint32 count, DataEntry::DataEntryList &entries)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::ENTRIES_TABLE));
	select->where.add(DBTABLES::ENTRIES::RANK, Convert::toSQL(0), DbSqlCondition::cfMajor | DbSqlCondition::cfAnd);
	select->orderBy.add(DbSqlField(DBTABLES::ENTRIES::RANK), DbSqlField::foAsc);
	select->orderBy.add(DbSqlField(DBTABLES::ENTRIES::SUBMIT_DATE), DbSqlField::foDesc);
	select->limit.setRange(start, count);

	DataTable result;
	if(db->execute(select, result) == false)
		return false;

	for(uint32 i = 0; i < result.rows(); i++)
	{
		shared_ptr<DataEntry> entry(OS_NEW DataEntry());
		// Carica l'entry
		if(entry->read(result[i]))
		{
            entries.push_back(entry);
		}
	}

	return true;
}

bool ExchangeContext::extractEntriesByID(shared_ptr<IPortalDatabase> db, uint32 count, DataEntry::DataEntryList &entries)
{
    shared_ptr<Portal> portal = getPortal();
    if(portal == nullptr)
        return false;

	OS_ASSERT(db != nullptr);

	OS_LOCK(m_dbCS);

    ObjectID lastExchangedObject = portal->getLastExchangedObject();

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::ENTRIES_TABLE));
	select->where.add(DBTABLES::ENTRIES::RANK, Convert::toSQL(0));
	if(lastExchangedObject.empty() == false)
        select->where.add(DBTABLES::ENTRIES::ID, Convert::toSQL(lastExchangedObject.toUTF16()), DbSqlCondition::cfMajor | DbSqlCondition::cfAnd);

	select->orderBy.add(DbSqlField(DBTABLES::ENTRIES::ID), DbSqlField::foAsc);
	select->limit.setCount(count);

	// Resetta l'ID dell'ultimo oggetto scambiato nel caso in cui si debba ripartire dall'inizio
    lastExchangedObject.clear();

	bool done = false;

	DataTable result;
	if(db->execute(select, result))
	{
		done = true;

        for(uint32 i = 0; i < result.rows(); i++)
        {
            shared_ptr<DataEntry> entry(OS_NEW DataEntry());
            // Carica l'entry
            if(entry->read(result[i]))
            {
                entries.push_back(entry);
				// Salva l'ID dell'ultimo oggetto scambiato (non è il "back" della lista visto che potrebbe essere passata già valorizzata e qui non caricare alcun oggetto...)
				lastExchangedObject = entry->id;
            }
        }
	}

    portal->setLastExchangedObject(lastExchangedObject);

	return done;
}
/*
bool ExchangeContext::extractPeers(shared_ptr<IPortalDatabase> db, uint32 count, data::peers_list &peers, bool validOnly)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::PEERS));
	if(validOnly)
		select->where.add(DBTABLES::PEERS::RETRIES, Convert::toSQL(static_cast<uint32>(0)));

	select->orderBy.fields.add(DbSqlField(DBTABLES::PEERS::RANK));
	select->limit.setCount(count);

	DataTable result;
	if(db->execute(select, result) == false)
		return false;

	for(uint32 i = 0; i < result.rows(); i++)
	{
		data::peer_ptr peer(OS_NEW data::Peer());
		if(peer->read(result[i]))
		{
            peers.push_back(peer);
			// Aggiorna il rank del nodo
			db->increasePeerRank(peer);
		}
	}

	return true;
}

bool ExchangeContext::savePeer(shared_ptr<IPortalDatabase> db, shared_ptr<IPAddress> address, bool validateIfExists)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	return db->savePeer(address, validateIfExists);
}

void ExchangeContext::removePeer(shared_ptr<IPortalDatabase> db, shared_ptr<IPAddress> address)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	if(address != nullptr)
		db->removePeer(address->getHost());
}

bool ExchangeContext::increasePeerRank(shared_ptr<IPortalDatabase> db, data::peer_ptr peer)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	return db->increasePeerRank(peer);
}

bool ExchangeContext::decreasePeerRank(shared_ptr<IPortalDatabase> db, data::peer_ptr peer)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	return db->decreasePeerRank(peer);
}

void ExchangeContext::validatePeer(shared_ptr<IPortalDatabase> db, shared_ptr<IPAddress> address)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	if(address != nullptr)
		db->validatePeer(address->getHost());
}

void ExchangeContext::invalidatePeer(shared_ptr<IPortalDatabase> db, shared_ptr<IPAddress> address)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	if(address != nullptr)
		db->invalidatePeer(address->getHost());
}
*/
bool ExchangeContext::requestObject(shared_ptr<IPortalDatabase> database, exchange_session_ptr session, const ObjectID &id, const DateTime &submit_date)
{
	// N.B.: non spostare questa funzione sotto ExchangeSession perchè la chiamata a tutte le sessioni attive di isIncomingID
	// e l'accodamento dell'eventuale richiesta dell'oggetto devono essere loccate sotto lo stesso mutex (m_sessionsCS)

	OS_ASSERT(session != nullptr);
	OS_LOCK(m_sessionsCS);

	for(Sessions::const_iterator i = m_sessions.begin(); i != m_sessions.end(); ++i)
	{
		exchange_session_ptr currentSession = *i;
		// Controlla se l'oggetto specificato è in attesa su una sessione
		if(currentSession->isIncomingID(id))
			return false;		// Non serve richiedere nuovamente l'oggetto, verrà serializzato dalla sessione che l'ha richiesto in precedenza
	}

	// Controlla se l'oggetto è già presente nel database
	if(needObject(database, id, submit_date) == false)
		return false;			// Oggetto già presente, non serve richiederlo

	// Accoda la richiesta dell'oggetto alla sessione specifica
	session->addObjectRequest(id);
	return true;
}

bool ExchangeContext::needObject(shared_ptr<IPortalDatabase> db, const ObjectID &id, const DateTime &submit_date) const
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	return db->needObject(id.toUTF16(), submit_date);
}

bool ExchangeContext::storeObject(shared_ptr<IPortalDatabase> db, shared_ptr<ObjectsIObject> object)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	//return db->storeObject(object);
	return object->store(db).empty();
}

void ExchangeContext::decreaseEntryRank(shared_ptr<IPortalDatabase> db, const ObjectID &id)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	db->decreaseEntryRank(id);
}

void ExchangeContext::increaseEntryRank(shared_ptr<IPortalDatabase> db, const ObjectID &id)
{
	OS_ASSERT(db != nullptr);
	OS_LOCK(m_dbCS);

	db->increaseEntryRank(id);
}

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
