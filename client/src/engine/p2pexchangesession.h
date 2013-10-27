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

#ifndef _OS_P2P_EXCHANGESESSION_H
#define _OS_P2P_EXCHANGESESSION_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "datetime.h"
#include "dbdb.h"
#include "objectsobjects.h"

#include "p2pp2p.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class Portal;

//////////////////////////////////////////////////////////////////////

namespace p2p {

//////////////////////////////////////////////////////////////////////

class EngineExport ExchangeSession : public Object
{
// Enumerated types
public:
	enum IDState
	{
		idRequested = 1,
		idRefused   = 2,
	};

// Types
public:
	typedef unordered_map<ObjectID, IDState>::type States;
	typedef unordered_set<ObjectID>::type KnownIDs;
	typedef Collections<RequestType, Request>::shared_collection Requests;
	typedef Collections<ObjectID, ObjectsIObject>::shared_collection ObjectsCollection;

// Construction
public:
	ExchangeSession(exchange_context_ptr context);
	virtual ~ExchangeSession();

// Attributes
public:
	exchange_context_ptr getContext() const;
	shared_ptr<Portal> getPortal() const;

	bool getNeedSerialization() const;

	// Indica se l'oggetto  conosciuto dalla sessione remota
	bool isKnownID(const ObjectID &id) const;
	// Indica se l'oggetto si trova nella cache
	bool isPendingID(const ObjectID &id) const;
	// Indica se l'oggetto  stato richiesto
	bool isRequestedID(const ObjectID &id) const;
	// Indica se l'oggetto  in attesa di essere serializzato/ricevuto
	bool isIncomingID(const ObjectID &id) const;

	bool needObject(shared_ptr<IPortalDatabase> database, const ObjectID &id, const DateTime &submit_date = DateTime::EMPTY) const;

// Operations
public:
	// Marca l'id come conosciuto a livello di sessione
	void setIDKnown(const ObjectID &id);
	// Marca l'id come richiesto dalla sessione remota
	void setIDRequested(const ObjectID &id);
	// Marca l'id come rifiutato dalla connessione remota
	void setIDRefused(const ObjectID &id);

	// Accoda una nuova richiesta
	request_ptr addRequest(RequestType type);
	// Rimuove una richiesta
	void removeRequest(RequestType type);
	// Carica la richiesta corrente
	void peekRequest(request_ptr &request);

	// Serializza un oggetto
	bool storeObject(shared_ptr<IPortalDatabase> database, shared_ptr<ObjectsIObject> object);
	// Inserisce un oggetto in cache per un successivo inserimento
	void cacheObject(shared_ptr<ObjectsIObject> object);

	// Richiede un oggetto
	bool requireObject(shared_ptr<IPortalDatabase> database, const ObjectID &id, const DateTime &submit_date = DateTime::EMPTY);

	// Accoda alle richieste attuali quella dell'oggetto (se la richiesta non esiste la crea)
	void addObjectRequest(const ObjectID &id);
	// Rimuove dalle richieste attuali quella dell'oggetto (se la richiesta risulta in seguito vuota, viene annullata)
	void removeObjectRequest(const ObjectID &id);

	// Finalizza la sessione inserendo eventuali oggetti in cache ed aggiornando i ranks
	void finalize(shared_ptr<Portal> portal);

private:
	void _setIDState(const ObjectID &id, IDState state);

	void _processPendings(shared_ptr<IPortalDatabase> database);
	void _updateRanks(shared_ptr<IPortalDatabase> database);

public:
	mutable boost::recursive_mutex m_cs;
	weak_ptr<ExchangeContext> m_context;
	KnownIDs m_knownIDs;				// ID noti alla sessione remota
	States m_states;					// Stato degli oggetti proposti (accettati/rifiutati)
	Requests m_requests;				// Elenco delle richieste da processare
	ObjectsCollection m_pendings;		// Cache degli oggetti da serializzare
};

//////////////////////////////////////////////////////////////////////

} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_EXCHANGESESSION_H
