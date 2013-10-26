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

#ifndef _OS_IDE_BUTTON_H
#define _OS_IDE_BUTTON_H

#include "idecontrol.h"
#include "iidexslrenderer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport IdeButton : public IXSLRenderer<IPageControl<IHtmlControl> >
{
	typedef IXSLRenderer<IPageControl<IHtmlControl> > ControlBase;

// Construction
public:
	IdeButton(const String &caption = String::EMPTY);
	virtual ~IdeButton();

// Attributes
public:
	inline bool getIsDefault() const;
	inline void setIsDefault(bool def);

	inline const String & getCaption() const;
	inline void setCaption(const String &caption);

	inline const String & getHref() const;
	inline void setHref(const String &href);

	inline const String & getConfirmMessage() const;
	inline void setConfirmMessage(const String &message);

	inline const String & getWaitMessage() const;
	inline void setWaitMessage(const String &message);

	shared_ptr<EventSource> getEventClick() const;

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// IControl overrides
protected:
	virtual void onInit();

protected:
	bool m_isDefault;
	String m_caption;
	String m_href;
	String m_confirmMessage;
	String m_waitMessage;

public:
	static const String EVENT_ONCLICK;
};

//////////////////////////////////////////////////////////////////////

inline bool IdeButton::getIsDefault() const { return m_isDefault; }
inline void IdeButton::setIsDefault(bool def) { m_isDefault = def; }

inline const String & IdeButton::getCaption() const { return m_caption; }
inline void IdeButton::setCaption(const String &caption) { m_caption = caption; }

inline const String & IdeButton::getHref() const { return m_href; }
inline void IdeButton::setHref(const String &href) { m_href = href; }

inline const String & IdeButton::getConfirmMessage() const { return m_confirmMessage; }
inline void IdeButton::setConfirmMessage(const String &message) { m_confirmMessage = message; }

inline const String & IdeButton::getWaitMessage() const { return m_waitMessage; }
inline void IdeButton::setWaitMessage(const String &message) { m_waitMessage = message; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_BUTTON_H
