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

#ifndef _OS_HTML_HTMLTAB_H
#define _OS_HTML_HTMLTAB_H

#include "htmldiv.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlTab : public HtmlDiv
{
	typedef HtmlDiv ControlBase;

// Enumerated types
public:
	enum Type
	{
		ttLeft	 = 0,
		ttTop	 = 1,
		ttRight	 = 2,
		ttBottom = 3,
	};

// Construction
public:
	HtmlTab();
	virtual ~HtmlTab();

// Attributes
public:
	inline Type getType() const;
	inline void setType(Type type);

	inline const String & getCssTable() const;
	inline void setCssTable(const String &cssTable);

	inline const String & getCssLink() const;
	inline void setCssLink(const String &cssLink);

	inline const String & getCssLinkHover() const;
	inline void setCssLinkHover(const String &cssLinkHover);

// Operations
public:
	void addPage(shared_ptr<IHtmlControl> page, const String &title);

protected:
	virtual void onPreRender();

protected:
	Type m_type;
	String m_cssTable;
	String m_cssLink;
	String m_cssLinkHover;
};

//////////////////////////////////////////////////////////////////////

inline HtmlTab::Type HtmlTab::getType() const { return m_type; }
inline void HtmlTab::setType(Type type) { m_type = type; }

inline const String & HtmlTab::getCssTable() const { return m_cssTable; }
inline void HtmlTab::setCssTable(const String &cssTable) { m_cssTable=cssTable; }

inline const String & HtmlTab::getCssLink() const { return m_cssLink; }
inline void HtmlTab::setCssLink(const String &cssLink) { m_cssLink=cssLink; }

inline const String & HtmlTab::getCssLinkHover() const { return m_cssLinkHover; }
inline void HtmlTab::setCssLinkHover(const String &cssLinkHover) { m_cssLinkHover=cssLinkHover; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLTAB_H
