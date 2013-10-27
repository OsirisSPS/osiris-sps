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

#ifndef _OS_HTML_HTMLATTRIBUTE_H
#define _OS_HTML_HTMLATTRIBUTE_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IHtmlControl;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlAttribute : public Object
{
// Construction
public:
	HtmlAttribute(IHtmlControl &control, const String &name, const String &value);
	virtual ~HtmlAttribute();

// Attributes
public:
	inline IHtmlControl & getControl();
	inline const IHtmlControl & getControl() const;

	inline const String & getName() const;
	inline void setName(const String &name);

	inline const String & getValue() const;
	inline void setValue(const String &value);

protected:
	IHtmlControl &m_control;	// Controllo di riferimento
	String m_name;				// Nome dell'attributo
	String m_value;				// Valore dell'attributo
};

//////////////////////////////////////////////////////////////////////

inline IHtmlControl & HtmlAttribute::getControl() { return m_control; }
inline const IHtmlControl & HtmlAttribute::getControl() const { return m_control; }

inline const String & HtmlAttribute::getName() const { return m_name; }
inline void HtmlAttribute::setName(const String &name) { m_name = name; }

inline const String & HtmlAttribute::getValue() const { return m_value; }
inline void HtmlAttribute::setValue(const String &value) { m_value = value; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLATTRIBUTE_H
