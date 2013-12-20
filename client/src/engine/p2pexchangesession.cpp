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
#include "p2pexchangesession.h"

#include "iportaldatabase.h"
#include "lock.h"

#include "objectsiobject.h"
#include "p2pexchangecontext.h"
#include "p2prequest.h"
#include "portalsportal.h"
#include "portalstransaction.h"
#include "realtimestats.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {

//////////////////////////////////////////////////////////////////////

ExchangeSession::ExchangeSession(exchange_context_ptr context)
{
	OS_ASSERT(context != nullptr);
	m_context = context;

	// Inizializza la richiesta di default
	addRequest(rtQuery);
}

ExchangeSession::~ExchangeSession()
{
	// Gli oggetti devo essere stati serializzati prima della chiamata al distruttore a meno che il portale non sia stato rimosso (caso per cui potrebbe essere nullptr)
	OS_ASSERT(getNeedSerialization() == false || getPortal() == nullptr);
}

exchange_context_ptr ExchangeSession::getContext() const
{
	return m_context.lock();
}

shared_ptr<Portal> ExchangeSession::getPortal() const
{
	exchange_context_ptr context = getContext();
	if(context == nullptr)
		return nullptr;

	return context->getPortal();
}

bool ExchangeSession::getNeedSerialization() const
{
	OS_LOCK(m_cs);

	if(m_pendings.empty() == false)
		return true;

	for(States::const_iterator i = m_states.begin(); i != m_states.end(); ++i)
	{
		if(i->second == idRequested)
            return true;
	}

	return false;
}

bool ExchangeSession::isKnownID(const ObjectID &id) const
{
	OS_LOCK(m_cs);

	return m_knownIDs.find(id) != m_knownIDs.end();
}

bool ExchangeSession::isPendingID(const ObjectID &id) const
{
	OS_LOCK(m_cs);

	return m_pendings.exists(id);
}

bool ExchangeSession::isRequestedID(const ObjectID &id) const
{
	OS_LOCK(m_cs);

	// Ottiene la richiesta di oggetti corrente
	request_ptr request = m_requests.get(rtObjects);
	// Se esiste una richiesta di oggetti, verifica che figuri quello specificato
	return request != nullptr ? request->objectExists(id.toUTF16()) : false;
}

bool ExchangeSession::isIncomingID(const ObjectID &id) const
{
	if(isPendingID(id))
		return true;	// L'oggetto si trova nella cache, verr serializzato in seguito

	if(isRequestedID(id))
		return true;	// L'oggetto  stato richiesto, verr ricevuto in seguito

	return false;		// L'oggetto non  previsto nella sessione
}

bool ExchangeSession::needObject(shared_ptr<IPortalDatabase> database, const ObjectID &id, const DateTime &submit_date) const
{
	// Controlla se l'id  nella cache
	if(isPendingID(id))
		return false;	// L'oggetto non  necessario

	// Controlla se l'oggetto figura tra le richieste attuali
	if(isRequestedID(id))
		return true;	// L'oggetto  necessario

	// Controlla se l'oggetto esiste a livello di database
	return getContext()->needObject(database, id, submit_date);
}

void ExchangeSession::setIDKnown(const ObjectID &id)
{
	OS_LOCK(m_cs);

	OS_ASSERT(id.empty() == false);
	m_knownIDs.insert(id);
}

void ExchangeSession::setIDRequested(const ObjectID &id)
{
	return _setIDState(id, idRequested);
}

void ExchangeSession::setIDRefused(const ObjectID &id)
{
	return _setIDState(id, idRefused);
}

request_ptr ExchangeSession::addRequest(RequestType type)
{
	OS_LOCK(m_cs);

	request_ptr request = m_requests.get(type);
	if(request == nullptr)
	{
		request.reset(OS_NEW Request(type));
		m_requests.push_back(type, request);
	}

	return request;
}

void ExchangeSession::removeRequest(RequestType type)
{
	OS_LOCK(m_cs);

	m_requests.remove(type);
}

void ExchangeSession::peekRequest(request_ptr &request)
{
	OS_LOCK(m_cs);

	if(m_requests.empty() == false)
	{
		request = m_requests.front();
		m_requests.remove(request->getType());
	}
}

bool ExchangeSession::storeObject(shared_ptr<IPortalDatabase> database, shared_ptr<ObjectsIObject> object)
{
	OS_LOCK(m_cs);

	// Serializza l'oggetto
	bool result = getContext()->storeObject(database, object);
	// Annulla un'eventuale richiesta sull'oggetto
	removeObjectRequest(object->id);

	RealtimeStatsManager::signal(_S("Objects"), _S("Stored objects"), 1, RealtimeStatsCounter::ctRelative, false, _S("objects")); // URGENT

	return result;
}

void ExchangeSession::cacheObject(shared_ptr<ObjectsIObject> object)
{
	OS_LOCK(m_cs);

	if(m_pendings.exists(object->id) == false)
		m_pendings.push_back(object->id, object);

	// Annulla un'eventuale richiesta sull'oggetto
	removeObjectRequest(object->id);
}

bool ExchangeSession::requireObject(shared_ptr<IPortalDatabase> database, const ObjectID &id, const DateTime &submit_date)
{
	// Verifica che l'oggetto sia necessario
	if(needObject(database, id, submit_date) == false)
		return false;

	// Richiede l'oggetto
	addObjectRequest(id);
	return true;
}

void ExchangeSession::addObjectRequest(const ObjectID &id)
{
	OS_LOCK(m_cs);

	// Crea la richiesta di oggetti se necessario
	request_ptr request = addRequest(rtObjects);
	if(request != nullptr)
	{
		// Accoda alla richiesta l'oggetto specificato
		request->addObject(id.toUTF16());
	}
}

void ExchangeSession::removeObjectRequest(const ObjectID &id)
{
	OS_LOCK(m_cs);

	// Ottiene la richiesta di oggetti corrente
	request_ptr request = m_requests.get(rtObjects);
	// Verifica che esista un richiesta di oggetti
	if(request != nullptr)
	{
		// Rimuove l'id specificato dalla richiesta
		request->removeObject(id.toUTF16());
		// Controlla se nella richiesta non figura pi alcun id
		if(request->hasObjects() == false)
		{
			// Resetta la richiesta
			removeRequest(rtObjects);
		}
	}
}

void ExchangeSession::finalize(shared_ptr<Portal> portal)
{
    if(portal == nullptr)
    {
        OS_ASSERTFALSE();
        return;
    }

	bool needSerialization = getNeedSerialization();

	shared_ptr<PortalsTransaction> databaseWork;
	shared_ptr<IPortalDatabase> database;
	if(needSerialization)
	{
		databaseWork = portal->startTransaction(true);
		database = databaseWork->getDatabase();
		database->beginTransaction();
	}

	OS_LOCK(m_cs);

	if(needSerialization)
	{
		_processPendings(database);
		_updateRanks(database);

		database->commit();
	}

	m_knownIDs.clear();
}

void ExchangeSession::_processPendings(shared_ptr<IPortalDatabase> database)
{
	// teoricamente gli utenti andrebbero serializzati prima degli altri oggetti quindi o si scorre prima la lista alla ricerca
	// degli utenti o in fase di salvataggio (forse  meglio) si decide se salvare l'oggetto in testa (utente) o in coda (altri oggetti)

	for(ObjectsCollection::iterator i = m_pendings.begin(); i != m_pendings.end(); ++i)
	{
		shared_ptr<ObjectsIObject> object = i->second;
		storeObject(database, object);
	}

	m_pendings.clear();
}

void ExchangeSession::_updateRanks(shared_ptr<IPortalDatabase> database)
{
	while(m_states.empty() == false)
	{
		States::iterator i = m_states.begin();
		switch(i->second)
		{
		case idRequested:		// N.B.: se l'oggetto  stato richiesto vuol dire che ci sono più possibilità che l'oggetto non sia diffuso,
								// pertanto decrementa il rank dell'entry per facilitare la diffusione
								getContext()->increaseEntryRank(database, i->first);
								RealtimeStatsManager::signal(_S("Objects"), _S("Requested objects"), 1, RealtimeStatsCounter::ctRelative, false, _S("objects")); // URGENT
								break;

		case idRefused:			// In fase di estrazione il rank dell'entry viene già incrementato, pertanto non serve incrementarlo nuovamente
								RealtimeStatsManager::signal(_S("Objects"), _S("Refused objects"), 1, RealtimeStatsCounter::ctRelative, false, _S("objects")); // URGENT
								break;

		default:				OS_ASSERTFALSE();
								break;
		}

		m_states.erase(i);
	}
}

void ExchangeSession::_setIDState(const ObjectID &id, IDState state)
{
	OS_LOCK(m_cs);

	OS_ASSERT(id.empty() == false);
	m_states[id] = state;
}

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
