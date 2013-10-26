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

#ifndef _OS_HTML_HTMLOPTIONGROUP_H
#define _OS_HTML_HTMLOPTIONGROUP_H

#include "ihtmlcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlOption;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlOptionGroup : public IHtmlControl
{
	typedef IHtmlControl ControlBase;

// Construction
public:
	HtmlOptionGroup(const String &label = String::EMPTY);
	virtual ~HtmlOptionGroup();

// Attributes
public:
	inline const String & getLabel() const;
	inline void setLabel(const String &label);

	shared_ptr<HtmlOption> getSelected() const;
	void setSelectedByValue(const String &value);

// Operations
public:
	shared_ptr<HtmlOption> addOption(const String &label = String::EMPTY, const String &value = String::EMPTY);

// IHtmlControl overrides
public:
	virtual void renderAttributes(HtmlWriter &writer);

protected:
	virtual void onRender(HtmlWriter &writer);

protected:
	String m_label;
};

//////////////////////////////////////////////////////////////////////

inline const String & HtmlOptionGroup::getLabel() const { return m_label; }
inline void HtmlOptionGroup::setLabel(const String &label) { m_label = label; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLOPTIONGROUP_H
