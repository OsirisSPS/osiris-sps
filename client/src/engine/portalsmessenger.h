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

#ifndef _PORTALS_MESSENGER_H
#define _PORTALS_MESSENGER_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "datadata.h"
#include "datalocalmessage.h"
#include "dbdb.h"
#include "objectsobjects.h"
#include "portalsportals.h"
#include "range.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class IdeSession;
class ObjectID;
class Portal;

//////////////////////////////////////////////////////////////////////

namespace portals {

//////////////////////////////////////////////////////////////////////

class EngineExport Messenger : public Object
{
private:
	typedef map<String, bool>::type UsersStatus;			// Associa l'id di un utente al suo stato (valido, non valido)
	typedef map<String, uint32>::type PendingMessages;		// Tiene traccia dei nuovi messaggi

// Construction
public:
	Messenger(shared_ptr<Portal> portal);
	virtual ~Messenger();

// Attributes
public:
	inline shared_ptr<Portal> getPortal() const;

	// Restituisce il numero di messaggi privati di un utente
	uint32 getMessagesCount(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, MessageFolder folder) const;
	// Restituisce il numero di nuovi messaggi di un utente
	uint32 getNewMessagesCount(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session) const;

	// Restituisce un messaggio privato
	shared_ptr<DataLocalMessage> getMessage(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, const ObjectID &id, bool markAsRead = false) const;
	// Restituisce i messaggi privati di un utente nel range specificato
	bool getMessages(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, DataLocalMessage::DataLocalMessageList &messages_list, MessageFolder folder = messageFolderNone, const RangeUint32 &range = RangeUint32(0, 0)) const;

	// Restituisce il numero di messaggi non segnalati per un utente (se peek == true il valore viene resettato)
	uint32 getPendingMessages(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, bool peek) const;

// Operations
public:
	// Gestisce l'inserimento di un messaggio privato
	void handleMessage(shared_ptr<ObjectsPrivateMessage> private_message);

	// Marca i nuovi messaggi di un utente come "non letti"
	void markNewMessagesAsUnread(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session) const;

	// Sposta i messaggi da un folder ad un altro
	void moveFolderMessages(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, MessageFolder from, MessageFolder to) const;
	// Marca i messaggi di un folder come cancellati definitivamente
	void deleteFolderMessages(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, MessageFolder folder) const;

	// Modifica lo stato di tutti i messaggi in un folder
	void changeMessagesStatus(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, MessageFolder folder, MessageStatus status) const;

	// Crea un messaggio privato
	bool createPrivateMessage(const shared_ptr<ObjectsMessage> &message, const shared_ptr<ObjectsUser> &adressee, shared_ptr<IPortalDatabase> database, const shared_ptr<IdeSession> &user, ObjectID &id, bool saveCopy);

	// Sposta un messaggio nel folder di posta in arrivo
	void moveMessageToInbox(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, const ObjectID &id) const;
	// Sposta un messaggio nel folder di posta eliminata
	void moveMessageToTrash(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, const ObjectID &id) const;

private:
	uint32 _getMessagesCount(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, MessageFolder folder, MessageStatus status) const;

	void _changeMessageFolder(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, const ObjectID &id, MessageFolder folder) const;

	void _validate(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session) const;
	void _invalidate(const ObjectID &user);

protected:
	mutable boost::recursive_mutex m_cs;
	mutable UsersStatus m_usersStatus;				// Mappa dello stato degli utenti (se la chiave non esiste vuol dire che l'utente  da validare)
	mutable PendingMessages m_pendingMessages;
	weak_ptr<Portal> m_portal;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<Portal> Messenger::getPortal() const { return m_portal.lock(); }

//////////////////////////////////////////////////////////////////////

} // portals
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _PORTALS_MESSENGER_H
