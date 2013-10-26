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

#ifndef _OS_ENGINE_IDEPICKERSELECT_H
#define _OS_ENGINE_IDEPICKERSELECT_H

#include "ideportalcontrol.h"
#include "htmldiv.h"
#include "entitiesentities.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlTextBox;

//////////////////////////////////////////////////////////////////////

class EngineExport IdePickerSelect : public IPageControl<HtmlDiv>
{
	typedef IPageControl<HtmlDiv> ControlBase;

// Construction
public:
	IdePickerSelect();
	virtual ~IdePickerSelect();

// Attributes
public:
	virtual String getValue() const;
	virtual void setValue(const String &val);

	inline virtual String getTemplateName() const;
	inline virtual void setTemplateName(const String &templateName);

// Events
private:
	
// Operations
public:
	String getKey(const String &key);
	String getKeyHref(const String &key);
		
// IControl interface
public:
	virtual void onInit();
	virtual void onPreRender();
	
protected:
	shared_ptr<HtmlTextBox> m_input;
	//shared_ptr<HtmlXSLControl> m_template;
	shared_ptr<XMLNode> m_nodeRoot;

	String m_templateName;
};

//////////////////////////////////////////////////////////////////////
 
String IdePickerSelect::getTemplateName() const { return m_templateName; }
void IdePickerSelect::setTemplateName(const String &templateName) { m_templateName = templateName; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_IDEPICKERSELECT_H
