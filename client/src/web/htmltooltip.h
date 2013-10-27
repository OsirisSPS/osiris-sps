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

#ifndef _OS_HTML_HTMLTOOLTIP_H
#define _OS_HTML_HTMLTOOLTIP_H

#include "htmldiv.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlTooltip : public HtmlDiv
{
	typedef HtmlDiv ControlBase;

// Enumerated types
public:
	enum Type
	{
		ttInfo		= 0,
		ttWarning	= 1,
	};

// Construction
public:
	HtmlTooltip();
	virtual ~HtmlTooltip();

// Attributes
public:
	inline Type getType() const;
	inline void setType(Type type);

	inline shared_ptr<IHtmlControl> getConnect() const;
	inline void setConnect(shared_ptr<IHtmlControl> connect);

	inline String getValue() const;
	inline void setValue(const String &value);

// Operations
public:

protected:
	virtual void onLoad();
	virtual void onPreRender();

protected:
	Type m_type;
	shared_ptr<IHtmlControl> m_connect;
	String m_value;
};

//////////////////////////////////////////////////////////////////////

inline HtmlTooltip::Type HtmlTooltip::getType() const { return m_type; }
inline void HtmlTooltip::setType(Type type) { m_type = type; }

inline shared_ptr<IHtmlControl> HtmlTooltip::getConnect() const { return m_connect; }
inline void HtmlTooltip::setConnect(shared_ptr<IHtmlControl> connect) { m_connect = connect; }

inline String HtmlTooltip::getValue() const { return m_value; }
inline void HtmlTooltip::setValue(const String &value) { m_value = value; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLTOOLTIP_H
