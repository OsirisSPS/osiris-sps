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

#ifndef _OS_HTML_HTMLSCRIPT_H
#define _OS_HTML_HTMLSCRIPT_H

#include "ihtmlcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlScript : public IHtmlControl
{
	typedef IHtmlControl ControlBase;

// Construction
public:
	HtmlScript(const String &code = String::EMPTY);
	virtual ~HtmlScript();

// Attributes
public:
	inline const String & getCode() const;
	inline void setCode(const String &code);

// Operations
public:
	static String linkCommand(const String &code);

// IHtmlControl overrides
public:
	virtual void renderAttributes(HtmlWriter &writer);

protected:
	virtual void onRender(HtmlWriter &writer);

protected:
	String m_code;
};

//////////////////////////////////////////////////////////////////////

inline const String & HtmlScript::getCode() const { return m_code; }
inline void HtmlScript::setCode(const String &code) { m_code = code; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLSCRIPT_H
