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

#ifndef _SEARCH_QUERY_H
#define _SEARCH_QUERY_H

#include "datetime.h"
#include "dbdb.h"
#include "enable_this_ptr.h"
#include "entitiesentities.h"
#include "base/object.h"
#include "objectsobjects.h"

#include "portalsportals.h"
#include "searchsearch.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class ISearchOptions;
class ISearchResults;
class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport SearchQuery : public Object,
								  public enable_this_ptr<SearchQuery>
{
// Types
public:
	typedef map<PortalObjectType, shared_ptr<ISearchOptions> >::type Options;
	typedef StringVector ParentsList;

// Construction
public:
	SearchQuery();
	virtual ~SearchQuery();

// Attributes
public:
	inline const String & getTemplateView() const;
	inline void setTemplateView(const String &templateName);

	inline const String & getTemplatePager() const;
	inline void setTemplatePager(const String &templateName);

	inline const String & getTemplateCustom() const;
	inline void setTemplateCustom(const String &templateCustom);

	// FullTextMode = true (Lucene only)
	inline const String & getText() const;
	inline void setText(const String &text);

	inline SearchTextMode getTextMode() const;
	inline void setTextMode(SearchTextMode searchMode);

	inline SearchTextWhere getTextWhere() const;
	inline void setTextWhere(SearchTextWhere searchWhere);

	// FullTextMode = false (Database only)
	inline const String & getByUser() const;
	inline void setByUser(const String &byUser);

	inline bool getShowDeleted() const;
	inline void setShowDeleted(bool showDeleted);

	inline const Options & getOptions() const;

	// Common mode (Lucene or Database)
	bool getFullTextMode() const;

	inline SearchOrderMode getOrderMode() const;
	inline void setOrderMode(SearchOrderMode orderMode);

	inline SearchOrderDirection getOrderDirection() const;
	inline void setOrderDirection(SearchOrderDirection orderDirection);

	inline SearchDateTime getFromSubmitDateType() const;
	inline void setFromSubmitDateType(SearchDateTime fromDateType);

	inline const DateTime & getFromSubmitDateSpecific() const;
	inline void setFromSubmitDateSpecific(const DateTime &fromDate);

	inline SearchDateTime getToSubmitDateType() const;
	inline void setToSubmitDateType(SearchDateTime toDateType);

	inline const DateTime & getToSubmitDateSpecific() const;
	inline void setToSubmitDateSpecific(const DateTime &toDate);

	inline SearchDateTime getFromInsertDateType() const;
	inline void setFromInsertDateType(SearchDateTime fromDateType);

	inline const DateTime & getFromInsertDateSpecific() const;
	inline void setFromInsertDateSpecific(const DateTime &fromDate);

	inline SearchDateTime getToInsertDateType() const;
	inline void setToInsertDateType(SearchDateTime toDateType);

	inline const DateTime & getToInsertDateSpecific() const;
	inline void setToInsertDateSpecific(const DateTime &toDate);

	inline const ParentsList & getParentsList() const;
	inline bool getParentsRecursive() const;
	inline void setParentsRecursive(bool parentsRecursive);
	String getParentsListString() const;
	void setParentsListString(const String &parentsList);

	inline bool getTreeMode() const;
	inline void setTreeMode(bool treeMode);

	inline bool getGroupMode() const;
	inline void setGroupMode(bool groupMode);

	inline uint32 getOffset() const;
	inline void setOffset(uint32 offset);
	inline bool hasOffset() const;

	inline uint32 getLimit() const;
	inline void setLimit(uint32 limit);
	inline bool hasLimit() const;

	// Note the differences between 'limit' and 'maxresults':
	// Minimum level are used for extracting,
	// But limit identify the pager size, maxresults the maximum results.
	// Think 100 possible results, with limit=10 and maxresults=5:
	// - We extract only 5 results, and no-pager are showed.
	// - Can't be only one param, because we need to know if max 5 results are expected, or
	//   if we want only first 5 results for a page/pager.
	inline uint32 getMaxResults() const;
	inline void setMaxResults(uint32 maxResults);
	inline bool hasMaxResults() const;

	inline uint32 getPagerSize() const;
	inline void setPagerSize(uint32 pagerSize);
	inline bool hasPagerSize() const;

	inline bool getShowInfo() const;
	inline void setShowInfo(bool showInfo);

	inline bool getShowFilterAlphabetic() const;
	inline void setShowFilterAlphabetic(bool showFilterAlphabetic);

	inline const String & getRssUrl() const;
	inline void setRssUrl(const String &rssUrl);

	// 1.0 - L'idea è quella di riusare il giro anche per mostrare oggetti direttamente, ad esempio gli oggetti in trash.
	inline String getSpecialQuery() const;
	inline void setSpecialQuery(const String &name);
	
// Operations
public:
	DateTime convertSearchDateTime(SearchDateTime type, const DateTime &dt);

	shared_ptr<ISearchResults> search(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal);

	void readXml(shared_ptr<XMLNode> node);
	void writeXml(shared_ptr<XMLNode> node);

	void computeParentsList(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, StringList &out) const;

	shared_ptr<ISearchOptions> getOption(const PortalObjectType type);

	static shared_ptr<SearchQuery> getSections();
	static shared_ptr<SearchQuery> getForums();

protected:
	void computeParentsList(shared_ptr<IPortalDatabase> database, shared_ptr<Portal> portal, const shared_ptr<EntitiesEntity> entity, StringList &out) const;

private:
	String m_templateView;
	String m_templatePager;
	String m_templateCustom;
	String m_text;
	SearchTextMode m_textMode;
	SearchTextWhere m_textWhere;

	String m_byUser;
	bool m_fullTextMode;
	SearchOrderMode m_orderMode;
	SearchOrderDirection m_orderDirection;
	SearchDateTime m_fromSubmitDateType;
	SearchDateTime m_toSubmitDateType;
	SearchDateTime m_fromInsertDateType;
	SearchDateTime m_toInsertDateType;
	DateTime m_fromSubmitDateSpecific;
	DateTime m_toSubmitDateSpecific;
	DateTime m_fromInsertDateSpecific;
	DateTime m_toInsertDateSpecific;
	bool m_treeMode;
	bool m_groupMode;
	uint32 m_offset;
	uint32 m_limit;
	uint32 m_maxResults;
	uint32 m_pagerSize;
	ParentsList m_parentsList;
	bool m_parentsRecursive;
	bool m_showDeleted;
	bool m_showInfo;
	bool m_showFilterAlphabetic;
	String m_rssUrl;
	String m_specialQuery;	
	Options m_options;
};

//////////////////////////////////////////////////////////////////////

inline const String & SearchQuery::getTemplateView() const { return m_templateView; }
inline void SearchQuery::setTemplateView(const String &templateName) { m_templateView = templateName; }

inline const String & SearchQuery::getTemplatePager() const { return m_templatePager; }
inline void SearchQuery::setTemplatePager(const String &templateName) { m_templatePager = templateName; }

inline const String & SearchQuery::getTemplateCustom() const { return m_templateCustom; }
inline void SearchQuery::setTemplateCustom(const String &templateCustom) { m_templateCustom = templateCustom; }

inline const String & SearchQuery::getText() const { return m_text; }
inline void SearchQuery::setText(const String &text) { m_text = text; }

inline SearchTextMode SearchQuery::getTextMode() const { return m_textMode; }
inline void SearchQuery::setTextMode(SearchTextMode textMode) { m_textMode = textMode; }

inline SearchTextWhere SearchQuery::getTextWhere() const { return m_textWhere; }
inline void SearchQuery::setTextWhere(SearchTextWhere textWhere) { m_textWhere = textWhere; }

inline const String & SearchQuery::getByUser() const { return m_byUser; }
inline void SearchQuery::setByUser(const String &byUser) { m_byUser = byUser; }

inline bool SearchQuery::getShowDeleted() const { return m_showDeleted; }
inline void SearchQuery::setShowDeleted(bool showDeleted) { m_showDeleted = showDeleted; }

inline const SearchQuery::Options & SearchQuery::getOptions() const { return m_options; }

inline SearchOrderMode SearchQuery::getOrderMode() const { return m_orderMode; }
inline void SearchQuery::setOrderMode(SearchOrderMode orderMode) { m_orderMode = orderMode; }

inline SearchOrderDirection SearchQuery::getOrderDirection() const { return m_orderDirection; }
inline void SearchQuery::setOrderDirection(SearchOrderDirection orderDirection) { m_orderDirection = orderDirection; }

inline SearchDateTime SearchQuery::getFromSubmitDateType() const { return m_fromSubmitDateType; }
inline void SearchQuery::setFromSubmitDateType(SearchDateTime fromDateType) { m_fromSubmitDateType = fromDateType; }

inline const DateTime & SearchQuery::getFromSubmitDateSpecific() const { return m_fromSubmitDateSpecific; }
inline void SearchQuery::setFromSubmitDateSpecific(const DateTime &fromSubmitDate) { m_fromSubmitDateSpecific = fromSubmitDate; }

inline SearchDateTime SearchQuery::getToSubmitDateType() const { return m_toSubmitDateType; }
inline void SearchQuery::setToSubmitDateType(SearchDateTime toDateType) { m_toSubmitDateType = toDateType; }

inline const DateTime & SearchQuery::getToSubmitDateSpecific() const { return m_toSubmitDateSpecific; }
inline void SearchQuery::setToSubmitDateSpecific(const DateTime &toSubmitDate) { m_toSubmitDateSpecific = toSubmitDate; }

inline SearchDateTime SearchQuery::getFromInsertDateType() const { return m_fromInsertDateType; }
inline void SearchQuery::setFromInsertDateType(SearchDateTime fromDateType) { m_fromInsertDateType = fromDateType; }

inline const DateTime & SearchQuery::getFromInsertDateSpecific() const { return m_fromInsertDateSpecific; }
inline void SearchQuery::setFromInsertDateSpecific(const DateTime &fromInsertDate) { m_fromInsertDateSpecific = fromInsertDate; }

inline SearchDateTime SearchQuery::getToInsertDateType() const { return m_toInsertDateType; }
inline void SearchQuery::setToInsertDateType(SearchDateTime toDateType) { m_toInsertDateType = toDateType; }

inline const DateTime & SearchQuery::getToInsertDateSpecific() const { return m_toInsertDateSpecific; }
inline void SearchQuery::setToInsertDateSpecific(const DateTime &toInsertDate) { m_toInsertDateSpecific = toInsertDate; }

inline const SearchQuery::ParentsList & SearchQuery::getParentsList() const { return m_parentsList; }
inline bool SearchQuery::getParentsRecursive() const { return m_parentsRecursive; }
inline void SearchQuery::setParentsRecursive(bool parentsRecursive) { m_parentsRecursive = parentsRecursive; }

inline bool SearchQuery::getTreeMode() const { return m_treeMode; }
inline void SearchQuery::setTreeMode(bool treeMode) { m_treeMode = treeMode; }

inline bool SearchQuery::getGroupMode() const { return m_groupMode; }
inline void SearchQuery::setGroupMode(bool groupMode) { m_groupMode = groupMode; }

inline bool SearchQuery::getShowInfo() const { return m_showInfo; }
inline void SearchQuery::setShowInfo(const bool showInfo) { m_showInfo = showInfo; }

inline bool SearchQuery::getShowFilterAlphabetic() const { return m_showFilterAlphabetic; }
inline void SearchQuery::setShowFilterAlphabetic(const bool showFilterAlphabetic) { m_showFilterAlphabetic = showFilterAlphabetic; }

inline const String & SearchQuery::getRssUrl() const { return m_rssUrl; }
inline void SearchQuery::setRssUrl(const String &rssUrl) { m_rssUrl = rssUrl; }

inline uint32 SearchQuery::getOffset() const { return m_offset; }
inline void SearchQuery::setOffset(uint32 offset) { m_offset = offset; }
inline bool SearchQuery::hasOffset() const { return m_offset != 0; }

inline uint32 SearchQuery::getLimit() const { return m_limit; }
inline void SearchQuery::setLimit(uint32 limit) { m_limit = limit; }
inline bool SearchQuery::hasLimit() const { return m_limit != 0; }

inline uint32 SearchQuery::getMaxResults() const { return m_maxResults; }
inline void SearchQuery::setMaxResults(uint32 maxResults) { m_maxResults = maxResults; }
inline bool SearchQuery::hasMaxResults() const { return m_maxResults != 0; }

inline uint32 SearchQuery::getPagerSize() const { return m_pagerSize; }
inline void SearchQuery::setPagerSize(uint32 pagerSize) { m_pagerSize = pagerSize; }
inline bool SearchQuery::hasPagerSize() const { return m_pagerSize != 0; }

inline String SearchQuery::getSpecialQuery() const { return m_specialQuery; }
inline void SearchQuery::setSpecialQuery(const String &name) { m_specialQuery = name; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _SEARCH_QUERY_H
