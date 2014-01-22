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
#include "p2pconnection.h"

#include "boost/bind.hpp"
#include "convert.h"
#include "connectionstatusguard.h"
#include "cryptkeyagreement.h"
#include "cryptmanager.h"
#include "dataentry.h"
#include "datapeer.h"
#include "iportaldatabase.h"
#include "ipaddress.h"
#include "lock.h"
#include "logmanager.h"
#include "objectsiobject.h"
#include "objectssystem.h"
#include "osiriscommon.h"
#include "p2pexchangecontext.h"
#include "p2pexchangesession.h"
#include "p2ppacketsauthenticationrequest.h"
#include "p2ppacketsauthenticationresponse.h"
#include "p2ppacketsbusy.h"
#include "p2ppacketserror.h"
#include "p2ppacketshello.h"
#include "p2ppacketsobjectsrequest.h"
#include "p2ppacketsobjectsresponse.h"
#include "p2ppacketsportalrequest.h"
#include "p2ppacketsportalresponse.h"
#include "p2ppacketsqueryrequest.h"
#include "p2ppacketsqueryresponse.h"
#include "p2ppacketsterminate.h"
#include "p2prequest.h"
#include "p2pserver.h"
#include "p2plocalmachine.h"
#include "p2pmachine.h"
#include "p2pp2psystem.h"
#include "portalspeer.h"
#include "portalspeersmanager.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "portalstransaction.h"
#include "tcpsocket.h"

//////////////////////////////////////////////////////////////////////

#define OS_P2P_CONNECTION_STATUS_GUARD()		shared_ptr<ConnectionStatusGuard> __status_guard = lockStatus()

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

using namespace p2p;
using namespace p2p::packets;

//////////////////////////////////////////////////////////////////////

class P2PConnection::P2PScope : public ConnectionScope
{
	typedef ConnectionScope ScopeBase;

// Construction
public:
	P2PScope(shared_ptr<boost::asio::io_service> service, shared_ptr<P2PConnection> connection);
	virtual ~P2PScope();

// Attributes
public:
	bool getSuccessful() const;
	void setSuccessful(bool successful);

// Operations
public:
	shared_ptr<P2PScope> extendTimeout();

private:
	bool m_successful;
	mutable boost::recursive_mutex m_p2pCS;
};

//////////////////////////////////////////////////////////////////////

P2PConnection::P2PScope::P2PScope(shared_ptr<boost::asio::io_service> service, shared_ptr<P2PConnection> connection) : ScopeBase(service, connection),
																													   m_successful(false)
{

}

P2PConnection::P2PScope::~P2PScope()
{
	shared_ptr<P2PConnection> connection = boost::dynamic_pointer_cast<P2PConnection>(getListener());
	if(connection == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	connection->finalizeConnection(getSuccessful());
}

bool P2PConnection::P2PScope::getSuccessful() const
{
	OS_LOCK(m_p2pCS);
	return m_successful;
}

void P2PConnection::P2PScope::setSuccessful(bool successful)
{
	OS_LOCK(m_p2pCS);
	m_successful = successful;
}

shared_ptr<P2PConnection::P2PScope> P2PConnection::P2PScope::extendTimeout()
{
	return boost::dynamic_pointer_cast<P2PScope>(ScopeBase::extendTimeout());
}

//////////////////////////////////////////////////////////////////////

P2PConnection::P2PConnection(shared_ptr<P2PServer> server, shared_ptr<TCPSocket> socket, uint32 timeout, bool outgoing, const String &origin) : ConnectionBase(server->getConnectionsManager()),
																																			    m_server(server),
																																			    m_socket(socket),
																																			    m_status(OS_NEW p2p::NodeStatus(outgoing)),
																																			    m_timeout(timeout),
																																				m_origin(origin),
																																			    m_run(true),
																																			    m_loopback(false),
																																			    m_localSession(OS_NEW LocalSession()),
																																			    m_remoteSession(OS_NEW RemoteSession())
{
	OS_ASSERT(m_socket != nullptr);
}

P2PConnection::~P2PConnection()
{
	_clear();
}

String P2PConnection::getPeerHostname() const
{
	shared_ptr<IPAddress> address = m_remoteSession != nullptr ? m_remoteSession->getAddress() : nullptr;
	if(address != nullptr)
		return address->toString();

	return String::EMPTY;
}

shared_ptr<Portal> P2PConnection::getPortal() const
{
	return m_status->getPortal();
}

shared_ptr<PortalOptions> P2PConnection::getPortalOptions() const
{
	shared_ptr<Portal> portal = getPortal();
	if(portal != nullptr)
		return portal->getOptions();

	return nullptr;
}

String P2PConnection::getPortalName() const
{
	shared_ptr<Portal> portal = getPortal();
	if(portal != nullptr)
		return portal->getPovName();

	return String::EMPTY;
}

std::string P2PConnection::getPortalPovID() const
{
	shared_ptr<Portal> portal = getPortal();
	if(portal != nullptr)
		return portal->getFullPovID();

	return "";
}

bool P2PConnection::setPortal(shared_ptr<Portal> portal)
{
	OS_P2P_CONNECTION_STATUS_GUARD();

	if(portal == nullptr)
		return false;

	m_status->setPortal(portal);

	// Salva il contest p2p
	m_exchangeContext = portal->getExchangeContext();
	if(m_exchangeContext == nullptr)
	{
        OS_ASSERTFALSE();
        return false;
	}

	// Crea una nuova sessione
	m_exchangeSession = m_exchangeContext->createSession();
	if(m_exchangeSession == nullptr)
	{
        OS_ASSERTFALSE();
        return false;
	}

	
	/*
	CLODOURGENT

	se il pov non Ë ancora presente

	if(povnoncello
	m_exchangeSession->addObjectRequest(portal->getPOV());
	*/


	return true;
}

bool P2PConnection::getRun() const
{
	OS_LOCK(m_cs);
	return m_run;
}

void P2PConnection::setRun(bool run)
{
	OS_LOCK(m_cs);
	m_run = run;
}

bool P2PConnection::isAllowed()
{
	shared_ptr<P2PServer> server = getServer();
	return server != nullptr && server->getAllowConnection(get_this_ptr());
}

bool P2PConnection::setPortalPov(const std::string &id)
{
	return setPortal(PortalsSystem::instance()->getPortalByFullPov(id)); 	
}

void P2PConnection::changeStatus(p2p::NodeStatus::Status status)
{
	OS_P2P_CONNECTION_STATUS_GUARD();
	m_status->setStatus(status);
}

bool P2PConnection::compareSessionKey(const Buffer &public_key) const
{
	OS_LOCK(m_cs);
	OS_ASSERT(public_key.empty() == false);

	if(m_localSession == nullptr)
		return false;

	return m_localSession->getPublicKey() == public_key;
}

shared_ptr<p2p::NodeStatus> P2PConnection::takeStatusSnapshot() const
{
	return m_status->clone();
}

void P2PConnection::openConnection(shared_ptr<IPAddress> address, shared_ptr<ConnectionScope> scope)
{
	if(address == nullptr || address->isAny())
		return;

	OS_P2P_CONNECTION_STATUS_GUARD();

	setPeer(address);
	changeStatus(p2p::NodeStatus::statusAttemptingConnection);

	try
	{
		String host = address->toString();
		updateLastEvent(String::format(_S("attempting connection to '%S:%d'").c_str(), host.c_str(), address->getPort()), logLevelNotice);

		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host.to_ascii()), address->getPort());
		getSocket().async_connect(endpoint, boost::bind(&P2PConnection::connectionCallback, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::dynamic_pointer_cast<P2PScope>(scope)));
	}
	catch(boost::system::system_error &e)
	{
		OS_LOG_ERROR(e.what());
	}
}

void P2PConnection::acceptConnection(shared_ptr<ConnectionScope> scope)
{
	OS_P2P_CONNECTION_STATUS_GUARD();

	boost::asio::ip::tcp::endpoint remoteEndpoint = getSocket().remote_endpoint();
	boost::asio::ip::address remoteAddress = remoteEndpoint.address();
	String host = remoteAddress.to_string();

	shared_ptr<IPAddress> peer(OS_NEW IPAddress());
	if(peer->setEndpoint(host, remoteEndpoint.port()) == false)
	{
		OS_ASSERTFALSE();
		return;
	}

	setPeer(peer);

	if(m_remoteSession->m_address->fromString(host) == false)
	{
		OS_ASSERTFALSE();
		return;
	}

	m_remoteSession->setValidable(false);

	OS_ASSERT(m_remoteSession != nullptr);
	updateLastEvent(_S("incoming connection from '") + m_remoteSession->m_address->toString() + _S("'"), logLevelInfo);

	changeStatus(NodeStatus::statusHandshaking);

	shared_ptr<Buffer> request(OS_NEW Buffer());
	//request->reserve(OS_P2P_KEY_AGREEMENT_HEADER_SIZE);
	//boost::asio::async_read(getSocket(), boost::asio::buffer(request->getData(), request->getSize()), boost::asio::transfer_all(), boost::bind(&P2PConnection::handleHandshakeRequest, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, request, boost::dynamic_pointer_cast<P2PScope>(scope)));
	getSocket().async_read_some(boost::asio::buffer(m_handshakeBuffer), boost::bind(&P2PConnection::readHandshakeRequest, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, request, boost::dynamic_pointer_cast<P2PScope>(scope)));
}

void P2PConnection::finalizeConnection(bool successful)
{
	changeStatus(NodeStatus::statusTerminating);

	// Verifica che sia stata stabilita una connessione e che non sia di loopback
	if(m_remoteSession->getAddress()->isAny() == false && getLoopback() == false)
	{
		if(successful)
		{
			shared_ptr<Portal> portal = getPortal();
			if(portal != nullptr)
			{
				shared_ptr<PortalOptions> portalOptions = portal->getOptions();
				if(portalOptions != nullptr)
					// Aggiorna la data di ultimo allineamento (senza considerare eventuali stati di busy)
					portalOptions->updateLastExchangeDate();
			}

			// Controlla se la connessione Ë terminata perchË il nodo remoto era sovraccarico
			if(m_remoteSession->getBusy())
			{
				updateLastEvent(_S("'") + getPeerHostname() + _S("' is busy"), logLevelNotice);
			}
			else if(m_localSession->getBusy())
			{
				updateLastEvent(_S("localhost is busy, data exchange with '") + getPeerHostname() + _S("' aborted"), logLevelNotice);
			}
			else
			{
				updateLastEvent(_S("data exchange with '") + getPeerHostname() + _S("' succeeded"), logLevelNotice);

				if(portal != nullptr)
					// Aggiorna la data di ultimo allineamento terminato con successo del portale (non in stato di busy)
					portal->updateLastSuccessfulExchange();
			}

			// N.B.: la connessione remota deve essere validata solo nel caso in cui sia stata effettuata una connessione diretta
			// e non nel caso in cui sia stata ricevuta perchË il nodo remoto potrebbe essere sotto NAT e quindi, per quanto attivo,
			// non "valido". In ogni caso bisogna assicurare che l'indirizzo IP venga salvato perchË nel caso sia stata ricevuta una connessione
			// l'indirizzo IP potrebbe non essere presente nel DB
			bool connectedSuccessfully = m_remoteSession->getValidable();
			savePeer(m_remoteSession->getAddress(), connectedSuccessfully, connectedSuccessfully ? getOrigin() : OS_PEER_ORIGIN_INCOMING);
		}
		else
		{
			// Controlla se si Ë verificato un errore
			if(m_remoteSession->getError())
			{
				updateLastEvent(_S("removing ip '") + getPeerHostname() + _S("'"), logLevelNotice);
				removePeer(m_remoteSession->getAddress());
			}
			else
			{
				updateLastEvent(_S("data exchange with '") + getPeerHostname() + _S("' failed"), logLevelNotice);
				invalidatePeer(m_remoteSession->getAddress());
			}
		}

		//OS_TRACE(String::format(_S("downloaded = %S, uploaded = %S\n").c_str(), utils::formatSize(m_status->getDownloadedBytes()).c_str(), PlatformManager::instance()->formatSize(m_status->getUploadedBytes()).c_str()));
	}

	_clear();
}

bool P2PConnection::handlePacket(packet_ptr &local, packet_ptr &remote, shared_ptr<P2PScope> scope)
{
	if(remote == nullptr)
		return false;

	switch(remote->getType())
	{
	case packets::ptError:						if(onError(boost::dynamic_pointer_cast<Error>(remote), local) == false)
													return false;

												break;

	case packets::ptBusy:						if(onBusy(boost::dynamic_pointer_cast<Busy>(remote), local, scope) == false)
													return false;

												break;

	case packets::ptHello:						if(onHello(boost::dynamic_pointer_cast<Hello>(remote), local, scope) == false)
													return false;

												break;

	case packets::ptTerminate:					if(onTerminate(boost::dynamic_pointer_cast<Terminate>(remote), local, scope) == false)
													return false;

												break;

	case packets::ptAuthenticationRequest:		if(onAuthenticationRequest(boost::dynamic_pointer_cast<AuthenticationRequest>(remote), local) == false)
													return false;

												break;

	case packets::ptAuthenticationResponse:		if(onAuthenticationResponse(boost::dynamic_pointer_cast<AuthenticationResponse>(remote), local) == false)
													return false;

												break;

	case packets::ptPortalRequest:				if(onPortalRequest(boost::dynamic_pointer_cast<PortalRequest>(remote), local) == false)
													return false;

												break;

	case packets::ptPortalResponse:				if(onPortalResponse(boost::dynamic_pointer_cast<PortalResponse>(remote), local) == false)
													return false;

												break;

	case packets::ptQueryRequest:				if(onQueryRequest(boost::dynamic_pointer_cast<QueryRequest>(remote), local) == false)
													return false;

												break;

	case packets::ptQueryResponse:				if(onQueryResponse(boost::dynamic_pointer_cast<QueryResponse>(remote), local) == false)
													return false;

												break;

	case packets::ptObjectsRequest:				if(onObjectsRequest(boost::dynamic_pointer_cast<ObjectsRequest>(remote), local) == false)
													return false;

												break;

	case packets::ptObjectsResponse:			if(onObjectsResponse(boost::dynamic_pointer_cast<ObjectsResponse>(remote), local) == false)
													return false;

												break;

	default:									OS_ASSERTFALSE();
												return false;
	}

	return true;
}

bool P2PConnection::validatePacket(packet_ptr &local, packet_ptr &remote)
{
	if(remote == nullptr)
		return false;

	if(validateType(remote->getType()) == false)
		return false;

	return local != nullptr ? local->validate(remote) : true;
}

bool P2PConnection::validateType(packets::PacketType type)
{
	switch(type)
	{
	case packets::ptError:
	case packets::ptBusy:
	case packets::ptHello:
	case packets::ptTerminate:
	case packets::ptAuthenticationRequest:
	case packets::ptAuthenticationResponse:
	case packets::ptPortalRequest:
	case packets::ptPortalResponse:
	case packets::ptQueryRequest:
	case packets::ptQueryResponse:
	case packets::ptObjectsRequest:
	case packets::ptObjectsResponse:
												return true;

	default:									OS_ASSERTFALSE();
												break;
	}

	return false;
}

packet_ptr P2PConnection::allocPacket(packets::PacketType type)
{
	switch(type)
	{
	case packets::ptError:						return packet_ptr(OS_NEW Error());

	case packets::ptBusy:						return packet_ptr(OS_NEW Busy());

	case packets::ptHello:						return packet_ptr(OS_NEW Hello());

	case packets::ptTerminate:					return packet_ptr(OS_NEW Terminate());

	case packets::ptAuthenticationRequest:		return packet_ptr(OS_NEW AuthenticationRequest());

	case packets::ptAuthenticationResponse:		return packet_ptr(OS_NEW AuthenticationResponse());

	case packets::ptPortalRequest:				return packet_ptr(OS_NEW PortalRequest());

	case packets::ptPortalResponse:				return packet_ptr(OS_NEW PortalResponse());

	case packets::ptQueryRequest:				return packet_ptr(OS_NEW QueryRequest());

	case packets::ptQueryResponse:				return packet_ptr(OS_NEW QueryResponse());

	case packets::ptObjectsRequest:				return packet_ptr(OS_NEW ObjectsRequest());

	case packets::ptObjectsResponse:			return packet_ptr(OS_NEW ObjectsResponse());

	default:									OS_ASSERTFALSE();
	}

	return nullptr;
}

packets::packet_ptr P2PConnection::createHelloPacket()
{
	shared_ptr<P2PServer> server = getServer();
	if(server == nullptr)							// PuÚ accadere in fase di chiusura
		return nullptr;

	OS_ASSERT(m_localSession != nullptr);
	Buffer signature;
	// Firma la chiave di sessione
	if(m_localSession->signKey(p2p::LocalMachine::instance()->getPrivateKey(), signature) == false)
		return nullptr;

	packets::hello_ptr hello(OS_NEW packets::Hello());
	hello->setMachineID(p2p::LocalMachine::instance()->getID());
	hello->setSignature(signature);

	uint32 serverPort = 0;
	if(server->getBindPort(serverPort))			// Il server potrebbe non essere in listen (ad esempio nessuna connessione in entrata) oppure potrebbe essere in fase di chiusura
		hello->setServerPort(serverPort);

	return hello;
}

void P2PConnection::generateRandomSeed(byte &size, Buffer &data)
{
	size = 0;
	CryptManager::instance()->randomBlock(&size, sizeof(uint8));

	data.clear();

	if(size > 0)
	{
		if(data.reserve(size))
		{
			CryptManager::instance()->randomBlock(data.getData(), data.getSize());					
		}
		else
		{
			OS_ASSERTFALSE();
			size = 0;
			data.clear();
		}		
	}	

	OS_ASSERT(static_cast<uint8>(data.getSize()) == size);
}

void P2PConnection::_clear()
{
	if(m_exchangeContext != nullptr)
	{
		if(m_exchangeSession != nullptr)
		{
		    shared_ptr<Portal> portal = getPortal();
		    if(portal != nullptr)
                // Finalizza la sessione corrente
                m_exchangeSession->finalize(portal);

			// Resetta la sessione
			m_exchangeContext->removeSession(m_exchangeSession);
			m_exchangeSession.reset();
		}

		m_exchangeContext.reset();
	}

	m_localPacket.reset();
	m_remotePacket.reset();
}

void P2PConnection::_applyPortalKeyDerivation(LocalSession::DerivationType type)
{
	OS_ASSERT(m_localSession != nullptr);

	shared_ptr<Portal> portal = getPortal();
	if(portal == nullptr)
		return;

	// Controlla se il portale Ë protetto da password
	if(portal->getOptions()->getPassword().empty() == false)
	{
		// Utilizza la password del portale come derivazione della chiave di sessione
		m_localSession->addKeyDerivation(type, portal->getOptions()->getPassword());
	}
}

bool P2PConnection::onHello(packets::hello_ptr hello, packet_ptr &response, shared_ptr<P2PScope> scope)
{
	if(isAllowed())
	{
		m_remoteSession->m_id = hello->getMachineID();
		m_remoteSession->m_signature = hello->getSignature();
		m_remoteSession->m_address->setPort(hello->getServerPort());

		shared_ptr<P2pMachine> machine = P2PSystem::instance()->getMachine(m_remoteSession->m_id);
		if(machine == nullptr)
		{
			m_remoteSession->m_known = false;
		}
		else
		{
			m_remoteSession->m_known = true;

			if(m_remoteSession->verifySignature(machine->getPublicKey(), m_localSession->getKey()) == false)
				return false;
		}

		if(m_remoteSession->m_known)
		{
			if(m_localSession->getHelloSended())
				return onAuthenticationCompleted(response);
			else
				return createHelloResponse(response);
		}

		response.reset(OS_NEW AuthenticationRequest());
	}
	else
	{
		// URGENT: da testare

		m_localSession->setBusy(true);

		scope->setSuccessful(true);
		setRun(false);

		response.reset(OS_NEW packets::Busy());

		// return sendBusyPacket(scope);
	}

	return true;
}

bool P2PConnection::onAuthenticationRequest(packets::authentication_request_ptr authentication_request, packets::packet_ptr &response)
{
	authentication_response_ptr authentication_response(OS_NEW AuthenticationResponse());
	authentication_response->setPublicKey(p2p::LocalMachine::instance()->getPublicKey());

	response = authentication_response;

	return true;
}

bool P2PConnection::onAuthenticationResponse(packets::authentication_response_ptr authentication_response, packets::packet_ptr &response)
{
	Buffer public_key = authentication_response->getPublicKey();
	if(P2PSystem::instance()->validateMachineID(m_remoteSession->m_id, public_key) == false)
		return false;

	if(m_remoteSession->verifySignature(public_key, m_localSession->getKey()) == false)
		return false;

	P2PSystem::instance()->addMachine(m_remoteSession->m_id, public_key);

	if(m_localSession->getHelloSended() == false)
		return createHelloResponse(response);

	return onAuthenticationCompleted(response);
}

bool P2PConnection::onPortalRequest(packets::portal_request_ptr portal_request, packets::packet_ptr &response)
{
	if(setPortalPov(portal_request->getPortalPov()))
	{
		response.reset(OS_NEW PortalResponse());
		// Applica un'eventuale derivazione della chiave sul portale dal prossimo pacchetto ricevuto (la riposta di accettazione del portale non deve subire alterazione perch√® potrebbe essere un rifiuto)
		_applyPortalKeyDerivation(LocalSession::dtNextPacketReceived);
	}
	else
	{
		// URGENT: da testare

		response.reset(OS_NEW packets::Error(packets::peInvalidPortal));
		setRun(false);
	}

	return true;
}

bool P2PConnection::onPortalResponse(packets::portal_response_ptr portal_response, packets::packet_ptr &response)
{
	// Applica un'eventuale derivazione della chiave sul portale dal prossimo pacchetto inviato (la risposta deve subire alterazione)
	_applyPortalKeyDerivation(LocalSession::dtNextPacketSended);
	// Carica il pacchetto di risposta
	peekPacket(response);

	return true;
}

bool P2PConnection::onTerminate(packets::terminate_ptr terminate, packet_ptr &response, shared_ptr<P2PScope> scope)
{
	// Constrolla se il pacchetto ricevuto √® la conferma di terminazione
	if(m_localPacket == nullptr || m_localPacket->getType() == packets::ptTerminate)
	{
		response.reset();

		scope->setSuccessful(true);
		setRun(false);
	}
	else // Il client remoto sta proponendo la terminazione
	{
		// Controlla se ci sono altre richieste
		peekPacket(response);
		if(response->getType() == packets::ptTerminate)
		{
			scope->setSuccessful(true);
			setRun(false);	// Conferma la proposta
		}
	}

	return true;
}

bool P2PConnection::onQueryRequest(packets::query_request_ptr request, packet_ptr &response)
{
    shared_ptr<Portal> portal = getPortal();
    if(portal == nullptr)
        return false;

	// Carica le statistiche del client remoto
	if(loadRequest(request) == false)
		return false;

	// URGENT: Le "return false" portano ad un rollback del db, Ë giusto?
	shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(false);
	shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();

	query_response_ptr query_response(OS_NEW QueryResponse());

	uint32 idToSend = OS_P2P_OBJECTS_EXCHANGE_COUNT;

	DataEntry::DataEntryList entries;

	uint32 start = 0;
	uint32 count = idToSend;
	bool tryByRank = true;

	for(;;)
	{
		DataEntry::DataEntryList tmp;
		uint32 extracted = 0;

		if(tryByRank)
		{
            // Effettua l'estrazione
            if(m_exchangeContext->extractEntriesByRank(database, start, count, tmp) == false)
                return false;

			// Salva il numero di entries estratte
			extracted = static_cast<uint32>(tmp.size());
			// Sposta l'offset iniziale
			start += extracted;

			if(tmp.empty() == false)
			{
				databaseWork->setExclusiveAccess(true);
				database->beginTransaction();
				// Aggiorna il rank degli entries
				for(DataEntry::DataEntryList::const_iterator i = tmp.begin(); i != tmp.end(); ++i)
				{
					const ObjectID &id = (*i)->id;

					// Aggiorna il rank dell'entry
					database->decreaseEntryRank(id);
				}
				database->commit();
			}
		}

        // Controlla se non Ë stato possibile estrarre alcun entry
		if(extracted == 0)
		{
		    tryByRank = false;

	         // Effettua l'estrazione
            if(m_exchangeContext->extractEntriesByID(database, count, tmp) == false)
                return false;

            // Salva il numero di entries estratte
            extracted = static_cast<uint32>(tmp.size());
		}

		// Controlla se non Ë stato possibile estrarre alcun entry
		if(extracted == 0)
			break;

		// Scandisce tutti le entries estratte
		for(DataEntry::DataEntryList::const_iterator i = tmp.begin(); i != tmp.end(); ++i)
		{
			const ObjectID &id = (*i)->id;

			// Nota: l'inizializzazione dello stato va comunque fatta indipendentemente dal fatto
			// che l'entry sia conosciuta dal nodo remoto (in modo tale da proporne sempre
			// diversi nel caso in cui si verifichi una successiva estrazione tra i due nodi)
            m_exchangeSession->setIDRefused(id);

			// Controlla se prendere in considerazione l'id
			if(m_exchangeSession->isKnownID(id) == false)
				entries.push_back(*i);
		}

		// Ottiene il numero di id disponibili
		uint32 availables = static_cast<uint32>(entries.size());
		// Controlla se si Ë raggiunto il numero richiesto
		if(idToSend <= availables)
			break;

		// Aggiorna il numero di id restanti
		count = idToSend - availables;
	}

	updateLastEvent(String::format(_S("Incoming query request from '%S', proposing %d objects for portal '%S'").c_str(), getPeerHostname().c_str(), entries.size(), getPortalName().c_str()), logLevelNotice);

	// Salva gli id nel pacchetto di risposta
	query_response->setEntries(entries);

	response = query_response;

	return true;
}

bool P2PConnection::onQueryResponse(packets::query_response_ptr response, packet_ptr &request)
{
    shared_ptr<Portal> portal = getPortal();
    if(portal == nullptr)
        return false;

	shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(false);
	shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();

	DataEntry::DataEntryList entries;
	// Ottiene l'elenco delle entries proposte
	response->getEntries(entries);
	// Scandisce l'elenco delle entries proposte
	for(DataEntry::DataEntryList::iterator i = entries.begin(); i != entries.end(); ++i)
	{
		shared_ptr<DataEntry> entry = *i;

		bool known = true;
		// Richiede l'oggetto se necessario
		if(m_exchangeContext->requestObject(database, m_exchangeSession, entry->id, entry->submit_date) == false)
		{
			// Nota se l'oggetto non √® necessario √® possibile che il nodo abbia una versione pi√π
			// aggiornata dell'oggetto e marcandolo come conosciuto eviterebbe di proporla in seguito al
			// nodo remoto, pertando marca l'id come conosciuto solo nel caso in cui la data coincida

			shared_ptr<DataEntry> ptr = m_exchangeContext->getEntry(database, entry->id);
			if(ptr != nullptr)
				known = ptr->submit_date == entry->submit_date;
		}

		// Verifica che l'id sia conosciuto dal nodo remoto
		if(known)
		{
			// Marca l'id come conosciuto dal nodo remoto
			m_exchangeSession->setIDKnown(entry->id);
		}
		else
		{
			// TODO: questa √® la circostanza in cui il nodo remoto ha una versione di un oggetto pi√π
			// vecchia rispetto alla versione locale, pertanto bisognerebbe proporre tale versione al nodo remoto
			updateLastEvent(String::format(_S("Localhost has a most recent version of '%S' than '%S'").c_str(), entry->id->toUTF16().c_str(), getPeerHostname().c_str()), logLevelNotice);
		}
	}

	// Ottiene la richiesta corrente
	peekPacket(request);

	return true;
}

bool P2PConnection::onObjectsRequest(packets::objects_request_ptr request, packet_ptr &response)
{
    shared_ptr<Portal> portal = getPortal();
    if(portal == nullptr)
        return false;

	OS_P2P_CONNECTION_STATUS_GUARD();

	bool somethingUploaded = false;

	shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(false);
	shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();

	objects_response_ptr objects_response(OS_NEW ObjectsResponse());

	StringList objects;
	// Ottiene gli oggetti richiesti
	request->getObjects(objects);
	// Carica l'elenco degli oggetti da inviare
	for(StringList::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		ObjectID id((*i).to_ascii());
		//String objectTypeName = ObjectsSystem::instance()->getTypeName(id.getObjectType());
		updateLastEvent(String::format(_S("'%S' request an object for portal '%S'").c_str(), getPeerHostname().c_str(), getPortalName().c_str()), logLevelInfo);

		// Carica l'oggetto corrente
		objects_response->insert(database, id.toUTF16());
		// Aggiorna lo stato dell'oggetto
		m_exchangeSession->setIDRequested(id);
		// Aggiorna il numero di oggetti richiesti
		m_status->addUploadedObject();

		somethingUploaded = true;
	}

	response = objects_response;

	if(somethingUploaded)
	{
		shared_ptr<PortalOptions> portalOptions = getPortalOptions();
		if(portalOptions != nullptr)
			portalOptions->updateLastUploadedObjectDate();
	}

	return true;
}

bool P2PConnection::onObjectsResponse(packets::objects_response_ptr response, packet_ptr &request)
{
    shared_ptr<Portal> portal = getPortal();
    if(portal == nullptr)
        return false;

	OS_P2P_CONNECTION_STATUS_GUARD();

	bool somethingDownloaded = false;

	StringList ids;
	// Carica gli id degli oggetti ricevuti
	response->getObjects(ids);

	if(ids.empty() == false)
	{
		shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(true);
		databaseWork->getDatabase()->beginTransaction();

		// Scandisce tutti gli oggetti ricevuti
		for(StringList::iterator i = ids.begin(); i != ids.end(); ++i)
		{
			ObjectID id((*i).to_ascii());
			//String objectTypeName = ObjectsSystem::instance()->getTypeName(id.getObjectType());
			updateLastEvent(String::format(_S("Incoming object from '%S' for portal '%S'").c_str(), getPeerHostname().c_str(), getPortalName().c_str()), logLevelInfo);

			shared_ptr<ObjectsIObject> object = response->getObject(id);
			// Carica l'oggetto corrente
			if(object != nullptr)
			{
				// Processa l'oggetto corrente
				processObject(databaseWork->getDatabase(), object);
				// Aggiorna il numero di oggetti scaricati
				m_status->addDownloadedObject();

				somethingDownloaded = true;
			}
		}

		databaseWork->getDatabase()->commit();
	}

	peekPacket(request);

	if(somethingDownloaded)
	{
		shared_ptr<PortalOptions> portalOptions = getPortalOptions();
		if(portalOptions != nullptr)
			portalOptions->updateLastDownloadedObjectDate();
	}

	return true;
}

bool P2PConnection::onBusy(packets::busy_ptr response, packets::packet_ptr &request, shared_ptr<P2PScope> scope)
{
	m_remoteSession->setBusy(true);

	request.reset();

	scope->setSuccessful(true);
	setRun(false);

	return true;
}

bool P2PConnection::onError(packets::error_ptr response, packets::packet_ptr &request)
{
	switch(response->getError())
	{
	case peUnknown:				updateLastEvent(String::format(_S("Unexpected error packet from peer '%S'").c_str(), getPeerHostname().c_str()), logLevelNotice);
								break;

	case peInvalidPortal:		updateLastEvent(String::format(_S("Remote peer '%S' is not subscribed to portal '%S'").c_str(), getPeerHostname().c_str(), getPortalName().c_str()), logLevelNotice);
								break;

	default:					OS_ASSERTFALSE();
								break;
	}

	m_remoteSession->setError(true);

	request.reset();
	setRun(false);

	return false;
}

bool P2PConnection::onAuthenticationCompleted(packets::packet_ptr &request)
{
	std::string portalPov = getPortalPovID();
	if(portalPov.empty())
		return false;	// Il portale potrebbe essere stato rimosso

	portal_request_ptr portal_request(OS_NEW PortalRequest());
	portal_request->setPortalPov(portalPov);

	request = portal_request;

	return true;
}

void P2PConnection::processObject(shared_ptr<IPortalDatabase> database, shared_ptr<ObjectsIObject> object)
{
	bool save = true;

	// Nota: la setIDKnown viene richiamata in fase di proposta degli id,
	// bisogna per√≤ contemplare anche il caso in cui gli id non vengano proposti,
	// ma siano direttamente richiesti (nel caso delle dipendenze)
	m_exchangeSession->setIDKnown(object->id);

	// Verifica che l'oggetto non rappresenti un utente (visto che l'id dell'autore
	// coincide con quello dell'utente). In alternativa si potrebbe verificare che object->id != author
	if(object->isUser() == false)
	{
		// N.B.: l'autore non deve essere richiesto a livello di contesto altrimenti si corre il rischio
		// di non poter validare gli oggetti ricevuti nella sessione perch√® l'autore √® attualmente in cache/attesa
		// da altre sessioni, pertanto viene indipendentemente richiesto e sar√† poi la prima sessione a riceverlo
		// che effettuer√† l'inserimento se necessario

		if(m_exchangeSession->requireObject(database, object->getAuthor()))
		{
			// Nota: se l'autore dell'oggetto non √® disponibile, bisogna salvare quest'ultimo
			// nella cache visto che non sarebbe possibile validarlo al momento.
			save = false;
		}
	}

	// Ottiene l'elenco delle dipendenze dell'oggetto
	list<ObjectID>::type dependencies;
	object->getDependencies(database, dependencies);
	// Scandisce l'elenco delle dipendenze
	for(list<ObjectID>::type::iterator i = dependencies.begin(); i != dependencies.end(); ++i)
	{
		// Richiede l'oggetto se necessario
		m_exchangeContext->requestObject(database, m_exchangeSession, *i);
	}

	// Verifica che l'oggetto sia effettivamente necessario
	if(m_exchangeSession->needObject(database, object->id, object->submit_date))
	{
		// Controlla se bisogna serializzare l'oggetto
		if(save)
			// Salva l'oggetto nel database
			m_exchangeSession->storeObject(database, object);
		else
			// Salva l'oggetto nella cache
			m_exchangeSession->cacheObject(object);

		// N.B.: se dal nodo remoto √® stato ricevuto almeno un oggetto, tenta di effettuare un ulteriore allineamento: ci√≤
		// fa si che lo scambio di dati continui finch√® da entrambe le parti non venga inserito neanche un oggetto
		m_exchangeSession->addRequest(rtQuery);
	}
}

bool P2PConnection::createHelloResponse(packets::packet_ptr &packet)
{
	// Crea il pacchetto di hello
	packet = createHelloPacket();
	if(packet == nullptr)
		return false;

	m_localSession->setHelloSended(true);
	return true;
}

void P2PConnection::savePeer(shared_ptr<IPAddress> address, bool connectedSuccessfully, const String &origin)
{
	shared_ptr<Portal> portal = getPortal();
	if(portal != nullptr)
	{
		// Se la connessione Ë andata a buon fine valida anche il nodo
		portal->getPeersManager()->savePeer(address, connectedSuccessfully, origin);
		// Se Ë stata effettuata una connessione verso il nodo assicura che venga salvato come supernodo
		if(connectedSuccessfully)
			portal->getPeersManager()->addSupernode(address);
	}
}

void P2PConnection::validatePeer(shared_ptr<IPAddress> address)
{
	shared_ptr<Portal> portal = getPortal();
	if(portal != nullptr)
		portal->getPeersManager()->validatePeer(address);
}

void P2PConnection::invalidatePeer(shared_ptr<IPAddress> address)
{
	shared_ptr<Portal> portal = getPortal();
	if(portal != nullptr)
		portal->getPeersManager()->invalidatePeer(address);
}

void P2PConnection::removePeer(shared_ptr<IPAddress> address)
{
	shared_ptr<Portal> portal = getPortal();
	if(portal != nullptr)
		portal->getPeersManager()->removePeer(address);
}

void P2PConnection::updateLastEvent(const String &description, LogLevel level)
{
	LogManager::instance()->log(description, level, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION);

	OS_P2P_CONNECTION_STATUS_GUARD();
	m_status->setLastEvent(description);
}

void P2PConnection::peekPacket(packet_ptr &packet)
{
	request_ptr request;
	// Ottiene la richiesta corrente
	m_exchangeSession->peekRequest(request);
	// Controlla se non ci sono altre richieste
	if(request == nullptr)
	{
		// Crea il pacchetto di proposta chiusura
		packet.reset(OS_NEW Terminate());
	}
	else
	{
		// Seleziona il tipo di richiesta
		switch(request->getType())
		{
		case rtQuery:		{
								// Crea il pacchetto di query
								query_request_ptr query_request(OS_NEW QueryRequest());
								// Salva le informazioni sul nodo
								saveRequest(query_request);

								packet = query_request;
							}

							break;

		case rtObjects:		{
								StringList objects;
								// Ottiene l'elenco degli oggetti richiesti
								request->getObjects(objects);
								// Crea il pacchetto per la richiesta degli oggetti
								packet.reset(OS_NEW ObjectsRequest(&objects));
							}

							break;

		default:			OS_ASSERTFALSE();
							break;
		}
	}
}

bool P2PConnection::loadRequest(packets::query_request_ptr request)
{
	list<shared_ptr<IPAddress> >::type addresses;
	request->getPeers(addresses);

	for(list<shared_ptr<IPAddress> >::type::iterator i = addresses.begin(); i != addresses.end(); ++i)
	{
		shared_ptr<IPAddress> address = *i;
		savePeer(address, false, OS_PEER_ORIGIN_PEX);
	}

	return true;
}

void P2PConnection::saveRequest(packets::query_request_ptr request)
{
	shared_ptr<Portal> portal = getPortal();
	if(portal == nullptr)
		return;

	list<shared_ptr<PortalsPeer> >::type supernodes;
	// Estra i supernodi conosciuti
	portal->getPeersManager()->extractSupernodes(OS_DEFAULT_PEERSREQUEST, supernodes);
	// Salva i supernodi nel pacchetto di richiesta
	for(list<shared_ptr<PortalsPeer> >::type::const_iterator i = supernodes.begin(); i != supernodes.end(); ++i)
	{
		// Propone come ip i supernodi conosciuti (saltando il nodo a cui si Ë connessi se Ë tra questi)

		shared_ptr<PortalsPeer> supernode = *i;
		OS_ASSERT(supernode != nullptr);

		// N.B.: il confronto deve essere fatto solo sull'indirizzo IP perchË potrebbe essere una connessione in entrata
		if(supernode->getIP() != getPeerHostname())
			request->addPeer(supernode->getIP(), supernode->getPort());
	}
}

shared_ptr<IPAddress> P2PConnection::getPeer() const
{
	return m_status->getPeer();
}

void P2PConnection::setPeer(shared_ptr<IPAddress> peer)
{
	OS_P2P_CONNECTION_STATUS_GUARD();
	m_status->setPeer(peer);
}

shared_ptr<ConnectionStatusGuard> P2PConnection::lockStatus()
{
	OS_LOCK(m_statusCS);

	shared_ptr<ConnectionStatusGuard> statusGuard = m_statusGuard.lock();
	if(statusGuard == nullptr)
	{
		statusGuard.reset(OS_NEW ConnectionStatusGuard(get_this_ptr(), m_status));
		m_statusGuard = statusGuard;
	}

	return statusGuard;
}

void P2PConnection::connectionCallback(const boost::system::error_code &e, shared_ptr<P2PScope> scope)
{
	scope->cancelTimeout();

	OS_P2P_CONNECTION_STATUS_GUARD();

	shared_ptr<IPAddress> address = getPeer();
	OS_ASSERT(address != nullptr);

	if(e)
	{
		updateLastEvent(String::format(_S("connection to '%S:%d' failed").c_str(), address->toString().c_str(), address->getPort()), logLevelNotice);
		invalidatePeer(address);
	}
	else
	{
		updateLastEvent(String::format(_S("connection to '%S:%d' succeeded").c_str(), address->toString().c_str(), address->getPort()), logLevelNotice);

		m_remoteSession->m_address->setIP(address->getIP());
		m_remoteSession->m_address->setPort(address->getPort());
		m_remoteSession->setValidable(true);

		changeStatus(NodeStatus::statusHandshaking);

		shared_ptr<CryptKeyAgreement> keyAgreement = m_localSession->initDH();
		if(keyAgreement != nullptr)
		{
			const Buffer &modulus = keyAgreement->getModulus();
			OS_ASSERT(modulus.getSize() == OS_P2P_KEY_AGREEMENT_MODULUS_SIZE);

			const Buffer &generator = keyAgreement->getGenerator();
			OS_ASSERT(generator.getSize() == OS_P2P_KEY_AGREEMENT_GENERATOR_SIZE);

			const Buffer &public_key = keyAgreement->getPublicKey();
			OS_ASSERT(public_key.getSize() == OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE);

			Buffer dh_buffer;
			dh_buffer.write(modulus.getData(), modulus.getSize());
			dh_buffer.write(generator.getData(), generator.getSize());
			dh_buffer.write(public_key.getData(), public_key.getSize());
			dh_buffer.seekToBegin();

			uint8 seedSize = 0;
			Buffer seedData;
			generateRandomSeed(seedSize, seedData);

			dh_buffer.xorValue(seedData.getData(), seedData.getSize());

			// Handshake request packet structure: 1 byte random seed lenght, nbytes <random_seed>, 33 bytes (modulus,generator,pub_key) DH xor random_seed

			shared_ptr<Buffer> buffer(OS_NEW Buffer());
			buffer->write(&seedSize, sizeof(uint8));
			buffer->write(seedData.getData(), seedData.getSize());
			buffer->write(dh_buffer.getData(), dh_buffer.getSize());
			
			OS_ASSERT(buffer->getSize() == (sizeof(uint8) + seedSize + OS_P2P_KEY_AGREEMENT_HEADER_SIZE));
			boost::asio::async_write(getSocket(), boost::asio::buffer(buffer->getData(), buffer->getSize()), boost::asio::transfer_all(), boost::bind(&P2PConnection::handshakeRequestCallback, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, buffer, scope->extendTimeout()));
		}
	}
}

void P2PConnection::handshakeRequestCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> transferredData, shared_ptr<P2PScope> scope)
{
	scope->cancelTimeout();

	OS_P2P_CONNECTION_STATUS_GUARD();

	updateUpload(transferredBytes);

	if(e)
	{
		// Se si verifica un errore di trasferimento invalida il nodo remoto (viene rimosso in caso di errore DH)

		updateLastEvent(_S("P2P handshake error: ") + e.message().c_str(), logLevelNotice);
		invalidatePeer(getPeer());
	}
	else
	{
		OS_ASSERT(transferredData->getSize() == static_cast<uint32>(transferredBytes));

		// Riutilizza il buffer per la ricezione della risposta

		transferredData->clear();
		//transferredData->reserve(OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE);
		//boost::asio::async_read(getSocket(), boost::asio::buffer(transferredData->getData(), transferredData->getSize()), boost::asio::transfer_all(), boost::bind(&P2PConnection::handshakeResponseCallback, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, transferredData, scope->extendTimeout()));
		getSocket().async_read_some(boost::asio::buffer(m_handshakeBuffer), boost::bind(&P2PConnection::readResponseCallback, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, transferredData, scope->extendTimeout()));
	}
}

void P2PConnection::readResponseCallback(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> transferredData, shared_ptr<P2PScope> scope)
{
	scope->cancelTimeout();

	OS_P2P_CONNECTION_STATUS_GUARD();

	updateDownload(transferredBytes);

	if(e)
	{
		// Se si verifica un errore di trasferimento invalida il nodo remoto (viene rimosso in caso di errore DH)

		updateLastEvent(_S("P2P handshake error: ") + e.message().c_str(), logLevelNotice);
		invalidatePeer(getPeer());
	}
	else
	{
		if(transferredData->append(m_handshakeBuffer.data(), static_cast<uint32>(transferredBytes)) != static_cast<uint32>(transferredBytes))
			return;

		transferredData->seekToBegin();

		bool readAgain = false;
		uint8 seedSize = 0;

		if(transferredData->empty())
		{
			readAgain = true;
		}
		else
		{			
			if(transferredData->read(&seedSize, sizeof(uint8)) != sizeof(uint8))
				return;

			if(transferredData->getAvailable() < static_cast<uint32>((seedSize + OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE)))
				readAgain = true;
		}

		if(readAgain)
		{
			getSocket().async_read_some(boost::asio::buffer(m_handshakeBuffer), boost::bind(&P2PConnection::readResponseCallback, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, transferredData, scope->extendTimeout()));
		}
		else
		{
			if(seedSize > 0)
			{
				Buffer seedData;
				if(seedData.reserve(seedSize) == false)
					return;

				if(transferredData->read(seedData.getData(), seedSize) != seedSize)
					return;

				OS_ASSERT(transferredData->getAvailable() == OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE);
				transferredData->xorValue(seedData.getData(), seedData.getSize());
			}

			OS_ASSERT(transferredData->getAvailable() == OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE);

			Buffer remotePublicKey;
			if(remotePublicKey.reserve(OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE) == false)
				return;

			if(transferredData->read(remotePublicKey.getData(), remotePublicKey.getSize()) != remotePublicKey.getSize())
				return;

			handshakeResponseCallback(remotePublicKey, scope);
		}
	}
}

void P2PConnection::handshakeResponseCallback(const Buffer &remotePublicKey, shared_ptr<P2PScope> scope)
{
	OS_ASSERT(remotePublicKey.getSize() == OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE);
		
	if(handleHandshakeResponse(remotePublicKey))
	{
		startExchange(scope, true);
	}
	else
	{
		removePeer(getPeer());
	}	
}

bool P2PConnection::handleHandshakeResponse(const Buffer &remotePublicKey)
{
	OS_P2P_CONNECTION_STATUS_GUARD();

	if(m_localSession->agree(remotePublicKey) == false)
	{
		updateLastEvent(_S("P2P handshake error: dh agree error"), logLevelNotice);
		return false;
	}

	shared_ptr<P2PServer> server = getServer();
	if(server == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	shared_ptr<P2PConnection> remoteConnection = server->detectLoopbackConnection(remotePublicKey);
	if(remoteConnection != nullptr)
	{
		updateLastEvent(_S("Detected loopback connection with '") + getPeerHostname() + _S("'"), logLevelDebug);

		// Gestisce il loopback su questa connessione
		handleLoopback();
		// Gestisce il loopback sulla connessione remota
		remoteConnection->handleLoopback();

		return false;
	}

	return true;
}

void P2PConnection::readHandshakeRequest(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> request, shared_ptr<P2PScope> scope)
{
	scope->cancelTimeout();

	OS_P2P_CONNECTION_STATUS_GUARD();

	updateDownload(transferredBytes);

	if(e)
	{
		// Se si verifica un errore di trasferimento invalida il nodo remoto (viene rimosso in caso di errore DH)

		updateLastEvent(_S("P2P handshake error: ") + e.message().c_str(), logLevelNotice);
	}
	else
	{
		if(request->append(m_handshakeBuffer.data(), static_cast<uint32>(transferredBytes)) != static_cast<uint32>(transferredBytes))
			return;

		request->seekToBegin();

		bool readAgain = false;
		uint8 seedSize = 0;

		if(request->empty())
		{
			readAgain = true;
		}
		else
		{			
			if(request->read(&seedSize, sizeof(uint8)) != sizeof(uint8))
				return;

			if(request->getAvailable() < static_cast<uint32>((seedSize + OS_P2P_KEY_AGREEMENT_HEADER_SIZE)))
				readAgain = true;
		}

		if(readAgain)
		{
			getSocket().async_read_some(boost::asio::buffer(m_handshakeBuffer), boost::bind(&P2PConnection::readHandshakeRequest, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, request, scope->extendTimeout()));
		}
		else
		{
			if(seedSize > 0)
			{
				Buffer seedData;
				if(seedData.reserve(seedSize) == false)
					return;

				if(request->read(seedData.getData(), seedSize) != seedSize)
					return;

				OS_ASSERT(request->getAvailable() == OS_P2P_KEY_AGREEMENT_HEADER_SIZE);
				request->xorValue(seedData.getData(), seedData.getSize());
			}

			handleHandshakeRequest(request, scope);
		}
	}
}

void P2PConnection::handleHandshakeRequest(shared_ptr<Buffer> request, shared_ptr<P2PScope> scope)
{
	OS_ASSERT(request->getAvailable() == OS_P2P_KEY_AGREEMENT_HEADER_SIZE);

	Buffer modulus;
	modulus.reserve(OS_P2P_KEY_AGREEMENT_MODULUS_SIZE);
	if(request->read(modulus.getData(), OS_P2P_KEY_AGREEMENT_MODULUS_SIZE) == false)
		return;

	Buffer generator;
	generator.reserve(OS_P2P_KEY_AGREEMENT_GENERATOR_SIZE);
	if(request->read(generator.getData(), OS_P2P_KEY_AGREEMENT_GENERATOR_SIZE) == false)
		return;

	Buffer public_key;
	public_key.reserve(OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE);
	if(request->read(public_key.getData(), OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE) == false)
		return;

	shared_ptr<CryptKeyAgreement> keyAgreement = m_localSession->initDH(modulus, generator);
	if(keyAgreement == nullptr || m_localSession->agree(public_key) == false)
	{
		updateLastEvent(_S("P2P connection error: dh agree error"), logLevelNotice);
		return;
	}

	const Buffer &publicKey = keyAgreement->getPublicKey();
	OS_ASSERT(publicKey.getSize() == OS_P2P_KEY_AGREEMENT_PUBLIC_KEY_SIZE);

	// N.B.: non serve effettuare una copia del buffer da inviare in quanto "keyAgreement" Ë membro di m_localSession e il buffer non viene di conseguenza distrutto

	Buffer dh_buffer;
	dh_buffer.put(publicKey.getData(), publicKey.getSize());
	OS_ASSERT(dh_buffer.getPosition() == dh_buffer.getData());

	uint8 seedSize = 0;
	Buffer seedData;
	generateRandomSeed(seedSize, seedData);

	dh_buffer.xorValue(seedData.getData(), seedData.getSize());

	// Handshake response packet structure: 1 byte random seed lenght, nbytes <random_seed>, 16 bytes (pub_key) DH xor random_seed

	shared_ptr<Buffer> buffer(OS_NEW Buffer());
	buffer->write(&seedSize, sizeof(uint8));
	buffer->write(seedData.getData(), seedData.getSize());
	buffer->write(dh_buffer.getData(), dh_buffer.getSize());

	boost::asio::async_write(getSocket(), boost::asio::buffer(buffer->getData(), buffer->getSize()), boost::asio::transfer_all(), boost::bind(&P2PConnection::handshakeResponseSent, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, buffer, scope->extendTimeout()));
}

void P2PConnection::handshakeResponseSent(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> response, shared_ptr<P2PScope> scope)
{
	scope->cancelTimeout();

	OS_P2P_CONNECTION_STATUS_GUARD();

	updateUpload(transferredBytes);

	if(e)
	{
		// Se si verifica un errore di trasferimento invalida il nodo remoto (viene rimosso in caso di errore DH)

		updateLastEvent(_S("P2P handshake error: ") + e.message().c_str(), logLevelNotice);
	}
	else
	{
		startExchange(scope, false);
	}
}

void P2PConnection::startExchange(shared_ptr<P2PScope> scope, bool starter)
{
	changeStatus(NodeStatus::statusExchanging);

	if(starter)
	{
		// Nel caso in cui sia stata aperta la connessioni invia per primo il pacchetto di hello

		m_localSession->setHelloSended(true);

		p2p::packets::packet_ptr helloPacket = createHelloPacket();
		if(helloPacket != nullptr)
			asyncSendPacket(helloPacket, scope);
		else
			updateLastEvent("Error creating hello packet, connection dropped", logLevelError);
	}
	else
	{
		asyncReceivePacket(scope);
	}
}

bool P2PConnection::asyncSendPacket(p2p::packets::packet_ptr packet, shared_ptr<P2PScope> scope)
{
	if(packet == nullptr || packet->create() == false)
	{
		OS_ASSERTFALSE();
		return false;
	}

	m_localPacket = packet;

	// Applica un'eventuale derivazione della chiave di sessione
	m_localSession->applyDerivation(LocalSession::dtNextPacketSended);
	// Aggiorna l'indice del pacchetto corrente
	m_localSession->updateIV();

	// Serializza il pacchetto
	shared_ptr<Buffer> packetBody(OS_NEW Buffer());
	if(packet->write(*packetBody) == false)
		return false;

	// Cripta il body se necessario
	if(packetBody->getSize() > 0)
	{
		shared_ptr<Buffer> encryptedBody(OS_NEW Buffer());
		if(m_localSession->encryptData(packetBody->getData(), packetBody->getSize(), *encryptedBody) == false)
			return false;

		packetBody = encryptedBody;
	}

	IPacket::HEADER packetHeader;
	packetHeader.type = Convert::toUint16(packet->getType());
	packetHeader.size = packetBody->getSize();

	shared_ptr<Buffer> packetData(OS_NEW Buffer());
	// Cripta l'header del pacchetto
	if(m_localSession->encryptData(&packetHeader, sizeof(IPacket::HEADER), *packetData) == false)
		return false;

	// Si porta alla fine del buffer per accodare il contenuto del pacchetto
	packetData->seekToEnd();

	if(packetBody->getSize() > 0)
	{
		OS_ASSERT(packetData->getPosition() == packetData->getEndPosition());
		// Accoda il contenuto del pacchetto
		packetData->write(packetBody->getData(), packetBody->getSize());
	}

	boost::asio::async_write(getSocket(), boost::asio::buffer(packetData->getData(), packetData->getSize()), boost::asio::transfer_all(), boost::bind(&P2PConnection::handlePacketSended, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, packetData, scope->extendTimeout()));

	return true;
}

void P2PConnection::handlePacketSended(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> packetData, shared_ptr<P2PScope> scope)
{
	scope->cancelTimeout();

	OS_P2P_CONNECTION_STATUS_GUARD();

	updateUpload(transferredBytes);

	if(e)
	{
		updateLastEvent(_S("P2P error: ") + e.message().c_str(), logLevelNotice);
	}
	else
	{
		OS_ASSERT(packetData->getSize() == transferredBytes);

		onPacketSended(scope);
	}
}

void P2PConnection::onPacketSended(shared_ptr<P2PScope> scope)
{
	if(getRun())
		asyncReceivePacket(scope);
}

void P2PConnection::asyncReceivePacket(shared_ptr<P2PScope> scope)
{
	// Applica un'eventuale derivazione della chiave di sessione
	m_localSession->applyDerivation(LocalSession::dtNextPacketReceived);
	// Aggiorna l'indice del pacchetto corrente
	m_localSession->updateIV();

	shared_ptr<Buffer> encryptedPacketHeader(OS_NEW Buffer());
	encryptedPacketHeader->reserve(sizeof(IPacket::HEADER));
	boost::asio::async_read(getSocket(), boost::asio::buffer(encryptedPacketHeader->getData(), encryptedPacketHeader->getSize()), boost::asio::transfer_all(), boost::bind(&P2PConnection::handlePacketHeader, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, encryptedPacketHeader, scope->extendTimeout()));
}

void P2PConnection::handlePacketHeader(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> encryptedPacketHeader, shared_ptr<P2PScope> scope)
{
	scope->cancelTimeout();

	OS_P2P_CONNECTION_STATUS_GUARD();

	updateDownload(transferredBytes);

	if(e)
	{
		updateLastEvent(_S("P2P error: ") + e.message().c_str(), logLevelNotice);
	}
	else
	{
		OS_ASSERT(encryptedPacketHeader->getSize() == transferredBytes);

		Buffer decryptedPacketHeader;
		// Decripta l'header
		if(m_localSession->decryptData(encryptedPacketHeader->getData(), encryptedPacketHeader->getSize(), decryptedPacketHeader) == false)
			return;

		// Carica l'header
		IPacket::HEADER packetHeader;
		if(decryptedPacketHeader.read(&packetHeader, sizeof(IPacket::HEADER)) == false)
			return;

		// Alloca il pacchetto
		m_remotePacket = allocPacket(Convert::toPacketType(packetHeader.type));
		if(m_remotePacket == nullptr)
			return;

		// Verifica che la dimensione richiesta non superi quella massima prevista
		if(packetHeader.size > OS_P2P_MAX_PACKET_SIZE)
			return;

		// Verifica che il pacchetto abbia un body
		if(packetHeader.size > 0)
		{
			shared_ptr<Buffer> encryptedPacketBody(OS_NEW Buffer());
			// Assicura che il buffer sia sufficientemente grande a ricevere il pacchetto
			encryptedPacketBody->reserve(packetHeader.size);

			boost::asio::async_read(getSocket(), boost::asio::buffer(encryptedPacketBody->getData(), encryptedPacketBody->getSize()), boost::asio::transfer_all(), boost::bind(&P2PConnection::handlePacketBody, get_this_ptr<P2PConnection>(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, encryptedPacketBody, scope->extendTimeout()));
		}
		else
		{
			onPacketReceived(scope);
		}
	}
}

void P2PConnection::handlePacketBody(const boost::system::error_code &e, size_t transferredBytes, shared_ptr<Buffer> encryptedPacketBody, shared_ptr<P2PScope> scope)
{
	scope->cancelTimeout();

	OS_P2P_CONNECTION_STATUS_GUARD();

	updateDownload(transferredBytes);

	if(e)
	{
		updateLastEvent(_S("P2P error: ") + e.message().c_str(), logLevelNotice);
	}
	else
	{
		OS_ASSERT(encryptedPacketBody->getSize() == transferredBytes);

		Buffer decryptedPacketBody;
		// Decripta il pacchetto
		if(m_localSession->decryptData(encryptedPacketBody->getData(), encryptedPacketBody->getSize(), decryptedPacketBody) == false)
			return;

		// Carica il pacchetto
		if(m_remotePacket->read(decryptedPacketBody) == false)
			return;

		onPacketReceived(scope);
	}
}

void P2PConnection::onPacketReceived(shared_ptr<P2PScope> scope)
{
	OS_ASSERT(m_remotePacket != nullptr);

	if(validatePacket(m_localPacket, m_remotePacket) == false)
		return;

	if(handlePacket(m_localPacket, m_remotePacket, scope) == false)
		return;

	if(m_localPacket != nullptr)
		asyncSendPacket(m_localPacket, scope);
}

void P2PConnection::updateDownload(size_t size)
{
	OS_P2P_CONNECTION_STATUS_GUARD();
	m_status->addDownloadedBytes(static_cast<uint32>(size));
}

void P2PConnection::updateUpload(size_t size)
{
	OS_P2P_CONNECTION_STATUS_GUARD();
	m_status->addUploadedBytes(static_cast<uint32>(size));
}

void P2PConnection::handleLoopback()
{
	{
		OS_LOCK(m_cs);

		m_loopback = true;
		m_run = false;

		m_remoteSession->setError(true);
	}

	close();
}

bool P2PConnection::getLoopback() const
{
	OS_LOCK(m_cs);
	return m_loopback;
}

shared_ptr<ConnectionScope> P2PConnection::createScope(const boost::posix_time::time_duration &timeout)
{
	shared_ptr<ConnectionScope> scope(OS_NEW P2PScope(getService(), get_this_ptr<P2PConnection>()));
	scope->extendTimeout(timeout);
	return scope;
}

TCPSocket & P2PConnection::getSocket()
{
	return *m_socket;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
