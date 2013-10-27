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

#ifndef _OS_UI_IIRCROOMPAGE_H
#define _OS_UI_IIRCROOMPAGE_H

#include "iircpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCTextCtrl;

//////////////////////////////////////////////////////////////////////

class IIRCRoomPage : public IIRCPage
{
	DECLARE_EVENT_TABLE()

	typedef IIRCPage WindowBase;

// Construction
public:
    IIRCRoomPage(IRCWindow *ircWindow, wxWindow *parent, shared_ptr<IIRCTarget> target);
	virtual ~IIRCRoomPage();

// Attributes
public:
	inline shared_ptr<IIRCTarget> getTarget() const;	

	shared_ptr<IRCRoom> getRoom() const;

// Operations
public:
	void addMessage(const std::string &user, const String &message);

protected:
	void updateControls(wxWindow *window, bool enabled);

// IIRCPage interface
public:
	virtual bool processCommand(const String &command);	

// Interface
public:
	virtual IRCTextCtrl * getEventsCtrl() const = 0;
	
	virtual bool matchMessageTarget(const std::string &sender, const std::string &target) = 0;

// IIRCPage overrides
public:
	virtual void handlePageClosed();

	virtual void updatePage();	

private:
	bool m_enabled;
	shared_ptr<IIRCTarget> m_target;	
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IIRCTarget> IIRCRoomPage::getTarget() const { return m_target; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_IIRCROOMPAGE_H
