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

#ifndef _PORTALS_PORTALSSYSTEM_H
#define _PORTALS_PORTALSSYSTEM_H

#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "ideide.h"
#include "dbdb.h"
#include "isystem.h"
#include "ordered_map.h"
#include "portalsportals.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DateTime;
class HttpEnumeratedDirectoryCallback;
class HttpVirtualDirectory;
class IPortalDatabase;
class IPortalsListener;
class IsisService;
class Portal;
class PortalID;
class PortalOptions;
class OsirisLink;

//////////////////////////////////////////////////////////////////////

class EngineExport PortalsSystem : public DynamicSingleton<PortalsSystem>,
								   public ISystem
{
public:
	typedef list<shared_ptr<Portal> >::type Portals;
	typedef list<shared_ptr<IPortalsListener> >::type PortalsListeners;

// Construction
public:
	PortalsSystem();
	virtual ~PortalsSystem();

// Attributes
public:
	inline shared_ptr<HttpVirtualDirectory> getPortalsDirectory() const;
	inline shared_ptr<HttpVirtualDirectory> getMainDirectory() const;
	inline shared_ptr<HttpEnumeratedDirectoryCallback> getTempDirectory() const;

	//inline bool hasPortal(const PortalID &id) const;
	inline bool hasPortal(const PortalID &id, const PovID &pov) const;
	inline bool hasPortalByFullPov(const String &id) const;

	inline shared_ptr<IsisService> getIsisService() const;

	uint32 getCacheSize() const;
	uint32 getPeersCacheSize() const;
	uint32 getDatabasesCacheSize() const;

	Portals getPortals() const;
	uint32 getPortalsCount() const;

	//shared_ptr<Portal> getPortal(const PortalID &id) const;
	shared_ptr<Portal> getPortal(const PortalID &id, const PovID &pov) const;
	shared_ptr<Portal> getPortalByFullPov(const String &id) const;
	shared_ptr<Portal> getPortalByIndex(uint32 index) const;
	shared_ptr<Portal> getFirstPortal(const PortalID &id) const;

	String getPath() const;

	static ide::portal::PortalPage getPortalPage(const String &name);
	//static String getPortalPage(const ide::portal::PortalPage &page);

	static ide::main::MainPage getMainPage(const String &name);
	//static String getMainPage(const ide::main::MainPage &page);

	//std::string getMainLink(ide::main::MainPage page, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getMainLink(const std::string &page, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	//std::string getPortalLink(const shared_ptr<Portal> &portal, ide::portal::PortalPage page, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getPortalLink(const shared_ptr<Portal> &portal, const std::string &page, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	//std::string getPortalLink(const PortalID &id, const ObjectID &user, const std::string &page, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getErrorLink(const String &message);
	std::string getRedirectLink(const std::string &url);
	std::string getExternalLink(const std::string &url, bool confirm);

	std::string getLoginLink(const shared_ptr<Portal> &portal, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getAccountsLink(const shared_ptr<Portal> &portal, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getAvatarLink(const shared_ptr<Portal> &portal, const String &id, const DateTime &dt);
	std::string getFileLink(const shared_ptr<Portal> &portal, const String &id);
	std::string getRegisterLink(const shared_ptr<Portal> &portal, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getSettingsLink(const shared_ptr<Portal> &portal, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));

	std::string getViewObjectLink(const shared_ptr<Portal> &portal, const String &id, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));
	std::string getEditObjectLink(const shared_ptr<Portal> &portal, const String &id, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));	
	std::string getUserLink(const shared_ptr<Portal> &portal, const String &id);
	std::string getSendMessageLink(const shared_ptr<Portal> &portal, const String &id, const ordered_map<std::wstring, std::wstring> &params = (ordered_map<std::wstring, std::wstring>()));

// Operations
public:
	//bool createAnarchicPortal(shared_ptr<PortalOptions> options);
	//bool createMonarchicPortal(shared_ptr<PortalOptions> options);
	//shared_ptr<Portal> createPortal(shared_ptr<PortalOptions> options);
	shared_ptr<Portal> subscribePortal(shared_ptr<PortalOptions> options);
	bool deletePortal(shared_ptr<PortalOptions> options, const String &path);
	shared_ptr<Portal> ensurePortal(shared_ptr<OsirisLink> link, const String &password = String::EMPTY);

	bool removePortal(const PortalID &id, const PovID &pov);
	//bool renamePortal(const PortalID &id, const String &name);

	void notifyPortalLoaded(shared_ptr<Portal> portal, shared_ptr<IPortalDatabase> database);

	void addPortalsListener(shared_ptr<IPortalsListener> listener);

private:
	bool loadPortals(const String &folder, bool recovery);
	void clearPortals();

	bool installPortals();

	shared_ptr<Portal> _loadPortal(const String &path, bool recovery, bool start);
	void _initDirectories();

	bool _initPortal(const shared_ptr<Portal> &portal, const String &path, bool recovery, bool start);
	bool _setupPortal(shared_ptr<PortalOptions> options, const String &path);

	void updateOptions();

	void updateIsis();
	void stopIsis();

// ISystem interface
public:
	virtual String getName() const;

	virtual bool start(bool recovery);
	virtual void stop();

private:
	Portals m_portals;
	PortalsListeners m_listeners;
	uint32 m_cacheSize;
	uint32 m_peersCacheSize;
	uint32 m_databasesCacheSize;
	mutable boost::recursive_mutex m_cs;
	shared_ptr<IsisService> m_isisService;

	shared_ptr<HttpVirtualDirectory> m_mainDirectory;
	shared_ptr<HttpVirtualDirectory> m_portalsDirectory;
	shared_ptr<HttpEnumeratedDirectoryCallback> m_tempDirectory;

public:
	static const String MAIN_DIRECTORY;
	static const String PORTALS_DIRECTORY;
	static const String TEMP_DIRECTORY;

	static const String PORTAL_PAGE_ACCOUNT;
	static const String PORTAL_PAGE_EDIT;
	static const String PORTAL_PAGE_USER;
	static const String PORTAL_PAGE_USERS;
	static const String PORTAL_PAGE_VIEW;	
	static const String PORTAL_PAGE_OBJECTDETAIL;
	static const String PORTAL_PAGE_ASSISTANT;
	static const String PORTAL_PAGE_SEARCH;
	static const String PORTAL_PAGE_MESSENGER;

	static const String MAIN_PAGE_ACCOUNTS;	
	static const String MAIN_PAGE_SETTINGS;
	static const String MAIN_PAGE_RESOURCES;
	static const String MAIN_PAGE_MAINTENANCE;
	static const String MAIN_PAGE_SUBSCRIBE;
	static const String MAIN_PAGE_ISIS;
	static const String MAIN_PAGE_JOBDETAIL;
	static const String MAIN_PAGE_JOBS;
	static const String MAIN_PAGE_IMPORT;
	static const String MAIN_PAGE_EXPORT;
	static const String MAIN_PAGE_OPTIONS;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<HttpVirtualDirectory> PortalsSystem::getPortalsDirectory() const { return m_portalsDirectory; }
inline shared_ptr<HttpVirtualDirectory> PortalsSystem::getMainDirectory() const { return m_mainDirectory; }
inline shared_ptr<HttpEnumeratedDirectoryCallback> PortalsSystem::getTempDirectory() const { return m_tempDirectory; }

inline bool PortalsSystem::hasPortal(const PortalID &id, const PovID &pov) const { return getPortal(id,pov) != null; }
inline bool PortalsSystem::hasPortalByFullPov(const String &id) const { return getPortalByFullPov(id) != null; }

inline shared_ptr<IsisService> PortalsSystem::getIsisService() const { return m_isisService; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _PORTALS_PORTALSSYSTEM_H
