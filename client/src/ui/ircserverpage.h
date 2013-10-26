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

#ifndef _OS_UI_IRCSERVERPAGE_H
#define _OS_UI_IRCSERVERPAGE_H

#include "iircpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCTextCtrl;

//////////////////////////////////////////////////////////////////////

class IRCServerPage : public IIRCPage
{
	DECLARE_EVENT_TABLE()

	typedef IIRCPage WindowBase;

// Construction
public:
    IRCServerPage(IRCWindow *ircWindow, wxWindow *parent);
	virtual ~IRCServerPage();

// Operations
public:
	void handleCommand(shared_ptr<IIRCCommand> command);
	void clearMessages();
	void updateChannels();

private:
	void createLayout();
	void initWindow();		
	void cleanupWindow();

	void m_splitterOnIdle(wxIdleEvent &e);	

	void addServerMessage(const String &message);

// Events
private:
	void onChannelActivated(wxListEvent &e);

// IIRCPage interface
public:
	virtual bool processCommand(const String &command);
	
// IIRCPage overrides
public:
	virtual void updatePage();

private:
	IRCTextCtrl *m_eventsCtrl;
	wxListCtrl *m_channelsCtrl;
	wxSplitterWindow *m_splitter;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_IRCSERVERPAGE_H
