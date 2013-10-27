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
#include "ircserverpage.h"

#include "algorithms.h"
#include "conversions.h"
#include "ircchannel.h"
#include "irccommandservermessage.h"
#include "irccommandraw.h"
#include "irctextctrl.h"
#include "ircwindow.h"
#include "locked.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(IRCServerPage, WindowBase)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

IRCServerPage::IRCServerPage(IRCWindow *ircWindow, wxWindow *parent) : WindowBase(ircWindow, parent),
																	   m_eventsCtrl(null),
																	   m_channelsCtrl(null),
																	   m_splitter(null)
{
	createLayout();
	initWindow();
}

IRCServerPage::~IRCServerPage()
{
	cleanupWindow();	
}

void IRCServerPage::handleCommand(shared_ptr<IIRCCommand> command)
{
	if(command->getType() == ircCommandTypeServerMessage)
		addServerMessage(boost::dynamic_pointer_cast<IRCCommandServerMessage>(command)->getMessage());
}

void IRCServerPage::clearMessages()
{
	m_eventsCtrl->Clear();
}

void IRCServerPage::updateChannels()
{
	wxWindowUpdateLocker windowLocker(this);

	m_channelsCtrl->DeleteAllItems();

	shared_ptr<IRCSession> session = getSession();
	if(session == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	Locked<const IRCSession::Channels>::unique channels = session->getChannels();
	for(IRCSession::Channels::const_iterator i = channels->begin(); i != channels->end(); ++i)
	{
		shared_ptr<IRCChannel> channel = i->second;
		if(channel == null)
		{
			OS_ASSERTFALSE();
			continue;
		}

		int32 index = m_channelsCtrl->InsertItem(m_channelsCtrl->GetItemCount(), wxEmptyString);
		if(index != -1)
		{
			m_channelsCtrl->SetItem(index, 0, conversions::from_string<wxString>(channel->getName()));
			m_channelsCtrl->SetItem(index, 1, conversions::from_string<wxString>(conversions::to_string(channel->getUsers())));
			m_channelsCtrl->SetItem(index, 2, conversions::from_string<wxString>(channel->getDescription()));
		}
	}
}

void IRCServerPage::createLayout()
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_splitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE );
	m_splitter->Connect( wxEVT_IDLE, wxIdleEventHandler( IRCServerPage::m_splitterOnIdle ), NULL, this );
	wxPanel* panelEvents;
	panelEvents = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* eventsSizer;
	eventsSizer = new wxBoxSizer( wxVERTICAL );
	
	m_eventsCtrl = new IRCTextCtrl( panelEvents, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	eventsSizer->Add( m_eventsCtrl, 1, wxEXPAND | wxALL, 0 );
	
	panelEvents->SetSizer( eventsSizer );
	panelEvents->Layout();
	eventsSizer->Fit( panelEvents );
	wxPanel* panelChannels;
	panelChannels = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* channelsSizer;
	channelsSizer = new wxBoxSizer( wxVERTICAL );
	
	m_channelsCtrl = new wxListCtrl( panelChannels, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	channelsSizer->Add( m_channelsCtrl, 1, wxALL|wxEXPAND, 0 );
	
	panelChannels->SetSizer( channelsSizer );
	panelChannels->Layout();
	channelsSizer->Fit( panelChannels );
	m_splitter->SplitHorizontally( panelEvents, panelChannels, 150 );
	mainSizer->Add( m_splitter, 1, wxEXPAND, 0 );
	
	this->SetSizer( mainSizer );
	this->Layout();
}

void IRCServerPage::initWindow()
{
	m_channelsCtrl->InsertColumn(0, wxT("Name"), wxLIST_FORMAT_LEFT, 100);
	m_channelsCtrl->InsertColumn(1, wxT("Users"), wxLIST_FORMAT_LEFT, 100);
	m_channelsCtrl->InsertColumn(2, wxT("Description"), wxLIST_FORMAT_LEFT, 200);

	m_channelsCtrl->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(IRCServerPage::onChannelActivated), NULL, this);
}

void IRCServerPage::cleanupWindow()
{
	m_channelsCtrl->Disconnect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(IRCServerPage::onChannelActivated), NULL, this);
}

void IRCServerPage::m_splitterOnIdle(wxIdleEvent &e)
{
	// N.B.: sembra che l'unico metodo per inizializzare la posizione dello splitter sia gestire l'evento di idle...

	m_splitter->SetSashPosition(0);	// Centra lo splitter
	m_splitter->Disconnect(wxEVT_IDLE, wxIdleEventHandler(IRCServerPage::m_splitterOnIdle), NULL, this);
}

void IRCServerPage::addServerMessage(const String &message)
{
	m_eventsCtrl->addServerMessage(message);

	getIRCWindow()->notifyPage(this);
}

void IRCServerPage::onChannelActivated(wxListEvent &e)
{
	e.Skip();

	int32 index = e.GetIndex();
	if(index != -1)
		getIRCWindow()->openChannel(conversions::to_string(m_channelsCtrl->GetItemText(index)));	
}

bool IRCServerPage::processCommand(const String &command)
{
	OS_ASSERT(command.ends_with(OS_IRC_COMMANDS_TERMINATOR) == false);

	shared_ptr<IRCSession> session = getSession();
	if(session == null)
		return false;

	return session->addRequest(shared_ptr<IRCCommandRaw>(OS_NEW IRCCommandRaw(session, command.to_utf8() + OS_IRC_COMMANDS_TERMINATOR)), true);
}

void IRCServerPage::updatePage()
{
	WindowBase::updatePage();

	if(getSession()->getConnectionStatus() == ircConnectionStatusDisconnected)
		m_channelsCtrl->DeleteAllItems();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
