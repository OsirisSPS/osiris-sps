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
#include "searchresultsdatabase.h"

#include "algorithms.h"
#include "convert.h"
#include "dbsqlselect.h"
#include "dbtables.h"
#include "entitiesentity.h"
#include "entitiessnapshotmanager.h"
#include "isearchoptions.h"
#include "objectsdescriptor.h"
#include "objectsuser.h"
#include "objectssystem.h"
#include "options.h"
#include "portalsportal.h"
#include "searchquery.h"
#include "searchresult.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

SearchResultsDatabase::SearchResultsDatabase() : m_totalResults(0)
{

}

SearchResultsDatabase::~SearchResultsDatabase()
{

}

void SearchResultsDatabase::add(const EntityID &id)
{
	shared_ptr<SearchResult> result(OS_NEW SearchResult(id));
	m_objects.push_back(result);
}

void SearchResultsDatabase::clear()
{
	m_objects.clear();
	m_totalResults = 0;
	m_partial = false;
}

shared_ptr<SearchResult> SearchResultsDatabase::getResult(shared_ptr<IPortalDatabase> database, uint32 index, bool asGroup)
{
	OS_EXCEPT_IF(index >= getResults(), "Invalid result index");
	return m_objects[index];
}

uint32 SearchResultsDatabase::getResults()
{
	return static_cast<uint32>(m_objects.size());
}

uint32 SearchResultsDatabase::getTotalResults()
{
	return m_totalResults;
}

void SearchResultsDatabase::executeQuery(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, shared_ptr<SearchQuery> query, const StringMap& objectsMap)
{
	clear();

	shared_ptr<DbSqlSelect> select(new DbSqlSelect(DBTABLES::SNAPSHOT_OBJECTS_TABLE));
	select->fields.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::ENTITY, DBTABLES::SNAPSHOT_OBJECTS_TABLE));
	// Se  il primo step di un groupMode, ho bisogno di estrarre anche il padre.
	if(query->getGroupMode() && (objectsMap.empty()))
		select->fields.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::PARENT, DBTABLES::SNAPSHOT_OBJECTS_TABLE));
	
	// Solo se non  specificata gi una lista di oggetti.
	// Le where si riferiscono agli oggetti singoli (ex. post), non al loro oggetto da visualizzare.
	// Quindi non  necessario specificarle nel secondo giro.
	if(objectsMap.empty())
	{
		// current != ""
		select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::CURRENT, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(String::EMPTY), DbSqlCondition::cfDifferent | DbSqlCondition::cfAnd);

		// Controlla se  stata specificata una data di creazione minima
		const DateTime &fromSubmitDate = query->convertSearchDateTime(query->getFromSubmitDateType(),query->getFromSubmitDateSpecific());
		if(fromSubmitDate.isValid())
		{
			// submit_date >= fromSubmitDate
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(fromSubmitDate), DbSqlCondition::cfMajor | DbSqlCondition::cfEqual | DbSqlCondition::cfAnd);
		}

		// Controlla se  stata specificata una data di creazione massima
		const DateTime &toSubmitDate = query->convertSearchDateTime(query->getToSubmitDateType(),query->getToSubmitDateSpecific());
		if(toSubmitDate.isValid())
		{
			// submit_date <= toSubmitDate
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(toSubmitDate), DbSqlCondition::cfMinor | DbSqlCondition::cfEqual | DbSqlCondition::cfAnd);
		}

		// Controlla se  stata specificata una data di inserimento minima
		const DateTime &fromInsertDate = query->convertSearchDateTime(query->getFromInsertDateType(),query->getFromInsertDateSpecific());
		if(fromInsertDate.isValid())
		{
			// insert_date >= fromInsertDate
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::INSERT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(fromInsertDate), DbSqlCondition::cfMajor | DbSqlCondition::cfEqual | DbSqlCondition::cfAnd);
		}

		// Controlla se  stata specificata una data di creazione massima
		const DateTime &toInsertDate = query->convertSearchDateTime(query->getToInsertDateType(),query->getToInsertDateSpecific());
		if(toInsertDate.isValid())
		{
			// insert_date <= toInsertDate
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::INSERT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(toInsertDate), DbSqlCondition::cfMinor | DbSqlCondition::cfEqual | DbSqlCondition::cfAnd);
		}

		// Non mostrare le entit virtuali. Dato che le entit virtuali hanno insertDate==null,
		select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::INSERT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(DateTime::EMPTY), DbSqlCondition::cfDifferent | DbSqlCondition::cfAnd);

		// Se non deve mostrare gli oggetti cancellati forza la visibilit ad 1
		if(query->getShowDeleted() == false)
			// visible == 1
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::VISIBLE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(true));

		StringList types;
		for(SearchQuery::Options::const_iterator i = query->getOptions().begin(); i != query->getOptions().end(); ++i)
		{
			shared_ptr<ISearchOptions> options = i->second;
			if(options->getActive())
			{
				types.push_back(Convert::toString(options->getType()));
			}
		}

		if(types.empty() == false)
			// type in (...)
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::TYPE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), types);

		// Ricerca in determinati padri
		StringList parentsList;
		query->computeParentsList(database, portal, parentsList);
		// Ci metto gli apici...
		for(StringList::iterator i = parentsList.begin(); i != parentsList.end(); ++i)
		{
			*i = _S("'") + String(*i) + _S("'");
		}
		if(!parentsList.empty())
		{
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::PARENT, DBTABLES::SNAPSHOT_OBJECTS_TABLE), parentsList);
		}

		// 0.14 RC3
		if(query->getText().substr(0,17) == _S("title-start-with:"))
		{
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::TITLE), Convert::toSQL(query->getText().substr(17) + _S("%")), DbSqlCondition::cfLike | DbSqlCondition::cfAnd);
		}

		// 0.14 RC3
		if(query->getText().substr(0,17) == _S("title-start-with:"))
		{
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::TITLE), query->getText().substr(17) + _S("%"), DbSqlCondition::cfLike | DbSqlCondition::cfAnd);
		}

		// 0.12
		if( (query->getByUser().empty() == false) && (query->getByUser() != _S("*")) )
		{
			String user = query->getByUser();
			
			// MySelf test
			if(user == _S("myself"))
			{
				// VERYURGENTRAZOR: Commentata perchè ho piallato il loggedUser come parametro... rivalutare la cosa
				/*
				if( (loggedUser != nullptr) && loggedUser->isLogged() && (loggedUser->getGuestMode() == false))
				{
					user = loggedUser->getUser()->id->toUTF16();
				}
				else
				{
					user == String::EMPTY;
				}
				*/
				user == String::EMPTY;
			}
			else
			{
				// VERYURGENT, un test migliore per verificare se è un ID?
				// VERYURGENT, l'sql potrebbe essere Sqlite-specifica.
				// Fatto meglio, dovrei avere un'altro combo "localizza per" "nick/id"
				// Inoltre qui ci sarebbe da supportare una ricerca "non exact-match" per farlo stile Invision.
				if(user.length() != 48)
				{
					algorithms::to_upper(user);
					String sql = String::format(_S("select rs.id from os_snapshot_users ts, os_users tu where ts.id=tu.id and upper(tu.name) = %S").c_str(), Convert::toSQL(user).c_str());
					DataTable result;
					if(database->execute(sql, result))
					{
						if(result.rows() > 0 && result.columns() > 0)
							user = static_cast<String>(*result[0][0]);
					}
				}
			}

			// Come autore della primaria
			select->tables.add(DbSqlTable(DBTABLES::ENTRIES_TABLE));
			// Crea la join
			select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::ENTITY, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField(DBTABLES::ENTRIES::ID, DBTABLES::ENTRIES_TABLE));
			select->where.add(DbSqlField(DBTABLES::ENTRIES::AUTHOR, DBTABLES::ENTRIES_TABLE), Convert::toSQL(user));

			// Come editore dell'oggetto
			// where DBTABLES::SNAPSHOT_OBJECTS::REFERENCE in (select id from os_entries where revision= getByUser();
		}
	}
	else
	{
		// Se objectsList  valorizzata, allora  il secondo passo di una ricerca in groupMode, gli passo l'elenco oggetti.

		StringList objectsList;
		for(StringMap::const_iterator i = objectsMap.begin(); i != objectsMap.end(); ++i)
		{
			objectsList.push_back(_S("'") + i->first + _S("'"));
		}
		//select->where.add(String(DBTABLES::SNAPSHOT_OBJECTS_TABLE) + _S(".") + DBTABLES::SNAPSHOT_OBJECTS::REFERENCE, objectsList);
		select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::ENTITY, DBTABLES::SNAPSHOT_OBJECTS_TABLE), objectsList);

	}

	// Se  il primo passaggio di una groupMode, l'ordinamento  inutile.
	// 0.12 RC5, tolto, con un limite nel primo giro di group-mode, devono essere ordinati.
	//if( (!query->getGroupMode()) || (!objectsMap.empty()) )
	{
		DbSqlField::Order order = DbSqlField::foNone;
		switch(query->getOrderDirection())
		{
		case searchOrderDirectionDescendent:	order = DbSqlField::foDesc;
												break;

		case searchOrderDirectionAscendent:		order = DbSqlField::foAsc;
												break;

		default:			OS_ASSERTFALSE();
							break;
		}

		SearchOrderMode orderMode = query->getOrderMode();

		// 0.12 RC5, da testare
		// Se è il primo passaggio di una group-mode, l'ordinamento "omLastObject" deve essere un "omSubmitDate".
		if( (query->getGroupMode()) && (objectsMap.empty()) )
		{
			if(orderMode == searchOrderModeLastObject)
				orderMode = searchOrderModeSubmitDate;
			else if(orderMode == searchOrderModeVotesTotal)
				orderMode = searchOrderModeSubmitDate;
			else if(orderMode == searchOrderModeVotesAverage)
				orderMode = searchOrderModeSubmitDate;
		}


		switch(orderMode)
		{
		case searchOrderModeRelevance:		// Non c' la rilevanza nelle search non basate sul Lucene.
											break;

		// TOCLEAN_SNAPSHOT_SCORE
		/* Per riprestinarlo, devo fare una join con la snapshot::users.
		case searchOrderModeReputation:		select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::SCORE, DBTABLES::SNAPSHOT_OBJECTS_TABLE));
											select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::POSITION, DBTABLES::SNAPSHOT_OBJECTS_TABLE), order);
											break;
		*/

		case searchOrderModeSubmitDate:		select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), order);
											break;

		case searchOrderModeInsertDate:		select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::INSERT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), order);
											break;

		case searchOrderModePosition:		select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::POSITION, DBTABLES::SNAPSHOT_OBJECTS_TABLE), order);
											break;

		case searchOrderModeTitle:			select->orderBy.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::TITLE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), order);
											break;

		case searchOrderModeLastObject:		{
												select->tables.add(DbSqlTable(DBTABLES::DISCUSSIONS_STATS_TABLE));
												// Crea la join
												select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::ENTITY, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField(DBTABLES::DISCUSSIONS_STATS::REFERENCE, DBTABLES::DISCUSSIONS_STATS_TABLE));

												DbSqlField last_entry_date(DBTABLES::DISCUSSIONS_STATS::LAST_ENTRY_DATE, DBTABLES::DISCUSSIONS_STATS_TABLE);
												last_entry_date.setOrder(order);
												select->orderBy.fields.add(last_entry_date);
											}
											break;

		case searchOrderModeVotesTotal:		{
												select->tables.add(DbSqlTable(DBTABLES::DISCUSSIONS_STATS_TABLE));
												// Crea la join
												select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::ENTITY, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField(DBTABLES::DISCUSSIONS_STATS::REFERENCE, DBTABLES::DISCUSSIONS_STATS_TABLE));

												DbSqlField last_entry_date(DBTABLES::DISCUSSIONS_STATS::VOTES, DBTABLES::DISCUSSIONS_STATS_TABLE);
												last_entry_date.setOrder(order);
												select->orderBy.fields.add(last_entry_date);
											}
											break;

		case searchOrderModeVotesAverage:		{
												select->tables.add(DbSqlTable(DBTABLES::DISCUSSIONS_STATS_TABLE));
												// Crea la join
												select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::ENTITY, DBTABLES::SNAPSHOT_OBJECTS_TABLE), DbSqlField(DBTABLES::DISCUSSIONS_STATS::REFERENCE, DBTABLES::DISCUSSIONS_STATS_TABLE));

												DbSqlField last_entry_date(DBTABLES::DISCUSSIONS_STATS::VOTES_AVERAGE, DBTABLES::DISCUSSIONS_STATS_TABLE);
												last_entry_date.setOrder(order);
												select->orderBy.fields.add(last_entry_date);
											}
											break;

		default:				OS_EXCEPTION("Unknown query order mode");
								break;
		}
	}

	uint32 searchLimit = Options::instance()->getOption<uint32>(Options::portals_options::search_limit);

	if(searchLimit != 0)
	{
		// 0.12 RC5 - Altrimenti, setto cmq un massimo di risultati, come limite massimo ragionevole.
		select->limit.setCount(searchLimit);
	}

	if(query->hasMaxResults())
	{
		// 0.13 RC2
		select->limit.setCount(query->getMaxResults());
	}

	// Fa prima una "count" per sapere quanti risultati totali, ad esempio per calcolare le pagine del pager.
	// Poi, vengono estratti solo i record che servono alla pagina attuale (se non è il primo giro di un group-mode).
	select->count = true;

	DataTable result;
	if(database->execute(select, result))
	{
		OS_ASSERT(result.hasRow(0));
		m_totalResults = result.get(0, 0);

		if(m_totalResults > 0)	// In teoria non serve rieseguire la query se non ci sono risultati...
		{
			// Se è in groupMode, ed è il primo passaggio, controllo se è parziale.
			if( (query->getGroupMode()) && (objectsMap.empty()) )
				if( (searchLimit != 0) && (m_totalResults >= searchLimit) )
					m_partial = true;

			// Se non è in groupMode, o è il secondo passaggio, allora filtro per il pager.
			if( (!query->getGroupMode()) || (!objectsMap.empty()) )
			{
				uint32 limit=0;
				if( (query->hasLimit()) && (query->hasMaxResults()) )
					limit = std::min<uint32>(query->getLimit(), query->getMaxResults());
				else if(query->hasLimit())
					limit = query->getLimit();
				else if(query->hasMaxResults())
					limit = query->getMaxResults();

				if(limit != 0)
				{
					select->limit.setCount(limit);
					select->limit.setOffset(query->getOffset());
				}
			}
			else
			{
				if(searchLimit != 0)
				{
					// 0.12 RC5 - Altrimenti, setto cmq un massimo di risultati, come limite massimo ragionevole.
					select->limit.setCount(searchLimit);
				}
			}

			select->count = false;

			result.clear();
			if(database->execute(select, result))
			{
				for(uint32 r = 0; r < result.rows(); r++)
				{
					EntityID reference = static_cast<String>(result.get(r, DBTABLES::SNAPSHOT_OBJECTS::ENTITY)).to_ascii();
					if(query->getGroupMode())
					{
						shared_ptr<EntitiesEntity> entity = database->getPortal()->getEntity(database, reference);
						if( (entity != NULL) && (entity->getCurrent() != NULL) )
						{
							if(ObjectsSystem::instance()->getDescriptor(entity->getObjectType())->isGroupable())
								reference = static_cast<String>(result.get(r, DBTABLES::SNAPSHOT_OBJECTS::PARENT)).to_ascii();
						}
					}
					add(reference);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
