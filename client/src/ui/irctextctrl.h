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

#ifndef _OS_UI_IRCTEXTCTRL_H
#define _OS_UI_IRCTEXTCTRL_H

//////////////////////////////////////////////////////////////////////

#define OS_IRC_TEXTCTRL_MAX_LINES			200

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCTextCtrl : public wxRichTextCtrl
{
	typedef wxRichTextCtrl CtrlBase;

// Construction
public:
    IRCTextCtrl(wxWindow *parent, wxWindowID id = -1, const wxString &value = wxEmptyString, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxRE_MULTILINE, const wxValidator &validator = wxDefaultValidator, const wxString &name = wxTextCtrlNameStr); 
	virtual ~IRCTextCtrl();

// Operations
public:
	void addUserMessage(const std::string &user, const String &message);
	void addServerMessage(const String &message);

	void limitLines();

private:
	void preAddMessage();
	void postAddMessage();
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_IRCTEXTCTRL_H
