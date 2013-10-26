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

#ifndef _OS_ENGINE_IDEPICKERCULTURE_H
#define _OS_ENGINE_IDEPICKERCULTURE_H

#include "ideportalcontrol.h"
#include "htmldiv.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlCheckBox;
class HtmlTextBox;
class LanguageCulture;

//////////////////////////////////////////////////////////////////////

class EngineExport IdePickerCulture : public IPageControl<HtmlDiv>
{
	typedef IPageControl<HtmlDiv> ControlBase;
	
// Construction
public:
	IdePickerCulture();
	virtual ~IdePickerCulture();

// Attributes:
public:
	shared_ptr<EventSource> getEventSelect() const;

	virtual String getValue() const;
	virtual void setValue(const String &id);

	inline bool getShowSystem() const;
	inline void setShowSystem(const bool showSystem);

// Operations:
protected:
	void addCulture(shared_ptr<XMLNode> node, shared_ptr<LanguageCulture> culture, const String &current, uint32 level);
	
// IControl overrides		
protected:
	virtual void onLoad();	
	virtual void onPreRender();

protected:
	bool m_showSystem;
	shared_ptr<HtmlCheckBox> m_showAll;
	shared_ptr<HtmlTextBox> m_input;
	shared_ptr<XMLNode> m_nodeRoot;

	void onSelect(IEvent *e);

public:
	static const String EVENT_ONSELECT;	
};

//////////////////////////////////////////////////////////////////////

inline bool IdePickerCulture::getShowSystem() const { return m_showSystem; }
inline void IdePickerCulture::setShowSystem(const bool showSystem) { m_showSystem = showSystem; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_IDEPICKERCULTURE_H
