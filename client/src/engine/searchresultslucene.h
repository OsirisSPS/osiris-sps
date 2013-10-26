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

#ifndef _SEARCH_RESULTSLUCENE_H
#define _SEARCH_RESULTSLUCENE_H

#include "isearchresults.h"

#include "datetime.h"
#include "pimpl_ptr.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport SearchResultsLucene : public ISearchResults
{
	typedef ISearchResults ResultsBase;

// Construction
public:
	SearchResultsLucene();
	virtual ~SearchResultsLucene();

// Attributes
public:
	// Restituisce la query (espansa) utilizzata per l'estrazione dei risultati.
	String getQueryText() const;

// IResults overrides
public:
	//virtual void exportXML(shared_ptr<XMLNode> node, search::query_ptr query, shared_ptr<IPortalPage> page, const String &title);
	virtual void exportXML(shared_ptr<XMLNode> node, shared_ptr<SearchQuery> query, shared_ptr<IPortalPage> page, bool external);

// IResults overrides
public:
	virtual void clear();

	virtual shared_ptr<SearchResult> getResult(shared_ptr<IPortalDatabase> database, uint32 index, bool asGroup = false);
	virtual uint32 getResults();
	virtual uint32 getTotalResults();

protected:
	void _addDateRange(String &text, const String &field, const DateTime &from, const DateTime &to);
	void _addFieldIn(String &text, const String &field, const StringList &list);
	virtual void executeQuery(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, shared_ptr<SearchQuery> query, const StringMap& objectsMap);

private:
	struct impl;
	pimpl_ptr<impl> m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _SEARCH_RESULTSLUCENE_H
