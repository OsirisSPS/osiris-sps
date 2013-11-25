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

#ifndef _OS_ENGINE_IDEPICKEROBJECTEX_H
#define _OS_ENGINE_IDEPICKEROBJECTEX_H

#include "ideportalcontrol.h"
#include "iidexslrenderer.h"
#include "entitiesentities.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport IdePickerObjectEx : public IXSLRenderer<IPortalPageControl<IHtmlControl> >
{
	typedef IXSLRenderer<IPortalPageControl<IHtmlControl> > ControlBase;

// Construction
public:
	IdePickerObjectEx();
	virtual ~IdePickerObjectEx();

// Attributes
public:
	inline bool getOnlyObjectsWithChilds() const;
	inline void setOnlyObjectsWithChilds(bool value);
	inline bool getMultipleSelection() const;
	inline void setMultipleSelection(bool value);
	virtual String getValue() const;
	virtual void setValue(const String &id);


// Events
private:
	void onOpen(IEvent *e);
	void onSelect(IEvent *e);

	static const String EVENT_OPEN;
	static const String EVENT_SELECT;

// Operations
private:
	void followParent(StringMap& opens, shared_ptr<EntitiesEntity> entity);
	void exportObject(StringMap& opens, shared_ptr<EntitiesEntity> entity, shared_ptr<XMLNode> nodeParent);

// IControl interface
	virtual void onLoad();
	virtual void onPreRender();
	virtual void onLoadViewState(const DataTree &state);
	virtual void onSaveViewState(DataTree &state);

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

protected:
	bool m_onlyObjectsWithChilds;
	bool m_multipleSelection;
	StringMap m_selected;
	StringMap m_opens;
};

//////////////////////////////////////////////////////////////////////

inline bool IdePickerObjectEx::getOnlyObjectsWithChilds() const { return m_onlyObjectsWithChilds; }
inline void IdePickerObjectEx::setOnlyObjectsWithChilds(bool value) { m_onlyObjectsWithChilds = value; }
inline bool IdePickerObjectEx::getMultipleSelection() const { return m_multipleSelection; }
inline void IdePickerObjectEx::setMultipleSelection(bool value) { m_multipleSelection = value; }

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_IDEPICKEROBJECTEX_H
