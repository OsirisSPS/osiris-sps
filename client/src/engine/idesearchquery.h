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

#ifndef _OS_IDE_SEARCHQUERY
#define _OS_IDE_SEARCHQUERY

#include "iidexslrenderer.h"
#include "ideportalcontrol.h"
#include "searchsearch.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlTextBox;
class IdeSearchGlobalParams;
class IdeSearchResults;
class SearchQuery;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeSearchQuery : public IXSLRenderer<IPortalPageControl<IHtmlControl> >
{
	typedef IXSLRenderer<IPortalPageControl<IHtmlControl> > ControlBase;
	
// Construction
public:
	//SearchResults(const String &title, search::query_ptr query = null, search::results_ptr results = null);
	IdeSearchQuery(shared_ptr<SearchQuery> query = null);
	virtual ~IdeSearchQuery();

// Attributes
public:
	inline shared_ptr<SearchQuery> getQuery() const;
	inline void setQuery(shared_ptr<SearchQuery> query);

	inline bool getShowParams() const;
	inline void setShowParams(const bool showParams);

	inline bool getDirectRun() const;
	inline void setDirectRun(const bool directRun);

// Operations
public:
	void doFeed(const String& title, const String& description, const String &body, const String& link, const String& language);

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// IXSLRenderer overrides
protected:
	virtual void onInit();
	virtual void onLoad();
	virtual void onPreRender();

protected:
	shared_ptr<SearchQuery> m_query; 		// Query di ricerca
	
	shared_ptr<IdeSearchGlobalParams> m_globalParams;
	shared_ptr<IdeSearchResults> m_results;

private:
	void doSearch();	
	void onChangeOffset(IEvent *e);
	void onFilterAlphabetic(IEvent *e);

	bool m_showParams;
	bool m_directRun;

	shared_ptr<HtmlTextBox> m_filterAlphabetic;		

	static const String EVENT_ONSEARCH;		
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<SearchQuery> IdeSearchQuery::getQuery() const { return m_query; }
inline void IdeSearchQuery::setQuery(shared_ptr<SearchQuery> query) { m_query = query; }

inline bool IdeSearchQuery::getShowParams() const { return m_showParams; }
inline void IdeSearchQuery::setShowParams(const bool showParams) { m_showParams = showParams; }

inline bool IdeSearchQuery::getDirectRun() const { return m_directRun; }
inline void IdeSearchQuery::setDirectRun(const bool directRun) { m_directRun = directRun; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_SEARCHQUERY
