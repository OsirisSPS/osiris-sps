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
#include "ircwindow.h"

#include "algorithms.h"
#include "application.h"
#include "boost/bind.hpp"
#include "buffer.h"
#include "conversions.h"
#include "engine.h"
#include "iirccommand.h"
#include "ircchannelpage.h"
#include "irccommandmessage.h"
#include "ircconfigwindow.h"
#include "ircroom.h"
#include "ircserverpage.h"
#include "ircservice.h"
#include "ircuser.h"
#include "ircuserpage.h"
#include "lock.h"
#include "log.h"
#include "options.h"
#include "platformmanager.h"
#include "soundmanager.h"
#include "uiapi.h"
#include "uistandards.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

#define OS_UI_IRC_HISTORY_SIZE					100

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static const uint32 cmdUpdate = wxNewId();
static const uint32 cmdSendMessage = wxNewId();
static const uint32 cmdToggleConnection = wxNewId();
static const uint32 cmdEditOptions = wxNewId();
static const uint32 cmdRefreshChannels = wxNewId();
static const uint32 cmdProcessNotify = wxNewId();
static const uint32 cmdClearServerMessages = wxNewId();

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(IRCWindow, WindowBase)
	EVT_BUTTON(cmdSendMessage, IRCWindow::onSendMessage)
	EVT_BUTTON(cmdToggleConnection, IRCWindow::onToggleConnection)
	EVT_BUTTON(cmdEditOptions, IRCWindow::onEditOptions)
	EVT_BUTTON(cmdRefreshChannels, IRCWindow::onRefreshChannels)
	EVT_BUTTON(cmdClearServerMessages, IRCWindow::onClearServerMessages)
	EVT_COMMAND(cmdUpdate, OS_UI_EVENT_NOTIFY, IRCWindow::onUpdate)
	EVT_COMMAND(cmdProcessNotify, OS_UI_EVENT_NOTIFY, IRCWindow::onProcessNotify)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

IRCWindow::IRCWindow(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style) : WindowBase(parent, id, pos, size, style),
																											m_ircService(OS_NEW IRCService()),
																											m_pages(nullptr),
																											m_serverPage(nullptr),
																											m_buttonToggleConnection(nullptr),
																											m_buttonEditOptions(nullptr),
																											m_buttonRefreshChannels(nullptr),
																											m_buttonClearServerMessages(nullptr),
																											m_messageCtrl(nullptr),
																											m_sendButton(nullptr),
																											m_historyLocked(false),
																											m_notifySound(false)
{
	resetHistoryPosition();

	Options::instance()->ensureOption<bool>(OS_IRC_OPTION_SERVER_AUTOMATIC_CONNECTION, false, true);
	Options::instance()->ensureOption<String>(OS_IRC_OPTION_SERVER_ADDRESS, _S("www.osiris-sps.org"), true);
	Options::instance()->ensureOption<uint32>(OS_IRC_OPTION_SERVER_PORT, 6667, true);
	Options::instance()->ensureOption<uint32>(OS_IRC_OPTION_SERVER_PORT_SSL, 9999, true);
	Options::instance()->ensureOption<bool>(OS_IRC_OPTION_SERVER_ENABLE_SSL, false, true);
	Options::instance()->ensureOption<uint32>(OS_IRC_OPTION_SERVER_TIMEOUT, OS_IRC_DEFAULT_TIMEOUT, true);
	Options::instance()->ensureOption<String>(OS_IRC_OPTION_NICKNAME, String::EMPTY, true);
	Options::instance()->ensureOption<String>(OS_IRC_OPTION_ALTERNATE_NICKNAME, String::EMPTY, true);
	Options::instance()->ensureOption<String>(OS_IRC_OPTION_USERNAME, String::EMPTY, true);
	Options::instance()->ensureOption<String>(OS_IRC_OPTION_EMAIL, String::EMPTY, true);
	Options::instance()->ensureOption<String>(OS_IRC_OPTION_DEFAULT_CHANNEL, _S("#general"), true);
	Options::instance()->ensureOption<uint32>(OS_IRC_OPTION_SOUNDS_MODE, OS_IRC_OPTION_SOUNDS_MODE_NOTIFY, true);

	Options::instance()->subscribeUpdateHandler(boost::bind(&IRCWindow::updateOptions, this));

	initSession();
	OS_ASSERT(m_session != nullptr);

	createLayout();
	initWindow();

	updateOptions();

	m_ircService->start();

	if(Options::instance()->getOption<bool>(OS_IRC_OPTION_SERVER_AUTOMATIC_CONNECTION))
		openConnection();
}

IRCWindow::~IRCWindow()
{
	cleanupWindow();
}

IIRCPage * IRCWindow::getActivePage() const
{
	int32 activePage = m_pages->GetSelection();
	if(activePage == -1)
	{
		OS_ASSERTFALSE();
		return m_serverPage;
	}

	OS_ASSERT(dynamic_cast<IIRCPage *>(m_pages->GetPage(activePage)) != nullptr);
	return dynamic_cast<IIRCPage *>(m_pages->GetPage(activePage));
}

IIRCRoomPage * IRCWindow::getRoomPage(const std::string &name) const
{
	size_t pages = m_pages->GetPageCount();
	for(size_t i = 0; i < pages; ++i)
	{
		IIRCRoomPage *page = dynamic_cast<IIRCRoomPage *>(m_pages->GetPage(i));
		if(page != nullptr)
		{
			shared_ptr<IIRCTarget> pageTarget = page->getTarget();
			if(pageTarget != nullptr && pageTarget->getName() == name)
				return page;
		}
	}

	return nullptr;
}

bool IRCWindow::getRequireOptions() const
{
	if(Options::instance()->getOption<String>(OS_IRC_OPTION_NICKNAME).empty())
		return true;

	if(Options::instance()->getOption<String>(OS_IRC_OPTION_SERVER_PORT).empty())
		return true;

	return false;
}

bool IRCWindow::Destroy()
{
	m_session.reset();
	m_ircService->stop();

	return WindowBase::Destroy();
}

void IRCWindow::notifyPage(IIRCPage *page)
{
	OS_ASSERT(page != nullptr);

	int32 pageIndex = m_pages->GetPageIndex(page);
	if(m_pages->GetSelection() != pageIndex)
		m_pages->SetPageBitmap(pageIndex, m_pageNotifyBitmap);
}

void IRCWindow::openChannel(const std::string &name)
{
	IIRCPage *page = getRoomPage(name);
	if(page != nullptr)
	{
		int32 pageIndex = m_pages->GetPageIndex(page);
		OS_ASSERT(pageIndex != -1);
		m_pages->SetSelection(pageIndex);
	}
	else
	{
		m_session->joinChannel(name);
	}
}

bool IRCWindow::openChat(uint32 userID)
{
	shared_ptr<IRCSession> session = getSession();
	if(session == nullptr)
		return false;

	shared_ptr<IRCUser> user = session->findUser(userID);
	if(user == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(user->isLocal())
		return false;

	IIRCPage *page = getRoomPage(user->getName());
	if(page != nullptr)
	{
		int32 pageIndex = m_pages->GetPageIndex(page);
		OS_ASSERT(pageIndex != -1);
		m_pages->SetSelection(pageIndex);
	}
	else
	{
		session->ensureRoom(user, true);
	}

	return true;
}

void IRCWindow::playSoundFile(SoundEvent sound)
{
	if(sound == soundEventNone)
		return;

	bool play = false;

	switch(Options::instance()->getOption<uint32>(OS_IRC_OPTION_SOUNDS_MODE))
	{
	case OS_IRC_OPTION_SOUNDS_MODE_NONE:
												break;

	case OS_IRC_OPTION_SOUNDS_MODE_ALWAYS:		play = true;
												break;

	case OS_IRC_OPTION_SOUNDS_MODE_NOTIFY:		if(m_notifySound || IsShownOnScreen() == false)
													play = true;

												m_notifySound = false;
												break;

	default:									OS_LOG_WARNING(_S("Invalid option value for '") + OS_IRC_OPTION_SOUNDS_MODE + _S("'"));
												break;
	}

	if(play == false)
		return;

	String filename;
	switch(sound)
	{
	case soundEventMessage:		filename = Application::instance()->getResourcesFilePath(_S("irc_message.wav"));
								break;

	default:					OS_ASSERTFALSE();
								break;
	}

	if(filename.empty())
		return;

	SoundManager::instance()->playSound(filename);
}

void IRCWindow::initSession()
{
	OS_ASSERT(m_ircService != nullptr);
	OS_ASSERT(m_session == nullptr);
	m_session = m_ircService->addSession();
	m_session->setClientName(Engine::getVersionName(false));

	m_session->registerNotifyCallback(boost::bind(&IRCWindow::notifyCallback, this, _1));
	m_session->registerCommandsCallback(boost::bind(&IRCWindow::commandsCallback, this, _1));
	m_session->registerRoomsCallback(boost::bind(&IRCWindow::roomsCallback, this, _1));
}

void IRCWindow::createLayout()
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* contentSizer;
	contentSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_pages = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	contentSizer->Add( m_pages, 1, wxEXPAND | wxALL, 0 );
	
	m_commandsPanel = new SkinnedWindow<wxPanel>( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* commandsSizer;
	commandsSizer = new wxBoxSizer( wxVERTICAL );
	
	m_buttonToggleConnection = new ToggleBitmapButton( m_commandsPanel, cmdToggleConnection, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsSizer->Add( m_buttonToggleConnection, 0, wxALL, 2 );
	
	m_buttonEditOptions = new wxSkinButton( m_commandsPanel, cmdEditOptions, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsSizer->Add( m_buttonEditOptions, 0, wxALL, 2 );
	
	m_buttonRefreshChannels = new wxSkinButton( m_commandsPanel, cmdRefreshChannels, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsSizer->Add( m_buttonRefreshChannels, 0, wxALL, 2 );
	
	m_buttonClearServerMessages = new wxSkinButton( m_commandsPanel, cmdClearServerMessages, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsSizer->Add( m_buttonClearServerMessages, 0, wxALL, 2 );
	
	m_commandsPanel->SetSizer( commandsSizer );
	m_commandsPanel->Layout();
	commandsSizer->Fit( m_commandsPanel );
	contentSizer->Add( m_commandsPanel, 0, wxEXPAND | wxALL, 0 );
	
	mainSizer->Add( contentSizer, 1, wxEXPAND, 0 );
	
	wxBoxSizer* controlsSizer;
	controlsSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_messageCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	controlsSizer->Add( m_messageCtrl, 1, wxALL|wxEXPAND, 0 );
	
	m_sendButton = new wxButton( this, cmdSendMessage, wxT("Send"), wxDefaultPosition, wxDefaultSize, 0 );
	controlsSizer->Add( m_sendButton, 0, wxALL, 0 );
	
	mainSizer->Add( controlsSizer, 0, wxEXPAND, 0 );
	
	this->SetSizer( mainSizer );
	this->Layout();
}

void IRCWindow::initWindow()
{
	OS_ASSERT(m_commandsPanel != nullptr);
	m_commandsPanel->getRenderer()->setBackgroundImage(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("window_commands_background.png"))), wxBITMAP_TYPE_PNG));

	m_pageNotifyBitmap = wxBitmap(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_icons_page_notify.png"))), wxBITMAP_TYPE_PNG));
	OS_ASSERT(m_pageNotifyBitmap.IsOk());

	OS_ASSERT(m_serverPage == nullptr);
	OS_ASSERT(m_session != nullptr);
	m_serverPage = new IRCServerPage(this, m_pages);
	addPage(m_serverPage, _S("Status"), false);

	m_messageCtrl->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(IRCWindow::onSendMessage), NULL, this);
	m_messageCtrl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCWindow::onMessageUpdated), NULL, this);
	m_messageCtrl->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(IRCWindow::onMessageKeyDown), NULL, this);

	m_pages->Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler(IRCWindow::onPageClose), NULL, this);
	m_pages->Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler(IRCWindow::onPageChanged), NULL, this);

	m_buttonToggleConnection->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_toggle_connection_normal_off.png"))), wxBITMAP_TYPE_PNG));
	m_buttonToggleConnection->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_toggle_connection_down_off.png"))), wxBITMAP_TYPE_PNG));
	m_buttonToggleConnection->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_toggle_connection_over_off.png"))), wxBITMAP_TYPE_PNG));
	m_buttonToggleConnection->setImageNormalChecked(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_toggle_connection_normal_on.png"))), wxBITMAP_TYPE_PNG));
	m_buttonToggleConnection->setImageDownChecked(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_toggle_connection_down_on.png"))), wxBITMAP_TYPE_PNG));
	m_buttonToggleConnection->setImageOverChecked(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_toggle_connection_over_on.png"))), wxBITMAP_TYPE_PNG));
	m_buttonToggleConnection->setAutoToggle(false);
	m_buttonToggleConnection->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.irc.toggle_connection.tooltip"))));

	m_buttonEditOptions->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_edit_options_normal.png"))), wxBITMAP_TYPE_PNG));
	m_buttonEditOptions->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_edit_options_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonEditOptions->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_edit_options_over.png"))), wxBITMAP_TYPE_PNG));
	m_buttonEditOptions->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.irc.edit_options.tooltip"))));

	m_buttonRefreshChannels->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_refresh_channels_normal.png"))), wxBITMAP_TYPE_PNG));
	m_buttonRefreshChannels->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_refresh_channels_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonRefreshChannels->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_refresh_channels_over.png"))), wxBITMAP_TYPE_PNG));
	m_buttonRefreshChannels->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.irc.refresh_channels.tooltip"))));

	m_buttonClearServerMessages->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_clear_server_messages_normal.png"))), wxBITMAP_TYPE_PNG));
	m_buttonClearServerMessages->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_clear_server_messages_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonClearServerMessages->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("irc_btn_clear_server_messages_over.png"))), wxBITMAP_TYPE_PNG));
	m_buttonClearServerMessages->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.irc.clear_server_messages.tooltip"))));

	updatePagesStyle();
	updateConnectionStatus();
}

void IRCWindow::cleanupWindow()
{
	m_messageCtrl->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(IRCWindow::onSendMessage), nullptr, this);
	m_messageCtrl->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCWindow::onMessageUpdated), NULL, this);
	m_messageCtrl->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(IRCWindow::onMessageKeyDown), NULL, this);

	m_pages->Disconnect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler(IRCWindow::onPageClose), NULL, this);
	m_pages->Disconnect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler(IRCWindow::onPageChanged), NULL, this);
}

void IRCWindow::notifyCallback(IRCSession::NotifyType notify)
{
	wxCommandEvent e(OS_UI_EVENT_NOTIFY, cmdProcessNotify);
	e.SetInt(static_cast<int32>(notify));
	wxPostEvent(this, e);
}

void IRCWindow::commandsCallback(shared_ptr<IIRCCommand> command)
{
	OS_ASSERT(command != nullptr);

	{
		OS_LOCK(m_cs);
		m_commands.push_back(command);
	}

	postUpdateCommandsEvent();
}

void IRCWindow::roomsCallback(shared_ptr<IRCRoom> room)
{
	OS_ASSERT(room != nullptr);

	{
		OS_LOCK(m_cs);
		m_pendingRooms.push_back(room);
	}

	postUpdateCommandsEvent();
}

void IRCWindow::updateConnectionStatus()
{
	OS_ASSERT(m_session != nullptr);

	updateControls();

	if(m_session->getConnectionStatus() == ircConnectionStatusConnected)
	{
		OS_ASSERT(m_session != nullptr);

		refreshChannels();

		String defaultChannel = Options::instance()->getOption<String>(OS_IRC_OPTION_DEFAULT_CHANNEL);
		if(defaultChannel.empty() == false)
			m_session->joinChannel(defaultChannel.to_ascii());
	}
}

void IRCWindow::handleCommand(shared_ptr<IIRCCommand> command)
{
	OS_ASSERT(m_serverPage != nullptr);
	m_serverPage->handleCommand(command);

	if(command->getType() == ircCommandTypeMessage)
	{
		shared_ptr<IRCSession> session = getSession();
		if(session != nullptr)
		{
			SoundEvent sound = soundEventNone;
			IIRCPage *handler = nullptr;

			shared_ptr<IRCCommandMessage> message = boost::dynamic_pointer_cast<IRCCommandMessage>(command);
			handler = dispatchMessage(message->isLocal() ? session->getNick() : message->getSender(), message->getTarget(), message->getMessage());
			if(handler != nullptr)
				sound = soundEventMessage;

			if(sound != soundEventNone)
			{
				if(handler != nullptr)
				{
					if(m_pages->GetSelection() != m_pages->GetPageIndex(handler))
						m_notifySound = true;
				}

				playSoundFile(sound);
			}
		}
	}
}

void IRCWindow::handleNotify(IRCSession::NotifyType notify)
{
	switch(notify)
	{
	case IRCSession::statusChanged:		updateConnectionStatus();
										updateAllPages();
										break;

	case IRCSession::channelsUpdated:	m_serverPage->updateChannels();
										break;
	}
}

void IRCWindow::postUpdateCommandsEvent()
{
	wxCommandEvent e(OS_UI_EVENT_NOTIFY, cmdUpdate);
	wxPostEvent(this, e);
}

bool IRCWindow::updateCommands()
{
	bool pending = true;

	shared_ptr<IIRCCommand> command;

	{
		OS_LOCK(m_cs);

		if(m_commands.empty())
		{
			pending = false;
		}
		else
		{
			command = utils::pop_front(m_commands);
			OS_ASSERT(command != nullptr);
			pending = m_commands.size() > 0;
		}
	}

	if(command != nullptr)
		handleCommand(command);

	return pending;
}

void IRCWindow::updatePagesStyle()
{
	OS_ASSERT(m_serverPage != nullptr);

	int32 selection = m_pages->GetSelection();
	if(selection == -1)
	{
		OS_ASSERTFALSE();
		return;
	}

	long style = wxAUI_NB_BOTTOM | wxAUI_NB_TAB_MOVE | wxAUI_NB_WINDOWLIST_BUTTON | wxAUI_NB_MIDDLE_CLICK_CLOSE;
	if(selection != m_pages->GetPageIndex(m_serverPage))
		style |= wxAUI_NB_CLOSE_ON_ACTIVE_TAB;

	m_pages->SetWindowStyleFlag(style);
}

bool IRCWindow::updateRooms()
{
	bool pending = true;

	shared_ptr<IRCRoom> room;

	{
		OS_LOCK(m_cs);

		if(m_pendingRooms.empty())
		{
			pending = false;
		}
		else
		{
			room = utils::pop_front(m_pendingRooms);
			OS_ASSERT(room != nullptr);
			pending = m_pendingRooms.size() > 0;
		}
	}

	if(room != nullptr)
		updateRoom(room);

	return pending;
}

void IRCWindow::updateRoom(shared_ptr<IRCRoom> room)
{
	OS_ASSERT(room != nullptr);

	IIRCRoomPage *roomPage = getRoomPage(room->getName());
	if(roomPage == nullptr)
	{
		switch(room->getType())
		{
		case ircTargetTypeUser:		roomPage = new IRCUserPage(this, m_pages, room->getTarget());
									m_notifySound = true;
									break;

		case ircTargetTypeChannel:	roomPage = new IRCChannelPage(this, m_pages, room->getTarget());
									break;

		default:					OS_ASSERTFALSE();
									break;
		}

		OS_ASSERT(roomPage != nullptr);
		addPage(roomPage, String::EMPTY, true);
	}

	OS_ASSERT(roomPage != nullptr);
	m_pages->SetPageText(m_pages->GetPageIndex(roomPage), conversions::from_string<wxString>(room->getName()));
	roomPage->updatePage();

	updateControls();
}

IIRCPage * IRCWindow::dispatchMessage(const std::string &sender, const std::string &target, const String &message)
{
	IIRCPage *handler = nullptr;

	size_t count = m_pages->GetPageCount();
	for(size_t i = 0; i < count; ++i)
	{
		IIRCRoomPage *page = dynamic_cast<IIRCRoomPage *>(m_pages->GetPage(i));
		if(page != nullptr)
		{
			if(page->matchMessageTarget(sender, target))
			{
				page->addMessage(sender, message);
				handler = page;
			}
		}
	}

	return handler;
}

bool IRCWindow::addPage(IIRCPage *page, const String &caption, bool select)
{
	if(page == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	OS_ASSERT(m_pages != nullptr);
	if(m_pages->AddPage(page, conversions::from_utf16<wxString>(caption), select, wxNullBitmap) == false)
		return false;

	page->handlePageOpened();
	return true;
}

void IRCWindow::updateAllPages()
{
	size_t count = m_pages->GetPageCount();
	for(size_t i = 0; i < count; ++i)
	{
		IIRCPage *page = dynamic_cast<IIRCPage *>(m_pages->GetPage(i));
		OS_ASSERT(page != nullptr);
		if(page != nullptr)
			page->updatePage();
	}
}

void IRCWindow::editOptions()
{
	IRCConfigWindow configWindow(this, wxID_ANY);
	if(configWindow.ShowModal() != wxID_OK)
		return;

	configWindow.saveConfig();

	Options::instance()->update();
}

void IRCWindow::updateOptions()
{
	m_session->setServer(Options::instance()->getOption<String>(OS_IRC_OPTION_SERVER_ADDRESS).to_ascii());
	if(Options::instance()->getOption<bool>(OS_IRC_OPTION_SERVER_ENABLE_SSL))
	{
		m_session->setPort(Options::instance()->getOption<uint32>(OS_IRC_OPTION_SERVER_PORT_SSL));
		m_session->setEnableSSL(true);
	}
	else
	{
		m_session->setPort(Options::instance()->getOption<uint32>(OS_IRC_OPTION_SERVER_PORT));
		m_session->setEnableSSL(false);
	}

	m_session->setTimeout(Options::instance()->getOption<uint32>(OS_IRC_OPTION_SERVER_TIMEOUT));

	m_session->setUser(Options::instance()->getOption<String>(OS_IRC_OPTION_USERNAME).to_utf8());
	m_session->setNick(Options::instance()->getOption<String>(OS_IRC_OPTION_NICKNAME).to_utf8());
}

void IRCWindow::refreshChannels()
{
	if(m_session != nullptr)
		m_session->requestChannelsList();
}

void IRCWindow::openConnection()
{
	if(getRequireOptions())
		editOptions();

	if(getRequireOptions())
		return;

	if(m_ircService == nullptr)
		return;

	m_session->openConnection(Engine::instance()->createTCPSocket(m_ircService->getService(), true, true));
}

void IRCWindow::closeConnection()
{
	if(m_session != nullptr)
		m_session->closeConnection();
}

void IRCWindow::browseHistory(bool forward)
{
	if(m_history.empty())
	{
		OS_ASSERT(m_historyPosition == m_history.end());
		return;
	}

	if(forward)
	{
		if(m_historyPosition != m_history.end())
			m_historyPosition++;
	}
	else
	{
		if(m_historyPosition == m_history.begin())
			return;		// Per non resettare il testo

		m_historyPosition--;
	}

	String str;
	if(m_historyPosition != m_history.end())
		str = *m_historyPosition;

	updateMessage(str);
}

void IRCWindow::saveHistory(const String &str, bool erasePosition)
{
	if(str.empty())
	{
		OS_ASSERTFALSE();
		return;
	}

	if(erasePosition && m_historyPosition != m_history.end())
		m_history.erase(m_historyPosition);

	m_history.push_back(str);
	m_historyPosition = m_history.end();

	utils::pop_front_until(m_history, OS_UI_IRC_HISTORY_SIZE);
}

void IRCWindow::resetHistoryPosition()
{
	m_historyPosition = m_history.end();
}

void IRCWindow::updateControls()
{
	OS_ASSERT(m_session != nullptr);

	IRCConnectionStatus connectionStatus = m_session->getConnectionStatus();

	bool enableMessageCommands = connectionStatus == ircConnectionStatusConnected;

	IIRCPage *activePage = getActivePage();

	IIRCRoomPage *roomPage = dynamic_cast<IIRCRoomPage *>(activePage);
	if(roomPage != nullptr)
	{
		if(roomPage->getRoom() == nullptr)
			enableMessageCommands = false;
	}

	m_messageCtrl->Enable(enableMessageCommands);
	m_sendButton->Enable(enableMessageCommands && (m_messageCtrl->GetValue().IsEmpty() == false));

	m_buttonToggleConnection->setChecked(connectionStatus == ircConnectionStatusConnected);
	m_buttonToggleConnection->Enable(connectionStatus != ircConnectionStatusConnecting);

	m_buttonRefreshChannels->Enable(connectionStatus == ircConnectionStatusConnected);
	m_buttonClearServerMessages->Enable(activePage == m_serverPage);
}

void IRCWindow::updateMessage(const String &str)
{
	m_historyLocked = true;
	m_messageCtrl->SetValue(conversions::from_utf16<wxString>(str));
	m_messageCtrl->SetInsertionPointEnd();
	m_historyLocked = false;
}

void IRCWindow::onSendMessage(wxCommandEvent &e)
{
	String command = algorithms::trim_copy(conversions::to_utf16(m_messageCtrl->GetValue()));
	if(command.empty())
		return;

	saveHistory(command, true);

	if(command.starts_with("/"))
	{
		m_serverPage->processCommand(command.mid(1));
	}
	else
	{
		IIRCPage *activePage = getActivePage();
		OS_ASSERT(activePage != nullptr);
		if(activePage != nullptr)
			activePage->processCommand(command);
	}

	updateMessage(constants::empty_string);
}

void IRCWindow::onMessageUpdated(wxCommandEvent &e)
{
	e.Skip();

	// Resetta la posizione dell'history se il testo è stato modificato dall'utente
	if(m_historyLocked == false)
		resetHistoryPosition();

	updateControls();
}

void IRCWindow::onMessageKeyDown(wxKeyEvent &e)
{
	switch(e.GetKeyCode())
	{
	case WXK_UP:		browseHistory(false);
						break;

	case WXK_DOWN:		browseHistory(true);
						break;

	default:			e.Skip();
						break;
	}
}

void IRCWindow::onToggleConnection(wxCommandEvent &e)
{
	if(m_session->getConnectionStatus() == ircConnectionStatusDisconnected)
		openConnection();
	else
		closeConnection();	// Chiude la connessione sia in stato di ircConnectionStatusConnecting che ircConnectionStatusConnected

	updateControls();
}

void IRCWindow::onEditOptions(wxCommandEvent &e)
{
	editOptions();
}

void IRCWindow::onRefreshChannels(wxCommandEvent &e)
{
	refreshChannels();
}

void IRCWindow::onClearServerMessages(wxCommandEvent &e)
{
	m_serverPage->clearMessages();
}

void IRCWindow::onPageClose(wxAuiNotebookEvent &e)
{
	if(e.GetSelection() == m_pages->GetPageIndex(m_serverPage))
	{
		e.Veto();
	}
	else
	{
		IIRCPage *page = dynamic_cast<IIRCPage *>(m_pages->GetPage(e.GetSelection()));
		OS_ASSERT(page != nullptr);
		if(page != nullptr)
			page->handlePageClosed();
	}
}

void IRCWindow::onPageChanged(wxAuiNotebookEvent &e)
{
	updatePagesStyle();
	updateControls();

	int32 selection = e.GetSelection();
	if(selection != -1)
		m_pages->SetPageBitmap(selection, wxNullBitmap);

	e.Skip();
}

void IRCWindow::onUpdate(wxCommandEvent &e)
{
	// Aggiorna prima le stanze per assicurare che vengano creati gli handlers dei comandi
	if(updateRooms() || updateCommands())
		postUpdateCommandsEvent();
}

void IRCWindow::onProcessNotify(wxCommandEvent &e)
{
	handleNotify(static_cast<IRCSession::NotifyType>(e.GetInt()));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
