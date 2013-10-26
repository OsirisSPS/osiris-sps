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

#ifndef _OS_UI_IRCWINDOW_H
#define _OS_UI_IRCWINDOW_H

#include "ircsession.h"
#include "multilanguagewindow.h"
#include "skinnedwindow.h"
#include "togglebitmapbutton.h"
#include "wxskinbutton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IIRCPage;
class IIRCRoomPage;
class IRCChannelPage;
class IRCServerPage;
class IRCService;

//////////////////////////////////////////////////////////////////////

class IRCWindow : public wxPanel,
				  public MultilanguageWindow
{
	DECLARE_EVENT_TABLE()

	typedef wxPanel WindowBase;

	typedef list<shared_ptr<IIRCCommand> >::type IRCCommands;
	typedef list<String>::type IRCHistory;

// Enumerated types
public:
	enum SoundEvent
	{
		soundEventNone		= 0,
		soundEventMessage	= 1,
	};

// Construction
public:
    IRCWindow(wxWindow *parent, wxWindowID id = static_cast<wxWindowID>(wxID_ANY), const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0);
	virtual ~IRCWindow();

// Attributes
public:
	inline shared_ptr<IRCSession> getSession() const;

	IIRCPage * getActivePage() const;

	IIRCRoomPage * getRoomPage(uint32 id) const;
	IIRCRoomPage * getRoomPage(const std::string &name) const;	

private:
	bool getRequireOptions() const;

// wxPanel overrides
public:
	virtual bool Destroy();

// Operations
public:
	void notifyPage(IIRCPage *page);

	void openChannel(const std::string &name);
	bool openChat(uint32 userID);	

	void playSoundFile(SoundEvent sound); 

private:
	void initSession();

	void createLayout();
	void initWindow();
	void cleanupWindow();

	void notifyCallback(IRCSession::NotifyType notify);
	void commandsCallback(shared_ptr<IIRCCommand> command);
	void roomsCallback(shared_ptr<IRCRoom> room);
	
	void updateConnectionStatus();	

	void handleCommand(shared_ptr<IIRCCommand> command);
	void handleNotify(IRCSession::NotifyType notify);

	void postUpdateCommandsEvent();	

	bool updateCommands();
	void updatePagesStyle();
	bool updateRooms();

	IIRCPage * dispatchMessage(const std::string &sender, const std::string &target, const String &message);

	void updateRoom(shared_ptr<IRCRoom> room);

	bool addPage(IIRCPage *page, const String &caption, bool select);

	void updateAllPages();

	void editOptions();
	void updateOptions();
	void refreshChannels();

	void openConnection();
	void closeConnection();	

	void browseHistory(bool forward);
	void saveHistory(const String &str, bool erasePosition);
	void resetHistoryPosition();

	void updateControls();
	void updateMessage(const String &str);

// Events
private:
	void onSendMessage(wxCommandEvent &e);
	void onMessageUpdated(wxCommandEvent &e);
	void onMessageKeyDown(wxKeyEvent &e);
	void onToggleConnection(wxCommandEvent &e);
	void onUpdate(wxCommandEvent &e);		
	void onProcessNotify(wxCommandEvent &e);		
	void onEditOptions(wxCommandEvent &e);
	void onRefreshChannels(wxCommandEvent &e);
	void onClearServerMessages(wxCommandEvent &e);
	void onPageClose(wxAuiNotebookEvent &e);
	void onPageChanged(wxAuiNotebookEvent &e);

private:
	shared_ptr<IRCService> m_ircService;
	shared_ptr<IRCSession> m_session;
	wxAuiNotebook *m_pages;
	IRCServerPage *m_serverPage;
	SkinnedWindow<wxPanel> *m_commandsPanel;
	ToggleBitmapButton *m_buttonToggleConnection;
	wxSkinButton *m_buttonEditOptions;
	wxSkinButton *m_buttonRefreshChannels;
	wxSkinButton *m_buttonClearServerMessages;
	wxTextCtrl *m_messageCtrl;
	wxButton *m_sendButton;		
	boost::recursive_mutex m_cs;
	IRCCommands m_commands;	
	list<shared_ptr<IRCRoom> >::type m_pendingRooms;
	IRCHistory m_history;
	IRCHistory::iterator m_historyPosition;
	bool m_historyLocked;
	wxBitmap m_pageNotifyBitmap;
	bool m_notifySound;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IRCSession> IRCWindow::getSession() const { return m_session; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_IRCWINDOW_H
