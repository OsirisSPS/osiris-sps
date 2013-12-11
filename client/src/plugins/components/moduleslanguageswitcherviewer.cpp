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
#include "moduleslanguageswitcherviewer.h"

#include "boost/bind.hpp"
#include "htmlevent.h"
#include "htmlliteral.h"
#include "httpresponse.h"
#include "ideaccount.h"
#include "ideaccountsmanager.h"
#include "idesystem.h"
#include "ideviewiobjectviewer.h"
#include "languageculture.h"
#include "languagefolder.h"
#include "languagemanager.h"
#include "log.h"
#include "moduleslanguageswitcher.h"
#include "modulesmodulesconfig.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "platformmanager.h"
#include "portalsaccount.h"
#include "idesession.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlschema.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

extern OS_NAMESPACE_NAME::String g_componentsPath;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {

//////////////////////////////////////////////////////////////////////

namespace modules {

//////////////////////////////////////////////////////////////////////

const String LanguageSwitcherViewer::EVENT_ONSELECT = _S("onSelect");

//////////////////////////////////////////////////////////////////////

LanguageSwitcherViewer::LanguageSwitcherViewer()
{

}

LanguageSwitcherViewer::~LanguageSwitcherViewer()
{

}

void LanguageSwitcherViewer::onSelect(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	shared_ptr<IdeSession> ideSession = getSessionAccount();
	if( (ideSession != null) && ideSession->isLogged() )
	{
		shared_ptr<IdeAccount> account = getSessionAccount()->getAccount();
		if(account->getLanguage() != htmlEvent->get(0))
		{
			account->setLanguage(htmlEvent->get(0));
			IdeAccountsManager::instance()->save(account);
			//account->updateAccount(getDatabase());
			IdeAccountsManager::instance()->save(account);
		}
	}
	else
	{
		getResponse()->setCookie("os_culture", htmlEvent->get(0).to_ascii());
	}	

	getPage()->reload();
}

void LanguageSwitcherViewer::onLoad()
{
	ViewerBase::onLoad();

	String controlID = getInstance().toUTF16();
	if(controlID.empty())
		controlID = _S("preview"); // Se il modulo è in preview, non ha un'ID di istanza. Ma un ID mi serve x forza per gli eventi.
	setID(controlID);

	getEvents()->get(EVENT_ONSELECT)->connect(boost::bind(&LanguageSwitcherViewer::onSelect, this, _1));

	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	OS_ASSERT(root != null);
	
	String style = root->getAttributeString(OS_MODULES_LANGUAGESWITCHER_STYLE);
	String cultures = root->getAttributeString(OS_MODULES_LANGUAGESWITCHER_CULTURES);
	
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLStylesheet> stylesheet(OS_NEW XMLStylesheet());
	// Inizializza le funzioni del template
	getPage()->initStylesheet(stylesheet);

	String path = utils::makeFilePath(utils::makeFolderPath(g_componentsPath, _S("languageswitcher")), _S("languageswitcher_viewer.xsl"));

	// Carica l'xsl
	if(stylesheet->parseFile(path))
	{
		// URGENT devo per forza creare un documento?
		// Se il secondo parametro della XSLControl  null non crea il controllo,
		// e non posso passare una "getDocument()" perch se  una nuova istanza  null pure lui...
		shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
		document->create(OS_MODULES_LANGUAGESWITCHER_ROOT);

		shared_ptr<XMLNode> root = document->getRoot();
		root->setAttributeString(OS_MODULES_LANGUAGESWITCHER_STYLE, style);

		shared_ptr<XMLNode> culturesNode = root->addChild(_S("cultures"));
		StringVector culturesArray;
		utils::split(cultures,_S(";"), culturesArray);

		for(uint32 a1=0; a1<culturesArray.size(); a1++)
		{
			String cultureID = culturesArray[a1];
			if(LanguageManager::instance()->hasCulture(cultureID))
			{
				shared_ptr<LanguageCulture> culture = LanguageManager::instance()->getCulture(cultureID);

				shared_ptr<XMLNode> cultureNode = culturesNode->addChild(_S("culture"));
				cultureNode->setAttributeString(_S("id"), cultureID);
				cultureNode->setAttributeString(_S("native"), culture->getNativeName());
				cultureNode->setAttributeString(_S("english"), culture->getEnglishName());

				cultureNode->setAttributeString(_S("href"), getEventCommand(LanguageSwitcherViewer::EVENT_ONSELECT, cultureID));

				cultureNode->setAttributeBool(_S("selected"), (getPage()->getLanguage().to_lower() == cultureID.to_lower()));
			}
		}				

		shared_ptr<HtmlXSLControl> templ(OS_NEW HtmlXSLControl(stylesheet, document));			
		getControls()->add(templ);
	}
	else
	{
		// Template non valido.
	}				
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
