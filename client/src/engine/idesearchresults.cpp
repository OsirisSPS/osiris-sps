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
#include "idesearchresults.h"

#include "boost/bind.hpp"
#include "conversions.h"
#include "ideskin.h"
#include "isearchresults.h"
#include "searchquery.h"
#include "htmlevent.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IdeSearchResults::EVENT_ONCHANGEOFFSET = _S("onChangeOffset");
const String IdeSearchResults::EVENT_ONFILTERALPHABETIC = _S("onFilterAlphabetic");

//////////////////////////////////////////////////////////////////////

IdeSearchResults::IdeSearchResults(shared_ptr<SearchQuery> query, shared_ptr<ISearchResults> results) :	m_query(query),
																					m_results(results)	
{		
}

IdeSearchResults::~IdeSearchResults()
{

}

shared_ptr<EventSource> IdeSearchResults::getEventChangeOffset() const
{
	return getEvents()->get(EVENT_ONCHANGEOFFSET);
}

String IdeSearchResults::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("search_results.xsl"));
}

String IdeSearchResults::getPageHref(uint32 offset) const
{
	return getEventCommand(EVENT_ONCHANGEOFFSET, conversions::to_utf16(offset));
}

void IdeSearchResults::onPreRender()
{
	// N.B.: la generazione dell'xml va effettuata in fase di PreRender in modo da consentire dall'esterno
	// di potersi agganciare all'evento di modifica della pagina e alterare la query di ricerca.

	if(m_query != nullptr && m_results != nullptr)
	{
		// Crea il documento per l'xsl
		shared_ptr<XMLNode> nodeRoot = getDocument()->create(_S("results"));	
		// Esporta i risultati
		//m_results->exportXML(nodeRoot, m_query, getPage(), m_title);
		m_results->exportXML(nodeRoot, m_query, getPage(), false);

		// Calcola il numero totale di risultati
		uint32 total_results = m_results->getTotalResults();
		// Calcola il numero di risultati da mostrare
		uint32 results_to_show = m_query->hasPagerSize() ? m_query->getPagerSize() : getSkin()->getPagerItems();
		//uint32 results_to_show = m_query->hasMaxResults() ? m_query->getMaxResults() : total_results;

		if( (m_query->hasMaxResults()) && (total_results > m_query->getMaxResults()) )
			total_results = m_query->getMaxResults();

		// Crea il pager dei risultati se necessario
		createPager(getSkin(), nodeRoot, total_results, results_to_show, m_query->getOffset());		

		if( (m_query->getShowFilterAlphabetic()) )
		{
			String currentAlphabetic = _S("*");
			if( (m_query->getText().substr(0,17) == _S("title-start-with:")) && (m_query->getText().length() == 18) )
				currentAlphabetic = m_query->getText().substr(17,1).to_upper();
			
			nodeRoot->setAttributeString(_S("current_filter_alphabetic"), currentAlphabetic);
			
			String hrefFilterAlphabetic = getEventCommand(EVENT_ONFILTERALPHABETIC, _S("Letter"));
			nodeRoot->setAttributeString(_S("href_filter_alphabetic"), hrefFilterAlphabetic);
		}
	}

	ControlBase::onPreRender();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
