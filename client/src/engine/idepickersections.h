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

#ifndef _OS_ENGINE_IDEPICKERSECTIONS_H
#define _OS_ENGINE_IDEPICKERSECTIONS_H

#include "ideportalcontrol.h"
#include "idepickerselect.h"
#include "entitiesentities.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

class EngineExport IdePickerSections : public IdePickerSelect
{
	typedef IdePickerSelect ControlBase;

// Construction
public:
	IdePickerSections();
	virtual ~IdePickerSections();

// Attributes
public:	
	inline bool getShowSystem() const;
	inline void setShowSystem(const bool showSystem);
	
// Events
private:	

// Operations
private:

	void addSkin(shared_ptr<IdeSkin> skin);
	
// IControl interface
public:
	virtual void onLoad();
	virtual void onPreRender();
	
protected:
	bool m_showSystem;	
};

//////////////////////////////////////////////////////////////////////

inline bool IdePickerSections::getShowSystem() const { return m_showSystem; }
inline void IdePickerSections::setShowSystem(const bool showSystem) { m_showSystem = showSystem; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_IDEPICKERSECTIONS_H
