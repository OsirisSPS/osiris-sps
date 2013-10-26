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

#ifndef _OS_IDE_POSITIONEDITOR_H
#define _OS_IDE_POSITIONEDITOR_H

#include "ideportalcontrol.h"
#include "htmldiv.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlComboBox;

//////////////////////////////////////////////////////////////////////

class EngineExport IdePositionEditor : public IPortalPageControl<HtmlDiv>
{
	typedef IPortalPageControl<HtmlDiv> ControlBase;

// Construction
public:
	IdePositionEditor();
	virtual ~IdePositionEditor();

// Attributes
public:
	double getPosition() const;
	void setPosition(shared_ptr<EntitiesEntity> parent, const double position);

// IHtmlControl overrides
protected:
	virtual void onInit();
	
protected:
	shared_ptr<HtmlComboBox> m_positions;

	double m_currentPosition;
	shared_ptr<EntitiesEntity> m_parent;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_POSITIONEDITOR_H
