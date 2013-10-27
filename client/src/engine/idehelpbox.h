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

#ifndef _OS_IDE_HELPBOX_H
#define _OS_IDE_HELPBOX_H

#include "idecontrol.h"
#include "iidexslrenderer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport IdeHelpBox : public IXSLRenderer<IPageControl<IHtmlControl> >
{
	typedef IXSLRenderer<IPageControl<IHtmlControl> > ControlBase;

// Construction
public:
	IdeHelpBox(const String &text, const String &helpID);
	virtual ~IdeHelpBox();

// Attributes
public:
	inline const String & getText() const;
	inline void setText(const String &text);

	inline const String & getHelpID() const;
	inline void setHelpID(const String &helpID);	

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// IControl overrides
protected:
	virtual void onInit();

protected:
	String m_text;
	String m_helpID;	
};

//////////////////////////////////////////////////////////////////////

inline const String & IdeHelpBox::getText() const { return m_text; }
inline void IdeHelpBox::setText(const String &text) { m_text = text; }

inline const String & IdeHelpBox::getHelpID() const { return m_helpID; }
inline void IdeHelpBox::setHelpID(const String &helpID) { m_helpID = helpID; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_BLOCK_H
