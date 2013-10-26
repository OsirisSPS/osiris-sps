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
#include "modulessearchviewer.h"

#include "htmlevent.h"
#include "htmlliteral.h"
#include "htmltext.h"
#include "httprequest.h"
#include "idesearchquery.h"
#include "ideskin.h"
#include "ideviewiobjectviewer.h"
#include "isearchresults.h"
#include "languageculture.h"
#include "languagefolder.h"
#include "languagemanager.h"
#include "log.h"
#include "modulessearch.h"
#include "modulesmodulesconfig.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "platformmanager.h"
#include "portalsaccount.h"
#include "idesession.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "searchquery.h"
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

SearchViewer::SearchViewer() : m_query(OS_NEW SearchQuery()),
							   m_ideSearchQuery(OS_NEW IdeSearchQuery(m_query))	
{
	m_ideSearchQuery->setID(_S("search"));	
}

SearchViewer::~SearchViewer()
{

}

void SearchViewer::onInit()
{
	ViewerBase::onInit();

	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	if(root != null)
		m_query->readXml(root);
}

void SearchViewer::onLoad()
{
	ViewerBase::onLoad();

	getControls()->add(m_ideSearchQuery);

	String controlID = getInstance().toUTF16();
	if(controlID.empty())
		controlID = _S("preview"); // Se il modulo è in preview, non ha un'ID di istanza. Ma un ID mi serve x forza per gli eventi.
	setID(controlID);
		
	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	if(root != null)
	{
		//String style = root->getAttributeString(OS_MODULES_LANGUAGESWITCHER_STYLE);
		//String cultures = root->getAttributeString(OS_MODULES_LANGUAGESWITCHER_CULTURES);

		bool showParams = root->getAttributeBool(_S("showParams"));
		bool directRun = root->getAttributeBool(_S("directRun"));		

		m_ideSearchQuery->setShowParams(showParams);
		m_ideSearchQuery->setDirectRun(directRun);

		bool allowRss = root->getAttributeBool(_S("allowRss"));
		if( (directRun) && (allowRss) )
		{
			if(getPage()->getRequest()->getUrlParam(_W("mode")) == _W("rss"))
			{
				doFeed();
			}
			else
			{			
				String urlRss = getPage()->getPortal()->getViewObjectLink(getInstance()) + _S("&mode=rss");
				m_query->setRssUrl(urlRss);
			}
		}	
	}	
}

void SearchViewer::onPreRender()
{
	ViewerBase::onPreRender();
}

void SearchViewer::doFeed()
{
	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	if(root != null)
	{
		String title = getPortal()->getOptions()->getName() + _S(" - ") + getTitle();
		String description = root->getAttributeString(_S("rssDescription"));		
		String body = root->getAttributeString(_S("rssBody"));		
		String link = getPage()->getPortal()->getViewObjectLink(getInstance());
		String language = getPage()->getLanguage();
		m_ideSearchQuery->doFeed(title, description, body, link, language);
	}
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
