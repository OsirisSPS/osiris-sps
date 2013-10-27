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
#include "portalsportal.h"

#include "compatibilityrazor.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "buffer.h"
#include "compatibilitymanager.h"
#include "convert.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "dataaccount.h"
#include "datapeer.h"
#include "idbconnection.h"
#include "isissystem.h"
#include "objectsprivatemessage.h"
#include "objectsuser.h"
#include "iportaldatabase.h"
#include "dbdatabasessystem.h"
#include "idbdriver.h"
#include "dbschemaschema.h"
#include "engine.h"
#include "entitiessnapshotmanager.h"
#include "extensionssystem.h"
#include "filesystem.h"
#include "httpparser.h"
#include "httpurl.h"
#include "ipaddress.h"
#include "ideaccount.h"
#include "ideportalaccount.h"
#include "idesession.h"
#include "isis.h"
#include "log.h"
#include "networksystem.h"
#include "notificationsmanager.h"
#include "objectsiobject.h"
#include "objectsirevisionable.h"
#include "objectsobjectscache.h"
#include "options.h"
#include "osirislink.h"
#include "p2pexchangecontext.h"
#include "platformmanager.h"
#include "portalsdatabasescache.h"
#include "portalsaccount.h"
#include "portalsimporterjob.h"
#include "idesession.h"
#include "portalsmessenger.h"
#include "portalsoptionsshared.h"
#include "portalsoptimizerjob.h"
#include "portalspeer.h"
#include "portalspeersmanager.h"
#include "portalsportaloptions.h"
#include "portalsportalprocessor.h"
#include "portalsportalssystem.h"
#include "portalsprofilescache.h"
#include "portalssyncjob.h"
#include "portalsstabilityjob.h"
#include "portalstransaction.h"
#include "randomnumbergenerator.h"
#include "realtimestats.h"
#include "reputationsmanager.h"
#include "xmlattribute.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Portal::DatabaseImpl : public IPortalDatabase
{
	typedef IPortalDatabase DatabaseBase;

// Construction
public:
	DatabaseImpl(shared_ptr<Portal> portal, const shared_ptr<IDbConnection> &connection);
	virtual ~DatabaseImpl();

// Attributes
public:
	inline shared_ptr<Portal> getPortal() const;

// Database overrides
public:
	virtual shared_ptr<ObjectsIObject> getObject(const ObjectID &id);

// Database interface
protected:
	virtual void onInsertRecord(shared_ptr<DataIRecord> record);
	virtual void onUpdateRecord(shared_ptr<DataIRecord> record);
	virtual void onRemovingRecord(shared_ptr<DataIRecord> record);

	void checkInvalidate(const shared_ptr<DataIRecord> &record);

protected:
	weak_ptr<Portal> m_portal;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<Portal> Portal::DatabaseImpl::getPortal() const { return m_portal.lock(); }

//////////////////////////////////////////////////////////////////////

Portal::DatabaseImpl::DatabaseImpl(shared_ptr<Portal> portal, const shared_ptr<IDbConnection> &connection) : DatabaseBase(connection),
																												   m_portal(portal)
{

}

Portal::DatabaseImpl::~DatabaseImpl()
{

}

shared_ptr<ObjectsIObject> Portal::DatabaseImpl::getObject(const ObjectID &id)
{
	return getPortal()->getObject(get_this_ptr(), id);
}

void Portal::DatabaseImpl::checkInvalidate(const shared_ptr<DataIRecord> &record)
{
	// URGENT DA CONTROLLARE CHE GIRI SERVONO PER I PM.
	// ABOLIRE PROPRIO la checkInvalidate, o cmq cambiare nome.
	if(record->getRecordType() == dataRecordTypeObject)
	{
		shared_ptr<ObjectsIObject> object = objects_object_cast(record);
		PortalObjectType objectType = object->getObjectType();

		switch(objectType)
		{
		case portalObjectTypePrivateMessage:		getPortal()->getMessenger()->handleMessage(objects_private_message_cast(object));
											break;
		}
	}
}

void Portal::DatabaseImpl::onInsertRecord(shared_ptr<DataIRecord> record)
{
	switch(record->getRecordType())
	{
	case dataRecordTypeObject:
		{
			getPortal()->getSnapshotManager()->onInsertObject(get_this_ptr(), objects_object_cast(record));

			checkInvalidate(record);
		} break;
	}
}

void Portal::DatabaseImpl::onUpdateRecord(shared_ptr<DataIRecord> record)
{
	switch(record->getRecordType())
	{
	case dataRecordTypeObject:	getPortal()->getSnapshotManager()->onUpdateObject(get_this_ptr(), objects_object_cast(record));
								checkInvalidate(record);

								break;
	}
}

void Portal::DatabaseImpl::onRemovingRecord(shared_ptr<DataIRecord> record)
{
	switch(record->getRecordType())
	{
	case dataRecordTypeObject:	getPortal()->getSnapshotManager()->onRemovingObject(get_this_ptr(), objects_object_cast(record));
								checkInvalidate(record);

								break;
	}
}

//////////////////////////////////////////////////////////////////////

Portal::DatabaseLockData::DatabaseLockData() : m_exclusiveRequests(0)
{

}

Portal::DatabaseLockData::~DatabaseLockData()
{

}

uint32 Portal::DatabaseLockData::getExclusiveRequests() const
{
	OS_LOCK(m_cs);
	return m_exclusiveRequests;
}

boost::recursive_mutex & Portal::DatabaseLockData::getDatabaseExclusiveLock() const
{
	return m_exclusiveCS;
}

void Portal::DatabaseLockData::addExclusiveRequest()
{
	updateExclusiveRequests(true);
}

void Portal::DatabaseLockData::removeExclusiveRequest()
{
	updateExclusiveRequests(false);
}

void Portal::DatabaseLockData::updateExclusiveRequests(bool added)
{
	OS_LOCK(m_cs);

	if(added)
	{
		m_exclusiveRequests++;
	}
	else
	{
		OS_ASSERT(m_exclusiveRequests > 0);
		m_exclusiveRequests--;
	}
}

//////////////////////////////////////////////////////////////////////

Portal::Portal(uint32 peersCacheSize, uint32 databasesCacheSize) : m_loaded(false),
																   m_snapshotManager(null),
																   m_optionsShared(null),
																   m_objects(null),
																   //m_accountsManager(null),
																   m_messenger(null),
																   m_lockData(OS_NEW DatabaseLockData()),
																   m_peersCS(OS_NEW_T(boost::recursive_mutex), os_delete_t()),
																   m_databasesCache(OS_NEW PortalsDatabasesCache(*this, databasesCacheSize)),
																   m_peersManager(OS_NEW PortalsPeersManager(*this, peersCacheSize)),
																   m_stabilizePending(false),
																   m_nodesSameAlign(0),
																   m_nodesSamePortalPov(0)
{

}

Portal::~Portal()
{
    OS_ASSERT(PortalsSystem::exists());

	unload();

	OS_ASSERT(getLoaded() == false);
}

Path Portal::getPath() const
{
	OS_LOCK(m_dataCS);
	return m_path;
}

bool Portal::getLoaded() const
{
	OS_LOCK(m_dataCS);
	return m_loaded;
}

String Portal::getName() const
{
	String name = String::EMPTY;

	if(getOptionsShared())
		name = getOptionsShared()->getPortalName();
	if( (m_options != null) && (name == String::EMPTY) )
		name = m_options->getName();

	return name;
}

const PortalID & Portal::getPortalID() const
{
	if(m_options != null)
		return m_options->getPortalID();

	OS_ASSERTFALSE();
	return PortalID::EMPTY;
}

const PovID & Portal::getPovID() const
{
	if(m_options != null)
		return m_options->getPovID();

	OS_ASSERTFALSE();	
	return PovID::EMPTY;
}

ObjectID Portal::getPovUserID() const // TOCLEAN
{
#ifdef OS_NOOBJECTID
	return getPovID();
#else
	return ObjectID::generate(portalObjectTypeUser, getPovID().getString());
#endif
}

std::string Portal::getFullPovID() const
{
	return generatePovID(getPortalID(),getPovID());
}

// Razor, ID univoco del network.
// Per ora è uguale a prima.
std::string Portal::getNetworkID() const
{
	return getFullPovID();
}

shared_ptr<ObjectsUser> Portal::getPovUser() const
{
	return m_user;
}

/*
PortalType Portal::getType_RRR() const
{
	return getPortalID().getPortalType();
}

PortalAccess Portal::getAccess() const
{
	return getPortalID().getPortalAccess();
}
*/

String Portal::getPovName() const
{	
	String name = getName();

	bool showUser = false;

	// Only if exists more that one subscription to the same portal, show user informations.
	PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
	for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
	{
		shared_ptr<Portal> portal = *i;

		if( (portal->getPortalID() == getPortalID()) && (portal->getPovID() != getPovID()) )
		{
			showUser = true;
			break;
		}
	}

	if(showUser)
	{
		if(getPovUser() != null)
			name += _S(" - ") + getPovUser()->name;
		else
			name += _S(" - ") + Engine::instance()->getText(_S("common.unknown"));
	}

	return name;
}

String Portal::getSearchPath() const
{	
	String path = utils::makeFolderPath(m_path, OS_PORTAL_SEARCH_PATH);
	//FileSystem::instance()->createDirectory(path);
	return path;
}

p2p::exchange_context_ptr Portal::getExchangeContext()
{
	OS_LOCK(m_contextCS);

	// Ottiene il contesto corrente
	p2p::exchange_context_ptr exchangeContext = m_exchangeContext.lock();
	// Controlla se il contesto  scaduto
	if(exchangeContext == null)
	{
		// Crea un nuovo contesto p2p
		exchangeContext.reset(OS_NEW p2p::ExchangeContext(get_this_ptr()));
		m_exchangeContext = exchangeContext;
	}

	return exchangeContext;
}

portals::profile_ptr Portal::getProfile(const shared_ptr<IPortalDatabase> database, const ObjectID &id)
{
	//OS_ASSERT(id.getObjectType() == portalObjectTypeUser);
	//return getSnapshotProfile()->getProfiles()->get(id, this);
	return getSnapshotManager()->getProfiles()->myGet(database, id);
}

shared_ptr<EntitiesEntity> Portal::getEntity(const shared_ptr<IPortalDatabase> database, const EntityID &id, bool force)
{
	return getSnapshotManager()->getEntity(database, id);
}

shared_ptr<ObjectsIObject> Portal::getObject(shared_ptr<IPortalDatabase> database, const ObjectID &id)
{
	// URGENT: in fase di chiusura è possibile una chiamata a questa funzione successiva a "_clearManagers", da verificare...
	if(m_objects == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	return m_objects->get(id, database.get());
}

/*
shared_ptr<ObjectsIObject> Portal::getObject(shared_ptr<IPortalDatabase> database, const String &id)
{
	return getObject(database, ObjectID(id.to_ascii()));
}
*/

ObjectID Portal::getLastExchangedObject() const
{
	if(m_options != null)
		return m_options->getLastExchangedObject();

	return ObjectID::EMPTY;
}

void Portal::setLastExchangedObject(const ObjectID &lastExchangedObject)
{
    if(m_options == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	m_options->setLastExchangedObject(lastExchangedObject);
}

String Portal::getSync() const 
{ 
	return m_options->getOptionValue(PortalOptions::options::sync); 
}

void Portal::setSync(const String &sync) 
{ 
	m_options->setOptionValue(PortalOptions::options::sync, sync); 

	// Run, other runs automatically after syncronization.
	if( (sync.empty() == false) && (getSnapshotManager()->getStabilityStatus() == EntitiesSnapshotManager::ssDone) )
		runJobSync();
}

/*
std::string Portal::getLink(ide::portal::PortalPage page, const ordered_map<std::wstring, std::wstring> &params)
{
	return PortalsSystem::instance()->getPortalLink(get_this_ptr(), page, params);
}
*/

std::string Portal::getLink(const std::string &page, const ordered_map<std::wstring, std::wstring> &params)
{
	return PortalsSystem::instance()->getPortalLink(get_this_ptr(), page, params);
}

std::string Portal::getViewObjectLink(const EntityID &id, const ordered_map<std::wstring, std::wstring> &params)
{
	return PortalsSystem::instance()->getViewObjectLink(get_this_ptr(), id.toUTF16(), params);
}

std::string Portal::getEditObjectLink(const EntityID &id, const ordered_map<std::wstring, std::wstring> &params)
{
	return PortalsSystem::instance()->getEditObjectLink(get_this_ptr(), id.toUTF16(), params);
}

std::string Portal::getUserLink(const ObjectID &id)
{
	return PortalsSystem::instance()->getUserLink(get_this_ptr(), id.toUTF16());
}

std::string Portal::getSendMessageLink(const ObjectID &id, const ordered_map<std::wstring, std::wstring> &params)
{
	return PortalsSystem::instance()->getSendMessageLink(get_this_ptr(), id.toUTF16(), params);
}

std::string Portal::getAvatarLink(const ObjectID &id, const DateTime &dt)
{
	return PortalsSystem::instance()->getAvatarLink(get_this_ptr(), id.toUTF16(), dt);
}

std::string Portal::getFileLink(const ObjectID &id)
{
	return PortalsSystem::instance()->getFileLink(get_this_ptr(), id.toUTF16());
}

std::string Portal::getPrivateMessageLink(const ObjectID &id, bool secure)
{
	return ide::portal::Account::getViewMessageLink(get_this_ptr(), id, secure);
}

void Portal::setCachesSize(uint32 size)
{
	// Aggiorna la dimensione della cache degli oggetti
	m_objects->setLimit(size);

	getSnapshotManager()->setCachesSize(size);
}

void Portal::setDatabasesCacheSize(uint32 size)
{
    OS_LOCK(m_databasesCacheCS);
    m_databasesCache->setOptimalSize(size);
}

/*
String Portal::getBootstrapUrl(bool isNotify) const
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_ISIS_PAGE_BOOTSTRAP_PARAM_PORTAL, getPortalID().toWide());
	params.set(OS_ISIS_PAGE_BOOTSTRAP_PARAM_POV, getPovID().toWide());
	params.set(OS_ISIS_PAGE_BOOTSTRAP_PARAM_PEERS, conversions::to_wstring<uint32>(isNotify ? 0 : OS_ISIS_DEFAULT_PEERS_REQUEST));
	// Notifica la porta di listening solo se non è settato un proxy
	if(Options::instance()->getOption<uint32>(Options::net_options::proxy_type) == OS_PROXY_TYPE_NONE)
		params.set(OS_ISIS_PAGE_BOOTSTRAP_PARAM_PORT, conversions::to_wstring(Options::instance()->getOption<uint32>(Options::p2p_options::server_port)));

	return Options::instance()->getIsisSecureLink(OS_ISIS_PAGE_BOOTSTRAP, params);
}
*/

boost::recursive_mutex & Portal::getPortalLock() const
{
	// TODO: da abolire
	return m_portalCS;
}

uint32 Portal::getDatabaseExclusiveRequests() const
{
	return m_lockData->getExclusiveRequests();
}

void Portal::setLoaded(bool loaded)
{
	OS_LOCK(m_dataCS);
	m_loaded = loaded;
}

std::string Portal::generatePovID(const PortalID &portalID, const PovID &povID)
{	
	//return portalID.getString() + "_" + userID.getString();	
	//return utils::generateHashMerge(portalID.toUTF16().to_wide(), userID.toUTF16().to_wide(), true); 

	// Indeciso.
	// V1:
	//String key = portalID.toUTF16() + userID.toUTF16();
	//return CryptManager:: instance()->SHA(key.buffer(), key.buffer_size()).toHex();

	// V2:
	return portalID.getString() + "_" + povID.getString();	
}

void Portal::reloadUser(const shared_ptr<IPortalDatabase> &database)
{
	OS_LOCK(m_portalCS);

	m_user = objects_user_cast(getObject(database, getPovUserID()));

	getOptionsShared()->reload(database);
}

// This save the user. If the private key isn't of the POV user, fail.
bool Portal::saveUser(const shared_ptr<IPortalDatabase> &database, shared_ptr<IdeSession> ideSession)
{
	bool success = false;

	if(ideSession->isLogged() == false)
		return false;
	if(ideSession->getUserID() != database->getPortal()->getPovUserID())
		return false;

	if(getPovUser())
	{
		String data;
		if(getOptionsShared()->writeString(data))
		{
			getPovUser()->options = data;
			LanguageResult result = database->_signAndUpdate(getPovUser(), ideSession->getPrivateKey(), true);
			if(result.empty())
			{
				success = true;

				reloadUser(database);
			}
		}
	}

	if(success == false)
	{
		// Nota: Razor, Qui ci finisce se un'altro account non del POV tenta di salvare opzioni.
		// Testare che non ci sia riuscito, ovviamente.
		// Ci finisce anche se le opzioni non sono validate. Sta a "ACP" evitare che arrivi qui.
		OS_ASSERT(false);
		OS_LOG_ERROR(_S("Cannot sign the user."));
	}

	return success;
}


void Portal::runJobStabilize()
{
	if(m_stabilizePending == false)
	{
		m_stabilizePending = true;
		Engine::instance()->addBackgroundJob(shared_ptr<PortalsStabilityJob>(OS_NEW PortalsStabilityJob(Engine::instance()->peekBackgroundJobID(), get_this_ptr())));
	}
}

void Portal::runJobOptimizer()
{
	Engine::instance()->addBackgroundJob(shared_ptr<PortalsOptimizerJob>(OS_NEW PortalsOptimizerJob(Engine::instance()->peekBackgroundJobID(), get_this_ptr())));
}

void Portal::runJobSync()
{
	Engine::instance()->addBackgroundJob(shared_ptr<PortalsSyncJob>(OS_NEW PortalsSyncJob(Engine::instance()->peekBackgroundJobID(), get_this_ptr())));
}

void Portal::runJobImporter(const String &url)
{
	// Note 29/08/2011: Job creation was in "importer.py". But, if i created it from python, during execution of the job it maintain a PythonState lock, that cause locks problems.
	uint32 jobID = Engine::instance()->peekBackgroundJobID();

	shared_ptr<PortalsImporterJob> job(OS_NEW PortalsImporterJob(jobID, get_this_ptr(), url));

	Engine::instance()->addBackgroundJob(job);
}


void Portal::save()
{
	if(m_options != null)
		m_options->writeToPath(getPath());
}

void Portal::start()
{
	runJobStabilize();	
}

bool Portal::load(const String &path, shared_ptr<PortalOptions> options, bool recovery)
{
	if(getLoaded())
		return false;

	OS_ASSERT(path.empty() == false);
	m_path = path;

	OS_ASSERT(options != null);
	m_options = options;

	// Inizializza le variabili
	_initVariables();

	NotificationsManager::instance()->notify(String::format(_S("Loading portal %S").c_str(), getName().c_str()));

	// Apre la connessione al database
	shared_ptr<PortalsTransaction> databaseTransaction = startTransaction(true);
	if(databaseTransaction == null)
	{
		OS_LOG_ERROR(String::format(_S("Error opening portal's database of '%S'").c_str(), getName().c_str()));
		return false;
	}

	shared_ptr<IPortalDatabase> database = databaseTransaction->getDatabase();
	database->beginTransaction();

	bool needInvalidate = false;
	bool needOptimize = false;

	// Controlla se  necessario aggiornare il database
	if(m_options->getDatabaseVersion() != OS_PORTAL_DB_VERSION)
	{
		// Effettua l'aggiornamento del database
		if(_updateDatabase(database, m_options->getDatabaseVersion(), OS_PORTAL_DB_VERSION) == false)
			return false;

		needOptimize = true;
		needInvalidate = true;

		// Aggiorna le opzioni del db
		m_options->setDatabaseVersion(OS_PORTAL_DB_VERSION);
		m_options->writeToPath(m_path);
	}

	if(needOptimize || recovery)
	{
		database->commit(); // Le operazioni sotto-citate devono essere senza una transazione.

		NotificationsManager::instance()->notify(String::format(_S("Optimizing portal %S").c_str(), getName().c_str()));
		database->getConnection()->vacuum();

		NotificationsManager::instance()->notify(String::format(_S("Reindexing portal %S").c_str(), getName().c_str()));
		database->getConnection()->reindex();

		NotificationsManager::instance()->notify(String::format(_S("Analyzing portal %S").c_str(), getName().c_str()));
		database->getConnection()->analyze();

		database->beginTransaction();
	}

	// Notifica l'evento alle extensions
	ExtensionsSystem::instance()->notifyPortalLoaded(get_this_ptr(), database);
	PortalsSystem::instance()->notifyPortalLoaded(get_this_ptr(), database);

	// Inizializza i gestori del portale
	_createManagers(database);

	if(recovery)
	{
		NotificationsManager::instance()->notify(String::format(_S("Destroying snapshot profiles of portal %S").c_str(), getName().c_str()));

		getSnapshotManager()->ensureSnapshot(database, true);		

		// Qui ci vorrebbero controlli di integrità, tipo
		// delete from os_texts where id not in (select id from os_entries where type=x)

		// E' importante che ci sia a monte il primo giro, quindi due "optimize" in tutto, per risolvere
		// eventuali corruzioni prima di eseguire altre select/update
		database->commit();

		NotificationsManager::instance()->notify(String::format(_S("Optimizing portal %S").c_str(), getName().c_str()));
		database->getConnection()->vacuum();

		database->beginTransaction();

		needInvalidate = true;
	}

	if(needInvalidate)
	{
		// Distruzione dei profili
		NotificationsManager::instance()->notify(String::format(_S("Destroying snapshot profiles of portal %S").c_str(), getName().c_str()));
		getSnapshotManager()->ensureSnapshot(database, true);		
	}

	reloadUser(database);
		
	getPeersManager()->loadPeers(database);

	database->commit();

	setLoaded(true);

	return true;
}

bool Portal::openDatabase(shared_ptr<IPortalDatabase> &database)
{
	if(m_options == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<IDbDriver> driver = DatabasesSystem::instance()->getDriver(m_options->getDatabaseDriver().to_ascii());
	if(driver == null)
		return false;

	shared_ptr<IDbConnection> connection = driver->createConnection(m_options->getDatabaseName().to_ascii(), m_options->getDatabaseOptions());
	if(connection == null)
		return false;

	database.reset(OS_NEW DatabaseImpl(get_this_ptr(), connection));
	return database->open(get_this_ptr());
}


/* // TOCLEAN
String Portal::createAccount(shared_ptr<IPortalDatabase> database, shared_ptr<DataAccount> account)
{
	OS_LOCK(m_portalCS);

	OS_ASSERT(account != null);

	if(getAccountsManager()->exists(account->id))
	{
		OS_ASSERTFALSE();
		return String::EMPTY;
	}

	// Salva l'account
	if(database->insertRecord(account) == false)
	{
		OS_ASSERTFALSE();
		return String::EMPTY;
	}

	// Nota: la notifica di creazione dell'account non pu essere gestita dal database
	// perch in quel momento l'utente associato all'account non  ancora stato creato (si potrebbe in alternativa
	// creare prima l'utente e nel caso rimuoverlo, ma si rischia il pericolo che nel frattempo venga trasmesso...)

	m_accountsManager->add(database, account->id);
	getSnapshotManager()->onAccountCreated(database, account->id);

	return account->id;
}

bool Portal::removeAccount(shared_ptr<IPortalDatabase> database, const String &id)
{
	OS_LOCK(m_portalCS);

	shared_ptr<IdeAccount> account = m_accountsManager->getByID(id);
	if(account == null)
		return false;

	account->setDeleted(true);
	account->updateAccount(database);

	// Effettua il logout di tutti gli utente connessi con l'account da rimuovere
	_logoutUsers(database, id);

	// Nota: la notifica di rimozione dell'account si potrebbe spostare anche sotto il database,
	// sono qui solo per coerenza con "createAccount"

	m_accountsManager->remove(id);

	return true;
}

bool Portal::deleteAccount(shared_ptr<IPortalDatabase> database, const String &id) // TOCLEAN
{
	OS_LOCK(m_portalCS);

	// Effettua la rimozione dell'account
	if(database->xxx_removeAccount(id) == false)
		return false;

	getSnapshotManager()->onAccountDeleted(database, id);

	return true;
}
*/

/*
bool Portal::updateIsisStatus()
{
	if(needBootstrapRequest() == false) // TOFIX: Remove "bootstrap" as term, change options. Probably is better to deprecate "notify only".
		return false;    

	OS_LOCK(m_netCS);

	DateTime now = DateTime::now();

	// Salva la data di bootstrap
	m_lastBootstrapRequest = now;
	// La richiesta di bootstrap implica anche la notifica
	m_lastBootstrapNotify = now;
	// "Resetta" la data di ultimo scambio
	m_lastSuccessfulExchange = now;

	shared_ptr<boost::asio::io_service> service = createAsioService();
	if(service == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	String url = IsisSystem::instance()->getPortalUrl(get_this_ptr());
	if(document->parseUrl(url.to_ascii(), Engine::getDefaultHttpUserAgent(), service, Engine::instance()->createTCPSocket(service, true, true)) == false)
	{
		OS_LOG_ERROR(_S("Bootstrap error"));
		return false;
	}

	shared_ptr<XMLNode> root = document->getRoot();

	if(root->getName() == _S("error"))
	{
		String message = root->getAttributeString(_S("msg"));
		if(message.empty())
			message = _S("Bootstrap error");

		OS_LOG_ERROR(message);
		return false;
	}

	if(root->getName() != _S("response"))
	{
		OS_LOG_ERROR(_S("Invalid bootstrap response"));
		return false;
	}
		
	// Nodes
	{
		bool foundNodes = false;

        shared_ptr<XMLNode> ips = root->getNode(_S("ips"));
        if(ips != null)
        {
            shared_ptr<XMLNodes> ips_list = ips->getNodes();
            for(XMLNodes::const_iterator i = ips_list->begin(); i != ips_list->end(); ++i)
            {
                String ip = (*i)->getAttributeString(_S("address"));

                foundNodes = true;

                uint32 port = 0;
                shared_ptr<XMLAttribute> portAttribute = (*i)->getAttribute(_S("port"));
                if(portAttribute != null)
                    port = conversions::from_utf16<uint32>(portAttribute->getValue());

                if(port == 0)
                    port = OS_P2P_OLD_DEFAULT_SERVER_PORT;

                getPeersManager()->savePeer(ip, port, false, OS_PEER_ORIGIN_ISIS);
            }
        }

        if(foundNodes)
            OS_LOG_NOTICE(getName() + _S(": bootstrap success"));
        else
            OS_LOG_NOTICE(getName() + _S(": bootstrap success, cannot find any node"));
    }

	return true;
}
*/

/*
void Portal::ensureBootstrap() // TOCLEAN, updateIsisStatus above is new edition.
{
    OS_LOCK(m_netCS);

    if(needBootstrapRequest())
        // Effettua una richiesta di bootstrap
        bootstrap(false);
    else if(needBootstrapNotify())
        // Effettua una notifica di bootstrap
        bootstrap(true);
}

bool Portal::bootstrap(bool isNotify) // TOCLEAN, updateIsisStatus above is new edition.
{
	OS_LOCK(m_netCS);

	DateTime now = DateTime::now();

	// Salva la data di bootstrap
	m_lastBootstrapRequest = now;
	// La richiesta di bootstrap implica anche la notifica
	m_lastBootstrapNotify = now;
	// "Resetta" la data di ultimo scambio
	m_lastSuccessfulExchange = now;

	shared_ptr<boost::asio::io_service> service = createAsioService();
	if(service == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	String url = getBootstrapUrl(isNotify);
	if(document->parseUrl(url.to_ascii(), Engine::getDefaultHttpUserAgent(), service, Engine::instance()->createTCPSocket(service, true, true)) == false)
	{
		OS_LOG_ERROR(_S("Bootstrap error"));
		return false;
	}

	shared_ptr<XMLNode> root = document->getRoot();

	if(root->getName() == _S("error"))
	{
		String message = root->getAttributeString(_S("msg"));
		if(message.empty())
			message = _S("Bootstrap error");

		OS_LOG_ERROR(message);
		return false;
	}

	if(root->getName() != _S("response"))
	{
		OS_LOG_ERROR(_S("Invalid bootstrap response"));
		return false;
	}

    if(isNotify)
    {
        // N.B.: in caso di notifica non salva eventuali indirizzi ip restituiti dal server

        OS_LOG_NOTICE(getName() + _S(": bootstrap notify success"));
    }
    else
    {
        bool foundNodes = false;

        shared_ptr<XMLNode> ips = root->getNode(_S("ips"));
        if(ips != null)
        {
            shared_ptr<XMLNodes> ips_list = ips->getNodes();
            for(XMLNodes::const_iterator i = ips_list->begin(); i != ips_list->end(); ++i)
            {
                String ip = (*i)->getAttributeString(_S("address"));

                foundNodes = true;

                uint32 port = 0;
                shared_ptr<XMLAttribute> portAttribute = (*i)->getAttribute(_S("port"));
                if(portAttribute != null)
                    port = conversions::from_utf16<uint32>(portAttribute->getValue());

                if(port == 0)
                    port = OS_P2P_OLD_DEFAULT_SERVER_PORT;

                getPeersManager()->savePeer(ip, port, false, OS_PEER_ORIGIN_ISIS);
            }
        }

        if(foundNodes)
            OS_LOG_NOTICE(getName() + _S(": bootstrap success"));
        else
            OS_LOG_NOTICE(getName() + _S(": bootstrap success, cannot find any node"));
    }

	return true;
}
*/

void Portal::updateLastSuccessfulExchange()
{
	OS_LOCK(m_netCS);
	m_lastSuccessfulExchange = DateTime::now();
}

bool Portal::validateUsername(const String &username)
{
	// URGENT: da spostare/pulire

    std::wstring str = username.to_wide();
	if(str.length() < OS_PORTAL_MIN_USERNAME_LENGTH)
		return false;

	if(str != algorithms::trim_copy(str))
		return false;

	OS_ASSERT(str.empty() == false);
	for(std::wstring::const_iterator i = str.begin(); i != str.end(); ++i)
	{
		if(iswalnum(*i))
			continue;

		if(i != str.begin())
		{
				if(((*i) == L' ') || ((*i) == L'_') || ((*i) == L'.') || ((*i) == L'-') || ((*i) == L'[') || ((*i) == L']') || ((*i) == L'/') || ((*i) == L'\"'))
					continue;
		}

		return false;
	}

	return true;
}


std::string Portal::generateInviteLink(bool http) const
{
	shared_ptr<PortalOptions> options = getOptions();
	if(options == null)
	{
		OS_ASSERTFALSE();
		return "";
	}

	/*
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_PORTAL_LINK_PARAM_NAME, options->getName().to_utf8());
	params.set(OS_PORTAL_LINK_PARAM_DESCRIPTION, options->getDescription().to_utf8());		
	
	
	String link = ShellLinkPortal::generate(getPortalID(), getUserID());
	*/

	OsirisLink link;
	link.setParam("type", _W("portal"));
	link.setParam("portal", getPortalID().toWide());
	link.setParam("pov", getPovID().toWide());
	link.setParam("name", getName().to_wide());	
	/*
	if(getPortalID().isMonarchic() == false)
		link.setParam("user", user.getString());
	*/
	
	std::string linkHref = link.generate();
	
	if(http)
		linkHref = Options::instance()->getIsisLink(OS_ISIS_PAGE_INVITELINK, "url=" + HttpParser::urlEncodeA(linkHref));

	return linkHref;
}

String Portal::generateExportLink() const
{
	ordered_map<std::wstring, std::wstring> params;

	params.set(OS_URL_PARAM_POV, utils::atow(getFullPovID()));
	//params.set(OS_URL_PARAM_PORTAL, getPortalID().toWide());
	//params.set(OS_URL_PARAM_USER, getUserID().toWide());

	return PortalsSystem::instance()->getMainLink("export", params);
}

void Portal::unload()
{
	if(getLoaded() == false)
		return;

	// Resetta i managers del portale
	_clearManagers();

	// Salva le opzioni
	save();

	bool deleted = m_options->getDeleted();
	// Se il portale è stato cancellato, evita di salvare i nodi conosciuti
	if(deleted == false)
	{
		shared_ptr<PortalsTransaction> databaseTransaction = startTransaction(true);
		shared_ptr<IPortalDatabase> database = databaseTransaction->getDatabase();
		database->beginTransaction();

		if(getPeersManager()->savePeers(database))
			database->commit();
		else
			database->rollback();
	}

	// N.B.: la cache deve essere resettata prima del check sulla cancellazione del portale (altrimenti potrebbero rimanere delle connessioni appese e non sarebbe possibile rimuovere il database del portale)	e va mantenuta loccata in tale fase
	{
		OS_LOCK(m_databasesCacheCS);
		m_databasesCache->clear();

		if(m_options != null)
		{
			m_options->unload();

			// Controlla se il portale è stato cancellato
			if(deleted)
			{
				// Rimuove il portale
				PortalsSystem::instance()->deletePortal(m_options, m_path);
			}
		}
	}

	m_options.reset();
	m_path.clear();
	setLoaded(false);
}

bool Portal::_updateDatabase(shared_ptr<IPortalDatabase> database, const Version &fromVersion, const Version &toVersion)
{
	shared_ptr<DbSchemaSchema> schemaTo(OS_NEW DbSchemaSchema());
	if(schemaTo->parse(DatabasesSystem::instance()->getDatabasePath(OS_PORTAL_DB_TYPE, toVersion, true)) == false)
		return false;

	if(fromVersion > schemaTo->getPrevVersion())
		return false;

	if(schemaTo->getPrevVersion() != fromVersion)
	{
		if(_updateDatabase(database, fromVersion, schemaTo->getPrevVersion()) == false)
			return false;

		return _updateDatabase(database, schemaTo->getPrevVersion(), toVersion);
	}

	shared_ptr<DbSchemaSchema> schemaFrom(OS_NEW DbSchemaSchema());
	if(schemaFrom->parse(DatabasesSystem::instance()->getDatabasePath(OS_PORTAL_DB_TYPE, fromVersion, true)) == false)
		return false;

	DbSqlCommands commands;
	if(schemaFrom->generateDifferences(schemaTo, commands) == false)
		return false;

	if(onPreDatabaseVersionUpdate(database, fromVersion, toVersion) == false)
		return false;

	if(commands.empty() == false)
	{
		NotificationsManager::instance()->notify(String::format(_S("Upgrading %S's database (0%% completed)").c_str(), getName().c_str()));

		size_t current_command = 0;
		for(DbSqlCommands::const_iterator i = commands.begin(); i != commands.end(); ++i)
		{
			if(database->execute(*i) == false)
				return false;

			NotificationsManager::instance()->notify(String::format(_S("Upgrading %S's database (%d%% completed)").c_str(), getName().c_str(), static_cast<uint32>((++current_command * 100) / commands.size())));
		}
	}

	if(onPostDatabaseVersionUpdate(database, fromVersion, toVersion) == false)
		return false;

	return true;
}

/*
bool Portal::needBootstrapRequest()
{
	// Calcola il tempo minimo prima di effettuare una richiesta di bootstrap
	TimeDuration requestDelay = TimeDuration::minutes(Options::instance()->getOption<uint32>(Options::p2p_options::bootstrap_request_delay));
	if(requestDelay.isNull())
		return false;	// bootstrap disabilitato

	OS_LOCK(m_netCS);

	DateTime now = DateTime::now();

	// Controlla se sono presenti degli indirizzi ip
	if(getPeersManager()->empty() == false)
	{
		// N.B.: è possibile che nonostante nel db siano presenti degli indirizzi ip la data di ultimo contatto terminato
		// con successo superi il delta previsto nel il bootstrap (per esempio perch i nodi disponibili rispondono sempre
		// che sono in stato di busy, capita...), pertanto in tale circostanza bisogna comunque effettuare un bootstrap

		DateTime exchangeCheckDate = std::max(now, m_lastSuccessfulExchange);
		if((exchangeCheckDate - m_lastSuccessfulExchange) < requestDelay)
			return false;	// Nel db sono presenti dei nodi e la data di ultimo contatto terminato con successo è al di sotto del delta
	}

	DateTime requestCheckDate = std::max(now, m_lastBootstrapRequest);
	// Controlla se l'ultima richiesta  precedente al tempo minimo previsto
	return (requestCheckDate - m_lastBootstrapRequest) >= requestDelay;
}

bool Portal::needBootstrapNotify()
{
	// N.B.: il bootstrap, anche nel caso di notifica, deve effettuare il download degli indirizzi ip, perchè
	// potrebbero esserci situazioni in cui sia necessario rigirare indirizzi ip a nodi isolati che non stanno
	// effettuando update di bootstrap, es:
	// A (passivo, update abilitato) <-> B (attivo, update disabilitato), arriva C (attivo). Se A effettua l'update
	// ma non scarica l'indirizzo ip di C, quest'ultimo non si potrà mai mettere in comunicazione con B (in caso contrario A
	// comunicherebbe a B l'indirizzo di C)

	// Calcola il tempo minimo prima di effettuare una notifica di bootstrap
	TimeDuration notifyDelay = TimeDuration::minutes(Options::instance()->getOption<uint32>(Options::p2p_options::bootstrap_notify_delay));
	if(notifyDelay.isNull())
		return false;

	OS_LOCK(m_netCS);

	// Calcola l'ora attuale
	DateTime now = std::max(DateTime::now(), m_lastBootstrapNotify);
	// Controlla se l'ultima richiesta  precedente al tempo minimo previsto
	return (now - m_lastBootstrapNotify) >= notifyDelay;
}
*/

void Portal::_initVariables()
{
	// N.B.: la data di notifica di bootstrap deve essere inizializzata, mentre quella di richiesta invece no in modo tale
	// che se sono gi disponibili degli indirizzi ip validi sfrutta quelli ed evita di fare un bootstrap

	DateTime now = DateTime::now();

	//m_lastBootstrapNotify = now;
	m_lastSuccessfulExchange = now;		// Inizializza la data di ultimo scambio
}

void Portal::_createManagers(shared_ptr<IPortalDatabase> database)
{
	m_objects.reset(OS_NEW ObjectsObjectsCache());
	m_reputationsManager.reset(OS_NEW ReputationsManager());
	m_messenger.reset(OS_NEW portals::Messenger(get_this_ptr()));
	m_snapshotManager.reset(OS_NEW EntitiesSnapshotManager(get_this_ptr(), database));
	m_optionsShared.reset(OS_NEW PortalsOptionsShared(get_this_ptr()));	
}

void Portal::_clearManagers()
{
	m_objects.reset();
	m_messenger.reset();
	
	if(m_snapshotManager != null)
	{
		m_snapshotManager->unload();
		m_snapshotManager.reset();
	}

	if(m_optionsShared != null)
	{
		m_optionsShared->unload();
		m_optionsShared.reset();
	}
}

bool Portal::onPreDatabaseVersionUpdate(shared_ptr<IPortalDatabase> database, const Version &fromVersion, const Version &toVersion)
{
	OS_ASSERT(database != null);

	/*
	if(toVersion == OS_MAKEVERSION(3, 0))
	{
		String patch;
		patch.append(_S("delete from os_votes;"));
		patch.append(String::format(_S("delete from os_entries where type=%d;"),portalObjectTypeVote));

		NotificationsManager::instance()->notify(String::format(_S("patching %S's database..."), getName().c_str()));
		if(m_database->execute(patch) == false)
			return false;
	}
	*/

	return true;
}

bool Portal::onPostDatabaseVersionUpdate(shared_ptr<IPortalDatabase> database, const Version &fromVersion, const Version &toVersion)
{
	OS_ASSERT(database != null);

#ifdef OS_TODOCIP
	if(toVersion == OS_MAKEVERSION(0, 1))
	{
		NotificationsManager::instance()->notify(String::format(_S("Patching %S's database...").c_str(), getName().c_str()));

		CompatibilityManager::instance()->razorPortalDatabaseUpgrade(database);
	}
#endif
	

	if(toVersion == OS_MAKEVERSION(6, 0))
	{
		String patch;
		patch.append(String::format(_S("update os_snapshot_objects set search_date = '%S' where search_date is null;").c_str(),DateTime::EMPTY.toString().c_str()));
		patch.append(_S("update os_snapshot_profiles set initialized=0;"));

		NotificationsManager::instance()->notify(String::format(_S("Patching %S's database...").c_str(), getName().c_str()));
		if(database->execute(patch) == false)
			return false;
	}

	if(toVersion == OS_MAKEVERSION(7, 0))
	{
		String patch;
        patch.append(_S("update os_entries set rank=0;"));

		// ------------------------------
		/*
		C'è un bel bordello con gli xml del db...
		Guarda questi 2 estratti degli indici sulla os_entries.

		Soluzione:
		- qui droppo tutti gli indici delle vecchie versioni e nuove
		- poi sempre qui metto la creazione di tutti gli indici (prendendo le sql da un db 0.11 pulito).

-- un DB 0.10 migrato alla 0.11
index	os_entries_author	os_entries	107	CREATE INDEX os_entries_author ON os_entries(author)
index	os_entries_index_rank_submit_date	os_entries	191	CREATE INDEX os_entries_index_rank_submit_date on os_entries (rank asc, submit_date desc)
index	os_entries_index_submit_date	os_entries	190	CREATE INDEX os_entries_index_submit_date on os_entries (submit_date asc)
index	os_entries_parent	os_entries	104	CREATE INDEX os_entries_parent ON os_entries(parent)
index	os_entries_rank	os_entries	109	CREATE INDEX os_entries_rank ON os_entries(rank)
index	os_entries_revision	os_entries	106	CREATE INDEX os_entries_revision ON os_entries(revision)
index	os_entries_type	os_entries	108	CREATE INDEX os_entries_type ON os_entries(type)

- un DB 0.11 pulito
index	os_entries_index_author	os_entries	46	CREATE INDEX os_entries_index_author on os_entries (author asc)
index	os_entries_index_parent	os_entries	43	CREATE INDEX os_entries_index_parent on os_entries (parent asc)
index	os_entries_index_rank_submit_date	os_entries	49	CREATE INDEX os_entries_index_rank_submit_date on os_entries (rank asc, submit_date desc)
index	os_entries_index_revision	os_entries	44	CREATE INDEX os_entries_index_revision on os_entries (revision asc)
index	os_entries_index_submit_date	os_entries	47	CREATE INDEX os_entries_index_submit_date on os_entries (submit_date asc)
index	os_entries_index_type	os_entries	48	CREATE INDEX os_entries_index_type on os_entries (type asc)
		*/

		patch.append(_S("drop index if exists os_attributes_author;"));
		patch.append(_S("drop index if exists os_attributes_reference;"));
		patch.append(_S("drop index if exists os_attributes_type;"));
		patch.append(_S("drop index if exists os_avatars_author;"));
		patch.append(_S("drop index if exists os_calendar_events_index_author;"));
		patch.append(_S("drop index if exists os_calendar_events_index_model;"));
		patch.append(_S("drop index if exists os_calendar_events_index_parent;"));
		patch.append(_S("drop index if exists os_calendar_events_index_pinned;"));
		patch.append(_S("drop index if exists os_calendar_events_index_revision;"));
		patch.append(_S("drop index if exists os_entries_author;"));
		patch.append(_S("drop index if exists os_entries_index_rank_submit_date;"));
		patch.append(_S("drop index if exists os_entries_index_submit_date;"));
		patch.append(_S("drop index if exists os_entries_parent;"));
		patch.append(_S("drop index if exists os_entries_rank;"));
		patch.append(_S("drop index if exists os_entries_revision;"));
		patch.append(_S("drop index if exists os_entries_type;"));
		patch.append(_S("drop index if exists os_files_index_author;"));
		patch.append(_S("drop index if exists os_files_index_model;"));
		patch.append(_S("drop index if exists os_files_index_parent;"));
		patch.append(_S("drop index if exists os_files_index_pinned;"));
		patch.append(_S("drop index if exists os_files_index_revision;"));
		patch.append(_S("drop index if exists os_instances_author;"));
		patch.append(_S("drop index if exists os_instances_parent;"));
		patch.append(_S("drop index if exists os_instances_revision;"));
		patch.append(_S("drop index if exists os_local_messages_author;"));
		patch.append(_S("drop index if exists os_local_messages_read_date;"));
		patch.append(_S("drop index if exists os_local_messages_submit_date;"));
		patch.append(_S("drop index if exists os_models_index_author;"));
		patch.append(_S("drop index if exists os_models_index_model;"));
		patch.append(_S("drop index if exists os_models_index_parent;"));
		patch.append(_S("drop index if exists os_models_index_pinned;"));
		patch.append(_S("drop index if exists os_models_index_revision;"));
		patch.append(_S("drop index if exists os_peers_index_retries_rank;"));
		patch.append(_S("drop index if exists os_peers_rank;"));
		patch.append(_S("drop index if exists os_polls_index_author;"));
		patch.append(_S("drop index if exists os_polls_index_model;"));
		patch.append(_S("drop index if exists os_polls_index_parent;"));
		patch.append(_S("drop index if exists os_polls_index_pinned;"));
		patch.append(_S("drop index if exists os_polls_index_revision;"));
		patch.append(_S("drop index if exists os_polls_options_author;"));
		patch.append(_S("drop index if exists os_polls_options_parent;"));
		patch.append(_S("drop index if exists os_polls_options_revision;"));
		patch.append(_S("drop index if exists os_polls_votes_author;"));
		patch.append(_S("drop index if exists os_polls_votes_parent;"));
		patch.append(_S("drop index if exists os_polls_votes_reference;"));
		patch.append(_S("drop index if exists os_posts_author;"));
		patch.append(_S("drop index if exists os_posts_parent;"));
		patch.append(_S("drop index if exists os_posts_reference;"));
		patch.append(_S("drop index if exists os_posts_revision;"));
		patch.append(_S("drop index if exists os_private_messages_adressee;"));
		patch.append(_S("drop index if exists os_private_messages_author;"));
		patch.append(_S("drop index if exists os_reputations_author;"));
		patch.append(_S("drop index if exists os_reputations_follow;"));
		patch.append(_S("drop index if exists os_reputations_index_author_follow_score;"));
		patch.append(_S("drop index if exists os_reputations_reference;"));
		patch.append(_S("drop index if exists os_reputations_score;"));
		patch.append(_S("drop index if exists os_sections_author;"));
		patch.append(_S("drop index if exists os_sections_component;"));
		patch.append(_S("drop index if exists os_sections_parent;"));
		patch.append(_S("drop index if exists os_sections_revision;"));
		patch.append(_S("drop index if exists os_snapshot_objects_depth;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_depth;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_insert_date;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_parent;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_parent_current;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_position;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_search_date;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_section;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_stability_date;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_stats_date;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_type;"));
		patch.append(_S("drop index if exists os_snapshot_objects_index_profile_visible;"));
		patch.append(_S("drop index if exists os_snapshot_objects_insert_date;"));
		patch.append(_S("drop index if exists os_snapshot_objects_parent;"));
		patch.append(_S("drop index if exists os_snapshot_objects_position;"));
		patch.append(_S("drop index if exists os_snapshot_objects_profile;"));
		patch.append(_S("drop index if exists os_snapshot_objects_section;"));
		patch.append(_S("drop index if exists os_snapshot_objects_stability_date;"));
		patch.append(_S("drop index if exists os_snapshot_objects_stats_date;"));
		patch.append(_S("drop index if exists os_snapshot_objects_type;"));
		patch.append(_S("drop index if exists os_snapshot_objects_visible;"));
		patch.append(_S("drop index if exists os_snapshot_users_index_profile_reference_score;"));
		patch.append(_S("drop index if exists os_snapshot_users_index_profile_score;"));
		patch.append(_S("drop index if exists os_snapshot_users_index_profile_stability_date;"));
		patch.append(_S("drop index if exists os_snapshot_users_index_profile_stats_date;"));
		patch.append(_S("drop index if exists os_snapshot_users_profile;"));
		patch.append(_S("drop index if exists os_snapshot_users_score;"));
		patch.append(_S("drop index if exists os_snapshot_users_stability_date;"));
		patch.append(_S("drop index if exists os_snapshot_users_stats_date;"));
		patch.append(_S("drop index if exists os_tags_author;"));
		patch.append(_S("drop index if exists os_tags_name;"));
		patch.append(_S("drop index if exists os_tags_parent;"));
		patch.append(_S("drop index if exists os_tags_revision;"));
		patch.append(_S("drop index if exists os_texts_author;"));
		patch.append(_S("drop index if exists os_texts_model;"));
		patch.append(_S("drop index if exists os_texts_parent;"));
		patch.append(_S("drop index if exists os_texts_pinned;"));
		patch.append(_S("drop index if exists os_texts_revision;"));
		patch.append(_S("drop index if exists os_users_join_date;"));
		patch.append(_S("drop index if exists os_users_name;"));
		patch.append(_S("drop index if exists os_votes_author;"));
		patch.append(_S("drop index if exists os_votes_reference;"));

		// Prendere le sql dalla sqlite_master di un DB 0.12 pulito con
		// select sql from sqlite_master where type="index" order by name
		// aggiungere via ultraedit " IF NOT EXISTS " e il ; finale
		// aggiungere "patch.append(_S("" e ""));" finale
		// e riportarle qui.
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_attributes_index_author on os_attributes (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_attributes_index_reference on os_attributes (reference asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_attributes_index_type on os_attributes (type asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_avatars_index_author on os_avatars (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_calendar_events_index_author on os_calendar_events (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_calendar_events_index_model on os_calendar_events (model asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_calendar_events_index_parent on os_calendar_events (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_calendar_events_index_pinned on os_calendar_events (pinned asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_calendar_events_index_revision on os_calendar_events (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_discussions_stats_index_pinned on os_discussions_stats (pinned asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_entries_index_author on os_entries (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_entries_index_parent on os_entries (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_entries_index_rank_submit_date on os_entries (rank asc, submit_date desc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_entries_index_revision on os_entries (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_entries_index_submit_date on os_entries (submit_date asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_entries_index_type on os_entries (type asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_files_index_author on os_files (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_files_index_model on os_files (model asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_files_index_parent on os_files (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_files_index_pinned on os_files (pinned asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_files_index_revision on os_files (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_instances_index_author on os_instances (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_instances_index_parent on os_instances (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_instances_index_revision on os_instances (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_local_messages_index_author on os_local_messages (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_local_messages_index_read_date on os_local_messages (read_date asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_local_messages_index_submit_date on os_local_messages (submit_date asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_models_index_author on os_models (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_models_index_model on os_models (model asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_models_index_parent on os_models (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_models_index_pinned on os_models (pinned asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_models_index_revision on os_models (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_index_author on os_polls (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_index_model on os_polls (model asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_index_parent on os_polls (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_index_pinned on os_polls (pinned asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_index_revision on os_polls (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_options_index_author on os_polls_options (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_options_index_parent on os_polls_options (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_options_index_revision on os_polls_options (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_votes_index_author on os_polls_votes (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_votes_index_parent on os_polls_votes (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_polls_votes_index_reference on os_polls_votes (reference asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_posts_index_author on os_posts (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_posts_index_parent on os_posts (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_posts_index_reference on os_posts (reference asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_posts_index_revision on os_posts (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_private_messages_index_adressee on os_private_messages (adressee asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_private_messages_index_author on os_private_messages (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_reputations_index_author on os_reputations (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_reputations_index_author_follow_score on os_reputations (author asc, follow asc, score asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_reputations_index_reference on os_reputations (reference asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_reputations_index_score on os_reputations (score asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_sections_index_author on os_sections (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_sections_index_parent on os_sections (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_sections_index_revision on os_sections (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_depth on os_snapshot_objects (profile asc, depth asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_insert_date on os_snapshot_objects (profile asc, insert_date asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_parent on os_snapshot_objects (profile asc, parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_parent_type on os_snapshot_objects (profile asc, parent asc, type asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_position on os_snapshot_objects (profile asc, position asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_search on os_snapshot_objects (profile asc, search asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_section on os_snapshot_objects (profile asc, section asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_stability_date_depth on os_snapshot_objects (profile asc, stability_date asc, depth asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_stats_depth on os_snapshot_objects (profile asc, stats asc, depth desc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_type on os_snapshot_objects (profile asc, type asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_objects_index_profile_visible on os_snapshot_objects (profile asc, visible asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_users_index_profile_reference_score on os_snapshot_users (profile asc, reference asc, score asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_users_index_profile_score on os_snapshot_users (profile asc, score asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_users_index_profile_stability_date on os_snapshot_users (profile asc, stability_date asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_snapshot_users_index_profile_stats on os_snapshot_users (profile asc, stats asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_tags_index_author on os_tags (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_tags_index_name on os_tags (name asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_tags_index_parent on os_tags (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_tags_index_revision on os_tags (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_texts_index_author on os_texts (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_texts_index_model on os_texts (model asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_texts_index_parent on os_texts (parent asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_texts_index_pinned on os_texts (pinned asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_texts_index_revision on os_texts (revision asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_users_index_join_date on os_users (join_date asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_users_index_name on os_users (name asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_votes_index_author on os_votes (author asc);"));
		patch.append(_S("CREATE INDEX IF NOT EXISTS os_votes_index_reference on os_votes (reference asc);"));

		NotificationsManager::instance()->notify(String::format(_S("Patching %S's database...").c_str(), getName().c_str()));
		if(database->execute(patch) == false)
			return false;
	}

	return true;
}

shared_ptr<PortalsTransaction> Portal::startTransaction(bool exclusiveAccess)
{
	shared_ptr<IPortalDatabase> database;

	{
        OS_LOCK(m_databasesCacheCS);
        database = m_databasesCache->peekEntry();
	}

	if(database == null)
		return null;

	return startTransaction(database, exclusiveAccess);
}

shared_ptr<PortalsTransaction> Portal::startTransaction(shared_ptr<IPortalDatabase> database, bool exclusiveAccess)
{
	shared_ptr<PortalsTransaction> databaseTransaction(OS_NEW PortalsTransaction(m_lockData, database));

	databaseTransaction->setExclusiveAccess(exclusiveAccess);

	return databaseTransaction;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
