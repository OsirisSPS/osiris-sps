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
#include "searchquery.h"

#include "iportaldatabase.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "isearchoptions.h"
#include "isearchresults.h"
#include "objectsirevisionable.h"
#include "searchresult.h"
#include "searchresultsdatabase.h"
#include "searchresultslucene.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "portalsportal.h"
#include "xmlattributes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

shared_ptr<SearchQuery> SearchQuery::getSections()
{
	static shared_ptr<SearchQuery> section = null;
	if(section == NULL)
	{
		section = shared_ptr<SearchQuery>(OS_NEW SearchQuery());
		section->setTreeMode(true);
	}

	return section;
}

shared_ptr<SearchQuery> SearchQuery::getForums()
{
	static shared_ptr<SearchQuery> forums = null;
	if(forums == NULL)
	{
		forums = shared_ptr<SearchQuery>(OS_NEW SearchQuery());
		forums->setTreeMode(true);
	}

	return forums;
}

//////////////////////////////////////////////////////////////////////

SearchQuery::SearchQuery()
{
	const ObjectsDescriptors &descriptors = ObjectsSystem::instance()->getDescriptors();
	for(ObjectsDescriptors::const_iterator i = descriptors.begin(); i != descriptors.end(); ++i)
	{
		PortalObjectType type = i->first;
		shared_ptr<ISearchOptions> options = i->second->getSearchOptions();
		if(options != null)
			m_options[type] = options;
	}

	m_templateView = "table";
	m_templatePager = "forum";

	m_textMode = searchTextModeAnyWords;
	m_textWhere = searchTextWhereAllFields;

	m_byUser = _S("*");
	m_showDeleted = false;

	m_fullTextMode = true;
	m_orderMode = searchOrderModeRelevance;
	m_orderDirection = searchOrderDirectionDescendent;
	m_treeMode = false;
	m_groupMode = false;
	m_offset = 0;
	m_maxResults = 0;
	m_pagerSize = 0;
	m_showInfo = false;
	m_parentsRecursive = true;
	m_showFilterAlphabetic = false;

	m_fromSubmitDateType = searchDateTimeNotSpecified;
	m_toSubmitDateType = searchDateTimeNotSpecified;
	m_fromInsertDateType = searchDateTimeNotSpecified;
	m_toInsertDateType = searchDateTimeNotSpecified;

	m_specialQuery = String::EMPTY;
}

SearchQuery::~SearchQuery()
{

}

bool SearchQuery::getFullTextMode() const 
{ 
	if(m_specialQuery.empty() == false)
		return false;

	if(m_text.empty())
		return false;

	if(m_text.substr(0,17) == _S("title-start-with:"))
		return false;
	
	return true;
}

String SearchQuery::getParentsListString() const
{
	return utils::desplit(m_parentsList, _S(";"), 0);
}

void SearchQuery::setParentsListString(const String& parentsList)
{
	utils::split(parentsList, _S(";"), m_parentsList, true);
}

DateTime SearchQuery::convertSearchDateTime(SearchDateTime type, const DateTime &dt)
{
	switch(type)
	{
	case searchDateTimeNotSpecified:
		{
			return DateTime::EMPTY;
		} break;
	case searchDateTimeSpecifiedDate:
		{
			return dt;
		} break;
	case searchDateTimeNow:
		{
			return DateTime::now();
		} break;
	case searchDateTimeReference:
		{
			return dt;
		} break;
	case searchDateTimeLast24Hour:
		{
			return DateTime::now() - TimeDuration(24,0,0);
		} break;
	case searchDateTimeLastWeek:
		{
			return DateTime::now() - TimeDuration(24*7,0,0);
		} break;
	case searchDateTimeLastTwoWeek:
		{
			return DateTime::now() - TimeDuration(24*7*2,0,0);
		} break;
	case searchDateTimeLastMonth:
		{
			return DateTime::now() - TimeDuration(24*30,0,0);
		} break;
	case searchDateTimeLastThreeMonth:
		{
			return DateTime::now() - TimeDuration(24*30*3,0,0);
		} break;
	case searchDateTimeLastYear:
		{
			return DateTime::now() - TimeDuration(24*365,0,0);
		} break;
	}
	return DateTime::EMPTY;
}

shared_ptr<ISearchResults> SearchQuery::search(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal)
{
	shared_ptr<ISearchResults> results;

	if(getFullTextMode())
	{
		// Lucene
		results.reset(OS_NEW SearchResultsLucene());
	}
	else
	{
		// Database
		results.reset(OS_NEW SearchResultsDatabase());
	}

	results->search(database, portal, get_this_ptr(), StringMap());

	if(getGroupMode())
	{
		// Se  in "groupMode", allora per alcuni tipi di oggetti (commenti, tag)  come se il risultato della ricerca fosse il padre, non l'oggetto stesso.
		// Devo per forza eseguire due passaggi, perch non posso ordinare correttamente con un solo passaggio, n posso filtrare con offset per realizzare il pager.
		// Questo metodo non pu restituire troppi risultati, altrimenti sarebbe troppo lunga l'sql.
		// Da notare anche che nella ricerca via Lucene vengono presi gli score di "relevance",
		// dato che tre post potrebbero avere "relevance" a 100%, 10%, 20%, ma compattati in un'unico oggetto "text".
		// Forse potrei fare una media, ma non ne vale la pena per ora.

		// Step 1: I due ResultsDatabase e ResultsLucene restituiscono gi l'oggetto "di gruppo".

		// Step 2: Faccio una distinct sui risultati

		// Calcola il numero totale di risultati
		//uint32 totalResults = results->getTotalResults();
		uint32 totalResults = results->getResults();

		StringMap items;

		for(uint32 i = 0; i < totalResults; i++)
		{
			shared_ptr<SearchResult> result = results->getResult(database, i, true);
			if(result != null)
				items[result->getID().toUTF16()] = true;
		}
		bool partial = results->getPartial();

		// Step 2: Ri-eseguo via database per avere l'elenco ordinato.
		results.reset(OS_NEW SearchResultsDatabase());
		results->search(database, portal, get_this_ptr(), items);

		results->setPartial(partial); // Se lo erano nel primo giro, lo son anche nel secondo.
	}

	return results;
}

void SearchQuery::readXml(shared_ptr<XMLNode> node)
{
	if(node->getAttributes()->exists(_S("template")))
		m_templateView = node->getAttributeString(_S("template"));
	if(node->getAttributes()->exists(_S("templatePager")))
		m_templatePager = node->getAttributeString(_S("templatePager"));
	if(node->getAttributes()->exists(_S("templateCustom")))
		m_templateCustom = node->getAttributeString(_S("templateCustom"));
	if(node->getAttributes()->exists(_S("text")))
		m_text = node->getAttributeString(_S("text"));
	if(node->getAttributes()->exists(_S("textMode")))
		m_textMode = (SearchTextMode) node->getAttributeUint32(_S("textMode"));
	if(node->getAttributes()->exists(_S("textWhere")))
		m_textWhere = (SearchTextWhere) node->getAttributeUint32(_S("textWhere"));
	if(node->getAttributes()->exists(_S("byUser")))
		m_byUser = node->getAttributeString(_S("byUser"));
	
	if(node->getAttributes()->exists(_S("orderMode")))
		m_orderMode = (SearchOrderMode) node->getAttributeUint32(_S("orderMode"));
	if(node->getAttributes()->exists(_S("orderDirection")))
		m_orderDirection = (SearchOrderDirection) node->getAttributeUint32(_S("orderDirection"));
	if(node->getAttributes()->exists(_S("fromSubmitDateType")))
		m_fromSubmitDateType = (SearchDateTime) node->getAttributeUint32(_S("fromSubmitDateType"));
	if(node->getAttributes()->exists(_S("fromSubmitDateSpecific")))
		m_fromSubmitDateSpecific = node->getAttributeDateTime(_S("fromSubmitDateSpecific"));
	if(node->getAttributes()->exists(_S("toSubmitDateType")))
		m_toSubmitDateType = (SearchDateTime) node->getAttributeUint32(_S("toSubmitDateType"));
	if(node->getAttributes()->exists(_S("toSubmitDateSpecific")))
		m_toSubmitDateSpecific = node->getAttributeDateTime(_S("toSubmitDateSpecific"));
	if(node->getAttributes()->exists(_S("fromInsertDateType")))
		m_fromInsertDateType = (SearchDateTime) node->getAttributeUint32(_S("fromInsertDateType"));
	if(node->getAttributes()->exists(_S("fromInsertDateSpecific")))
		m_fromInsertDateSpecific = node->getAttributeDateTime(_S("fromInsertDateSpecific"));
	if(node->getAttributes()->exists(_S("toInsertDateType")))
		m_toInsertDateType = (SearchDateTime) node->getAttributeUint32(_S("toInsertDateType"));
	if(node->getAttributes()->exists(_S("toInsertDateSpecific")))
		m_toInsertDateSpecific = node->getAttributeDateTime(_S("toInsertDateSpecific"));
	if(node->getAttributes()->exists(_S("treeMode")))
		m_treeMode = node->getAttributeBool(_S("treeMode"));
	if(node->getAttributes()->exists(_S("groupMode")))
		m_groupMode = node->getAttributeBool(_S("groupMode"));
	if(node->getAttributes()->exists(_S("offset")))
		m_offset = node->getAttributeUint32(_S("offset"));
	if(node->getAttributes()->exists(_S("maxResults")))
		m_maxResults = node->getAttributeUint32(_S("maxResults"));
	if(node->getAttributes()->exists(_S("pagerSize")))
		m_pagerSize = node->getAttributeUint32(_S("pagerSize"));
	if(node->getAttributes()->exists(_S("parentsList")))
		setParentsListString(node->getAttributeString(_S("parentsList")));
	if(node->getAttributes()->exists(_S("parentsRecursive")))
		m_parentsRecursive = node->getAttributeBool(_S("parentsRecursive"));
	if(node->getAttributes()->exists(_S("showInfo")))
		m_showInfo = node->getAttributeBool(_S("showInfo"));
	if(node->getAttributes()->exists(_S("showDeleted")))
		m_showDeleted = node->getAttributeBool(_S("showDeleted"));
	if(node->getAttributes()->exists(_S("showFilterAlphabetic")))
		m_showFilterAlphabetic = node->getAttributeBool(_S("showFilterAlphabetic"));	
}

void SearchQuery::writeXml(shared_ptr<XMLNode> node)
{
	node->setAttributeString(_S("template"), m_templateView);
	node->setAttributeString(_S("templatePager"), m_templatePager);
	node->setAttributeString(_S("templateCustom"), m_templateCustom);
	node->setAttributeString(_S("text"), m_text);
	node->setAttributeUint32(_S("textMode"), m_textMode);
	node->setAttributeUint32(_S("textWhere"), m_textWhere);
	node->setAttributeString(_S("byUser"), m_byUser);
	node->setAttributeUint32(_S("orderMode"), m_orderMode);
	node->setAttributeUint32(_S("orderDirection"), m_orderDirection);
	node->setAttributeUint32(_S("fromSubmitDateType"), m_fromSubmitDateType);
	node->setAttributeDateTime(_S("fromSubmitDateSpecific"), m_fromSubmitDateSpecific);
	node->setAttributeUint32(_S("toSubmitDateType"), m_toSubmitDateType);
	node->setAttributeDateTime(_S("toSubmitDateSpecific"), m_toSubmitDateSpecific);
	node->setAttributeUint32(_S("fromInsertDateType"), m_fromInsertDateType);
	node->setAttributeDateTime(_S("fromInsertDateSpecific"), m_fromInsertDateSpecific);
	node->setAttributeUint32(_S("toInsertDateType"), m_toInsertDateType);
	node->setAttributeDateTime(_S("toInsertDateSpecific"), m_toInsertDateSpecific);
	node->setAttributeBool(_S("treeMode"), m_treeMode);
	node->setAttributeBool(_S("groupMode"), m_groupMode);
	node->setAttributeUint32(_S("offset"), m_offset);
	node->setAttributeUint32(_S("maxResults"), m_maxResults);
	node->setAttributeUint32(_S("pagerSize"), m_pagerSize);
	node->setAttributeString(_S("parentsList"), getParentsListString());
	node->setAttributeBool(_S("parentsRecursive"), m_parentsRecursive);
	node->setAttributeBool(_S("showInfo"), m_showInfo);
	node->setAttributeBool(_S("showDeleted"), m_showDeleted);
	node->setAttributeBool(_S("showFilterAlphabetic"), m_showFilterAlphabetic);	
}

void SearchQuery::computeParentsList(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, StringList &out) const
{
	for(ParentsList::const_iterator i = m_parentsList.begin(); i != m_parentsList.end(); ++i)
	{
		EntityID id = (*i).to_ascii();
		shared_ptr<EntitiesEntity> entity = portal->getEntity(database, id);
		computeParentsList(database, portal, entity, out);
	}
}

void SearchQuery::computeParentsList(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, const shared_ptr<EntitiesEntity> entity, StringList &out) const
{
	if(entity == null)
		return;

	if(entity->getObjectType() != portalObjectTypeSection)
		return;
		
	out.push_back(entity->getEntityID().toUTF16());

	if(m_parentsRecursive)
	{
		shared_ptr<EntitiesEntities> childsSections = entity->getChilds(database, portalObjectTypeSection);
		for(EntitiesEntities::iterator i = childsSections->begin(); i != childsSections->end(); ++i)
		{
			shared_ptr<EntitiesEntity> childEntity = childsSections->get(database, *i);
			computeParentsList(database, portal, childEntity, out);
		}
	}
}

shared_ptr<ISearchOptions> SearchQuery::getOption(const PortalObjectType type)
{
	Options::const_iterator i = m_options.find(type);
	if(i != m_options.end())
		return i->second;

	return null;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
