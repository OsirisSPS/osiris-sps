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
#include "ideportalsearch.h"

#include "buffer.h"
#include "convert.h"
#include "conversions.h"
#include "css.h"
#include "entitiessnapshotmanager.h"
#include "htmlevent.h"
#include "httprequest.h"
#include "htmlbutton.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmldiv.h"
#include "htmlfilebrowser.h"
#include "htmllink.h"
#include "htmlpanel.h"
#include "htmlspan.h"
#include "htmltable.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "htmlxslcontrol.h"
#include "htmlliteral.h"
#include "idebutton.h"
#include "idesearchquery.h"
#include "idesession.h"
#include "ideskin.h"
#include "idetablequery.h"
#include "ideutils.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "objectsreputation.h"
#include "objectsuser.h"
#include "portalsaccount.h"
#include "portalsportal.h"
#include "portalsprofile.h"
#include "reputationsmanager.h"
#include "reputationsfeedback.h"
#include "searchquery.h"
#include "xmldocument.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace portal {

//////////////////////////////////////////////////////////////////////

Search::Search(shared_ptr<HttpSession> session) : PageBase(session),
												  m_query(OS_NEW SearchQuery()),
												  m_ideSearchQuery(OS_NEW IdeSearchQuery(m_query))
{
	m_ideSearchQuery->setID(_S("search"));	
	m_ideSearchQuery->setShowParams(true);

	if(getRequest()->getUrlParam(_W("mode")) == _W("ajax"))
		setAjax(true);
}

Search::~Search()
{

}

String Search::getPageName() const
{
	return _S("portal.pages.search");
}

void Search::onLoad()
{
	PageBase::onLoad();

	if(getRequest()->hasUrlParam(_W("text")))
	{
		String text = getRequest()->getUrlParam(_W("text"));
		m_query->setText(text);
	}

	if(getRequest()->hasUrlParam(_W("user")))
	{
		String user = getRequest()->getUrlParam(_W("user"));
		m_query->setByUser(user);
	}

	if(getRequest()->hasUrlParam(_W("orderMode")))
	{
		int orderMode = conversions::from_utf16<int32>(getRequest()->getUrlParam(_W("orderMode")));
		m_query->setOrderMode(static_cast<SearchOrderMode>(orderMode));
	}

	if(getRequest()->hasUrlParam(_W("groupMode")))
	{
		String groupMode = getRequest()->getUrlParam(_W("groupMode"));
		m_query->setGroupMode(groupMode != _S("0"));
	}

	if(getRequest()->hasUrlParam(_W("templateView")))
	{
		String templateView = getRequest()->getUrlParam(_W("templateView"));
		m_query->setTemplateView(templateView);
	}

	if(getRequest()->hasUrlParam(_W("run")))
	{
		m_ideSearchQuery->setDirectRun(true);
	}

	if(getRequest()->hasUrlParam(_W("hide_params")))
	{
		m_ideSearchQuery->setShowParams(false);
	}

	if(getRequest()->hasUrlParam(_W("rss")))
	{
		String rssUrl = getRequest()->getRawUrl() + "&format=rss";
		m_query->setRssUrl(rssUrl);
	}

	m_query->setShowInfo(true);

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());	
	shared_ptr<XMLNode> nodeRoot = document->create(_S("page"));
	
	String path = getSkin()->getTemplatePath(_S("search.xsl"));
	shared_ptr<HtmlXSLControl> pageTemplate(OS_NEW HtmlXSLControl(loadStylesheet(path), document));	

	pageTemplate->addChildParam(m_ideSearchQuery);
	
	// Carica il template di riferimento
	if(getAjax())
		getControls()->add(pageTemplate);
	else
		getArea(pageAreaContent)->getControls()->add(pageTemplate);
}

void Search::onPreRender()
{
	PageBase::onPreRender();	
	
	if(getRequest()->hasUrlParam(_W("format")))
	{
		if(getRequest()->getUrlParam(_W("format")) == "rss")
		{
			String rssTitle = getRequest()->getUrlParam(_W("rss"));
			String rssDescription = String::EMPTY;
			String rssBody = _S("full");
			String rssLink = String(getRequest()->getRawUrl()).replace_all("&format=rss","");
			m_ideSearchQuery->doFeed(rssTitle, rssDescription, rssBody, rssLink, getLanguage());
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // portal
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
