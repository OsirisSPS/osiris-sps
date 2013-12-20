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
#include "isisconnection.h"

#include "conversions.h"
#include "connectionstatusguard.h"
#include "iportaldatabase.h"
#include "engine.h"
#include "entitiessnapshotmanager.h"
#include "httpclient.h"
#include "httpparser.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "idesystem.h"
#include "isisconnectionstatus.h"
#include "isisendpoint.h"
#include "lock.h"
#include "log.h"
#include "httpcontent.h"
#include "httppostparam.h"
#include "objectsuser.h"
#include "osiriscommon.h"
#include "portalsportal.h"
#include "portalstransaction.h"
#include "realtimestats.h"
#include "regexmanager.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

#define OS_ISIS_CONNECTION_STATUS_GUARD()		shared_ptr<ConnectionStatusGuard> __status_guard = lockStatus()

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IsisConnection::IsisConnection(shared_ptr<Portal> portal, shared_ptr<IsisEndpoint> endpoint, shared_ptr<ConnectionsManager> connectionsManager, shared_ptr<TCPSocket> socket, const HttpUrl &url) : ConnectionBase(connectionsManager, socket, url),
																																																			 m_endpoint(endpoint),
																																																			 m_status(OS_NEW IsisConnectionStatus(portal, endpoint->getName()))
{
	m_firstrequest = true;

	// We dump also the ID, because maybe more connections on the same IsisEndPoint.
	m_realtimeStatsCounterName = _S("Isis - ") + endpoint->getUrl().toString() + _S(" for portal '") + portal->getName() + _S("' (") + getID() + _S(")");
	RealtimeStatsManager::add(_S("Isis"), m_realtimeStatsCounterName, RealtimeStatsCounter::ctAbsolute, true, _S("status"));	
}

IsisConnection::~IsisConnection()
{
	realtimeStatsSet(0);
}

shared_ptr<Portal> IsisConnection::getPortal() const
{
	return m_status->getPortal();
}

shared_ptr<IsisConnectionStatus> IsisConnection::takeStatusSnapshot() const
{
	return m_status->clone();
}

shared_ptr<ConnectionStatusGuard> IsisConnection::lockStatus()
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

void IsisConnection::run()
{
	OS_ISIS_CONNECTION_STATUS_GUARD();

	m_status->setStatus(IsisConnectionStatus::isSendRequest);

	step(nullptr, nullptr);

	ConnectionBase::run();
}

boost::posix_time::time_duration IsisConnection::getTimeout() const
{
	// TODO: solo nel caso in cui lo stato sia l'attesa della risposta di request.php restituisce il timeout "isis" che viene passato dall'endpoint nel costruttore
	shared_ptr<IsisEndpoint> endPoint = getEndpoint();
	if((endPoint != nullptr) && (m_status->getStatus() == IsisConnectionStatus::isReceiveRequest))
	{
		uint32 timeout = endPoint->getIsisOptions().getOption(_S("tuning.request.timeout"));
		if(timeout > 300 * 1000000)
			timeout = 300 * 1000000;
		if(timeout < 10 * 1000000)
			timeout = 10 * 1000000;

		timeout += 2 * 1000000; // Aggiungo un paio di secondi per sicurezza.

		return boost::posix_time::microseconds(timeout);
	}

	return ConnectionBase::getTimeout();
}

void IsisConnection::onResponseCompleted(shared_ptr<ConnectionScope> scope)
{
	ConnectionBase::onResponseCompleted(scope);

	OS_ISIS_CONNECTION_STATUS_GUARD();

	shared_ptr<HttpResponse> response = getResponse();
	OS_ASSERT(response != nullptr);

	shared_ptr<HttpContent> responseContent = response->getContent();
	OS_ASSERT(responseContent != nullptr);

	if(response->getStatusCode() != 200)
	{
		String desc = String::format(_S("Invalid status code: %u").c_str(), response->getStatusCode());
		error(scope, desc, false, false);
		return;
	}

	// Estraggo l'XML dalla risposta.
	// Alcuni web-hosting inseriscono codice html (ad esempio Google Analytics).
	const Buffer &bufferResponse = responseContent->getContent();
	String xmlResponse;
	xmlResponse.from_utf8(std :: string(reinterpret_cast<const char *>(bufferResponse.getData()), bufferResponse.getSize()));

	String::size_type posStart = xmlResponse.find(_S("<?xml"));
	if(posStart == String::npos)
	{
		error(scope, _S("Invalid response."), false, false);
		return;
	}
	String::size_type posEnd = xmlResponse.find(_S("</isis>"));
	if(posEnd == String::npos)
	{
		error(scope, _S("Invalid response."), false, false);
		return;
	}
	xmlResponse = xmlResponse.substr(posStart,posEnd-posStart + 7);

	// Controllo che la risposta sia XML
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	//if(document->parseBuffer(responseContent->getContent()) == false)
	if(document->parseString(xmlResponse) == false)
	{
		error(scope, _S("Invalid response."), false, false);
		return;
	}

	shared_ptr<XMLNode> root = document->getRoot();
	// Controllo che la risposta non sia un'errore fatale
	switch(static_cast<ErrorCode>(root->getAttributeInt32(_S("error_code"))))
	{
	case ieSuccess:			step(scope, root);

							break;

	case ieFatal:			// Un esempio di errore fatale è il ban dell'ip, o una richiesta su un portale non abilitato.
							error(scope, root->getAttributeString(_S("error_desc")), true);

							break;

	case ieError:			// Un esempio di errore normale è la non-disponibilità temporanea del server, o un errore di comunicazione.
							error(scope, root->getAttributeString(_S("error_desc")), false, false);

							break;

	case ieRetry:			// Un esempio di errore è il timeout lato server.
							error(scope, root->getAttributeString(_S("error_desc")), false, true);

							break;

	case ieSystemUpdate:	m_status->setStatus(IsisConnectionStatus::isSendSystem);
							step(scope, root);

							break;

	default:				OS_ASSERTFALSE();

							break;
	}
}

void IsisConnection::onResponseError(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{
    ConnectionBase::onResponseError(e, scope);

    if(isAsioEOF(e))
    {
        resolveUrl(scope);
    }
	else
	{
		//error(scope, e.message(), false, true); // Raffo: "Http error: Operazione completata" .. che è?
	}
}

void IsisConnection::onTimeout(const boost::system::error_code &e, shared_ptr<ConnectionScope> scope)
{
	ConnectionBase::onTimeout(e, scope);

	// Il concetto è che se il timeout capita quando sto inviando una risposta ad un job, ci riprovo subito.
	// Questo per evitare che il visitatore che attende quel job vada in timeout aspettando che un'altro
	// IsisConnection se ne occupi, e anche per evitare che la risposta del job (la richiesta in locale) sia stata generata inutilmente.
	// Altrimenti, lascio che l'IsisConnection venga terminato, e ci riproverò in seguito.
	// TOFIX: non dovrebbe modificare lo stato ad un valore enumerato diverso per evitare, se torna ancora qui, di andare in loop?
	if(m_status->getStatus() == IsisConnectionStatus::isReceiveResponse)
		resolveUrl(scope);
}

shared_ptr<HttpRequest> IsisConnection::initializeRequest(const String &url)
{
	m_request.reset(OS_NEW HttpRequest());
	m_response.reset(OS_NEW HttpResponse());

	shared_ptr<IsisEndpoint> endPoint = getEndpoint();
	if(endPoint == nullptr)
		return nullptr;

	shared_ptr<Portal> portal = getPortal();
	if(portal == nullptr)
		return nullptr;

	shared_ptr<HttpRequest> request = getRequest();

	HttpUrl fullUrl((endPoint->getUrl().toString() + _S("/") + url).to_ascii());

	// URGENT, da sistemare?
	setUrl(fullUrl); // Non dovrebbe essere request->setUrl ?
	request->setUrl(fullUrl); // Se non la faccio è vuota al secondo giro

	request->setMethod(httpMethodPost);
	request->setUserAgent(Engine::getDefaultHttpUserAgent());
	request->setAccept("*/*;q=0.1");
	request->setAcceptLanguage("en;q=0.9");
	request->setAcceptCharset("utf-8, *;q=0.1");

	request->setPostParamString(_W("portal"), portal->getPortalID().toWide());
	request->setPostParamString(_W("pov"), portal->getPovID().toWide());
	request->setPostParamString(_W("password"), endPoint->getPassword().to_wide());

	return request;
}

void IsisConnection::sendTheRequest(shared_ptr<ConnectionScope> scope)
{
	// URGENT, non so come sistemarlo; magari basta un flag di bool da passare al costruttore della base,
	// così qui gli posso dire che mi smazzo io quando avviare la prima richiesta.
	if(m_firstrequest)
	{
		m_firstrequest = false;
	}
	else
	{
		// VERYURGENT
		// Il problema della ricerca di "balle spaziali" sulla ciurma.
		// La causa non è ancora stata accertata.
		// Clodo: Personalmente, penso sia un problema legato al keep-alive, forse che la precedente richiesta non era completa, o che
		// arrivano altri bytes nella richiesta successiva...
		// Non so esattamente. Però non sembra minimamente un problema di timeout, e il fatto che l'errore non si presenti "disattivando"
		// il keep-alive mi fa pensare solo a un problema legato a + richieste con la stessa connessione.

		// Aggiungo che sarebbe veramente importante capire il problema: la differenza in prestazioni è notevole.

		//sendRequest(scope);
		resolveUrl(scope);
	}
}

void IsisConnection::step(shared_ptr<ConnectionScope> scope, shared_ptr<XMLNode> data)
{
	OS_ISIS_CONNECTION_STATUS_GUARD();

	shared_ptr<IsisEndpoint> endPoint = getEndpoint();
	if(endPoint == nullptr)
		return;

	shared_ptr<Portal> portal = getPortal();
	if(portal == nullptr)
		return;
	
	try
	{
		// Razor
		/*
		entities::snapshot_profile_ptr profile = portal->getSnapshotManager()->getProfile(endPoint->getSnapshotProfileID());
		// Se non c'è il profilo, e non sto ricevendo un "system", chiamo la system in modo da costruirlo.
		if( (profile == nullptr) && (m_status->getStatus() != IsisConnectionStatus::isReceiveSystem) )
		{
			m_status->setStatus(IsisConnectionStatus::isSendSystem);
		}
		*/

		switch(m_status->getStatus())
		{
		case IsisConnectionStatus::isSendSystem:		{
															log(_S("Request system information"));
															realtimeStatsSet(1);

															initializeRequest(_S("system.php"));

															m_status->setStatus(IsisConnectionStatus::isReceiveSystem);
															sendTheRequest(scope);
														}

														break;

		case IsisConnectionStatus::isReceiveSystem:		{
															realtimeStatsSet(2);

															// Mi salvo il vecchio ID, perchè se cambia dovrò piallare quello vecchio.
															//String oldSnapshotProfile = endPoint->getSnapshotProfileID();

															endPoint->parseSystemInformation(data);

															/*
															if(portal->getPortalID().isMonarchic() == false)
															{
																String newSnapshotProfile = endPoint->getSnapshotProfileID();

																if(newSnapshotProfile.empty())
																{
																	error(scope, _S("In Isis configuration, no user for anarchic portal was specified."), true);
																	return;
																}

																shared_ptr<PortalsTransaction> databaseWork = portal->startTransaction(true);
																shared_ptr<IPortalDatabase> database = databaseWork->getDatabase();
																database->beginTransaction();

																// TOFIX: Da IDE, se uno toglie un Isis, dovrei scandagliare gli altri Isis e decidere se va tolto anche il profilo.
																if(oldSnapshotProfile != newSnapshotProfile)
																{
																	// Distruggo il vecchio profilo
																	portal->getSnapshotManager()->destroyProfile(database, oldSnapshotProfile);
																}

																shared_ptr<ObjectsUser> user = objects_user_cast(portal->getObject(database, endPoint->getUser().toUTF16()));
																if(user == nullptr)
																{
																	error(scope, _S("Unknown requested user for anarchic portal."), false, false);
																	return;
																}

																// Controllo che esista, o creo, il profilo giusto.
																portal->getSnapshotManager()->ensureProfile(database, newSnapshotProfile, user);

																database->commit();
															}
															*/

															if(endPoint->canStart())
															{
																m_status->setStatus(IsisConnectionStatus::isSendRequest);
																step(scope, data);
															}
														}

														break;

		case IsisConnectionStatus::isSendRequest:		{
															log(_S("Waiting for Isis job"));
															realtimeStatsSet(1);

															shared_ptr<HttpRequest> request = initializeRequest(_S("request.php"));
															if(request == nullptr)
															{
																OS_ASSERTFALSE();
																return;
															}

															request->setPostParamString(_W("system"), static_cast<String>(endPoint->getIsisOptions().getOption(_S("isis.last_modification"))).to_wide());

															m_status->setStatus(IsisConnectionStatus::isReceiveRequest);

															sendTheRequest(scope);
														}

														break;

		case IsisConnectionStatus::isReceiveRequest:	{
															try
															{
																realtimeStatsSet(2);

																String isisVersion = data->getAttributeString(_S("isis_version"));
																String isisRequestRequestID = data->getAttributeString(_S("request_id"));
																String isisRequestPriority = data->getAttributeString(_S("priority"));
																String isisRequestVerb = data->getAttributeString(_S("verb"));
																String isisRequestUrl = data->getAttributeString(_S("url"));
																String isisRequestUserAgent = data->getAttributeString(_S("user_agent"));
																String isisRequestCulture = data->getAttributeString(_S("culture"));
																String isisRequestIfModifiedSince = data->getAttributeString(_S("if_modified_since"));
																String isisRequestContentType = data->getAttributeString(_S("content_type"));
																String isisRequestPostData = data->getAttributeString(_S("post_data"));


																// E' riferita al portale giusto?
																HttpUrl localUrl(IdeSystem::instance()->getLocalUrl(isisRequestUrl).to_ascii());
																std::string urlParams = localUrl.getParams();
																ordered_map<std::wstring, std::wstring> params;
																HttpParser::loadUrlParams(urlParams, params);

																if(params.exists(_W("portal")))
																{
																	String portalUrlParam = params.get(_W("portal"));																	
																	if(portalUrlParam != portal->getFullPovID())
																	{
																		OS_EXCEPTION("Request on wrong portal");
																	}
																	/*
																	String portalUrlParam = params.get(_W("portal"));
																	String userUrlParam = params.get(_W("user"));
																	if( (portalUrlParam != portal->getPortalID().getString()) || (userUrlParam != portal->getUserID().getString()) )
																	{
																		OS_EXCEPTION("Request on wrong portal");
																	}
																	*/
																}
																else
																{
																	// URGENT, tutte le richieste dovrebbero averlo...
																	// OS_EXCEPTION(_S("Missing portal param."));
																}

																// Qui eseguo la richiesta in locale

																shared_ptr<boost::asio::io_service> service = getService();
																OS_EXCEPT_IF(service == nullptr, "Internal error (null service)");

																HttpClient client(service, Engine::instance()->createTCPSocket(service, false, false));

																// Specify that Isis want redirect responses
																client.setFollowRedirects(false);

																shared_ptr<HttpRequest> clientRequest = client.getRequest();

																if(isisRequestVerb == _S("GET"))
																	clientRequest->setMethod(httpMethodGet);
																else if (isisRequestVerb == _S("POST"))
																	clientRequest->setMethod(httpMethodPost);
																else
																	OS_EXCEPTION("Verb not supported");
																clientRequest->setUserAgent(isisRequestUserAgent.to_ascii());
																clientRequest->setAccept("*/*;q=0.1");
																clientRequest->setAcceptLanguage((isisRequestCulture + _S(";q=0.9")).to_ascii());
																clientRequest->setAcceptCharset("utf-8, *;q=0.1");
																//localRequestHeader += _S("Cookie2: $Version=1\r\n");
																//clientRequest->setCookie(_S("cookies"), true);
																//clientRequest->setCookie("os_session", constants::empty_string);
																clientRequest->setCookie("isis_url", endPoint->getUrl().toString());

																boost::posix_time::ptime timeModifiedSince;
																if( (isisRequestIfModifiedSince.empty() == false) && (HttpParser::parseDate(isisRequestIfModifiedSince.to_ascii(), timeModifiedSince)) )
																	clientRequest->setIfModifiedSince(timeModifiedSince);

																if(isisRequestContentType.empty() == false)
																	clientRequest->setContentType(isisRequestContentType.to_ascii());

																if(isisRequestPostData.empty() == false)
																{
																	//clientRequest->parsePostParams(isisRequestPostData.to_ascii(), OS_HTTP_CONTENTTYPE_TEXTPLAIN, String::EMPTY);
																	clientRequest->parsePostParams(isisRequestPostData.to_ascii(), OS_HTTP_CONTENTTYPE_URLENCODED, constants::empty_string);
																}

																// Richiesta in locale ...
																log(_S("Executing Isis job"));
																realtimeStatsSet(3);

																if(client.perform(localUrl) == false)
																	OS_EXCEPTION("Local request failed");

																uint32 localResponseStatus = client.getResponse()->getStatusCode();
																std::string localResponseHeaders = client.getResponse()->getRawHeaders(false);
																const Buffer &localResponseContent = client.getResponse()->getContent()->getContent();

																shared_ptr<HttpRequest> request = initializeRequest(_S("response.php"));
																if(request == nullptr)
																{
																	OS_ASSERTFALSE();
																	return;
																}

																request->setPostParamString(_W("request"), isisRequestRequestID.to_wide());
																request->setPostParamString(_W("status"), conversions::to_utf16(localResponseStatus).to_wide());

																Buffer localResponseHeaderBuffer;
																localResponseHeaderBuffer.write(localResponseHeaders.data(), static_cast<uint32>(localResponseHeaders.size()));

																shared_ptr<HttpPostParam> postParamHeader(OS_NEW HttpPostParam());
																postParamHeader->setName(_W("file_header"));
																//postParamHeader->setString(localResponseHeader);
																postParamHeader->setBuffer(localResponseHeaderBuffer);
																postParamHeader->setFilename(_W("header.txt"));
																postParamHeader->setContentType(OS_HTTP_CONTENTTYPE_TEXTPLAIN);
																request->addPostParam(_W("file_header"), postParamHeader);

																// For example, in http 304 the body are empty.
																// Commentato 01/12/2008, perchè se non lo valorizzo, lato php va in crisi (non valorizza gli altri parametri), e non so riuscito a capire xchè.
																// if(localResponseContent.getSize() != 0)
																{
																	shared_ptr<HttpPostParam> postParamBody(OS_NEW HttpPostParam());
																	postParamBody->setName(_W("file_body"));
																	postParamBody->setBuffer(localResponseContent);
																	postParamBody->setFilename(_W("body.bin"));
																	postParamBody->setContentType("application/octet-stream");
																	request->addPostParam(_W("file_body"), postParamBody);
																}

																log(_S("Sending Isis job answer"));
																realtimeStatsSet(4);

																m_status->addRequest();
																m_status->setStatus(IsisConnectionStatus::isReceiveResponse);

																sendTheRequest(scope);
															}
															catch(std::exception &e)
															{
																error(scope, e.what(), false, false);

																// TODO: Qui ci sarebbe da informare Isis che non è riuscita, in modo che non aspetti la risposta.
																// Idem se la risposta non è un http 200/304.
															}
														}

														break;

		case IsisConnectionStatus::isReceiveResponse:	{
															log(_S("Isis job completed."));
															realtimeStatsSet(5);

															m_status->setStatus(IsisConnectionStatus::isSendRequest);
															step(scope, nullptr);
														}

														break;

		default:										OS_ASSERTFALSE();
														break;
		}
	}
	catch(std::exception &e)
	{
		error(scope, e.what(), false, false);
	}
}

void IsisConnection::error(shared_ptr<ConnectionScope> scope, const String& desc, bool fatal, bool retry)
{
	OS_ISIS_CONNECTION_STATUS_GUARD();

	if(fatal)
	{
		log(_S("Isis Fatal Error - ") + desc);
		// Se l'errore è fatale, disabilito l'Isis.
		// Se capita un fatale (ad esempio ban IP, o portale rifiutato, o X errori normali (indirizzo sbagliato?).
		// L'utente dovrà ri-attivarlo a mano.

		shared_ptr<IsisEndpoint> endpoint = getEndpoint();
		if(endpoint != nullptr)
		{
			endpoint->setEnabled(false);
			endpoint->setLastEvent(_S("Fatal Error: ") + desc);
		}

		m_status->setStatus(IsisConnectionStatus::isNone);
	}
	else
	{
		// Qui la logica è che se l'errore non è fatale, si può ritentare anche lo stesso step.
		// Certo è inutile riprovare al volo, xchè sarebbero tentativi connessioni in continuazione a siti magari semplicemente down.
		// Metterei una pausa in un'opzione, a livello di sistema.
		// Non tutti però. Timeout ad esempio, non ha "ritardi" ma viene bensì eseguito subito. Da notare che non loggo nemmeno l'errore.

		// Old:
		// Qui attende un bel pò (fare un'opzione?), ad esempio se Isis è down.
		//log(String::format(_S("Waiting %d seconds before trying again").c_str(), m_waitingAfterFatal));
		//PlatformManager::instance()->sleep(waitingAfterFatal * 1000);
		// cercarmi deadline_timer

		//m_status = isSendRequest; // Non son sicuro di questo, forse potrebbe ritentar diretto lo stesso step..
		if(retry)
		{
			sendTheRequest(scope);
		}
		else
		{
			log(_S("Isis Error - ") + desc);
			m_status->setStatus(IsisConnectionStatus::isNone);
		}
	}
}

void IsisConnection::log(const String &desc)
{
	m_status->setLastEvent(desc);

	shared_ptr<IsisEndpoint> endpoint = getEndpoint();
	if(endpoint == nullptr)
		return;

	shared_ptr<Portal> portal = getPortal();
	if(portal == nullptr)
		return;

	String endPointUrl = endpoint->getUrl().toString();
	String suffix = String::format(_S(" (portal:'%S' isis:'%S')").c_str(), portal->getName().c_str(), endPointUrl.c_str());
	OS_LOG_NOTICE(desc + _S(" ") + suffix);
	endpoint->setLastEvent(desc);
}

void IsisConnection::realtimeStatsSet(const uint32 &status)
{
	shared_ptr<IsisEndpoint> endpoint = getEndpoint();
	if(endpoint == nullptr)
		return;

	//shared_ptr<Portal> portal = getPortal();
	//if(portal == nullptr)
	//	return;
	//String endPointUrl = endPoint->getUrl().toString();
	//String suffix = String::format(_S(" (portal:'%S' isis:'%S')").c_str(), portal->getName().c_str(), endPointUrl.c_str());
	//m_realtimeStats.reset(OS_NEW RealtimeStatsScopeTimer(_S("Isis"), desc + _S(" ") + suffix));

	RealtimeStatsManager::signal(m_realtimeStatsCounterName, status);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
