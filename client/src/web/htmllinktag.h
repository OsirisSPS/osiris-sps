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

#ifndef _OS_HTML_HTMLLINKTAG_H
#define _OS_HTML_HTMLLINKTAG_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlWriter;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlLinkTag : public Object
{
// Construction
public:
	HtmlLinkTag();
	virtual ~HtmlLinkTag();

// Attributes
public:
	inline const String & getRel() const;
	inline void setRel(const String &rel);

	inline const String & getHref() const;
	inline void setHref(const String &href);

	inline const String & getType() const;
	inline void setType(const String &type);

	inline const String & getTitle() const;
	inline void setTitle(const String &title);

	inline const String & getHreflang() const;
	inline void setHreflang(const String &hreflang);

// Operations
public:
	virtual void render(HtmlWriter &writer);

public:
	static shared_ptr<HtmlLinkTag> create(const String &rel, const String &href);	

protected:
	String m_rel;
	String m_href;
	String m_type;
	String m_title;
	String m_hreflang;
};

//////////////////////////////////////////////////////////////////////

inline const String & HtmlLinkTag::getRel() const { return m_rel; }
inline void HtmlLinkTag::setRel(const String &rel) { m_rel = rel; }

inline const String & HtmlLinkTag::getHref() const { return m_href; }
inline void HtmlLinkTag::setHref(const String &href) { m_href = href; }

inline const String & HtmlLinkTag::getType() const { return m_type; }
inline void HtmlLinkTag::setType(const String &type) { m_type = type; }

inline const String & HtmlLinkTag::getTitle() const { return m_title; }
inline void HtmlLinkTag::setTitle(const String &title) { m_title = title; }

inline const String & HtmlLinkTag::getHreflang() const { return m_hreflang; }
inline void HtmlLinkTag::setHreflang(const String &hreflang) { m_hreflang = hreflang; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLMETATAG_H
