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
#include "portalsportalssystem.h"

#include "boost/bind.hpp"
#include "compatibilitymanager.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "iportaldatabase.h"
#include "dataaccount.h"
#include "dbdatabasessystem.h"
#include "idbconnection.h"
#include "idbdriver.h"
#include "dbschemaschema.h"
#include "engine.h"
#include "httpenumerateddirectorycallback.h"
#include "httpparser.h"
#include "httppath.h"
#include "httpsession.h"
#include "ideaccount.h"
#include "ideaccountsmanager.h"
#include "idesystem.h"
#include "idemainaccounts.h"
#include "idemainresources.h"
#include "idemainsettings.h"
#include "idepages.h"
#include "ideportalaccount.h"
#include "ideportaledit.h"
#include "ideportaluser.h"
#include "ideportalusers.h"
#include "ideportalview.h"
#include "ihttpserver.h"
#include "file.h"
#include "filesystem.h"
#include "isisservice.h"
#include "log.h"
#include "notificationsmanager.h"
#include "options.h"
#include "osirislink.h"
#include "platformmanager.h"
#include "portalspeersmanager.h"
#include "portalsiportalslistener.h"
#include "portalsmaindirectory.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalsdirectory.h"
#include "portalstransaction.h"
#include "upnpserver.h"
#include "utils.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//Razor
#include "idbresult.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SYSTEM(OS_NAMESPACE_NAME::PortalsSystem)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String PortalsSystem::MAIN_DIRECTORY = _S("main");
const String PortalsSystem::PORTALS_DIRECTORY = _S("portals");
const String PortalsSystem::TEMP_DIRECTORY = _S("temp");

const String PortalsSystem::PORTAL_PAGE_ACCOUNT = _S("account");
const String PortalsSystem::PORTAL_PAGE_EDIT = _S("edit");
const String PortalsSystem::PORTAL_PAGE_USER = _S("user");
const String PortalsSystem::PORTAL_PAGE_USERS = _S("users");
const String PortalsSystem::PORTAL_PAGE_VIEW = _S("view");
const String PortalsSystem::PORTAL_PAGE_OBJECTDETAIL = _S("objectDetail");
const String PortalsSystem::PORTAL_PAGE_ASSISTANT = _S("assistant");
const String PortalsSystem::PORTAL_PAGE_SEARCH = _S("search");
const String PortalsSystem::PORTAL_PAGE_MESSENGER = _S("privatemessages");

const String PortalsSystem::MAIN_PAGE_ACCOUNTS = _S("accounts2");
const String PortalsSystem::MAIN_PAGE_SETTINGS = _S("settings");
const String PortalsSystem::MAIN_PAGE_RESOURCES = _S("resources");
const String PortalsSystem::MAIN_PAGE_MAINTENANCE = _S("maintenance");
const String PortalsSystem::MAIN_PAGE_SUBSCRIBE = _S("subscribe2");
const String PortalsSystem::MAIN_PAGE_ISIS = _S("isis");
const String PortalsSystem::MAIN_PAGE_JOBDETAIL = _S("jobdetail");
const String PortalsSystem::MAIN_PAGE_JOBS = _S("jobs");
const String PortalsSystem::MAIN_PAGE_IMPORT = _S("import");
const String PortalsSystem::MAIN_PAGE_EXPORT = _S("export");
const String PortalsSystem::MAIN_PAGE_OPTIONS = _S("options");

//////////////////////////////////////////////////////////////////////

PortalsSystem::PortalsSystem() : m_isisService(OS_NEW IsisService()),
								 m_mainDirectory(OS_NEW portals::MainDirectory(MAIN_DIRECTORY)),
								 m_portalsDirectory(OS_NEW portals::PortalsDirectory(PORTALS_DIRECTORY)),
								 m_tempDirectory(OS_NEW HttpEnumeratedDirectoryCallback(TEMP_DIRECTORY))
{
	updateOptions();
	Options::instance()->subscribeUpdateHandler(boost::bind(&PortalsSystem::updateOptions, this));
}

PortalsSystem::~PortalsSystem()
{
	clearPortals();
}

uint32 PortalsSystem::getCacheSize() const
{
	OS_LOCK(m_cs);
	return m_cacheSize;
}

uint32 PortalsSystem::getPeersCacheSize() const
{
	OS_LOCK(m_cs);
	return m_peersCacheSize;
}

uint32 PortalsSystem::getDatabasesCacheSize() const
{
	OS_LOCK(m_cs);
	return m_databasesCacheSize;
}

PortalsSystem::Portals PortalsSystem::getPortals() const
{
	OS_LOCK(m_cs);
	return m_portals;
}

uint32 PortalsSystem::getPortalsCount() const
{
	OS_LOCK(m_cs);
	return static_cast<uint32>(m_portals.size());
}

/*
shared_ptr<Portal> PortalsSystem::getPortal(const PortalID &id) const
{
	OS_LOCK(m_cs);

	for(Portals::const_iterator i = m_portals.begin(); i != m_portals.end(); ++i)
	{
		if((*i)->getPortalID() == id)		
			return *i;
	}

	return null;
}
*/

shared_ptr<Portal> PortalsSystem::getPortal(const PortalID &id, const PovID &pov) const
{
	OS_LOCK(m_cs);

	for(Portals::const_iterator i = m_portals.begin(); i != m_portals.end(); ++i)
	{
		if( ((*i)->getPortalID() == id) && ((*i)->getPovID() == pov) )
			return *i;
	}

	return null;
}

shared_ptr<Portal> PortalsSystem::getPortalByFullPov(const String &id) const
{
	OS_LOCK(m_cs);

	for(Portals::const_iterator i = m_portals.begin(); i != m_portals.end(); ++i)
	{
		if((*i)->getFullPovID() == id)		
			return *i;
	}

	return null;
}

shared_ptr<Portal> PortalsSystem::getPortalByIndex(uint32 index) const
{
	OS_LOCK(m_cs);

	Portals::const_iterator i = utils::find_index(m_portals.begin(), m_portals.end(), index);
	if(i != m_portals.end())
		return *i;

	return null;
}

shared_ptr<Portal> PortalsSystem::getFirstPortal(const PortalID &id) const
{
	OS_LOCK(m_cs);

	for(Portals::const_iterator i = m_portals.begin(); i != m_portals.end(); ++i)
	{
		if((*i)->getPortalID() == id)
			return *i;
	}

	return null;
}

String PortalsSystem::getPath() const
{
	return utils::makeFolderPath(Options::instance()->getDataPath(), OS_PORTALS_PATH);
}

ide::portal::PortalPage PortalsSystem::getPortalPage(const String &name)
{
	if(name.compare_no_case(PORTAL_PAGE_ACCOUNT))
		return ide::portal::ppAccount;

	if(name.compare_no_case(PORTAL_PAGE_EDIT))
		return ide::portal::ppEdit;

	if(name.compare_no_case(PORTAL_PAGE_USER))
		return ide::portal::ppUser;

	if(name.compare_no_case(PORTAL_PAGE_USERS))
		return ide::portal::ppUsers;

	if(name.compare_no_case(PORTAL_PAGE_VIEW))
		return ide::portal::ppView;

	if(name.compare_no_case(PORTAL_PAGE_OBJECTDETAIL))
		return ide::portal::ppObjectDetail;

	if(name.compare_no_case(PORTAL_PAGE_ASSISTANT))
		return ide::portal::ppAssistant;

	if(name.compare_no_case(PORTAL_PAGE_SEARCH))
		return ide::portal::ppSearch;

	if(name.compare_no_case(PORTAL_PAGE_MESSENGER))
		return ide::portal::ppMessenger;

	return ide::portal::ppUnknown;
}

/*
String PortalsSystem::getPortalPage(const ide::portal::PortalPage &page)
{
	switch(page)
	{
	case ide::portal::ppAccount:		return PORTAL_PAGE_ACCOUNT;
	case ide::portal::ppEdit:			return PORTAL_PAGE_EDIT;
	case ide::portal::ppUser:			return PORTAL_PAGE_USER;
	case ide::portal::ppUsers:			return PORTAL_PAGE_USERS;
	case ide::portal::ppView:			return PORTAL_PAGE_VIEW;
	case ide::portal::ppObjectDetail:	return PORTAL_PAGE_OBJECTDETAIL;
	case ide::portal::ppAssistant:		return PORTAL_PAGE_ASSISTANT;
	case ide::portal::ppSearch:			return PORTAL_PAGE_SEARCH;

	case ide::portal::ppUnknown:		break;
	default:							OS_ASSERTFALSE();
										break;
	}

	return String::EMPTY;
}
*/
ide::main::MainPage PortalsSystem::getMainPage(const String &name)
{
	if(name.compare_no_case(MAIN_PAGE_ACCOUNTS))
		return ide::main::mpAccounts;

	if(name.compare_no_case(MAIN_PAGE_SETTINGS))
		return ide::main::mpSettings;

	if(name.compare_no_case(MAIN_PAGE_RESOURCES))
		return ide::main::mpResources;

	if(name.compare_no_case(MAIN_PAGE_MAINTENANCE))
		return ide::main::mpMaintenance;
	
	if(name.compare_no_case(MAIN_PAGE_SUBSCRIBE))
		return ide::main::mpSubscribe;

	if(name.compare_no_case(MAIN_PAGE_ISIS))
		return ide::main::mpIsis;

	if(name.compare_no_case(MAIN_PAGE_JOBDETAIL))
		return ide::main::mpJobDetail;

    if(name.compare_no_case(MAIN_PAGE_JOBS))
		return ide::main::mpJobs;

	if(name.compare_no_case(MAIN_PAGE_IMPORT))
		return ide::main::mpImport;

	if(name.compare_no_case(MAIN_PAGE_EXPORT))
		return ide::main::mpExport;

	if(name.compare_no_case(MAIN_PAGE_OPTIONS))
		return ide::main::mpOptions;

	return ide::main::mpUnknown;
}

/*
String PortalsSystem::getMainPage(const ide::main::MainPage &page)
{
	switch(page)
	{
	case ide::main::mpAccounts:	    return MAIN_PAGE_ACCOUNTS;
	case ide::main::mpSettings:	    return MAIN_PAGE_SETTINGS;
	case ide::main::mpResources:    return MAIN_PAGE_RESOURCES;
	case ide::main::mpMaintenance:  return MAIN_PAGE_MAINTENANCE;
	case ide::main::mpSubscribe:    return MAIN_PAGE_SUBSCRIBE;
	case ide::main::mpIsis:		    return MAIN_PAGE_ISIS;
	case ide::main::mpJobDetail:	return MAIN_PAGE_JOBDETAIL;
	case ide::main::mpJobs:			return MAIN_PAGE_JOBS;
	case ide::main::mpImport:		return MAIN_PAGE_IMPORT;
	case ide::main::mpExport:		return MAIN_PAGE_EXPORT;
	case ide::main::mpOptions:		return MAIN_PAGE_OPTIONS;

	case ide::main::mpUnknown:
                                    break;

	default:						OS_ASSERTFALSE();
                                    break;
	}

	return String::EMPTY;
}
*/

/*
std::string PortalsSystem::getMainLink(ide::main::MainPage page, const ordered_map<std::wstring, std::wstring> &params)
{
	return getMainLink(getMainPage(page).to_ascii(), params);
}
*/

std::string PortalsSystem::getMainLink(const std::string &page, const ordered_map<std::wstring, std::wstring> &params)
{
	return HttpParser::generateLink((_S("/") + m_mainDirectory->getName() + _S("/") + page).to_ascii(), params);
}

/*
std::string PortalsSystem::getPortalLink(const shared_ptr<Portal> &portal, ide::portal::PortalPage page, const ordered_map<std::wstring, std::wstring> &params)
{
	return getPortalLink(portal, getPortalPage(page).to_ascii(), params);
}
*/

std::string PortalsSystem::getPortalLink(const shared_ptr<Portal> &portal, const std::string &page, const ordered_map<std::wstring, std::wstring> &params)
{
	OS_ASSERT(portal != null);

	ordered_map<std::wstring, std::wstring> portalParams(params);

	portalParams.set(OS_URL_PARAM_PORTAL, utils::atow(portal->getFullPovID()));
	
	return HttpParser::generateLink((_S("/") + m_portalsDirectory->getName() + _S("/") + page).to_ascii(), portalParams);

	//return getPortalLink(portal->getPortalID(), portal->getUserID(), page, params);
}

/*
std::string PortalsSystem::getPortalLink(const PortalID &id, const ObjectID &user, const std::string &page, const ordered_map<std::wstring, std::wstring> &params)
{
	ordered_map<std::wstring, std::wstring> portalParams(params);

	portalParams.set(OS_URL_PARAM_PORTAL, id.toWide());
	portalParams.set(OS_URL_PARAM_USER, user.toWide());

	return HttpParser::generateLink((_S("/") + m_portalsDirectory->getName() + _S("/") + page).to_ascii(), portalParams);
}
*/

std::string PortalsSystem::getErrorLink(const String &message)
{
	OS_ASSERT(message.empty() == false);

	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_MESSAGE, message.to_wide());
	return getMainLink(OS_IDE_PAGE_ERROR, params);
}

std::string PortalsSystem::getRedirectLink(const std::string &url)
{
	OS_ASSERT(url.empty() == false);

	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_URL, utils::atow(url));
	return getMainLink(OS_IDE_PAGE_REDIRECT, params);
}

std::string PortalsSystem::getExternalLink(const std::string &url, bool confirm)
{
	if(url.empty())
		return constants::empty_string;

	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_URL, utils::atow(url));
	params.set(OS_URL_PARAM_CONFIRM, conversions::to_wstring(confirm));	
	return getMainLink(OS_IDE_PAGE_EXTERNAL, params);
}

std::string PortalsSystem::getLoginLink(const shared_ptr<Portal> &portal, const ordered_map<std::wstring, std::wstring> &params)
{
	ordered_map<std::wstring, std::wstring> portalParams(params);
	portalParams.set(OS_URL_PARAM_POV, utils::atow(portal->getFullPovID()));
	//portalParams.set(OS_URL_PARAM_PORTAL, portal->getPortalID().toWide());
	//portalParams.set(OS_URL_PARAM_USER, portal->getUserID().toWide());
	return getMainLink("login", portalParams);
}

std::string PortalsSystem::getAccountsLink(const shared_ptr<Portal> &portal, const ordered_map<std::wstring, std::wstring> &params)
{
	ordered_map<std::wstring, std::wstring> portalParams(params);
	portalParams.set(OS_URL_PARAM_POV, utils::atow(portal->getFullPovID()));
	//portalParams.set(OS_URL_PARAM_PORTAL, portal->getPortalID().toWide());
	//portalParams.set(OS_URL_PARAM_USER, portal->getUserID().toWide());
	return getMainLink("accounts", portalParams);
}

std::string PortalsSystem::getAvatarLink(const shared_ptr<Portal> &portal, const String &id, const DateTime &dt)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(ide::main::Resources::PARAM_AVATAR, id.to_wide());
	params.set(OS_URL_PARAM_POV, utils::atow(portal->getFullPovID()));
	//params.set(OS_URL_PARAM_PORTAL, portal->getPortalID().toWide());
	//params.set(OS_URL_PARAM_USER, portal->getUserID().toWide());
	params.set(OS_URL_PARAM_DT, dt.toString().to_wide());

	return getMainLink("resources", params);
}

std::string PortalsSystem::getFileLink(const shared_ptr<Portal> &portal, const String &id)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(ide::main::Resources::PARAM_FILE, id.to_wide());
	params.set(OS_URL_PARAM_POV, utils::atow(portal->getFullPovID()));
	//params.set(OS_URL_PARAM_PORTAL, portal->getPortalID().toWide());
	//params.set(OS_URL_PARAM_USER, portal->getUserID().toWide());

	return getMainLink("resources", params);
}

std::string PortalsSystem::getRegisterLink(const shared_ptr<Portal> &portal, const ordered_map<std::wstring, std::wstring> &params)
{
	ordered_map<std::wstring, std::wstring> portalParams(params);
	portalParams.set(OS_URL_PARAM_POV, utils::atow(portal->getFullPovID()));
	//portalParams.set(OS_URL_PARAM_PORTAL, portal->getPortalID().toWide());
	//portalParams.set(OS_URL_PARAM_USER, portal->getUserID().toWide());
	return getMainLink(OS_IDE_PAGE_REGISTER, portalParams);
}

std::string PortalsSystem::getSettingsLink(const shared_ptr<Portal> &portal, const ordered_map<std::wstring, std::wstring> &params)
{
	ordered_map<std::wstring, std::wstring> portalParams(params);
	portalParams.set(OS_URL_PARAM_POV, utils::atow(portal->getFullPovID()));
	//portalParams.set(OS_URL_PARAM_PORTAL, portal->getPortalID().toWide());
	//portalParams.set(OS_URL_PARAM_USER, portal->getUserID().toWide());
	return getMainLink("settings", portalParams);
}

std::string PortalsSystem::getViewObjectLink(const shared_ptr<Portal> &portal, const String &id, const ordered_map<std::wstring, std::wstring> &params)
{
	ordered_map<std::wstring, std::wstring> portalParams(params);
	if(id.empty() == false)
		portalParams.set(OS_URL_PARAM_ID, id.to_wide());

	return getPortalLink(portal, "view", portalParams);
}

std::string PortalsSystem::getEditObjectLink(const shared_ptr<Portal> &portal, const String &id, const ordered_map<std::wstring, std::wstring> &params)
{
	ordered_map<std::wstring, std::wstring> portalParams(params);
	if(id.empty() == false)
		portalParams.set(OS_URL_PARAM_ID, id.to_wide());

	return getPortalLink(portal, "edit", portalParams);
}

std::string PortalsSystem::getUserLink(const shared_ptr<Portal> &portal, const String &id)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_ID, id.to_wide());

	return getPortalLink(portal, "user", params);
}

std::string PortalsSystem::getSendMessageLink(const shared_ptr<Portal> &portal, const String &id, const ordered_map<std::wstring, std::wstring> &params)
{
	ordered_map<std::wstring, std::wstring> portalParams(params);
	portalParams.set(ide::portal::Account::ACTION, ide::portal::Account::getAction(ide::portal::Account::acEditMessage).to_wide());
	portalParams.set(OS_URL_PARAM_ID, id.to_wide());

	return getPortalLink(portal, "account", portalParams);
}

/*
bool PortalsSystem::createAnarchicPortal(shared_ptr<PortalOptions> options)
{
	PortalAccess access = options->getPassword().empty() ? portalAccessPublic : portalAccessPrivate;

	PortalID id = PortalID::generateAnarchic(access);
	// Inizializza l'id
	options->setID(id);

	// Crea il portale
	return _createPortal(options);
}

bool PortalsSystem::createMonarchicPortal(shared_ptr<PortalOptions> options)
{
	Buffer public_key;
	Buffer private_key;

	// Genera la coppia di chiavi dell'amministratore
	if(CryptManager::instance()->rsaGenerateKeys(rsaType2048, private_key, public_key) == false)
		return false;

	PortalAccess access = options->getPassword().empty() ? portalAccessPublic : portalAccessPrivate;

	// Genera l'id del portale sulla base della chiave pubblica dell'amministratore
	PortalID id = PortalID::generateMonarchic(public_key, access);
	// Inizializza l'id
	options->setID(id);

	if(_createPortal(options) == false)
		return false;

	shared_ptr<Portal> portal = getPortal(id);
	OS_EXCEPT_IF(portal == null, "Internal error (invalid portal)");

	shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(false);
	shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();
	database->beginTransaction();

	// Crea l'account dell'amministratore
	String accountID = portal->createAccount(database, OS_DEFAULT_ADMIN_USERNAME, OS_DEFAULT_ADMIN_PASSWORD, String::EMPTY, String::EMPTY, true, &public_key, &private_key);
	if(accountID.empty())
	{
		database->rollback();
		removePortal(id);
		return false;
	}

	database->commit();

	return true;
}
*/

/*
shared_ptr<Portal> PortalsSystem::createPortal(shared_ptr<PortalOptions> options) // TOCLEAN
{
	options->setPortalID(PortalID::generate());

	
	Buffer public_key;
	Buffer private_key;

	// Genera la coppia di chiavi dell'amministratore
	if(CryptManager::instance()->rsaGenerateKeys(rsaType2048, private_key, public_key) == false)
		return false;
	

	//PortalAccess access = options->getPassword().empty() ? portalAccessPublic : portalAccessPrivate;
	//options->setPortalID(PortalID::generateAnarchic(access));
	

	
	if(options->getPortalID().empty())
		options->setPortalID(PortalID::generateAnarchic(access));
	
	if(options->getUserID().empty())
	{
		// Create account
		shared_ptr<IdeAccount> account = IdeAccountsManager::instance()->createAccount(OS_DEFAULT_ADMIN_USERNAME, OS_DEFAULT_ADMIN_PASSWORD, true);
		if(account == null)
		{
			OS_LOG_ERROR("Cannot create account");
			return false;
		}

		Buffer publicKey;
		account->getAccount()->decodePublicKey(OS_DEFAULT_ADMIN_PASSWORD, publicKey);
		options->setUserID(DataAccount::getReferenceUser(publicKey));
	}
	
			
	shared_ptr<Portal> portal = subscribePortal(options);
	if(portal == null)
	{
		OS_LOG_ERROR("Cannot create portal");
		return null;
	}

	return portal;
}
*/
shared_ptr<Portal> PortalsSystem::subscribePortal(shared_ptr<PortalOptions> options)
{
	OS_LOCK(m_cs);

	if(options == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	PortalID id = options->getPortalID();
	if(id.empty())
	{
		id = PortalID::generate();		
		options->setPortalID(id);
	}

	PovID pov = options->getPovID();
	if(pov.empty())
		return null;
	
	//if(id.getType() != ID::idPortal || id.validate(false) == false)
	//	return null;

	String fullPov = Portal::generatePovID(id, pov);

	if(getPortal(id, pov) != null)
		return null;

	if(options->getDatabaseName().empty())
		options->setDatabaseName(fullPov);

	if(options->validate() == false)
		return null;

	String folder = utils::standardisePath(getPath() + fullPov);

	if(FileSystem::instance()->exists(folder))
	{
		OS_LOG_ERROR(String::format(_S("Folder '%S' already exists").c_str(), folder.c_str()));
		return null;
	}

	// Assicura che la root dei portali esista
	FileSystem::instance()->createDirectory(getPath());
	// Crea la root del portale
	if(FileSystem::instance()->createDirectory(folder) == false)
	{
		OS_LOG_ERROR(String::format(_S("Cannot create folder '%S'").c_str(), folder.c_str()));
		return null;
	}

	// Crea il portale
	if(_setupPortal(options, folder) == false)
	{
		FileSystem::instance()->removeDirectory(folder);
		return null;
	}

	return _loadPortal(folder, false, true);
}


bool PortalsSystem::deletePortal(shared_ptr<PortalOptions> options, const String &path)
{
	if(options == null || options->getDeleted() == false)
		return false;

	NotificationsManager::instance()->notify(_S("Removing portal '") + options->getName() + _S("'"));

	String driverName = options->getDatabaseDriver();
	shared_ptr<IDbDriver> driver = DatabasesSystem::instance()->getDriver(driverName.to_ascii());
	if(driver == null)
	{
		OS_LOG_ERROR(_S("Invalid database driver '") + driverName + _S("'"));
		return false;
	}

	String databaseName = options->getDatabaseName();
	if(driver->removeDatabase(databaseName.to_ascii(), options->getDatabaseOptions()) == false)
	{
		OS_LOG_ERROR(_S("Cannot remove database '") + databaseName + _S("'"));
		return false;
	}

	if(FileSystem::instance()->removeDirectory(path) == false)
	{
		OS_LOG_ERROR(_S("Cannot remove directory '") + path + _S("'"));
		return false;
	}

	return true;
}

shared_ptr<Portal> PortalsSystem::ensurePortal(shared_ptr<OsirisLink> link, const String &password)
{
	if(link == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	OS_LOCK(m_cs);

	shared_ptr<Portal> portal = getPortal(link->getPortal(), link->getPov());
	if(portal != null)
		return portal;

	shared_ptr<IDbDriver> defaultDriver = DatabasesSystem::instance()->getDefaultDriver();
	if(defaultDriver == null)
	{
		OS_LOG_ERROR("Invalid database driver");
		return null;
	}

	PortalID portalID = link->getPortal();
	PovID povID = link->getPov();
	String portalName = link->getName();
	if(portalName.empty())
		portalName = portalID.toUTF16();
	//String portalDescription = link->getDescription();

	shared_ptr<PortalOptions> portalOptions(OS_NEW PortalOptions());
	portalOptions->setName(portalName);
	//portalOptions->setDescription(portalDescription);
	portalOptions->setPortalID(portalID);
	portalOptions->setPovID(povID);
	portalOptions->setPassword(password);
	portalOptions->setDatabaseDriver(defaultDriver->getName());
	portalOptions->setDatabaseOptions(defaultDriver->createOptions());

	// VERYURGENT: le varie createPortal/subscribePortal dovrebbero restituire il riferimento al portale
	portal = subscribePortal(portalOptions);
	if(portal == null)
	{
		OS_LOG_ERROR("Cannot create portal");
		return null;
	}

	//portal = getPortal(portalID, povID);
	//OS_ASSERT(portal != null);
	return portal;
}
/*
bool PortalsSystem::removePortal(const PortalID &id)
{
	OS_LOCK(m_cs);

	for(Portals::iterator i = m_portals.begin(); i != m_portals.end(); ++i)
	{
		shared_ptr<Portal> portal = *i;
		if(portal->getPortalID() == id)
		{
			shared_ptr<PortalOptions> options = portal->getOptions();
			options->setDeleted(true);

			portal->save();

			m_portals.erase(i);

			return true;
		}
	}

	return false;
}
*/
bool PortalsSystem::removePortal(const PortalID &id, const PovID &pov)
{
	OS_LOCK(m_cs);

	for(Portals::iterator i = m_portals.begin(); i != m_portals.end(); ++i)
	{
		shared_ptr<Portal> portal = *i;
		if( (portal->getPortalID() == id) && (portal->getPovID() == pov) )
		{
			shared_ptr<PortalOptions> options = portal->getOptions();
			options->setDeleted(true);

			portal->save();

			m_portals.erase(i);

			return true;
		}
	}

	return false;
}
/*
bool PortalsSystem::renamePortal(const PortalID &id, const String &name)
{
	// TODO: fixme
	return false;
}
*/

void PortalsSystem::notifyPortalLoaded(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database)
{
	OS_ASSERT(portal != null);
	for(PortalsListeners::const_iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
	{
		(*i)->onPortalLoaded(portal, database);
	}
}

void PortalsSystem::addPortalsListener(shared_ptr<IPortalsListener> listener)
{
	if(listener == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	if(utils::exists(m_listeners, listener))
	{
		OS_ASSERTFALSE();
	}
	else
	{
		m_listeners.push_back(listener);
	}
}



bool PortalsSystem::loadPortals(const String &folder, bool recovery)
{
	OS_LOCK(m_cs);

	if(FileSystem::instance()->directoryExists(folder))
	{
		StringList portals;
		FileSystem::instance()->getDirectories(folder, portals, false);
		for(StringList::const_iterator i = portals.begin(); i != portals.end(); ++i)
		{
			//if((*i).length() == 40) // Length of POV hash, V1
			if((*i).length() == 40+40+1) // Length of POV hash, V2
				_loadPortal(utils::standardisePath(folder + *i), recovery, false);
		}
	}

	for(Portals::const_iterator i = m_portals.begin(); i != m_portals.end(); (*i)->start(), ++i);

	return true;
}

void PortalsSystem::clearPortals()
{
	OS_LOCK(m_cs);
	
	m_portals.clear();
}

bool PortalsSystem::installPortals()
{
	OS_LOCK(m_cs);

	NotificationsManager::instance()->notify(_S("Install bundle portals"));

	String path = utils::makeFolderPath(Options::instance()->getSharePath(), OS_PORTALS_PATH);

	if(FileSystem::instance()->directoryExists(path) == false)
		return true;

	StringList portals;
	if(FileSystem::instance()->getFiles(path, portals, false) == false)
		return false;

	for(StringList::const_iterator i = portals.begin(); i != portals.end(); ++i)
	{
		NotificationsManager::instance()->notify(_S("Installing default portal ") + *i);

		uint32 jobID = Engine::instance()->processFile(utils::makeFilePath(path, *i), String::EMPTY);
	}

	return true;
}


shared_ptr<Portal> PortalsSystem::_loadPortal(const String &path, bool recovery, bool start)
{
	try
	{
		shared_ptr<Portal> portal(OS_NEW Portal(getPeersCacheSize(), getDatabasesCacheSize()));
		if(_initPortal(portal, path, recovery, start) == false)
			return null;

		/*
		if(getPortal(portal->getPortalID()) != null)
			return false;
		*/
		if(getPortalByFullPov(portal->getFullPovID()) != null)
			return null;

		m_portals.push_back(portal);
		return portal;
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
	}
	catch(...)
	{
		OS_LOG_ERROR(_S("Unknown error loading portal '") + path + _S("'"));
	}

	return null;
}

void PortalsSystem::_initDirectories()
{
	shared_ptr<IHttpServer> server = IdeSystem::instance()->getServer();
	OS_ASSERT(server != null);

	if(server != null)
	{
		server->addDirectory(m_mainDirectory);
		server->addDirectory(m_portalsDirectory);
		server->addDirectory(m_tempDirectory);

		server->setDefaultDirectory(MAIN_DIRECTORY);
	}
}

bool PortalsSystem::_initPortal(const shared_ptr<Portal> &portal, const String &path, bool recovery, bool start)
{
	shared_ptr<PortalOptions> options(OS_NEW PortalOptions());
	// Carica le impostazioni del portale
	if(options->readFromPath(path) == false)
		return false;

	if(options->getDeleted())
	{
		deletePortal(options, path);
		return false;		// Portale rimosso
	}

	if(portal->load(path, options, recovery) == false)
		return false;

	portal->setCachesSize(getCacheSize());

	if(start)
		portal->start();

	portal->save();

	return true;
}

bool PortalsSystem::_setupPortal(shared_ptr<PortalOptions> options, const String &path)
{
	if(options->writeFile(utils::makeFilePath(path, OS_PORTAL_XML)) == false)
		return false;

	shared_ptr<DbSchemaSchema> schema(OS_NEW DbSchemaSchema());
	if(schema->parse(DatabasesSystem::instance()->getDatabasePath(OS_PORTAL_DB_TYPE, OS_PORTAL_DB_VERSION, false)) == false)
	{
		OS_LOG_ERROR(_S("Cannot parse portal's database schema"));
		return false;
	}

	shared_ptr<IDbDriver> driver = DatabasesSystem::instance()->getDriver(options->getDatabaseDriver().to_ascii());
	if(driver == null)
	{
		OS_LOG_ERROR(_S("Invalid database driver"));
		return false;
	}

	return DatabasesSystem::instance()->createDatabase(driver, options->getDatabaseName(), options->getDatabaseOptions(), schema);
}

void PortalsSystem::updateOptions()
{
    {
        OS_LOCK(m_cs);

        m_cacheSize = Options::instance()->getOption<uint32>(Options::web_options::cache_size);
        m_peersCacheSize = Options::instance()->getOption<uint32>(Options::portals_options::peers_cache_size);
        m_databasesCacheSize = Options::instance()->getOption<uint32>(Options::portals_options::databases_cache_size);

        for(Portals::const_iterator i = m_portals.begin(); i != m_portals.end(); ++i)
        {
            (*i)->setCachesSize(m_cacheSize);
            (*i)->getPeersManager()->setPeersCacheSize(m_cacheSize);
            (*i)->setDatabasesCacheSize(m_databasesCacheSize);
        }
    }

    updateIsis();
}

void PortalsSystem::updateIsis()
{
    if(Options::instance()->getOption<bool>(Options::isis_options::enable) == false)
    {
        stopIsis();
        return;
    }

    if(m_isisService->running() == false)
	{
		m_isisService->setWorkerThreadsCount(Options::instance()->getHardwareConcurrency());
        m_isisService->start();
	}
}

void PortalsSystem::stopIsis()
{
    m_isisService->stop();
}

String PortalsSystem::getName() const
{
	return _S("portals");
}

bool PortalsSystem::start(bool recovery)
{
	OS_LOCK(m_cs);

	if(CompatibilityManager::instance()->razorUpgrade(getPath()) == false)
		return false;

	if(loadPortals(getPath(), recovery) == false)
		return false;

	if(Options::instance()->isLastVersionCurrent() == false)
		if(installPortals() == false)
			return false;

	updateIsis();

	_initDirectories();

	return true;
}

void PortalsSystem::stop()
{
	stopIsis();

	clearPortals();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
