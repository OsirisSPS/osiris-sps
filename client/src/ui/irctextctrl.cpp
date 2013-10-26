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
#include "irctextctrl.h"

#include "conversions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IRCTextCtrl::IRCTextCtrl(wxWindow *parent, wxWindowID id, const wxString &value, const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator, const wxString &name) : CtrlBase(parent, id, value, pos, size, style, validator, name)
{
	GetCaret()->Hide();
}

IRCTextCtrl::~IRCTextCtrl()
{
	
}

void IRCTextCtrl::addUserMessage(const std::string &user, const String &message)
{
	preAddMessage();

	BeginBold();
	WriteText(conversions::from_string<wxString>(user + ": "));
	EndBold();

	WriteText(conversions::from_utf16<wxString>(message));	
	
	postAddMessage();
}

void IRCTextCtrl::addServerMessage(const String &message)
{
	preAddMessage();

	WriteText(conversions::from_utf16<wxString>(message));	

	postAddMessage();
}

void IRCTextCtrl::limitLines()
{
	/*
	while(GetNumberOfLines() > 2)//OS_IRC_TEXTCTRL_MAX_LINES)
	{
		qui
	}
	*/
}

void IRCTextCtrl::preAddMessage()
{
	SetInsertionPointEnd();

	if(GetLastPosition() > 0)
		Newline();
}

void IRCTextCtrl::postAddMessage()
{
	// eventsCtrl->ShowPosition in alcune circostanze non funziona
	ScrollIntoView(GetLastPosition(), WXK_DOWN);

	limitLines();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
