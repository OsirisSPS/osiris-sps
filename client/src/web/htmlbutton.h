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

#ifndef _OS_HTML_HTMLBUTTON_H
#define _OS_HTML_HTMLBUTTON_H

#include "ihtmlcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlLiteral;

//////////////////////////////////////////////////////////////////////

class WebExport HtmlButton : public IHtmlControl
{
	typedef IHtmlControl ControlBase;

// Construction
public:
	HtmlButton(const String &caption = String::EMPTY);
	virtual ~HtmlButton();

// Attributes
public:
	const String & getCaption() const;
	void setCaption(const String &caption);

	shared_ptr<EventSource> getEventClick() const;

// IHtmlControl overrides
public:
	virtual void renderAttributes(HtmlWriter &writer);

protected:
	virtual void onInit();
	virtual void onRender(HtmlWriter &writer);

protected:
	shared_ptr<HtmlLiteral> m_caption;

public:
	static const String EVENT_ONCLICK;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_HTMLBUTTON_H
