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
#include "iircroompage.h"

#include "conversions.h"
#include "iirccommand.h"
#include "iirctarget.h"
#include "irccommandmessage.h"
#include "ircroom.h"
#include "irctextctrl.h"
#include "ircwindow.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(IIRCRoomPage, WindowBase)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

IIRCRoomPage::IIRCRoomPage(IRCWindow *ircWindow, wxWindow *parent, shared_ptr<IIRCTarget> target) : WindowBase(ircWindow, parent),
																								    m_target(target),
																									m_enabled(true)
{
	OS_ASSERT(target != null);
}

IIRCRoomPage::~IIRCRoomPage()
{
	
}

shared_ptr<IRCRoom> IIRCRoomPage::getRoom() const 
{ 
	return getSession()->findRoom(getTarget()->getName());
}

void IIRCRoomPage::addMessage(const std::string &user, const String &message)
{
	IRCTextCtrl *eventsCtrl = getEventsCtrl();
	if(eventsCtrl == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	eventsCtrl->addUserMessage(user, message);

	getIRCWindow()->notifyPage(this);
}

void IIRCRoomPage::updateControls(wxWindow *window, bool enabled)
{
	OS_ASSERT(window != null);

	wxWindowList &controls = window->GetChildren();
	for(wxWindowListNode *control = controls.GetFirst(); control != null; control = control->GetNext())
	{
		updateControls(control->GetData(), enabled);
		control->GetData()->Enable(enabled);
	}
}

bool IIRCRoomPage::processCommand(const String &command)
{
	shared_ptr<IRCRoom> room = getRoom();
	if(room == null)
		return false;

	return room->sendMessage(command);
}

void IIRCRoomPage::handlePageClosed()
{
	WindowBase::handlePageClosed();

	shared_ptr<IRCSession> session = getSession();
	if(session == null)
		return;

	shared_ptr<IRCRoom> room = getRoom();
	if(room == null)
		return;
	
	session->leaveRoom(room);
}

void IIRCRoomPage::updatePage()
{
	WindowBase::updatePage();

	bool enabled = true;

	shared_ptr<IRCSession> session = getSession();
	if((session == null) || (session->getConnectionStatus() != ircConnectionStatusConnected))
		enabled = false;

	if(getRoom() == null)
		enabled = false;

	if(m_enabled != enabled)
	{
		m_enabled = enabled;
		updateControls(this, enabled);
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
