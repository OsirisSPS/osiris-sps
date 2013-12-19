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

#ifndef _IDE_PORTAL_IPORTALPAGE_H
#define _IDE_PORTAL_IPORTALPAGE_H

#include "entitiesentities.h"
#include "iidepage.h"
#include "dataitem.h"
#include "ids.h"
#include "objectsobjects.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeAssistant;
class IsisEndpoint;
class ObjectID;

//////////////////////////////////////////////////////////////////////

class EngineExport IPortalPage : public IPage
{
	typedef IPage PageBase;

// Construction
public:
	IPortalPage(shared_ptr<HttpSession> session);
	virtual ~IPortalPage();

// Attributes
public:
	inline shared_ptr<Portal> getPortal() const;
		
	//virtual shared_ptr<IPortalDatabase> getDatabase() const;
	virtual String getLanguage() const;
	virtual int32 getTimeOffset() const;
	virtual int32 getTimeDST() const;
	virtual shared_ptr<IdeSkin> getSkin() const;

	// Restituisce un oggetto del portale
	shared_ptr<ObjectsIObject> getObject(const ObjectID &id) const;

	// Restituisce il target della vista corrente
	virtual EntityID getTargetObject() const;

	inline shared_ptr<IdeAssistant> getAssistant() const;

	inline shared_ptr<IsisEndpoint> getIsisEndpoint() const;

	inline bool getLoadInstances() const;
	inline void setLoadInstances(bool loadInstances);

	String getStabilityStatus();

protected:
	// Indica se la pagina carica le istanze posizionate nell'area con
	virtual bool getShowInstance(shared_ptr<ObjectsInstance> instance) const;

// Operations
public:
	//void redirect(ide::portal::PortalPage page);
	void redirectPage(const std::string &page);

	// Ricerca le istanze da collocare in base all'ID, e le colloca nelle aree.
	virtual uint32 loadInstances(const EntityID &ID);

	virtual DataItem getOption(const String &name) const;

	// Se la richiesta arriva da un Isis, restituisce l'url.
	String getRequestIsisUrl() const;

	void updateReferenceDate();


// HtmlPage overrides
public:
	virtual void renderLinktags(HtmlWriter &writer);
	virtual void redirect(const std::string &url);
	virtual bool transmit();

// IPage overrides
public:
	// Pathway
	virtual void onPathway() const;
	virtual void onPathway2() const;
	// Restituisce un testo multilingua
	//virtual String getText(const String &name) const;	
	// Restituisce la formattazione html di un testo
	virtual shared_ptr<OMLContext> parseOmlEx(const String &text, bool enableWarning, bool full, bool secure, bool row, OMLRenderMode mode, const String &refID, const String &fullUrl);

	// Inizializza un template xsl
	virtual void initStylesheet(shared_ptr<XMLStylesheet> stylesheet);

	// Ripristina una sessione precedente
	//virtual bool restoreSession(shared_ptr<IdeSession> ideSession);

	// Accesso MCP richiesto
	virtual bool isMcpModeRequired() const;

	// Accesso Isis consentito
	virtual bool isIsisAllowed() const;

protected:
	virtual String getBaseTitle();

	virtual void onRenderDocument(shared_ptr<XMLNode> nodeRoot);
	virtual void onRenderInformations(shared_ptr<XMLNode> node);
	virtual void onRenderActions(shared_ptr<XMLNode> nodeActions);

	virtual uint32 onLoadInstances(shared_ptr<EntitiesEntity> entity, EntityID origin);

protected:
	virtual void onInit();
	virtual void onLoad();
	virtual void onPreRender();

	virtual void onPortalLogin();
	virtual void onPortalLogout();


protected:
	shared_ptr<Portal> m_portal;

	mutable shared_ptr<LoggedUser> m_loggedUser;

	shared_ptr<IdeAssistant> m_assistant;

	bool m_loadInstances;	

	shared_ptr<IsisEndpoint> m_isisEndpoint;

public:
	static const String EVENT_ONPORTALLOGIN;
	static const String EVENT_ONPORTALLOGOUT;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<Portal> IPortalPage::getPortal() const { return m_portal; }

inline shared_ptr<IdeAssistant> IPortalPage::getAssistant() const { return m_assistant; }
inline shared_ptr<IsisEndpoint> IPortalPage::getIsisEndpoint() const { return m_isisEndpoint; }

inline bool IPortalPage::getLoadInstances() const { return m_loadInstances; }
inline void IPortalPage::setLoadInstances(bool loadInstances) { m_loadInstances = loadInstances; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_PORTAL_IPORTALPAGE_H
