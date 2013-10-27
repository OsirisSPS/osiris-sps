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

#include "stdafx.h"
#include "ircchannelpage.h"

#include "application.h"
#include "conversions.h"
#include "ircchannel.h"
#include "ircroom.h"
#include "irctextctrl.h"
#include "ircuser.h"
#include "ircwindow.h"
#include "locked.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static const uint32 cmdSendMessage = wxNewId();
static const uint32 cmdUserKick = wxNewId();
static const uint32 cmdUserBan = wxNewId();
static const uint32 cmdUserOperator = wxNewId();
static const uint32 cmdUserHalfOperator = wxNewId();
static const uint32 cmdUserVoice = wxNewId();

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(IRCChannelPage, WindowBase)
	EVT_MENU(cmdSendMessage, IRCChannelPage::onSendMessage)
	EVT_MENU(cmdUserKick, IRCChannelPage::onUserKick)
	EVT_MENU(cmdUserBan, IRCChannelPage::onUserBan)
	EVT_MENU(cmdUserOperator, IRCChannelPage::onUserOperator)
	EVT_MENU(cmdUserHalfOperator, IRCChannelPage::onUserHalfOperator)
	EVT_MENU(cmdUserVoice, IRCChannelPage::onUserVoice)	
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

IRCChannelPage::IRCChannelPage(IRCWindow *ircWindow, wxWindow *parent, shared_ptr<IIRCTarget> target) : WindowBase(ircWindow, parent, target),
																										m_splitter(null),
																										m_eventsCtrl(null),
																										m_usersCtrl(null),
																										m_imageList(16, 16)
{
	OS_ASSERT(target->getType() == ircTargetTypeChannel);

	createLayout();
	initWindow();
}

IRCChannelPage::~IRCChannelPage()
{
	cleanupWindow();	
}

shared_ptr<IRCUser> IRCChannelPage::getUserByIndex(int32 index) const
{
	OS_ASSERT(index != -1);
	OS_ASSERT(m_usersCtrl != null);

	shared_ptr<IRCSession> session = getSession();
	if(session == null)
		return null;

	return session->findUser(static_cast<uint32>(m_usersCtrl->GetItemData(index)));
}

shared_ptr<IRCUser> IRCChannelPage::getSelectedUser() const
{
	OS_ASSERT(m_usersCtrl != null);

	int32 index = m_usersCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
	if(index == -1)
		return null;

	return getUserByIndex(index);
}

void IRCChannelPage::createLayout()
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_splitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter->Connect( wxEVT_IDLE, wxIdleEventHandler( IRCChannelPage::m_splitterOnIdle ), NULL, this );
	wxPanel* eventsPanel;
	eventsPanel = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* eventsSizer;
	eventsSizer = new wxBoxSizer( wxVERTICAL );
	
	m_eventsCtrl = new IRCTextCtrl( eventsPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	eventsSizer->Add( m_eventsCtrl, 1, wxEXPAND | wxALL, 0 );
	
	eventsPanel->SetSizer( eventsSizer );
	eventsPanel->Layout();
	eventsSizer->Fit( eventsPanel );
	wxPanel* usersPanel;
	usersPanel = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* usersSizer;
	usersSizer = new wxBoxSizer( wxVERTICAL );
	
	m_usersCtrl = new wxListCtrl( usersPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL );
	usersSizer->Add( m_usersCtrl, 1, wxALL|wxEXPAND, 0 );
	
	usersPanel->SetSizer( usersSizer );
	usersPanel->Layout();
	usersSizer->Fit( usersPanel );
	m_splitter->SplitVertically( eventsPanel, usersPanel, 0 );
	mainSizer->Add( m_splitter, 1, wxEXPAND, 0 );
	
	this->SetSizer( mainSizer );
	this->Layout();
}

void IRCChannelPage::initWindow()
{
	m_iconsMap[ircUserTypeNormal] = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_icons_user_normal.png"))), wxBITMAP_TYPE_PNG));
	m_iconsMap[ircUserTypeOperator] = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_icons_user_operator.png"))), wxBITMAP_TYPE_PNG));
	m_iconsMap[ircUserTypeHalfOperator] = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_icons_user_half_operator.png"))), wxBITMAP_TYPE_PNG));
	
	m_usersCtrl->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(IRCChannelPage::onUserActivated), NULL, this);
	m_usersCtrl->Connect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler(IRCChannelPage::onUserRightClick), NULL, this);
	
	m_usersCtrl->InsertColumn(0, wxEmptyString);
	m_usersCtrl->SetImageList(&m_imageList, wxIMAGE_LIST_SMALL);
}

void IRCChannelPage::cleanupWindow()
{
	m_usersCtrl->Disconnect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(IRCChannelPage::onUserActivated), NULL, this);
	m_usersCtrl->Disconnect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler(IRCChannelPage::onUserRightClick), NULL, this);
}

void IRCChannelPage::m_splitterOnIdle(wxIdleEvent &e)
{
	m_splitter->SetSashPosition(GetSize().GetWidth() * 2 / 3);
	m_splitter->Disconnect(wxEVT_IDLE, wxIdleEventHandler(IRCChannelPage::m_splitterOnIdle), NULL, this);
}

void IRCChannelPage::toggleSelectedUserMode(IRCModeType mode)
{
	toggleUserMode(getSelectedUser(), mode);
}

void IRCChannelPage::toggleUserMode(shared_ptr<IRCUser> user, IRCModeType mode)
{
	if(user == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<IRCRoom> room = getRoom();
	if(room == null)
		return;

	shared_ptr<IRCRoom::UserDetails> userDetails = room->getUserDetails(user->getID());
	if(userDetails == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	bool enabled = true;
	switch(mode)
	{
	case ircModeTypeOperator:		enabled = !(userDetails->getType() == ircUserTypeOperator);
									break;

	case ircModeTypeHalfOperator:	enabled = !(userDetails->getType() == ircUserTypeHalfOperator);
									break;
				
	case ircModeTypeVoice:			enabled = !(userDetails->getVoice());
									break;

	case ircModeTypeBan:			enabled = true;
									break;

	default:						OS_ASSERTFALSE();
									return;
	}

	room->changeUserMode(user->getID(), mode, enabled);
}

void IRCChannelPage::kickSelectedUser()
{
	kickUser(getSelectedUser());
}

void IRCChannelPage::kickUser(shared_ptr<IRCUser> user)
{
	if(user == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<IRCRoom> room = getRoom();
	if(room == null)
		return;

	room->kickUser(user->getID());
}

void IRCChannelPage::onSendMessage(wxCommandEvent &e)
{
	e.Skip();

	shared_ptr<IRCUser> user = getSelectedUser();
	if(user == null || user->isLocal())
		return;

	getIRCWindow()->openChat(user->getID());		
}

void IRCChannelPage::onUserActivated(wxListEvent &e)
{
	e.Skip();

	int32 index = e.GetIndex();
	if(index != -1)
		getIRCWindow()->openChat(static_cast<uint32>(m_usersCtrl->GetItemData(index)));		
}

void IRCChannelPage::onUserRightClick(wxListEvent &e)
{
	e.Skip();

	int32 index = e.GetIndex();
	if(index == -1)
		return;

	shared_ptr<IRCUser> targetUser = getUserByIndex(index);
	if(targetUser == null || targetUser->isLocal())
		return;

	shared_ptr<IRCRoom> room = getRoom();
	if(room == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<IRCRoom::UserDetails> localUserDetails = room->getLocalUserDetails();
	if(localUserDetails == null)
	{
		OS_ASSERTFALSE();
		return;
	}
	
	shared_ptr<IRCRoom::UserDetails> targetUserDetails = room->getUserDetails(targetUser->getID());
	if(targetUserDetails == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	scoped_ptr<wxMenu> menu(new wxMenu());

	wxMenu *userMenu = menu.get();
	if(localUserDetails->hasOperatorPrivileges())
	{
		wxMenu *adminMenu = new wxMenu();
		wxMenuItem *menuItem;

		// 0.15 : Note the sequence: SetCheckable must be set BEFORE the item is append to menù, otherwise don't work under Linux.

		menuItem = new wxMenuItem(menu.get(), cmdUserOperator, conversions::from_utf16<wxString>(getText("ui.irc_channel_window.admin_menu.user_operator")));		
		menuItem->SetCheckable(true);
		adminMenu->Append(menuItem);
		menuItem->Check(targetUserDetails->getType() == ircUserTypeOperator);
		
		menuItem = new wxMenuItem(menu.get(), cmdUserHalfOperator, conversions::from_utf16<wxString>(getText("ui.irc_channel_window.admin_menu.user_half_operator")));		
		menuItem->SetCheckable(true);
		adminMenu->Append(menuItem);
		menuItem->Check(targetUserDetails->getType() == ircUserTypeHalfOperator);		

		menuItem = new wxMenuItem(menu.get(), cmdUserVoice, conversions::from_utf16<wxString>(getText("ui.irc_channel_window.admin_menu.user_voice")));
		menuItem->SetCheckable(true);
		adminMenu->Append(menuItem);		
		menuItem->Check(targetUserDetails->getVoice());
		
		adminMenu->AppendSeparator();

		adminMenu->Append(new wxMenuItem(menu.get(), cmdUserKick, conversions::from_utf16<wxString>(getText("ui.irc_channel_window.admin_menu.user_kick"))));
		adminMenu->Append(new wxMenuItem(menu.get(), cmdUserBan, conversions::from_utf16<wxString>(getText("ui.irc_channel_window.admin_menu.user_ban"))));
		
		menu->AppendSubMenu(adminMenu, conversions::from_utf16<wxString>(getText("ui.irc_channel_window.admin_menu")));

		userMenu = new wxMenu();
		menu->AppendSubMenu(userMenu, conversions::from_utf16<wxString>(getText("ui.irc_channel_window.user_menu")));
	}

	OS_ASSERT(userMenu != null);
	userMenu->Append(new wxMenuItem(menu.get(), cmdSendMessage, conversions::from_utf16<wxString>(getText("ui.irc_channel_window.user_menu.send_message"))));

	PopupMenu(menu.get());	
}

void IRCChannelPage::onUserKick(wxCommandEvent &e)
{
	e.Skip();

	kickSelectedUser();
}

void IRCChannelPage::onUserBan(wxCommandEvent &e)
{
	e.Skip();

	shared_ptr<IRCUser> user = getSelectedUser();
	if(user != null)
	{
		toggleUserMode(user, ircModeTypeBan);
		kickUser(user);
	}
}

void IRCChannelPage::onUserOperator(wxCommandEvent &e)
{
	e.Skip();

	toggleSelectedUserMode(ircModeTypeOperator);
}

void IRCChannelPage::onUserHalfOperator(wxCommandEvent &e)
{
	e.Skip();

	toggleSelectedUserMode(ircModeTypeHalfOperator);
}

void IRCChannelPage::onUserVoice(wxCommandEvent &e)
{
	e.Skip();

	toggleSelectedUserMode(ircModeTypeVoice);
}

IRCTextCtrl * IRCChannelPage::getEventsCtrl() const
{
	return m_eventsCtrl;
}

bool IRCChannelPage::matchMessageTarget(const std::string &sender, const std::string &target)
{
	OS_ASSERT(target.empty() == false);

	shared_ptr<IRCRoom> room = getRoom();
	if(room == null)
		return false;

	return room->getName() == target;
}

void IRCChannelPage::updatePage()
{
	WindowBase::updatePage();

	shared_ptr<IRCRoom> room = getRoom();
	if(room == null)
		return;

	wxWindowUpdateLocker windowLocker(this);

	m_usersCtrl->DeleteAllItems();

	Locked<const IRCRoom::Users>::unique users = room->getUsers();
	for(IRCRoom::Users::const_iterator i = users->begin(); i != users->end(); ++i)
	{
		shared_ptr<IRCUser> user = i->second.first;
		shared_ptr<IRCRoom::UserDetails> userDetails = i->second.second;

		int32 index = m_usersCtrl->InsertItem(m_usersCtrl->GetItemCount(), wxEmptyString);
		if(index != -1)
		{
			m_usersCtrl->SetItemData(index, user->getID());
			m_usersCtrl->SetItem(index, 0, conversions::from_string<wxString>(user->getName()));

			uint32 icon = 0;

			IconsMap::const_iterator i = m_iconsMap.find(userDetails->getType());
			if(i == m_iconsMap.end())
			{
				OS_ASSERT(m_iconsMap.find(ircUserTypeNormal) != m_iconsMap.end());
				icon = m_iconsMap[ircUserTypeNormal];
			}
			else
			{
				icon = i->second;
			}

			m_usersCtrl->SetItemImage(index, icon);
		}
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
