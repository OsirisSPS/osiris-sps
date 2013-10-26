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

#ifndef _OS_UI_IRCUSERPAGE_H
#define _OS_UI_IRCUSERPAGE_H

#include "iircroompage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCUser;

//////////////////////////////////////////////////////////////////////

class IRCUserPage : public IIRCRoomPage
{
	DECLARE_EVENT_TABLE()

	typedef IIRCRoomPage WindowBase;

// Construction
public:
    IRCUserPage(IRCWindow *ircWindow, wxWindow *parent, shared_ptr<IIRCTarget> target);
	virtual ~IRCUserPage();

// Operations
private:
	void createLayout();
	void initWindow();		

// IIRCRoomPage interface
public:
	virtual IRCTextCtrl * getEventsCtrl() const;

	virtual bool matchMessageTarget(const std::string &sender, const std::string &target);

private:
	IRCTextCtrl *m_eventsCtrl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_IRCUSERPAGE_H
