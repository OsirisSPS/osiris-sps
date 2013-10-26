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

#ifndef _IDE_IPAGE_H
#define _IDE_IPAGE_H

#include "dbdb.h"
#include "ideide.h"
#include "datetime.h"
#include "htmlpage.h"
#include "languageresult.h"
#include "objectsobjects.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DataItem;
class HtmlDiv;
class HtmlPopup;
class HtmlXSLControl;
class IdeSession;
class IdeSessionPortal;
class IPortalDatabase;
class OMLContext;
class LoggedUser;
class Portal;
class PortalsTransaction;
class IdePathway;
class IdeSkin;
class XMLNode;
class XMLStylesheet;

//////////////////////////////////////////////////////////////////////

class EngineExport IPage : public HtmlPage
{
	typedef HtmlPage PageBase;

	typedef unordered_map<PageArea, shared_ptr<IHtmlControl> >::type AreasMap;

// Internal
protected:
	class EngineExport Message : public Object
	{
	// Enumerated types
	public:
		enum MessageType
		{
			mtInfo		= 0,
			mtWarning	= 1,
			mtError		= 2,
		};

	// Construction
	public:
		Message(MessageType type, const String &description, const String &title);
		virtual ~Message();

	// Operations
	public:
		void exportXML(shared_ptr<IPage> page, shared_ptr<XMLNode> node) const;

	private:
		MessageType m_type;
		String m_title;
		String m_description;
	};

	typedef shared_ptr<Message> message_ptr;
	typedef list<message_ptr>::type Messages;

// Enumerated types
public:
	enum RequestSource
	{
		rsOsiris	= 0,
		rsIsis	= 1
	};

// Construction
public:
	IPage(shared_ptr<HttpSession> session);
	virtual ~IPage();

// Attributes
public:
	inline shared_ptr<HtmlXSLControl> getTemplate() const;
	// Restituisce il tipo di richiesta (Osiris / Isis-Proxy / Isis-Redirect)
	inline RequestSource getRequestSource() const;
	// Specifica se è in accesso ACP
	inline bool getMcpMode() const;
	inline void setMcpMode(bool mcpMode);
	// Account Session
	inline shared_ptr<IdeSession> getSessionAccount() const;
	//inline shared_ptr<IdeSessionPortal> getSessionPortal() const;

	// Restituisce la lingua corrente
	virtual String getLanguage() const;
	// Restituisce il timeOffset corrente
	virtual int32 getTimeOffset() const;
	// Restituisce il DST corrente
	virtual int32 getTimeDST() const;
	// Restituisce un testo multilingua
	virtual String getText(const String &name) const;
	virtual String getText(const LanguageResult &result) const;
	// Formatta una data
	virtual String formatDate(const DateTime &dt, const DateTime::DateTimePattern &pattern) const;
	virtual String formatDate(const DateTime &dt, const String &pattern) const;
	// Restituisce la formattazione html di un testo
	String parseOml(const String &text, bool full = true, bool secure = false, bool row = false, OMLRenderMode mode = omlRenderModeOsiris, const String &refID = String::EMPTY, const String &fullUrl = String::EMPTY);
	virtual shared_ptr<OMLContext> parseOmlEx(const String &text, bool enableWarning, bool full, bool secure, bool row, OMLRenderMode mode = omlRenderModeOsiris, const String &refID = String::EMPTY, const String &fullUrl = String::EMPTY);
	// Restituisce un link all'help system
	String helpLink(const String &id) const;

	virtual shared_ptr<IdePathway> getPathway() const;
	virtual shared_ptr<IdeSkin> getSkin() const;

	// Restituisce il puntatore al portale ricavandolo dall'url
	virtual shared_ptr<Portal> getPortalFromUrl() const;
		
// Operations
public:
	void ensureAccountSession();

	// Mostra un messaggio
	void showMessage(const String &description, const String &title = String::EMPTY);
	void showWarning(const String &description, const String &title = String::EMPTY);
	void showError(const String &description, const String &title = String::EMPTY);

	shared_ptr<IHtmlControl> getArea(const PageArea &area);

	virtual DataItem getOption(const String &name) const;

protected:
	

// Overridables
public:
	virtual String getPageName() const;

	// Pathway
	virtual void onPathway() const;

	// Restituisce il pathway della pagina (di default coincide con quello della richiesta)
	virtual String getPagePathway() const;

	// Crea un template xsl
	virtual shared_ptr<XMLStylesheet> createStylesheet();
	// Carica e inizializza un template xsl
	virtual shared_ptr<XMLStylesheet> loadStylesheet(const String &pathname);

	// Inizializza un template xsl
	virtual void initStylesheet(shared_ptr<XMLStylesheet> stylesheet);

	// Effettua il login in un portale
	virtual bool loginAccountWithName(const String &login, const String &password, bool save_password = false);
	virtual bool loginAccountWithID(const String &accountID, const String &password, bool save_password = false);

	// Accesso ACP richiesto
	virtual bool isMcpModeRequired() const; // TODO: In Razor, when we clean portals pages derived from IMainPage, i can move this to PortalPage?

	// Accesso Isis consentito
	virtual bool isIsisAllowed() const; // TODO: In Razor, when we clean portals pages derived from IMainPage, i can move this to PortalPage?

protected:
	// Carica le risorse di base
	virtual void onLoadResources();

	virtual void onRenderDocument(shared_ptr<XMLNode> nodeRoot);

	virtual void onRenderInformations(shared_ptr<XMLNode> node);
	virtual void onRenderMessages(shared_ptr<XMLNode> node);
	virtual void onRenderActions(shared_ptr<XMLNode> node);
	virtual void onRenderPathWay(shared_ptr<XMLNode> node);

	virtual void onXSLError(const String &description);

// Page Overrides
public:
	virtual void renderLinktags(HtmlWriter &writer);
    virtual void redirect(const std::string &url);
	virtual bool transmit();

protected:
	virtual void onInit();
	virtual void onLoad();
	virtual void onRender(HtmlWriter &writer);
	virtual void onLogout();

protected:
	shared_ptr<IdeSession> m_session;
	shared_ptr<HtmlPopup> m_stats;
	Messages m_messages;
	shared_ptr<IdePathway> m_pathway;

	RequestSource m_requestSource;
	bool m_mcpMode;

private:
	AreasMap m_areas;
	shared_ptr<HtmlXSLControl> m_template;

// URGENT: Questo è temporaneo/sbagliato, il problema è che delle pagine di main dovrebbero essere delle IPortalPage: accounts,resources,settings...
// Il concetto è che DEVO avere una connessione al db e le main non dovrebbero richiederlo. Per pulirlo, semplicemente spostarle nella IPortalPage.
public:
	shared_ptr<IPortalDatabase> getDatabase() const;
	void closeDatabase(bool commit = true);

protected:
	mutable shared_ptr<PortalsTransaction> m_databaseWork;

public:
	static const String EVENT_ONLOGOUT;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<HtmlXSLControl> IPage::getTemplate() const { return m_template; }
inline IPage::RequestSource IPage::getRequestSource() const { return m_requestSource; }
inline bool IPage::getMcpMode() const { return m_mcpMode; }
inline void IPage::setMcpMode(bool mcpMode) { m_mcpMode = mcpMode; }
inline shared_ptr<IdeSession> IPage::getSessionAccount() const { return m_session; }
//inline shared_ptr<IdeSession> IPage::getSessionPortal() const { return m_session ? m_session->getSessionPortal(getPortal()) : null; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_IPAGE_H
