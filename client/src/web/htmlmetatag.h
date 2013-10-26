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

#ifndef _OS_HTML_HTMLMETATAG_H
#define _OS_HTML_HTMLMETATAG_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlWriter;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlMetaTag : public Object
{
// Construction
public:
	HtmlMetaTag();
	virtual ~HtmlMetaTag();

// Attributes
public:
	inline const String & getName() const;
	inline void setName(const String &name);

	inline const String & getHttpEquiv() const;
	inline void setHttpEquiv(const String &httpEquiv);

	inline const String & getContent() const;
	inline void setContent(const String &content);

	inline const String & getLang() const;
	inline void setLang(const String &lang);

	inline const String & getUrl() const;
	inline void setUrl(const String &url);

// Operations
public:
	virtual void render(HtmlWriter &writer);

public:
	static shared_ptr<HtmlMetaTag> createByName(const String &name, const String &content = String::EMPTY);
	static shared_ptr<HtmlMetaTag> createByHttpEquiv(const String &httpEquiv, const String &content = String::EMPTY);

protected:
	String m_name;
	String m_httpEquiv;
	String m_content;
	String m_lang;
	String m_url;
};

//////////////////////////////////////////////////////////////////////

inline const String & HtmlMetaTag::getName() const { return m_name; }
inline void HtmlMetaTag::setName(const String &name) { m_name = name; }

inline const String & HtmlMetaTag::getHttpEquiv() const { return m_httpEquiv; }
inline void HtmlMetaTag::setHttpEquiv(const String &httpEquiv) { m_httpEquiv = httpEquiv; }

inline const String & HtmlMetaTag::getContent() const { return m_content; }
inline void HtmlMetaTag::setContent(const String &content) { m_content = content; }

inline const String & HtmlMetaTag::getLang() const { return m_lang; }
inline void HtmlMetaTag::setLang(const String &lang) { m_lang = lang; }

inline const String & HtmlMetaTag::getUrl() const { return m_url; }
inline void HtmlMetaTag::setUrl(const String &url) { m_url = url; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLMETATAG_H
