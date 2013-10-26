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

#ifndef _OS_UI_IRCCHANNELPAGE_H
#define _OS_UI_IRCCHANNELPAGE_H

#include "iircroompage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCChannel;

//////////////////////////////////////////////////////////////////////

class IRCChannelPage : public IIRCRoomPage
{
	DECLARE_EVENT_TABLE()

	typedef IIRCRoomPage WindowBase;

	typedef unordered_map<IRCUserType, uint32>::type IconsMap;

// Construction
public:
    IRCChannelPage(IRCWindow *ircWindow, wxWindow *parent, shared_ptr<IIRCTarget> target);
	virtual ~IRCChannelPage();

// Attributes
public:
	shared_ptr<IRCUser> getUserByIndex(int32 index) const;
	shared_ptr<IRCUser> getSelectedUser() const;

// Operations
private:
	void createLayout();
	void initWindow();
	void cleanupWindow();

	void m_splitterOnIdle(wxIdleEvent &e);

	void toggleSelectedUserMode(IRCModeType mode);
	void toggleUserMode(shared_ptr<IRCUser> user, IRCModeType mode);

	void kickSelectedUser();
	void kickUser(shared_ptr<IRCUser> user);

// Events
private:
	void onSendMessage(wxCommandEvent &e);
	void onUserActivated(wxListEvent &e);
	void onUserRightClick(wxListEvent &e);		
	void onUserKick(wxCommandEvent &e);
	void onUserBan(wxCommandEvent &e);
	void onUserOperator(wxCommandEvent &e);
	void onUserHalfOperator(wxCommandEvent &e);
	void onUserVoice(wxCommandEvent &e);
	
// IIRCRoomPage interface
public:
	virtual IRCTextCtrl * getEventsCtrl() const;
	
	virtual bool matchMessageTarget(const std::string &sender, const std::string &target);

// IIRCRoomPage overrides
public:
	virtual void updatePage();

private:
	wxSplitterWindow *m_splitter;
	IRCTextCtrl *m_eventsCtrl;
	wxListCtrl *m_usersCtrl;
	unordered_map<uint32, uint32>::type m_users;
	wxImageList m_imageList;
	IconsMap m_iconsMap;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_IRCCHANNELPAGE_H
