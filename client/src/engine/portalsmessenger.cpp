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
#include "portalsmessenger.h"

#include "convert.h"
#include "iportaldatabase.h"
#include "idbconnection.h"
#include "dbsqlselect.h"
#include "dbtables.h"
#include "datalocalmessage.h"
#include "engine.h"
#include "ideaccount.h"
#include "idesession.h"
#include "lock.h"
#include "objectsmessage.h"
#include "objectsprivatemessage.h"
#include "objectsuser.h"
#include "portalsaccount.h"


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace portals {

//////////////////////////////////////////////////////////////////////

Messenger::Messenger(shared_ptr<Portal> portal) : m_portal(portal)
{

}

Messenger::~Messenger()
{

}

uint32 Messenger::getMessagesCount(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, MessageFolder folder) const
{
	// Estrae il numero di messaggi nella directory specificata
	return _getMessagesCount(database, session, folder, messageStatusNone);
}

uint32 Messenger::getNewMessagesCount(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session) const
{
	// Estrae i nuovi messaggi in qualunque directory
	return _getMessagesCount(database, session, messageFolderNone, messageStatusNew);
}

shared_ptr<DataLocalMessage> Messenger::getMessage(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, const ObjectID &id, bool markAsRead) const
{
	// N.B.: qui in teoria la validazione dell'utente non serve visto che si punta ad un messaggio specifico

	OS_LOCK(m_cs);

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::LOCAL_MESSAGES_TABLE));
	select->where.add(DBTABLES::LOCAL_MESSAGES::PROFILE, Convert::toSQL(session->getAccount()->getID()));
	select->where.add(DBTABLES::LOCAL_MESSAGES::ID, Convert::toSQL(id));
	select->limit.setCount(1);

	DataTable result;
	if(database->execute(select, result) == false)
		return nullptr;

	if(result.rows() != 1)
		return nullptr;

	shared_ptr<DataLocalMessage> message(OS_NEW DataLocalMessage());
	if(message->read(result[0]) == false)
		return nullptr;

	if(markAsRead && message->getStatus() != messageStatusRead)
	{
		message->setStatus(messageStatusRead);
		message->read_date = DateTime::now();

		database->updateRecord(message);
	}

	return message;
}

bool Messenger::getMessages(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, DataLocalMessage::DataLocalMessageList &messages_list, MessageFolder folder, const RangeUint32 &range) const
{
	// Trattandosi di una richiesta in un range  necessario prima assicurare che l'utente sia validato
	_validate(database, session);

	OS_LOCK(m_cs);

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::LOCAL_MESSAGES_TABLE));
	select->where.add(DBTABLES::LOCAL_MESSAGES::PROFILE, Convert::toSQL(session->getAccount()->getID()));
	select->where.add(DBTABLES::LOCAL_MESSAGES::STATUS, Convert::toSQL(static_cast<uint32>(messageStatusDeleted)), DbSqlCondition::cfDifferent | DbSqlCondition::cfAnd);
	if(folder != messageFolderNone)
		select->where.add(DBTABLES::LOCAL_MESSAGES::FOLDER, Convert::toSQL(static_cast<uint32>(folder)));

	select->orderBy.add(DBTABLES::LOCAL_MESSAGES::SUBMIT_DATE, DbSqlField::foDesc);
	select->limit.setRange(range.start(), range.size());

	DataTable result;
	if(database->execute(select, result) == false)
		return false;

	for(uint32 i = 0; i < result.rows(); i++)
	{
		shared_ptr<DataLocalMessage> message(OS_NEW DataLocalMessage());
		if(message->read(result[i]))
			messages_list.push_back(message);
	}

	return true;
}

uint32 Messenger::getPendingMessages(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, bool peek) const
{
	_validate(database, session);

	OS_LOCK(m_cs);

	uint32 pendings = 0;

	ObjectID user = session->getUserID();

	PendingMessages::iterator i = m_pendingMessages.find(user.toUTF16());
	if(i != m_pendingMessages.end())
	{
		pendings = i->second;
		if(peek)
			i->second = 0;
	}

	return pendings;
}

void Messenger::handleMessage(shared_ptr<ObjectsPrivateMessage> private_message)
{
	_invalidate(private_message->adressee);
}

void Messenger::markNewMessagesAsUnread(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session) const
{
	_validate(database, session);

	OS_LOCK(m_cs);

	String sql = String::format(_S("update os_local_messages set status=%d, read_date=%S where profile=%S and status=%d").c_str(),
									messageStatusUnread,
									Convert::toSQL(DateTime::now()).c_str(),
									Convert::toSQL(session->getAccount()->getID()).c_str(),
									messageStatusNew);

	database->execute(sql);
}

void Messenger::changeMessagesStatus(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, MessageFolder folder, MessageStatus status) const
{
	_validate(database, session);

	OS_LOCK(m_cs);

	String sql = String::format(_S("update os_local_messages set status=%d where profile=%S and folder=%d").c_str(),
									status,
									Convert::toSQL(session->getAccount()->getID()).c_str(),
									folder);

	database->execute(sql);
}

void Messenger::moveFolderMessages(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, MessageFolder from, MessageFolder to) const
{
	_validate(database, session);

	OS_LOCK(m_cs);

	String sql = String::format(_S("update os_local_messages set folder=%d where profile=%S and folder=%d").c_str(),
									to,
									Convert::toSQL(session->getAccount()->getID()).c_str(),
									from);

	database->execute(sql);
}

void Messenger::deleteFolderMessages(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, MessageFolder folder) const
{
	_validate(database, session);

	OS_LOCK(m_cs);

	String sql = String::format(_S("update os_local_messages set status=%d where profile=%S and folder=%d").c_str(),
									messageStatusDeleted,
									Convert::toSQL(session->getAccount()->getID()).c_str(),
									folder);

	database->execute(sql);
}

bool Messenger::createPrivateMessage(const shared_ptr<ObjectsMessage> &message, const shared_ptr<ObjectsUser> &adressee, shared_ptr<IPortalDatabase> database, const shared_ptr<IdeSession> &user, ObjectID &id, bool saveCopy)
{
	Buffer session;
	Buffer content;
	if(message->encode(adressee->public_key, session, content) == false)
		return false;

	shared_ptr<ObjectsPrivateMessage> private_message(OS_NEW ObjectsPrivateMessage());
	private_message->author = user->getUserID();
	private_message->id = id;
	private_message->adressee = adressee->id;
	private_message->session = session;
	private_message->content = content;
	private_message->create(database, user); // CLODOURGENT valore di ritorno

	/*
	DateTime now;
	if(Engine::instance()->getSynchronizedDateTime(now) == false)
		return false;

	if(database->__createPrivateMessage(session, content, adressee, user->getUserID().toUTF16(), user->getPrivateKey(), id, now) == false)
		return false;
	*/

	if(saveCopy)
	{
		Buffer sessionCopy;
		Buffer contentCopy;
		if(message->encode(user->getPublicKey(), sessionCopy, contentCopy) == false)
			return false;

		shared_ptr<DataLocalMessage> messageCopy(OS_NEW DataLocalMessage());
		messageCopy->profile = user->getAccount()->getID();
		messageCopy->id = id;
		messageCopy->author = user->getUserID();
		messageCopy->session = sessionCopy;
		messageCopy->content = contentCopy;
		messageCopy->submit_date = private_message->submit_date;
		messageCopy->read_date = private_message->submit_date;
		messageCopy->status = messageStatusRead;
		messageCopy->folder = messageFolderSentItems;

		if(database->insertRecord(messageCopy) == false)
			return false;
	}

	return true;
}

void Messenger::moveMessageToInbox(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, const ObjectID &id) const
{
	_changeMessageFolder(database, session, id, messageFolderInbox);
}

void Messenger::moveMessageToTrash(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, const ObjectID &id) const
{
	_changeMessageFolder(database, session, id, messageFolderTrash);
}

uint32 Messenger::_getMessagesCount(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, MessageFolder folder, MessageStatus status) const
{
	_validate(database, session);

	OS_LOCK(m_cs);

    shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::LOCAL_MESSAGES_TABLE));
	select->count = true;
	select->where.add(DBTABLES::LOCAL_MESSAGES::PROFILE, Convert::toSQL(session->getAccount()->getID()));

	if(folder != messageFolderNone)
		select->where.add(DBTABLES::LOCAL_MESSAGES::FOLDER, Convert::toSQL(static_cast<uint32>(folder)));

	if(status == messageStatusNone)
		select->where.add(DBTABLES::LOCAL_MESSAGES::STATUS, Convert::toSQL(static_cast<uint32>(messageStatusDeleted)), DbSqlCondition::cfDifferent | DbSqlCondition::cfAnd);
	else
		select->where.add(DBTABLES::LOCAL_MESSAGES::STATUS, Convert::toSQL(static_cast<uint32>(status)));

	return database->getConnection()->value_of(select);
}

void Messenger::_changeMessageFolder(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session, const ObjectID &id, MessageFolder folder) const
{
	OS_LOCK(m_cs);

	String sql = String::format(_S("update os_local_messages set folder=%d where profile=%S and id=%S").c_str(),
									folder,
									Convert::toSQL(session->getAccount()->getID()).c_str(),
									Convert::toSQL(id.toUTF16()).c_str());

	database->execute(sql);
}

void Messenger::_validate(shared_ptr<IPortalDatabase> database, shared_ptr<IdeSession> session) const
{
	OS_LOCK(m_cs);

	ObjectID user = session->getUserID();

	UsersStatus::iterator i = m_usersStatus.find(user.toUTF16());
	if(i == m_usersStatus.end())
	{
		std::pair<UsersStatus::iterator, bool> result = m_usersStatus.insert(UsersStatus::value_type(user.toUTF16(), false));
		if(result.second == false)
			return;

		i = result.first;
	}

	// Controlla se l'utente  gi stato validato
	if(i->second)
		return;

	// Valida lo stato dell'utente
	i->second = true;

	shared_ptr<IdeAccount> account = session->getAccount(); // CLODOURGENT adressee e non destination

	String sql = String::format(_S("insert into os_local_messages (profile, id, author, destination, session, content, submit_date, read_date, status, folder) select %S, id, author, adressee, session, content, submit_date, %S, %d, %d from os_private_messages pm where pm.adressee = %S and pm.id not in (select id from os_local_messages where profile = %S)").c_str(),
							   Convert::toSQL(account->getID()).c_str(),
							   Convert::toSQL(DateTime::EMPTY.toString()).c_str(),
							   messageStatusNew,
							   messageFolderInbox,
							   Convert::toSQL(user.toUTF16()).c_str(),
							   Convert::toSQL(account->getID()).c_str());

	// Aggiorna il numero di messaggi privati per l'account
	uint32 count = database->getConnection()->execute(sql);
	// Salva il numero di messaggi non segnalati all'account
	m_pendingMessages[user.toUTF16()] = count;
}

void Messenger::_invalidate(const ObjectID &user)
{
	OS_LOCK(m_cs);

	// Nota: se l'utente non esiste non serve inserirlo nella mappa perch potrebbe non essere associato ad un account locale,
	// se in seguito verr chiamata la validate su tale id verr inserito e validato

	UsersStatus::iterator i = m_usersStatus.find(user.toUTF16());
	if(i != m_usersStatus.end())
		i->second = false;
}

//////////////////////////////////////////////////////////////////////

} // portals
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
