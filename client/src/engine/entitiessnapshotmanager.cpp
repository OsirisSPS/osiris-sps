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
#include "entitiessnapshotmanager.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "buffer.h"
#include "constants.h"
#include "convert.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "dataistatistics.h"
#include "dataitem.h"
#include "idbconnection.h"
#include "idesession.h"
#include "iportaldatabase.h"
#include "engine.h"
#include "entitiesentitiescache.h"
#include "entitiesentity.h"
#include "entitiesstatisticscache.h"
#include "extensionssystem.h"
#include "iextensionscomponent.h"
#include "filesystem.h"
#include "log.h"
#include "lucene.h"
#include "notificationsmanager.h"
#include "objectsdescriptor.h"
#include "objectsiobject.h"
#include "objectsirevisionable.h"
#include "objectsobjectscache.h"
#include "objectssystem.h"
#include "objectspollvote.h"
#include "objectsreputation.h"
#include "objectssection.h"
#include "objectsuser.h"
#include "objectsvote.h"
#include "omlmanager.h"
#include "options.h"
#include "platformmanager.h"
#include "portalsaccount.h"
#include "portalsoptionsshared.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsprofilescache.h"
#include "portalsstabilityjob.h"
#include "portalstransaction.h"
#include "realtimestats.h"
#include "reputationsmanager.h"
#include "timerperformance.h"
#include "utils.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// Per spostare la dipendenza del lucene solo nel cpp (si potrebbe volendo anche spostare qui le altre relative metodi/variabili)

template <>
class pimpl<EntitiesSnapshotManager>
{
public:
	pimpl();
	virtual ~pimpl();

public:
	lucene::analysis::standard::StandardAnalyzer luceneAnalyzer;
	shared_ptr<lucene::index::IndexWriter> luceneIndexWriter;
};

//////////////////////////////////////////////////////////////////////

pimpl<EntitiesSnapshotManager>::pimpl()
{

}

pimpl<EntitiesSnapshotManager>::~pimpl()
{

}

//////////////////////////////////////////////////////////////////////

EntitiesSnapshotManager::EntitiesSnapshotManager(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database) : m_portal(portal),
																									m_entities(OS_NEW EntitiesEntitiesCache()),
																									m_profiles(OS_NEW portals::ProfilesCache()),
																									m_statistics(OS_NEW EntitiesStatisticsCache()),
																									m_stabilityStatus(ssNone)
{
	m_enableEnsureLoadingEntity = false;
	m_enableEnsureLoadingChilds = false;
	//m_deltaStabilityTick = 5000;
	m_scoreThresholdChange = 0.9;

	//m_snapshotProfile.reset(OS_NEW SnapshotProfile(getPortal()));
	//m_snapshotProfile->load(database, entities::SnapshotProfile::GLOBAL);

	ensureSnapshot(database, false);

	resetStabilityStatus(ssNone);

	updateLastActionTick();
}


EntitiesSnapshotManager::~EntitiesSnapshotManager()
{
}

void EntitiesSnapshotManager::unload()
{
	deInitSearchIndexes();
}

/* -----------------------------------------------------------
   Attributi
----------------------------------------------------------- */

void EntitiesSnapshotManager::setStabilityStatus(StabilityStatus status)
{
	m_stabilityStatus = status;

	if(!m_stabilityStatusCounterName.empty())
		RealtimeStatsManager::signal(m_stabilityStatusCounterName, status);
}

uint32 EntitiesSnapshotManager::getSearchLevel() const
{
	uint32 searchLevel = Options::instance()->getOption<uint32>(Options::portals_options::search_level);
	if(searchLevel > 2)
		searchLevel = 2;
	return searchLevel;
}

uint32 EntitiesSnapshotManager::getObjectsStep() const
{
	uint32 objectsStep = Options::instance()->getOption<uint32>(Options::portals_options::objects_step);
	if(objectsStep == 0)
		objectsStep = 1;
	return objectsStep;
}

/* -----------------------------------------------------------
   Gestione Eventi
----------------------------------------------------------- */

void EntitiesSnapshotManager::onChangeReputationLevel(const shared_ptr<IPortalDatabase> &database)
{
	OS_LOCK(m_stabilityCS);

	updateLastActionTick();

	invalidateAll(database);
}

void EntitiesSnapshotManager::onInsertObject(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIObject> &object)
{
	OS_LOCK(m_stabilityCS);

	updateLastActionTick();

	getPortal()->getOptions()->updateLastObjectDate();
	getPortal()->getOptions()->updateAlignmentHash(object->id, object->submit_date, true);
	//getPortal()->save(); // TOOPTIMIZE.. saving alignmenthash is important, if a crash occur maybe unaligned.
	
	getPortal()->getObjects()->remove(object->id);
	
	if(IPortalDatabase::isRevisionable(object->getObjectType()))
	{
		shared_ptr<ObjectsIRevisionable> revision = objects_revisionable_cast(object);
        //if(revision->isEntity()) // OS_TODOCIP: Da abolire
		{
			createObjectRecord(database, revision);		// TOCLEAN: rinominare in "ensureObjectRecord"	
		}

		/*
		// 0.11
		switch(object->id->getObjectType())
		{
		case portalObjectTypePost:
		case portalObjectTypeTag:
			invalidateEntity(database, revision, true, true, false, true);
		default:
			invalidateEntity(database, revision, true, true, true, true);
		}
		*/
		// 0.12
		invalidateEntity(database, revision->getEntityID(), true, true);
		
		// Se già c'erano dei figli, sono attualmente figli di "orphans".
		// Invalidare l'oggetto non invalida i figli, dato che han già un padre valido.
		// Per cui li cerco e li invalido qui.
		
		String sql;
		sql += _S("select distinct ts.entity ");
		sql += _S("from  ");
		sql += _S("os_entries te, ");
		sql += _S("os_snapshot_objects ts ");
		sql += _S("where ");
		sql += _S("te.parent = '%S' and ");
		sql += _S("ts.entity = te.entity and ");
		sql += _S("ts.parent='%S' ");		

		sql = String::format(sql.c_str(),
			revision->getEntityID().toUTF16().c_str(),
			ObjectsSystem::instance()->getOrphanID().toUTF16().c_str());
		
		DataTable result;
		database->execute(sql,result);

		for(uint32 r=0;r<result.rows();r++)
		{
			String childID = result.get(r,_S("entity"));
			invalidateEntity(database, childID.to_ascii(), true, true);
		}				
	}
	else
	{
		switch(object->getObjectType())
		{
		case portalObjectTypeReputation:
			{
				onChangeReputation(database,objects_reputation_cast(object));
			} break;

		case portalObjectTypeUser:
			{
				onInsertUser(database,objects_user_cast(object));
			} break;

		case portalObjectTypePollVote:
			{
				shared_ptr<ObjectsPollVote> pollVote = objects_poll_vote_cast(object);
				invalidateEntity(database, pollVote->reference, false, true);
			} break;
		case portalObjectTypeVote:
			{
				shared_ptr<ObjectsVote> vote = objects_vote_cast(object);
				invalidateEntity(database, vote->reference, false, true);
			} break;
		}
	}

	// For the POV user, force the reload of options (and relaunch stability if pov has changed)
	if( (object->getObjectType() == portalObjectTypeUser) && (object->id == getPortal()->getPovUserID()) )
	{
		getPortal()->reloadUser(database);
	}

	invalidateUser(database, object->getAuthor(), false, true);	
}

void EntitiesSnapshotManager::onUpdateObject(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIObject> &object)
{
	OS_LOCK(m_stabilityCS);

	updateLastActionTick();
	getPortal()->getOptions()->updateLastObjectDate();

	// PAZZO, DEBUGGARE, DEVE RIMUOVERE IL VECCHIO...
	getPortal()->getOptions()->updateAlignmentHash(object->id, object->submit_date, false);
	getPortal()->getOptions()->updateAlignmentHash(object->id, object->submit_date, true);

	getPortal()->getObjects()->remove(object->id);

    switch(object->getObjectType())
	{
	case portalObjectTypeReputation:
		{
			onChangeReputation(database,objects_reputation_cast(object));
		} break;

	case portalObjectTypePollVote:
		{
			shared_ptr<ObjectsPollVote> pollVote = objects_poll_vote_cast(object);
			invalidateEntity(database, pollVote->reference, false, true);
		} break;
	case portalObjectTypeVote:
		{
			shared_ptr<ObjectsVote> vote = objects_vote_cast(object);
			invalidateEntity(database, vote->reference, false, true);
		} break;
	}

	// For the POV user, force the reload of options (and relaunch stability if pov has changed)
	if( (object->getObjectType() == portalObjectTypeUser) && (object->id == getPortal()->getPovUserID()) )
	{
		getPortal()->reloadUser(database);
	}

	invalidateUser(database, object->getAuthor(), false, true);
}

void EntitiesSnapshotManager::onRemovingObject(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIObject> &object)
{
	OS_LOCK(m_stabilityCS);

	updateLastActionTick();

	getPortal()->getOptions()->updateAlignmentHash(object->id, object->submit_date, false);

	// 05/01/2012 Razor: per me sta roba non dovrebbe servir +

	/*

	if(IPortalDatabase::isRevisionable(object->getObjectType()))
	{
		shared_ptr<ObjectsIRevisionable> revision = objects_revisionable_cast(object);
				
		// Invalido l'oggetto (anche se non verrà cmq + computato), in modo che vengano invalidate le statistiche
		invalidateEntity(database, revision, true, true, false, false);
		// Invalido i figli, in modo che diventino orfani
		invalidateChilds(database, revision->getEntityID());

        if(revision->isEntity()) // OS_TODOCIP: Da abolire
			removeObjectRecord(database,revision);
	}
	else
	{
		switch(object->getObjectType())
		{
		case portalObjectTypeReputation:
			{
				onChangeReputation(database, objects_reputation_cast(object));
			} break;

		case portalObjectTypeUser:
			{
				onRemoveUser(database, objects_user_cast(object));
			} break;

		case portalObjectTypePollVote:
			{
				shared_ptr<ObjectsPollVote> poolVote = objects_poll_vote_cast(object);
				invalidateEntity(database, poolVote->reference, false, true, false, false);
			} break;
		case portalObjectTypeVote:
			{
				shared_ptr<ObjectsVote> vote = objects_vote_cast(object);
				invalidateEntity(database, vote->reference, false, true, false, false);
			} break;
		}
	}

	invalidateUser(database, object->getAuthor(), false, true);
	*/

	getPortal()->getObjects()->remove(object->id);
}

/*
void EntitiesSnapshotManager::onAccountCreated(const shared_ptr<IPortalDatabase> &database, const String &id)
{
	if(isMonarchic())
	{
		// Lo creo quando creo il primo account...
		if(existsProfile(SnapshotProfile::GLOBAL) == false)
			createProfile(database, SnapshotProfile::GLOBAL);

		// Da notare che non creo in ogni caso il profilo, lo creer se si modificher le soglie.
	}
	else
	{
		createProfile(database, id);
	}	
}

void EntitiesSnapshotManager::onAccountDeleted(const shared_ptr<IPortalDatabase> &database, const String &id)
{
	//destroyProfile(database,id);
}

void EntitiesSnapshotManager::onAccountChanged(const shared_ptr<IPortalDatabase> &database, const String &id, const shared_ptr<ObjectsUser> &user)
{
	//ensureProfile(database,id,user);
}
*/

void EntitiesSnapshotManager::onInsertUser(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsUser> &user)
{
	OS_TRACE(String::format(_S("SnapshotManager::onInsertUser '%S'").c_str(),user->id->toUTF16().c_str()).to_ascii());
	createUserRecord(database,user);
	resetStabilityStatus(ssStabilityUsers);
}

void EntitiesSnapshotManager::onRemoveUser(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsUser> &user)
{
	removeUserRecord(database,user);
}

void EntitiesSnapshotManager::onChangeReputation(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsReputation> &reputation)
{
	// Razor
	ReputationsScore scoreDetail = computeUserStability(database, ObjectID(reputation->author));
	if(scoreDetail.getFollow())
		invalidateAll(database);
	
	/*
	for(Profiles::const_iterator i = m_profiles.begin(); i != m_profiles.end(); ++i)
	{
		snapshot_profile_ptr profile = i->second;
		if(profile->isUserAvailable())
		{
			// Testo se il profilo "ascolta" il parere dell'autore della reputazione.
			// Se si, per ora invalido tutto (ulteriori ottimizzazioni forse fattibili).
			// Al posto di invalidare tutto, potrei invalidare solo le primarie di cui esistono delle entry create dall'utente "to" della reputazione.

			ReputationsScore scoreDetail = computeUserStability(database, profile, ObjectID(reputation->author));
			if(scoreDetail.getFollow())
			{
				profile->invalidateAll(database);
			}
		}
		else
		{
			// Non è il massimo...
			// URGENT
			// Qui, come in altri punti, il fatto che l'account non sia associabile all'utente mi impedisce di ottimizzare.
			// Mi chiedo se ne vale la pena: tanto Osiris è installabile in un volume TrueCrypt.
			profile->invalidateAll(database);
		}
	}
	*/
}

/* -----------------------------------------------------------
   Altri
----------------------------------------------------------- */

/*
void EntitiesSnapshotManager::rebuildAccount(const shared_ptr<IPortalDatabase> &database, const String &id)
{
	OS_LOCK(m_stabilityCS);

	onAccountDeleted(database, id);
	onAccountCreated(database, id);
}
*/

void EntitiesSnapshotManager::setCachesSize(uint32 size)
{
	m_entities->setLimit(size);
	m_profiles->setLimit(size);
	m_statistics->setLimit(size);
}

/* -----------------------------------------------------------
   Calcolo singolo
----------------------------------------------------------- */

void EntitiesSnapshotManager::ensure(const shared_ptr<IPortalDatabase> &database, const EntityID &primaryID)
{
	RealtimeStatsManager::signal(_S("Objects"), _S("Stability - Ensure"), 1, RealtimeStatsCounter::ctRelative, false, _S("objects")); // URGENT

	OS_LOCK(m_stabilityCS);

	RecursiveGuiltiesMap recursiveGuilties;
	EnsureHitMap ensureHit;
	ensure(database, primaryID, recursiveGuilties, ensureHit);
}

int32 ensureHitted(EntitiesSnapshotManager::EnsureHitMap& ensureHit, const EntityID &primaryID)
{
	EntitiesSnapshotManager::EnsureHitMap::iterator i = ensureHit.find(primaryID.toUTF16());
	if(i == ensureHit.end())
		return -1;

	return i->second;
}

void EntitiesSnapshotManager::ensure(const shared_ptr<IPortalDatabase> &database, const EntityID &primaryID, RecursiveGuiltiesMap& recursiveGuilties, EnsureHitMap& ensureHit)
{
	RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::ensure"));

	if(primaryID.getString() == "03000002AA2EB1E25AD4F834EA5855D8C5F910DEDC745060")
		OS_LOG_INFO(_S("hit"));
	
#ifndef OS_TODOCIP
	shared_ptr<ObjectsIRevisionable> primaryObj = objects_revisionable_cast(getPortal()->getObject(database, primaryID.toUTF16()));
	if(primaryObj == nullptr)
		return;
#endif

	PortalObjectType objectType;
		
	DateTime currentStabilityDate;
	shared_ptr<ObjectsIRevisionable> currentEntry;
	EntityID currentParent;

	// Se invalido un oggetto, i suoi figli sono invalidi, ma ancora in cache e quindi trovati qui...
	// Vedi commento in SnapshotProfile::resetObjectStability.
	// Per ora quando invalido un oggetto, al posto di togliere tutti i figli in ricorsivo,
	// svuoto direttamente tutta la cache.
	shared_ptr<EntitiesEntity> currentEntity = getEntities()->find(primaryID);
	//currentEntity = nullptr; // Se decommentato, funziona tutto cmq senza usare la cache. // FABRYURGENT
	if(currentEntity != nullptr)
	{
		// Uso i dati in cache
		objectType = currentEntity->getObjectType();
		currentStabilityDate = currentEntity->getStabilityDate();
		currentEntry = currentEntity->getCurrent();
		currentParent = currentEntity->getParentID();
	}
	else
	{
		// Li estraggo dalla os_snapshot_objects.
		RealtimeStatsScopeTimer RSSXX1(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::ensure::aaa1"));
		DataTable result;
		String sql = String::format(_S("select type,stability_date,current,parent from os_snapshot_objects where entity='%S'").c_str(), primaryID.toUTF16().c_str());
		database->execute(sql,result);
		RSSXX1.stop();
		if(result.rows() == 1)
		{
			objectType = Convert::toObjectType(static_cast<uint32>(result.get(0,_S("type"))));
			currentStabilityDate = String(result.get(0, _S("stability_date")));
			ObjectID currentEntryID = static_cast<String>(result.get(0, _S("current"))).to_ascii();
			currentParent = static_cast<String>(result.get(0, _S("parent"))).to_ascii();

			if(!currentEntryID.empty())
			{
				RealtimeStatsScopeTimer RSSXX2(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::ensure::aaa2"));
				currentEntry = objects_revisionable_cast(getPortal()->getObject(database, currentEntryID));
				RSSXX2.stop();
			}
		}
		else
		{
			// Se non c', inutile proseguire.
			return;
		}
	}

	bool needFirstPhase = false;
	// Se la stability_date  a nullptr, va calcolata la prima fase.
	if(currentStabilityDate.isNull()) needFirstPhase = true;

	/*
	// Se la stability_date  minore della stability_date del profilo, allora va calcolata la prima fase.
	// Se le reputazioni vengono modificate, o le soglie, tutto va ricalcolato come prima fase perch potrebbe variare la corrente,
	// ma il cambio di reputazioni non ha settato a nullptr tutti i record della os_snapshot_objects.
	if( (currentStabilityDate.isValid()) && (currentStabilityDate<profile->getStabilityDate(database)) )
		needFirstPhase = true;
	*/

	// Potrebbe essere in una situazione di ricorsività, per cui verifico anche che non sia già stata fatta
	// la prima fase in questo ciclo.
	if( (needFirstPhase) && (ensureHitted(ensureHit, primaryID) < 1) )
	{
		RealtimeStatsScopeTimer RSS(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::ensure::FirstPhase"));

		// Prima fase. Calcolo tutto tranne le relazioni con i padri e la visibilit.
		ObjectID currentID;
		double position = 0;
		DateTime insertDate;
		double score;
		String title;

		if(ObjectsSystem::instance()->isVirtual(primaryID))
		{
			currentID = ObjectsSystem::instance()->getVirtualObjectID(primaryID);
			currentEntry = objects_revisionable_cast(database->getPortal()->getObject(database, currentID));
			position = 0;
			insertDate = DateTime::EMPTY;
			score = 1;
			title = String::EMPTY;
		}
		else
		{
			String table = ObjectsSystem::instance()->getDescriptor(objectType)->getTableName();

			// Estrae gli utenti che hanno creato revisioni di quell'oggetto e verifica che siano stabilizzati.
			// Non serve se tutti gli utenti sono gi stati stabilizzati.
			if(getStabilityStatus() <= ssStabilityUsers)
			{
#ifdef OS_TODOCIP
				RealtimeStatsScopeTimer RSS(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::ensure::users"));
				
				DataTable result;
				String sql = String::format(_S("select distinct author from %S where entity='%S'").c_str(), table.c_str(), primaryID.toUTF16().c_str());
				database->execute(sql,result);

				for(uint32 r=0;r<result.rows();r++)
				{
					String childAuthor=result.get(r,_S("author"));

					computeUserStability(database, childAuthor.to_ascii());					
				}
#else
				RealtimeStatsScopeTimer RSS(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::ensure::users"));
				String primaryAuthor = primaryObj->author->toUTF16();
				computeUserStability(database, primaryAuthor.to_ascii());

				DataTable result;
				String sql = String::format(_S("select distinct author from %S where revision='%S'").c_str(), table.c_str(), primaryID.toUTF16().c_str());
				database->execute(sql,result);

				for(uint32 r=0;r<result.rows();r++)
				{
					String childAuthor=result.get(r,_S("author"));

					if(childAuthor != primaryAuthor) // Se  l'autore primario, l'ho gi stabilizzato.
					{
						computeUserStability(database, childAuthor.to_ascii());
					}
				}
#endif
			}

			{
				RealtimeStatsScopeTimer RSS(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::ensure::getObjectRevision"));
				getObjectRevision(database, primaryID, objectType, currentID, score);
			}

			shared_ptr<ObjectsIRevisionable> currentObj;
			if(!currentID.empty())
				currentObj = objects_revisionable_cast(getPortal()->getObject(database, currentID));

			// Calcola la insert_date
			if(currentObj != nullptr)
			{
				DataTable result;
				String sql=String::format(_S("select insert_date from os_entries where id='%S'").c_str(), currentID.toUTF16().c_str());
				database->execute(sql,result);
				if(result.rows()>=1)
					insertDate = String(result.get(0, _S("insert_date")));
			}

			// Calcola il position
			if(currentObj != nullptr)
			{
				position = currentObj->position;
				title = currentObj->getTitle();
			}

			currentEntry = currentObj;

			// Se l'entit  una sezione e il componente della nuova corrente  diverso dal componente della corrente precedente,
			// notifica al vecchio componente di rimuovere le statistiche, e al nuovo di crearle.
			if(objectType == portalObjectTypeSection)
			{
				DataTable result;
				String sql=String::format(_S("select current from os_snapshot_objects where entity='%S'").c_str(), primaryID.toUTF16().c_str());
				database->execute(sql,result);
				if(result.rows()==1)
				{
					ObjectID oldCurrentID = static_cast<String>(result.get(0, _S("current"))).to_ascii();
					shared_ptr<ObjectsSection> oldCurrentObj = objects_section_cast(getPortal()->getObject(database, oldCurrentID));
					shared_ptr<ObjectsSection> newCurrentObj = objects_section_cast(currentObj);

					if( (oldCurrentObj != nullptr) && (newCurrentObj != nullptr) && (oldCurrentObj->component == newCurrentObj->component) )
					{
						// Nulla da fare
					}
					else
					{
						if(oldCurrentObj != nullptr)
							oldCurrentObj->getDescriptor()->removeStatistics(database, oldCurrentObj);
						if(newCurrentObj != nullptr)
							newCurrentObj->getDescriptor()->createStatistics(database, newCurrentObj);
					}
				}
			}
		}
				
		OS_TRACE(String::format(_S("SnapshotManager:: Stability(1): %S\n").c_str(),primaryID.toUTF16().c_str()).to_ascii());

		// Da notare che non salvo il parent, dato che potrebbe non essere esatto:
		// il vero parent lo calcolo nello step 2.
		// Prima update sulla os_snapshot_objects

		// Nota: tolto lo score, vedi TOCLEAN_SNAPSHOT_SCORE
		/*
		String sql=String::format(_S("update os_snapshot_objects set current='%S',position=%f,title=%S,score=%f,insert_date='%S',stability_date='%S' where reference='%S'").c_str(),
			currentID.toUTF16().c_str(),
			position,
			Convert::toSQL(title).c_str(),
			score,
			insertDate.toString().c_str(),
			DateTime::EMPTY.toString().c_str(),
			primaryID.toUTF16().c_str());
		database->execute(sql);
		*/
		String sql=String::format(_S("update os_snapshot_objects set current='%S',position=%f,title=%S,insert_date='%S',stability_date='%S' where entity='%S'").c_str(),
			currentID.toUTF16().c_str(),
			position,
			Convert::toSQL(title).c_str(),
			insertDate.toString().c_str(),
			DateTime::EMPTY.toString().c_str(),
			primaryID.toUTF16().c_str());
		database->execute(sql);

		if(currentEntry != nullptr)
			currentParent = currentEntry->parent;

		// Lo rimuovo dalla cache
		getEntities()->remove(primaryID);

		ensureHit[primaryID.toUTF16()] = 1; // Segno che sto questa entità è stata stabilizzata alla prima fase
	}

	// Seconda fase.
	// La faccio se ho fatto la prima fase, oppure se uno dei padri fino alla root ha una data di stabilit + grande.
	// Devo calcolare il parent, il section, il visible.
	bool needSecondPhase = needFirstPhase;


	// Nota bene: Questa parte va fatta cmq, anche se l'entità è già stata stabilizzata.
	{
		RealtimeStatsScopeTimer RSSX2(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::ensure::bbb"));

		EntityID parentID;

		if(recursiveGuilties.find(primaryID.toUTF16()) != recursiveGuilties.end())
		{
			parentID = ObjectsSystem::instance()->getRecursiveID();
		}
		else if(currentStabilityDate.isNull() == false)
		{
			parentID = currentParent;
		}
		else if(currentEntry == nullptr)
		{
			parentID = ObjectsSystem::instance()->getSkippedID();
		}
		else
		{
			parentID = currentEntry->parent;
		}

		if(primaryID != ObjectsSystem::instance()->getRootID())
		{
#ifdef OS_TODOCIP
			if( (currentEntry != nullptr) && (currentEntry->parent != currentEntry->getEntityParent()) )
				recursiveGuilties[primaryID.toUTF16()] = true;
#else
			if( (currentEntry != nullptr) && (currentEntry->parent != primaryObj->parent) )
				recursiveGuilties[primaryID.toUTF16()] = true;
#endif

			ensure(database, parentID, recursiveGuilties, ensureHit);

			// L'ensure potrebbe aver stabilizzato questa stessa entità.
			// In questo caso, la seconda fase non dev'essere fatta.
			if(ensureHitted(ensureHit, primaryID) == 2)
			{
				return;
			}
			else
			{
				shared_ptr<EntitiesEntity> parentEntity = getEntity(database, parentID);
				if(parentEntity == nullptr)
				{
					parentID = ObjectsSystem::instance()->getOrphanID();
					// Virtual ensurata a mano.
					ensure(database, parentID, recursiveGuilties, ensureHit);
				}
				else
				{
					DateTime parentStabilityDate = parentEntity->getStabilityDate();
					OS_ASSERT(parentStabilityDate.isValid());
					if(!parentStabilityDate.isValid())
						return;

					if( (currentStabilityDate.isValid()) && (parentStabilityDate>currentStabilityDate) )
						needSecondPhase = true;
				}
			}
		}

		if(parentID != currentParent)
			needSecondPhase = true;

		RSSX2.stop();

		if(needSecondPhase)
		{
			RealtimeStatsScopeTimer RSS(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::ensure::SecondPhase"));

			uint32 depth = 0;
			EntityID sectionID;
			bool visible = true;

			if(ObjectsSystem::instance()->isVirtual(primaryID))
			{
				depth = 0;
				sectionID = primaryID;
				visible = true;
			}
			else
			{
				shared_ptr<EntitiesEntity> parentEntity = getEntity(database, parentID);

				OS_ASSERT(parentEntity != nullptr);
				if(parentEntity == nullptr)
					return;

				depth = parentEntity->getDepth()+1;


				if( (currentEntry != nullptr) && (objectType == portalObjectTypeSection) )
					sectionID = primaryID;
				else
					sectionID = parentEntity->getSectionID();

				if(currentEntry == nullptr)
				{
					visible = false;
				}
				else
				{
					visible = parentEntity->getVisible();
					if(currentEntry->visible == false)
						visible = false;
				}
			}

			currentStabilityDate = DateTime::now();

			OS_TRACE(String::format(_S("SnapshotManager:: Stability(2): %S\n").c_str(),primaryID.toUTF16().c_str()).to_ascii());

			// 0.12
			// Qui segno che dovrò ristabilizzare i figli diretti (e poi stabilizzandoli di conseguenza i sotto-figli a loro volta).
			// Questa è "pesante" se ad esempio stabilizzo una sezione con milioni di topic.
			// D'altro canto, sarebbe lento identificarli poi se non lo faccio qui.
			// NB: Faccio prima questa, poi l'aggiornamento del record dell'oggetto corrente. Perchè sennò stabilizzando la root,
			// questa la reinvaliderebbe, essendo la root un caso speciale (padre di se stessa). Evito di estendere la where.
			invalidateChilds(database, primaryID);

			String sql=String::format(_S("update os_snapshot_objects set parent='%S', section='%S', visible=%d, depth=%d, stability_date='%S', stats=0 where entity='%S'").c_str(),
				parentID.toUTF16().c_str(),
				sectionID.toUTF16().c_str(),
				visible ? 1:0,
				depth,
				currentStabilityDate.toString().c_str(),
				primaryID.toUTF16().c_str());
			database->execute(sql);

			// Lo rimuovo dalla cache
			getEntities()->remove(primaryID);

			// 0.11 Rimuovo anche il padre dalla cache
			//profile->getEntities()->remove(parentID);

			// 0.12 Se il padre è in cache, svuoto la cache dei figli
			shared_ptr<EntitiesEntity> parentEntityCache = getEntities()->find(parentID);
			if(parentEntityCache != nullptr)
				parentEntityCache->clearChildsCache();

			ensureHit[primaryID.toUTF16()] = 2; // Segno che sto questa entità è stata stabilizzata completamente.
		}
	}
}

void EntitiesSnapshotManager::ensureChilds(const shared_ptr<IPortalDatabase> &database, const EntityID &id)
{
	// Abolita nella 0.5, forse potrebbe essere un'opzione.
	// Se saltata (come ora) e guardo un topic, vedo solo i post stabilizzati. Quelli ok ma non ancora stabilizzati non li vedo.
	// Se non saltata, quelli ok ma non ancora stabilizzati verrebbero stabilizzati e quindi comparirebbero.
	// Per ora abolita perch il vantaggio non giustifica il rallentamento.
	if(!m_enableEnsureLoadingChilds) return;

	OS_LOCK(m_stabilityCS);

	ensure(database, id);

	DataTable result;
	String sql = String::format(_S("select entity from os_snapshot_objects where parent='%S'").c_str(),id.toUTF16().c_str());
	database->execute(sql,result);

	for(uint32 r=0;r<result.rows();r++)
	{
		String child_id = result.get(r,_S("entity"));

		ensure(database, child_id.to_ascii());
	}
}


ReputationsScore EntitiesSnapshotManager::computeUserStability(const shared_ptr<IPortalDatabase> &database, const ObjectID &id)
{
	// TOOPTIMIZE: Qui potrei prima cercarlo nella cache, come con le entità.
	// URGENT: Qui è tutto da pulire: "profiles" per la cache utenti è incoerente (dovrebbe essere sotto "entities", non cachizza le stats, è mal-strutturato.

	DataTable result;
	String sql = String::format(_S("select score,description,follow,stability_date from os_snapshot_users where id='%S'").c_str(),id.toUTF16().c_str());
	database->execute(sql,result);

	if (!result.hasRow(0))
	{
		// Manca, potrebbe essere che la "prepare" non  ancora terminata.

		// Non dovrebbero mai accadere degli "ensure" su oggetti senza che la fase di preparazione sia terminata
		// (se m_enableEnsureLoadingEntity == false && m_enableEnsureLoadingChilds == false

		// L'unico caso che capita ora  quando computa l'utente attuale, dalla allowChangeThresholds, per
		// sapere se necessita di uno snapshot_profile ad hoc.
		return ReputationsScore();
	}

	DateTime stabilityDate = static_cast<String>(result.get(0,_S("stability_date")));
	DateTime profileStabilityDate = getStabilityDate(database);

	if ( (stabilityDate.isValid() == false) || (stabilityDate<profileStabilityDate) )
	{
		ReputationsScore scoreDetail = computeReputation(database, id.toUTF16());

		bool visible = compareReputation(getPortal()->getOptionsShared()->getAuthorsReputationThreshold(),scoreDetail.getSafeValue());
		
		sql = String::format(_S("update os_snapshot_users set visible=%d, score=%f, description=%S, follow=%d, stability_date='%S', stats=0 where id='%S'").c_str(),visible ? 1:0,scoreDetail.getRoundedValue(),Convert::toSQL(scoreDetail.getDescription()).c_str(),scoreDetail.getFollow() ? 1:0,profileStabilityDate.toString().c_str(),id.toUTF16().c_str());
		database->execute(sql);

		// Lo rimuovo dalla cache
		getProfiles()->remove(id);

		return scoreDetail;
	}
	else
	{
		return ReputationsScore(result.get(0,_S("score")),result.get(0,_S("description")),result.get(0,_S("follow")));
	}
}


void EntitiesSnapshotManager::computeStatistics(const shared_ptr<IPortalDatabase> &database, const ObjectOrEntityID &id)
{
#ifdef OS_TODOCIP
	shared_ptr<ObjectsIObject> object = nullptr;
	
	shared_ptr<EntitiesEntity> entity = getEntity(database, EntityID(id.to_ascii()));	
	if(entity != nullptr)
		object = entity->getCurrent();
	else
		object = getPortal()->getObject(database, ObjectID(id.to_ascii()));
	
#else
	shared_ptr<ObjectsIObject> object = getPortal()->getObject(database, id.toUTF16());
	OS_ASSERT(object != nullptr);
	if( (object != nullptr) && (IPortalDatabase::isRevisionable(object->getObjectType())) )
	{
		shared_ptr<EntitiesEntity> entity = getEntity(database, id.toEntityID());
		OS_ASSERT(entity != nullptr);
		if(entity != nullptr)
			object = entity->getCurrent();
		else
			object = nullptr;
	}
#endif

	if(object != nullptr)
		object->getDescriptor()->computeStatistics(database, object);
}

void EntitiesSnapshotManager::updateObjectStatistics(const shared_ptr<IPortalDatabase> &database, const EntityID &id)
{
	RealtimeStatsManager::signal(_S("Objects"), _S("Stability - Statistics Objects"), 1, RealtimeStatsCounter::ctRelative, false, _S("objects")); // URGENT

	RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("SnapshotManager::updateObjectStatistics"));
	computeStatistics(database, id.toUTF16());

	String sql = String::format(_S("update os_snapshot_objects set stats=1, search=0 where entity='%S'").c_str(),
		id.toUTF16().c_str());
	OS_TRACE(String::format(_S("SnapshotManager:: Statistics: %S\n").c_str(),id.toUTF16().c_str()).to_ascii());
	database->execute(sql);

	// Rimuovo l'oggetto e le statistiche dalla cache, in modo che venga riletta la nuova versione
	// N.B. Commentato, dato che le statistiche non sono lette dalle entit.
	// profile->invalidateEntity(id, false);
}

/*
void SnapshotManager::updateObjectSearch(const shared_ptr<IPortalDatabase> &database, snapshot_profile_ptr &profile,const ObjectID &id)
{
	RealtimeStatsScopeTimer RSUSO0(_S("Debug"), _S("Debug: uso0")); // 29%
	entity_ptr entity = profile->getEntity(database, id);

	OS_ASSERT(entity != NULL);
	shared_ptr<ObjectsIRevisionable> current = entity->getCurrent();
	bool visible = ( (entity->getVisible()) && (current != nullptr) );
	RSUSO0.stop();

	// URGENT, da togliere e usare current->insert_date.
	// Calcola la insert_date
	RealtimeStatsScopeTimer RSUSO1(_S("Debug"), _S("Debug: uso1")); // 9%
	DateTime insertDate;
	if(current != nullptr)
	{
		// URGENT, da togliere e usare current->insert_date.
		// Calcola la insert_date
		DataTable result;
		String sql=String::format(_S("select insert_date from os_entries where id='%S'").c_str(), ObjectID(current->id).toString().c_str());
		database->execute(sql,result);
		if(result.rows()>=1)
			insertDate = result.get(0, _S("insert_date"));
	}
	RSUSO1.stop();

	if(profile->updateSearchObject(id, current, insertDate, entity->getScore(), visible, entity->getSectionID())) // 46%
	{
		RealtimeStatsScopeTimer RSUSO2(_S("Debug"), _S("Debug: uso2")); // 14%
		String sql = String::format(_S("update os_snapshot_objects set search=1 where reference='%S'").c_str(),id.toString().c_str());
		OS_TRACE(String::format(_S("SnapshotManager:: Search: %S (%S)\n").c_str(),id.toString().c_str(),id.getObjectTypeName().c_str()));
		database->execute(sql);
		RSUSO2.stop();
	}
}
*/

void EntitiesSnapshotManager::updateUserStatistics(const shared_ptr<IPortalDatabase> &database, const ObjectID &id)
{
	computeStatistics(database, id.toUTF16());

	String sql=String::format(_S("update os_snapshot_users set stats=1 where id='%S'").c_str(), id.toUTF16().c_str());
	database->execute(sql);

	// Rimuovo l'oggetto e le statistiche dalla cache, in modo che venga riletta la nuova versione
	getProfiles()->remove(id);
}

/* -----------------------------------------------------------
   Stability thread actions
----------------------------------------------------------- */

bool EntitiesSnapshotManager::canStabilityRun(bool withThis, shared_ptr<IJob> job)
{
	// Se "update()" è richiamato sincrono.
	if(job == nullptr)
		return true;

	// withThis serve xchè se è chiamata dopo che è stato preso il database dalla update(), quella non deve essere considerata...
	uint32 nt = withThis ? 1:0;
	if( (job->stopped()) || (getPortal()->getDatabaseExclusiveRequests()>nt) || (checkLastActionTick() == false) )
		return false;
	else
		return true;
}

bool EntitiesSnapshotManager::update(shared_ptr<IJob> job)
{
	// Restituisce la priorità di una ri-esecuzione.
	// 0: Completato.
	// 1: Da non variare (rimandato)
	// 2..x: Dalla pi alta alla + bassa.

	if(!canStabilityRun(false, job))
	{
		// Attesa per evitare innumerevoli inutili chiamate. Potrebbe essere stability.delta_start/5.
		if(job != nullptr)
			job->applyDelay(2000);
		return false;
	}
	else
	{
		if(job != nullptr)
			job->resetDelay();
	}
	
	RealtimeStatsScopeTimer RSS(_S("Activity"), String::format(_S("Stability running '%S'").c_str(), getPortal()->getPovName().c_str()));

	int32 newPriority = 1;

	if(getStabilityStatus() == ssDone)
	{
		newPriority = 0;
		if(job != nullptr)
			job->setPriority(taskPriorityIdle);
		return true;
	}
	else if(getStabilityStatus() == ssNone)
	{
		setStabilityStatus(ssAccept);
		if(job != nullptr)
			job->setPriority(taskPriorityCritical);
	}
	else
	{
		shared_ptr<PortalsTransaction> databaseWork = getPortal()->startTransaction(true);
		shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();
		database->beginTransaction();

		OS_LOCK(m_stabilityCS);

		if(getStabilityStatus() == ssAccept)
		{
			if(computeStabilityAccept(database, job))
			{
				setStabilityStatus(ssPrepare);
				if(job != nullptr)
					job->setPriority(taskPriorityHigh);
			}
		}
		else if(getStabilityStatus() == ssPrepare)
		{
			if(computeStabilityPrepare(database, job))
			{
				setStabilityStatus(ssStabilityUsers);
				if(job != nullptr)
					job->setPriority(taskPriorityHigh);
			}
		}
		else if(getStabilityStatus() == ssStabilityUsers)
		{
			if(computeStabilityUsers(database, job))
			{
				setStabilityStatus(ssStabilityObjects);
				if(job != nullptr)
					job->setPriority(taskPriorityAboveNormal);
			}
		}
		else if(getStabilityStatus() == ssStabilityObjects)
		{
			if(computeStabilityObjects(database, job))
			{
				setStabilityStatus(ssStatisticsObjects);
				if(job != nullptr)
					job->setPriority(taskPriorityNormal);
			}
		}
		else if(getStabilityStatus() == ssStatisticsObjects)
		{
			if(computeStatisticsObjects(database, job))
			{
				setStabilityStatus(ssStatisticsUsers);
				if(job != nullptr)
					job->setPriority(taskPriorityBelowNormal);
			}
		}
		else if(getStabilityStatus() == ssStatisticsUsers)
		{
			if(computeStatisticsUsers(database, job))
			{
				setStabilityStatus(ssSearchObjects);
				if(job != nullptr)
					job->setPriority(taskPriorityLow);
			}
		}
		else if(getStabilityStatus() == ssSearchObjects)
		{
			if(computeSearchObjects(database, job))
			{
				setStabilityStatus(ssFinalize);
				if(job != nullptr)
					job->setPriority(taskPriorityIdle);
			}
		}
		else if(getStabilityStatus() == ssFinalize)
		{
			if(computeFinalize(database, job))
			{
				setStabilityStatus(ssDone);
				if(job != nullptr)
					job->setPriority(taskPriorityIdle);
			}
		}

		database->commit();
	}

	return false;
}

bool EntitiesSnapshotManager::computeStabilityAccept(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job)
{
	RealtimeStatsScopeTimer RSS1(_S("Activity"), _S("Stability: Accept"));

	DataTable result;
	RealtimeStatsScopeTimer RSS2(_S("Debug"), _S("SnapshotManager::computeStabilityAccept"));
	String sql = String::format(_S("select id from os_entries where rank == -2 limit %d").c_str(),getObjectsStep());
	database->execute(sql,result);
	RSS2.stop();

	for(uint32 r=0;r<result.rows();r++)
	{
		ObjectID id = static_cast<String>(result.get(r,_S("id"))).to_ascii();

		shared_ptr<ObjectsIObject> object = getPortal()->getObject(database, id);
		OS_ASSERT(object!=nullptr);

		LanguageResult acceptedMessage = object->acceptable(database);
		if(acceptedMessage.empty())
		{
			sql = _S("update os_entries set rank=0, accept_msg=nullptr where id='") + id.toUTF16() + _S("'"); // Mark as accepted
			database->execute(sql);

			// createObjectRecord? e abolisco la Prepare?
		}
		else
		{
			sql = _S("update os_entries set rank=-1, accept_msg=") + Convert::toSQL(acceptedMessage.getId()) + _S(" where id='") + id.toUTF16() + _S("'"); // Mark for trash
			database->execute(sql);
			// deleteObjectRecord?
		}		

		if(!canStabilityRun(true, job))
			return false;
	}
	if(result.rows() != 0)
		return false;

	return true;
}

bool EntitiesSnapshotManager::computeStabilityPrepare(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job)
{
	return true; // TOCLEAN: "Prepare" da piallare.
	/*
	RealtimeStatsScopeTimer RSS1(_S("Activity"), _S("Stability: Prepare"));

	// TOOPTIMIZE: C' da valutare se abolire questo step, facendolo direttamente durante la creazione del profilo.
	// Certo dovrebbero aspettare, ma eviterei altri problemi (non sono stra-testati i giri in mancanza dei records di stabilit).

	bool skip = false;

	// Questo passaggio è solo un'ottimizzazione, in quanto la "select ... where not exists(..)" fa un full-scan di tutti gli oggetti.
	// Conto gli utenti come "entry" e gli utenti presenti nel punto di vista. Se il numero coincide, la "prepare" è già stata fatta.

	// Riflessioni:
	// Q:E se creo un portale anarchico e ci importo solo oggetti?
	// A:Gli oggetti avrebbero cmq gli utenti, altrimenti non sarebbero presenti perchè non avrebbero passato la validazione, giusto?
	if(skip == false)
	{
		String sql1 = _S("select count(*) from os_entries where type=1");
		uint32 nUsers1 = database->value_of(sql1);
		//String sql2 = String::format(_S("select count(*) from os_snapshot_users").c_str(),profile->getID_RRR().c_str());
		String sql2 = _S("select count(*) from os_snapshot_users");
		uint32 nUsers2 = database->value_of(sql2);

		if(nUsers1 == nUsers2)
			skip = true;
	}

	// Objects Cache
	if(skip == false)
	{
		DataTable result;
		// N.B. Non filtro i tipi di oggetto, dato che gli oggetti non revisionabili hanno 'revision'==nullptr.
		RealtimeStatsScopeTimer RSS2(_S("Debug"), _S("SnapshotManager::computeStabilityPrepare::objects enumeration"));
#ifdef OS_TODOCIP
		//String sql=String::format(_S("select distinct entity_id as id from os_entries where not exists(select reference from os_snapshot_objects where reference=entity_id) limit %d").c_str(),getObjectsStep());
		
		String sql=String::format(_S("select distinct entity_id as id from os_entries where entity_id is not nullptr and not exists(select reference from os_snapshot_objects where reference=entity_id)").c_str());
#else
		String sql=String::format(_S("select id from os_entries where revision='' and not exists(select reference from os_snapshot_objects where reference=id) limit %d").c_str(),getObjectsStep());
#endif
		database->execute(sql,result);
		RSS2.stop();

		for(uint32 r=0;r<result.rows();r++)
		{
			String id=result.get(r,_S("id"));

			shared_ptr<ObjectsIRevisionable> primary=objects_revisionable_cast(getPortal()->getObject(database, id));
			OS_ASSERT(primary!=nullptr);

			createObjectRecord(database, primary);

			if(!canStabilityRun(true, job))
				return false;
		}
		if(result.rows() != 0)
			return false;
	}

	if(!canStabilityRun(true, job))
		return false;

	// Users Cache, dal nuovo account verso gli altri
	if(skip == false)
	{
		DataTable result;
		RealtimeStatsScopeTimer RSS2(_S("Debug"), _S("SnapshotManager::computeStabilityPrepare::users enumeration"));
		String sql=String::format(_S("select id from os_users where not exists(select reference from os_snapshot_users where reference=id) limit %d").c_str(),getObjectsStep());
		database->execute(sql,result);
		RSS2.stop();

		for(uint32 r=0;r<result.rows();r++)
		{
			String user_id=result.get(r,_S("id"));

			shared_ptr<ObjectsUser> user=objects_user_cast(getPortal()->getObject(database, user_id));
			OS_ASSERT(user!=nullptr);

			OS_TRACE("Start SnapshotManager::computeStabilityPrepare - createUserRecord\n");
			createUserRecord(database, user);

			if(!canStabilityRun(true, job))
				return false;
		}
		if(result.rows() != 0)
			return false;
	}

	return true;
	*/
}

bool EntitiesSnapshotManager::computeStabilityUsers(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job)
{
	RealtimeStatsScopeTimer RSS1(_S("Activity"), _S("Stability: Users"));

	RealtimeStatsScopeTimer RSS2(_S("Debug"), _S("SnapshotManager::computeStabilityUsers::objects enumeration"));
	DataTable result;
	String sql=String::format(_S("select id from os_snapshot_users where stability_date<'%S' limit %d").c_str(), getStabilityDate(database).toString().c_str(),getObjectsStep());
	database->execute(sql,result);
	RSS2.stop();

	if(result.rows() == 0)
	{
		return true;
	}
	else
	{
		for(uint32 r=0;r<result.rows();r++)
		{
			String id=result.get(r,_S("id"));
			computeUserStability(database, id.to_ascii());

			if(!canStabilityRun(true, job))
				return false;
		}
		return false;
	}
}

bool EntitiesSnapshotManager::computeStabilityObjects(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job)
{
	RealtimeStatsScopeTimer RSS(_S("Activity"), _S("Stability: Objects"));

	// Il concetto  che prima stabilizzo cosa devo stabilizzare indipendentemente dal padre,
	// dato che potrebbe anche non esserci (nei casi degli orfani).
	// Se  tutto ok, allora cerco in base alla relazione down-up con i padri.
	// TOOPTIMIZE: Qua faccio due select distinte, potrebbero essere compattate in un'unica con una outer join sul padre.

	/* 0.11
	DateTime profileStabilityDate = profile->getStabilityDate(database);
	DataTable result;
	{
		String sql = _S("select reference from os_snapshot_objects where ");
		sql += _S("profile = '%S' and ");
		sql += _S("stability_date < '%S'");
		sql += _S("limit %d");

		RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::objects enumeration 1"));
		sql = String::format(sql.c_str(), profile->getID().c_str(), profileStabilityDate.toString().c_str(), getObjectsStep());
		database->execute(sql,result);
		RSS1.stop();
	}
	if(result.rows() == 0)
	{
		// Cambiata nella 0.11
		String sql = _S("select sc.reference from os_snapshot_objects sc, os_entries sce, os_snapshot_objects sp where ");
		sql += _S("sc.profile = '%S' and sc.profile = sp.profile and sce.id = sc.current and sp.reference = sce.parent and ");
		sql += _S("(sp.stability_date>sc.stability_date or sc.stability_date < '%S') order by sc.depth ");
		sql += _S("limit %d");
		sql=String::format(sql.c_str(),profile->getID().c_str(), profileStabilityDate.toString().c_str(), getObjectsStep());
		RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::objects enumeration 2"));
		database->execute(sql,result);
		RSS1.stop();
	}
	*/

	uint32 recordsForStep = getObjectsStep();

	// Questo "for" esiste perchè la "ensure" potrebbe a sua volta invalidare altri oggetti.
	// Così cerco cmq di stabilizzare x (getObjectsStep()) oggetti prima del commit.
	for(;;)
	{
		// 0.12
		DateTime profileStabilityDate = getStabilityDate(database);
		DataTable result;
		{
			String sql = _S("select entity from os_snapshot_objects where ");
			sql += _S("stability_date = '%S' ");
			sql += _S("order by depth ");
			sql += _S("limit %d ");

			RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("SnapshotManager::computeStabilityObjects::objects enumeration"));
			sql = String::format(sql.c_str(), DateTime::EMPTY.toString().c_str(), recordsForStep);
			database->execute(sql,result);
			//OS_LOG_WARNING(_S("computeStabilityObjects: ") + sql); // TEMP
			RSS1.stop();
		}

		if(result.rows() == 0)
		{
			OS_TRACE("SnapshotManager:: Stabilities step completed.\n");
			return true;
		}
		else
		{
			uint32 nRows = result.rows();
			OS_TRACE("SnapshotManager:: Stabilities step...\n");
			for(uint32 r=0; r<nRows; r++)
			{
				String entity=result.get(r,_S("entity"));

				// Hack:
				// Nella 0.13, stabilizza all'infinito (SOLO un anarchico).
				// Per riprodurlo:
				// - creo un portale anarchico da zero
				// - creo un utente nuovo, lo stabilizza
				// - creo un'altro utente nuovo, stabilizza all'infinito.
				// Al riavvio di Osiris, stabilizza anche quello nuovo.
				// Accade perchè in cache c'è una "Entity" di root che ha stability_date valorizzato, quando nel db è ancora a 000000000.

				// Ora, dato che il giro dei profili multipli andrà abolito con la 0.14, ho messo questo hack.
				getEntities()->remove(EntityID(entity.to_ascii()));

				ensure(database, entity.to_ascii());

				if(!canStabilityRun(true, job))
					return false;
			}
			recordsForStep -= nRows;
			if(recordsForStep<=0)
				return false;
		}
	}
	return false;
}

bool EntitiesSnapshotManager::computeStatisticsObjects(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job)
{
	RealtimeStatsScopeTimer RSS(_S("Activity"), _S("Stability: Objects Statistics"));

	// Qua filtro e 'statisticizzo' solo i tipi di oggetto che supportano stats,
	// evitando post, istanze etc..
	String types;
	const ObjectsDescriptors &descriptors = ObjectsSystem::instance()->getDescriptors();
	for(ObjectsDescriptors::const_iterator i = descriptors.begin(); i != descriptors.end(); ++i)
	{
		PortalObjectType type = i->first;
		shared_ptr<ObjectsIDescriptor> desc = i->second;

		if(desc->hasStatistics())
		{
			if(!types.empty()) types += _S(",");
			types += Convert::toString(type);
		}
	}

	// Cerco le entit che hanno almeno un figlio la cui data di statistica  maggiore di lei,
	// o che hanno una data di statistica precedente alla data di stabilizzazione.
	// Versione vecchia, <0.12, prima di spezzarla in 2 query distinte.
	/*
	sql += _S(" select sc.reference, sc.stats_date  ");
	sql += _S(" from  ");
	sql += _S(" os_snapshot_objects sc  ");
	sql += _S(" where  ");
	sql += _S(" sc.profile = '%S' and ");
	sql += _S(" sc.type in (%S) and ");
	sql += _S(" ( (sc.stats_date < sc.stability_date) or ");
	sql += _S("   ( ");
	sql += _S("     exists  ");
	sql += _S("     (select * from os_snapshot_objects sh  ");
	sql += _S("      where sh.profile = sc.profile and  ");
	sql += _S("      sh.parent = sc.reference and ");
	sql += _S("      sh.type in (%S) and ");
	sql += _S("      sh.stats_date > sc.stats_date ");
	sql += _S("     ) ");
	sql += _S("   ) ");
	sql += _S(" ) ");
	sql += _S(" order by sc.depth desc ");
	sql += _S(" limit %d ");
	*/

	// N.B. potrei estrarre tre record con depth 5,2,1, ma in realt la statisticizzazione del depth=5 ha reso disponibile
	// un'altro oggetto con depth=4, quindi ci vorrebbe un controllo per evitare che il depth=2 sia statisticizzato prima.
	// Un controllo tipo una 'break' se depth!=primoDepth, o estraggo prima una max(depth) e poi uso una where.. da valutare cosa  meglio..
	// Non c' perch dovrei valutare se ne vale la pena, in ogni caso attualmente depth=2 viene prima di depth=4, ma
	// quando depth=4  calcolato, depth=2 sar nuovamente ri-calcolato.. in teoria, da verificare

	// 0.11
	/*
	DataTable result;
	{
		String sql = _S("");

		sql += _S("select sc.reference, sc.stats_date ");
		sql += _S("from ");
		sql += _S("os_snapshot_objects sc   ");
		sql += _S("where   ");
		sql += _S("sc.profile = '%S' and  ");
		sql += _S("sc.type in (%S) and  ");
		sql += _S("sc.stats_date < sc.stability_date ");
		sql += _S("order by sc.depth desc ");
		sql += _S("limit %d ");

		RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("SnapshotManager::computeStatisticsObjects::objects enumeration 1"));
		sql=String::format(sql.c_str(),profile->getID().c_str(), types.c_str(), getObjectsStep());
		database->execute(sql,result);
		RSS1.stop();
	}
	if(result.rows() == 0)
	{
		String sql = _S("");

		sql += _S("select sc.reference, sc.stats_date ");
		sql += _S("from ");
		sql += _S("os_snapshot_objects sc, ");
		sql += _S("os_snapshot_objects sh ");
		sql += _S("where   ");
		sql += _S("sc.profile = '%S' and  ");
		sql += _S("sc.type in (%S) and  ");
		sql += _S("sh.profile = sc.profile and ");
		sql += _S("sh.parent = sc.reference and ");
		sql += _S("sh.type in (%S) and ");
		sql += _S("sh.stats_date > sc.stats_date ");
		sql += _S("order by sc.depth desc ");
		sql += _S("limit %d ");

		RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("SnapshotManager::computeStatisticsObjects::objects enumeration 2"));
		sql=String::format(sql.c_str(),profile->getID().c_str(), types.c_str(), types.c_str(), getObjectsStep());
		database->execute(sql,result);
		RSS1.stop();
	}
	*/

	uint32 recordsForStep = getObjectsStep();

	for(;;)
	{
		// 0.12
		DateTime profileStabilityDate = getStabilityDate(database);
		DataTable result;
		{
			String sql = _S("select entity from os_snapshot_objects ");
			//sql += _S("indexed by os_snapshot_objects_index_profile_stats_depth "); // TOFIX_SQLITE
			sql += _S("where "); // TOFIX_SQLITE
			sql += _S("stats = 0 ");
			sql += _S("order by depth desc ");
			sql += _S("limit %d ");

			RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("SnapshotManager::computeStatisticsObjects::objects enumeration"));
			sql = String::format(sql.c_str(), getObjectsStep());
			database->execute(sql,result);
			//OS_LOG_WARNING(_S("computeStatisticsObjects: ") + sql); // TEMP
			RSS1.stop();
		}

		if(result.rows() == 0)
		{
			OS_TRACE("SnapshotManager:: Statistics step completed.\n");
			return true;
		}
		else
		{
			OS_TRACE("SnapshotManager:: Statistics step...\n");
			uint32 nRows = result.rows();
			for(uint32 r=0;r<nRows;r++)
			{
				String entity=result.get(r,_S("entity"));
				updateObjectStatistics(database, entity.to_ascii());

				if(!canStabilityRun(true, job))
					return false;
			}
			recordsForStep -= nRows;
			if(recordsForStep<=0)
				return false;
		}
	}
	return false;
}

bool EntitiesSnapshotManager::computeStatisticsUsers(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job)
{
	RealtimeStatsScopeTimer RSS(_S("Activity"), _S("Stability: Users Statistics"));

	RealtimeStatsScopeTimer RSS2(_S("Debug"), _S("SnapshotManager::computeStatisticsUsers::objects enumeration"));
	DataTable result;

	String sql = _S("select id from os_snapshot_users ");
	//sql += _S("indexed by os_snapshot_users_index_profile_stats "); // TOFIX_SQLITE
	sql += _S("where stats = 0 limit %d");

	sql=String::format(sql.c_str(),getObjectsStep());

	database->execute(sql,result);
	RSS2.stop();

	if(result.rows() == 0)
	{
		return true;
	}
	else
	{
		for(uint32 r=0;r<result.rows();r++)
		{
			String id=result.get(r,_S("id"));
			updateUserStatistics(database, id.to_ascii());

			if(!canStabilityRun(true, job))
				return false;
		}
		return false;
	}
}

bool EntitiesSnapshotManager::computeSearchObjects(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job)
{
	RealtimeStatsScopeTimer RSS(_S("Activity"), _S("Stability: Search indexes"));

	uint32 searchLevel = getSearchLevel();

	// Indici di ricerca disattivati
	if(searchLevel == 0)
		return true;

	DataTable result;
	String sql = _S("");

	sql += _S("select sc.entity, sc.document ");
	sql += _S("from  ");
	sql += _S("os_snapshot_objects sc ");
	//sql += _S("indexed by os_snapshot_objects_index_profile_search "); // TOFIX_SQLITE
	sql += _S("where ");
	sql += _S("sc.search == 0 ");
	
	sql += _S("limit %d ");	

	RealtimeStatsScopeTimer RSS1(_S("Debug"), _S("SnapshotManager::computeSearchObjects::objects enumeration"));
	sql=String::format(sql.c_str(),
		getObjectsStep());
	database->execute(sql,result);
	RSS1.stop();

	if(result.rows() == 0)
	{
		OS_TRACE("SnapshotManager:: Search step completed.\n");
		return true;
	}
	else
	{
		OS_TRACE("SnapshotManager:: Search step...\n");
		for(uint32 r=0;r<result.rows();r++)
		{
			String entity=result.get(r,_S("entity"));
			String document=result.get(r,_S("document"));
			updateObjectSearch(database, entity.to_ascii(), document, searchLevel);
			if(!canStabilityRun(true, job))
				return false;
		}

		closeSearchIndexWriter();

		return false;
	}
}

bool EntitiesSnapshotManager::computeFinalize(const shared_ptr<IPortalDatabase> &database, shared_ptr<IJob> job)
{
	// Physical removing of objects	
	if(getPortal()->getOptionsShared()->getObjectsPhysicalRemove())
	{
		int32 objectsPhysicalRemoveDays = getPortal()->getOptionsShared()->getObjectsPhysicalRemoveDays();
		if(objectsPhysicalRemoveDays >= 0)
		{
			DateTime dateReference = DateTime::now() - TimeDuration::days(objectsPhysicalRemoveDays);
		
			DataTable result;
			String sql = String::format(_S("select id from os_entries where submit_date<='%S' and rank = -1").c_str(), dateReference.toString().c_str());
			database->execute(sql,result);
		
			if(result.rows() == 0)
			{
				OS_TRACE("SnapshotManager:: Finalize step completed.\n");
				return true;
			}
			else
			{
				OS_TRACE("SnapshotManager:: Finalize step...\n");
				for(uint32 r=0;r<result.rows();r++)
				{
					ObjectID id = static_cast<String>(result.get(r,_S("id"))).to_ascii();

					shared_ptr<ObjectsIObject> object = database->getPortal()->getObject(database, id);
					OS_ASSERT(object != nullptr);

					//database->removeRecord(object); // CLODOURGENT tolto per sicurezza, decommentare semplicemente dopo test esaustivi.

					if(!canStabilityRun(true, job))
						return false;
				}

				return false;
			}
		}
	}
	
	return true;
}

/* -----------------------------------------------------------
   Record management
----------------------------------------------------------- */

void EntitiesSnapshotManager::createObjectRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIRevisionable> &object)
{
	String sql = String::format(_S("select count(*) from os_snapshot_objects where entity='%S'").c_str(), object->getEntityID().toUTF16().c_str());
	bool exists = (int(database->getConnection()->value_of(sql)) != 0);		
	
	if(exists == false)
	{
		bool visible = false; //object->visible;
		double position = 0; //object->position;
		//double score = 0;
		uint32 depth = 0;

		// Nota: tolto lo score, vedi TOCLEAN_SNAPSHOT_SCORE
		/*
		String sql=String::format(_S("insert into os_snapshot_objects (reference, type, submit_date, parent, section, current, visible, position, score, depth, insert_date, stability_date, stats, search, document) values ('%S',%d,'%S','%S','%S','%S',%d,%f,%f,%d,'%S','%S',0,0,'')").c_str(),
			object->id->toUTF16().c_str(),
			object->getObjectType(),
			object->submit_date->toString().c_str(),
			object->getParent().toUTF16().c_str(),
			//ObjectsSystem::instance()->getSkippedID().toString().c_str(),
			String::EMPTY.c_str(),
			String::EMPTY.c_str(),
			visible,
			position,
			score,
			depth,
			DateTime::EMPTY.toString().c_str(),
			DateTime::EMPTY.toString().c_str()
			);
		*/

	

		String sql=String::format(_S("insert into os_snapshot_objects \
									 (entity, type, submit_date, parent, section, current, visible, position, depth, insert_date, stability_date, stats, search, document) \
									 values \
									 ('%S',%d,'%S','%S','%S','%S',%d,%f,%d,'%S','%S',0,0,'')").c_str(),
			object->getEntityID().toUTF16().c_str(),
			object->getObjectType(),
			object->getEntityDate().toString().c_str(),
			object->getParent().toUTF16().c_str(),
			//ObjectsSystem::instance()->getSkippedID().toString().c_str(),
			String::EMPTY.c_str(),
			String::EMPTY.c_str(),
			visible,
			position,
			depth,
			DateTime::EMPTY.toString().c_str(),
			DateTime::EMPTY.toString().c_str()
			);
		database->execute(sql);

		createStatisticRecord(database, object);
	}
}

void EntitiesSnapshotManager::removeObjectRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIRevisionable> &object)
{
	String sql = String::format(_S("select count(*) from os_entries where entity='%S'").c_str(), object->getEntityID().toUTF16().c_str());
	int nEntries = int(database->getConnection()->value_of(sql));
	if(nEntries <= 1)
	{
		String sql = String::format(_S("delete from os_snapshot_objects where entity='%S'").c_str(), object->getEntityID().toUTF16().c_str());
		database->execute(sql);

		removeStatisticRecord(database, object);
	}
}

void EntitiesSnapshotManager::createUserRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsUser> &user)
{
	OS_TRACE(String::format(_S("SnapshotManager::createUserRecord '%S'").c_str(),user->id->toUTF16().c_str()).to_ascii());
	String sql=String::format(_S("insert into os_snapshot_users (id,visible,score,description,follow,stability_date,stats) values ('%S',1,0,'',0,'%S',0)").c_str(), 
		user->id->toUTF16().c_str(),DateTime::EMPTY.toString().c_str());
	database->execute(sql);

	createStatisticRecord(database, user);
}

void EntitiesSnapshotManager::removeUserRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsUser> &user)
{
	String sql=String::format(_S("delete from os_snapshot_users where id='%S'").c_str(),user->id->toUTF16().c_str());
	database->execute(sql);

	removeStatisticRecord(database, user);
}

void EntitiesSnapshotManager::createStatisticRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIObject> &object)
{
	// Se  una sezione, i suoi record statistici verranno creati durante la stabilizzazione.
	if(object->getObjectType() != portalObjectTypeSection)
		object->getDescriptor()->createStatistics(database, object);
}

void EntitiesSnapshotManager::removeStatisticRecord(const shared_ptr<IPortalDatabase> &database, const shared_ptr<ObjectsIObject> &object)
{
	object->getDescriptor()->removeStatistics(database, object);
}

/* -----------------------------------------------------------
   Cambio stabilit oggetti virtuali
----------------------------------------------------------- */

void EntitiesSnapshotManager::invalidateAll(const shared_ptr<IPortalDatabase> &database)
{
	OS_LOCK(m_stabilityCS);

	m_entities->clear();
	m_profiles->clear();
	m_statistics->clear();

	updateStabilityDate(database);
}

void EntitiesSnapshotManager::invalidateTotal(const shared_ptr<IPortalDatabase> &database)
{
	invalidateAll(database);

	database->execute(_S("update os_entries set rank=-2"));

	resetStabilityStatus(ssNone);

	NotificationsManager::instance()->notify(_S("Re-stabilization of '") + database->getPortal()->getPovName() + _S("' launched."));
}

/*
void EntitiesSnapshotManager::invalidateObject(const shared_ptr<IPortalDatabase> &database, EntityID primaryID, bool invalidateStability, bool invalidateStatistics, bool invalidateParentStability, bool invalidateParentStatistics)
{
	shared_ptr<ObjectsIRevisionable> revisionable = objects_revisionable_cast(getPortal()->getObject(database, primaryID.toUTF16()));
	invalidateObject(database, revisionable, invalidateStability, invalidateStatistics, invalidateParentStability, invalidateParentStatistics);
}

void EntitiesSnapshotManager::invalidateObject(const shared_ptr<IPortalDatabase> &database, shared_ptr<ObjectsIRevisionable> object, bool invalidateStability, bool invalidateStatistics, bool invalidateParentStability, bool invalidateParentStatistics)
{
	if( (invalidateStability == false) && (invalidateStatistics == false) )
		return;

	if(object == nullptr)
		return;

	EntityID entityID = object->getEntityID();
	
	if(invalidateStability)
		invalidateStatistics = true;
	if(invalidateStatistics)
		invalidateParentStatistics = true;
	if(invalidateParentStability)
		invalidateParentStatistics = true;

	if(ObjectsSystem::instance()->isVirtual(entityID))
	{
		invalidateParentStability = false;
		invalidateParentStatistics = false;
	}

	// Nota bene: L'invalidazione  importante perch se un'entit non  nella cache, potrebbe essere da stabilizzare.

	// Estraggo la versone attuale nella cache.
	shared_ptr<EntitiesEntity> entityInCache = getEntities()->find(entityID);

	// resetObjectStability(database, object->getEntityID(), invalidateStability, invalidateStatistics);
		

	if(invalidateStability)
	{
		// Vedi commento nella SnapshotManager::ensure,
		// il concetto  che qui per essere ottimizzato dovrei togliere dalla cache
		// il primaryID e tutti i figli in ricorsivo.
		// Per ora svuoto direttamente tutta la cache.

		//m_entities->remove(primaryID);
		m_entities->clear();
	}

	// TOOPTIMIZE, potrei fare un'unica sql.
	if(ObjectsSystem::instance()->isVirtual(entityID) == false)
		if(invalidateStability)
		{
			String sql=String::format(_S("update os_snapshot_objects set stability_date='%S' where entity='%S' and stability_date!='%S'").c_str(),
				DateTime::EMPTY.toString().c_str(),
				entityID.toUTF16().c_str(),
				DateTime::EMPTY.toString().c_str());
			database->execute(sql);
		}

	if(invalidateStatistics)
	{
		String sql=String::format(_S("update os_snapshot_objects set stats=0 where entity='%S' and stats!=0").c_str(),
			entityID.toUTF16().c_str());
		database->execute(sql);
	}
	


	if( (invalidateParentStability) || (invalidateParentStatistics) )
	{
		// Rimuovo l'entit padre dalla cache.
		//resetObjectStability(database, object->getParent(), invalidateParentStability, invalidateParentStatistics);
		// Questo invalida le statistiche fino alla root.
		invalidateObject(database, object->getParent(), invalidateParentStability, invalidateParentStatistics, false, true);

		// Determino se il padre  cambiato (revisione di move), se s invalido anche il padre vecchio.
		if( (entityInCache != nullptr) &&
			(entityInCache->getCurrent() != nullptr) &&
			(entityInCache->getCurrent()->getParent() != object->getParent()) )
		{
			//resetObjectStability(database, entityInCache->getCurrent()->getParent(), invalidateParentStability, invalidateParentStatistics);
			invalidateObject(database, entityInCache->getCurrent()->getParent(), invalidateParentStability, invalidateParentStatistics, false, true);
		}
	}
	
	resetStabilityStatus(ssStabilityObjects);	
}
*/

void EntitiesSnapshotManager::invalidateEntity(const shared_ptr<IPortalDatabase> &database, EntityID entityID, bool invalidateStability, bool invalidateStatistics)
{
	if( (invalidateStability == false) && (invalidateStatistics == false) )
		return;

	bool invalidateParentStability = false;
	bool invalidateParentStatistics = false;
		
	if(invalidateStability)
		invalidateStatistics = true;
	if(invalidateStatistics)
		invalidateParentStatistics = true;
	if(invalidateParentStability)
		invalidateParentStatistics = true;

	if(ObjectsSystem::instance()->isVirtual(entityID))
	{
		invalidateParentStability = false;
		invalidateParentStatistics = false;
	}

	// Nota bene: L'invalidazione  importante perch se un'entit non  nella cache, potrebbe essere da stabilizzare.

	// Estraggo la versone attuale nella cache.
	shared_ptr<EntitiesEntity> entityInCache = getEntities()->find(entityID);

	// resetObjectStability(database, object->getEntityID(), invalidateStability, invalidateStatistics);
		

	if(invalidateStability)
	{
		// Vedi commento nella SnapshotManager::ensure,
		// il concetto  che qui per essere ottimizzato dovrei togliere dalla cache
		// il primaryID e tutti i figli in ricorsivo.
		// Per ora svuoto direttamente tutta la cache.

		//m_entities->remove(primaryID);
		m_entities->clear();
	}

	// TOOPTIMIZE, potrei fare un'unica sql.
	if(ObjectsSystem::instance()->isVirtual(entityID) == false)
		if(invalidateStability)
		{
			String sql=String::format(_S("update os_snapshot_objects set stability_date='%S' where entity='%S' and stability_date!='%S'").c_str(),
				DateTime::EMPTY.toString().c_str(),
				entityID.toUTF16().c_str(),
				DateTime::EMPTY.toString().c_str());
			database->execute(sql);
		}

	if(invalidateStatistics)
	{
		String sql=String::format(_S("update os_snapshot_objects set stats=0 where entity='%S' and stats!=0").c_str(),
			entityID.toUTF16().c_str());
		database->execute(sql);
	}
	


	if( (invalidateParentStability) || (invalidateParentStatistics) )
	{
		DataTable result;
		String sql = String::format(_S("select parent from os_snapshot_objects where entity='%S'").c_str(), entityID.toUTF16().c_str());
		database->execute(sql,result);
		if(result.rows() == 1)
		{
			EntityID parent  = static_cast<String>(result.get(0, _S("parent"))).to_ascii();

			// Questo invalida le statistiche fino alla root.
			invalidateEntity(database, parent, invalidateParentStability, invalidateParentStatistics);

			// Determino se il padre  cambiato (revisione di move), se s invalido anche il padre vecchio.
			if( (entityInCache != nullptr) &&
				(entityInCache->getCurrent() != nullptr) &&
				(entityInCache->getCurrent()->getParent() != parent) )
			{
				invalidateEntity(database, entityInCache->getCurrent()->getParent(), invalidateParentStability, invalidateParentStatistics);
			}
		}
	}
	
	resetStabilityStatus(ssStabilityObjects);	
}

void EntitiesSnapshotManager::invalidateUser(const shared_ptr<IPortalDatabase> &database, const ObjectID& userID, bool invalidateStability, bool invalidateStatistics)
{
	if( (invalidateStability == false) && (invalidateStatistics == false) )
		return;

	m_profiles->remove(userID);
	
	if(invalidateStability)
	{
		String sql=String::format(_S("update os_snapshot_users set stability_date='%S' where id='%S' and stability_date!='%S'").c_str(), DateTime::EMPTY.toString().c_str(),userID.toUTF16().c_str(),DateTime::EMPTY.toString().c_str());
		database->execute(sql);
	}
	
	if(invalidateStatistics)
	{
		String sql=String::format(_S("update os_snapshot_users set stats=0 where id='%S' and stats!=0").c_str(), userID.toUTF16().c_str());
		database->execute(sql);
	}	
}

/*
void EntitiesSnapshotManager::invalidateUser(const shared_ptr<IPortalDatabase> &database, shared_ptr<ObjectsUser> user, bool invalidateStability, bool invalidateStatistics)
{
	invalidateUser(database, user->id,invalidateStability,invalidateStatistics);
}
*/


void EntitiesSnapshotManager::invalidateChilds(const shared_ptr<IPortalDatabase> &database, EntityID primaryID)
{
	String sql=String::format(_S("update os_snapshot_objects set stability_date='%S' where parent='%S' and stability_date!='%S'").c_str(),
		DateTime::EMPTY.toString().c_str(),
		primaryID.toUTF16().c_str(),
		DateTime::EMPTY.toString().c_str());
	database->execute(sql);
}

void EntitiesSnapshotManager::resetStabilityStatus(StabilityStatus requiredStatus)
{
	setStabilityStatus(std::min(m_stabilityStatus, requiredStatus));
	if(getPortal()->getLoaded())
		getPortal()->runJobStabilize();
}


bool EntitiesSnapshotManager::rebuildAlignmentHash(const shared_ptr<IPortalDatabase> &database)
{
	String sql = String::format(_S("select id from os_entries").c_str());
	DataTable result;
	database->execute(sql,result);	

	getPortal()->getOptions()->resetAlignmentHash();
	
	for(uint32 r=0;r<result.rows();r++)
	{
		ObjectID id = static_cast<String>(result.get(r,_S("id"))).to_ascii();
		shared_ptr<ObjectsIObject> object = database->getPortal()->getObject(database, id);
		getPortal()->getOptions()->updateAlignmentHash(object->id, object->submit_date, true);
	}

	getPortal()->save();

	return true;
}

/* -----------------------------------------------------------
   Profile management
----------------------------------------------------------- */

void EntitiesSnapshotManager::ensureSnapshot(shared_ptr<IPortalDatabase> database, bool destroy)
{
	bool exists = false;

	//String profileID = entities::SnapshotProfile::GLOBAL;
	
	if(destroy)
	{
		// Objects Cache
		{
			String sql = _S("delete from os_snapshot_objects");
			database->execute(sql);
		}

		// Users Cache
		{
			String sql = _S("delete from os_snapshot_users");
			database->execute(sql);
		}

		// Notifico ai componenti che il profilo  stato rimosso
		{
			const ExtensionsSystem::Components &components = ExtensionsSystem::instance()->getComponents();
			for(ExtensionsSystem::Components::const_iterator i = components.begin(); i != components.end(); ++i)
			{
				shared_ptr<IExtensionsComponent> component = i->second;
				component->removeProfileData(database);
			}
		}

		// Notifico ai descriptor che il profilo  stato rimosso
		{
			const ObjectsDescriptors &descriptors = ObjectsSystem::instance()->getDescriptors();
			for(ObjectsDescriptors::const_iterator i = descriptors.begin(); i != descriptors.end(); ++i)
			{
				shared_ptr<ObjectsIDescriptor> descriptor = i->second;
				descriptor->removeStatistics(database);
			}
		}

		// Lucene
		destroySearchIndexes(database, false);

		invalidateTotal(database);

		exists = false;
	}
	else
	{
		String sql = String::format(_S("select count(*) from os_snapshot_objects where entity='%S'").c_str(), ObjectsSystem::instance()->getRootID().toUTF16().c_str());
		exists = (int(database->getConnection()->value_of(sql)) != 0);		
	}

	if(exists == false)
	{
		NotificationsManager::instance()->notify(_S("DB initialization of '") + database->getPortal()->getPovName() + _S("'."));
		
		
		createObjectRecord(database, ObjectsSection::getRootSection());
		createObjectRecord(database, ObjectsSection::getRecursiveSection());
		createObjectRecord(database, ObjectsSection::getOrphanSection());
		createObjectRecord(database, ObjectsSection::getSkippedSection());

		// Stabilizzazione manuale degli oggetti virtuali
		// Nota: tolto lo score, vedi TOCLEAN_SNAPSHOT_SCORE
		//String sql=String::format(_S("update os_snapshot_objects set visible=1, score=1, depth=1, current=reference where reference in ('%S','%S','%S')").c_str(),ObjectsSystem::instance()->getOrphanID().toUTF16().c_str(),ObjectsSystem::instance()->getRecursiveID().toUTF16().c_str(), ObjectsSystem::instance()->getSkippedID().toUTF16().c_str());
		String sql=String::format(_S("update os_snapshot_objects set visible=1, depth=1, current=entity where entity in ('%S','%S','%S')").c_str(), ObjectsSystem::instance()->getOrphanID().toUTF16().c_str(),ObjectsSystem::instance()->getRecursiveID().toUTF16().c_str(), ObjectsSystem::instance()->getSkippedID().toUTF16().c_str());
		database->execute(sql);

		// Nota: tolto lo score, vedi TOCLEAN_SNAPSHOT_SCORE
		//String sql2=String::format(_S("update os_snapshot_objects set visible=1, score=1, depth=0, current=reference where reference='%S'").c_str(),ObjectID::getRootID().toUTF16().c_str());
		String sql2=String::format(_S("update os_snapshot_objects set visible=1, depth=0, current=entity where entity='%S'").c_str(), ObjectsSystem::instance()->getRootID().toUTF16().c_str());
		database->execute(sql2);

		// Snapshot objects records
		{
			DataTable result;
			// "where entity is not nullptr" == revisionable objects.
			String sql=String::format(_S("select id from os_entries where entity is not nullptr").c_str());
			database->execute(sql, result);		

			for(uint32 r=0;r<result.rows();r++)
			{
				ObjectID id = static_cast<String>(result.get(r,_S("id"))).to_ascii();

				shared_ptr<ObjectsIRevisionable> obj = objects_revisionable_cast(getPortal()->getObject(database, id));
				OS_ASSERT(obj!=nullptr);

				createObjectRecord(database, obj);
			}
		}

		// Snapshot users records
		{
			DataTable result;
			String sql=String::format(_S("select id from os_users").c_str());
			database->execute(sql,result);		

			for(uint32 r=0;r<result.rows();r++)
			{
				ObjectID userID = static_cast<String>(result.get(r,_S("id"))).to_ascii();

				shared_ptr<ObjectsUser> user=objects_user_cast(getPortal()->getObject(database, userID));
				OS_ASSERT(user!=nullptr);
								
				createUserRecord(database, user);
			}
		}
		
		invalidateAll(database);
	}
	
	initSearchIndexes();

	if(destroy == true)
		rebuildAlignmentHash(database);
}

/* -----------------------------------------------------------
   Utils operations
----------------------------------------------------------- */

// TOCLEAN : con razor non dovrebbe più servire. E quindi non servirà più rtInherit
/*
String EntitiesSnapshotManager::getReputationSqlRule(ObjectsReputationThreshold threshold) const
{
	switch(threshold)
	{
	case objects::rtAll:
		return _S(">=-1");
	case objects::rtNegative:
		return _S(">-1");

	case objects::rtInherit:		// Compatibility (!=Razor)
	case objects::rtNotNegative:
		return _S(">=0");
	case objects::rtPositive:
		return _S(">0");
	default:
		{
			OS_ASSERTFALSE();
			return _S(">=-1");
		}
	}
}
*/

bool EntitiesSnapshotManager::compareReputation(ObjectsReputationThreshold threshold, double score) const
{
	switch(threshold)
	{
	case rtAll:
		return true;
	case rtNegative:
		return (score>-1);

	case rtInherit:			// Compatibility (!=Razor)
	case rtNotNegative:
		return (score>=0);
	case rtPositive:
		return (score>0);
	default:
		{
			OS_ASSERTFALSE();
			return true;
		}
	}
}

void EntitiesSnapshotManager::computeSqlForObjectRevisions(const shared_ptr<IPortalDatabase> &database, const EntityID &entityID, const PortalObjectType &objectType, bool onlyFirst, DataTable &result)
{
#ifdef OS_TODOCIP
	// TOCLEAN: objectType non + usato.
	String sql;
	sql = _S("select id, -1 as score, submit_date as fo from os_entries");
	sql += _S(" where entity='" + entityID.toUTF16() + "' and rank>=0");

	if(m_takeBackFrom.isValid())
		sql += String::format(_S(" and submit_date>='%S'").c_str(), m_takeBackFrom.toString().c_str());
	if(m_takeBackTo.isValid())
		sql += String::format(_S(" and submit_date<='%S'").c_str(), m_takeBackTo.toString().c_str());

	sql += _S(" order by fo desc");

	if(onlyFirst)
		sql += _S(" limit 1;");

	database->execute(sql,result);
#else
	//String table = primaryObj->getDescriptor()->getTableName();
	String table = DBTABLES::ENTRIES_TABLE;
	
	String fields = _S("t.id as id");
	if(onlyFirst)
		fields += _S(",so.score as score");
	fields += _S(",t.submit_date as fo");

	// Pre-ottimizzazione
	String sql;
	/*
	sql += String::format(_S("select %S from %S t,os_snapshot_users so ").c_str(), fields.c_str(), table.c_str());
	sql += String::format(_S("where so.profile='%S' and (t.revision='%S' or t.id='%S') and t.author=so.reference and ").c_str(), getID().c_str(), primaryObj->id->toString().c_str(), primaryObj->id->toString().c_str());
	if(m_takeBackFrom.isValid())
		sql += String::format(_S("t.submit_date>='%S' and ").c_str(), m_takeBackFrom.toString().c_str());
	if(m_takeBackTo.isValid())
		sql += String::format(_S("t.submit_date<='%S' and ").c_str(), m_takeBackTo.toString().c_str());
	if( (ot == portalObjectTypeSection) ||
		(ot == portalObjectTypeInstance) )
	{
		sql += String::format(_S("so.score %S ").c_str(), getReputationSqlRule(getEditorsReputationThreshold()).c_str());
	}
	else
	{
		sql += String::format(_S("( ( (t.author!='%S') and (so.score %S) ) || ").c_str(), primaryObj->author->toString().c_str(), getReputationSqlRule(getEditorsReputationThreshold()).c_str());
		sql += String::format(_S("  ( (t.author='%S') and (so.score %S) ) ) ").c_str(), primaryObj->author->toString().c_str(), getReputationSqlRule(getAuthorsReputationThreshold()).c_str());
	}
	*/

	// La spezzo in due usando un'union, in teoria con il cambio di interpretazione primaria dovrebbe essere riusabile senza union.

	// TOCLEAN1: Con Razor credo posso pulire poi sta parte: mentre stabilizza qualsiasi oggetto con rank>=0 è già valido, senza le where su score e soglie.
	// --> Fatto, da testare. 

	// TOCLEAN2: Con Razor & CIP, posso abolire la UNION.

	// TOCLEAN_SNAPSHOT_SCORE: Non ho idea del perchè mi salvo nella os_snapshot_objects lo "score". Mi obbliga qui a una join in più. E a memoria non serve a nulla, per cui sarà inusato.
	// --> Ad una prima verifica solo qui, non lo uso, per cui potrei provare a rimuoverlo.
	// --> Provo a togliere il campo, vedrò se incontro problemi.

	// Parte "id"
	// TOFIX_SQLITE
	sql += String::format(_S("select %S from %S t, os_entries te, os_snapshot_users so ").c_str(), fields.c_str(), table.c_str());
	sql += String::format(_S("where t.id='%S' and te.id=t.id and so.reference=t.author and ").c_str(), entityID.toUTF16().c_str());
	if(m_takeBackFrom.isValid())
		sql += String::format(_S("t.submit_date>='%S' and ").c_str(), m_takeBackFrom.toString().c_str());
	if(m_takeBackTo.isValid())
		sql += String::format(_S("t.submit_date<='%S' and ").c_str(), m_takeBackTo.toString().c_str());

	sql += _S("t.rank >= 0 ");

	/*
	if( (ot == portalObjectTypeSection) ||
		(ot == portalObjectTypeInstance) )
	{
		sql += String::format(_S("so.score %S ").c_str(), getReputationSqlRule(getPortal()->getOptionsShared()->getEditorsReputationThreshold()).c_str());
	}
	else
	{
		sql += String::format(_S("( ( (t.author!='%S') and (so.score %S) ) || ").c_str(), primaryObj->author->toUTF16().c_str(), getReputationSqlRule(getPortal()->getOptionsShared()->getEditorsReputationThreshold()).c_str());
		sql += String::format(_S("  ( (t.author='%S') and (so.score %S) ) ) ").c_str(), primaryObj->author->toUTF16().c_str(), getReputationSqlRule(getPortal()->getOptionsShared()->getAuthorsReputationThreshold()).c_str());
	}
	*/

	sql += _S(" union all ");

	// Parte "revision"
	// TOFIX_SQLITE
	sql += String::format(_S("select %S from %S t, os_entries te, os_snapshot_users so ").c_str(), fields.c_str(), table.c_str());
	sql += String::format(_S("where t.revision='%S' and te.id=t.revision and so.reference=t.author and ").c_str(), entityID.toUTF16().c_str());
	if(m_takeBackFrom.isValid())
		sql += String::format(_S("t.submit_date>='%S' and ").c_str(), m_takeBackFrom.toString().c_str());
	if(m_takeBackTo.isValid())
		sql += String::format(_S("t.submit_date<='%S' and ").c_str(), m_takeBackTo.toString().c_str());

	sql += _S("t.rank>=0 ");
	/*
	if( (ot == portalObjectTypeSection) ||
		(ot == portalObjectTypeInstance) )
	{
		sql += String::format(_S("so.score %S ").c_str(), getReputationSqlRule(getPortal()->getOptionsShared()->getEditorsReputationThreshold()).c_str());
	}
	else
	{
		sql += String::format(_S("( ( (t.author!='%S') and (so.score %S) ) || ").c_str(), primaryObj->author->toUTF16().c_str(), getReputationSqlRule(getPortal()->getOptionsShared()->getEditorsReputationThreshold()).c_str());
		sql += String::format(_S("  ( (t.author='%S') and (so.score %S) ) ) ").c_str(), primaryObj->author->toUTF16().c_str(), getReputationSqlRule(getPortal()->getOptionsShared()->getAuthorsReputationThreshold()).c_str());
	}
	*/


	sql += _S("order by fo desc");

	if(onlyFirst)
		sql += _S(" limit 1;");

	database->execute(sql,result);
#endif
}

/* -----------------------------------------------------------
   ActionTick operations
----------------------------------------------------------- */

void EntitiesSnapshotManager::updateLastActionTick()
{
	// Il concetto  che lo stability_thread lavora solo se  trascorso 'deltaStabilityTick' tempo dall'ultima azione.
	// Esempi di azione sono l'arrivo di un nuovo oggetto, o l'utente che naviga nel portale.
	// Questo per evitare che il thread lavori mentre l'utente naviga (indipendentemente dal fatto che  in priority idle,
	// oppure per evitare che riparta in continuazione ad ogni oggetto che arriva.
	m_lastActionTick = PlatformManager::instance()->tickCount();
}

bool EntitiesSnapshotManager::checkLastActionTick()
{
	if(m_lastActionTick == 0)
		return false;
	double currentTick = PlatformManager::instance()->tickCount();

	uint32 stabilityDelta = Options::instance()->getOption<uint32>(Options::portals_options::stability_delta);

	return ((currentTick-m_lastActionTick) > stabilityDelta);
}

/* ******************************************************************************************
   Operations for Lucene support.
******************************************************************************************

TOOPTIMIZE

1-
http://www.nabble.com/forum/ViewPost.jtp?post=4290095&framed=y
Il "deleteDocuments" nella versione CLucene (ad oggi 18/12/2007) non  supportato nell'IndexWriter.
Appena lo sar sarebbe da usare, dato che un IndexReader  aperto/chiuso ad ogni indicizzazione di ogni oggetto.
Non posso tenerne uno aperto perch l'IndexReader vede solo gli oggetti presenti da prima della sua apertura.

2-
01/01/2008 - Ci sarebbe da valutare l'uso di un SimpleAnalyzer al posto di uno StandardAnalyzer.
08/02/2008 - Verificato, apparentemente irrilevante.

3-
C' da valutare se degli "optimize" ogni x documenti aggiunti migliorano le performance.

4-
Calibrare setMergeFactor

5-
Calibrare setMinMergeDocs

6-
http://wiki.apache.org/lucene-java/ImproveIndexingSpeed?highlight=%28speed%29


*/

String EntitiesSnapshotManager::convertScoreForLucene(const double &score)
{
	return String::format(_S("%04d").c_str(), int32((score+1)*1000));
}

std::string EntitiesSnapshotManager::getSearchPathForLucene() const
{
	// Lucene non supporta i path che terminano in '/' o '\'
	String path = getPortal()->getSearchPath();
	path.trim_right(_S("/\\"));
	return path.to_ascii();
}

void EntitiesSnapshotManager::initSearchIndexes()
{
	OS_LOCK(m_searchCS);

	// Se la directory degli indici non esiste (cancellata manualmente?) la ricrea.
	// Se precedentemente il processo era stato interrotto mentre il lucene aveva aperto delle lock, qui vengono rimosse.
	std::string path = getSearchPathForLucene();
	//bool create = true;

	FileSystem::instance()->ensureDirectory(getPortal()->getSearchPath());
	if(lucene::index::IndexReader::indexExists(path.c_str()))
	{
		//create = false;

		if(lucene::index::IndexReader::isLocked(path.c_str()))
			lucene::index::IndexReader::unlock(path.c_str());
	}

	// Quale Analyzer usare? Esempi: http://today.java.net/pub/a/today/2003/07/30/LuceneIntro.html
	// TOOPTIMIZE: Bisognerebbe fare dei test di tempi tra lo StandardAnalyzer e il SimpleAnalyzer
	// TOOPTIMIZE: L'IndexModifier  una skifezza, tiene internamente un IndexReader e un IndexWriter che alterna (chiudendoli).
	// C' da aggiornare il CLucene quando implementeranno il deleteDocuments nell'IndexWriter, cosa gi fatta nella versione Java.

	//initSearchIndexWriter(create);
}

void EntitiesSnapshotManager::openSearchIndexWriter()
{
	if(m_impl->luceneIndexWriter != nullptr)
		return;

	OS_LOCK(m_searchCS);

	std::string path = getSearchPathForLucene();

	bool create = true;
	if(lucene::index::IndexReader::indexExists(path.c_str()))
		create = false;

	m_impl->luceneIndexWriter.reset(new lucene::index::IndexWriter(path.c_str(), &m_impl->luceneAnalyzer, create));

	/*
	Determines how often segment indices are merged by addDocument().  With
	smaller values, less RAM is used while indexing, and searches on
	unoptimized indices are faster, but indexing speed is slower.  With larger
	values more RAM is used while indexing and searches on unoptimized indices
	are slower, but indexing is faster.  Thus larger values (> 10) are best
	for batched index creation, and smaller values (< 10) for indices that are
	interactively maintained.
	This must never be less than 2.  The default value is 10.

	08/02/2008 - Differenza poco rilevante con ==100
	*/
	m_impl->luceneIndexWriter->setMergeFactor(10);

	/*
	Determines the minimal number of documents required before the buffered
	in-memory documents are merging and a new Segment is created.
	Since Documents are merged in a {@link RAMDirectory},
	large value gives faster indexing.  At the same time, mergeFactor limits
	the number of files open in a FSDirectory.
	The default value is 10.
	*/
	m_impl->luceneIndexWriter->setMinMergeDocs(1000);
}

void EntitiesSnapshotManager::closeSearchIndexWriter()
{
	if(m_impl->luceneIndexWriter != nullptr)
	{
		OS_LOCK(m_searchCS);

		m_impl->luceneIndexWriter->close();
		m_impl->luceneIndexWriter = nullptr;
	}
}

void EntitiesSnapshotManager::deInitSearchIndexes()
{
	closeSearchIndexWriter();
}

void EntitiesSnapshotManager::destroySearchIndexes(const shared_ptr<IPortalDatabase> &database, bool forRebuild)
{
	OS_LOCK(m_searchCS);

	deInitSearchIndexes();

	if(forRebuild)
	{
		String sql=String::format(_S("update os_snapshot_objects set search=0,document=''").c_str());
		database->execute(sql);
		resetStabilityStatus(ssSearchObjects);
	}

	FileSystem::instance()->removeDirectory(getPortal()->getSearchPath());

	initSearchIndexes();
}

void EntitiesSnapshotManager::optimizeSearchIndexes()
{
	OS_LOCK(m_searchCS);

	try
	{
		// Qui chiudo e riapro il writer. Questo per forzare il "flush" dei documenti in ram su disco. Non ho trovato altro modo.
		// Serve davvero?
		//m_impl->luceneIndexWriter->close();
		//initSearchIndexWriter(false);

		openSearchIndexWriter();

		m_impl->luceneIndexWriter->optimize();

		closeSearchIndexWriter();

		//m_autoOptimizerCounter = 0;
	}
	catch(CLuceneError &error)
	{
		OS_LOG_ERROR(String(_S("Lucene: ")) + error.twhat());
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(String(_S("Lucene: ")) + e.what());
    }
    catch(...)
    {
		OS_LOG_ERROR(String(_S("Lucene: ")) + OS_ERR_UNKNOWN(lucene));
    }
}

void EntitiesSnapshotManager::updateObjectSearch(const shared_ptr<IPortalDatabase> &database, const EntityID &primaryID, const String &document, const int32 &level)
{
	OS_LOCK(m_searchCS);

	if(level == 0)
		return;

	// Estrazione dati sull'entità
	RealtimeStatsScopeTimer RSRX(_S("Debug"), _S("Debug: SnapshotProfile::updateSearchObject::extractInfo"));
	shared_ptr<EntitiesEntity> entity = getEntity(database, primaryID);
	shared_ptr<ObjectsIRevisionable> currentObj = entity->getCurrent();
	bool visible = ( (entity->getVisible()) && (currentObj != nullptr) );
	//const double &score = entity->getScore(); // TOCLEAN_SNAPSHOT_SCORE. Se vorrò riprestinare la modalità di ordinamento "reputazioni" nelle ricerche, ha senso che qui rintraccio l'utente e prendo lo score da lì.
	const EntityID &sectionID = entity->getSectionID();

	DateTime insertDate;
	ObjectID currentID;
	if(currentObj != nullptr)
	{
		currentID = currentObj->id;
		// URGENT, da togliere e usare current->insert_date.
		// Calcola la insert_date
		DataTable result;
		String sql=String::format(_S("select insert_date from os_entries where id='%S'").c_str(), currentID.toUTF16().c_str());
		database->execute(sql,result);
		if(result.rows()>=1)
			insertDate = String(result.get(0, _S("insert_date")));
	}
	RSRX.stop();

	// Aggiornamento Lucene
	bool luceneError = false;
	bool luceneUpdate = false;
	String currentDocumentID;
	
	if(ObjectsSystem::instance()->isVirtual(primaryID) == false)
	{
		/*
		if(m_autoOptimizerCounter >= 1000) // URGENT, farci un'opzione
		{
			optimizeSearchIndexes();
		}
		else
			m_autoOptimizerCounter++;
		*/

		try
		{
			std::string path = getSearchPathForLucene();

			if(visible)
			{
				//String currentDocumentKey = String::format(_S("%S%S%g%d").c_str(), currentID.toUTF16().c_str(), sectionID.toUTF16().c_str(), score, visible); // TOCLEAN_SNAPSHOT_SCORE
				String currentDocumentKey = String::format(_S("%S%S%d").c_str(), currentID.toUTF16().c_str(), sectionID.toUTF16().c_str(), visible); // TOCLEAN_SNAPSHOT_SCORE
				currentDocumentID = CryptManager:: instance()->SHA(currentDocumentKey.buffer(), currentDocumentKey.buffer_size()).toHex();
			}

			bool needRemove = false;
			bool needAdd = false;
			if(currentDocumentID != document)
			{
				needRemove = (document.empty() == false);
				needAdd = visible;
				luceneUpdate = true;
			}

			/*
			RealtimeStatsScopeTimer RSL(_S("Debug"), _S("Debug: SnapshotProfile::updateSearchObject::localize"));
			// OPTIMNOTE :
			// Su una totale "computeSearchObjects" di 230 sec, questa "localizzazione" impiega circa 15 secondi. Il 6,5%.
			// TOOPTIMIZE: Si potrebbe valutare di inserire un flag in una tabella che eviti questa search.. da valutare..
			// Creare un'unico "lucene::search::IndexSearcher" al posto di crearlo per ogni oggetto e' irrilevante.
			lucene::search::IndexSearcher searcher(path.c_str());
			lucene::index::Term term(OS_SEARCH_FIELD_ID.to_wide().c_str(), primaryID.toUTF16().to_wide().c_str());
			scoped_ptr<lucene::search::Query> query(new lucene::search::TermQuery(&term));
			scoped_ptr<lucene::search::Hits> hits(searcher.search(query.get()));
			uint32 totalResults = hits->length();
			RSL.stop();

			RealtimeStatsScopeTimer RSD(_S("Debug"), _S("Debug: SnapshotProfile::updateSearchObject::checking_diff"));
			bool needRemove = false;
			bool needAdd = false;

			if(totalResults>1)
			{
				// Non dovrebbe mai accadere che lo stesso documento  presente in + copie nell'indice.
				OS_ASSERT(false);
				needRemove = true;
			}

			if(totalResults != 1)
			{
				if(visible)
					needAdd = true;
			}
			else
			{
				if(visible == false)
					needRemove = true;
				else
				{
					//ObjectID currentID = obj->id;

					// Confronto
					ObjectID actualCurrent = static_cast<String>(hits->doc(0).get(OS_SEARCH_FIELD_CURRENT.to_wide().c_str()));
					String actualScore = static_cast<String>(hits->doc(0).get(OS_SEARCH_FIELD_SCORE.to_wide().c_str()));
					ObjectID actualSection = static_cast<String>(hits->doc(0).get(OS_SEARCH_FIELD_SECTION.to_wide().c_str()));

					if( (actualCurrent != currentID) || (actualScore != convertScoreForLucene(score)) || (actualSection != sectionID) )
					{
						needRemove = true;
						needAdd = true;
					}
				}
			}
			RSD.stop();
			*/

			if(needRemove)
			{
				RealtimeStatsScopeTimer RSR(_S("Debug"), _S("Debug: SnapshotProfile::updateSearchObject::needRemove"));

				// Paura, chiudo l'IndexWriter per permettere all'IndexReader di modificare l'indice senza errori di lock.
				// Temporaneo perche'  l'IndexWriter che dovrebbe avere la "deleteDocuments":  gia' cosi' nel Lucene-Java (>=v2)
				// ma ad oggi 01/01/2008 non supportato dal CLucene.
				//m_impl->luceneIndexWriter->close();
				closeSearchIndexWriter();

				boost::scoped_ptr<lucene::index::IndexReader> reader(lucene::index::IndexReader::open(path.c_str()));
				lucene::index::Term term(OS_SEARCH_FIELD_ID.to_wide().c_str(), primaryID.toUTF16().to_wide().c_str());
				reader->deleteDocuments(&term);
				reader->close();

				//initSearchIndexWriter(false);

				RSR.stop();
			}

			if(needAdd)
			{
				RealtimeStatsScopeTimer RSA2(_S("Debug"), _S("Debug: SnapshotProfile::updateSearchObject::needAdd"));

				openSearchIndexWriter();

				String title = currentObj->getTitle();
				String content = currentObj->getSearchContent();
				if(level == 2)
				{
					// Su una totale "computeSearchObjects" di 230 sec, questa "parse" impiega circa 55 secondi. Il 24%.
					RealtimeStatsScopeTimer RSA2C(_S("Debug"), _S("Debug: SnapshotProfile::updateSearchObject::parse content"));
					title = OMLManager::instance()->parse(title, nullptr, true, false, false, omlRenderModeSearch, primaryID.getString(), String::EMPTY);
					content = OMLManager::instance()->parse(content, nullptr, true, false, false, omlRenderModeSearch, primaryID.getString(), String::EMPTY);
					RSA2C.stop();
				}


				/* // TOCLEAN_SNAPSHOT_SCORE
				// Lo score  espresso da -1 a +1. Dato che nel Lucene deve essere inserito come stringa, e gli ordinamenti
				// sono sulla stringa, viene utilizzata la formula sottostante di conversione.
				// Ex:		-1	->	0000
				// Ex.		 0  ->  1000
				// Ex.      +1  ->  2000
				String scoreString = convertScoreForLucene(score);
				*/

				lucene::document::Document document;

				// Nota: A causa di qualche problema del Lucene
				// http://www.mail-archive.com/lucene-user@jakarta.apache.org/msg12320.html
				// OS_SEARCH_FIELD_PARENT, nonostante potrebbe essere un Field::Keyword, attualmente  un Field::Text.
				// Non comporta problemi, solamente potenzialmente inutile dato che il tokenizer non riuscira' a
				// tokenizzarlo e lo trattera' cmq come una keyword.
				// Item OS_SEARCH_FIELD_PARENTSECTION e OS_SEARCH_FIELD_PARENTGROUP

				document.add(*lucene::document::Field::Keyword(OS_SEARCH_FIELD_ID.to_wide().c_str(), primaryID.toUTF16().to_wide().c_str()));
				//document.add(*lucene::document::Field::Text(OS_SEARCH_FIELD_ID, obj->getPrimary().toString().c_str()));
				document.add(*lucene::document::Field::Text(OS_SEARCH_FIELD_CURRENT.to_wide().c_str(), currentID.toUTF16().to_wide().c_str())); // Aggiunto nella 0.11
				document.add(*lucene::document::Field::Text(OS_SEARCH_FIELD_TITLE.to_wide().c_str(), title.to_wide().c_str()));
				document.add(*lucene::document::Field::Text(OS_SEARCH_FIELD_CONTENT.to_wide().c_str(), content.to_wide().c_str()));
				document.add(*lucene::document::Field::Text(OS_SEARCH_FIELD_PARENT.to_wide().c_str(), currentObj->getParent().toUTF16().to_wide().c_str()));
				document.add(*lucene::document::Field::Keyword(OS_SEARCH_FIELD_SUBMITDATE.to_wide().c_str(), currentObj->submit_date->toString().to_wide().c_str()));
				document.add(*lucene::document::Field::Keyword(OS_SEARCH_FIELD_INSERTDATE.to_wide().c_str(), insertDate.toString().to_wide().c_str()));
				//document.add(*lucene::document::Field::Keyword(OS_SEARCH_FIELD_SCORE.to_wide().c_str(), scoreString.to_wide().c_str())); // TOCLEAN_SNAPSHOT_SCORE
				document.add(*lucene::document::Field::Keyword(OS_SEARCH_FIELD_VISIBLE.to_wide().c_str(), conversions::to_utf16(visible).to_wide().c_str()));
				document.add(*lucene::document::Field::Text(OS_SEARCH_FIELD_SECTION.to_wide().c_str(), sectionID.toUTF16().to_wide().c_str()));

				/*
				document.add(*lucene::document::Field::Text(_S("alfa"), _S("03000002ED6375236CF39ECEB1D93D96F684AA1DBC8A65C2")));
				document.add(*lucene::document::Field::Keyword(_S("gamma"), _S("03000002ed6375236cf39eceb1d93d96f684aa1dbc8a65c2")));
				document.add(*lucene::document::Field::Keyword(_S("beta"), _S("03000002ed637")));
				document.add(*lucene::document::Field::Keyword(_S("delta"), _S("ed6375236cf39")));
				document.add(*lucene::document::Field::Keyword(_S("omega"), _S("e3000002ed6375236cf39eceb1d93d96f684aa1dbc8a65c2")));

				OS_TRACE(_S("------------------------------\n"));
				OS_TRACE(String::format(_S("%S:%S\n").c_str(), OS_SEARCH_FIELD_ID, obj->getPrimary().toString().c_str()));
				OS_TRACE(String::format(_S("%S:%S\n").c_str(), OS_SEARCH_FIELD_SUBMITDATE, obj->submit_date->toString().c_str()));
				OS_TRACE(String::format(_S("%S:%S\n").c_str(), OS_SEARCH_FIELD_PARENT, obj->getParent().toString().c_str()));
				*/

				RealtimeStatsScopeTimer RSA(_S("Debug"), _S("Debug: SnapshotProfile::updateSearchObject::addDocument"));
				m_impl->luceneIndexWriter->addDocument(&document);
				RSA.stop();
				RSA2.stop();
			}
		}
		catch(CLuceneError &error)
		{
			OS_LOG_ERROR(String(_S("Lucene: ")) + error.twhat());
			luceneError = true;
		}
		catch(std::exception &e)
		{
			OS_LOG_ERROR(String(_S("Lucene: ")) + e.what());
			luceneError = true;
		}
		catch(...)
		{
			OS_LOG_ERROR(String(_S("Lucene: ")) + OS_ERR_UNKNOWN(lucene));
			luceneError = true;
		}
	}

	// Aggiornamento record
	if(luceneError == false)
	{

		RealtimeStatsScopeTimer RSRXG(_S("Debug"), _S("Debug: SnapshotProfile::updateSearchObject::updatingRecord"));
		String sql = _S("update os_snapshot_objects set search=1 ");
		if(luceneUpdate)
			sql += _S(", document='") + currentDocumentID + _S("' ");
		sql += _S(" where entity='") + primaryID.toUTF16() + _S("'");
		database->execute(sql);
		RSRXG.stop();
	}
}
































/* -----------------------------------------------------------
   Ex SnapshotProfile
----------------------------------------------------------- */
/*
ObjectID EntitiesSnapshotManager::getUserID()
{
	// Razor
	return getPortal()->getUserID();	
}
*/


shared_ptr<EntitiesEntity> EntitiesSnapshotManager::getEntity(const shared_ptr<IPortalDatabase> database, const EntityID id)
{
	RealtimeStatsScopeTimer RSS(_S("Debug"), _S("SnapshotProfile::getEntity"));
	return getEntities()->myGet(database, id);
}

ReputationsScore EntitiesSnapshotManager::computeReputation(const shared_ptr<IPortalDatabase> &database, const String &reference, const shared_ptr<ReputationsFeedback> &feedback)
{
	return getPortal()->getReputationManager()->compute(database, getPortal()->getPovUserID(), reference.to_ascii(), feedback);
}


void EntitiesSnapshotManager::getObjectRevision(const shared_ptr<IPortalDatabase> &database, const EntityID &entityID, const PortalObjectType &objectType, ObjectID &id, double &score)
{
	DataTable result;
	computeSqlForObjectRevisions(database, entityID, objectType, true, result);

	if(result.hasRow(0))
	{
		id = static_cast<String>(result.get(0,_S("id"))).to_ascii();
		score = result.get(0,_S("score"));
	}
	else
	{
		id.clear();
		score = 0;
	}
}

void EntitiesSnapshotManager::getObjectRevisions(const shared_ptr<IPortalDatabase> &database, const EntityID &entityID, const PortalObjectType &objectType, StringList &results)
{
	DataTable result;
	computeSqlForObjectRevisions(database, entityID, objectType, false, result);

	for(uint32 r=0;r<result.rows();r++)
	{
		String id = result.get(r,_S("id"));
		results.push_back(id);
	}
}



void EntitiesSnapshotManager::updateStabilityDate(const shared_ptr<IPortalDatabase> &database)
{
	setStabilityDate(database, DateTime::now());
}

DateTime EntitiesSnapshotManager::getStabilityDate(const shared_ptr<IPortalDatabase> &database)
{
	// VERYURGENT: 17/12/2009, 0.13 : Perchè non uso quella di root?
	// Non ricordo il motivo, e mi eviterebbe di avere una tabella in +....
	//return getEntity(database, ObjectID::getRootID())->getStabilityDate();
	//return getData()->stability_date;

	// Razor
	return getPortal()->getOptions()->getLastStabilizationDate();
}

void EntitiesSnapshotManager::setStabilityDate(const shared_ptr<IPortalDatabase> &database, const DateTime &date)
{
	// Invalido gli oggetti virtuali.
	String sql=String::format(_S("update os_snapshot_objects set stability_date = '%S' where entity in ('%S','%S','%S','%S')").c_str(),
		DateTime::EMPTY.toString().c_str(),
		ObjectsSystem::instance()->getRootID().toUTF16().c_str(),
		ObjectsSystem::instance()->getSkippedID().toUTF16().c_str(),
		ObjectsSystem::instance()->getOrphanID().toUTF16().c_str(),
		ObjectsSystem::instance()->getRecursiveID().toUTF16().c_str());
	database->execute(sql);

	getPortal()->getOptions()->setLastStabilizationDate(date);
	//getData()->stability_date = date;
	//database->updateRecord(getData());

	resetStabilityStatus(ssStabilityUsers);
}



/*
ObjectsReputationThreshold SnapshotProfile::getAuthorsReputationThreshold() const
{
	if(isGlobal())
	{
		return rtNotNegative;
	}
	else
	{
		if( (m_user == nullptr) || (m_user->authors_threshold.isNull()) )
			return rtNotNegative;
		else
			return Convert::toReputationThreshold(m_user->authors_threshold);
	}	
}

ObjectsReputationThreshold SnapshotProfile::getEditorsReputationThreshold() const
{
	if(isGlobal())
	{
		// URGENT DA PRENDERE DA UN'OPZIONE
		return rtPositive;
	}
	else
	{
		if( (m_user == nullptr) || (m_user->authors_threshold.isNull()) )
			return rtNotNegative;
		else
			return Convert::toReputationThreshold(m_user->editors_threshold);
	}
}
*/



/*
void EntitiesSnapshotManager::resetUserStability(const shared_ptr<IPortalDatabase> &database, ObjectID userID, bool invalidateStability, bool invalidateStatistics)
{	
}
*/

/*
void EntitiesSnapshotManager::resetObjectStability(const shared_ptr<IPortalDatabase> &database, EntityID primaryID, bool invalidateStability, bool invalidateStatistics)
{
	
}
*/















void EntitiesSnapshotManager::cronChecking()
{
	// Eseguita una volta al giorno... o?

	// Oggetti o post più vecchi di X
	// select where submit_date<...
	//		for each, invalidateAccepted(object)
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
