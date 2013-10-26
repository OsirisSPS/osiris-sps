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

#ifndef _OS_HTML_HTMLISTBOX_H
#define _OS_HTML_HTMLISTBOX_H

#include "ihtmlselect.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlListBox : public IHtmlSelect
{
	typedef IHtmlSelect ControlBase;

// Construction
public:
	HtmlListBox();
	virtual ~HtmlListBox();

// Attributes
public:
	inline uint32 getSize() const;
	inline void setSize(uint32 size);

	inline bool getAllowMultipleSelections() const;
	inline void setAllowMultipleSelections(bool allow);

// IHtmlControl overrides
public:
	virtual void renderAttributes(HtmlWriter &writer);

protected:
	uint32 m_size;
	bool m_allowMultipleSelections;
};

//////////////////////////////////////////////////////////////////////

inline uint32 HtmlListBox::getSize() const { return m_size; }
inline void HtmlListBox::setSize(uint32 size) { m_size = size; }

inline bool HtmlListBox::getAllowMultipleSelections() const { return m_allowMultipleSelections; }
inline void HtmlListBox::setAllowMultipleSelections(bool allow) { m_allowMultipleSelections = allow; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLISTBOX_H
