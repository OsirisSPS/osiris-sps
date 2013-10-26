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

#ifndef _OS_HTML_HTMLTABCTRL_H
#define _OS_HTML_HTMLTABCTRL_H

#include "ihtmlcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlDiv;
class HtmlImage;
class HtmlTabPage;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlTabCtrl : public IHtmlControl
{
	typedef IHtmlControl ControlBase;

// Construction
public:
	HtmlTabCtrl();
	virtual ~HtmlTabCtrl();

// Attributes
public:
	inline shared_ptr<HtmlDiv> getView() const;

	int32 getCurSel() const;
	int32 getPagesCount() const;
	String getPageLink(uint32 index);

// Operations
public:
	shared_ptr<HtmlTabPage> addPage(const String &caption = String::EMPTY, shared_ptr<HtmlImage> icon = null);
	shared_ptr<HtmlTabPage> addPage(const String &caption = String::EMPTY, const String &image_path = String::EMPTY);

// Overridables
protected:
	virtual shared_ptr<HtmlTabPage> createPage(int32 index) const;

// IHtmlControl overrides
protected:
	virtual void onInit();

protected:
	shared_ptr<HtmlDiv> m_pages;
	shared_ptr<HtmlDiv> m_view;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<HtmlDiv> HtmlTabCtrl::getView() const { return m_view; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLTABCTRL_H
