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

#ifndef _IPORTALDATABASE_H
#define _IPORTALDATABASE_H

#include "boost/thread/recursive_mutex.hpp"
#include "convert.h"
#include "datetime.h"
#include "datadata.h"
#include "dbdb.h"
#include "dbsqlselect.h"
#include "dbtables.h"
#include "enable_this_ptr.h"
#include "entitiesentities.h"
#include "extensionsextensions.h"
#include "languageresult.h"
#include "base/object.h"
#include "objectsobjects.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IDbConnection;
class IDbStatement;
class IPAddress;
class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport IPortalDatabase : public Object,
                                     public enable_this_ptr<IPortalDatabase>
{
// Construction
public:
	IPortalDatabase(shared_ptr<IDbConnection> connection);
	virtual ~IPortalDatabase();

// Attributes
public:
	inline shared_ptr<DbBinder> getBinder() const;

	inline shared_ptr<IDbConnection> getConnection() const;
	inline bool connected() const;

	inline bool inTransaction() const;

	//inline const String & getPortalID() const; // Razor
	inline shared_ptr<Portal> getPortal() const;

	inline const uint32 & getQuery() const;
	inline const uint32 & getTransaction() const;

	shared_ptr<ObjectsIDescriptor> getDescriptor(PortalObjectType type) const;

	bool xxx_accountExists(const String &id) const;	
	shared_ptr<DataAccount> xxx_getAccount(const String &id);
	shared_ptr<ObjectsPrivateMessage> getPrivateMessage(const ObjectID &id);
	shared_ptr<ObjectsUser> getUser(const ObjectID &id);
	shared_ptr<ObjectsReputation> getReputation(const ObjectID &id);
	shared_ptr<ObjectsAvatar> getAvatar(const ObjectID &id);
	shared_ptr<ObjectsTag> getTag(const ObjectID &id);
	shared_ptr<ObjectsFile> getFile(const ObjectID &id);
	shared_ptr<DataEntry> getEntry(const ObjectID &id) const;
	bool peerExists(const String &ip) const;
	shared_ptr<DataPeer> getPeer(const String &ip) const;
	shared_ptr<ObjectsVote> getVote(const ObjectID &id);
	shared_ptr<ObjectsPollVote> getPollVote(const ObjectID &id);

	uint32 xxx_getAccounts() const;
	uint32 getEntries() const;
	uint32 getPeers() const;
	uint32 getUsers() const;

	bool recordExists(const String &table, const String &field, const String &value) const;
	bool recordExists(const String &table, const StringMap &conditions) const;

	bool needObject(const String &id, const DateTime &submit_date = DateTime::EMPTY) const;

	//virtual shared_ptr<ObjectsIObject> getObject(const ObjectID &id); // TOCLEAN: Abolire, non è sotto cache. Usare loadObject.
	virtual shared_ptr<ObjectsIObject> loadObject(const ObjectID &id);

	//shared_ptr<ObjectsIRevisionable> getRevisionable(const ObjectID &id);

	shared_ptr<DataInstanceCache> getInstanceCache(const ObjectID &reference);

	uint32 getStableObjectsCount(const EntityID &reference, PortalObjectType type);

// Operations
public:
	bool open(shared_ptr<Portal> portal);
	void close();

	//void parse(shared_ptr<DbSqlICommand> command, String &sql);
	//shared_ptr<IDbStatement> prepare(const String &sql);

	bool execute(const String &sql) const;
	bool execute(const String &sql, DataTable &table) const;
	bool execute(shared_ptr<DbSqlICommand> command) const;
	bool execute(shared_ptr<DbSqlICommand> command, DataTable &table) const;
	DataItem queryValue(const String &sql) const;
	//bool executeStatement(const String &sql, const DbSqlValues &values) const;

	//virtual uint32 executeEx(const String &sql);
	//virtual void queryEx(const String &sql, DataTable &result);

	void addToList(const String &sql, StringList &list, bool append = false);

	// Inserisce un nuovo record
	bool insertRecord(shared_ptr<DataIRecord> record);
	// Aggiorna un record esistente
	bool updateRecord(shared_ptr<DataIRecord> record);
	// Rimuove un record
	bool removeRecord(shared_ptr<DataIRecord> record);
	bool removeObject(shared_ptr<ObjectsIObject> object);
	// Valida un record
	bool validateRecord(shared_ptr<DataIRecord> record);
	
	bool beginTransaction();
	bool commit();
	bool rollback();

	bool storeObject2(const shared_ptr<ObjectsIObject> &object);

	uint32 getMinEntryRank();
	uint32 getMaxEntryRank();
	uint32 getNewEntryRank();

//	uint32 getMinPeerRank();
//	uint32 getMaxPeerRank();
//	uint32 getNewPeerRank();

	void computeVotesStats(shared_ptr<ObjectsIRevisionable> object, DbValue<uint32>& outCount, DbValue<double>& outAverage);
	void computeLastObjectStats(shared_ptr<ObjectsIRevisionable> object, DbValue<EntityID>& outID, DbValue<DateTime> &outDate);
	
	bool increaseEntryRank(const ObjectID &id);
	bool decreaseEntryRank(const ObjectID &id);

//	bool increasePeerRank(data::peer_ptr peer);
//	bool decreasePeerRank(data::peer_ptr peer);

//	bool validatePeer(const String &ip);
//	bool invalidatePeer(const String &ip);

//	bool createPeer(const String &ip, const uint32 &port);
//	bool removePeer(const String &ip);
//	bool savePeer(shared_ptr<IPAddress> address, bool validateIfExists);

	LanguageResult setUserAvatar(shared_ptr<ObjectsUser> user, const Buffer &privateKey, const String &fileName, const String &fileType, const Buffer &fileData);
	//bool setReputationValue(shared_ptr<ObjectsReputation> reputation, double score, const String &description, bool follow, const Buffer &private_key);

	shared_ptr<ObjectsVote> getObjectVote(shared_ptr<ObjectsUser> user, shared_ptr<ObjectsIRevisionable> object);
	shared_ptr<ObjectsPollVote> getPollVote(shared_ptr<ObjectsUser> user, shared_ptr<ObjectsPoll> poll);

	LanguageResult voteObject(shared_ptr<ObjectsUser> user, const Buffer &private_key, const EntityID &reference, uint32 score);
	LanguageResult unvoteObject(shared_ptr<ObjectsUser> user, const Buffer &private_key, const EntityID &reference);

	LanguageResult votePoll(shared_ptr<ObjectsUser> user, const Buffer &private_key, shared_ptr<ObjectsPoll> poll, EntityID poll_option);
	LanguageResult unvotePoll(shared_ptr<ObjectsUser> user, const Buffer &private_key, shared_ptr<ObjectsPoll> poll);

	// Se object == nullptr rimuove tutte le statistiche del profilo, altrimenti solo quello sull'oggetto specificato
	bool removeProfileStatistics(const String &table, ObjectOrEntityID reference);

	template <typename T>
	shared_ptr<DataIStatistics> loadStatistics(const String &table, const ObjectOrEntityID &object_id) const;

protected:
	shared_ptr<DataAccount> xxx_loadAccount(const String &id) const;
	shared_ptr<DataEntry> loadEntry(const String &id) const;
	shared_ptr<DataPeer> loadPeer(const String &ip) const;
	shared_ptr<DataPollStats> loadPollStats(const String &reference) const;
	shared_ptr<DataPollOptionStats> loadPollOptionStats(const String &reference) const;
	shared_ptr<DataDiscussionStats> loadDiscussionStats(const String &reference) const;
	shared_ptr<DataUserStats> loadUserStats(const String &reference) const;

public:
	// Crea un nuovo oggetto firmandolo con la chiave privata specificata
	LanguageResult _signAndInsert(shared_ptr<ObjectsIObject> object, const Buffer &private_key); // TOCLEAN
	// Aggiorna un oggetto esistente (aggiornando la firma)
	LanguageResult _signAndUpdate(shared_ptr<ObjectsIObject> object, const Buffer &private_key, bool updateSubmitDate); // TOCLEAN

	// Genera l'ID di un oggetto
	//static ObjectID generateID(PortalObjectType type, bool primary, const std::string &hash);
	// Genera un ID casuale
	//static ObjectID generateRandomID(PortalObjectType type, bool primary);

	// Dato l'id di un utente restituisce un id univoco che ha una relazione di dipendenza
	static ObjectID getUserRelatedObjectID(const ObjectID &user_id, const std::string &relation, const std::string &reference, PortalObjectType type);

	// Restituisce l'id della reputazione di un utente verso l'utente specificato
	static ObjectID getUserReputationID(const ObjectID &user_id, const ObjectID &reference);
	// Restituisce l'id dell'avatar di un utente
	static ObjectID getUserAvatarID(const ObjectID &user_id);

	// Restituisce l'id di un voto ad un poll
	static ObjectID getUserPollVoteID(const ObjectID &user_id, const EntityID &object_id);
	// Restituisce l'id di un voto ad un oggetto
	static ObjectID getUserVoteID(const ObjectID &user_id, const EntityID &object_id);

	static bool isUpdatable(PortalObjectType type);
	static bool isRevisionable(PortalObjectType type);
	static bool isVotable(PortalObjectType type);

	

private:
	void logError(const String &sql, std::exception &e) const;

	void _query() const;

	bool _createEntry(shared_ptr<ObjectsIObject> object, const uint32 &rank);

	bool _validateObject(const shared_ptr<ObjectsIObject> &object);

	/*
	bool _insertRecord(const String &table, const shared_ptr<DataIRecord> &record);

	bool _insertObject(const shared_ptr<ObjectsIObject> &object);
	bool _insertStatistics(const shared_ptr<DataIStatistics> &statistics);
	bool _insertLocalMessage(const shared_ptr<DataLocalMessage> &message);
	bool _insertInstanceCache(const data::instance_cache_ptr &instance_cache);

	bool xxx__updateAccount(const shared_ptr<DataAccount> &account);
	bool _updateEntry(const shared_ptr<DataEntry> &entry);
	bool _updateObject(const shared_ptr<ObjectsIObject> &object);
	bool _updateObject(const String &table, const shared_ptr<ObjectsIObject> &object);
	bool _updatePeer(const data::peer_ptr &peer);
	bool _updateStatistics(const shared_ptr<DataIStatistics> &statistics);
	bool _updateStatistics(const String &table, const shared_ptr<DataIStatistics> &statistics);
	bool _updateLocalMessage(const shared_ptr<DataLocalMessage> &message);
	bool _updateInstanceCache(const data::instance_cache_ptr &instance_cache);
	*/

	/*
	bool _removeEntry(const shared_ptr<DataEntry> &entry);
	bool _removeObject(const shared_ptr<ObjectsIObject> &object);
	bool _removeObject(const String &table, const shared_ptr<ObjectsIObject> &object);
	bool _removePeer(const data::peer_ptr &peer);
	bool _removeStatistics(const shared_ptr<DataIStatistics> &statistics);
	bool _removeStatistics(const String &table, const data::IStatistics &statistics);
	bool _removeLocalMessage(const shared_ptr<DataLocalMessage> &message);
	bool _removeInstanceCache(const data::instance_cache_ptr &instance_cache);
	*/

	template <typename T>
	shared_ptr<T> _recordFromSQL(shared_ptr<DbSqlICommand> sql) const;

	uint32 _getRecords(const String &table) const;

	//bool __createObject(shared_ptr<ObjectsIObject> object, PortalObjectType type, /*bool isPrimary,*/ ObjectID &id, DateTime submit_date, const uint32 &version);
	//bool __createEntry(objects::entry_ptr entry, PortalObjectType type, /*bool isPrimary,*/ ObjectID &id, const DateTime &submit_date, const uint32 &version, const ObjectID &author, const Buffer &private_key);
	//bool __createRevisionable(shared_ptr<ObjectsIRevisionable> revisionable, PortalObjectType type, bool isPrimary, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const ObjectID &parent, shared_ptr<ObjectsIRevisionable> primary, bool visible, double position, const Buffer &private_key);
	//bool __createPrimary(shared_ptr<ObjectsIRevisionable> revisionable, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const String &parent, const Buffer &private_key);
	//bool __createRevision(shared_ptr<ObjectsIRevisionable> revisionable, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const String &parent, const String &revision, bool visible, double position, const Buffer &private_key);

	//bool __createObjectImpl(shared_ptr<ObjectsIObject> object, PortalObjectType type, /*bool isPrimary,*/ ObjectID &id, const DateTime &submit_date, const uint32 &version, const Buffer &private_key);
	//bool __createEntryImpl(objects::entry_ptr entry, PortalObjectType type, /*bool isPrimary,*/ ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const Buffer &private_key);
	//bool __createPrimaryImpl(shared_ptr<ObjectsIRevisionable> revisionable, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const String &parent, const Buffer &private_key);
	//bool __createRevisionImpl(shared_ptr<ObjectsIRevisionable> revisionable, PortalObjectType type, ObjectID &id, const DateTime &submit_date, const uint32 &version, const String &author, const String &parent, const String &revision, bool visible, double position, const Buffer &private_key);

public:
	//bool __insertObject(shared_ptr<ObjectsIObject> object, const Buffer &private_key);

	//bool __createRevisionDelete(shared_ptr<ObjectsIRevisionable> revisionable, ObjectID &id, const String &author, const Buffer &private_key);
	//bool __createRevisionMove(shared_ptr<ObjectsIRevisionable> revisionable, const String &newParent, ObjectID &id, const String &author, const Buffer &private_key);
	//bool __createRevisionRestore(shared_ptr<ObjectsIRevisionable> revision, ObjectID &id, const String &author, const Buffer &private_key);

	//bool __createReputation(const ObjectID &reference, double score, const String &description, bool follow, const String &author, const Buffer &private_key, ObjectID &id);

	//bool __createUser(const String &name, const Buffer &public_key, const Buffer &private_key, ObjectID &id);

	//bool __createPrivateMessage(const Buffer &session, const Buffer &content, const shared_ptr<ObjectsUser> &adressee, const String &author, const Buffer &private_key, ObjectID &id, const DateTime &submitDate = DateTime::EMPTY);

	LanguageResult __voteObject(shared_ptr<ObjectsUser> user, const Buffer &private_key, const EntityID &reference, bool isNull, uint32 score);
	LanguageResult __votePoll(shared_ptr<ObjectsUser> user, const Buffer &private_key, shared_ptr<ObjectsPoll> poll, EntityID poll_option, bool isNull, uint32 score);

// Interface
protected:
	virtual void onInsertRecord(shared_ptr<DataIRecord> record) = 0;
	virtual void onUpdateRecord(shared_ptr<DataIRecord> record) = 0;
	virtual void onRemovingRecord(shared_ptr<DataIRecord> record) = 0;

public: // toclean
	static uint32 __increaseRank(const uint32 &rank, uint32 factor);
	static uint32 __decreaseRank(const uint32 &rank, uint32 factor);

protected:
	shared_ptr<DbBinder> m_binder;
	shared_ptr<IDbConnection> m_connection;			// Connessione al database

	weak_ptr<Portal> m_portal;
	bool m_connected;						// Stato della connessione
	//String m_portalID;						// ID di base delle entit del database
	mutable uint32 m_query;					// Indice della query corrente
	uint32 m_transaction;					// Livello transazione corrente
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<DbBinder> IPortalDatabase::getBinder() const { return m_binder; }

inline shared_ptr<IDbConnection>  IPortalDatabase::getConnection() const { return m_connection; }
inline bool IPortalDatabase::connected() const { return m_connected; }

inline bool IPortalDatabase::inTransaction() const { return m_transaction > 0; }

//inline const String & IPortalDatabase::getPortalID() const { return m_portalID; }
inline shared_ptr<Portal> IPortalDatabase::getPortal() const { return m_portal.lock(); }

inline const uint32 & IPortalDatabase::getQuery() const { return m_query; }
inline const uint32 & IPortalDatabase::getTransaction() const { return m_transaction; }

//////////////////////////////////////////////////////////////////////

template <typename T>
shared_ptr<DataIStatistics> IPortalDatabase::loadStatistics(const String &table, const ObjectOrEntityID &object_id) const
{
	shared_ptr<DbSqlSelect> select(OS_NEW DbSqlSelect(table));
	select->where.add(DBTABLES::REFERENCE, Convert::toSQL(object_id));
	select->limit.setCount(1);

	return _recordFromSQL<T>(select);
}

template <typename T>
shared_ptr<T> IPortalDatabase::_recordFromSQL(shared_ptr<DbSqlICommand> sql) const
{
	shared_ptr<T> ptr;

	DataTable result;
	if(execute(sql, result) == false)
		return ptr;		// nullptr

	if(result.rows() != 1)
		return ptr;		// nullptr

	ptr.reset(OS_NEW_T(T), os_delete_t());
	ptr->read(result[0]);
	return ptr;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IPORTALDATABASE_H
