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
#include "iidepage.h"

#include "boost/bind.hpp"
#include "convert.h"
#include "conversions.h"
#include "css.h"
#include "dataaccount.h"
#include "iportaldatabase.h"
#include "isissystem.h"
#include "omlcontext.h"
#include "omlmanager.h"
#include "ideaccount.h"
#include "ideaccountsmanager.h"
#include "idefunctionsfunctions.h"
#include "idepathway.h"
#include "idesession.h"
#include "ideskin.h"
#include "idesystem.h"
#include "idexslresolver.h"
#include "isis.h"
#include "languageculture.h"
#include "languagemanager.h"
#include "log.h"
#include "objectssystem.h"
#include "options.h"
#include "platformmanager.h"
#include "portalsaccount.h"
#include "idesession.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "portalstransaction.h"
#include "engine.h"
#include "htmlparser.h"
#include "htmlwriter.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpsession.h"
#include "httpsessionstate.h"
#include "htmlcontrols.h"
#include "htmldiv.h"
#include "htmlimage.h"
#include "htmlliteral.h"
#include "htmlpanel.h"
#include "htmlpopup.h"
#include "htmlspan.h"
#include "htmltext.h"
#include "htmlxslcontrol.h"
#include "idepages.h"
#include "regexmanager.h"
#include "timerperformance.h"
#include "xmlstylesheet.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IPage::EVENT_ONLOGOUT = _S("onLogout");

//////////////////////////////////////////////////////////////////////

IPage::Message::Message(Message::MessageType type, const String &description, const String &title)
{
	m_type = type;
	m_title = title;
	m_description = description;
}

IPage::Message::~Message()
{

}

void IPage::Message::exportXML(shared_ptr<IPage> page, shared_ptr<XMLNode> node) const
{
	String type;
	switch(m_type)
	{
	case mtInfo:		type = _S("info");
						break;

	case mtWarning:		type = _S("warning");
						break;

	case mtError:		type = _S("error");
						break;
	}

	node->setAttributeString(_S("type"), type);
	node->setAttributeString(_S("title"), m_title);
	node->setAttributeString(_S("description"), m_description);
}

//////////////////////////////////////////////////////////////////////

IPage::IPage(shared_ptr<HttpSession> session) : PageBase(session),
												m_pathway(OS_NEW IdePathway()),
												m_template(OS_NEW HtmlXSLControl())
{
	getEvents()->get(EVENT_ONLOGOUT)->connect(boost::bind(&IPage::onLogout, this));

	String mcpPassword = Options::instance()->getOption<String>(Options::web_options::mcp_password);
	if(mcpPassword == String::EMPTY)
	{
		m_mcpMode = true;
	}
	else if( (session->getState()) && (session->getState()->get(_S("mcp.password")) == mcpPassword) )
	{
		m_mcpMode = true;
	}
	else
	{
		m_mcpMode = false;
	}
	
	if(getRequest()->hasUrlParam(_W("page-mode")))
	{
		std::wstring mode = getRequest()->getUrlParam(_W("page-mode"));
		if(mode == _W("content"))
			setPageMode(httpPageModeContent);
		else if(mode == _W("ajax"))
			setPageMode(httpPageModeAjax);
	}
}

IPage::~IPage()
{
	closeDatabase(false);
}

/*
shared_ptr<IPortalDatabase> IPage::getDatabaseX() const
{
	shared_ptr<Portal> portal = getPortalFromUrl();
	return portal != nullptr ? getActiveDatabaseX(portal) : nullptr;
}
*/

String IPage::getLanguage() const
{
	shared_ptr<IdeSession> ideSession = getSessionAccount();
	if( (ideSession != nullptr) && ideSession->isLogged() )
	{
		return ideSession->getLanguage();
	}
	
	return Options::instance()->getDefaultLanguage();
}

int32 IPage::getTimeOffset() const
{
	shared_ptr<IdeSession> ideSession = getSessionAccount();
	if( (ideSession != nullptr) && ideSession->isLogged() )
	{
		int time_offset = ideSession->getAccount()->getAccount()->getTimeOffset();
		if(time_offset != OS_TIME_DETECTION_SYSTEM)
			return time_offset;		
	}

	return Engine::instance()->getTimeOffset();	
}

int32 IPage::getTimeDST() const
{
	shared_ptr<IdeSession> ideSession = getSessionAccount();
	if( (ideSession != nullptr) && ideSession->isLogged() )
	{
		int time_dst = ideSession->getAccount()->getAccount()->getTimeDST();
		if(time_dst != OS_TIME_DETECTION_SYSTEM)
			return time_dst;
	}

	return Engine::instance()->getTimeDST();	
}

String IPage::getText(const String &name) const
{
	//return getSkin()->getText(getLanguage(), name);
	return LanguageManager::instance()->getText(getLanguage(),name);
}

String IPage::getText(const LanguageResult &result) const
{
	//return getSkin()->getText(getLanguage(), name);
	return LanguageManager::instance()->getText(getLanguage(), result);
}

String IPage::formatDate(const DateTime &dt, const DateTime::DateTimePattern &pattern) const
{
	shared_ptr<LanguageCulture> culture = LanguageManager::instance()->getCulture(getLanguage());
	OS_EXCEPT_IF(culture == nullptr, "Invalid culture");

	return culture->formatDate(dt, pattern, getTimeOffset(), getTimeDST());
}

String IPage::formatDate(const DateTime &dt, const String &pattern) const
{
	shared_ptr<LanguageCulture> culture = LanguageManager::instance()->getCulture(getLanguage());
	OS_EXCEPT_IF(culture == nullptr, "Invalid culture");

	return culture->formatDate(dt, pattern, getTimeOffset(), getTimeDST());
}

String IPage::parseOml(const String &text, bool full, bool secure, bool row, OMLRenderMode mode, const String &refID, const String &fullUrl)
{
	return parseOmlEx(text, false, full, secure, row, mode, refID, fullUrl)->getOutput();
}

shared_ptr<OMLContext> IPage::parseOmlEx(const String &text, bool enableWarning, bool full, bool secure, bool row, OMLRenderMode mode, const String &refID, const String &fullUrl)
{
	return OMLManager::instance()->parse(text, get_this_ptr<IPage>(), enableWarning, full, secure, row, mode, refID, fullUrl);
}

String IPage::helpLink(const String &id) const
{
	String eId = HtmlParser::instance()->encodeEx(id);

	bool local = false;
	if(local)
	{
		// TODO: Fare che se il portale ufficiale è già installato, link a quello.
		return String::EMPTY;
	}

	return Options::instance()->getIsisLink(OS_ISIS_PAGE_LINK, (_S("help=") + eId).to_ascii());
}

shared_ptr<IdePathway> IPage::getPathway() const
{
	return m_pathway;
}

shared_ptr<IdeSkin> IPage::getSkin() const
{
	//return IdeSystem::instance()->getDefaultSkin();
	shared_ptr<IdeSession> ideSession = getSessionAccount();
	if(ideSession != nullptr && ideSession->isLogged())
	{
		shared_ptr<IdeSkin> skin;
		skin = ideSession->getCurrentSkin();

		if(skin != nullptr && skin->isValid())
			return skin;
	}

	return IdeSystem::instance()->getDefaultSkin();
}

shared_ptr<Portal> IPage::getPortalFromUrl() const
{
	String pov = static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_PORTAL)).to_ascii();
	return PortalsSystem::instance()->getPortalByFullPov(pov);
	//return PortalsSystem::instance()->getPortalByPov(static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_PORTAL)).to_ascii());
	//return PortalsSystem::instance()->getPortal(static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_PORTAL)).to_ascii(), static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_USER)).to_ascii());
}

String IPage::getPageName() const
{
	return String::EMPTY;
}

void IPage::onPathway() const
{	
}

String IPage::getPagePathway() const
{
	return getRequest()->getRawUrl();
}

shared_ptr<XMLStylesheet> IPage::createStylesheet()
{
	// Crea il template
	shared_ptr<XMLStylesheet> stylesheet(OS_NEW XMLStylesheet());
	// Inizializza le funzioni del template
	initStylesheet(stylesheet);

	return stylesheet;
}

shared_ptr<XMLStylesheet> IPage::loadStylesheet(const String &pathname)
{
	shared_ptr<XMLStylesheet> stylesheet = createStylesheet();
	stylesheet->parseFile(pathname);

	return stylesheet;
}

void IPage::initStylesheet(shared_ptr<XMLStylesheet> stylesheet)
{
	stylesheet->getEventError().connect(boost::bind(&IPage::onXSLError, this, _1));

	// Inizializza il risolutore dei path
	stylesheet->setResolver(shared_ptr<IXMLResolver>(OS_NEW IdeXSLResolver(getSkin())));

	shared_ptr<IPage> this_ptr = get_this_ptr<IPage>();

	// Registra la funzione compare
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::CompareDates(this_ptr)));
	// Registra la funzione format-date
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::CurrentDateTime(this_ptr)));
	// Registra la funzione current-datetime
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::FormatDate(this_ptr)));
	// Registra la funzione short-date
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::ShortDate(this_ptr)));
	// Registra la funzione long-date
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::LongDate(this_ptr)));
	// Registra la funzione short-time
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::ShortTime(this_ptr)));
	// Registra la funzione long-time
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::LongTime(this_ptr)));
	// Registra la funzione short-datetime
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::ShortDateTime(this_ptr)));
	// Registra la funzione long-datetime
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::LongDateTime(this_ptr)));
	// Registra la funzione userfriendly-datetime
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::UserFriendlyDateTime(this_ptr)));
	// Registra la funzione rfc822-datetime
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::RFC822DateTime(this_ptr)));


	// Registra la funzione text
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::Text(this_ptr)));
	// Registra la funzione english-name
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::EnglishName(this_ptr)));
	// Registra la funzione native-name
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::NativeName(this_ptr)));

	// Registra la funzione format-size
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::FormatSize(this_ptr)));
	// Registra la funzione parse
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::Parse(this_ptr)));
	// Registra la funzione parse-reverse
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::ParseReverse(this_ptr)));	
	// Registra la funzione version-major
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::VersionMajor(this_ptr)));
	// Registra la funzione version-minor
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::VersionMinor(this_ptr)));
	// Registra la funzione get-option
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::GetOption(this_ptr)));
	// Registra la funzione html-encode
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::HtmlEncode(this_ptr)));
	// Registra la funzione html-decode
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::HtmlDecode(this_ptr)));
	// Registra la funzione js-encode
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::JSEncode(this_ptr)));
	// Registra la funzione url-encode
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::UrlEncode(this_ptr)));
	// Registra la funzione url-decode
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::UrlDecode(this_ptr)));
	// Registra la funzione help-link
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::HelpLink(this_ptr)));
	// Registra la funzione find-resource
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::ResourceUrl(this_ptr)));
	// Registra la funzione replace
	stylesheet->registerFunction(shared_ptr<IXMLFunction>(OS_NEW ide::functions::Replace(this_ptr)));
}

bool IPage::loginAccountWithName(const String &login, const String &password, bool save_password)
{
	shared_ptr<IdeAccount> account = IdeAccountsManager::instance()->getByName(login);
	if(account == nullptr)
		return false;

	return loginAccountWithID(account->getID(), password, save_password);
}

bool IPage::loginAccountWithID(const String &accountID, const String &password, bool save_password)
{	
	// Ottiene la sessione sul portale specificato
	shared_ptr<IdeSession> session = getSessionAccount();
	if(session == nullptr)
		return false;

	// Effettua il login e restituisce il risultato dell'operazione
	if(session->login(accountID, password, save_password) == false)
		return false;

	// Salva il nome dell'ultimo account usato
	getResponse()->setCookie("osiris.account", session->getAccount()->getID().to_ascii());

	return true;
}

void IPage::ensureAccountSession()
{
	String sessionID;
	if(getRequestSource() != rsOsiris)
	{
		// URGENT: Qui ci vorrebbe un SHA dell'ip di getRequest()->getIP()?
		sessionID = _S("9DD9CC18C86879BB9548B251543B974232216942");
	}
	else
		sessionID = getRequest()->getCookie("osiris.session");

	// Ottiene la sessione attiva
	if(sessionID.empty() == false)
		m_session = IdeSystem::instance()->getSession(sessionID);

	// Controlla se l'utente loggatto non esiste
	if(m_session == nullptr)
	{
		// Crea un nuovo utente
		m_session = IdeSystem::instance()->createSession();

		// Se  una richiesta da un Isis,  inutile settare questo cookie.
		if(getRequestSource() == rsOsiris)
			getResponse()->setCookie("osiris.session", m_session->getSessionID().to_ascii());

		// Se c'è il cookie dell'ultimo account usato, e posso loggarlo automaticamente, login.
		String lastAccountUsed = getRequest()->getCookie("osiris.account");
		if(lastAccountUsed.empty() == false)
		{
			shared_ptr<IdeAccount> account = IdeAccountsManager::instance()->getByID(lastAccountUsed);
			if( (account != nullptr) && (account->getAccount()->hasAutologin()) )
			{
				loginAccountWithID(account->getID(), account->getRealPassword());
			}
		}
	}
}

bool IPage::isMcpModeRequired() const
{
	return true;
}

bool IPage::isIsisAllowed() const
{
	return false;
}

void IPage::onRenderDocument(shared_ptr<XMLNode> nodeRoot)
{
	onRenderInformations(nodeRoot);
	onRenderPathWay(nodeRoot->addChild(_S("pathway")));
	onRenderMessages(nodeRoot->addChild(_S("messages")));
	onRenderActions(nodeRoot->addChild(_S("actions")));
}

void IPage::onRenderInformations(shared_ptr<XMLNode> node)
{
	node->setAttributeString(_S("page_mode"), getPageMode() == httpPageModeFull ? _S("full") : _S("content"));

	/* TOCLEAN
	node->setAttributeString(_S("href_home"), Options::instance()->getIsisLink(OS_ISIS_PAGE_LINK, "id=home"));
	node->setAttributeString(_S("href_docs"), Options::instance()->getIsisLink(OS_ISIS_PAGE_LINK, "id=docs"));
	node->setAttributeString(_S("href_forum"), Options::instance()->getIsisLink(OS_ISIS_PAGE_LINK, "id=forum"));

	node->setAttributeString(_S("version"), Engine::getVersionName(true));
	*/

	if(getSessionAccount()->isLogged())
	{
		node->setAttributeBool(_S("logged"), true);
		node->setAttributeString(_S("account_name"), getSessionAccount()->getAccount()->getName());
	}
	else
	{
		node->setAttributeBool(_S("logged"), false);
	}

	node->setAttributeInt32(_S("isis_status"), IsisSystem::instance()->getStatus());
}

void IPage::onRenderMessages(shared_ptr<XMLNode> node)
{
	for(Messages::const_iterator i = m_messages.begin(); i != m_messages.end(); ++i)
	{
		message_ptr message = *i;
		message->exportXML(get_this_ptr<IPage>(), node->addChild(_S("message")));
	}
}

void IPage::onRenderActions(shared_ptr<XMLNode> node)
{
	if(getRequestSource() == rsOsiris)
	{
		shared_ptr<XMLNode> nodeMainActionHome = node->addChild(_S("action"));
		nodeMainActionHome->setAttributeString(_S("type"), _S("main"));
		nodeMainActionHome->setAttributeString(_S("title"), getText(_S("main.actions.home")));
		nodeMainActionHome->setAttributeString(_S("href"), PortalsSystem::instance()->getMainLink(OS_IDE_PAGE_OSIRIS));

		if(getSessionAccount()->isLogged())
		{
			shared_ptr<XMLNode> nodeActionLogout = node->addChild(_S("action"));
			nodeActionLogout->setAttributeString(_S("name"), _S("logout"));
			//nodeActionLogout->setAttributeString(_S("title"), getText(_S("loggeduser.actions.logout")));
			nodeActionLogout->setAttributeString(_S("href"), getEventCommand(EVENT_ONLOGOUT));
			nodeActionLogout->setAttributeString(_S("accesskey"), _S("l"));
		}
		else
		{
			shared_ptr<XMLNode> nodeActionLogin = node->addChild(_S("action"));
			nodeActionLogin->setAttributeString(_S("name"), _S("login"));
			//nodeActionLogin->setAttributeString(_S("title"), getText(_S("loggeduser.actions.logout")));
			nodeActionLogin->setAttributeString(_S("href"), PortalsSystem::instance()->getMainLink("accounts"));
			nodeActionLogin->setAttributeString(_S("accesskey"), _S("l"));
		}
	}
}

void IPage::onRenderPathWay(shared_ptr<XMLNode> node)
{
	m_pathway->exportXML(node);
}

void IPage::onXSLError(const String &description)
{
	showError(description, _S("XSL Error"));
}

void IPage::onLoadResources()
{
	// Ottiene la skin corrente
	shared_ptr<IdeSkin> skin = getSkin();

	//addCss(skin->getResourceUrl(_S("css/config.less")));
	/*
	addCss(_S("/") + skin->getID().toUTF16() + _S("/css/skin.less"));
	*/
	
	/*
	shared_ptr<Portal> portal = getPortalFromUrl();
	if(portal != nullptr)
		addCss(portal->getLink("lesslib"));
	else
		addCss(PortalsSystem::instance()->getMainLink("lesslib"));	
	*/
	
	addCss(skin->getResourceUrl(_S("css/osiris.less")));
	
	addJavascript(skin->getResourceUrl(_S("js/jquery/jquery.js")));	
	addJavascript(skin->getResourceUrl(_S("js/jquery/jquery.ui.js")));	
	addJavascript(skin->getResourceUrl(_S("js/libraries.js"))); // Other libraries, direct/simple code.

	addJavascript(skin->getResourceUrl(_S("js/less.js")));	
	
	addJavascript(skin->getResourceUrl(_S("js/osiris.js")));

	// Warning: after "osiris.js" because contain initialization parameters that need other functions.
	addJavascript(skin->getResourceUrl(_S("js/osirislibraries.js"))); // Other libraries, direct/simple code.

	

	if(skin != nullptr)
	{
		// Applica la skin alla pagina
		skin->apply(get_this_ptr<IPage>());
	}

	// Platform-specific CSS
	// Questa parte  temporanea, ma almeno posso "calibrare" i css quando mi capita.
	// La soluzione corretta e definitiva  che si dovrebbe poter specificare delle skins per ogni platform,
	// non basta variare qualche css, a farlo bene variano anche gli XSLT.
	String userAgent = getRequest()->getUserAgent();
	if(regex_match(userAgent, RegexManager::instance()->getRegex(_S(".*Windows CE.*"))))
		addCss(skin->getResourceUrl(_S("css/platform_wince.css")));
	else if(regex_match(userAgent, RegexManager::instance()->getRegex(_S(".*Nintendo Wii.*"))))
		addCss(skin->getResourceUrl(_S("css/platform_wii.css")));
	else if(regex_match(userAgent, RegexManager::instance()->getRegex(_S(".*PlayStation Portable.*"))))
		addCss(skin->getResourceUrl(_S("css/platform_psp.css")));
	else if(regex_match(userAgent, RegexManager::instance()->getRegex(_S(".*iPhone.*"))))
		addCss(skin->getResourceUrl(_S("css/platform_iPhone.css")));
	else if(regex_match(userAgent, RegexManager::instance()->getRegex(_S(".*PLAYSTATION 3.*"))))
		addCss(skin->getResourceUrl(_S("css/platform_ps3.css")));
}

void IPage::showMessage(const String &description, const String &title)
{
	m_messages.push_back(message_ptr(OS_NEW Message(Message::mtInfo, description, title.empty() ? getText("ide.messages.information") : title)));
	OS_LOG_NOTICE(description);
}

void IPage::showWarning(const String &description, const String &title)
{
	m_messages.push_back(message_ptr(OS_NEW Message(Message::mtWarning, description, title.empty() ? getText("ide.messages.warning") : title)));
	OS_LOG_WARNING(description);
}

void IPage::showError(const String &description, const String &title)
{
	m_messages.push_back(message_ptr(OS_NEW Message(Message::mtError, description, title.empty() ? getText("ide.messages.error") : title)));
	OS_LOG_ERROR(description);
}

shared_ptr<IHtmlControl> IPage::getArea(const PageArea &area)
{
	AreasMap::const_iterator i = m_areas.find(area);
	if(i != m_areas.end())
		return i->second;

	shared_ptr<IHtmlControl> areaControl = shared_ptr<IHtmlControl>(OS_NEW HtmlDiv());
	m_areas.insert(AreasMap::value_type(area, areaControl));

	String areaName;
	switch(area)
	{
	case pageAreaContent:		areaName = OS_PAGE_AREANAME_CONTENT;
								break;

	case pageAreaContentTop:	areaName = OS_PAGE_AREANAME_CONTENT_TOP;
								break;

	case pageAreaContentBottom:	areaName = OS_PAGE_AREANAME_CONTENT_BOTTOM;
								break;

	case pageAreaLeft:			areaName = OS_PAGE_AREANAME_LEFT;
								break;

	case pageAreaTop:			areaName = OS_PAGE_AREANAME_TOP;
								break;

	case pageAreaRight:			areaName = OS_PAGE_AREANAME_RIGHT;
								break;

	case pageAreaBottom:		areaName = OS_PAGE_AREANAME_BOTTOM;
								break;

	default:					OS_ASSERTFALSE();

								areaName = OS_PAGE_AREANAME_UNKNOWN;
								break;
	}

	OS_ASSERT(m_template != nullptr);
	m_template->addChildParam(areaControl, areaName);

	return areaControl;
}

DataItem IPage::getOption(const String &name) const
{
	return Options::instance()->getOptionValue(name);
}

void IPage::renderLinktags(HtmlWriter &writer)
{
	std::string style = "";
	style += "<style id=\"lesslib\" rel=\"stylesheet/less\">";
	style += "@import url(\"/htdocs/css/default.less\");";	
	//style += "@import url(\"/" + getSkin()->getID().getString() + "/css/skin.less\");";		
	style += "@import url(\"" + getSkin()->getResourceUrl(_S("css/options.less")).to_ascii() + "\");";		
	style += "@import url(\"" + getSkin()->getResourceUrl(_S("css/custom.less")).to_ascii() + "\");";		
	
	style += "</style>";
	writer.writeLine(style);
	
	PageBase::renderLinktags(writer);
}

void IPage::redirect(const std::string &url)
{
	// Chiudo la transazione del db, se aperta. E' qui xchè è inutile tenerla aperta durante la "transmit".
	closeDatabase();

    PageBase::redirect(url);
}

bool IPage::transmit()
{
	/*
	// Note: we don't call the base...
	if(ensureStatus(get_this_ptr<HtmlPage>(), csPreRender) == false)
		return false;
	*/
	ensureStatus(get_this_ptr<HtmlPage>(), csPreRender);

	if(getSession()->getProcessed())
		return true;

	if(getAjax() == false)
	{	
		if(IdeSystem::instance()->getEventBeforePageRender()->empty() == false)
		{
			IdeSystem::EventBeforePageRender e;
			e.setPage(get_this_ptr<IPage>());
			IdeSystem::instance()->getEventBeforePageRender()->fire(&e);		
		}
	}

	HtmlWriter writer;
	render(get_this_ptr<HtmlPage>(), writer);

	if(getSession()->getProcessed())
		return true;

	if(getAjax() == false)
	{
		if(IdeSystem::instance()->getEventAfterPageRender()->empty() == false)
		{
			IdeSystem::EventAfterPageRender e;
			e.setHtml(writer.getHtml());
			IdeSystem::instance()->getEventAfterPageRender()->fire(&e);
			writer.setHtml(e.getHtml());
		}
	}

	return onTransmit(writer);
}

void IPage::onInit()
{
	PageBase::onInit();

	if(getRequest()->hasCookie("isis_url"))
	{
		// Proxy version
		m_requestSource = rsIsis;
	}
	else
	{
		m_requestSource = rsOsiris;
	}

	ensureAccountSession();

	// Isis Mode Check
	if(getRequestSource() != rsOsiris)
	{
		if(isIsisAllowed() == false)
			OS_EXCEPTION("Invalid url request");

		m_mcpMode = false;
	}

	// Mcp Mode Check
	if( (getMcpMode() == false) && (isMcpModeRequired() == true) )
	{		
		String defaultPortalID = Options::instance()->getOption<String>(Options::web_options::default_portal).to_ascii();

		shared_ptr<Portal> defaultPortal = PortalsSystem::instance()->getPortalByFullPov(defaultPortalID);
		if(defaultPortal == nullptr)
		{
			//reportError(httpStatusForbidden);
			if(getAjax() == false)
				redirect(PortalsSystem::instance()->getMainLink("mcp"));

			// URGENT: E' normale che continua il processing della pagina? Non dovrebbe lanciare un'eccezione?
		}
		else
		{		
			if(getAjax() == false)
				redirect(defaultPortal->getLink("view"));

			//OS_ASSERT(false); // Check, qui non ci deve arrivare.
		}
	}

	if(getAjax() == false)
		getControls()->add(m_template);

	
}

void IPage::onLoad()
{
	PageBase::onLoad();

	if(getAjax() == false)
	{
		// Inizializza la favicon
		setFavicon(getSkin()->getFaviconUrl());

		// Inizializza il pathway
		onPathway();		

		// Inizializza il template
		String templatePath = getSkin()->getTemplatePath(_S("page.xsl"));
		m_template->setStylesheet(loadStylesheet(templatePath));
		m_template->setDocument(shared_ptr<XMLDocument>(OS_NEW XMLDocument()));

		// Inizializza le risorse
		onLoadResources();
	}
}

void IPage::onRender(HtmlWriter &writer)
{
	if(getAjax() == false)
		onRenderDocument(m_template->getDocument()->create("page"));

	// La parte di render dell'xml deve essere fatta prima della render dei figli, pertanto la onRender della base va chiamata successivamente
	PageBase::onRender(writer);

	// Chiudo la transazione del db, se aperta. E' qui xchè è inutile tenerla aperta durante la "transmit".
	closeDatabase();
}


void IPage::onLogout()
{
	shared_ptr<IdeSession> ideSession = getSessionAccount();
	if((ideSession != nullptr) && (ideSession->isLogged()))
	{
		// Effettua il logout
		ideSession->logout();
				
		getResponse()->removeCookie("osiris.account");
		/*
		shared_ptr<Portal> portal = getPortal();
		if(portal != nullptr)
		{
			// Rimuove il cookie dell'ultimo account usato in modo tale che il metodo restoreSession (se successivamente chiamato) non trovi il cookie
			

			redirect(portal->getLink("view"));
		}
		*/
	}
}

// URGENT: Temporanea, vedi commento nel .H
shared_ptr<IPortalDatabase> IPage::getDatabase() const
{
	if(m_databaseWork == nullptr)
	{
		shared_ptr<Portal> portal = getPortalFromUrl();
		if(portal == nullptr)
			return nullptr;
		m_databaseWork = portal->startTransaction(true); // CLODOURGENT, Perchè exclusive access?
		m_databaseWork->getDatabase()->beginTransaction();
	}
	shared_ptr<IPortalDatabase> database = m_databaseWork->getDatabase();
	return database;
}

// URGENT: Da sistemare
void IPage::closeDatabase(bool commit)
{
	if(m_databaseWork != nullptr)
	{
		shared_ptr<IPortalDatabase> database = m_databaseWork->getDatabase();
		if(database->inTransaction())
		{
			if(commit)
				database->commit();
			else
				database->rollback();
		}
		m_databaseWork = nullptr;
	}
}



//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
