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
#include "options.h"

#include "bandwidthmanager.h"
#include "conversions.h"
#include "engine.h"
#include "filesystem.h"
#include "httpparser.h"
#include "httpurl.h"
#include "idesystem.h"
#include "ihttpserver.h"
#include "isis.h"
#include "languagemanager.h"
#include "log.h"
#include "portalsportal.h"
#include "p2pp2psystem.h"
#include "p2pserver.h"
#include "platformmanager.h"
#include "temporaryfile.h"
#include "utils.h"
#include "unicode.h"
#include "xmlattribute.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

#define OS_OPTIONS_TEMPORARY_FILE_PREFIX		"file"
#define OS_OPTIONS_TEMPORARY_FILE_EXTENSION		".tmp"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SINGLETON(OS_NAMESPACE_NAME::Options)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String Options::db_options::default_driver = _S("db.default_driver");

const String Options::ide_options::pager_pages = _S("ide.pager.pages");
const String Options::ide_options::pager_items = _S("ide.pager.items");
const String Options::ide_options::skin_id = _S("ide.skin.id");
const String Options::ide_options::objects_hot_state_threshold = _S("ide.objects.hot_state_threshold");

const String Options::folders_options::share = _S("folders.share");
const String Options::folders_options::data = _S("folders.data");
const String Options::folders_options::temp = _S("folders.temp");
const String Options::folders_options::log = _S("folders.log");

const String Options::isis_options::enable = _S("isis.enable");

const String Options::language_options::id = _S("language.id");
const String Options::language_options::alternative = _S("language.alternative");
const String Options::language_options::time_offset = _S("language.time_offset");
const String Options::language_options::time_dst = _S("language.time_dst");

const String Options::log_options::enable = _S("log.enable");
const String Options::log_options::level = _S("log.level");
const String Options::log_options::serialize = _S("log.serialize");

const String Options::net_options::connection_timeout = _S("net.connection_timeout");
const String Options::net_options::max_download = _S("net.max_download");
const String Options::net_options::max_upload = _S("net.max_upload");
const String Options::net_options::proxy_type = _S("net.proxy.type");
const String Options::net_options::proxy_host = _S("net.proxy.host");
const String Options::net_options::proxy_port = _S("net.proxy.port");
const String Options::net_options::proxy_username = _S("net.proxy.username");
const String Options::net_options::proxy_password = _S("net.proxy.password");
const String Options::net_options::proxy_restrict_to_unsafe_connections = _S("net.proxy.restrict_to_unsafe_connections");

const String Options::p2p_options::bootstrap_request_delay = _S("p2p.bootstrap.request_delay");
const String Options::p2p_options::bootstrap_notify_delay = _S("p2p.bootstrap.notify_delay");
const String Options::p2p_options::enable = _S("p2p.enable");
const String Options::p2p_options::server_port = _S("p2p.server.port");
const String Options::p2p_options::server_enable_upnp = _S("p2p.server.enable_upnp");
const String Options::p2p_options::server_bind_ip = _S("p2p.server.bind_ip");
const String Options::p2p_options::max_incoming_connections = _S("p2p.max_incoming_connections");
const String Options::p2p_options::max_outgoing_connections = _S("p2p.max_outgoing_connections");

const String Options::portals_options::peers_cache_size = _S("portals.peers_cache_size");
const String Options::portals_options::peers_contact_date_check = _S("portals.peers_contact_date_check");
const String Options::portals_options::databases_cache_size = _S("portals.databases_cache_size");
const String Options::portals_options::search_level = _S("portals.search_level");
const String Options::portals_options::search_limit = _S("portals.search_limit");
const String Options::portals_options::objects_step = _S("portals.objects_step");
const String Options::portals_options::stability_delta = _S("portals.stability_delta");

const String Options::privacy_options::allow_external_images = _S("privacy.allow_external_images");
const String Options::privacy_options::allow_external_urls = _S("privacy.allow_external_urls");

//const String Options::anonymity_options::html_mode = _S("anonymity.html_mode");
const String Options::anonymity_options::object_mode = _S("anonymity.object_mode");
const String Options::anonymity_options::object_trusted_domains = _S("anonymity.object_trusted_domains");
const String Options::anonymity_options::object_trusted_types = _S("anonymity.object_trusted_types");

const String Options::system_options::instance_name = _S("system.instance_name");
const String Options::system_options::isis = _S("system.isis");
const String Options::system_options::isis_enable_ssl = _S("system.isis.enable_ssl");
const String Options::system_options::isis_port = _S("system.isis.port");
const String Options::system_options::isis_port_ssl = _S("system.isis.port_ssl");
const String Options::system_options::clock_synchronization_method = _S("system.clock_synchronization_method");
const String Options::system_options::last_version_major = _S("system.last_version.major");
const String Options::system_options::last_version_minor = _S("system.last_version.minor");
//const String Options::system_options::last_version_check_delay = _S("system.last_version.check_delay"); // TOCLEAN
//const String Options::system_options::last_version_check_enable = _S("system.last_version.check_enable"); // TOCLEAN
//const String Options::system_options::last_version_check_time = _S("system.last_version.check_time"); // TOCLEAN
const String Options::system_options::background_tasks = _S("system.background_tasks");
const String Options::system_options::hardware_concurrency = _S("system.hardware_concurrency");

const String Options::url_options::protocols_find_link = _S("url.protocols.find_link");
const String Options::url_options::protocols_no_redirect = _S("url.protocols.no_redirect");
const String Options::url_options::protocols_no_confirm = _S("url.protocols.no_confirm");
const String Options::url_options::protocols_collect = _S("url.protocols.collect");

const String Options::web_options::enable = _S("web.enable");
const String Options::web_options::allow_remote_connections = _S("web.allow_remote_connections");
const String Options::web_options::mcp_password = _S("web.mcp_password");
const String Options::web_options::default_portal = _S("web.default_portal");
const String Options::web_options::browser = _S("web.browser");
const String Options::web_options::cache_size = _S("web.cache_size");
const String Options::web_options::redirect_url = _S("web.redirect_url");
const String Options::web_options::server_port = _S("web.server.port");
const String Options::web_options::server_enable_upnp = _S("web.server.enable_upnp");
const String Options::web_options::server_bind_ip = _S("web.server.bind_ip");
const String Options::web_options::session_duration = _S("web.session.duration");

const String Options::ui_options::tray_always = _S("ui.tray.always");
const String Options::ui_options::tray_balloon = _S("ui.tray.balloon");
const String Options::ui_options::tray_minimize = _S("ui.tray.minimize");
const String Options::ui_options::exit_confirm = _S("ui.exit_confirm");
const String Options::ui_options::start_minimized = _S("ui.start_minimized");

//////////////////////////////////////////////////////////////////////

Options::Options() : m_userOptions(OS_NEW XMLOptions()),
                     m_systemOptions(OS_NEW XMLOptions()),
					 m_filesID(0)
{
	ensureSystemOption<String>(folders_options::share, String::EMPTY);
	ensureSystemOption<String>(folders_options::data, String::EMPTY);
	ensureSystemOption<String>(folders_options::temp, String::EMPTY);
	ensureSystemOption<String>(folders_options::log, String::EMPTY);

	ensureUserOption<String>(db_options::default_driver, _S("sqlite"), true);

	// Public to false, to avoid option in Misc tab.
	ensureUserOption<String>(language_options::id, OS_DEFAULT_LANGUAGE, false);
	ensureUserOption<String>(language_options::alternative, OS_DEFAULT_ALTERNATIVE_LANGUAGE, false);
	ensureUserOption<int32>(language_options::time_offset, OS_DEFAULT_TIME_DST, false);
	ensureUserOption<int32>(language_options::time_dst, OS_DEFAULT_TIME_DST, false);	

    ensureUserOption<bool>(isis_options::enable, true, true);

	ensureUserOption<bool>(log_options::enable, true, true);
#ifdef OS_DEBUG
	ensureUserOption<uint32>(log_options::level, logLevelDebug, true);
#else
	ensureUserOption<uint32>(log_options::level, logLevelDefault, true);
#endif
	ensureUserOption<bool>(log_options::serialize, false, true);	

	ensureUserOption<uint32>(net_options::connection_timeout, OS_DEFAULT_CONNECTION_TIMEOUT, true);
	ensureUserOption<uint32>(net_options::max_download, OS_NET_UNLIMITED_BANDWIDTH, true);
	ensureUserOption<uint32>(net_options::max_upload, OS_NET_UNLIMITED_BANDWIDTH, true);
	ensureUserOption<uint32>(net_options::proxy_type, OS_PROXY_TYPE_NONE, true);
	ensureUserOption<String>(net_options::proxy_host, String::EMPTY, true);
	ensureUserOption<uint32>(net_options::proxy_port, 0, true);
	ensureUserOption<String>(net_options::proxy_username, String::EMPTY, true);
	ensureUserOption<String>(net_options::proxy_password, String::EMPTY, true);
	ensureUserOption<bool>(net_options::proxy_restrict_to_unsafe_connections, false, true);

	ensureUserOption<uint32>(ide_options::pager_pages, 5, true);
	ensureUserOption<uint32>(ide_options::pager_items, 20, true);
	ensureUserOption<String>(ide_options::skin_id, OS_IDE_SKIN_DEFAULT, false); // Public to false, to avoid option in Misc tab.
	ensureUserOption<uint32>(ide_options::objects_hot_state_threshold, 15, true);

	ensureUserOption<uint32>(p2p_options::bootstrap_request_delay, OS_P2P_BOOTSTRAP_REQUEST_DELAY, true); // TOCLEAN: non + usata?
	ensureUserOption<uint32>(p2p_options::bootstrap_notify_delay, OS_P2P_BOOTSTRAP_NOTIFY_DELAY, true); // TOCLEAN: non + usata?
	ensureUserOption<bool>(p2p_options::enable, true, true);
	ensureUserOption<uint32>(p2p_options::server_port, OS_P2P_NEW_DEFAULT_SERVER_PORT, true);
	ensureUserOption<bool>(p2p_options::server_enable_upnp, true, true);
	ensureUserOption<String>(p2p_options::server_bind_ip, String::EMPTY, true);
	ensureUserOption<uint32>(p2p_options::max_incoming_connections, OS_P2P_MAX_INCOMING_CONNECTIONS, true);
	ensureUserOption<uint32>(p2p_options::max_outgoing_connections, OS_P2P_MAX_OUTGOING_CONNECTIONS, true);

	ensureUserOption<uint32>(portals_options::peers_cache_size, OS_PORTAL_PEERS_CACHE_SIZE, true);
	ensureUserOption<uint32>(portals_options::peers_contact_date_check, OS_PORTAL_PEERS_CONTACT_DATE_CHECK, false);
    ensureUserOption<uint32>(portals_options::databases_cache_size, OS_PORTAL_DATABASES_CACHE_SIZE, true);
	ensureUserOption<uint32>(portals_options::search_level, OS_PORTAL_SEARCH_LEVEL, true);
	ensureUserOption<uint32>(portals_options::search_limit, OS_PORTAL_SEARCH_LIMIT, true);
	ensureUserOption<uint32>(portals_options::objects_step, OS_PORTAL_OBJECTS_STEP, true);
	ensureUserOption<uint32>(portals_options::stability_delta, 5000, true);

	ensureUserOption<bool>(privacy_options::allow_external_images, true, true);
	ensureUserOption<bool>(privacy_options::allow_external_urls, true, true);

	//ensureUserOption<uint32>(anonymity_options::html_mode, 0);
	ensureUserOption<uint32>(anonymity_options::object_mode, 1, true);
	ensureUserOption<String>(anonymity_options::object_trusted_domains, String::EMPTY, false);
	ensureUserOption<String>(anonymity_options::object_trusted_types, String::EMPTY, false);

	ensureUserOption<String>(system_options::instance_name, String::EMPTY, true);
	ensureUserOption<String>(system_options::isis, OS_ISIS_DEFAULT_URL, true);
	ensureUserOption<bool>(system_options::isis_enable_ssl, true, true);
	ensureUserOption<uint32>(system_options::isis_port, OS_ISIS_DEFAULT_PORT, true);
	ensureUserOption<uint32>(system_options::isis_port_ssl, OS_ISIS_DEFAULT_PORT_SSL, true);
	ensureUserOption<String>(system_options::clock_synchronization_method, String::EMPTY, true);
	ensureUserOption<uint32>(system_options::last_version_major, 0, false);
	ensureUserOption<uint32>(system_options::last_version_minor, 0, false);
	//ensureUserOption<bool>(system_options::last_version_check_enable, true, true);  // TOCLEAN
	//ensureUserOption<uint32>(system_options::last_version_check_delay, 3, true);  // TOCLEAN
	//ensureUserOption<String>(system_options::last_version_check_time, DateTime::EMPTY.toString(), false);  // TOCLEAN
	ensureUserOption<uint32>(system_options::background_tasks, OS_ENGINE_DEFAULT_BACKGROUND_TASKS, true);
	ensureUserOption<uint32>(system_options::hardware_concurrency, PlatformManager::instance()->getOptimalHardwareConcurrency(), true);

  	ensureUserOption<String>(url_options::protocols_find_link, _S("osiris|http|https|ftp|ftps|gopher|news|nntp|telnet|wais|file|mailto|ed2k|ants2k|magnet|stealthnet"), true);
	ensureUserOption<String>(url_options::protocols_no_redirect, _S("osiris|gopher|news|nntp|telnet|wais|file|mailto|ed2k|ants2k|magnet|stealthnet|http://localhost:23402/offsystem/.*"), true);
	ensureUserOption<String>(url_options::protocols_no_confirm, _S("osiris|mailto|ed2k|ants2k|magnet|stealthnet|http://localhost:23402/offsystem/.*"), true);
	ensureUserOption<String>(url_options::protocols_collect, _S("osiris|ed2k|ants2k|magnet|stealthnet|http://localhost:23402/offsystem/.*"), true);
	//ensureUserOption<String>(url_options::trusted_domains, String::EMPTY);

	ensureUserOption<bool>(web_options::enable, true, true);
	ensureUserOption<bool>(web_options::allow_remote_connections, false, true);
	ensureUserOption<String>(web_options::mcp_password, String::EMPTY, true);
	ensureUserOption<String>(web_options::default_portal, String::EMPTY, false);
	ensureUserOption<String>(web_options::browser, String::EMPTY, true);
	ensureUserOption<uint32>(web_options::cache_size, OS_DEFAULT_CACHESIZE, true);
	//ensureUserOption<String>(web_options::redirect_url, _S("http://anonym.to/?#url#"));
	ensureUserOption<String>(web_options::redirect_url, _S("https://www.osiris-sps.org/isis/redirect.php?#url#"), true);	
	ensureUserOption<uint32>(web_options::server_port, OS_DEFAULT_WEB_PORT, true);
	ensureUserOption<bool>(web_options::server_enable_upnp, false, true);
	ensureUserOption<String>(web_options::server_bind_ip, String::EMPTY, true);
	ensureUserOption<uint32>(web_options::session_duration, OS_HTTP_DEFAULT_SESSION_DURATION, true);

	// Hack: explained here: http://www.osiris-sps.org/forums/index.php?showtopic=1775
	if(PlatformManager::instance()->getEnv("KDE_SESSION_VERSION") != String::EMPTY)
	{
		NotificationsManager::instance()->notify(_S("KDE detected. Tray icon disabled for unresolved issue."));
		ensureUserOption<bool>(ui_options::tray_minimize, false, true);
	}
	else
	{
		ensureUserOption<bool>(ui_options::tray_minimize, true, true);
	}

	// Hack: explained here: http://www.osiris-sps.org/forums/index.php?showtopic=1775
#if OS_PLATFORM == OS_PLATFORM_LINUX
{
	ensureUserOption<bool>(ui_options::tray_always, true, true); 
}
#else
{
	ensureUserOption<bool>(ui_options::tray_always, false, true); 
}
#endif

	ensureUserOption<bool>(ui_options::tray_balloon, false, true);
	ensureUserOption<bool>(ui_options::exit_confirm, true, true);
	
	ensureUserOption<bool>(ui_options::start_minimized, false, true);
}

Options::~Options()
{

}

bool Options::getOptionBool(const String &name) const
{
	return getOptionValue(name);
}

void Options::setOptionBool(const String &name, const bool &value)
{
	setOptionValue(name, value);
}

String Options::getOptionString(const String &name) const
{
	return getOptionValue(name);
}

void Options::setOptionString(const String &name, const String &value)
{
	setOptionValue(name, value);
}

uint32 Options::getOptionUInt32(const String &name) const
{
	return getOptionValue(name);
}

void Options::setOptionUInt32(const String &name, const uint32 &value)
{
	setOptionValue(name, value);
}

String Options::getRootPath() const
{
	OS_LOCK(m_dataCS);
	return m_rootPath;
}

String Options::getSharePath() const
{
	static String path = getOptionPath(folders_options::share); // Cache with the value of the first call
	return path;
}

String Options::getDataPath() const
{
	static String path = getOptionPath(folders_options::data); // Cache with the value of the first call
	return path;
}

String Options::getTempPath() const
{
	static String path = getOptionPath(folders_options::temp); // Cache with the value of the first call
	return path;	
}

String Options::getLogPath() const
{
	return getOptionPath(folders_options::log);
}

String Options::getConfigPath() const
{
	return utils::makeFolderPath(getDataPath(), OS_CONFIG_PATH);
}

String Options::getDefaultLanguage() const
{
	String language = getOption<String>(language_options::id);
	if(language.empty())
		language = PlatformManager::instance()->getSystemCulture();

	if(LanguageManager::instance()->hasCulture(language))
		return language;

	return _S("en-US");
}

std::string Options::getIsisLink(const std::string &url, const std::string &params) const
{
	return makeIsisLink(url, params, false);
}

std::string Options::getIsisLink(const std::string &url, const ordered_map<std::wstring, std::wstring> &params) const
{
	std::string str;
	HttpParser::saveUrlParams(params, str);
	return getIsisLink(url, str);
}

std::string Options::getIsisSecureLink(const std::string &url, const std::string &params) const
{
	return makeIsisLink(url, params, getOption<bool>(system_options::isis_enable_ssl));
}

uint32 Options::getServerPort() const
{
	return getOption<uint32>(p2p_options::server_port);
}

uint32 Options::getHardwareConcurrency() const
{
	return utils::max<uint32>(getOption<uint32>(system_options::hardware_concurrency), 1);
}

String Options::getWebMcpPassword() const
{
	return getOption<String>(Options::web_options::mcp_password);
}

String Options::getWebDefaultPortal() const
{
	return getOption<String>(Options::web_options::default_portal);
}

void Options::setWebDefaultPortal(const String& id)
{
	setOption(Options::web_options::default_portal, id);
}

bool Options::getHelpTipStatus(const String& id) const
{
	return getOption<bool>("help." + id);
}

void Options::setHelpTipStatus(const String& id, bool status)
{
	setOption("help." + id, status);
}

bool Options::isFirstRun()
{
	int major = Options::instance()->getOption<uint32>(Options::system_options::last_version_major);
	int minor = Options::instance()->getOption<uint32>(Options::system_options::last_version_minor);

	return ( (major == 0) && (minor == 0) );
}

bool Options::isLastVersionCurrent()
{
	int major = Options::instance()->getOption<uint32>(Options::system_options::last_version_major);
	int minor = Options::instance()->getOption<uint32>(Options::system_options::last_version_minor);

	if(major != OS_OSIRIS_VERSION_MAJOR)
		return false;
	if(minor != OS_OSIRIS_VERSION_MINOR)
		return false;
	
	return true;
}

std::string Options::getIsisSecureLink(const std::string &url, const ordered_map<std::wstring, std::wstring> &params) const
{
	std::string str;
	HttpParser::saveUrlParams(params, str);
	return getIsisSecureLink(url, str);
}

String Options::getOptionPath(const String &optionName) const
{
	String path = PlatformManager::instance()->getExpandedEnvironmentPath(getOption<String>(optionName));
	if(FileSystem::instance()->isRelativePath(path))
	{
		// Assicura di restituire sempre un path assoluto
		path = utils::makeFolderPath(getRootPath(), path);
	}

	return path;
}

bool Options::exists(const String &name) const
{
	// Non si può testare getOption(name) != DataItem::EMPTY perchè potrebbe essere un valore di un'opzione valido

	OS_LOCK(m_dataCS);

	if(m_userOptions->exists(name))
		return true;

	if(m_systemOptions->exists(name))
		return true;

	return false;
}


Locked<const XMLOptions::Options, boost::shared_mutex>::shared Options::getOptions() const
{
	return m_userOptions->getOptions();
}

DataItem Options::getOptionValue(const String &name) const
{
	OS_LOCK(m_dataCS);

    if(m_userOptions->exists(name))
        return m_userOptions->getOption(name);

    return m_systemOptions->getOption(name);
}

bool Options::setOptionValue(const String &name, const DataItem &value)
{
	OS_LOCK(m_dataCS);

	return m_userOptions->setOption(name, value);
}

bool Options::load(const String &rootPath)
{
	OS_LOCK(m_dataCS);

	m_rootPath = rootPath;

    // Carica le opzioni a livello di macchina
	if(loadSystemOptions(rootPath) == false)
		return false;

	// Inizializza le directory di base
	if(initEnvironment() == false)
		return false;

	// Inizializza le opzioni a livello di utente
	if(initUserOptions() == false)
		return false;

	return true;
}

bool Options::save()
{
	OS_LOCK(m_dataCS);

	Options::instance()->setOption(Options::system_options::last_version_major, OS_OSIRIS_VERSION_MAJOR);
	Options::instance()->setOption(Options::system_options::last_version_minor, OS_OSIRIS_VERSION_MINOR);

	// N.B.: è necessario qui farsi restituire sempre un documento valido altrimenti, nel caso in cui tutte le opzioni siano al valore di default, verrebbe restituito nullptr e il file delle opzioni non verrebbe aggiornato
    shared_ptr<XMLDocument> document = m_userOptions->toXML(false);
    if(document == nullptr)
	{
		OS_ASSERTFALSE();	// In seguito a m_userOptions->toXML(false) il documento restituito dovrebbe essere sempre valido
        return false;
	}

	return document->writeFile(utils::makeFilePath(getConfigPath(), OS_USER_OPTIONSXML));
}

void Options::update()
{
	fireUpdateEvent();
}

void Options::cleanup()
{
	OS_LOCK(m_dataCS);

	// Svuota la directory temporanea
	String tempPath = getTempPath();
	if(FileSystem::instance()->removeDirectory(tempPath) == false)
	{
		OS_LOG_ERROR(_S("Cannot remove directory \"") + tempPath + _S("\""));
		return;
	}
}

void Options::toDefault()
{
	OS_LOCK(m_dataCS);

	m_userOptions->toDefault();
}

bool Options::ensureOptionValue(const String &name, const DataItem &value, bool publicOption, shared_ptr<IOptionValidator> validator)
{
	return ensureUserOption(name, value, publicOption, validator);
}

void Options::subscribeInitHandler(const InitEvent::slot_type &handler)
{
	OS_LOCK(m_eventsCS);
	m_initEvent.connect(handler);
}

void Options::subscribeUpdateHandler(const UpdateEvent::slot_type &handler)
{
	OS_LOCK(m_eventsCS);
	m_updateEvent.connect(handler);
}

shared_ptr<boost::recursive_mutex::scoped_lock> Options::lock(bool acquireLocked)
{
	if(acquireLocked)
		return shared_ptr<boost::recursive_mutex::scoped_lock>(OS_NEW_T(boost::recursive_mutex::scoped_lock)(m_dataCS), os_delete_t());

	return shared_ptr<boost::recursive_mutex::scoped_lock>(OS_NEW_T(boost::recursive_mutex::scoped_lock)(m_dataCS, boost::defer_lock), os_delete_t());
}

shared_ptr<TemporaryFile> Options::createTemporaryFile()
{
	OS_LOCK(m_dataCS);

	String tempPath = getTempPath();
	String filename;
	for(;;)
	{
		filename = utils::makeFilePath(tempPath, OS_OPTIONS_TEMPORARY_FILE_PREFIX + conversions::to_utf16(m_filesID++) + OS_OPTIONS_TEMPORARY_FILE_EXTENSION);
		if(FileSystem::instance()->exists(filename) == false)
			break;
	}

	shared_ptr<TemporaryFile> file(OS_NEW TemporaryFile(false));
	if(file->open(filename, File::ofReadWrite) == false)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	return file;
}

void Options::fireInitEvent()
{
	OS_LOCK(m_eventsCS);
	m_initEvent();
}

bool Options::initEnvironment()
{
	// Inizializza la directory di base
	String dataPath = getDataPath();
	if(FileSystem::instance()->ensureDirectory(dataPath) == false)
	{
		OS_LOG_CRITICAL(_S("Cannot ensure directory \"") + dataPath + _S("\""));
		return false;
	}		
	
	// Inizializza la directory di configurazione
	String configPath = getConfigPath();		
	if(FileSystem::instance()->ensureDirectory(configPath) == false)
	{
		OS_LOG_ERROR(_S("Cannot ensure directory \"") + configPath + _S("\""));
		return false;
	}

	// Inizializza la directory temporanea
	String tempPath = getTempPath();
	if(FileSystem::instance()->ensureDirectory(tempPath) == false)
	{
		OS_LOG_ERROR(_S("Cannot ensure directory \"") + tempPath + _S("\""));
		return false;
	}

	// Alcune librerie leggono come variabile d'ambiente TEMP (lucene ad esempio) mentre altre TMP (boost ad esempio) pertanto vengono qui settate entrambe

	if(PlatformManager::instance()->setEnv(OS_TEMP_ENV, utils::standardiseEnvPath(tempPath)) == false)
		OS_LOG_WARNING(String::format(_S("Cannot set env '%S'").c_str(), OS_TEMP_ENV.c_str()));

	if(PlatformManager::instance()->setEnv(OS_TMP_ENV, utils::standardiseEnvPath(tempPath)) == false)
		OS_LOG_WARNING(String::format(_S("Cannot put env '%S'").c_str(), OS_TMP_ENV.c_str()));

	return true;
}

bool Options::ensureUserOptionValue(const String &name, const DataItem &value, bool publicOption, shared_ptr<IOptionValidator> validator)
{
	return ensureOption(m_userOptions.get(), name, value, publicOption, validator);
}

bool Options::ensureSystemOptionValue(const String &name, const DataItem &value, shared_ptr<IOptionValidator> validator)
{
	return ensureOption(m_systemOptions.get(), name, value, false, validator);
}

void Options::fireUpdateEvent()
{
	OS_LOCK(m_eventsCS);
	m_updateEvent();
}

bool Options::loadSystemOptions(const String &rootPath)
{
	return loadOptions(m_systemOptions.get(), utils::makeFilePath(rootPath, OS_SYSTEM_OPTIONSXML), false);
}

bool Options::initUserOptions()
{
	bool done = true;

	String filename = utils::makeFilePath(utils::makeFolderPath(getDataPath(), OS_CONFIG_PATH), OS_USER_OPTIONSXML);
	if(FileSystem::instance()->fileExists(filename))
		// Carica le opzioni a livello di utente
		done = loadOptions(m_userOptions.get(), filename);
	else
		// Richiama l'evento di inizializzazione delle opzioni
		fireInitEvent();

	return done;
}

bool Options::loadOptions(XMLOptions *options, const String &filename, bool withSchema)
{
	OS_ASSERT(options != nullptr);

	if(FileSystem::instance()->fileExists(filename) == false)
		return false;

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	if(withSchema)
	{
		shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
		schema->parseFile(utils::makeFilePath(utils::makeFolderPath(getSharePath(),OS_SCHEMAS_PATH), OS_SCHEMA_OPTIONS));
		document->setSchema(schema);
	}

	if(document->parseFile(filename) == false)
		return false;

    options->parseXML(document);
	return true;
}

bool Options::ensureOption(XMLOptions *options, const String &name, const DataItem &value, bool publicOption, shared_ptr<IOptionValidator> validator)
{
	OS_ASSERT(options != nullptr);

	OS_LOCK(m_dataCS);
	return options->ensureOption(name, value, publicOption, validator);
}

std::string Options::makeIsisLink(const std::string &url, const std::string &params, bool secure) const
{
	String isis = getOption<String>(system_options::isis);
	if(isis.empty())
		return constants::empty_string;	// Isis disabilitato

	HttpUrl isisLink(isis.to_ascii());
	isisLink.setSSL(secure);
	isisLink.setPath(utils::makeUrlFilePath(isisLink.getPath(), url).to_ascii());
	isisLink.setParams(params);

	uint32 isisPort = 0;
	if(secure)
		isisPort = getOption<uint32>(system_options::isis_port_ssl);
	else
		isisPort = getOption<uint32>(system_options::isis_port);

	isisLink.setPort(static_cast<uint16>(isisPort));

	return isisLink.toString();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
