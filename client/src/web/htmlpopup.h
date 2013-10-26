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

#ifndef _OS_HTML_HTMLPOPUP_H
#define _OS_HTML_HTMLPOPUP_H

#include "htmldiv.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlPopup : public HtmlDiv
{
	typedef HtmlDiv ControlBase;

// Enumerated types
public:
	enum Type
	{
		ptStatic	  = 0,
		ptStaticClick = 1,
		ptTooltip	  = 2,
		ptPopup		  = 3,
	};

// Construction
public:
	HtmlPopup();
	virtual ~HtmlPopup();

// Attributes
public:
	inline Type getType() const;
	inline void setType(Type type);

// Operations
public:
	shared_ptr<HtmlDiv> getHeader();
	shared_ptr<HtmlDiv> getBody();

protected:
	virtual void onPreRender();

protected:
	Type m_type;
	shared_ptr<HtmlDiv> m_header;
	shared_ptr<HtmlDiv> m_body;
};

//////////////////////////////////////////////////////////////////////

inline HtmlPopup::Type HtmlPopup::getType() const { return m_type; }
inline void HtmlPopup::setType(Type type) { m_type = type; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLPOPUP_H
