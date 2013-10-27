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

#ifndef _OS_HTML_HTMLTEXTBOX_H
#define _OS_HTML_HTMLTEXTBOX_H

#include "ihtmlinput.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WebExport HtmlTextBox : public IHtmlInput
{
	typedef IHtmlInput ControlBase;

// Construction
public:
	HtmlTextBox();
	virtual ~HtmlTextBox();

// Attributes
public:
	inline bool getPassword() const;
	inline void setPassword(bool password);

	inline bool getHidden() const;
	inline void setHidden(bool hidden);

	inline uint32 getMaxLength() const;
	inline void setMaxLength(const uint32 &value);

	inline uint32 getSize() const;
	inline void setSize(const uint32 &value);

// IHtmlControl overrides
public:
	virtual void renderAttributes(HtmlWriter &writer);

protected:
	bool m_hidden;
	bool m_password;
	uint32 m_maxlength;
	uint32 m_size;
};

//////////////////////////////////////////////////////////////////////

inline bool HtmlTextBox::getPassword() const { return m_password; }
inline void HtmlTextBox::setPassword(bool password) { m_password = password; }

inline bool HtmlTextBox::getHidden() const { return m_hidden; }
inline void HtmlTextBox::setHidden(bool hidden) { m_hidden = hidden; }

inline uint32 HtmlTextBox::getMaxLength() const { return m_maxlength; }
inline void HtmlTextBox::setMaxLength(const uint32 &maxlength) { m_maxlength = maxlength; }

inline uint32 HtmlTextBox::getSize() const { return m_size; }
inline void HtmlTextBox::setSize(const uint32 &size) { m_size = size; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLTEXTBOX_H
