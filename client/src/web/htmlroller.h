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

#ifndef _OS_HTML_HTMLROLLER_H
#define _OS_HTML_HTMLROLLER_H

#include "htmldiv.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlRoller : public HtmlDiv
{
	typedef HtmlDiv ControlBase;

// Construction
public:
	HtmlRoller();
	virtual ~HtmlRoller();

// Attributes
public:
	inline bool getAllowClose() const;
	inline void setAllowClose(bool allowClose);

	inline bool getStartClose() const;
	inline void setStartClose(bool startClose);

	inline const String & getCssTable() const;
	inline void setCssTable(const String &cssTable);

	inline const String & getCssHeaderOpen() const;
	inline void setCssHeaderOpen(const String &cssHeaderOpen);

	inline const String & getCssHeaderOpenHover() const;
	inline void setCssHeaderOpenHover(const String &cssHeaderOpenHover);

	inline const String & getCssHeaderClose() const;
	inline void setCssHeaderClose(const String &cssHeaderClose);

	inline const String & getCssHeaderCloseHover() const;
	inline void setCssHeaderCloseHover(const String &cssHeaderCloseHover);

// Operations
public:
	shared_ptr<HtmlDiv> getHeader();
	shared_ptr<HtmlDiv> getBody();

protected:
	virtual void onPreRender();

protected:
	shared_ptr<HtmlDiv> m_header;
	shared_ptr<HtmlDiv> m_body;

	bool m_allowClose;
	bool m_startClose;

	String m_cssTable;
	String m_cssHeaderOpen;
	String m_cssHeaderOpenHover;
	String m_cssHeaderClose;
	String m_cssHeaderCloseHover;
};

//////////////////////////////////////////////////////////////////////

inline bool HtmlRoller::getAllowClose() const { return m_allowClose; }
inline void HtmlRoller::setAllowClose(bool allowClose) { m_allowClose=allowClose; }

inline bool HtmlRoller::getStartClose() const { return m_startClose; }
inline void HtmlRoller::setStartClose(bool startClose) { m_startClose=startClose; }

inline const String & HtmlRoller::getCssTable() const { return m_cssTable; }
inline void HtmlRoller::setCssTable(const String &cssTable) { m_cssTable=cssTable; }

inline const String & HtmlRoller::getCssHeaderOpen() const { return m_cssHeaderOpen; }
inline void HtmlRoller::setCssHeaderOpen(const String &cssHeaderOpen) { m_cssHeaderOpen=cssHeaderOpen; }

inline const String & HtmlRoller::getCssHeaderOpenHover() const { return m_cssHeaderOpenHover; }
inline void HtmlRoller::setCssHeaderOpenHover(const String &cssHeaderOpenHover) { m_cssHeaderOpenHover=cssHeaderOpenHover; }

inline const String & HtmlRoller::getCssHeaderClose() const { return m_cssHeaderClose; }
inline void HtmlRoller::setCssHeaderClose(const String &cssHeaderClose) { m_cssHeaderClose=cssHeaderClose; }

inline const String & HtmlRoller::getCssHeaderCloseHover() const { return m_cssHeaderCloseHover; }
inline void HtmlRoller::setCssHeaderCloseHover(const String &cssHeaderCloseHover) { m_cssHeaderCloseHover=cssHeaderCloseHover; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLROLLER_H
