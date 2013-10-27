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

#ifndef _OS_IDE_SEARCHGLOBALPARAMS
#define _OS_IDE_SEARCHGLOBALPARAMS

#include "iidepageable.h"
#include "iidexslrenderer.h"
#include "ideportalcontrol.h"
#include "searchsearch.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlTextBox;
class HtmlComboBox;
class IdePickerBool;
class IdePickerDateTimeSpecial;
class SearchQuery;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeSearchGlobalParams : public IXSLRenderer<IPortalPageControl<IHtmlControl> >
{
	typedef IXSLRenderer<IPortalPageControl<IHtmlControl> > ControlBase;

// Construction
public:
	IdeSearchGlobalParams(shared_ptr<SearchQuery> query = null);
	virtual ~IdeSearchGlobalParams();

// Attributes
public:
	inline shared_ptr<SearchQuery> getQuery() const;
	inline void setQuery(shared_ptr<SearchQuery> query);

// Operations
public:
	void fillQuery();

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

	shared_ptr<HtmlTextBox> m_text;
	shared_ptr<HtmlComboBox> m_textMode;
	shared_ptr<HtmlComboBox> m_textWhere;
	shared_ptr<HtmlComboBox> m_orderMode;
	shared_ptr<HtmlComboBox> m_orderDirection;
	shared_ptr<IdePickerDateTimeSpecial> m_fromSubmitDate;
	shared_ptr<IdePickerDateTimeSpecial> m_toSubmitDate;
	shared_ptr<IdePickerDateTimeSpecial> m_fromInsertDate;
	shared_ptr<IdePickerDateTimeSpecial> m_toInsertDate;
	shared_ptr<HtmlTextBox> m_user;
	shared_ptr<HtmlTextBox> m_parentsList;
	shared_ptr<IdePickerBool> m_parentsRecursive;
	shared_ptr<IdePickerBool> m_treeMode;
	shared_ptr<IdePickerBool> m_groupMode;
	shared_ptr<IdePickerBool> m_showDeleted;
	shared_ptr<IdePickerBool> m_showInfo;
	shared_ptr<HtmlComboBox> m_templateView;
	shared_ptr<HtmlComboBox> m_templatePager;
	shared_ptr<IdePickerBool> m_showFilterAlphabetic;
	shared_ptr<HtmlTextBox> m_maxResults;
	shared_ptr<HtmlTextBox> m_pagerSize;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<SearchQuery> IdeSearchGlobalParams::getQuery() const { return m_query; }
inline void IdeSearchGlobalParams::setQuery(shared_ptr<SearchQuery> query) { m_query = query; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_SEARCHGLOBALPARAMS
