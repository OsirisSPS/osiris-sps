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
#include "ircconfigwindow.h"

#include "conversions.h"
#include "options.h"
#include "uistandards.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(IRCConfigWindow, WindowBase)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

IRCConfigWindow::IRCConfigWindow(wxWindow *parent, wxWindowID id) : WindowBase(parent, id, wxEmptyString, wxDefaultPosition, wxSize(400, 300), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)
{
	createLayout();	
	initWindow();

	SetSizeHints(230, 276);
}

IRCConfigWindow::~IRCConfigWindow()
{
	cleanupWindow();
}

void IRCConfigWindow::saveConfig()
{
	Options::instance()->setOption(OS_IRC_OPTION_USERNAME, conversions::to_utf16(m_usernameText->GetValue()));
	Options::instance()->setOption(OS_IRC_OPTION_EMAIL, conversions::to_utf16(m_emailText->GetValue()));
	Options::instance()->setOption(OS_IRC_OPTION_NICKNAME, conversions::to_utf16(m_nicknameText->GetValue()));
	Options::instance()->setOption(OS_IRC_OPTION_ALTERNATE_NICKNAME, conversions::to_utf16(m_alternateNicknameText->GetValue()));
	Options::instance()->setOption(OS_IRC_OPTION_DEFAULT_CHANNEL, conversions::to_utf16(m_defaultChannelText->GetValue()));
	Options::instance()->setOption(OS_IRC_OPTION_SERVER_AUTOMATIC_CONNECTION, m_automaticConnectionCheckBox->GetValue());
	Options::instance()->setOption(OS_IRC_OPTION_SERVER_ADDRESS, conversions::to_utf16(m_serverText->GetValue()));
	Options::instance()->setOption(OS_IRC_OPTION_SERVER_PORT, conversions::from_utf16<uint32>(conversions::to_utf16(m_portText->GetValue())));
	Options::instance()->setOption(OS_IRC_OPTION_SERVER_PORT_SSL, conversions::from_utf16<uint32>(conversions::to_utf16(m_portSslText->GetValue())));
	Options::instance()->setOption(OS_IRC_OPTION_SERVER_ENABLE_SSL, m_enableSslCheckBox->GetValue());
	Options::instance()->setOption(OS_IRC_OPTION_SERVER_TIMEOUT, conversions::from_utf16<uint32>(conversions::to_utf16(m_timeoutText->GetValue())));
}

void IRCConfigWindow::loadConfig()
{
	m_usernameText->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(OS_IRC_OPTION_USERNAME)));
	m_emailText->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(OS_IRC_OPTION_EMAIL)));
	m_nicknameText->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(OS_IRC_OPTION_NICKNAME)));
	m_alternateNicknameText->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(OS_IRC_OPTION_ALTERNATE_NICKNAME)));
	m_defaultChannelText->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(OS_IRC_OPTION_DEFAULT_CHANNEL)));
	m_automaticConnectionCheckBox->SetValue(Options::instance()->getOption<bool>(OS_IRC_OPTION_SERVER_AUTOMATIC_CONNECTION));
	m_serverText->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(OS_IRC_OPTION_SERVER_ADDRESS)));
	m_portText->SetValue(conversions::from_utf16<wxString>(conversions::to_utf16(Options::instance()->getOption<uint32>(OS_IRC_OPTION_SERVER_PORT))));
	m_portSslText->SetValue(conversions::from_utf16<wxString>(conversions::to_utf16(Options::instance()->getOption<uint32>(OS_IRC_OPTION_SERVER_PORT_SSL))));
	m_enableSslCheckBox->SetValue(Options::instance()->getOption<bool>(OS_IRC_OPTION_SERVER_ENABLE_SSL));
	m_timeoutText->SetValue(conversions::from_utf16<wxString>(conversions::to_utf16(Options::instance()->getOption<uint32>(OS_IRC_OPTION_SERVER_TIMEOUT))));
}

void IRCConfigWindow::createLayout()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* contentSizer;
	contentSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pages = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_FIXED_WIDTH );
	wxPanel* basicPage;
	basicPage = new wxPanel( m_pages, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* basicPageSizer;
	basicPageSizer = new wxBoxSizer( wxVERTICAL );
	
	m_basicPageGridSizer = new wxGridBagSizer( 0, 0 );
	m_basicPageGridSizer->SetFlexibleDirection( wxBOTH );
	m_basicPageGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_nicknameLabel = new wxStaticText( basicPage, wxID_ANY, wxT("Nickname"), wxDefaultPosition, wxDefaultSize, 0 );
	m_nicknameLabel->Wrap( -1 );
	m_basicPageGridSizer->Add( m_nicknameLabel, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 5 );
	
	m_nicknameText = new wxTextCtrl( basicPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_basicPageGridSizer->Add( m_nicknameText, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_alternateNicknameLabel = new wxStaticText( basicPage, wxID_ANY, wxT("Alternate nickname"), wxDefaultPosition, wxDefaultSize, 0 );
	m_alternateNicknameLabel->Wrap( -1 );
	m_basicPageGridSizer->Add( m_alternateNicknameLabel, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 5 );
	
	m_alternateNicknameText = new wxTextCtrl( basicPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_basicPageGridSizer->Add( m_alternateNicknameText, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_defaultChannelLabel = new wxStaticText( basicPage, wxID_ANY, wxT("Default channel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_defaultChannelLabel->Wrap( -1 );
	m_basicPageGridSizer->Add( m_defaultChannelLabel, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 5 );
	
	m_defaultChannelText = new wxTextCtrl( basicPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_basicPageGridSizer->Add( m_defaultChannelText, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_automaticConnectionCheckBox = new wxCheckBox( basicPage, wxID_ANY, wxT("Automatic connection"), wxDefaultPosition, wxDefaultSize, 0 );
	
	m_basicPageGridSizer->Add( m_automaticConnectionCheckBox, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxALL, 5 );
	
	m_usernameLabel = new wxStaticText( basicPage, wxID_ANY, wxT("Username"), wxDefaultPosition, wxDefaultSize, 0 );
	m_usernameLabel->Wrap( -1 );
	m_basicPageGridSizer->Add( m_usernameLabel, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 5 );
	
	m_usernameText = new wxTextCtrl( basicPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_basicPageGridSizer->Add( m_usernameText, wxGBPosition( 4, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_emailLabel = new wxStaticText( basicPage, wxID_ANY, wxT("Email"), wxDefaultPosition, wxDefaultSize, 0 );
	m_emailLabel->Wrap( -1 );
	m_basicPageGridSizer->Add( m_emailLabel, wxGBPosition( 5, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 5 );
	
	m_emailText = new wxTextCtrl( basicPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_basicPageGridSizer->Add( m_emailText, wxGBPosition( 5, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	basicPageSizer->Add( m_basicPageGridSizer, 1, wxEXPAND, 5 );
	
	basicPage->SetSizer( basicPageSizer );
	basicPage->Layout();
	basicPageSizer->Fit( basicPage );
	m_pages->AddPage( basicPage, wxT("Basic"), true, wxNullBitmap );
	wxPanel* advancedPage;
	advancedPage = new wxPanel( m_pages, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* advancedPageSizer;
	advancedPageSizer = new wxBoxSizer( wxVERTICAL );
	
	m_advancedPageGridSizer = new wxGridBagSizer( 0, 0 );
	m_advancedPageGridSizer->SetFlexibleDirection( wxBOTH );
	m_advancedPageGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_serverLabel = new wxStaticText( advancedPage, wxID_ANY, wxT("Server"), wxDefaultPosition, wxDefaultSize, 0 );
	m_serverLabel->Wrap( -1 );
	m_advancedPageGridSizer->Add( m_serverLabel, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 5 );
	
	m_serverText = new wxTextCtrl( advancedPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_advancedPageGridSizer->Add( m_serverText, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_portLabel = new wxStaticText( advancedPage, wxID_ANY, wxT("Port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_portLabel->Wrap( -1 );
	m_advancedPageGridSizer->Add( m_portLabel, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 5 );
	
	m_portText = new wxTextCtrl( advancedPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_advancedPageGridSizer->Add( m_portText, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_portSslLabel = new wxStaticText( advancedPage, wxID_ANY, wxT("SSL Port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_portSslLabel->Wrap( -1 );
	m_advancedPageGridSizer->Add( m_portSslLabel, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 5 );
	
	m_portSslText = new wxTextCtrl( advancedPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_advancedPageGridSizer->Add( m_portSslText, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_enableSslCheckBox = new wxCheckBox( advancedPage, wxID_ANY, wxT("Enable SSL"), wxDefaultPosition, wxDefaultSize, 0 );
	
	m_advancedPageGridSizer->Add( m_enableSslCheckBox, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxALL, 5 );
	
	m_timeoutLabel = new wxStaticText( advancedPage, wxID_ANY, wxT("Timeout"), wxDefaultPosition, wxDefaultSize, 0 );
	m_timeoutLabel->Wrap( -1 );
	m_advancedPageGridSizer->Add( m_timeoutLabel, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 5 );
	
	m_timeoutText = new wxTextCtrl( advancedPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_advancedPageGridSizer->Add( m_timeoutText, wxGBPosition( 4, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	advancedPageSizer->Add( m_advancedPageGridSizer, 1, wxEXPAND, 5 );
	
	advancedPage->SetSizer( advancedPageSizer );
	advancedPage->Layout();
	advancedPageSizer->Fit( advancedPage );
	m_pages->AddPage( advancedPage, wxT("Advanced"), false, wxNullBitmap );
	
	contentSizer->Add( m_pages, 1, wxEXPAND | wxALL, 0 );
	
	mainSizer->Add( contentSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* controlsSizer;
	controlsSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxButton* buttonOk;
	buttonOk = new wxButton( this, wxID_OK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonOk->SetDefault(); 
	controlsSizer->Add( buttonOk, 0, wxALL, 5 );
	
	wxButton* buttonCancel;
	buttonCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	controlsSizer->Add( buttonCancel, 0, wxALL, 5 );
	
	mainSizer->Add( controlsSizer, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
}

void IRCConfigWindow::initWindow()
{
	SetTitle(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.title"))));

	m_usernameLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.username"))));
	m_emailLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.email"))));
	m_nicknameLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.nickname"))));
	m_alternateNicknameLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.alternate_nickname"))));
	m_defaultChannelLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.default_channel"))));
	m_automaticConnectionCheckBox->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.automatic_connection"))));
	m_serverLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.server"))));
	m_portLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.port"))));
	m_portSslLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.port_ssl"))));
	m_enableSslCheckBox->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.enable_ssl"))));
	m_timeoutLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.irc_config_window.timeout"))));	

	loadConfig();

	m_nicknameText->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
	m_serverText->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
	m_portText->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
	m_portSslText->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
	m_timeoutText->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
	m_enableSslCheckBox->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);

	m_pages->SetSelection(0);
	m_nicknameText->SetFocus();

	refreshControls();

	// Il codice generato da designer da problemi (viene richiamato prima della creazione dei controlli figli e le wx lanciano un'assert)
	m_basicPageGridSizer->AddGrowableCol(1);
	m_basicPageGridSizer->Layout();
	m_advancedPageGridSizer->AddGrowableCol(1);	
	m_advancedPageGridSizer->Layout();
}

void IRCConfigWindow::cleanupWindow()
{
	m_nicknameText->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
	m_serverText->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
	m_portText->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
	m_portSslText->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
	m_timeoutText->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
	m_enableSslCheckBox->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(IRCConfigWindow::onControlsChanged), NULL, this);
}

void IRCConfigWindow::refreshControls()
{
	m_nicknameText->SetBackgroundColour(m_nicknameText->GetValue().IsEmpty() ? OS_UI_TEXTBOX_BACKGROUND_INVALID : OS_UI_TEXTBOX_BACKGROUND_STANDARD);
	m_serverText->SetBackgroundColour(m_serverText->GetValue().IsEmpty() ? OS_UI_TEXTBOX_BACKGROUND_INVALID : OS_UI_TEXTBOX_BACKGROUND_STANDARD);
	m_portText->SetBackgroundColour((m_portText->GetValue().IsEmpty() && (m_enableSslCheckBox->GetValue() == false)) ? OS_UI_TEXTBOX_BACKGROUND_INVALID : OS_UI_TEXTBOX_BACKGROUND_STANDARD);
	m_portSslText->SetBackgroundColour((m_portSslText->GetValue().IsEmpty() && m_enableSslCheckBox->GetValue()) ? OS_UI_TEXTBOX_BACKGROUND_INVALID : OS_UI_TEXTBOX_BACKGROUND_STANDARD);
	m_timeoutText->SetBackgroundColour(m_timeoutText->GetValue().IsEmpty() ? OS_UI_TEXTBOX_BACKGROUND_INVALID : OS_UI_TEXTBOX_BACKGROUND_STANDARD);

	Refresh();
}

void IRCConfigWindow::onControlsChanged(wxCommandEvent &e)
{
	refreshControls();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
