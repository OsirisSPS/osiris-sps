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
#include "isisendpoint.h"

#include "connectionsmanager.h"
#include "engine.h"
#include "entitiessnapshotmanager.h"
#include "isisconnection.h"
#include "isisservice.h"
#include "lock.h"
#include "options.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IsisEndpoint::IsisEndpoint(const PortalID &portalID, const PovID &povID) : m_portalID(portalID),
																		   m_povID(povID),
																		   m_enabled(true)
{
	// URGENT, da pulire, mettere #define, costanti etc.
	//m_isisOptions.ensureOption(_S("isis.user"), String::EMPTY, false);
	m_isisOptions.ensureOption(_S("isis.redirect"), String::EMPTY, false);

	m_isisOptions.ensureOption(Options::ide_options::objects_hot_state_threshold, static_cast<uint32>(15), false);
	m_isisOptions.ensureOption(Options::ide_options::pager_items, static_cast<uint32>(20), false);
	m_isisOptions.ensureOption(Options::ide_options::pager_pages, static_cast<uint32>(5), false);
	m_isisOptions.ensureOption(Options::ide_options::skin_id, OS_IDE_SKIN_DEFAULT, false);

	m_isisOptions.ensureOption(Options::url_options::protocols_find_link, _S("osiris|http|https|ftp|ftps|gopher|news|nntp|telnet|wais|file|mailto|ed2k|ants2k|magnet|stealthnet"), false);
	m_isisOptions.ensureOption(Options::url_options::protocols_no_redirect, _S("osiris|gopher|news|nntp|telnet|wais|file|mailto|ed2k|ants2k|magnet|stealthnet|http://localhost:23402/offsystem/.*"), false);
	m_isisOptions.ensureOption(Options::url_options::protocols_no_confirm, _S("osiris|mailto|ed2k|ants2k|magnet|stealthnet|http://localhost:23402/offsystem/.*"), false);
	m_isisOptions.ensureOption(Options::url_options::protocols_collect, _S("osiris|ed2k|ants2k|magnet|stealthnet|http://localhost:23402/offsystem/.*"), false);

	m_isisOptions.ensureOption(Options::privacy_options::allow_external_images, true, false);
	m_isisOptions.ensureOption(Options::privacy_options::allow_external_urls, true, false);
	//m_isisOptions.ensureOption(Options::anonymity_options::html_mode, 0);
	m_isisOptions.ensureOption(Options::anonymity_options::object_mode, 1, false);
	//m_isisOptions.ensureOption(Options::anonymity_options::object_trusted_domains, String::EMPTY);
	//m_isisOptions.ensureOption(Options::anonymity_options::object_trusted_types, String::EMPTY);

	//m_isisOptions.ensureOption(Options::web_options::redirect_url, _S("http://anonym.to/?#url#"));
	m_isisOptions.ensureOption(Options::web_options::redirect_url, _S("https://isis.osiris-sps.org/redirect.php?#url#"), false);	
}

IsisEndpoint::~IsisEndpoint()
{
	close();
}

String IsisEndpoint::getName() const
{
	OS_LOCK(m_cs);
	return m_name;
}

void IsisEndpoint::setName(const String &name)
{
	OS_LOCK(m_cs);
	m_name = name;
}

HttpUrl IsisEndpoint::getUrl() const
{
	OS_LOCK(m_cs);
	return m_url;
}

void IsisEndpoint::setUrl(const HttpUrl &url)
{
	OS_LOCK(m_cs);
	m_url = url;
}

bool IsisEndpoint::getEnabled() const
{
	OS_LOCK(m_cs);
	return m_enabled;
}

void IsisEndpoint::setEnabled(bool enabled)
{
	OS_LOCK(m_cs);
	m_enabled = enabled;
}

String IsisEndpoint::getPassword() const
{
	OS_LOCK(m_cs);
	return m_password;
}

void IsisEndpoint::setPassword(const String &password)
{
	OS_LOCK(m_cs);
	m_password = password;
}

/*
ObjectID IsisEndpoint::getUser() const
{
	OS_LOCK(m_cs);
	return m_userID;
}

void IsisEndpoint::setUser(const ObjectID &userID)
{
	OS_LOCK(m_cs);
	m_userID = userID;
}
*/

String IsisEndpoint::getLastEvent() const
{
	OS_LOCK(m_cs);
	return m_lastEvent;
}

void IsisEndpoint::setLastEvent(const String &lastEvent)
{
	OS_LOCK(m_cs);
	m_lastEvent = lastEvent;
}

bool IsisEndpoint::isValid() const
{
	OS_LOCK(m_cs);
	return m_url.empty() == false;
}

bool IsisEndpoint::matchUrl(const HttpUrl &url)
{
	OS_LOCK(m_cs);
	return m_url == url;
}

void IsisEndpoint::update(shared_ptr<ConnectionsManager> connectionsManager)
{
	OS_LOCK(m_cs);		

	if(canStart() == false)
		return;

	if(m_url.empty())
	{
		OS_ASSERTFALSE();
		return;
	}	

	if(connectionsManager == null)
	{
		OS_ASSERTFALSE();
		return;
	}	

	shared_ptr<IsisConnection> activeConnection = getActiveConnection();
	if(activeConnection != null)
		return;	

	shared_ptr<Portal> portal = PortalsSystem::instance()->getPortal(m_portalID, m_povID);
	if(portal == null)
		return;	

	activeConnection.reset(OS_NEW IsisConnection(portal, get_this_ptr(), connectionsManager, Engine::instance()->createTCPSocket(connectionsManager->getService(), true, true), m_url));
	m_activeConnection = activeConnection;
	connectionsManager->addConnection(activeConnection, true);
}

void IsisEndpoint::close()
{
	// N.B.: non resettare qui i dati

    OS_LOCK(m_cs);

	shared_ptr<IsisConnection> activeConnection = getActiveConnection();
	if(activeConnection != null)
		activeConnection->notifyCompleted();
}

void IsisEndpoint::parseSystemInformation(shared_ptr<XMLNode> root)
{
	OS_LOCK(m_cs);
	
	shared_ptr<XMLNode> optionsNode = root->getNode(OS_XMLOPTIONS_NODE_ROOT);
	if(optionsNode != null)
	{
		m_isisOptions.parseXMLNode(optionsNode);

		//m_user = static_cast<String>(m_isisOptions.getOption(_S("isis.user"))).to_ascii();
	}
}

/*
String IsisEndpoint::getSnapshotProfileID()
{
	if(m_portalID.isMonarchic())
	{
		return entities::SnapshotProfile::GLOBAL;
	}
	else
	{
		return getUser().getHash();		
	}
}
*/

bool IsisEndpoint::canStart()
{
	if(m_enabled == false)
	{
		return false;	
	}
	
	shared_ptr<Portal> portal = PortalsSystem::instance()->getPortal(m_portalID, m_povID);
	if(portal == null)
	{
		setLastEvent(_S("Disabled, portal not found."));
		return false;	
	}

	/*
	entities::snapshot_profile_ptr profile = portal->getSnapshotManager()->getProfile(getSnapshotProfileID());
	if(profile == null)
	{
		// Non c'è il profilo, è necessario chiamare la "system" per costruirlo, quindi ok.
		return true;
	}
	else
	{
	*/
		//bool snapshotReady = ( (profile != null) && (profile->getStabilityStatus() == entities::ssDone) );
		bool snapshotReady = (portal->getSnapshotManager()->getStabilityStatus() == EntitiesSnapshotManager::ssDone);
		if(snapshotReady)
		{
			return true;
		}
		else
		{
			setLastEvent(_S("Disabled, waiting for stabilization."));
			return false;
		}
	/*}*/
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
