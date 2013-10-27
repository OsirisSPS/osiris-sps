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

#ifndef _OS_IDE_SEARCHRESULTS
#define _OS_IDE_SEARCHRESULTS

#include "iidepageable.h"
#include "iidexslrenderer.h"
#include "ideportalcontrol.h"
#include "searchsearch.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ISearchResults;
class SearchQuery;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeSearchResults :	public IXSLRenderer<IPortalPageControl<IHtmlControl> >,
										public IIdePageable
{
	typedef IXSLRenderer<IPortalPageControl<IHtmlControl> > ControlBase;

// Construction
public:
	IdeSearchResults(shared_ptr<SearchQuery> query = null, shared_ptr<ISearchResults> results = null);
	virtual ~IdeSearchResults();

// Attributes
public:
	inline shared_ptr<SearchQuery> getQuery() const;
	inline void setQuery(shared_ptr<SearchQuery> query);

	inline shared_ptr<ISearchResults> getResults() const;
	inline void setResults(shared_ptr<ISearchResults> results);

	shared_ptr<EventSource> getEventChangeOffset() const;

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// IXSLRenderer overrides
protected:
	virtual void onPreRender();

// Pager interface
public:
	virtual String getPageHref(uint32 offset) const;

protected:
	shared_ptr<SearchQuery> m_query; 		// Query di ricerca
	shared_ptr<ISearchResults> m_results;	// Risultati di ricerca

public:
	static const String EVENT_ONCHANGEOFFSET;
	static const String EVENT_ONFILTERALPHABETIC;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<SearchQuery> IdeSearchResults::getQuery() const { return m_query; }
inline void IdeSearchResults::setQuery(shared_ptr<SearchQuery> query) { m_query = query; }

inline shared_ptr<ISearchResults> IdeSearchResults::getResults() const { return m_results; }
inline void IdeSearchResults::setResults(shared_ptr<ISearchResults> results) { m_results = results; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_SEARCHRESULTS
