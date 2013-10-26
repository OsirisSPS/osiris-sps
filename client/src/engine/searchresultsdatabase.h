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

#ifndef _SEARCH_RESULTSDATABASE_H
#define _SEARCH_RESULTSDATABASE_H


#include "portalsportals.h"
#include "isearchresults.h"
#include "searchsearch.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport SearchResultsDatabase : public ISearchResults
{
	typedef vector<shared_ptr<SearchResult> >::type Objects;

// Construction
public:
	SearchResultsDatabase();
	virtual ~SearchResultsDatabase();

// Operations
public:
	void add(const EntityID &id);

// IResults overrides
public:
	virtual void clear();

	virtual shared_ptr<SearchResult> getResult(shared_ptr<IPortalDatabase> database, uint32 index, bool asGroup = false);
	virtual uint32 getResults();
	virtual uint32 getTotalResults();

protected:
	virtual void executeQuery(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, shared_ptr<SearchQuery> query, const StringMap& objectsMap);

protected:
	Objects m_objects;
	uint32 m_totalResults;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _SEARCH_RESULTSDATABASE_H
