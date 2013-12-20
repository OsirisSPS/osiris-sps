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

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include "base/object.h"
#include "boost/signals.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "constants.h"
#include "dataitem.h"
#include "lock.h"
#include "ordered_map.h"
#include "singleton.h"
#include "xmloptions.h"

//////////////////////////////////////////////////////////////////////

// Clodo note: TODO, For me, this class need to be cleaned: must be derived from XMLOptions, each "options" classes derived from XMLOptions (to avoid actual repetition of functions).
// In this specific case, system options maybe moved in a separate "options" class, maybe directly accessible from system class "Engine".

//////////////////////////////////////////////////////////////////////

#define OS_TEMP_PATH							_S("temp")
#define OS_TEMP_ENV								_S("TEMP")
#define OS_TMP_ENV								_S("TMP")

#define OS_DEFAULT_CONNECTION_TIMEOUT			10000		// in millisecondi

#define OS_DEFAULT_WEB_PORT						0           // any port
#define OS_DEFAULT_CACHESIZE					1000

#define OS_SCHEMA_OPTIONS						_S("options.xsd")

#define OS_USER_OPTIONSXML						_S("options.xml")
#define OS_SYSTEM_OPTIONSXML					_S("system.xml")

#define OS_PROXY_TYPE_NONE						0
#define OS_PROXY_TYPE_HTTP						1
#define OS_PROXY_TYPE_SOCKS5					2

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IOptionValidator;
class TemporaryFile;
class XMLOptions;

//////////////////////////////////////////////////////////////////////

class EngineExport Options : public StaticSingleton<Options>,
							 public Object
{
public:
	typedef map<String, DataItem>::type Values;		// mappa standard per tenere le opzioni in ordine alfabetico
	typedef boost::signal<void ()> InitEvent;
	typedef boost::signal<void ()> UpdateEvent;

	struct EngineExport db_options
	{
		static const String default_driver;
	};

	struct EngineExport ide_options
	{
		static const String pager_pages;
		static const String pager_items;
		static const String skin_id;
		static const String objects_hot_state_threshold;
	};

	struct EngineExport folders_options
	{
		static const String share;
		static const String data;
		static const String temp;
		static const String log;
	};

	struct EngineExport isis_options
	{
		static const String enable;
	};

	struct EngineExport language_options
	{
		static const String id;
		static const String alternative;
		static const String time_offset;		
		static const String time_dst;
	};

	struct EngineExport log_options
	{
		static const String enable;
		static const String level;
		static const String serialize;		
	};

	struct EngineExport net_options
	{
		static const String connection_timeout;
		static const String max_download;
		static const String max_upload;
		static const String proxy_type;
		static const String proxy_host;
		static const String proxy_port;
		static const String proxy_username;
		static const String proxy_password;
		static const String proxy_restrict_to_unsafe_connections;
	};

	struct EngineExport p2p_options
	{
		static const String bootstrap_request_delay;
		static const String bootstrap_notify_delay;
		static const String enable;
		static const String server_port;
		static const String server_enable_upnp;
		static const String server_bind_ip;
		static const String max_incoming_connections;
		static const String max_outgoing_connections;
	};

	struct EngineExport portals_options
	{
		static const String peers_cache_size;
		static const String peers_contact_date_check;
		static const String databases_cache_size;
		static const String search_level;
		static const String search_limit;
		static const String objects_step;
		static const String stability_delta;
	};

	struct EngineExport privacy_options
	{
		static const String allow_external_images;
		static const String allow_external_urls;
	};

	struct EngineExport anonymity_options
	{
		//static const String html_mode;
		static const String object_mode;
		static const String object_trusted_domains;
		static const String object_trusted_types;
	};

	struct EngineExport system_options
	{
		static const String instance_name;
		static const String isis;
		static const String isis_enable_ssl;
		static const String isis_port;
		static const String isis_port_ssl;
		static const String clock_synchronization_method;
		static const String last_version_major;
		static const String last_version_minor;
		//static const String last_version_check_delay;  // TOCLEAN
		//static const String last_version_check_enable;  // TOCLEAN
		//static const String last_version_check_time;  // TOCLEAN
		static const String background_tasks;
		static const String hardware_concurrency;		
	};

	struct EngineExport url_options
	{
		static const String protocols_find_link;			// Elenco protocolli da renderizzare come link se fuori tag [url].
		static const String protocols_no_redirect;			// Elenco protocolli che non richiedono il passaggio alla pagina di redirect
		static const String protocols_no_confirm;			// Elenco protocolli che non richiedono conferma come link esterni
		static const String protocols_collect;				// Elenco protocolli di cui raccogliere l'url per aperture automatiche e elenco		
	};

	struct EngineExport web_options
	{
		static const String enable;
		static const String allow_remote_connections;
		static const String mcp_password;
		static const String default_portal;		
		static const String browser;
		static const String cache_size;
		static const String redirect_url;
		static const String server_port;
		static const String server_enable_upnp;
		static const String server_bind_ip;
		static const String session_duration;
	};

	struct EngineExport ui_options
	{
		static const String tray_always;
		static const String tray_balloon;
		static const String tray_minimize;
		static const String exit_confirm;		
		static const String start_minimized;		
	};

// Construction
protected:
	Options();

public:
	virtual ~Options();

// Attributes
public:
	bool exists(const String &name) const;
	
	Locked<const XMLOptions::Options, boost::shared_mutex>::shared getOptions() const;

	template <typename T>
	T getOption(const String &name) const;
	DataItem getOptionValue(const String &name) const;

	template <typename T>
	bool setOption(const String &name, const T &value);
	bool setOptionValue(const String &name, const DataItem &value);

	// Queste esistono perchè non riesco a fare il wrap python di DataItem.
	bool getOptionBool(const String &name) const; 
	void setOptionBool(const String &name, const bool &value);
	String getOptionString(const String &name) const; 
	void setOptionString(const String &name, const String &value);
	uint32 getOptionUInt32(const String &name) const; 
	void setOptionUInt32(const String &name, const uint32 &value);




	String getRootPath() const;

	String getSharePath() const;
	String getDataPath() const;
	String getTempPath() const;
	String getLogPath() const;
	String getConfigPath() const;

	String getDefaultLanguage() const;

	std::string getIsisLink(const std::string &url, const std::string &params = constants::empty_string) const;
	std::string getIsisLink(const std::string &url, const ordered_map<std::wstring, std::wstring> &params) const;

	std::string getIsisSecureLink(const std::string &url, const std::string &params = constants::empty_string) const;
	std::string getIsisSecureLink(const std::string &url, const ordered_map<std::wstring, std::wstring> &params) const;

	uint32 getServerPort() const;
	uint32 getHardwareConcurrency() const;
	String getWebMcpPassword() const;
	String getWebDefaultPortal() const;
	void setWebDefaultPortal(const String& id);
	
	bool getHelpTipStatus(const String& id) const;
	void setHelpTipStatus(const String& id, bool status);

	bool isFirstRun();
	bool isLastVersionCurrent();

private:
	String getOptionPath(const String &optionName) const;

// Operations
public:
	// Carica le opzioni
	bool load(const String &rootPath);
	// Salva le impostazioni
	bool save();
	// Applica le impostazioni
	void update();
	// Effettua pulizia di caso se necessario (ad esempio directory temporaneo in versione noinstall)
	void cleanup();

	void toDefault();

	template <typename T>
	bool ensureOption(const String &name, const T &value, bool publicOption, shared_ptr<IOptionValidator> validator = nullptr);
	bool ensureOptionValue(const String &name, const DataItem &value, bool publicOption, shared_ptr<IOptionValidator> validator = nullptr);

	void subscribeInitHandler(const InitEvent::slot_type &handler);
	template <typename T>
	void unsubscribeInitHandler(const T &handler);
	
	void subscribeUpdateHandler(const UpdateEvent::slot_type &handler);
	template <typename T>
	void unsubscribeUpdateHandler(const T &handler);

	shared_ptr<boost::recursive_mutex::scoped_lock> lock(bool acquireLocked = true);

	shared_ptr<TemporaryFile> createTemporaryFile();

private:
	void fireInitEvent();
	void fireUpdateEvent();

	bool initEnvironment();

	template <typename T>
	bool ensureUserOption(const String &name, const T &value, bool publicOption, shared_ptr<IOptionValidator> validator = nullptr);
	bool ensureUserOptionValue(const String &name, const DataItem &value, bool publicOption, shared_ptr<IOptionValidator> validator = nullptr);

	template <typename T>
	bool ensureSystemOption(const String &name, const T &value, shared_ptr<IOptionValidator> validator = nullptr);
	bool ensureSystemOptionValue(const String &name, const DataItem &value, shared_ptr<IOptionValidator> validator = nullptr);

	bool loadSystemOptions(const String &rootPath);
	bool initUserOptions();

	bool loadOptions(XMLOptions *options, const String &filename, bool withSchema = true);
	bool ensureOption(XMLOptions *options, const String &name, const DataItem &value, bool publicOption, shared_ptr<IOptionValidator> validator);

	std::string makeIsisLink(const std::string &url, const std::string &params, bool secure) const;

private:
	mutable boost::recursive_mutex m_dataCS;
	mutable boost::recursive_mutex m_eventsCS;
	uint32 m_filesID;
	scoped_ptr<XMLOptions> m_userOptions;
	scoped_ptr<XMLOptions> m_systemOptions;
	InitEvent m_initEvent;
	UpdateEvent m_updateEvent;
	String m_rootPath;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
T Options::getOption(const String &name) const
{
    // Mac FIX: la riga sottostante non si compila SOLO sotto Mac.
    // return getOptionValue(name).get<T>();
	return static_cast<DataItem>(getOptionValue(name)).get<T>();
}

template <typename T>
bool Options::setOption(const String &name, const T &value)
{
	return setOptionValue(name, DataItem(value));
}

template <typename T>
bool Options::ensureOption(const String &name, const T &value, bool publicOption, shared_ptr<IOptionValidator> validator)
{
	return ensureOptionValue(name, DataItem(value), publicOption, validator);
}

template <typename T>
void Options::unsubscribeInitHandler(const T &handler)
{
	OS_LOCK(m_eventsCS);
	m_initEvent.disconnect(handler);
}

template <typename T>
void Options::unsubscribeUpdateHandler(const T &handler)
{
	OS_LOCK(m_eventsCS);
	m_updateEvent.disconnect(handler);
}

template <typename T>
bool Options::ensureUserOption(const String &name, const T &value, bool publicOption, shared_ptr<IOptionValidator> validator)
{
	return ensureUserOptionValue(name, DataItem(value), publicOption, validator);
}

template <typename T>
bool Options::ensureSystemOption(const String &name, const T &value, shared_ptr<IOptionValidator> validator)
{
	return ensureSystemOptionValue(name, DataItem(value), validator);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OPTIONS_H
