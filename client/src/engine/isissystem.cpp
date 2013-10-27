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
#include "isissystem.h"

#include "archivesmanager.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "engine.h"
#include "httpurl.h"
#include "isis.h"
#include "log.h"
#include "buffer.h"
#include "filesystem.h"
#include "ntp.h"
#include "ntpclient.h"
#include "path.h"
#include "portalspeer.h"
#include "portalspeersmanager.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "options.h"
#include "platformmanager.h"
#include "xmlattribute.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SYSTEM(OS_NAMESPACE_NAME::IsisSystem)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////


IsisSystem::IsisSystemThread::IsisSystemThread()
{
}

IsisSystem::IsisSystemThread::~IsisSystemThread()
{
}

bool IsisSystem::IsisSystemThread::executeTask()
{
	uint32 result = IsisSystem::instance()->doTask(false);

	if(result != 0)
		m_delay = result;
	else
		m_delay = 60 * 1000;
	
	return false;	 // false: wait, true: reexecute now.
}

//////////////////////////////////////////////////////////////////////

IsisSystem::IsisSystem()
{
	// Ogni tot, chiama la "system" di Isis.
	// Restituisce:
	// - Gli aggiornamenti di extensions disponibili
	// - Lo stato di allineamento dei portali noti
	// - Ultima versione del software

	m_status = 0;
	m_online = false;	
	m_lastRequest.clear();
	m_ip.clear();
	m_reachable = false;
	m_tor = 0;
	m_lastValidationDate.clear();
	m_lastCheckDate.clear();

	m_intervalUpdate = 60;
	m_nMaxPortals = 2;

	m_lastClockSynchronization = 0;
}

IsisSystem::~IsisSystem()
{
}

String IsisSystem::getName() const
{
	return _S("isis");
}

bool IsisSystem::start(bool recovery)
{
	m_thread.reset(OS_NEW IsisSystemThread());

	m_thread->start();
	
	return true;
}

void IsisSystem::stop()
{	
	m_thread->stop();
}

uint32 IsisSystem::doTask(bool force)
{
	OS_LOCK(m_cs);

	uint32 result = 0;
	try
	{
		result = requestIsis(force);
	}
	catch(std::exception &e)
	{
		m_lastError = e.what();		
		result = 0;
	}
		
	if( (result == 0) && (isInternetDateTimeAvailable() == false) )
	{
		// Can't have the internet clock from Isis, try with NTP.
		doSynchronizeClockWithNTP();
	}

	m_status = 2; // Green

	if(m_reachable == false)
		m_status = 1; // Yellow
	
	if(isInternetDateTimeAvailable() == false)
		m_status = 0; // Red

	// Note: If Isis can't be reached, is yellow. Only if also NTP can't be reached, is red.

	return result;
}

bool IsisSystem::isInternetDateTimeAvailable()
{
	OS_LOCK(m_clockCS);

	return (m_lastClockSynchronization != 0);
}

DateTime IsisSystem::getInternetDateTime()
{
	if(isInternetDateTimeAvailable() == false)
		return DateTime::EMPTY;

	OS_LOCK(m_clockCS);

#ifdef OS_ENABLE_NTP_DIFFERENCES // TOCLEAN?
	DateTime test;
	uint32 delta = static_cast<uint32>(PlatformManager::instance()->tickCount());
	test = getClockWithNTP();
	delta = static_cast<uint32>(PlatformManager::instance()->tickCount()) - delta;
#endif // OS_ENABLE_NTP_DIFFERENCES

	// Calcola l'offset trascorso dall'ultima sincronizzazione
	TimeDuration offset = TimeDuration::millisec(static_cast<uint32>(PlatformManager::instance()->tickCount()) - m_lastClockSynchronization);
	// Calcola l'ora ntp corrente
	DateTime dt = m_synchronizedClock + offset;

#ifdef OS_ENABLE_NTP_DIFFERENCES
	// Verifica che la richiesta ntp sia andata a buon fine
	if(test.isNull() == false)
	{
		DateTime maxTime = std::max(test, dt);
		DateTime minTime = std::min(test, dt);
		// Calcola l'errore
		TimeDuration difference = maxTime - minTime;
		uint32 delta_seconds = TimeDuration::millisec(delta).getTotalSeconds();
		//OS_ASSERT((difference.getTotalSeconds() - delta_seconds) <= 1);	// Se lo scarto è oltre il secondo c'è qualcosa che non va...
		OS_LOG_DEBUG(String::format(_S("difference from ntp %d second(s)").c_str(), difference.getTotalSeconds()));
	}
#endif // OS_ENABLE_NTP_DIFFERENCES

	return dt;
}

String IsisSystem::getUrl(ordered_map<std::wstring, std::wstring> &params) const
{
	return Options::instance()->getIsisSecureLink(OS_ISIS_PAGE_OSIRIS, params);
}

/*
String IsisSystem::getPortalUrl(shared_ptr<Portal> portal) const
{
	ordered_map<std::wstring, std::wstring> params;
	
	params.set(OS_ISIS_PAGE_OSIRIS_PARAM_ACTION, OS_ISIS_PAGE_OSIRIS_ACTIONS_PORTAL);
	params.set(OS_ISIS_PAGE_OSIRIS_PARAM_PORTAL, portal->getPortalID().toWide());
	params.set(OS_ISIS_PAGE_OSIRIS_PARAM_POV, portal->getPovID().toWide());
	params.set(OS_ISIS_PAGE_OSIRIS_PARAM_NODE, String(Engine::instance()->getMachineID()).to_wide());
	params.set(OS_ISIS_PAGE_OSIRIS_PARAM_ALIGN, portal->getOptions()->getAlignHash().toWide());
	params.set(OS_ISIS_PAGE_OSIRIS_PARAM_PEERS, conversions::to_wstring<uint32>(OS_ISIS_DEFAULT_PEERS_REQUEST));
	// Notifica la porta di listening solo se non è settato un proxy
	if(Options::instance()->getOption<uint32>(Options::net_options::proxy_type) == OS_PROXY_TYPE_NONE)
		params.set(OS_ISIS_PAGE_OSIRIS_PARAM_PORT, conversions::to_wstring(Options::instance()->getOption<uint32>(Options::p2p_options::server_port)));

	return getUrl(params);
}
*/

/*
void IsisSystem::updatePortal(shared_ptr<Portal> portal)
{
	portal->updateIsisStatus();
}
*/

std::string IsisSystem::resolveItemLink(const std::string &id)
{
	// Se il portale Osiris Official c'è, restituisce un url locale.
	// else...
	return Options::instance()->getIsisLink(OS_ISIS_PAGE_LINK, "id=" + id);
}

uint32 IsisSystem::requestIsis(bool force)
{
	DateTime now = DateTime::now();
		
	shared_ptr<boost::asio::io_service> service = createAsioService();
	if(service == null)
	{
		m_lastRequest = now;
		m_lastError = _S("No Asio service.");
		return 0;
	}

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_ISIS_PAGE_OSIRIS_PARAM_NODE, String(Engine::instance()->getMachineID()).to_wide());
	if(force)
		params.set(OS_ISIS_PAGE_OSIRIS_PARAM_FORCE, _W("true"));
	
	// Notifica la porta di listening solo se non è settato un proxy
	if(Options::instance()->getOption<uint32>(Options::net_options::proxy_type) == OS_PROXY_TYPE_NONE)
		params.set(OS_ISIS_PAGE_OSIRIS_PARAM_PORT, conversions::to_wstring(Options::instance()->getOption<uint32>(Options::p2p_options::server_port)));

	// Collect portals request info
	String portalsRequest;

	for(int iIndex = 0; iIndex <= m_nMaxPortals; iIndex++)
	{
		DateTime olderDate;
		shared_ptr<Portal> olderPortal;

		PortalsSystem::Portals portals = PortalsSystem::instance()->getPortals();
		for(PortalsSystem::Portals::const_iterator i = portals.begin(); i != portals.end(); ++i)
		{
			shared_ptr<Portal> portal = *i;

			if(portal->getOptions()->getExchangeEnabled())
			{			
				if( (olderPortal == null) || (portal->getLastIsisRequest()<olderDate) )
				{
					olderDate = portal->getLastIsisRequest();
					olderPortal = portal;
				}			
			}
		}

		if(olderPortal == null)
			break;
		else
		{
			if(portalsRequest.empty() == false)
				portalsRequest += _S(";");
			portalsRequest += _S("portal:") + olderPortal->getPortalID().toWide() + _S(",pov:") + olderPortal ->getPovID().toWide() + _S(",align:") + olderPortal->getOptions()->getAlignHash().toUTF16();
			olderPortal->setLastIsisRequest(DateTime::now());
		}
	}

	params.set(OS_ISIS_PAGE_OSIRIS_PARAM_PORTALS, portalsRequest.to_wide());

	String url = Options::instance()->getIsisSecureLink(OS_ISIS_PAGE_OSIRIS, params);

	if(document->parseUrl(url.to_ascii(), Engine::getDefaultHttpUserAgent(), service, Engine::instance()->createTCPSocket(service, true, true)) == false)
	{
		m_lastRequest = now;
		m_lastError = _S("Unable to contact Isis Server.");
		return 0;
	}

	// Reset
	m_lastRequest = now;
	m_lastError = String::EMPTY;
	m_online = false;
	m_status = 0;
	m_latestOsirisVersion = "";
	m_latestOsirisNotes = "";
	m_ip = "";
	m_countryCode = String::EMPTY;
	m_countryName = String::EMPTY;


	shared_ptr<XMLNode> root = document->getRoot();

	if(root->getName() == _S("error"))
	{
		String message = root->getAttributeString(_S("msg"));

		m_lastError = message;
		return 0;
	}

	// Clock Update
	m_synchronizedClock = root->getAttributeUint32("clock");
	m_lastClockSynchronization = static_cast<uint32>(PlatformManager::instance()->tickCount());
	m_synchronizedClockMethod = "isis";



	

	
	m_online = true;

	m_latestOsirisVersion = root->getAttributeString("latest_osiris_version").to_ascii();
	m_latestOsirisNotes = root->getAttributeString("latest_osiris_notes").to_ascii();
	m_ip = root->getAttributeString("ip").to_ascii();
	m_countryCode = root->getAttributeString("country_code");
	m_countryName = root->getAttributeString("country_name");
	m_reachable = root->getAttributeBool("reachable");
	m_tor = root->getAttributeInt32("tor");
	m_lastValidationDate = time_t(root->getAttributeUint32("last_validation_date"));
	m_lastCheckDate = root->getAttributeUint32("last_check_date");

	
	
	// Isis Server-side load limits
	m_nMaxPortals = root->getAttributeInt32("nMaxPortals");
	m_intervalUpdate = root->getAttributeInt32("intervalUpdate");
	if(m_intervalUpdate == 0)
		m_intervalUpdate = 600;
	
	// Read Portals
	shared_ptr<XMLNode> nodePortals = root->getNode(_S("portals"));
	if(nodePortals != null)
	{
		shared_ptr<XMLNodes> nodesPortals = nodePortals->getNodes();
		for(XMLNodes::const_iterator i = nodesPortals->begin(); i != nodesPortals->end(); ++i)
		{
			shared_ptr<XMLNode> nodePortal = *i;
						
			shared_ptr<Portal> portal = PortalsSystem::instance()->getPortal(nodePortal->getAttributeString("portal").to_ascii(), nodePortal->getAttributeString("pov").to_ascii());
			if(portal != null)
			{
				// Read info
				int nodesSameAlign = nodePortal->getAttributeUint32("nodesSameAlign");
				int nodesSamePortalPov = nodePortal->getAttributeUint32("nodesSamePortalPov");

				portal->setNodesSameAlign(nodesSameAlign);
				portal->setNodesSamePortalPov(nodesSamePortalPov);

				// Read peers
				shared_ptr<XMLNode> nodePeers = nodePortal->getNode(_S("nodes"));
				if(nodePeers != null)
				{
					shared_ptr<XMLNodes> nodesPeers = nodePeers->getNodes();
					for(XMLNodes::const_iterator i = nodesPeers->begin(); i != nodesPeers->end(); ++i)
					{
						shared_ptr<XMLNode> nodePeer = *i;

						String ip = (*i)->getAttributeString(_S("ip"));
				                
						uint32 port = 0;
						shared_ptr<XMLAttribute> portAttribute = (*i)->getAttribute(_S("port"));
						if(portAttribute != null)
							port = conversions::from_utf16<uint32>(portAttribute->getValue());

						if(port == 0)
							port = OS_P2P_OLD_DEFAULT_SERVER_PORT;

						portal->getPeersManager()->savePeer(ip, port, false, OS_PEER_ORIGIN_ISIS);
					}
				}
			}
		}
	}
		
	return m_intervalUpdate * 1000;
}

void IsisSystem::doSynchronizeClockWithNTP()
{
	DateTime dt = getClockWithNTP();
	if(dt.isNull() == false)
	{
		m_synchronizedClock = dt;
		m_lastClockSynchronization = static_cast<uint32>(PlatformManager::instance()->tickCount());
		m_synchronizedClockMethod = "ntp";
	}
}

DateTime IsisSystem::getClockWithNTP()
{
	// N.B.: creare qui un io_service temporaneo da passare a NTPClient::query perchè internamente chiama una io_service::run (va ristrutturata)
	shared_ptr<boost::asio::io_service> service = createAsioService();
	if(service == null)
	{
		OS_ASSERTFALSE();
		return DateTime::EMPTY;
	}

	int retries = 3;

	while(retries > 0)
	{
		shared_ptr<NTPResponse> response = NTPClient::query(service, Engine::instance()->createUDPSocket(service, true, true), OS_NTP_DEFAULT_SERVER_NAME, OS_NTP_DEFAULT_SERVER_PORT);
		if(response != null)
		{
			DateTime dt = static_cast<boost::posix_time::ptime>(response->transmit_time);
			return dt;
		}

		retries--;
	}

	return DateTime::EMPTY;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
