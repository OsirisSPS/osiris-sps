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
#include "idesystem.h"

#include "boost/bind.hpp"
#include "conversions.h"
#include "cryptmanager.h"
#include "engine.h"
#include "extensionsextension.h"
#include "extensionssystem.h"
#include "filesystem.h"
#include "format.h"
#include "htmlparser.h"
#include "httpurl.h"
#include "httpsession.h"
#include "ihttpserver.h"
#include "ideaccount.h"
#include "ideaccountsmanager.h"
#include "idesession.h"
#include "ideskin.h"
#include "languageculture.h"
#include "languagemanager.h"
#include "log.h"
#include "networksystem.h"
#include "objectsuser.h"
#include "options.h"
#include "platformmanager.h"
#include "portalsportalssystem.h"
#include "upnpserver.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SYSTEM(OS_NAMESPACE_NAME::IdeSystem)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

using namespace ide;

//////////////////////////////////////////////////////////////////////

//const String IdeSystem::SKINS_ALIAS = _S("skins");

//////////////////////////////////////////////////////////////////////

class IdeSystem::HttpServer : public UPnPServer<IHttpServer>
{
	typedef UPnPServer<IHttpServer> HttpServerBase;

// Construction
public:
	HttpServer(const std::string &name, bool allowRemoteConnections = false, uint32 sessionDuration = OS_HTTP_DEFAULT_SESSION_DURATION, boost::optional<uint32> workerThreadsCount = boost::none);
	virtual ~HttpServer();

// IServer interface
protected:
	virtual shared_ptr<IConnection> createConnection();
};

//////////////////////////////////////////////////////////////////////

IdeSystem::HttpServer::HttpServer(const std::string &name, bool allowRemoteConnections, uint32 sessionDuration, boost::optional<uint32> workerThreadsCount) : HttpServerBase(name, allowRemoteConnections, sessionDuration, workerThreadsCount)
{

}

IdeSystem::HttpServer::~HttpServer()
{

}

shared_ptr<IConnection> IdeSystem::HttpServer::createConnection()
{
	return shared_ptr<IConnection>(OS_NEW HttpSession(get_this_ptr<IHttpServer>(), Engine::instance()->createTCPSocket(getService(), false, false)));
}

//////////////////////////////////////////////////////////////////////

IdeSystem::IdeSystem() : m_server(OS_NEW HttpServer(Engine::getDefaultHttpUserAgent())),
						 //m_accountsManager(null),
						 m_eventBeforePageRender(OS_NEW EventSource()),
						 m_eventAfterPageRender(OS_NEW EventSource())
{
	Options::instance()->subscribeUpdateHandler(boost::bind(&IdeSystem::updateOptions, this));
}

IdeSystem::~IdeSystem()
{

}

shared_ptr<IHttpServer> IdeSystem::getServer() const 
{ 
	return m_server; 
}

shared_ptr<IdeSkin> IdeSystem::getSkin(const SkinID &id, bool load) const
{
	OS_LOCK(m_cs);

	Skins::const_iterator i = m_skins.find(id);
	if(i != m_skins.end())
		return i->second;

	if(load == false)
		return null;

	// 0.14
	return null;

	// 0.13
	/*
	shared_ptr<IdeSkin> skin(OS_NEW IdeSkin());
	if(skin->load(id.to_ascii()) == false)
		skin.reset();		// Salva comunque il riferimento alla skin (invalido) per evitare di caricarla successivamente

	m_skins[id] = skin;
	return skin;
	*/

}

shared_ptr<IdeSkin> IdeSystem::getDefaultSkin() const
{
	OS_LOCK(m_cs);
	
	SkinID defaultSkinID = SkinID(Options::instance()->getOption<String>(Options::ide_options::skin_id).to_ascii());
	shared_ptr<IdeSkin> defaultSkin = getSkin(defaultSkinID, false);
	if(defaultSkin == null)
	{
		defaultSkinID = SkinID(OS_IDE_SKIN_DEFAULT.to_ascii());
		defaultSkin = getSkin(defaultSkinID, false);
		OS_ASSERT(defaultSkin);
	}

	return defaultSkin;	
}

Locked<const IdeSystem::Skins>::unique IdeSystem::getAvailableSkins() const
{
	return Locked<const Skins>::unique(m_skins, m_cs);
}

String IdeSystem::getLocalUrl(const String &relativeUrl) const
{
	OS_ASSERT(m_server != null);

	String localUrl;
	if(m_server->running())
	{
		uint32 port = 0;
		if(m_server->getBindPort(port))
		{
			localUrl = _S("http://127.0.0.1");		
			if(port != OS_HTTP_HTTP_PORT_DEFAULT)
				localUrl += _S(":") + conversions::to_utf16(port);

			if(relativeUrl.empty() == false)
			{
				if(relativeUrl.starts_with(_S("/")) == false)
					localUrl += _S("/");

				localUrl += relativeUrl;
			}
		}
	}

	return localUrl;
}

String IdeSystem::getHtdocsPath() const
{
	static String path = utils::makeFolderPath(ExtensionsSystem::instance()->getDataPath(), _S("/") + OS_EXTENSIONS_CORE + _S("/htdocs"));
	return path;
}

bool IdeSystem::isTrustedDomain(const std::string& domain)
{
	return true;
}

String IdeSystem::trustDomainFromUrl(const std::string& url)
{
	return url;
}

void IdeSystem::trustDomain(const String& domain)
{	
}

void IdeSystem::updateServer()
{
	// N.B.: il server non va resettato o si rischia di perdere eventuali directories sottoscritte dall'esterno
	
	try
	{
		OS_ASSERT(m_server != null);

		// Controlla se l'allineamento è disabilitato
		if(Options::instance()->getOption<bool>(Options::web_options::enable) == false)
		{
			stopServer();
			return;
		}

		bool needRestart = false;

		String bindIP = Options::instance()->getOption<String>(Options::web_options::server_bind_ip);
		uint32 bindPort = Options::instance()->getOption<uint32>(Options::web_options::server_port);
		bool allowRemoteConnections = Options::instance()->getOption<bool>(Options::web_options::allow_remote_connections);
		bool enableUPnP = allowRemoteConnections && Options::instance()->getOption<bool>(Options::web_options::server_enable_upnp);
		uint32 hardwareConcurrency = Options::instance()->getHardwareConcurrency();

		if(m_server->running())
		{
			boost::asio::ip::address currentBindAddress;
			if(m_server->getBindAddress(currentBindAddress))
			{
				if(NetworkSystem::instance()->compareAddresses(bindIP.to_ascii(), currentBindAddress) == false)
					needRestart = true;
			}

			uint32 currentBindPort = 0;
			if(m_server->getBindPort(currentBindPort))
			{
				if(currentBindPort != bindPort)
					needRestart = true;
			}

			if(m_server->getUPnPEnabled() != enableUPnP)
				needRestart = true;

			if(m_server->getWorkerThreadsCount() != hardwareConcurrency)
				needRestart = true;
		}
		else
		{
			needRestart = true;			
		}

		if(needRestart)
		{
			stopServer();

			m_server->setWorkerThreadsCount(hardwareConcurrency);
			m_server->setUPnPEnabled(enableUPnP);

			if(m_server->bind(bindIP, bindPort))
			{
				uint32 listeningPort = 0;
				if(m_server->getBindPort(listeningPort))
					OS_LOG_INFO(String::format(_S("Web server listening on port '%d'").c_str(), listeningPort));

				m_server->start();	
			}
			else
			{
				if(bindIP.empty())
					OS_LOG_CRITICAL(String::format(_S("Web server error: cannot bind port '%d'").c_str(), bindPort));
				else
					OS_LOG_CRITICAL(String::format(_S("Web server error: cannot bind '%S:%d'").c_str(), bindIP.c_str(), bindPort));
			}
		}

		m_server->setAllowRemoteConnections(allowRemoteConnections);				
		m_server->setSessionsDuration(TimeDuration::minutes(Options::instance()->getOption<uint32>(Options::web_options::session_duration)));

		if(bindPort == OS_DEFAULT_WEB_PORT)
		{
			if(m_server->getBindPort(bindPort))
				// Se si tratta del primo avvio salva la porta di ascolto
				Options::instance()->setOption(Options::web_options::server_port, bindPort);
		}
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(_S("Web server error: ") + e.what());
	}
}

void IdeSystem::stopServer()
{
	OS_ASSERT(m_server != null);
	m_server->stop();
}

void IdeSystem::updateOptions()
{
	//updateDefaultSkin();
	updateServer();
}

bool IdeSystem::loadBaseSkins()
{
	OS_LOCK(m_cs);

	m_skins.clear();
	
	return true;
}

/*
bool IdeSystem::updateDefaultSkin()
{
	OS_LOCK(m_cs);

	m_defaultSkin = UniqueID(Options::instance()->getOption<String>(Options::ide_options::skin_id).to_ascii());
	shared_ptr<IdeSkin> defaultSkin = getSkin(m_defaultSkin, true);
	if(defaultSkin == null)
		return false;

	String skinSchema = Options::instance()->getOption<String>(Options::ide_options::skin_schema);
	if(skinSchema.empty() == false)
		defaultSkin->setSchema(skinSchema);
		
	return true;
}
*/

void IdeSystem::registerSkin(shared_ptr<IdeSkin> skin)
{
	OS_LOCK(m_cs);

	if(skin == null)
		return;

	OS_LOG_INFO("Skin '" + skin->getName() + "' registered.");

	m_skins[skin->getID()] = skin;
}

void IdeSystem::unregisterSkin(shared_ptr<IdeSkin> skin)
{
	OS_LOCK(m_cs);

	if(skin == null)
		return;

	if(m_skins.exists(skin->getID()))
	{
		OS_LOG_INFO("Skin '" + skin->getName() + "' unregistered.");
	
		m_skins.erase(skin->getID());
	}
}

shared_ptr<IdeSession> IdeSystem::getSession(const String &id) const
{
	OS_LOCK(m_sessionsCS);
	return m_sessions.get(id);
}

shared_ptr<IdeSession> IdeSystem::createSession()
{
	OS_LOCK(m_sessionsCS);

	String sessionID = CryptManager::instance()->randomSHA().toHex();

	// Crea l'utente loggato
	shared_ptr<IdeSession> session(OS_NEW IdeSession(sessionID));
	// Associa la sessione all'utente loggato
	m_sessions.push_back(sessionID, session);

	return session;
}

void IdeSystem::logoutSessions(const String &id)
{
	OS_LOCK(m_sessionsCS);

	// Scorre tutti gli utenti connessi al portale
	for(SessionsCollection::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i)
	{
		if(i->second->getAccount()->getID() == id)
			// Invalida l'utente corrente se collegato con l'account rimosso
			i->second->logout();
	}
}

bool IdeSystem::isAccountLogged(const String &id)
{
	OS_LOCK(m_sessionsCS);

	// Scorre tutti gli utenti connessi al portale
	for(SessionsCollection::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i)
	{
		shared_ptr<IdeSession> session = i->second;
		if( (session->isLogged()) && (session->getAccount()->getID() == id) )
			return true;
	}

	return false;
}

bool IdeSystem::removeAccount(const String &id)
{
	logoutSessions(id);

	return IdeAccountsManager::instance()->remove(id);
}


String IdeSystem::getName() const
{
	return _S("ide");
}

bool IdeSystem::start(bool recovery)
{	
	if(loadBaseSkins() == false)
	{
		OS_LOG_ERROR(_S("Cannot load base skin"));
		return false;
	}

	//m_accountsManager.reset(OS_NEW IdeAccountsManager());

	/*
	if(updateDefaultSkin() == false)
	{
		OS_LOG_ERROR(_S("Cannot load default skin"));
		return false;
	}
	*/

	updateServer();

	return true;
}

void IdeSystem::stop()
{
	stopServer();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
