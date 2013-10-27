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

#ifndef _OS_HTML_HTMLINK_H
#define _OS_HTML_HTMLINK_H

#include "ihtmlcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlText;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlLink : public IHtmlControl
{
	typedef IHtmlControl ControlBase;

// Construction
public:
	HtmlLink(const String &caption = String::EMPTY, const String &href = String::EMPTY);
	virtual ~HtmlLink();

// Attributes
public:
	inline const String & getHref() const;
	inline void setHref(const String &href);

	const String & getCaption() const;
	void setCaption(const String &caption);

// IHtmlControl overrides
public:
	virtual void renderAttributes(HtmlWriter &writer);

protected:
	virtual void onInit();
	virtual void onRender(HtmlWriter &writer);

protected:
	String m_href;
	shared_ptr<HtmlText> m_caption;
};

//////////////////////////////////////////////////////////////////////

inline const String & HtmlLink::getHref() const { return m_href; }
inline void HtmlLink::setHref(const String &href) { m_href = href; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLINK_H
