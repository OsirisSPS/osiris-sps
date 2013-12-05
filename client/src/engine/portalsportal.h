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

#ifndef _PORTALS_PORTAL_H
#define _PORTALS_PORTAL_H

#include "boost/noncopyable.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "datadata.h"
#include "datetime.h"
#include "dbdb.h"
#include "enable_this_ptr.h"
#include "entitiesentities.h"
#include "ideide.h"
#include "lock.h"
#include "path.h"
#include "p2pp2p.h"
#include "base/object.h"
#include "objectsobjects.h"
#include "ordered_map.h"
#include "portalsportals.h"
#include "reputationsreputations.h"
#include "searchsearch.h"

//////////////////////////////////////////////////////////////////////

#define OS_PORTAL_PEERS_CACHE_SIZE						1000
#define OS_PORTAL_PEERS_CONTACT_DATE_CHECK				15		// Espresso in minuti
#define OS_PORTAL_DATABASES_CACHE_SIZE                  3
#define OS_PORTAL_SEARCH_LEVEL							2
#define OS_PORTAL_SEARCH_LIMIT							1000
#define OS_PORTAL_OBJECTS_STEP							500
#define OS_PORTAL_MIN_USERNAME_LENGTH					3

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;
class IdeSession;
class IPAddress;
class IPortalDatabase;
class PortalsDatabasesCache;
class PortalID;
class PortalOptions;
class PortalsOptionsShared;
class PortalsPeersManager;
class PortalsTransaction;
class XMLDocument;
class Version;

//////////////////////////////////////////////////////////////////////

class EngineExport Portal : public Object,
							public enable_this_ptr<Portal>,
							public boost::noncopyable
{
	class DatabaseImpl;

public:
	class EngineExport DatabaseLockData : public Object
	{
	// Construction
	public:
		DatabaseLockData();
		virtual ~DatabaseLockData();

	// Attributes
	public:
		uint32 getExclusiveRequests() const;

		boost::recursive_mutex & getDatabaseExclusiveLock() const;

	// Operations
	public:
		void addExclusiveRequest();
		void removeExclusiveRequest();

	private:
		void updateExclusiveRequests(bool added);

	private:
		mutable boost::recursive_mutex m_exclusiveCS;
		mutable boost::recursive_mutex m_cs;
		uint32 m_exclusiveRequests;
	};

// Construction
public:
	Portal(uint32 peersCacheSize, uint32 databasesCacheSize);
	virtual ~Portal();

// Attributes
public:
	Path getPath() const;
	bool getLoaded() const;

	inline shared_ptr<PortalOptions> getOptions() const;
	inline shared_ptr<PortalsOptionsShared> getOptionsShared() const;
	inline EntitiesSnapshotManager * getSnapshotManager() const;
	inline shared_ptr<ReputationsManager> getReputationManager() const;
	inline ObjectsObjectsCache * getObjects() const;
	inline portals::Messenger * getMessenger() const;
	inline PortalsPeersManager * getPeersManager() const;

	p2p::exchange_context_ptr getExchangeContext();

	String getName() const;

	const PortalID & getPortalID() const;
	//const ObjectID & getUserID() const;	
	const PovID & getPovID() const;	
	ObjectID getPovUserID() const;	// ObjectID/user of getUserID.
	std::string getFullPovID() const; // Both PortalID and PovID
	std::string getNetworkID() const; 
	shared_ptr<ObjectsUser> getPovUser() const;	

	//PortalType getType_RRR() const;
	//PortalAccess getAccess() const;	

	String getPovName() const;
	
    ObjectID getLastExchangedObject() const;
    void setLastExchangedObject(const ObjectID &lastExchangedObject);

	String getSync() const;
	void setSync(const String &sync);

	String getSearchPath() const;

	// Restituisce un profilo del portale
	portals::profile_ptr getProfile(shared_ptr<IPortalDatabase> database, const ObjectID &id);
	// Restituisce un'entità del portale
	shared_ptr<EntitiesEntity> getEntity(shared_ptr<IPortalDatabase> database, const EntityID &id, bool force = false);
	// Restituisce un'entry del portale
	shared_ptr<ObjectsIObject> getObject(shared_ptr<IPortalDatabase> database, const ObjectID &id);	
	//shared_ptr<ObjectsIObject> getObject(shared_ptr<IPortalDatabase> database, const String &id);	// TOCLEAN


	//std::string getLink(ide::portal::PortalPage page, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getLink(const std::string &page, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getViewObjectLink(const EntityID &id, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getEditObjectLink(const EntityID &id, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));	
	std::string getUserLink(const ObjectID &id);
	std::string getSendMessageLink(const ObjectID &id, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getAvatarLink(const ObjectID &id, const DateTime &dt);
	std::string getFileLink(const ObjectID &id);
	std::string getFileEntityLink(shared_ptr<IPortalDatabase> database, const EntityID &id);
	std::string getPrivateMessageLink(const ObjectID &id, bool secure);

	void setCachesSize(uint32 size);

	void setDatabasesCacheSize(uint32 size);

	//String getBootstrapUrl(bool isNotify) const;

	boost::recursive_mutex & getPortalLock() const;

	uint32 getDatabaseExclusiveRequests() const;
	shared_ptr<PortalsTransaction> startTransaction(bool exclusiveAccess);	

	inline bool getStabilizePending() const;
	inline void setStabilizePending(bool value);

	inline DateTime getLastIsisRequest() const;
	inline void setLastIsisRequest(DateTime value);

	inline uint32 getNodesSameAlign() const;
	inline void setNodesSameAlign(uint32 value);

	inline uint32 getNodesSamePortalPov() const;
	inline void setNodesSamePortalPov(uint32 value);

private:
	void setLoaded(bool loaded);

// Operations
public:
	static std::string generatePovID(const PortalID &portalID, const PovID &povID);

	void reloadUser(const shared_ptr<IPortalDatabase> &database);

	bool saveUser(const shared_ptr<IPortalDatabase> &database, shared_ptr<IdeSession> ideSession);

	void runJobStabilize();
	void runJobOptimizer();
	void runJobSync();
	void runJobImporter(const String &url);

	void save();
	void start();

	bool load(const String &path, shared_ptr<PortalOptions> options, bool recovery);

	bool openDatabase(shared_ptr<IPortalDatabase> &database);

	//bool updateIsisStatus();

	//void ensureBootstrap();

	// Effettua un bootstrap
	//bool bootstrap(bool isNotify);

	// Aggiorna la data di ultimo allineamento terminato con successo
	void updateLastSuccessfulExchange();

	static bool validateUsername(const String &username);
	
	std::string generateInviteLink(bool http) const;
	String generateExportLink() const;
	
private:
	void unload();

	static String createSession();

	bool _updateDatabase(shared_ptr<IPortalDatabase> database, const Version &fromVersion, const Version &toVersion);

	// Richiesta di bootstrap
	//bool needBootstrapRequest();
	// Notifica di bootstrap
	//bool needBootstrapNotify();

	void _initVariables();
	void _createManagers(shared_ptr<IPortalDatabase> database);
	void _clearManagers();

	shared_ptr<PortalsTransaction> startTransaction(shared_ptr<IPortalDatabase> database, bool exclusiveAccess);

	bool onPreDatabaseVersionUpdate(shared_ptr<IPortalDatabase> database, const Version &fromVersion, const Version &toVersion);
	bool onPostDatabaseVersionUpdate(shared_ptr<IPortalDatabase> database, const Version &fromVersion, const Version &toVersion);

protected:
	shared_ptr<ObjectsUser> m_user;
	bool m_loaded;
	Path m_path;
	shared_ptr<PortalOptions> m_options;
	shared_ptr<PortalsOptionsShared> m_optionsShared;
	scoped_ptr<EntitiesSnapshotManager> m_snapshotManager;
	shared_ptr<ReputationsManager> m_reputationsManager;
	scoped_ptr<portals::Messenger> m_messenger;
	scoped_ptr<ObjectsObjectsCache> m_objects;
	scoped_ptr<PortalsDatabasesCache> m_databasesCache;
	scoped_ptr<PortalsPeersManager> m_peersManager;
	bool m_stabilizePending;

	weak_ptr<p2p::ExchangeContext> m_exchangeContext;
	shared_ptr<DatabaseLockData> m_lockData;

	//DateTime m_lastBootstrapRequest;		// Data di ultima richiesta di bootstrap
	//DateTime m_lastBootstrapNotify;			// Data di ultima notifica di bootstrap
	DateTime m_lastSuccessfulExchange;		// Data di ultimo scambio di dati finito
	DateTime m_lastObjectExchange;			// Data di ultimo oggetto ricevuto
	DateTime m_lastIsisRequest;				
	uint32 m_nodesSameAlign;
	uint32 m_nodesSamePortalPov;

	mutable boost::recursive_mutex m_databasesCacheCS;
	mutable boost::recursive_mutex m_portalCS;
	mutable boost::recursive_mutex m_netCS;
	mutable boost::recursive_mutex m_contextCS;
	mutable boost::recursive_mutex m_dataCS;
	shared_ptr<boost::recursive_mutex> m_peersCS;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<PortalOptions> Portal::getOptions() const { return m_options; }
inline shared_ptr<PortalsOptionsShared> Portal::getOptionsShared() const { return m_optionsShared; }
inline EntitiesSnapshotManager * Portal::getSnapshotManager() const { return m_snapshotManager.get(); }
inline shared_ptr<ReputationsManager> Portal::getReputationManager() const { return m_reputationsManager; }
inline ObjectsObjectsCache * Portal::getObjects() const { return m_objects.get(); }
inline portals::Messenger * Portal::getMessenger() const { return m_messenger.get(); }
inline PortalsPeersManager * Portal::getPeersManager() const { return m_peersManager.get(); }
inline bool Portal::getStabilizePending() const { return m_stabilizePending; }
inline void Portal::setStabilizePending(bool value) { m_stabilizePending = value; }

inline DateTime Portal::getLastIsisRequest() const { return m_lastIsisRequest; }
inline void Portal::setLastIsisRequest(DateTime value) { m_lastIsisRequest = value; }

inline uint32 Portal::getNodesSameAlign() const { return m_nodesSameAlign; }
inline void Portal::setNodesSameAlign(uint32 value) { m_nodesSameAlign = value; }

inline uint32 Portal::getNodesSamePortalPov() const { return m_nodesSamePortalPov; }
inline void Portal::setNodesSamePortalPov(uint32 value) { m_nodesSamePortalPov = value; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _PORTALS_PORTAL_H
