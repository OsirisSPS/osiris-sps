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

#ifndef _ENTITIES_SNAPSHOTMANAGER_H
#define _ENTITIES_SNAPSHOTMANAGER_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "datadata.h"
#include "datatable.h"
#include "datetime.h"
#include "dbdb.h"
#include "ids.h"
#include "ijob.h"
#include "ipaddress.h"
#include "entitiesentities.h"
#include "extensionsextensions.h"
#include "languageresult.h"
#include "objectsobjects.h"
#include "pimpl_ptr.h"
#include "portalsportals.h"
#include "reputationsreputations.h"
#include "reputationsscore.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ObjectsIObject;
class IPortalDatabase;
class ObjectID;
class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport EntitiesSnapshotManager : public Object
{
public:
	typedef map<String, bool>::type RecursiveGuiltiesMap;
	typedef map<String, int32>::type EnsureHitMap;
	//typedef map<String, snapshot_profile_ptr>::type Profiles;

// Enumerated types
public:	
	enum StabilityStatus
	{
		ssNone					= 0, 
		ssAccept					= 1,
		ssPrepare					= 2,
		ssStabilityUsers			= 3,
		ssStabilityObjects			= 4,
		ssStatisticsObjects			= 5,
		ssStatisticsUsers			= 6,
		ssSearchObjects				= 7,	
		ssFinalize					= 8,
		ssDone						= 9,
	};

// Construction
public:
	EntitiesSnapshotManager(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database);
	virtual ~EntitiesSnapshotManager();

// Attributes
public:
	inline shared_ptr<Portal> getPortal() const;
	
	inline EntitiesEntitiesCache * getEntities() const;
	inline portals::ProfilesCache * getProfiles() const;
	inline EntitiesStatisticsCache * getStatistics() const;

	inline const StabilityStatus& getStabilityStatus() const;
	void setStabilityStatus(StabilityStatus status);
	
	uint32 getSearchLevel() const;
	uint32 getObjectsStep() const;

// Operations
public:

	void unload();

	// Gestione Eventi
public:
	void onChangeReputationLevel(const shared_ptr<IPortalDatabase> &database);
	void onInsertObject(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIObject> &object);
	void onUpdateObject(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIObject> &object);
	void onRemovingObject(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIObject> &object);

	/*
	void onAccountCreated(const shared_ptr<IPortalDatabase> &database, const String &id);
	void onAccountDeleted(const shared_ptr<IPortalDatabase> &database, const String &id);
	void onAccountChanged(const shared_ptr<IPortalDatabase> &database, const String &id, const shared_ptr<ObjectsUser> &user);
	*/

protected:

	//bool allowChangeThresholds(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsUser> &user);
	void onInsertUser(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsUser> &user);
	void onRemoveUser(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsUser> &user);
	void onChangeReputation(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsReputation> &reputation);

	// Altri

public:
	//void rebuildAccount(const shared_ptr<IPortalDatabase> &database, const String &id);
	void setCachesSize(uint32 size);

	// Calcolo singolo
public:
	void ensure(const shared_ptr<IPortalDatabase> &database, const EntityID &primaryID);
	void ensureChilds(const shared_ptr<IPortalDatabase> &database, const EntityID &id);
protected:
	void ensure(const shared_ptr<IPortalDatabase> &database, const EntityID &primaryID, RecursiveGuiltiesMap& recursiveGuilties, EnsureHitMap& ensureHit);
	ReputationsScore computeUserStability(const shared_ptr<IPortalDatabase> &database, const ObjectID &id);
	void computeStatistics(const shared_ptr<IPortalDatabase> &database, const ObjectOrEntityID &id);

	void updateObjectStatistics(const shared_ptr<IPortalDatabase> &database, const EntityID &id);
	void updateObjectSearch(const shared_ptr<IPortalDatabase> &database, const EntityID &id, const int32 &level);
	void updateUserStatistics(const shared_ptr<IPortalDatabase> &database, const ObjectID &id);

	// Stability thread actions
	// I metodo sotto-citati restituiscono 'true' se il loro lavoro  completo.
public:
	bool update(shared_ptr<IJob> job);
protected:
	// La stabilit pu girare?
	bool canStabilityRun(bool withThis, shared_ptr<IJob> job);
	bool computeStabilityAccept(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job);
	bool computeStabilityPrepare(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job);
	bool computeStabilityUsers(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job);
	bool computeStabilityObjects(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job);
	bool computeStatisticsObjects(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job);
	bool computeStatisticsUsers(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job);
	bool computeSearchObjects(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job);
	bool computeFinalize(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job);

	// Record management
	void createObjectRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIRevisionable> &object);
	void removeObjectRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIRevisionable> &object);
	void createUserRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsUser> &user);
	void removeUserRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsUser> &user);
	void createStatisticRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIObject> &object);
	void removeStatisticRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIObject> &object);

	// Cambio stabilit oggetti virtuali
	// Aggiorna lo stato di stabilit di tutti i profili
public:
	void invalidateAll(const shared_ptr<IPortalDatabase> &database);
	void invalidateTotal(const shared_ptr<IPortalDatabase> &database); // In + della invalidateAll, computa anche lo step di Prepare. Usata solo per test.
	//void invalidateObject(const shared_ptr<IPortalDatabase> &database, EntityID primaryID, bool invalidateStability, bool invalidateStatistics, bool invalidateParentStability, bool invalidateParentStatistics);
	//void invalidateObject(const shared_ptr<IPortalDatabase> &database, shared_ptr<ObjectsIRevisionable> object, bool invalidateStability, bool invalidateStatistics, bool invalidateParentStability, bool invalidateParentStatistics);
	void invalidateEntity(const shared_ptr<IPortalDatabase> &database, EntityID entityID, bool invalidateStability, bool invalidateStatistics);
	void invalidateUser(const shared_ptr<IPortalDatabase> &database, const ObjectID& userID, bool invalidateStability, bool invalidateStatistics);
	//void invalidateUser(const shared_ptr<IPortalDatabase> &database, shared_ptr<ObjectsUser> user, bool invalidateStability, bool invalidateStatistics);
	void invalidateChilds(const shared_ptr<IPortalDatabase> &database, EntityID primaryID);
	void resetStabilityStatus(StabilityStatus requiredStatus = ssNone);
	bool rebuildAlignmentHash(const shared_ptr<IPortalDatabase> &database);
protected:
	
	// Profile management
public:
	void ensureSnapshot(shared_ptr<IPortalDatabase> database, bool destroy);
	
	// Utils
	//String getReputationSqlRule(ObjectsReputationThreshold threshold) const;
	bool compareReputation(ObjectsReputationThreshold threshold, double score) const;
	void computeSqlForObjectRevisions(const shared_ptr<IPortalDatabase> &database, const EntityID &entityID, const PortalObjectType &objectType, bool onlyFirst, DataTable &result);

	// ActionTick
public:
	void updateLastActionTick();
protected:
	bool checkLastActionTick();

	// Lucene	
public:
	std::string getSearchPathForLucene() const;
	void optimizeSearchIndexes();
	void destroySearchIndexes(const shared_ptr<IPortalDatabase> &database, bool forRebuild);
protected:
	String convertScoreForLucene(const double &score);
	void initSearchIndexes();
	void deInitSearchIndexes();	
	void updateObjectSearch(const shared_ptr<IPortalDatabase> &database, const EntityID &primaryID, const String &document, const int32 &level);
	void openSearchIndexWriter();
	void closeSearchIndexWriter();

	// Ex SnapshotProfile
public:
	//ObjectID getUserID();

	shared_ptr<EntitiesEntity> getEntity(const shared_ptr<IPortalDatabase> database, const EntityID id);

	ReputationsScore computeReputation(const shared_ptr<IPortalDatabase> &database, const String &reference, const shared_ptr<ReputationsFeedback> &feedback = nullptr);
	

	// Dell'oggetto specificato, ricerca le revisioni valide, e restituisce l'ID di quella da ritenere corrente (primaria esclusa). Restituisce anche lo score dell'utente che l'ha creata.
	// Attenzione, presuppone che gli utenti siano gi stati stabilizzati.
	void getObjectRevision(const shared_ptr<IPortalDatabase> &database, const EntityID &entityID, const PortalObjectType &objectType, ObjectID &id, double &score);
	// Come la getObjectRevision, ma restituisce l'elenco delle revisioni estratte.
	void getObjectRevisions(const shared_ptr<IPortalDatabase> &database, const EntityID &entityID, const PortalObjectType &objectType, StringList &results);

	void updateStabilityDate(const shared_ptr<IPortalDatabase> &database);

	//const DateTime & getStabilityDate(const shared_ptr<IPortalDatabase> &database);
	DateTime getStabilityDate(const shared_ptr<IPortalDatabase> &database);
	void setStabilityDate(const shared_ptr<IPortalDatabase> &database, const DateTime &date);

	//ObjectsReputationThreshold getAuthorsReputationThreshold() const;
	//ObjectsReputationThreshold getEditorsReputationThreshold() const;
	
	// Resetta lo status della stabilit, in modo che il thread di stabilit
	// controlli cosa  necessario stabilizzare.
	
	//void resetUserStability(const shared_ptr<IPortalDatabase> &database, ObjectID userID, bool invalidateStability, bool invalidateStatistics);
	//void resetObjectStability(const shared_ptr<IPortalDatabase> &database, EntityID primaryID, bool invalidateStability, bool invalidateStatistics);




	// Pov Engine	
	void cronChecking();

protected:
	pimpl_of<EntitiesSnapshotManager>::type m_impl;

	weak_ptr<Portal> m_portal;

	//Profiles m_profiles;
	//entities::snapshot_profile_ptr m_snapshotProfile;

	double m_lastActionTick;

	scoped_ptr<EntitiesEntitiesCache> m_entities;
	scoped_ptr<portals::ProfilesCache> m_profiles;
	scoped_ptr<EntitiesStatisticsCache> m_statistics;

	StabilityStatus m_stabilityStatus;
	String m_stabilityStatusCounterName;

	mutable boost::recursive_mutex m_stabilityCS;
	//mutable boost::recursive_mutex m_profilesCS;
	mutable boost::recursive_mutex m_requestsCS;
	boost::recursive_mutex m_searchCS;

public: // Andrebbero come opzioni di sistema o come opzioni portale, da decidere:
	bool m_enableEnsureLoadingEntity; // Se carico un'entit, se  a true viene mostrata la revisione effettiva (stabilizzata al volo).
	bool m_enableEnsureLoadingChilds; // Se guardo un topic, se  a true i post sono quelli effettivi (stabilizzati al volo), se  a false vedo solo i post gi stabilizzati.
	double m_scoreThresholdChange; // Punteggio minimo per permettere ad un utente in un portale monarchico di cambiare le soglie	
	//double m_deltaStabilityTick; // Il thread di stabilit lavora solo se  trascorso questo tempo dall'ultima azione dell'utente
	DateTime m_takeBackFrom;
	DateTime m_takeBackTo;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<Portal> EntitiesSnapshotManager::getPortal() const { return m_portal.lock(); }
//inline SnapshotManager::Profiles SnapshotManager::getProfiles() const { return m_profiles; }
//inline entities::snapshot_profile_ptr SnapshotManager::getSnapshotProfile() const { return m_snapshotProfile; }
//inline uint32 SnapshotManager::getPrimariesCount() const { return m_primariesCount; }
//inline uint32 SnapshotManager::getUsersCount() const { return m_usersCount; }

inline EntitiesEntitiesCache * EntitiesSnapshotManager::getEntities() const { return m_entities.get(); }
inline portals::ProfilesCache * EntitiesSnapshotManager::getProfiles() const { return m_profiles.get(); }
inline EntitiesStatisticsCache * EntitiesSnapshotManager::getStatistics() const { return m_statistics.get(); }

inline const EntitiesSnapshotManager::StabilityStatus& EntitiesSnapshotManager::getStabilityStatus() const { return m_stabilityStatus; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENTITIES_SNAPSHOTMANAGER_H
