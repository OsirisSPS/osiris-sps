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

#ifndef _OS_UI_IRCCONFIGWINDOW_H
#define _OS_UI_IRCCONFIGWINDOW_H

#include "multilanguagewindow.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCConfigWindow : public wxDialog,
						public MultilanguageWindow
{
	DECLARE_EVENT_TABLE()

	typedef wxDialog WindowBase;

// Construction
public:
    IRCConfigWindow(wxWindow *parent, wxWindowID id);
	virtual ~IRCConfigWindow();

// Operations
public:
	void saveConfig();

private:
	void loadConfig();

	void createLayout();
	void initWindow();
	void cleanupWindow();

	void refreshControls();

// Events
private:
	void onControlsChanged(wxCommandEvent &e);

private:
	wxAuiNotebook *m_pages;
	wxStaticText *m_usernameLabel;
	wxTextCtrl *m_usernameText;
	wxStaticText *m_emailLabel;
	wxTextCtrl *m_emailText;
	wxStaticText *m_nicknameLabel;
	wxTextCtrl *m_nicknameText;
	wxStaticText *m_alternateNicknameLabel;
	wxTextCtrl *m_alternateNicknameText;
	wxStaticText *m_defaultChannelLabel;
	wxTextCtrl *m_defaultChannelText;
	wxCheckBox *m_automaticConnectionCheckBox;
	wxStaticText *m_serverLabel;
	wxTextCtrl *m_serverText;
	wxStaticText *m_portLabel;
	wxTextCtrl *m_portText;
	wxStaticText *m_portSslLabel;
	wxTextCtrl *m_portSslText;
	wxCheckBox *m_enableSslCheckBox;
	wxStaticText *m_timeoutLabel;
	wxTextCtrl *m_timeoutText;	
	wxGridBagSizer *m_basicPageGridSizer;
	wxGridBagSizer *m_advancedPageGridSizer;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_IRCCONFIGWINDOW_H
