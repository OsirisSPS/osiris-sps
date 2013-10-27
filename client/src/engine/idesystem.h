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

#ifndef _IDE_IDESYSTEM_H
#define _IDE_IDESYSTEM_H

#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "eventsource.h"
#include "ideide.h"
#include "ids.h"
#include "ievent.h"
#include "isystem.h"
#include "locked.h"
#include "portalsportals.h"
#include "ordered_map.h"
#include "singleton.h"
#include "uniqueid.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IHttpServer;
class IdeAccountsManager;
class IdeSession;
class IdeSkin;
class IPage;
template <typename T> class UPnPServer;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeSystem : public DynamicSingleton<IdeSystem>,
							   public ISystem
{	
public:
	typedef ordered_map<UniqueID, shared_ptr<IdeSkin> >::type Skins;
	typedef Collections<String, IdeSession>::shared_collection SessionsCollection;

// Events
public:
	class EventBeforePageRender : public IEvent
	{
		// Attributes
		public:
			const shared_ptr<IPage> & getPage() const;
			void setPage(const shared_ptr<IPage> &page);

		protected:
			shared_ptr<IPage> m_page;
	};

	class EventAfterPageRender : public IEvent
	{
		// Attributes
		public:
			const String & getHtml() const;
			void setHtml(const String &html);

		protected:
			String m_html;
	};

// Internal
private:
	class HttpServer;

// Construction
public:
	IdeSystem();
	virtual ~IdeSystem();

// Attributes
public:
	shared_ptr<IHttpServer> getServer() const;
	//shared_ptr<IdeAccountsManager> getAccountsManager() const;

	shared_ptr<IdeSkin> getSkin(const SkinID &id, bool load = true) const;
	shared_ptr<IdeSkin> getDefaultSkin() const;
	Locked<const Skins>::unique getAvailableSkins() const;
	
	String getLocalUrl(const String &relativeUrl = String::EMPTY) const;

	String getHtdocsPath() const;

	bool isTrustedDomain(const std::string& domain);

	inline shared_ptr<EventSource> getEventBeforePageRender() const;
	inline shared_ptr<EventSource> getEventAfterPageRender() const;

// Operations
public:

	void registerSkin(shared_ptr<IdeSkin> skin);
	void unregisterSkin(shared_ptr<IdeSkin> skin);

	shared_ptr<IdeSession> getSession(const String &id) const;
	shared_ptr<IdeSession> createSession();
	void logoutSessions(const String &id);
	bool isAccountLogged(const String &id);
	bool removeAccount(const String &id);

	String trustDomainFromUrl(const std::string& url);
	void trustDomain(const String& domain);

// Operations
private:	
	void updateServer();
	void stopServer();
	
	void updateOptions();

	bool loadBaseSkins();
	//bool updateDefaultSkin();

// ISystem interface
public:
	virtual String getName() const;

	virtual bool start(bool recovery);
	virtual void stop();

public:
	//static const String SKINS_ALIAS;	

protected:
	mutable boost::recursive_mutex m_cs;
	mutable boost::recursive_mutex m_sessionsCS;
	SessionsCollection m_sessions;
	shared_ptr<HttpServer> m_server;
	mutable Skins m_skins;
		
	shared_ptr<EventSource> m_eventBeforePageRender;
	shared_ptr<EventSource> m_eventAfterPageRender;
};

//////////////////////////////////////////////////////////////////////

inline const shared_ptr<IPage> & IdeSystem::EventBeforePageRender::getPage() const { return m_page; }
inline void IdeSystem::EventBeforePageRender::setPage(const shared_ptr<IPage> &page) { m_page = page; }
inline const String & IdeSystem::EventAfterPageRender::getHtml() const { return m_html; }
inline void IdeSystem::EventAfterPageRender::setHtml(const String &html) { m_html = html; }

inline shared_ptr<EventSource> IdeSystem::getEventBeforePageRender() const { return m_eventBeforePageRender; }
inline shared_ptr<EventSource> IdeSystem::getEventAfterPageRender() const { return m_eventAfterPageRender; }

//inline shared_ptr<IdeAccountsManager> IdeSystem::getAccountsManager() const { return m_accountsManager; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_IDESYSTEM_H
