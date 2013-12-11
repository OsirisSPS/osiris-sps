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
#include "iportaldatabase.h"

#include "cryptmanager.h"
#include "dataaccount.h"
#include "datadiscussionstats.h"
#include "dataentry.h"
#include "datainstancecache.h"
#include "dataistatistics.h"
#include "datalocalmessage.h"
#include "datapollstats.h"
#include "datapolloptionstats.h"
#include "datauserstats.h"
#include "engine.h"
#include "entitiessnapshotmanager.h"
#include "isissystem.h"
#include "objectsattribute.h"
#include "objectsavatar.h"
#include "objectscalendarevent.h"
#include "objectsfile.h"
#include "objectsiobject.h"
#include "objectsinstance.h"
#include "objectsmodel.h"
#include "objectssystem.h"
#include "objectssection.h"
#include "objectssystem.h"
#include "objectstag.h"
#include "datapeer.h"
#include "objectssystem.h"
#include "objectspoll.h"
#include "objectspolloption.h"
#include "objectspollvote.h"
#include "objectspost.h"
#include "objectsprivatemessage.h"
#include "objectsreputation.h"
#include "objectstext.h"
#include "objectsuser.h"
#include "objectsvote.h"
#include "dbbinder.h"
#include "dbdb.h"
#include "idbconnection.h"
#include "idbparameter.h"
#include "idbstatement.h"
#include "dbdatabasessystem.h"
#include "dbsqldelete.h"
#include "dbsqlinsert.h"
#include "dbsqlselect.h"
#include "dbsqlupdate.h"
#include "dbtables.h"
#include "dataitem.h"
#include "ipaddress.h"
#include "log.h"
#include "platformmanager.h"
#include "portalsoptionsshared.h"
#include "portalsportal.h"
#include "realtimestats.h"
#include "timerperformance.h"

//////////////////////////////////////////////////////////////////////

// Nota: non modificare MAI questi valori!

#define OS_IDPARAM_USER_AVATAR			"os_avatar"
#define OS_IDPARAM_USER_REPUTATION		"os_reputation"
#define OS_IDPARAM_USER_POLL_VOTE		"os_poll_vote"
#define OS_IDPARAM_USER_VOTE			"os_vote"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IPortalDatabase::IPortalDatabase(shared_ptr<IDbConnection> connection) : m_binder(OS_NEW DbBinder())
{
	OS_EXCEPT_IF(connection == null, "Invalid connection");

	m_connection = connection;
	m_connected = false;

	m_query = 0;
	m_transaction = 0;
}

IPortalDatabase::~IPortalDatabase()
{
	close();
}

shared_ptr<ObjectsIDescriptor> IPortalDatabase::getDescriptor(PortalObjectType type) const
{
	return ObjectsSystem::instance()->getDescriptor(type);
}

bool IPortalDatabase::xxx_accountExists(const String &id) const
{
	return recordExists(DBTABLES::ACCOUNTS_TABLE, DBTABLES::ACCOUNTS::ID, Convert::toSQL(id));
}

shared_ptr<DataAccount> IPortalDatabase::xxx_getAccount(const String &username)
{
	return xxx_loadAccount(username);
}

shared_ptr<ObjectsPrivateMessage> IPortalDatabase::getPrivateMessage(const ObjectID &id)
{
	return objects_private_message_cast(loadObject(id));
}

shared_ptr<ObjectsUser> IPortalDatabase::getUser(const ObjectID &id)
{
	return objects_user_cast(loadObject(id));
}

shared_ptr<ObjectsReputation> IPortalDatabase::getReputation(const ObjectID &id)
{
	return objects_reputation_cast(loadObject(id));
}

shared_ptr<ObjectsAvatar> IPortalDatabase::getAvatar(const ObjectID &id)
{
	return objects_avatar_cast(loadObject(id));
}

shared_ptr<ObjectsFile> IPortalDatabase::getFile(const ObjectID &id)
{
	return objects_file_cast(loadObject(id));
}

shared_ptr<DataEntry> IPortalDatabase::getEntry(const ObjectID &id) const
{
	return loadEntry(id.toUTF16());
}

shared_ptr<ObjectsTag> IPortalDatabase::getTag(const ObjectID &id)
{
	return objects_tag_cast(loadObject(id));
}

bool IPortalDatabase::peerExists(const String &ip) const
{
	return recordExists(DBTABLES::PEERS_TABLE, DBTABLES::PEERS::IP, Convert::toSQL(ip));
}

shared_ptr<DataPeer> IPortalDatabase::getPeer(const String &ip) const
{
	return loadPeer(ip);
}

shared_ptr<ObjectsVote> IPortalDatabase::getVote(const ObjectID &id)
{
	return objects_vote_cast(loadObject(id));
}

shared_ptr<ObjectsPollVote> IPortalDatabase::getPollVote(const ObjectID &id)
{
	return objects_poll_vote_cast(loadObject(id));
}

uint32 IPortalDatabase::xxx_getAccounts() const
{
	return _getRecords(DBTABLES::ACCOUNTS_TABLE);
}

uint32 IPortalDatabase::getEntries() const
{
	return _getRecords(DBTABLES::ENTRIES_TABLE);
}

uint32 IPortalDatabase::getPeers() const
{
	return _getRecords(DBTABLES::PEERS_TABLE);
}

uint32 IPortalDatabase::getUsers() const
{
	return _getRecords(DBTABLES::USERS_TABLE);
}

bool IPortalDatabase::recordExists(const String &table, const String &field, const String &value) const
{
	StringMap condition;
	condition[field] = value;
	return recordExists(table, condition);
}

bool IPortalDatabase::recordExists(const String &table, const StringMap &conditions) const
{
	OS_ASSERT(conditions.empty() == false);

	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(table));
	select->count = true;
	for(StringMap::const_iterator i = conditions.begin(); i != conditions.end(); select->where.add(i->first, i->second), ++i);
	select->limit.setCount(1);

	return m_connection->value_of(select) > 0;
}

bool IPortalDatabase::needObject(const String &id, const DateTime &submit_date) const
{
	if(id.empty())
		return false;

	shared_ptr<DataEntry> existing = getEntry(id.to_ascii());
	if(existing == null)
		return true;	// L'entry non esiste pertanto l'oggetto  necessario...

	if(isUpdatable(existing->getObjectType()) == false) // CLODOURGENT
		return false;	// L'oggetto specificato non  aggiornabile, quindi non  necessario essendo gi disponibile

	// Controlla se  stata specificata una data valida
	if(submit_date != DateTime::EMPTY)
		// Restituisce true nel caso in cui la data specificata sia pi recente della versione disponibile
		return existing->submit_date < submit_date;

	return false;
}
/*
bool IPortalDatabase::getTags(const String &author, objects::tags_list &tags) const
{
	sql::select_ptr select(OS_NEW sql::Select(DBTABLES::TAGS));
	select->where.add(DBTABLES::TAGS::AUTHOR, Convert::toSQL(author));

	DataTable result;
	if(execute(select, result) == false)
		return false;

	for(uint32 i = 0; i < result.rows(); i++)
	{
		shared_ptr<ObjectsTag> tag(OS_NEW ObjectsTag());
		if(tag->read(result[i]))
			tags.push_back(tag);
	}

	return true;
}

bool IPortalDatabase::getPeers(uint32 start, uint32 count, data::peers_list &peers, bool orderByRank) const
{
	sql::select_ptr select(OS_NEW sql::Select(DBTABLES::PEERS));
	if(orderByRank)
		select->orderBy.fields.add(sql::Field(DBTABLES::PEERS::RANK));
	select->limit.setRange(start, count);

	DataTable result;
	if(execute(select, result) == false)
		return false;

	for(uint32 i = 0; i < result.rows(); i++)
	{
		data::peer_ptr peer(OS_NEW data::Peer());
		if(peer->read(result[i]))
			peers.push_back(peer);
	}

	return true;
}

bool IPortalDatabase::getReputations(const String &author, const double &thresholdListen, objects::reputations_list &reputations) const
{
	sql::select_ptr select(OS_NEW sql::Select(DBTABLES::REPUTATIONS));
	select->where.add(DBTABLES::REPUTATIONS::AUTHOR, Convert::toSQL(author));
	select->where.add(DBTABLES::REPUTATIONS::FOLLOW, Convert::toSQL(true));
	select->where.add(DBTABLES::REPUTATIONS::SCORE, Convert::toSQL(thresholdListen), DbSqlCondition::cfMajor | DbSqlCondition::cfAnd);

	DataTable result;
	if(execute(select, result) == false)
		return false;

	for(uint32 i = 0; i < result.rows(); i++)
	{
		shared_ptr<ObjectsReputation> ptr(OS_NEW ObjectsReputation());
		if(ptr->read(result[i]))
            reputations.push_back(ptr);
	}

	return true;
}
*/
/*
shared_ptr<ObjectsIObject> IPortalDatabase::getObject(const ObjectID &id)
{
	return loadObject(id);
}
*/

shared_ptr<ObjectsIObject> IPortalDatabase::loadObject(const ObjectID &id)
{
	RealtimeStatsScopeTimer RSSXX1(_S("Debug"), _S("Object Load"));

	PortalObjectType objectType = Convert::toObjectType(static_cast<uint32>(getConnection()->value_of(_S("select type from os_entries where id=") + Convert::toSQL(id.toUTF16()))));

	if(objectType == portalObjectTypeUnknown)
		return null;

	shared_ptr<ObjectsIDescriptor> descriptor = ObjectsSystem::instance()->getDescriptor(objectType);
	return descriptor != null ? descriptor->loadObject(get_this_ptr(), id) : null;
}

/*
shared_ptr<ObjectsIRevisionable> IPortalDatabase::getRevisionable(const ObjectID &id)
{
	return objects_revisionable_cast(loadObject(id));
}
*/

shared_ptr<DataInstanceCache> IPortalDatabase::getInstanceCache(const ObjectID &reference)
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::INSTANCES_CACHE_TABLE));
	select->where.add(DBTABLES::REFERENCE, Convert::toSQL(reference.toUTF16()));
	select->limit.setCount(1);

	return _recordFromSQL<DataInstanceCache>(select);
}

uint32 IPortalDatabase::getStableObjectsCount(const EntityID &reference, PortalObjectType type)
{
	String sql = String::format(_S("select count(*) from os_snapshot_objects where \
								os_snapshot_objects.parent = '%S' \
								and os_snapshot_objects.type = %d\
								and os_snapshot_objects.visible = 1").c_str(),
								reference.toUTF16().c_str(),
								type);

	DataTable table;
	if(execute(sql, table))
		return static_cast<uint32>(*table[0][0]);

	return 0;
}

bool IPortalDatabase::open(shared_ptr<Portal> portal)
{
	close();

	try
	{
		m_connection->open();
		m_connected = true;

		m_portal = portal;

		return true;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}

	return false;
}

void IPortalDatabase::close()
{
	m_connection->close();
	m_connected = false;

	m_portal.reset();

	OS_ASSERT(m_transaction == 0);
	m_transaction = 0;
	m_query = 0;
}

/*
void IPortalDatabase::parse(shared_ptr<DbSqlICommand> command, String &sql)
{
	m_connection->parse(command, sql);
}

shared_ptr<IDbStatement> IPortalDatabase::prepare(const String &sql)
{
	return m_connection->prepare(sql);
}
*/

bool IPortalDatabase::execute(const String &sql) const // TOCLEAN, chiamare getConnection()->execute
{
	_query();
	return (m_connection->execute(sql) != -1);
}

bool IPortalDatabase::execute(const String &sql, DataTable &result) const // TOCLEAN, chiamare getConnection()->execute
{
	//TimerPerformance TP(_S("IPortalDatabase::execute2 - ") + sql, 100);

	_query();
	return m_connection->query(sql, result);	
}

bool IPortalDatabase::execute(shared_ptr<DbSqlICommand> command) const // TOCLEAN, chiamare getConnection()->execute
{
	_query();
	//m_connection->parse(command, sql);
	return (m_connection->execute(command) != -1);	
}

bool IPortalDatabase::execute(shared_ptr<DbSqlICommand> command, DataTable &result) const // TOCLEAN, chiamare getConnection()->execute
{
	String sql;

	try
	{
		m_connection->_parse(command, sql);

		return execute(sql, result);
	}
	catch(std::exception &e)
	{
		logError(sql, e);
	}

	return false;
}



/*
uint32 IPortalDatabase::executeEx(const String &sql)
{
	_query();

	OS_ASSERT(m_connection != null);

	uint32 result = 0;
	try
	{
		result = m_connection->execute(sql);
	}
	catch(std::exception &e)
	{
		logError(sql, e);

		throw;
	}

	return result;
}

void IPortalDatabase::queryEx(const String &sql, DataTable &result)
{
	try
	{
		_query();
		m_connection->query(sql, result);
	}
	catch(std::exception &e)
	{
		logError(sql, e);

		throw;
	}
}
*/

void IPortalDatabase::addToList(const String &sql, StringList &list, bool append)
{
	if(!append)
		list.clear();

	DataTable result;
	execute(sql,result);

	OS_ASSERT(result.columns() == 1);
	for(uint32 r=0;r<result.rows();r++)
	{
		String value = result.get(r,0);
		list.push_back(value);
	}
}

bool IPortalDatabase::insertRecord(shared_ptr<DataIRecord> record)
{
	// Valida il record
	if(validateRecord(record) == false)
	{
#ifdef OS_DEBUG
		OS_ASSERTFALSE();
		bool result = validateRecord(record); // Per capire quando capita il perchè
#endif
		return false;
	}

	bool done = record->insert(m_connection);
	
	if(record->getRecordType() == dataRecordTypeObject)
	{
		shared_ptr<ObjectsIObject> object = objects_object_cast(record);
		/*
		if(object->insert(m_connection, object->getDescriptor()->getTableName()) == false)
			return false;
		*/

		if(_createEntry(object, getNewEntryRank()) == false)
		{
			removeObject(object);		// Chiama l'implementazione in modo da non lanciare l'evento
			return false;
		}

		ObjectsSystem::instance()->fireNewObject(object);
	}

	/*
	bool done = false;
	switch(record->getRecordType())
	{
	case dataRecordTypeAccount:			done = record->insert(m_connection, DBTABLES::ACCOUNTS_TABLE);
										break;
	
	case dataRecordTypeEntry:			done = record->insert(m_connection, DBTABLES::ENTRIES_TABLE);
										break;

	case dataRecordTypeObject:			done = _insertObject(objects_object_cast(record));
										break;

	case dataRecordTypePeer:			done = record->insert(m_connection, DBTABLES::PEERS_TABLE);
										break;

	case dataRecordTypeStatistics:		done = _insertStatistics(data::statistics_cast(record));
										break;

	case dataRecordTypeLocalMessage:	done = _insertLocalMessage(data::local_message_cast(record));
										break;

	case dataRecordTypeInstanceCache:	done = _insertInstanceCache(data::instance_cache_cast(record));
										break;

	default:						OS_ASSERTFALSE();
									break;
	}
	*/

	if(done)
	{
		// Richiama l'evento di inserimento dell'oggetto
		onInsertRecord(record);
	}
	
	return done;
}

bool IPortalDatabase::updateRecord(shared_ptr<DataIRecord> record)
{
	// Valida il record
	if(validateRecord(record) == false)
		return false;

	bool done = record->update(m_connection);

	if( (done) && (record->getRecordType() == dataRecordTypeObject) )
	{
		shared_ptr<ObjectsIObject> object = objects_object_cast(record);

		/*
		if(_updateObject(object->getDescriptor()->getTableName(), object) == false)
			return false;
		*/

		shared_ptr<DataEntry> entry = getEntry(object->id);
		// Carica l'entry associata all'oggetto
		if(entry != null)
		{
			entry->type = Convert::toUint32(object->getObjectType());		
			entry->submit_date = object->submit_date;
			entry->insert_date = DateTime::now();
			entry->rank = getNewEntryRank();

			// Effettua l'aggiornamento dell'entry
			//return _updateEntry(entry);
			done = entry->update(m_connection);
		}
		else
		{
			// Genera un nuovo entry associato all'oggetto
			done = _createEntry(object, getNewEntryRank());
		}
	}
	/*
	bool done = false;
	switch(record->getRecordType())
	{
	case dataRecordTypeAccount:			done = xxx__updateAccount(data::account_cast(record));
										break;
	
	case dataRecordTypeEntry:			done = _updateEntry(dataentry_cast(record));
										break;

	case dataRecordTypeObject:			done = _updateObject(objects_object_cast(record));
										break;

	case dataRecordTypePeer:			done = _updatePeer(data::peer_cast(record));
										break;

	case dataRecordTypeStatistics:		done = _updateStatistics(data::statistics_cast(record));
										break;

	case dataRecordTypeLocalMessage:	done = _updateLocalMessage(data::local_message_cast(record));
										break;

	case dataRecordTypeInstanceCache:	done = _updateInstanceCache(data::instance_cache_cast(record));
										break;

	default:							OS_ASSERTFALSE();
										break;
	}
	*/

	if(done)
	{
		// Richiama l'evento di inserimento dell'oggetto
		onUpdateRecord(record);
	}

	return done;
}

bool IPortalDatabase::removeRecord(shared_ptr<DataIRecord> record)
{
	// 0.12 - Spostata in testa, rinominata da onRemoveRecord a onRemovingRecord
	// Richiama l'evento di rimozione dell'oggetto
	onRemovingRecord(record);

	bool done = record->remove(m_connection);

	if(record->getRecordType() == dataRecordTypeObject)
	{
		shared_ptr<ObjectsIObject> object = objects_object_cast(record);
		return removeObject(object);
	}
	else
		return done;

	/*
	switch(record->getRecordType())
	{
	case dataRecordTypeAccount:			done = xxx_removeAccount(data::account_cast(record)->id);
										break;
	
	case dataRecordTypeEntry:			done = _removeEntry(dataentry_cast(record));
										break;

	case dataRecordTypeObject:			done = _removeObject(objects_object_cast(record));
										break;

	case dataRecordTypePeer:			done = _removePeer(data::peer_cast(record));
										break;

	case dataRecordTypeStatistics:		done = _removeStatistics(data::statistics_cast(record));
										break;

	case dataRecordTypeLocalMessage:	done = _removeLocalMessage(data::local_message_cast(record));
										break;

	case dataRecordTypeInstanceCache:	done = _removeInstanceCache(data::instance_cache_cast(record));
										break;

	default:							OS_ASSERTFALSE();
										break;
	}
	*/
}

bool IPortalDatabase::removeObject(shared_ptr<ObjectsIObject> object)
{
	shared_ptr<DataEntry> entry = getEntry(object->id);
	if(entry != null)
	{
		// Richiama la remove base per richiamare l'evento di cancellazione del record
		return removeRecord(entry);
	}
	else
		return false;
}

bool IPortalDatabase::validateRecord(shared_ptr<DataIRecord> record)
{
	bool valid = record->validate(get_this_ptr());
	/* TOCLEAN
	if(valid == true)
	{
		switch(record->getRecordType())
		{
		case dataRecordTypeAccount:		
		case dataRecordTypeEntry:
		case dataRecordTypePeer:
		case dataRecordTypeStatistics:
		case dataRecordTypeLocalMessage:
		case dataRecordTypeInstanceCache:
										// Nota: se servono delle validazioni specifiche in base alla tipologia di record si possono aggiungere qui
										valid = true;
										break;

		case dataRecordTypeObject:		valid = _validateObject(objects_object_cast(record));
										break;

		default:						OS_ASSERTFALSE();
										break;
		}
	}
	*/

	return valid;
}


bool IPortalDatabase::beginTransaction()
{
	m_connection->beginTransaction();
	m_transaction++;

	return true;
}

bool IPortalDatabase::commit()
{
	if(m_transaction == 0)
		return false;

	RealtimeStatsScopeTimer RSS(_S("Activity"), _S("Database commit"));

	m_connection->commit();
	m_transaction--;

	return true;
}

bool IPortalDatabase::rollback()
{
	if(m_transaction == 0)
		return false;

	m_connection->rollback();
	m_transaction--;

	return true;
}

bool IPortalDatabase::storeObject2(const shared_ptr<ObjectsIObject> &object)
{
	// Controlla se l'oggetto  gi stato salvato in precedenza
	if(object->getDescriptor()->objectExists(get_this_ptr(), object->id))
		// Aggiorna il record
		return updateRecord(object);

	// Inserisce il record
	return insertRecord(object);
}

uint32 IPortalDatabase::getMinEntryRank()
{
	// TODO, 'min'  specifica del database
	return m_connection->value_of(String::format(_S("select min(%S) from %S limit 1").c_str(), DBTABLES::ENTRIES::RANK.c_str(), DBTABLES::ENTRIES_TABLE.c_str()));
}

uint32 IPortalDatabase::getMaxEntryRank()
{
	// TODO, 'max'  specifica del database, il altri db  'greatest'
	return m_connection->value_of(String::format(_S("select max(%S) from %S limit 1").c_str(), DBTABLES::ENTRIES::RANK.c_str(), DBTABLES::ENTRIES_TABLE.c_str()));
}

uint32 IPortalDatabase::getNewEntryRank()
{
	// N.B.: il rank dei nuovi oggetti deve essere il valore massimo attuale, NON bisogna incrementarlo
	// di OS_ENTRYRANK_INCREASEFACTOR altrimenti si corre il rischio che in seguito ad un allineamento
	// con un nodo con molti oggetti il rank venga portato in alto (il continuo inserimento di oggetti porta il rank sempre più in alto)
	// con la conseguenza che verrebbero proposti solo gli oggetti ricevuti dall'ultimo allineamento.
	// E' solo in seguito alla richiesta di un oggetto che, per favorirne la diffusione, se ne alza il punteggio

	return utils::max<uint32>(getMaxEntryRank(), OS_ENTRYRANK_INCREASEFACTOR);
}
/*
uint32 IPortalDatabase::getMinPeerRank()
{
	sql::select_ptr select(OS_NEW sql::Select(DBTABLES::PEERS));
	select->fields.add(sql::Field(DBTABLES::PEERS::RANK)).setAttribute(sql::Field::faMin);
	select->limit.setCount(1);

	return value_of(select);
}

uint32 IPortalDatabase::getMaxPeerRank()
{
	sql::select_ptr select(OS_NEW sql::Select(DBTABLES::PEERS));
	select->fields.add(sql::Field(DBTABLES::PEERS::RANK)).setAttribute(sql::Field::faMax);
	select->limit.setCount(1);

	return value_of(select);
}

uint32 IPortalDatabase::getNewPeerRank()
{
	// N.B.: guardare le note di getMinEntryRank

	return getMinPeerRank();
}
*/

void IPortalDatabase::computeVotesStats(shared_ptr<ObjectsIRevisionable> object, DbValue<uint32>& outCount, DbValue<double>& outAverage)
{
	// TOCLEAN_SNAPSHOT_SCORE
	/*
	String sql = String::format(_S(" \
		select count(tv.score), avg(tv.score) \
		from \
		os_snapshot_users ts, \
		os_votes tv \
		where \
		ts.reference = tv.author and \
		ts.score %S and \
		tv.score is not null and \
		tv.reference = '%S'").c_str(),
		getPortal()->getSnapshotManager()->getReputationSqlRule(getPortal()->getOptionsShared()->getAuthorsReputationThreshold()).c_str(),
		object->getPrimary().toUTF16().c_str());
	*/
	String sql = String::format(_S(" \
		select count(tv.score), avg(tv.score) \
		from \
		os_votes tv, \
		os_entries te \
		where \
		tv.reference = '%S' and \
		te.id = tv.author and \
		te.rank >= 0").c_str(),
		object->getEntityID().toUTF16().c_str());

	DataTable table;
	execute(sql, table);
	OS_ASSERT(table.rows() == 1);

	outCount = *table[0][0];
	outAverage = *table[0][1];
}


void IPortalDatabase::computeLastObjectStats(shared_ptr<ObjectsIRevisionable> object, DbValue<EntityID>& outID, DbValue<DateTime>& outDate)
{
	String sql = String::format(_S(" \
		select entity, submit_date \
		from \
		os_snapshot_objects \
		where \
		parent = '%S' and \
		type = '%d' and \
		visible = 1 \
		order by submit_date desc limit 1").c_str(),
		object->getEntityID().toUTF16().c_str(),
		portalObjectTypePost);

	DataTable table;
	execute(sql, table);
	if(table.rows() > 0)
	{
		outID = *table[0][0];
		outDate = *table[0][1];
	}
	else
	{
		outID = EntityID::EMPTY;
		outDate = DateTime::EMPTY;
	}
}

bool IPortalDatabase::increaseEntryRank(const ObjectID &id)
{
	/*
	shared_ptr<DataEntry> ptr = getEntry(id);
	if(ptr == null)
		return false;

	ptr->rank = __increaseRank(ptr->rank, OS_ENTRYRANK_INCREASEFACTOR);
	return _updateEntry(ptr);*/

	String sql = String::format(_S("update os_entries set rank = rank + %d where id = '%S'").c_str(), OS_ENTRYRANK_INCREASEFACTOR, id.toUTF16().c_str());
	return execute(sql);
}

bool IPortalDatabase::decreaseEntryRank(const ObjectID &id)
{
	/*
	shared_ptr<DataEntry> ptr = getEntry(id);
	if(ptr == null)
		return false;

	if(ptr->rank == 0)
		return true;

	ptr->rank = __decreaseRank(ptr->rank, OS_ENTRYRANK_DECREASEFACTOR);
	return _updateEntry(ptr);
	*/

	// TODO, 'max'  specifica del database, il altri db  'greatest'
	String sql = String::format(_S("update os_entries set rank = max(rank - %d , 0) where id = '%S'").c_str(), OS_ENTRYRANK_DECREASEFACTOR, id.toUTF16().c_str());
	return execute(sql);
}

LanguageResult IPortalDatabase::setUserAvatar(shared_ptr<ObjectsUser> user, const Buffer &privateKey, const String &fileName, const String &fileType, const Buffer &fileData)
{
	DateTime now = IsisSystem::instance()->getInternetDateTime();
	if(now.isNull())
		return LanguageResult("no_clock_sync");
		
	bool exists = true;
	shared_ptr<ObjectsAvatar> avatar = getAvatar(user->getAvatarID());
	if(avatar == null)
	{
		exists = false;

		avatar.reset(OS_NEW ObjectsAvatar());

		avatar->id = user->getAvatarID();
		avatar->author = user->id;
	}

	// Salva la data di creazione/aggiornamento
	avatar->submit_date = now;

	avatar->file_name = fileName;
	avatar->file_data = fileData;
	avatar->file_type = fileType;
	avatar->file_size = fileData.getSize();

	LanguageResult result;

	if(exists)
		result = _signAndUpdate(avatar, privateKey, false);	// La data  gi allineata in precedenza
	else
		result = _signAndInsert(avatar, privateKey);

	// 16/07/2012 : Ignoro perchè aggiornare l'utente se viene inserito un avatar. Commentato.
	/*
	if(result.empty() == false)
		return result;	
	
	// Aggiorna l'avatar
	user->submit_date = now;

	return _signAndUpdate(user, privateKey, false);			// La data  gi allineata in precedenza
	*/
	return result;
}

/*
bool IPortalDatabase::setReputationValue(shared_ptr<ObjectsReputation> reputation, double score, const String &description, bool follow, const Buffer &private_key)
{
	reputation->score = score;
	reputation->description = description;
	reputation->follow = follow;

	return _signAndUpdate(reputation, private_key, true);
}
*/

shared_ptr<ObjectsVote> IPortalDatabase::getObjectVote(shared_ptr<ObjectsUser> user, shared_ptr<ObjectsIRevisionable> object)
{
	return objects_vote_cast(loadObject(getUserVoteID(user->id, object->getEntityID())));
}

shared_ptr<ObjectsPollVote> IPortalDatabase::getPollVote(shared_ptr<ObjectsUser> user, shared_ptr<ObjectsPoll> poll)
{
	return objects_poll_vote_cast(loadObject(getUserPollVoteID(user->id, poll->getEntityID())));
}

LanguageResult IPortalDatabase::voteObject(shared_ptr<ObjectsUser> user, const Buffer &private_key, const EntityID &reference, uint32 score)
{
	return __voteObject(user, private_key, reference, false, score);
}

LanguageResult IPortalDatabase::unvoteObject(shared_ptr<ObjectsUser> user, const Buffer &private_key, const EntityID &reference)
{
	return __voteObject(user, private_key, reference, true, 0);
}

LanguageResult IPortalDatabase::votePoll(shared_ptr<ObjectsUser> user, const Buffer &private_key, shared_ptr<ObjectsPoll> poll, EntityID poll_option)
{
	return __votePoll(user, private_key, poll, poll_option, false, OS_PORTAL_OBJECT_VOTE_DEFAULT);
}

LanguageResult IPortalDatabase::unvotePoll(shared_ptr<ObjectsUser> user, const Buffer &private_key, shared_ptr<ObjectsPoll> poll)
{
	return __votePoll(user, private_key, poll, EntityID::EMPTY, true, 0);
}

bool IPortalDatabase::removeProfileStatistics(const String &table, ObjectOrEntityID reference)
{
	String sql = String::format(_S("delete from %S").c_str(), table.c_str());
	if(reference != String::EMPTY)
		sql.append(String::format(_S(" where reference='%S'").c_str(), reference.c_str()));

	return execute(sql);
}

shared_ptr<DataAccount> IPortalDatabase::xxx_loadAccount(const String &id) const
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::ACCOUNTS_TABLE));
	select->where.add(DBTABLES::ACCOUNTS::ID, Convert::toSQL(id));
	select->limit.setCount(1);

	return _recordFromSQL<DataAccount>(select);
}

shared_ptr<DataEntry> IPortalDatabase::loadEntry(const String &id) const
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::ENTRIES_TABLE));
	select->where.add(DBTABLES::ENTRIES::ID, Convert::toSQL(id));
	select->limit.setCount(1);

	return _recordFromSQL<DataEntry>(select);
}

shared_ptr<DataPeer> IPortalDatabase::loadPeer(const String &ip) const
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::PEERS_TABLE));
	select->where.add(DBTABLES::PEERS::IP, Convert::toSQL(ip));
	select->limit.setCount(1);

	return _recordFromSQL<DataPeer>(select);
}

shared_ptr<DataPollStats> IPortalDatabase::loadPollStats(const String &reference) const
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::POLLS_STATS_TABLE));
	select->where.add(DBTABLES::REFERENCE, Convert::toSQL(reference));
	select->limit.setCount(1);

	return _recordFromSQL<DataPollStats>(select);
}

shared_ptr<DataPollOptionStats> IPortalDatabase::loadPollOptionStats(const String &reference) const
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::POLLS_OPTIONS_STATS_TABLE));
	select->where.add(DBTABLES::REFERENCE, Convert::toSQL(reference));
	select->limit.setCount(1);

	return _recordFromSQL<DataPollOptionStats>(select);
}

shared_ptr<DataDiscussionStats> IPortalDatabase::loadDiscussionStats(const String &reference) const
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::DISCUSSIONS_STATS_TABLE));
	select->where.add(DBTABLES::REFERENCE, Convert::toSQL(reference));
	select->limit.setCount(1);

	return _recordFromSQL<DataDiscussionStats>(select);
}

shared_ptr<DataUserStats> IPortalDatabase::loadUserStats(const String &reference) const
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(DBTABLES::USERS_STATS_TABLE));
	select->where.add(DBTABLES::REFERENCE, Convert::toSQL(reference));
	select->limit.setCount(1);

	return _recordFromSQL<DataUserStats>(select);
}

LanguageResult IPortalDatabase::_signAndInsert(shared_ptr<ObjectsIObject> object, const Buffer &private_key)
{
	/*
	OS_EXCEPT_IF(getPortal() == null, "Invalid database");		// L'id del database deve essere valido

	// Firma l'oggetto con la chiave privata specificata e lo associa all'id del database
	if(object->sign(private_key, getPortal()->getPortalID(), getPortal()->getUserID()) == false)
		return false;
	OS_ASSERT(object->signature->empty() == false);
	// Inserisce il record
	return insertRecord(object);
	*/
	// CLODOURGENT
	LanguageResult result = object->create(get_this_ptr(), private_key);
	return result;

}

LanguageResult IPortalDatabase::_signAndUpdate(shared_ptr<ObjectsIObject> object, const Buffer &private_key, bool updateSubmitDate)
{
	OS_EXCEPT_IF(getPortal() == null, "Invalid database");		// L'id del database deve essere valido

	// Controlla se aggiornare la data dell'oggetto
	if(updateSubmitDate)
	{
		DateTime now = IsisSystem::instance()->getInternetDateTime();
		if(now.isNull())
			return LanguageResult("no_clock_sync");

		object->submit_date = now;
	}

	/*
	// Firma l'oggetto con la chiave privata specificata e lo associa all'id del database
	if(object->sign(private_key, getPortal()->getPortalID(), getPortal()->getUserID()) == false)
		return false;
	OS_ASSERT(object->signature->empty() == false);
	// Aggiorna il record
	return updateRecord(object);
	*/
	LanguageResult result = object->create(get_this_ptr(), private_key);
	return result;
}
/*
ObjectID IPortalDatabase::generateID(PortalObjectType type, bool primary, const std::string &hash)
{
	return ObjectID::generate(type, hash, primary);
}

ObjectID IPortalDatabase::generateRandomID(PortalObjectType type, bool primary)
{
	return generateID(type, primary, CryptManager::instance()->randomSHA().toHex());
}
*/

ObjectID IPortalDatabase::getUserRelatedObjectID(const ObjectID &user_id, const std::string &relation, const std::string &reference, PortalObjectType type)
{
	// Nota: le associazioni di oggetti per gli utenti sono (attualmente) sempre delle primarie


#ifdef OS_NOOBJECTID
	String data = user_id.getString() + relation + reference;	// NON modificare mai l'ordine di questa somma
	return CryptManager::instance()->SHA(data.buffer(), data.buffer_size()).toHex();
#else
	String data = user_id.getHash() + relation + reference;	// NON modificare mai l'ordine di questa somma
	return generateID(type, true, CryptManager::instance()->SHA(data.buffer(), data.buffer_size()).toHex());
#endif
}

ObjectID IPortalDatabase::getUserReputationID(const ObjectID &user_id, const ObjectID &reference)
{
#ifdef OS_NOOBJECTID
	return getUserRelatedObjectID(user_id, OS_IDPARAM_USER_REPUTATION, reference.getString(), portalObjectTypeReputation);
#else
	return getUserRelatedObjectID(user_id, OS_IDPARAM_USER_REPUTATION, reference.getHash(), portalObjectTypeReputation);
#endif
}

ObjectID IPortalDatabase::getUserAvatarID(const ObjectID &user_id)
{
	return getUserRelatedObjectID(user_id, OS_IDPARAM_USER_AVATAR, "", portalObjectTypeAvatar);
}

ObjectID IPortalDatabase::getUserPollVoteID(const ObjectID &user_id, const EntityID &object_id)
{
	return getUserRelatedObjectID(user_id, OS_IDPARAM_USER_POLL_VOTE, object_id.getString(), portalObjectTypePollVote);
}

ObjectID IPortalDatabase::getUserVoteID(const ObjectID &user_id, const EntityID &object_id)
{
	return getUserRelatedObjectID(user_id, OS_IDPARAM_USER_VOTE, object_id.getString(), portalObjectTypeVote);
}

bool IPortalDatabase::isUpdatable(PortalObjectType type)
{
	switch(type)
	{
	case portalObjectTypeUser:
	case portalObjectTypeReputation:
	case portalObjectTypeAvatar:
	case portalObjectTypeAttribute:
	case portalObjectTypeVote:
	case portalObjectTypePollVote:
										return true;

	case portalObjectTypeInstance:
	case portalObjectTypePost:
	case portalObjectTypeText:
	case portalObjectTypeFile:
	case portalObjectTypeTag:
	case portalObjectTypeSection:
	case portalObjectTypePrivateMessage:
	case portalObjectTypePoll:
	case portalObjectTypePollOption:
	case portalObjectTypeModel:
	case portalObjectTypeCalendarEvent:
	case portalObjectTypeAttachment:
										return false;

	default:						OS_ASSERTFALSE();
									break;
	}

	return false;
}

bool IPortalDatabase::isRevisionable(PortalObjectType type)
{
	switch(type)
	{
	case portalObjectTypeInstance:
	case portalObjectTypePost:
	case portalObjectTypeText:
	case portalObjectTypeFile:
	case portalObjectTypeSection:
	case portalObjectTypeTag:
	case portalObjectTypePoll:
	case portalObjectTypePollOption:
	case portalObjectTypeModel:
	case portalObjectTypeCalendarEvent:
	case portalObjectTypeAttachment:
									return true;

	case portalObjectTypeUser:
	case portalObjectTypeReputation:
	case portalObjectTypeAvatar:
	case portalObjectTypeAttribute:
	case portalObjectTypePrivateMessage:
	case portalObjectTypeVote:
	case portalObjectTypePollVote:
									return false;

	default:						OS_ASSERTFALSE();
									break;
	}

	return false;
}

bool IPortalDatabase::isVotable(PortalObjectType type)
{
	switch(type)
	{
	case portalObjectTypeCalendarEvent:
	case portalObjectTypeFile:
	case portalObjectTypeModel:
	case portalObjectTypeText:
	case portalObjectTypePoll:
									return true;
	case portalObjectTypeInstance:
	case portalObjectTypePost:
	case portalObjectTypeSection:
	case portalObjectTypeTag:
	case portalObjectTypeUser:
	case portalObjectTypeReputation:
	case portalObjectTypeAvatar:
	case portalObjectTypeAttribute:
	case portalObjectTypePrivateMessage:
	case portalObjectTypeVote:
	case portalObjectTypePollOption:
	case portalObjectTypePollVote:
	case portalObjectTypeAttachment:
									return false;

	default:						OS_ASSERTFALSE();
									break;
	}

	return false;
}

void IPortalDatabase::logError(const String &sql, std::exception &e) const
{
	m_connection->logError(sql, e);	
}

void IPortalDatabase::_query() const
{
	m_query++;
}

bool IPortalDatabase::_createEntry(shared_ptr<ObjectsIObject> object, const uint32 &rank)
{
	shared_ptr<DataEntry> entry(OS_NEW DataEntry());
	entry->id = object->id;
	entry->author = object->getAuthor();
	entry->submit_date = object->submit_date;
	entry->insert_date = DateTime::now();
	entry->type = Convert::toUint32(object->getObjectType());
	entry->rank = rank;

	// N.B.: il campo revision di un'entry segue questa logica:
	// - null se non  un oggetto revisionabile
	// - "" se  una primaria
	// - id se  una revisione
	// la stessa logica viene seguita per il parent

#ifdef OS_TODOCIP
	shared_ptr<ObjectsIRevisionable> revisionable = objects_revisionable_cast(object);
	if(revisionable != null)
	{
		// IMPORTANTE: i campi parent e revision di un oggetto revisionabile non devono essere mai lasciati a null,
		// se dovesse succedere le note descritte in precedenza non sarebbero rispettate quando si copia il valore nell'entry (potrebbe finirci null)

		OS_ASSERT(revisionable->parent.isNull() == false);
		OS_ASSERT(revisionable->entity.isNull() == false);
		OS_ASSERT(revisionable->entity_author.isNull() == false);
		OS_ASSERT(revisionable->entity_date.isNull() == false);
		OS_ASSERT(revisionable->entity_parent.isNull() == false);

		entry->parent = revisionable->parent;

		entry->entity = revisionable->entity;
		entry->entity_author = revisionable->entity_author;
		entry->entity_date = revisionable->entity_date;
		//entry->entity_parent = revisionable->entity_parent;
	}
	else
	{
		entry->parent.setNull();
		entry->entity.setNull();
		entry->entity_author.setNull();
		entry->entity_date.setNull();
		//entry->entity_parent.setNull();

		// TOOPTIMIZE: Volendo, entity e entity_date forse potrei evitarli risparmiando spazio.
		// Non so se sono usate. entity_author si, è usata nelle stats utente.
		// Forse non serve.
		/*
		entry->entity.setNull(); 
		entry->entity_author = entry->author;
		entry->entity_date = entry->submit_date;
		*/
	}
#else

	shared_ptr<ObjectsIRevisionable> revisionable = objects_revisionable_cast(object);
	if(revisionable != null)
	{
		// IMPORTANTE: i campi parent e revision di un oggetto revisionabile non devono essere mai lasciati a null,
		// se dovesse succedere le note descritte in precedenza non sarebbero rispettate quando si copia il valore nell'entry (potrebbe finirci null)
		
		OS_ASSERT(revisionable->parent.isNull() == false);
		OS_ASSERT(revisionable->revision.isNull() == false);

		entry->parent = revisionable->parent;
		entry->revision = revisionable->revision;
	}
	else
	{
		entry->parent.setNull();
		entry->revision.setNull();
	}
#endif

	return insertRecord(entry);
}

bool IPortalDatabase::_validateObject(const shared_ptr<ObjectsIObject> &object) // TOCLEAN
{
	return true;
	/*
	// Controlla se l'oggetto rappresenta un utente
	if(object->isUser())
	{
		// Valida l'utente con la sua chiave pubblica
		return objects_user_cast(object)->verify(getPortal()->getPortalID().getString());
	}

	shared_ptr<ObjectsUser> author = getUser(object->getAuthor()); // CLODOURGENT, qui è un giro che fà quando salvo il record, mentre dovrebbe farlo prima insieme all'acceptable
	// Carica l'autore dell'oggetto
	if(author == null)
		return false;

	// Verifica la firma digitale dell'oggetto sulla base del proprio autore
	return object->verify(author->public_key, getPortal()->getPortalID().getString());
	*/
}

/*
bool IPortalDatabase::_insertRecord(const String &table, const shared_ptr<DataIRecord> &record)
{
	DbSqlValues values;
	record->write(values);
	return executeStatement(m_connection->prepare_insert(table, values), values);
}
*/

/*
bool IPortalDatabase::_insertObject(const shared_ptr<ObjectsIObject> &object)
{
	if(object->insert(m_connection, object->getDescriptor()->getTableName()) == false)
		return false;

	if(_createEntry(object, getNewEntryRank()) == false)
	{
		_removeObject(object);		// Chiama l'implementazione in modo da non lanciare l'evento
		return false;
	}

	ObjectsSystem::instance()->fireNewObject(object);

	return true;
}
*/

/*
bool IPortalDatabase::_insertStatistics(const shared_ptr<DataIStatistics> &statistics)
{
	return statistics->insert(m_connection, statistics->getTableName());
}

bool IPortalDatabase::_insertLocalMessage(const data::local_message_ptr &message)
{
	return message->insert(m_connection, DBTABLES::LOCAL_MESSAGES_TABLE);
}

bool IPortalDatabase::_insertInstanceCache(const data::instance_cache_ptr &instance_cache)
{
	return instance_cache->insert(m_connection, DBTABLES::INSTANCES_CACHE_TABLE);
}
*/
/*
bool IPortalDatabase::_updateObject(const shared_ptr<ObjectsIObject> &object)
{
	if(_updateObject(object->getDescriptor()->getTableName(), object) == false)
		return false;

	shared_ptr<DataEntry> entry = getEntry(object->id);
	// Carica l'entry associata all'oggetto
	if(entry != null)
	{
		entry->type = Convert::toUint32(object->getObjectType());		
		entry->submit_date = object->submit_date;
		entry->insert_date = DateTime::now();
		entry->rank = getNewEntryRank();

		// Effettua l'aggiornamento dell'entry
		return _updateEntry(entry);
	}

	// Genera un nuovo entry associato all'oggetto
	return _createEntry(object, getNewEntryRank());
}
*/

/*
bool IPortalDatabase::xxx__updateAccount(const shared_ptr<DataAccount> &account)
{
	DbSqlValues values;
	account->write(values);
	return executeStatement(m_connection->prepare_update(DBTABLES::ACCOUNTS_TABLE, values, DBTABLES::ACCOUNTS::ID, Convert::toSQL(account->id)), values);
}
*/

/*
bool IPortalDatabase::_updateEntry(const shared_ptr<DataEntry> &entry)
{
	DbSqlValues values;
	entry->write(values);
	return executeStatement(m_connection->prepare_update(DBTABLES::ENTRIES_TABLE, values, DBTABLES::ENTRIES::ID, Convert::toSQL(entry->id)), values);
}
*/

/*
bool IPortalDatabase::_updateObject(const String &table, const shared_ptr<ObjectsIObject> &object)
{
	DbSqlValues values;
	object->write(values);
	return executeStatement(m_connection->prepare_update(table, values, DBTABLES::ID, Convert::toSQL(object->id)), values);
}
*/

/*
bool IPortalDatabase::_updatePeer(const data::peer_ptr &peer)
{
	DbSqlValues values;
	peer->write(values);
	return executeStatement(m_connection->prepare_update(DBTABLES::PEERS_TABLE, values, DBTABLES::PEERS::IP, Convert::toSQL(peer->ip)), values);
}

bool IPortalDatabase::_updateStatistics(const shared_ptr<DataIStatistics> &statistics)
{
	return _updateStatistics(statistics->getTableName(), statistics);
}

bool IPortalDatabase::_updateStatistics(const String &table, const shared_ptr<DataIStatistics> &statistics)
{
	DbSqlValues values;
	statistics->write(values);
	return executeStatement(m_connection->prepare_update(table, values, DBTABLES::REFERENCE, Convert::toSQL(statistics->reference)), values);
}
*/

/*
bool IPortalDatabase::_updateLocalMessage(const data::local_message_ptr &message)
{
	DbSqlValues values;
	message->write(values);
	return executeStatement(m_connection->prepare_update(DBTABLES::LOCAL_MESSAGES_TABLE, values, DBTABLES::LOCAL_MESSAGES::PROFILE, Convert::toSQL(message->profile), DBTABLES::LOCAL_MESSAGES::ID, Convert::toSQL(message->id)), values);
}

bool IPortalDatabase::_updateInstanceCache(const data::instance_cache_ptr &instance_cache)
{
	DbSqlValues values;
	instance_cache->write(values);
	return executeStatement(m_connection->prepare_update(DBTABLES::INSTANCES_CACHE_TABLE, values, DBTABLES::INSTANCES_CACHE::REFERENCE, Convert::toSQL(instance_cache->reference)), values);
}
*/

/*
bool IPortalDatabase::_removeEntry(const shared_ptr<DataEntry> &entry)
{
	return execute(m_connection->sql_remove(DBTABLES::ENTRIES_TABLE, DBTABLES::ENTRIES::ID, entry->id.get().toUTF16()));
}
*/

/*
bool IPortalDatabase::_removeObject(const shared_ptr<ObjectsIObject> &object)
{
	bool done = false;
	switch(object->getObjectType())
	{
	case portalObjectTypeInstance:		done = _removeObject(DBTABLES::INSTANCES_TABLE, object);
									break;

	case portalObjectTypePost:			done = _removeObject(DBTABLES::POSTS_TABLE, object);
									break;

	case portalObjectTypeText:			done = _removeObject(DBTABLES::TEXTS_TABLE, object);
									break;

	case portalObjectTypeUser:			done = _removeObject(DBTABLES::USERS_TABLE, object);
									break;

	case portalObjectTypeReputation:		done = _removeObject(DBTABLES::REPUTATIONS_TABLE, object);
									break;

	case portalObjectTypeAvatar:			done = _removeObject(DBTABLES::AVATARS_TABLE, object);
									break;

	case portalObjectTypeFile:			done = _removeObject(DBTABLES::FILES_TABLE, object);
									break;

	case portalObjectTypeTag:			done = _removeObject(DBTABLES::TAGS_TABLE, object);
									break;

	case portalObjectTypeSection:		done = _removeObject(DBTABLES::SECTIONS_TABLE, object);
									break;

	case portalObjectTypeAttribute:		done = _removeObject(DBTABLES::ATTRIBUTES_TABLE, object);
									break;

	case portalObjectTypePrivateMessage:	done = _removeObject(DBTABLES::PRIVATE_MESSAGES_TABLE, object);
									break;

	case portalObjectTypePoll:			done = _removeObject(DBTABLES::POLLS_TABLE, object);
									break;

	case portalObjectTypePollOption:		done = _removeObject(DBTABLES::POLLS_OPTIONS_TABLE, object);
									break;

	case portalObjectTypeVote:			done = _removeObject(DBTABLES::VOTES_TABLE, object);
									break;

	case portalObjectTypeModel:			done = _removeObject(DBTABLES::MODELS_TABLE, object);
									break;

	case portalObjectTypeCalendarEvent:	done = _removeObject(DBTABLES::CALENDAR_EVENTS_TABLE, object);
									break;

	case portalObjectTypePollVote:		done = _removeObject(DBTABLES::POLLS_VOTES_TABLE, object);
									break;

	case portalObjectTypeAttachment:		done = _removeObject(DBTABLES::ATTACHMENTS_TABLE, object);
									break;

	default:						OS_ASSERTFALSE();
									break;
	}

	// Verifica che la cancellazione dell'oggetto sia andata a buon fine
	if(done)
	{
		shared_ptr<DataEntry> entry = getEntry(object->id);
		if(entry != null)
		{
			// Richiama la remove base per richiamare l'evento di cancellazione del record
			done = removeRecord(entry);
		}
		else
		{
			done = false;
		}
	}

	return done;
}
*/

/*
bool IPortalDatabase::_removeObject(const String &table, const shared_ptr<ObjectsIObject> &object)
{
	return execute(m_connection->sql_remove(table, DBTABLES::ID, object->id->toUTF16()));
}

bool IPortalDatabase::_removePeer(const data::peer_ptr &peer)
{
	return execute(m_connection->sql_remove(DBTABLES::PEERS_TABLE, DBTABLES::PEERS::IP, peer->ip));
}

bool IPortalDatabase::_removeStatistics(const shared_ptr<DataIStatistics> &statistics)
{
	// TODO: fixme

	OS_ASSERTFALSE();
	return false;
}

bool IPortalDatabase::_removeStatistics(const String &table, const data::IStatistics &statistics)
{
	// TODO: fixme

	OS_ASSERTFALSE();
	return false;
}

bool IPortalDatabase::_removeLocalMessage(const data::local_message_ptr &message)
{
	return execute(m_connection->sql_remove(DBTABLES::LOCAL_MESSAGES_TABLE, DBTABLES::LOCAL_MESSAGES::PROFILE, message->profile, DBTABLES::LOCAL_MESSAGES::ID, message->id->toUTF16()));
}

bool IPortalDatabase::_removeInstanceCache(const data::instance_cache_ptr &instance_cache)
{
	return execute(m_connection->sql_remove(DBTABLES::INSTANCES_CACHE_TABLE, DBTABLES::INSTANCES_CACHE::REFERENCE, instance_cache->reference->toUTF16()));
}
*/



uint32 IPortalDatabase::_getRecords(const String &table) const
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(table));
	select->count = true;

	return m_connection->value_of(select);
}

/*
bool IPortalDatabase::__createObject(shared_ptr<ObjectsIObject> object, PortalObjectType type, ObjectID &id, DateTime submit_date, const uint32 &version)
{
	// VERYURGENT: Ste cose non le fà IObject::onCreate()? 

	// OS_TODOCIP: Suppongo che isPrimary è TRUE, perchè chiamata sempre con TRUE questa, gli oggetti revisionabili seguono un'altro giro e non usano questa helper.
	bool isPrimary = true;

	OS_ASSERT(object != null);
	if(object == null)
		return false;

	// Controlla se non  stato specificato un id valido
	if(id.empty())
	{
		// Genera un nuovo id per l'oggetto
		id = generateRandomID(type, isPrimary);
	}
	else
	{
		// Se l'ID  valorizzato dall'esterno devono coincidere il tipo e i flags

		if(id.getObjectType() != type)
			return false;

		//if(id.isPrimary() != isPrimary)
		//	return false;		
	}

	// Controlla se non  stata specificata una data valida
	if(submit_date.isNull())
	{
		// Inizializza la data di creazione dell'oggetto
		if(Engine::instance()->getSynchronizedDateTime(submit_date) == false)
			return false;
	}

	// Salva l'id dell'oggetto
	object->id = id;
	// Salva la data dell'oggetto
	object->submit_date = submit_date;
	// Salva la versione dell'oggetto
	object->version = version;
	// Inizializza la firma (viene valorizzato in una fase successiva)
	object->signature.reset();

	return true;
}

bool IPortalDatabase::__createEntry(objects::entry_ptr entry, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const ObjectID &author, const Buffer &private_key)
{
	if(author.empty())
		return false;

	// Carica l'utente di riferimento
	shared_ptr<ObjectsUser> user = getUser(author);
	if(user == null)
		return false;

	// Assicura che l'utente sia valido
	if(validateRecord(user) == false)
		return false;

	// Verifica che la chiave privata corrisponda a quella dell'utente
	if(CryptManager::instance()->rsaCheckKeys(private_key, user->public_key) == false)
		return false;

	OS_ASSERT(entry != null);
	if(__createObject(entry, type, id, submit_date, version) == false)
		return false;

	entry->author = author;

	return true;
}
*/

/*
bool IPortalDatabase::__createRevisionable(shared_ptr<ObjectsIRevisionable> revisionable, PortalObjectType type, bool isPrimary, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const ObjectID &parent, shared_ptr<ObjectsIRevisionable> primary, bool visible, double position, const Buffer &private_key)
{
	OS_ASSERT(revisionable != null);

	// Controlla se l'oggetto  una revisione
	if(primary != null)
	{
		// Controlla se la primaria specificata in realt corrisponde ad una revisione
		if(primary->isRevision())
			return false;

		// Assicura che la primaria sia valida
		if(validateRecord(primary) == false)
			return false;

		// Verifica che la tipologia di oggetti corrisponda
		if(primary->getObjectType() != revisionable->getObjectType())
			return false;

		// Se  stata specificata una data valida la nuova revisione non pu essere antecedente alla base
		if(submit_date.isValid() && (submit_date < primary->submit_date))
			return false;
	}

	if(parent.empty() == false)
	{
		// Carica il padre corrente
		shared_ptr<ObjectsIObject> parent_object = getObject(parent);
		if(parent_object == null)
			return false;

		// Assicura che il padre sia un oggetto valido
		if(validateRecord(parent_object) == false)
			return false;

		// Verifica che il padre specificato sia ammesso per l'oggetto
		if(revisionable->allowParent(parent_object->getObjectType()) == false)
			return false;
	}

	if(__createEntry(revisionable, type, isPrimary, id, submit_date, version, author.to_ascii(), private_key) == false)
		return false;

	revisionable->parent = parent;
	if(primary != null)
        revisionable->revision = primary->id;
    else
        revisionable->revision = ObjectID::EMPTY;
	revisionable->visible = visible;

	if(position == OS_PORTAL_OBJECT_POSITION_NULL)
        revisionable->position = static_cast<double>(revisionable->submit_date->toTime_t());
	else
		revisionable->position = position;

	return true;
}
*/

/*
bool IPortalDatabase::__createPrimary(shared_ptr<ObjectsIRevisionable> revisionable, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const String &parent, const Buffer &private_key)
{
	return __createRevisionable(revisionable, type, true, id, submit_date, version, author, parent.to_ascii(), null, true, 0, private_key);
}
*/

/*
bool IPortalDatabase::__createRevision(shared_ptr<ObjectsIRevisionable> revisionable, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const String &parent, const String &revision, bool visible, double position, const Buffer &private_key)
{
	// Carica la base della revisione
	shared_ptr<ObjectsIRevisionable> primary = getRevisionable(revision.to_ascii());
	if(primary == null)
		return false;

	return __createRevisionable(revisionable, type, false, id, submit_date, version, author, parent.to_ascii(), primary, visible, position, private_key);
}
*/

/*
bool IPortalDatabase::__createObjectImpl(shared_ptr<ObjectsIObject> object, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const Buffer &private_key)
{
	if(__createObject(object, type, id, submit_date, version) == false)
		return false;

	return __insertObject(object, private_key);
}

bool IPortalDatabase::__createEntryImpl(objects::entry_ptr entry, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const Buffer &private_key)
{
	if(__createEntry(entry, type, id, submit_date, version, author.to_ascii(), private_key) == false)
		return false;

	return __insertObject(entry, private_key);
}
*/

/*
bool IPortalDatabase::__createPrimaryImpl(shared_ptr<ObjectsIRevisionable> revisionable, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const String &parent, const Buffer &private_key)
{
	if(__createPrimary(revisionable, type, id, submit_date, version, author, parent, private_key) == false)
		return false;

	return __insertObject(revisionable, private_key);
}
*/

/*
bool IPortalDatabase::__createRevisionImpl(shared_ptr<ObjectsIRevisionable> revisionable, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const String &parent, const String &revision, bool visible, double position, const Buffer &private_key)
{
	if(__createRevision(revisionable, type, id, submit_date, version, author, parent, revision, visible, position, private_key) == false)
		return false;

	return __insertObject(revisionable, private_key);
}
*/

/*
bool IPortalDatabase::__insertObject(shared_ptr<ObjectsIObject> object, const Buffer &private_key)
{
	OS_ASSERT(getPortal());		// L'id del database deve essere valido

	// URGENT: la firma dell'oggetto non va qui, bisogna spostarla nel metodo IObject::create dopo
	// aver chiamato la onCreate, vanno per prima sistemati tutti i giri di creazione degli oggetti

	// Firma l'oggetto con la chiave privata specificata e lo associa all'id primario
	if(object->sign(private_key, getPortal()->getPortalID(), getPortal()->getUserID()) == false)
		return false;
	OS_ASSERT(object->isSigned());

	// Inserisce il record
	return insertRecord(object);
}
*/

/*
bool IPortalDatabase::__createRevisionDelete(shared_ptr<ObjectsIRevisionable> revisionable, ObjectID &id, const String &author, const Buffer &private_key)
{
	if(revisionable == null)
		return false;

	shared_ptr<ObjectsIRevisionable> delete_revision = objects_revisionable_cast(revisionable->clone());
	if(delete_revision == null)
		return false;

	return __createRevisionImpl(delete_revision, delete_revision->getObjectType(), id, DateTime::EMPTY, delete_revision->version, author, revisionable->getParent().toUTF16(), revisionable->getPrimary().toUTF16(), false, 0, private_key);
}

bool IPortalDatabase::__createRevisionMove(shared_ptr<ObjectsIRevisionable> revisionable, const String &newParent, ObjectID &id, const String &author, const Buffer &private_key)
{
	if(revisionable == null)
		return false;

	shared_ptr<ObjectsIRevisionable> move_revision = objects_revisionable_cast(revisionable->clone());
	if(move_revision == null)
		return false;

	return __createRevisionImpl(move_revision, move_revision->getObjectType(), id, DateTime::EMPTY, move_revision->version, author, newParent, revisionable->getPrimary().toUTF16(), revisionable->visible, 0, private_key);
}

bool IPortalDatabase::__createRevisionRestore(shared_ptr<ObjectsIRevisionable> revision, ObjectID &id, const String &author, const Buffer &private_key)
{
	if(revision == null)
		return false;

	shared_ptr<ObjectsIRevisionable> new_revision = objects_revisionable_cast(revision->clone());
	if(new_revision == null)
		return false;

	return __createRevisionImpl(new_revision, new_revision->getObjectType(), id, DateTime::EMPTY, new_revision->version, author, new_revision->getParent().toUTF16(), new_revision->getPrimary().toUTF16(), new_revision->visible, new_revision->position, private_key);
}
*/

/*
bool IPortalDatabase::__createReputation(const ObjectID &reference, double score, const String &description, bool follow, const String &author, const Buffer &private_key, ObjectID &id)
{
	shared_ptr<ObjectsReputation> reputation(OS_NEW ObjectsReputation());
	reputation->reference = reference;
	reputation->score = utils::limit <double>(score, OS_REPUTATION_MIN, OS_REPUTATION_MAX);
	reputation->description = description;
	reputation->follow = follow;

	// Forza l'id della reputazione
	id = getUserReputationID(author.to_ascii(), reference);

	// Nota: gli oggetti updatabili come le reputazioni sono sempre considerati delle primarie
	return __createEntryImpl(reputation, portalObjectTypeReputation, id, DateTime::EMPTY, objects::Reputation::VERSION, author, private_key);
}
*/

/*
bool IPortalDatabase::__createUser(const String &name, const Buffer &public_key, const Buffer &private_key, ObjectID &id)
{
	DateTime now;
	if(Engine::instance()->getSynchronizedDateTime(now) == false)
		return false;

	shared_ptr<ObjectsUser> user(OS_NEW ObjectsUser());
	user->public_key = public_key;
	user->name = name;
	user->description = String::EMPTY;
	//user->authors_threshold = objects::rtNotNegative;
	//user->editors_threshold = objects::rtNotNegative;
	user->join_date = now;
	user->birth_date = DateTime::EMPTY;
	user->misc = String::EMPTY;
	user->mark = String::EMPTY;
	user->show_mark = true;
	user->show_avatar = true;

	return __createObjectImpl(user, portalObjectTypeUser, id, now, objects::User::VERSION, private_key);
}
*/

/*
bool IPortalDatabase::__createPrivateMessage(const Buffer &session, const Buffer &content, const shared_ptr<ObjectsUser> &adressee, const String &author, const Buffer &private_key, ObjectID &id, const DateTime &submitDate)
{
	shared_ptr<ObjectsPrivateMessage> private_message(OS_NEW ObjectsPrivateMessage());
	private_message->adressee = adressee->id;
	private_message->session = session;
	private_message->content = content;

	return __createEntryImpl(private_message, portalObjectTypePrivateMessage, id, submitDate, objects::PrivateMessage::VERSION, author, private_key);
}
*/

LanguageResult IPortalDatabase::__voteObject(shared_ptr<ObjectsUser> user, const Buffer &private_key, const EntityID &reference, bool isNull, uint32 score)
{
	// TOFIX_VALIDATING: 
	/*
	if(isVotable(reference.getObjectType()) == false)
		return false;
	*/

	DateTime now = IsisSystem::instance()->getInternetDateTime();
	if(now.isNull())
		return LanguageResult("no_clock_sync");

	ObjectID vote_id = getUserVoteID(user->id, reference);

	bool exists = true;

	shared_ptr<ObjectsVote> vote = getVote(vote_id);
	if(vote == null)
	{
		exists = false;

		vote.reset(OS_NEW ObjectsVote());

		vote->id = vote_id;
		vote->author = user->id;
	}

	// Salva la data di creazione/aggiornamento
	vote->submit_date = now;

	vote->reference = reference;
	if(isNull)
		vote->score.setNull();
	else
        vote->score = score;
		
	if(exists)
		return _signAndUpdate(vote, private_key, false);
	else
		return _signAndInsert(vote, private_key);
}

LanguageResult IPortalDatabase::__votePoll(shared_ptr<ObjectsUser> user, const Buffer &private_key, shared_ptr<ObjectsPoll> poll, EntityID poll_option, bool isNull, uint32 score)
{	
	// TOFIX_VALIDATING: 
	/*
	if(poll->getObjectType() != portalObjectTypePoll)
		return false;
	*/

	DateTime now = IsisSystem::instance()->getInternetDateTime();
	if(now.isNull())
		return LanguageResult("no_clock_sync");

	ObjectID poll_vote_id = getUserPollVoteID(user->id, poll->getEntityID());

	bool exists = true;

	shared_ptr<ObjectsPollVote> vote = getPollVote(poll_vote_id);
	
	if(vote == null)
	{
		exists = false;

		vote.reset(OS_NEW ObjectsPollVote());

		//automatico vote->id = poll_vote_id;
		vote->author = user->id;
	}

	// Salva la data di creazione/aggiornamento
	vote->submit_date = now;

	vote->parent = poll->getEntityID();
	if(isNull)
	{
		vote->setNull();
	}
	else
	{
		vote->reference = poll_option;
        vote->score = score;
	}

	if(exists)
		return _signAndUpdate(vote, private_key, false);
	else
		return _signAndInsert(vote, private_key);	
}

uint32 IPortalDatabase::__increaseRank(const uint32 &rank, uint32 factor)
{
	return rank + factor;
}

uint32 IPortalDatabase::__decreaseRank(const uint32 &rank, uint32 factor)
{
	uint32 value = rank;
	if(value > factor)
		value -= factor;
	else
		value = 0;

	return value;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
