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
#include "iideportalpage.h"

#include "boost/bind.hpp"
#include "convert.h"
#include "dataaccount.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "entitiessnapshotmanager.h"
#include "omlcontext.h"
#include "omlmanager.h"
#include "ideaccount.h"
#include "ideblock.h"
#include "idefunctionsfunctions.h"
#include "idepages.h"
#include "idepathway.h"
#include "idesession.h"
#include "idesessionportal.h"
#include "ideskin.h"
#include "ideassistant.h"
#include "idesystem.h"
#include "ideportalaccount.h"
#include "ideviewiobjectviewer.h"
#include "isisendpoint.h"
#include "languagemanager.h"
#include "objectsinstance.h"
#include "objectsirevisionable.h"
#include "objectsobjectscache.h"
#include "objectssystem.h"
#include "objectsuser.h"
#include "osiriscommon.h"
#include "portalsaccount.h"
#include "idesession.h"
#include "portalsmessenger.h"
#include "portalsoptionsshared.h"
#include "portalsportaloptions.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "htmlevent.h"
#include "htmlmetatag.h"
#include "htmlwriter.h"
#include "httpresponse.h"
#include "httprequest.h"
#include "htmlcontrols.h"
#include "htmlimage.h"
#include "htmlliteral.h"
#include "htmlpopup.h"
#include "htmlscript.h"
#include "xmlnode.h"
#include "xmlportalexporter.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IPortalPage::EVENT_ONPORTALLOGIN = _S("onPortalLogin");
const String IPortalPage::EVENT_ONPORTALLOGOUT = _S("onPortalLogout");

//////////////////////////////////////////////////////////////////////

IPortalPage::IPortalPage(shared_ptr<HttpSession> session) : PageBase(session),
													  m_loadInstances(true)
{
	getEvents()->get(EVENT_ONPORTALLOGIN)->connect(boost::bind(&IPortalPage::onPortalLogin, this));
	getEvents()->get(EVENT_ONPORTALLOGOUT)->connect(boost::bind(&IPortalPage::onPortalLogout, this));
}

IPortalPage::~IPortalPage()
{

}

String IPortalPage::getLanguage() const
{
	if(getRequestSource() != rsOsiris)
	{
		// Qui c'è da chiederlo al cookie, al browser
		String language = LanguageManager::instance()->localize(getRequest()->getCookie("os_culture"));
		if(language.empty() == false)
			return language;

		// Lingue specificate nel browser
		String languages = getRequest()->getAcceptLanguage();
		String::size_type posComma = languages.find(_S(";"));
		if(posComma != String::npos)
			languages = languages.substr(0, posComma);
		StringVector tarray;
		utils::split(languages, _S(","), tarray);
		for(uint32 a1 = 0; a1 < tarray.size(); a1++)
		{
			language = LanguageManager::instance()->localize(tarray[a1]);
			if(language.empty() == false)
				return language;
		}
	}			

	return IPage::getLanguage();
}

int32 IPortalPage::getTimeOffset() const
{
	if(getRequestSource() != rsOsiris)
		return getOption("language.time_offset");		

	return IPage::getTimeOffset();
}

int32 IPortalPage::getTimeDST() const
{
	if(getRequestSource() != rsOsiris)
		return getOption("language.time_dst");				
	
	return IPage::getTimeDST();
}

shared_ptr<IdeSkin> IPortalPage::getSkin() const
{	
	shared_ptr<IdeSkin> skin;
	if((getRequestSource() != rsOsiris) && (m_isisEndpoint != null))
	{
		String id = m_isisEndpoint->getIsisOptions().getOption(Options::ide_options::skin_id);
		skin = IdeSystem::instance()->getSkin(SkinID(id.to_ascii()));
	}
	else
	{
		shared_ptr<IdeSession> ideSession = getSessionAccount();
		if(ideSession != null && ideSession->isLogged())
			skin = ideSession->getCurrentSkin();
	}

	if(skin != null && skin->isValid())
		return skin;	

	return PageBase::getSkin();
}

shared_ptr<ObjectsIObject> IPortalPage::getObject(const ObjectID &id) const
{
	shared_ptr<Portal> portal = getPortal();
	if(portal != null)
		return portal->getObject(getDatabase(), id);

	return null;
}


String IPortalPage::getStabilityStatus()
{
	String stabilityStatus;
	switch(getPortal()->getSnapshotManager()->getStabilityStatus())
	{
	case EntitiesSnapshotManager::ssNone:
		stabilityStatus=_S("snapshot_none");
		break;
	case EntitiesSnapshotManager::ssAccept:
		stabilityStatus=_S("snapshot_accept");
		break;
	case EntitiesSnapshotManager::ssPrepare:
		stabilityStatus=_S("snapshot_prepare");
		break;
	case EntitiesSnapshotManager::ssStabilityUsers:
		stabilityStatus=_S("snapshot_users_stability");
		break;
	case EntitiesSnapshotManager::ssStabilityObjects:
		stabilityStatus=_S("snapshot_objects_stability");
		break;
	case EntitiesSnapshotManager::ssStatisticsObjects:
		stabilityStatus=_S("snapshot_objects_statistics");
		break;
	case EntitiesSnapshotManager::ssStatisticsUsers:
		stabilityStatus=_S("snapshot_users_statistics");
		break;
	case EntitiesSnapshotManager::ssSearchObjects:
		stabilityStatus=_S("snapshot_objects_search");
		break;
	case EntitiesSnapshotManager::ssDone:
		stabilityStatus=_S("snapshot_done");
		break;
	}

	return stabilityStatus;
}

EntityID IPortalPage::getTargetObject() const
{
	return ObjectsSystem::instance()->getRootID();
}

bool IPortalPage::getShowInstance(shared_ptr<ObjectsInstance> instance) const
{
	if(instance == null)
		return false;

	// Di default tutte le pagine del portale caricano istanze che non siano nell'area content
	return instance->getArea() != iaContent;
}

/*
void IPortalPage::redirect(ide::portal::PortalPage page)
{
	shared_ptr<Portal> portal = getPortal();
	OS_ASSERT(portal != null);
	if(portal != null)
		redirect(portal->getLink(page));
}
*/

void IPortalPage::redirectPage(const std::string &page) // TOCLEAN: mai usato?
{
	shared_ptr<Portal> portal = getPortal();
	OS_ASSERT(portal != null);
	if(portal != null)
		redirect(portal->getLink(page));
}

uint32 IPortalPage::loadInstances(const EntityID &ID)
{
	shared_ptr<EntitiesEntity> entity = getPortal()->getEntity(getDatabase(), ID);
	if(entity == null)
		return 0;

	return onLoadInstances(entity, ID);
}

DataItem IPortalPage::getOption(const String &name) const
{
	if((getRequestSource() != rsOsiris) && (m_isisEndpoint != null))
		return m_isisEndpoint->getIsisOptions().getOption(name);

	return Options::instance()->getOptionValue(name);
}

String IPortalPage::getRequestIsisUrl() const 
{
	if(getRequestSource() != rsIsis)
		return String::EMPTY;

	return getRequest()->getCookie("isis_url");
}

void IPortalPage::updateReferenceDate()
{
	getSessionAccount()->updateReferenceDate();
}

uint32 IPortalPage::onLoadInstances(shared_ptr<EntitiesEntity> entity, EntityID origin)
{
	if(entity == null)
		return 0;

	uint32 count = 0;
	//if(entity->getPrimaryID() != ObjectID::getRootID())
	if(ObjectsSystem::instance()->isVirtual(entity->getEntityID()) == false)
	{
		String parentID = entity->getParentID().toUTF16();
		shared_ptr<EntitiesEntity> parentEntity = getPortal()->getEntity(getDatabase(), parentID.to_ascii());
		count += onLoadInstances(parentEntity, origin);
	}

	if(entity->getObjectType() == portalObjectTypeSection)
	{
		shared_ptr<EntitiesEntities> childs = entity->getChilds(getDatabase(), portalObjectTypeInstance, RangeUint32(0, 0), EntitiesEntity::coPositionAsc);
		for(EntitiesEntities::iterator i = childs->begin(); i != childs->end(); ++i)
		{
			shared_ptr<EntitiesEntity> instanceEntity = childs->get(getDatabase(), *i);
			if(instanceEntity != null)
			{
				shared_ptr<ObjectsInstance> instanceObject = objects_instance_cast(instanceEntity->getCurrent());
				if(instanceObject != null)
				{
					bool show = true;
					switch(instanceObject->getInherit())
					{
					case iiNone:			// Demanda la visualizzazione dell'istanza al relativo viewer
													show = false;
													break;

					case iiParent:			// Osiris <0.13 RC3 - Mostra l'istanza solo se l'origine di caricamento delle istanze è il padre dell'istanza
													// show = (origin->getPrimary() == instanceObject->getParent());													
													// Osiris >0.13 RC3 - Mostra l'istanza solo se l'origine di caricamento delle istanze è il padre dell'istanza, ed è in "view".
													show = ( (origin == instanceObject->getParent()) && (getPageName() == _S("portal.pages.view")) );
													break;

					case iiRecursive:		// Propaga la visualizzazione dell'istanza indipendentemente dal padre e dalle pagine.
													show = true;
													break;

					default:						OS_ASSERTFALSE();
													break;
					}

					// Verifica se caricare l'istanza
					if(show && getShowInstance(instanceObject))
					{
						// Crea il controllo di vista dell'istanza
						shared_ptr<IHtmlControl> instanceControl = instanceObject->getDescriptor()->createViewControl(instanceEntity);
						if(instanceControl != null)
						{
							// Inserisce il controllo nell'area di riferimento dell'istanza
							getArea(static_cast<PageArea>(instanceObject->getArea()))->getControls()->add(instanceControl);
							// Aggiorna il numero di istanze renderizzate
							count++;
						}
					}
				}
			}
		}
	}

	return count;
}

// Pathway
void IPortalPage::onPathway() const
{
	PageBase::onPathway();

	getPathway()->add(getPortal()->getName(), getPortal()->getViewObjectLink(ObjectsSystem::instance()->getRootID()));

	onPathway2();

	/*
	if(getPageName() != _S("portal.pages.view"))
	{
		getPathway()->add(getPageName() + _S(".title")), getPagePathway(), ObjectID::getRootID());		
	}
	*/
}

void IPortalPage::onPathway2() const
{
	if(getPageName().empty() == false)
		getPathway()->add(getText(getPageName() + _S(".title")), getPagePathway());		
}
/* Razor: per me inutile
String IPortalPage::getText(const String &name) const
{
	shared_ptr<IdeSession> ideSession = getLoggedUser();
	if(loggedUser != null && loggedUser->isLogged() && (loggedUser->getGuestMode() == false))
	{
		shared_ptr<IdeAccount> account = loggedUser->getAccount();
		if(account->hasLanguage())
		{
			//return getSkin()->getText(account->getLanguage(),name);
			return LanguageManager::instance()->getText(account->getLanguage(),name);
		}
	}

	return PageBase::getText(name);
}
*/
shared_ptr<OMLContext> IPortalPage::parseOmlEx(const String &text, bool enableWarning, bool full, bool secure, bool row, OMLRenderMode mode, const String &refID, const String &fullUrl)
{
	return OMLManager::instance()->parse(text, get_this_ptr<IPortalPage>(), enableWarning, full, secure, row, mode, refID, fullUrl);
}

void IPortalPage::initStylesheet(shared_ptr<XMLStylesheet> stylesheet)
{
	PageBase::initStylesheet(stylesheet);

	// Registra la funzione reference-date
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::ReferenceDate(get_this_ptr<IPortalPage>())));
}

// CLODOURGENT serve riabilitare questo?
/*
bool IPortalPage::restoreSession(shared_ptr<IdeSession> ideSession)
{
	if(loggedUser == null)
		return false;

	OS_ASSERT(loggedUser->isLogged() == false);

	// Ottiene il portale di riferimento
	shared_ptr<Portal> portal = loggedUser->getPortal();
	if(portal == null)
		return false;

	// Ottiene dai cookies l'ultimo account usato
	std::string id = getRequest()->getCookie(getAccountCookie(portal).to_ascii());
	if(loggedUser->loginAutomatic(getDatabase(), id))
		return true;

	// Tenta il login anonimo
	if(loggedUser->loginGuest(get_this_ptr<const IPortalPage>(), getDatabase()))
		return true;

	return false;
}
*/

bool IPortalPage::isMcpModeRequired() const
{
	return false;
}

bool IPortalPage::isIsisAllowed() const
{
	return true;
}

String IPortalPage::getBaseTitle()
{
	return m_portal != null ? m_portal->getName() : String::EMPTY;
}

void IPortalPage::onRenderDocument(shared_ptr<XMLNode> nodeRoot)
{
	PageBase::onRenderDocument(nodeRoot);

	nodeRoot->setAttributeString(_S("portal_name"), getPortal()->getName());
	nodeRoot->setAttributeString(_S("assistant_href"), getPortal()->getLink("assistant2"));	

	bool skipDefaultHeader = (getPortal()->getOptionsShared()->getLayoutHeader() == false);			
	nodeRoot->setAttributeBool(_S("skip_default_header"), skipDefaultHeader);

	shared_ptr<XMLNode> nodeUser = nodeRoot->addChild(_S("user"));
	shared_ptr<XMLNode> userActions = nodeUser->addChild(_S("actions"));

	/*
	shared_ptr<XMLNode> nodeUserActionAssistant = userActions->addChild(_S("action"));
	nodeUserActionAssistant->setAttributeString(_S("type"), _S("portal"));
	nodeUserActionAssistant->setAttributeString(_S("title"), getText(_S("portal.actions.assistant")));
	nodeUserActionAssistant->setAttributeString(_S("href"), HtmlScript::linkCommand(_S("Osiris.Assistant.toggle();")));
	*/
		
	if(getSessionAccount()->isPortalGuest(getDatabase()) == false)
	{
		shared_ptr<ObjectsUser> user = getSessionAccount()->getUser(getDatabase());
		OS_ASSERT(user != null);

		shared_ptr<XMLPortalExporter> userExporter(OS_NEW XMLPortalExporter(nodeUser, get_this_ptr<IPortalPage>(), XMLPortalExporter::emFull));
		user->exportXML(userExporter);

		//nodeUser->setAttributeString(_S("name"), user->name);
		//nodeUser->setAttributeString(_S("href"), getPortal()->getUserLink(user->id));

		shared_ptr<XMLNode> nodeUserActionUser = userActions->addChild(_S("action"));
		nodeUserActionUser->setAttributeString(_S("name"), _S("user"));
		nodeUserActionUser->setAttributeString(_S("title"), user->name);
		nodeUserActionUser->setAttributeString(_S("href"), getPortal()->getUserLink(user->id));
		nodeUserActionUser->setAttributeString(_S("accesskey"), _S("u"));

		shared_ptr<XMLNode> nodeUserActionAccount = userActions->addChild(_S("action"));
		nodeUserActionAccount->setAttributeString(_S("type"), _S("user"));
		nodeUserActionAccount->setAttributeString(_S("name"), _S("account"));
		//nodeUserActionAccount->setAttributeString(_S("title"), getText(_S("portal.actions.account")));
		nodeUserActionAccount->setAttributeString(_S("href"), getPortal()->getLink("account"));
		nodeUserActionAccount->setAttributeString(_S("accesskey"), _S("p"));

		shared_ptr<XMLNode> nodeUserActionMessenger = userActions->addChild(_S("action"));
		nodeUserActionMessenger->setAttributeString(_S("type"), _S("user"));
		// Calcola i nuovi messaggi privati dell'utente
		uint32 newMessages = getPortal()->getMessenger()->getNewMessagesCount(getDatabase(), getSessionAccount());
		nodeUserActionMessenger->setAttributeString(_S("name"), _S("messenger"));
		//nodeUserActionMessenger->setAttributeString(_S("title"), String::format(getText(_S("messenger.new_messages")).c_str(), newMessages));
		ordered_map<std::wstring, std::wstring> accountInboxParams;
		accountInboxParams.set(ide::portal::Account::ACTION, ide::portal::Account::getAction(ide::portal::Account::acViewInbox).to_wide());
		nodeUserActionMessenger->setAttributeString(_S("href"), getPortal()->getLink("account", accountInboxParams));
		nodeUserActionMessenger->setAttributeString(_S("accesskey"), _S("m"));

		nodeUser->setAttributeUint32(_S("new_messages"), newMessages);
		nodeUser->setAttributeString(_S("new_messages_tooltip"), String::format(getText(_S("messenger.new_messages")).c_str(), newMessages));

		// Discussioni ha cui ha partecipato l'utente
		shared_ptr<XMLNode> nodeUserActionMyPosts = userActions->addChild(_S("action"));
		nodeUserActionMyPosts->setAttributeString(_S("type"), _S("user"));
		nodeUserActionMyPosts->setAttributeString(_S("name"), _S("latest_user_discussions"));
		//nodeUserActionMyPosts->setAttributeString(_S("title"), getText(_S("portal.actions.myposts")));
		nodeUserActionMyPosts->setAttributeString(_S("href"), getPortal()->getLink(OS_IDE_PAGE_LATEST_USER_DISCUSSIONS));

		

	}
	else if(getRequestSource() == rsOsiris)
	{
		if(getSessionAccount()->isLogged())
		{
			if(getSessionAccount()->getSessionPortal(getPortal())->getGuest())
			{	

				if(getSessionAccount()->getUser(getDatabase()) != null)
				{
					shared_ptr<XMLNode> nodeActionLogin = userActions->addChild(_S("action"));
					nodeActionLogin->setAttributeString(_S("name"), _S("login"));
					nodeActionLogin->setAttributeString(_S("href"), getEventCommand(EVENT_ONPORTALLOGIN));
					nodeActionLogin->setAttributeString(_S("accesskey"), _S("l"));
				}
				else
				{
					if(getPortal()->getOptionsShared()->getAuthorsReputationThreshold() <= rtNotNegative)
					{
						shared_ptr<XMLNode> nodeActionRegister = userActions->addChild(_S("action"));
						nodeActionRegister->setAttributeString(_S("name"), _S("register"));
						nodeActionRegister->setAttributeString(_S("href"), getEventCommand(EVENT_ONPORTALLOGIN));
						nodeActionRegister->setAttributeString(_S("accesskey"), _S("l"));
					}
				}				
			}
			else
			{
				shared_ptr<XMLNode> nodeActionLogout = userActions->addChild(_S("action"));
				nodeActionLogout->setAttributeString(_S("name"), _S("logout"));
				//nodeActionLogout->setAttributeString(_S("title"), getText(_S("loggeduser.actions.logout")));
				nodeActionLogout->setAttributeString(_S("href"), getEventCommand(EVENT_ONPORTALLOGOUT));
				nodeActionLogout->setAttributeString(_S("accesskey"), _S("l"));

				// TODO: DEREGISTER. Set a "deregistered" bool at user object.
			}
		}

		// Commentato per Razor, il giro no-mcp & login/logout è da rifare/ricontrollare.
		/*
		if(getMcpMode())
		{
			shared_ptr<XMLNode> actionLogin = userActions->addChild(_S("action"));
			actionLogin->setAttributeString(_S("name"), _S("login"));
			//actionLogin->setAttributeString(_S("title"), getText(_S("loggeduser.actions.login")));
			ordered_map<std::wstring, std::wstring> params;
			params.set(OS_URL_PARAM_PORTAL, getPortal()->getPortalID().toWide());
			params.set(OS_URL_PARAM_USER, getPortal()->getUserID().toWide());
			actionLogin->setAttributeString(_S("href"), PortalsSystem::instance()->getMainLink("accounts", params));
		}
		else
		{
			if(getPortal()->getOptions()->getLoginEnabled())
			{
				shared_ptr<XMLNode> actionLogin = userActions->addChild(_S("action"));
				actionLogin->setAttributeString(_S("name"), _S("login"));
				//actionLogin->setAttributeString(_S("title"), getText(_S("loggeduser.actions.login")));
				actionLogin->setAttributeString(_S("href"), getPortal()->getLink("login"));
			
				if(getPortal()->getOptions()->getRegisterEnabled())
				{
					shared_ptr<XMLNode> actionRegister = userActions->addChild(_S("action"));
					actionRegister->setAttributeString(_S("name"), _S("register"));
					//actionRegister->setAttributeString(_S("title"), getText(_S("loggeduser.actions.register")));
					actionRegister->setAttributeString(_S("href"), getPortal()->getLink("register"));
				}
			}
		}
		*/
	}
	else
	{		
	}
}


void IPortalPage::onRenderInformations(shared_ptr<XMLNode> node)
{
	node->setAttributeBool(_S("guest"), getSessionAccount()->isPortalGuest(getDatabase()));	
	node->setAttributeBool(_S("mcp"), getMcpMode());

	if(getRequestSource() == rsIsis)
		node->setAttributeBool(_S("isis"), true);
	
	

	node->setAttributeString(_S("stability_status"), getStabilityStatus());

	PageBase::onRenderInformations(node);
}

void IPortalPage::onRenderActions(shared_ptr<XMLNode> nodeActions) // TOCLEAN, abolibile
{
	PageBase::onRenderActions(nodeActions);

	/*
	shared_ptr<XMLNode> nodePortalActionView = nodeActions->addChild(_S("action"));
	nodePortalActionView->setAttributeString(_S("type"), _S("portal"));
	nodePortalActionView->setAttributeString(_S("title"), getText(_S("portal.pages.home.title")));
	nodePortalActionView->setAttributeString(_S("href"), getPortal()->getLink(ide::portal::ppView));

	shared_ptr<XMLNode> nodePortalActionSearch = nodeActions->addChild(_S("action"));
	nodePortalActionSearch->setAttributeString(_S("type"), _S("portal"));
	nodePortalActionSearch->setAttributeString(_S("title"), getText(_S("portal.pages.search.title")));
	nodePortalActionSearch->setAttributeString(_S("href"), getPortal()->getLink(OS_IDE_PAGE_SEARCH));

	shared_ptr<XMLNode> nodePortalActionUsers = nodeActions->addChild(_S("action"));
	nodePortalActionUsers->setAttributeString(_S("type"), _S("portal"));
	nodePortalActionUsers->setAttributeString(_S("title"), getText(_S("portal.pages.users.title")));
	nodePortalActionUsers->setAttributeString(_S("href"), getPortal()->getLink(ide::portal::ppUsers));

#ifdef OS_DEBUG
	if(getLoggedUser()->getReadOnlyMode() == false)
	{
		shared_ptr<XMLNode> nodePortalActionStress = nodeActions->addChild(_S("action"));
		nodePortalActionStress->setAttributeString(_S("type"), _S("portal"));
		nodePortalActionStress->setAttributeString(_S("title"), getText(_S("portal.actions.stress")));
		nodePortalActionStress->setAttributeString(_S("href"), getPortal()->getLink(_S("stress/stress")));
	}
#endif // OS_DEBUG

	if(getLoggedUser()->getGuestMode() == false)
	{

	}

	shared_ptr<XMLNode> nodePortalActionChanges = nodeActions->addChild(_S("action"));
	nodePortalActionChanges->setAttributeString(_S("type"), _S("portal"));
	nodePortalActionChanges->setAttributeString(_S("title"), getText(_S("portal.pages.changes")));
	nodePortalActionChanges->setAttributeString(_S("href"), getPortal()->getLink(OS_IDE_PAGE_CHANGES));

	shared_ptr<XMLNode> nodePortalActionLatestDiscussions = nodeActions->addChild(_S("action"));
	nodePortalActionLatestDiscussions->setAttributeString(_S("type"), _S("portal"));
	nodePortalActionLatestDiscussions->setAttributeString(_S("title"), getText(_S("portal.pages.latest_discussions")));
	nodePortalActionLatestDiscussions->setAttributeString(_S("href"), getPortal()->getLink(OS_IDE_PAGE_LATEST_DISCUSSIONS));
	*/
}

void IPortalPage::onInit()
{
	PageBase::onInit();

	m_portal = getPortalFromUrl();
	if(m_portal == null)
	{
		redirect(PortalsSystem::instance()->getMainLink(OS_IDE_PAGE_OSIRIS));
		return;
	}

	if(m_requestSource != rsOsiris)
	{
		// Localizza l'endpoint
		m_isisEndpoint = m_portal->getOptions()->findIsisEndpoint(getRequestIsisUrl().to_ascii());
		OS_EXCEPT_IF(m_isisEndpoint == null, "Invalid Isis endpoint");
	}
	else
	{
		if( (getMcpMode() == false) && (getPortal()->getOptions()->getPublicEnabled() == false) )
		{
			reportError(httpStatusForbidden);
			return;
		}
	}

	m_portal->getSnapshotManager()->updateLastActionTick();


	// Se esiste l'utente, allora lo loggo nel portale in automatico.
	if(getSessionAccount()->isLogged() == true)
	{	
		shared_ptr<ObjectsUser> user = objects_user_cast(getPortal()->getObject(getDatabase(), getSessionAccount()->getUserID()));
		if(user != null)
		{
			LanguageResult result = getSessionAccount()->portalLogin(getDatabase());
		}
	}

	/*
	shared_ptr<IdeSession> ideSession = getSessionAccount();
	if(ideSession->isLogged())
		return;
	*/

	/* // CLODOURGENT vedi l'altro commento in restoreSession
	if(restoreSession(loggedUser) == false)
	{
		if(m_requestSource == rsOsiris)
		{
			// Se il ripristino della sessione non va a buon fine effettua il redirect sulla pagina degli accounts del portale
			if(getAjax() == false)
				redirect(PortalsSystem::instance()->getAccountsLink(m_portal));
		}
		else
		{
			// Restituisci un errore se è un isis.
			OS_EXCEPTION("Cannot enter as guest.");
		}
	}
	*/

	if(getAjax() == false)
	{
		//if(getPageMode() == httpPageModeFull) // RC1
		if(getPageMode() != httpPageModeAjax) // RC2
		{
			// Controlla se effettuare il caricamento delle istanze
			if(getLoadInstances())
				// Carica le istanze
				loadInstances(getTargetObject());
		}
	}
}

void IPortalPage::onLoad()
{
	PageBase::onLoad();

	if(getAjax() == false)
	{
		// Inizializza il titolo e descrizione
		if(getPageName().empty() == false)
		{
			setPageTitle(getText(getPageName() + _S(".title")));
			setPageDescription(getText(getPageName() + _S(".description")));
		}

		/*
		// Razor: primo test giri
		if(getPortal()->getSnapshotManager()->getOptions()->getDescription().empty() == false)
			addMetaTag(HtmlMetaTag::createByName(_S("description"), getPortal()->getSnapshotManager()->getOptions()->getDescription()));
		*/

		if(getPageMode() == httpPageModeFull)
		{
			if(getSessionAccount()->isPortalGuest(getDatabase()) == false)			
			{
				shared_ptr<Portal> portal = getPortal();
				if(portal != null)
				{
					uint32 pendingPrivateMessage = portal->getMessenger()->getPendingMessages(getDatabase(), getSessionAccount(), true);
					if(pendingPrivateMessage > 0)
					{
						if(pendingPrivateMessage == 1)
							showMessage(getText(_S("messenger.alert.new_message")));
						else
							showMessage(String::format(getText(_S("messenger.alert.new_messages")).c_str(), pendingPrivateMessage));
					}
				}
			}

			m_assistant.reset(OS_NEW IdeAssistant());
			getTemplate()->addChildParam(m_assistant, _S("assistant"));

			String customCss = getPortal()->getOptionsShared()->getLayoutCss();			
			if(customCss.empty() == false)
			{
				customCss = OMLManager::instance()->parseCss(customCss, get_this_ptr<IPortalPage>(), false, true);
				customCss = _S("<style type=\"text/css\" rel=\"stylesheet/less\">") + customCss + _S("</style>");

				setAdditionalHead(customCss);
			}
		}

		/* 0.14 - Deprecated. Injected by Isis directly.
		// Inizializza le aree per Isis
		if(getRequestSource() != rsOsiris)
		{
			String htmlSystemBar = getIsisEndpoint()->getIsisOptions().getOption(_S("isis.output.area.systembar"));
			getTemplate()->addChildParam(shared_ptr<IHtmlControl>(OS_NEW HtmlLiteral(htmlSystemBar)), _S("isis_area_systembar"));
			String htmlFooter = getIsisEndpoint()->getIsisOptions().getOption(_S("isis.output.area.footer"));
			getTemplate()->addChildParam(shared_ptr<IHtmlControl>(OS_NEW HtmlLiteral(htmlFooter)), _S("isis_area_footer"));
		}
		*/
	}
}

void IPortalPage::onPreRender()
{
	

	PageBase::onPreRender();
}

void IPortalPage::onPortalLogin()
{
	if(getSessionAccount()->isLogged() == false)
		return;

	LanguageResult result = getSessionAccount()->portalLogin(getDatabase());
	if(result.empty() == false)
		showError(getText(result));
}

void IPortalPage::onPortalLogout()
{
	if(getSessionAccount()->isLogged() == false)
		return;
	getSessionAccount()->portalLogout(getPortal());
}

void IPortalPage::renderLinktags(HtmlWriter &writer)
{
	std::string style = "";
	style += "<style id=\"lesslib\" rel=\"stylesheet/less\">";
	style += "@import url(\"/htdocs/css/default.less\");";	
	//style += "@import url(\"/" + getSkin()->getID().getString() + "/css/skin.less\");";		
	//style += "@import url(\"" + getSkin()->getResourceUrl(_S("css/skin.less")).to_ascii() + "\");";		
	style += "@import url(\"" + getSkin()->getResourceUrl(_S("css/options.less")).to_ascii() + "\");";		
	style += "@import url(\"" + getSkin()->getResourceUrl(_S("css/custom.less")).to_ascii() + "\");";		
	style += getPortal()->getOptionsShared()->getLayoutSkinParams().to_ascii();
	style += "</style>";
	writer.writeLine(style);
		
	PageBase::renderLinktags(writer);
}

void IPortalPage::redirect(const std::string &url)		// Necessario per non "oscurare" il metodo redirect della base
{
	PageBase::redirect(url);
}

bool IPortalPage::transmit()
{
	bool result = PageBase::transmit();

	// Vedere IPortalPage::onInit
	shared_ptr<Portal> portal = getPortal();
	if(portal != null)
		portal->getSnapshotManager()->updateLastActionTick();

	return result;
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
