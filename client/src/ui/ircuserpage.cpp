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
#include "ircuserpage.h"

#include "ircroom.h"
#include "irctextctrl.h"
#include "ircuser.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(IRCUserPage, WindowBase)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

IRCUserPage::IRCUserPage(IRCWindow *ircWindow, wxWindow *parent, shared_ptr<IIRCTarget> target) : WindowBase(ircWindow, parent, target),
																								  m_eventsCtrl(null)
{
	OS_ASSERT(target->getType() == ircTargetTypeUser);

	createLayout();
	initWindow();
}

IRCUserPage::~IRCUserPage()
{
	
}

void IRCUserPage::createLayout()
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_eventsCtrl = new IRCTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	mainSizer->Add( m_eventsCtrl, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( mainSizer );
	this->Layout();
}

void IRCUserPage::initWindow()
{

}

IRCTextCtrl * IRCUserPage::getEventsCtrl() const
{
	return m_eventsCtrl;
}

bool IRCUserPage::matchMessageTarget(const std::string &sender, const std::string &target)
{
	OS_ASSERT(target.empty() == false);

	shared_ptr<IRCSession> session = getSession();
	if(session == null)
		return false;

	shared_ptr<IRCRoom> room = getRoom();
	if(room == null)
		return false;

	if((session->getNick() == sender) && (room->getName() == target))
		return true;

	if((session->getNick() == target) && (room->getName() == sender))
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
