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
#include "idesearchquery.h"

#include "boost/bind.hpp"
#include "buffer.h"
#include "conversions.h"
#include "htmlevent.h"
#include "htmlliteral.h"
#include "htmltextbox.h"
#include "httpsession.h"
#include "idesearchglobalparams.h"
#include "idesearchresults.h"
#include "idesession.h"
#include "ideskin.h"
#include "idesystem.h"
#include "isearchresults.h"
#include "mimemanager.h"
#include "osiriscommon.h"
#include "searchquery.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlschema.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN() 

//////////////////////////////////////////////////////////////////////

const String IdeSearchQuery::EVENT_ONSEARCH = _S("onSearch");

//////////////////////////////////////////////////////////////////////

IdeSearchQuery::IdeSearchQuery(shared_ptr<SearchQuery> query) :	m_query(query),
														m_results(OS_NEW IdeSearchResults(query)),
														m_filterAlphabetic(OS_NEW HtmlTextBox())
{	
	m_showParams = true;
	m_directRun = false;

	setID(_S("query"));

	m_results->setID(_S("results"));

	m_filterAlphabetic->setID(_S("filterAlphabetic"));
	m_filterAlphabetic->setValue(_S("*"));
	m_filterAlphabetic->setHidden(true);	
}

IdeSearchQuery::~IdeSearchQuery()
{

}

String IdeSearchQuery::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("search_query.xsl"));
}

void IdeSearchQuery::onInit()
{
	ControlBase::onInit();

	getTemplate()->addChildParam(m_results);
	
	getEvents()->get(EVENT_ONSEARCH)->connect(boost::bind(&IdeSearchQuery::doSearch, this));

	m_results->getEvents()->get(IdeSearchResults::EVENT_ONCHANGEOFFSET)->connect(boost::bind(&IdeSearchQuery::onChangeOffset, this, _1));
	m_results->getEvents()->get(IdeSearchResults::EVENT_ONFILTERALPHABETIC)->connect(boost::bind(&IdeSearchQuery::onFilterAlphabetic, this, _1));
	
	// Crea il documento per l'xsl
	shared_ptr<XMLNode> nodeRoot = getDocument()->create(_S("query"));

	if(m_showParams)
	{
		m_globalParams.reset(OS_NEW IdeSearchGlobalParams(m_query));
		m_globalParams->setID(_S("globalParams"));
		getTemplate()->addChildParam(m_globalParams);

		nodeRoot->setAttributeString(_S("submit_href"), getEventCommand(IdeSearchQuery::EVENT_ONSEARCH));		
	}

	getControls()->add(m_filterAlphabetic);
}

void IdeSearchQuery::onLoad()
{
	ControlBase::onLoad();

	if( (m_showParams == false) || (m_directRun == true) )
		doSearch();
}

void IdeSearchQuery::onPreRender()
{
	ControlBase::onPreRender();
}

void IdeSearchQuery::onChangeOffset(IEvent *e)
{	
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	m_query->setOffset(conversions::from_utf16<uint32>(htmlEvent->get(0)));

	doSearch();
}

void IdeSearchQuery::onFilterAlphabetic(IEvent *e)
{
	HtmlEvent *htmlEvent = dynamic_cast<HtmlEvent *>(e);
	if(htmlEvent == null)
		return;

	String letter = htmlEvent->get(0);
	m_filterAlphabetic->setValue(letter);
	
	doSearch();
}


void IdeSearchQuery::doSearch()
{
	if(m_globalParams != null)
		m_globalParams->fillQuery();
	
	if( (m_query->getShowFilterAlphabetic()) )
	{
		String letter = m_filterAlphabetic->getValue();
		if(letter == _S("*"))
			m_query->setText(String::EMPTY); // URGENT: Modifiche a mano?
		else
		{
			m_query->setText(_S("title-start-with:") + letter);
		}
	}

	//m_query->setLimit(getSkin()->getPagerItems());
	m_query->setLimit(m_query->hasPagerSize() ? m_query->getPagerSize() : getSkin()->getPagerItems());

	if(m_query->getFromSubmitDateType() == searchDateTimeReference)
		m_query->setFromSubmitDateSpecific(getSessionAccount()->getReferenceDate());

	if(m_query->getToSubmitDateType() == searchDateTimeReference)
		m_query->setToSubmitDateSpecific(getSessionAccount()->getReferenceDate());

	if(m_query->getFromInsertDateType() == searchDateTimeReference)
		m_query->setFromInsertDateSpecific(getSessionAccount()->getReferenceDate());

	if(m_query->getToInsertDateType() == searchDateTimeReference)
		m_query->setToInsertDateSpecific(getSessionAccount()->getReferenceDate());


	shared_ptr<ISearchResults> results = m_query->search(getDatabase(), getPortal());
	m_results->setResults(results);
	//m_results.reset(OS_NEW ide::SearchResults(m_query, results));	
	
}

void IdeSearchQuery::doFeed(const String &title, const String &description, const String &body, const String &link, const String &language)
{
	shared_ptr<ISearchResults> results = m_query->search(getDatabase(), getPortal());

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());

	// Crea il documento per l'xsl
	shared_ptr<XMLNode> nodeRoot = document->create(_S("feed"));

	shared_ptr<XMLNode> nodeResults = nodeRoot->addChild(_S("results"));
	
	// Esporta i risultati
	results->exportXML(nodeResults, m_query, getPage(), true);

	nodeRoot->setAttributeString(_S("title"), title);
	nodeRoot->setAttributeString(_S("description"), description);
	nodeRoot->setAttributeString(_S("body"), body);
	nodeRoot->setAttributeString(_S("link"), IdeSystem::instance()->getLocalUrl(link));
	nodeRoot->setAttributeString(_S("language"), language);

	String path = getSkin()->getTemplatePath(_S("search_rss.xsl"));

	shared_ptr<XMLStylesheet> stylesheet(OS_NEW XMLStylesheet());
	// Inizializza le funzioni del template
	getPage()->initStylesheet(stylesheet);

	// Carica l'xsl
	if(stylesheet->parseFile(path) == false)
		return;

	String output;

	if(stylesheet->applyToString(document, output) == false)
		return;

	output = _S("<?xml version='1.0' encoding='UTF-8'?>\n") + output;

	// TODO:
	// - L'output bbcode non dovrebbe avere class css, o cmq minimali
	// - Gli url dovrebbero essere completi, non relativi
	
	std::string s = output.to_utf8();

	String mime_type = _S("application/rss+xml");
	getPage()->getSession()->transmitBufferPtr(s.data(), static_cast<uint32>(s.size()), String::EMPTY, mime_type);
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
