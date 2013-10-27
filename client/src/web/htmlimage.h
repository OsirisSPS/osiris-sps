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

#ifndef _OS_HTML_HTMLIMAGE_H
#define _OS_HTML_HTMLIMAGE_H

#include "ihtmlcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlImage : public IHtmlControl
{
	typedef IHtmlControl ControlBase;

// Construction
public:
	HtmlImage(const String &url = String::EMPTY);
	virtual ~HtmlImage();

// Attributes
public:
	inline const String & getUrl() const;
	inline void setUrl(const String &imageUrl);

	inline const String & getTitle() const;
	inline void setTitle(const String &title);

// IHtmlControl overrides
public:
	virtual void renderAttributes(HtmlWriter &writer);

protected:
	virtual void onRender(HtmlWriter &writer);

protected:
	String m_url;
	String m_title;
};

//////////////////////////////////////////////////////////////////////

inline const String & HtmlImage::getUrl() const { return m_url; }
inline void HtmlImage::setUrl(const String &url) { m_url = url; }

inline const String & HtmlImage::getTitle() const { return m_title; }
inline void HtmlImage::setTitle(const String &title) { m_title = title; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLIMAGE_H
