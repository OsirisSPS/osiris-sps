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
#include "searchresultslucene.h"

#include "entitiesentity.h"
#include "entitiessnapshotmanager.h"
#include "iportaldatabase.h"
#include "lucene.h"
#include "log.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "platformmanager.h"
#include "idesession.h"
#include "portalsportal.h"
#include "searchquery.h"
#include "searchresult.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

struct SearchResultsLucene::impl
{
	impl()
	{
        offset = 0;
		results = 0;
		totalResults = 0;
	}

	String queryText;
	scoped_ptr<lucene::search::IndexSearcher> searcher;
	scoped_ptr<lucene::analysis::standard::StandardAnalyzer> analyzer;
	scoped_ptr<lucene::search::Query> query;
	scoped_ptr<lucene::search::Sort> sort;
	scoped_ptr<lucene::search::Hits> hits;

	uint32 offset;
	uint32 results;
	uint32 totalResults;
};

//////////////////////////////////////////////////////////////////////

SearchResultsLucene::SearchResultsLucene()
{

}

SearchResultsLucene::~SearchResultsLucene()
{

}

String SearchResultsLucene::getQueryText() const
{
	return m_impl->queryText;
}

void SearchResultsLucene::_addDateRange(String &text, const String &field, const DateTime &from, const DateTime &to)
{
	if(from.isValid() || to.isValid())
	{
		String sFrom =	(from.isValid() ? from.toString() : _S("00000000000000"));
		String sTo = (to.isValid() ? to.toString() : _S("99999999999999"));

		text = String::format(_S("(%S) and (%S:[%S TO %S])").c_str(), text.c_str(), field.c_str(), sFrom.c_str(), sTo.c_str());
	}
}

void SearchResultsLucene::_addFieldIn(String &text, const String &field, const StringList &list)
{
	if(list.empty()) return;

	String subSearch;
	for(StringList::const_iterator i = list.begin(); i != list.end(); ++i)
	{
		String value = *i;
		if(!subSearch.empty()) subSearch += _S(" or ");
		subSearch += field + _S(":") + value;
	}
	text = _S("(") + text + _S(") and (") + subSearch + _S(")");
}

void SearchResultsLucene::exportXML(shared_ptr<XMLNode> node, shared_ptr<SearchQuery> query, shared_ptr<IPortalPage> page, bool external)
{
	//ResultsBase::exportXML(node, query, page, title);
	ResultsBase::exportXML(node, query, page, external);

	// Esporto la query in versione 'espansa' utilizzata per generare i risultati.
	if(query->getShowInfo())
		node->setAttributeString(_S("query"), m_impl->queryText);
}

void SearchResultsLucene::clear()
{
	m_impl->queryText.clear();

	m_impl->searcher.reset();
	m_impl->analyzer.reset();
	m_impl->query.reset();
	m_impl->sort.reset();
	m_impl->hits.reset();

	m_impl->offset = 0;
	m_impl->results = 0;
	m_impl->totalResults = 0;
}

shared_ptr<SearchResult> SearchResultsLucene::getResult(shared_ptr<IPortalDatabase> database, uint32 index, bool asGroup)
{
	OS_EXCEPT_IF(index >= getResults(), "Invalid result index");

	// Corregge l'indice specificato applicando l'offset di ricerca
	index += m_impl->offset;

	EntityID id = static_cast<String>(m_impl->hits->doc(index).get(OS_SEARCH_FIELD_ID.to_wide().c_str())).to_ascii();	
	double score = m_impl->hits->score(index);
	if(asGroup)
	{		
		shared_ptr<EntitiesEntity> entity = database->getPortal()->getEntity(database, id);
		if( (entity != NULL) && (entity->getCurrent() != NULL) )
		{
			if(ObjectsSystem::instance()->getDescriptor(entity->getObjectType())->isGroupable())
				id = String(m_impl->hits->doc(index).get(OS_SEARCH_FIELD_PARENT.to_wide().c_str())).to_ascii();
		}
	}

	return shared_ptr<SearchResult>(OS_NEW SearchResult(id, score));
}

uint32 SearchResultsLucene::getResults()
{
	return m_impl->results;
}

uint32 SearchResultsLucene::getTotalResults()
{
	return m_impl->totalResults;
}

void SearchResultsLucene::executeQuery(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, shared_ptr<SearchQuery> query, const StringMap& objectsMap)
{
	// Composizione della query Lucene
	String q = query->getText();

	// Se m_searchMode != smAnyWords, si suppone che la query non contenga sintassi avanzata, ma solo un elenco di keyword.
	// A farlo bene, bisognerebbe fare un detect se nella query c' qualcosa di diverso di una semplice lista di keyword,
	// e in tal caso considerare la query cone smAnyWords & swAllFields, indipendentemente da quanto specificato.
	switch(query->getTextMode())
	{
	case searchTextModeAnyWords:
		{
			// E' il comportamento di default del Lucene.
		} break;
	case searchTextModeAllWords:
		{
			q.replace_all(_S(" "), _S("+"));
		} break;
	case searchTextModeExactPhrase:
		{
			q = _S("\"") + q + _S("\"");
		} break;
	}

	switch(query->getTextWhere())
	{
	case searchTextWhereAllFields:
		{
			q = _S("title:(") + q + _S(") content:(") + q + _S(")");
		} break;
	case searchTextWhereOnlyTitle:
		{
			q = _S("title:(") + q + _S(")");
		} break;
	case searchTextWhereCustom:
		{
		} break;
	}

	const DateTime &fromSubmitDate = query->convertSearchDateTime(query->getFromSubmitDateType(),query->getFromSubmitDateSpecific());
	const DateTime &toSubmitDate = query->convertSearchDateTime(query->getToSubmitDateType(),query->getToSubmitDateSpecific());
	const DateTime &fromInsertDate = query->convertSearchDateTime(query->getFromInsertDateType(),query->getFromInsertDateSpecific());
	const DateTime &toInsertDate = query->convertSearchDateTime(query->getToInsertDateType(),query->getToInsertDateSpecific());

	_addDateRange(q, OS_SEARCH_FIELD_SUBMITDATE, fromSubmitDate, toSubmitDate);
	_addDateRange(q, OS_SEARCH_FIELD_INSERTDATE, fromInsertDate, toInsertDate);

	// Ricerca in determinati padri
	StringList parentsList;
	query->computeParentsList(database, portal, parentsList);
	_addFieldIn(q, OS_SEARCH_FIELD_PARENT, parentsList);


	// Esecuzione della ricerca

	clear();

	try
	{
		m_impl->queryText =	q;

		std::string path = portal->getSnapshotManager()->getSearchPathForLucene();

		m_impl->searcher.reset(new lucene::search::IndexSearcher(path.c_str()));

		m_impl->analyzer.reset(new lucene::analysis::standard::StandardAnalyzer());

		// OS_SEARCH_FIELD_CONTENT qui identifica il field di default se non specificato nella query.
		m_impl->query.reset(lucene::queryParser::QueryParser::parse(m_impl->queryText.to_wide().c_str(), OS_SEARCH_FIELD_CONTENT.to_wide().c_str(), m_impl->analyzer.get()));

		// Sorting
		bool reverse = query->getOrderDirection() == searchOrderDirectionDescendent;
		lucene::search::Sort *sorter = lucene::search::Sort::RELEVANCE;

		String sortField;
		switch(query->getOrderMode())
		{
		case searchOrderModeRelevance:
											break;

		/* // TOCLEAN_SNAPSHOT_SCORE. Vedi commento nella SnapshotManager, per riprestinarlo dovrò leggere l'utente da là.
		case searchOrderModeReputation:		sorter = new lucene::search::Sort(OS_SEARCH_FIELD_SCORE.to_wide().c_str(), reverse);
											break;
		*/

		case searchOrderModeSubmitDate:		sorter = new lucene::search::Sort(OS_SEARCH_FIELD_SUBMITDATE.to_wide().c_str(), reverse);
											break;

		case searchOrderModeInsertDate:		sorter = new lucene::search::Sort(OS_SEARCH_FIELD_INSERTDATE.to_wide().c_str(), reverse);
											break;

		case searchOrderModeTitle:			sorter = new lucene::search::Sort(OS_SEARCH_FIELD_TITLE.to_wide().c_str(), reverse);
											break;

		case searchOrderModePosition:
		case searchOrderModeLastObject:
		case searchOrderModeVotesTotal:
		case searchOrderModeVotesAverage:
											OS_EXCEPTION("Unsupported query order mode");
											break;

		default:							OS_EXCEPTION("Unknown query order mode");
											break;
		}

		if(sorter != lucene::search::Sort::RELEVANCE)
		{
			m_impl->sort.reset(sorter);
		}

		// Effettua la ricerca
		m_impl->hits.reset(m_impl->searcher->search(m_impl->query.get(), sorter));

		// Controlla se  stato specificato un offset di ricerca
		if(query->hasOffset())
		{
			// Salva l'offset di ricerca
			m_impl->offset = query->getOffset();
		}

		// Calcola il numero totale di risultati
		uint32 totalResults = m_impl->hits->length();
		// Controlla se l'offset di ricerca supera il numero totale di risultati (o implicitamente se non ci sono risultati)
		if(m_impl->offset >= totalResults)
		{
			// Nessun risultato

			m_impl->results = 0;
			m_impl->totalResults = 0;
		}
		else
		{
			m_impl->totalResults = totalResults;
			// Controlla se la query ha specificato un numero massimo di risultati
			if(query->hasMaxResults())
				// Se il numero totale di risultati (a partire dall'offset specificato) supera il limite massimo
				// usa quest'ultimo, altrimenti usa il numero di risultati disponibile
				m_impl->results = std::min<uint32>(query->getMaxResults(), totalResults - m_impl->offset);
			else
				// Se non  stato specificato un limite massimo il numero di risultati coincide col totale
				m_impl->results = totalResults;
		}
	}
	catch(CLuceneError &error)
	{
		OS_LOG_ERROR(String(_S("Lucene error: ")) + error.twhat());
	}
	catch(std::exception &e)
	{
		OS_LOG_ERROR(e.what());
    }
    catch(...)
    {
		OS_LOG_ERROR(OS_ERR_UNKNOWN(lucene));
    }
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
