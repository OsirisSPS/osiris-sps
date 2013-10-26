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
#include "optionswizardpageproxy.h"

#include "application.h"
#include "conversions.h"
#include "options.h"
#include "wxcustomclientdata.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static const uint32 idProxyType = wxNewId();

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(OptionsWizardPageProxy, WizardPageBase)	
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

OptionsWizardPageProxy::OptionsWizardPageProxy(wxWizard *parent) : WizardPageBase(parent)
{
	createLayout();
	initWindow();
}

OptionsWizardPageProxy::~OptionsWizardPageProxy()
{
	cleanupWindow();
}

void OptionsWizardPageProxy::createLayout()
{
	this->SetSizeHints( wxSize( 562,352 ), wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* pageCaptionSizer;
	pageCaptionSizer = new wxBoxSizer( wxVERTICAL );

	wxPanel* pageCaptionPanel;
	pageCaptionPanel = new wxPanel( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	pageCaptionPanel->SetBackgroundColour( wxColour( 255, 255, 255 ) );

	wxBoxSizer* pageCaptionPanelSizer;
	pageCaptionPanelSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* pageCaptionPanelIconSizer;
	pageCaptionPanelIconSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pageIcon = new wxStaticBitmap( pageCaptionPanel, static_cast<wxWindowID>(wxID_ANY), wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	pageCaptionPanelIconSizer->Add( m_pageIcon, 0, wxALL, 5 );

	m_pageCaptionTitle = new wxStaticText( pageCaptionPanel, static_cast<wxWindowID>(wxID_ANY), wxT("General"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pageCaptionTitle->Wrap( -1 );
	m_pageCaptionTitle->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 77, 90, 92, false, wxEmptyString ) );
	m_pageCaptionTitle->SetForegroundColour( wxColour( 0, 0, 0 ) );

	pageCaptionPanelIconSizer->Add( m_pageCaptionTitle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	pageCaptionPanelSizer->Add( pageCaptionPanelIconSizer, 1, wxEXPAND, 5 );

	m_pageCaptionDescription = new wxStaticText( pageCaptionPanel, static_cast<wxWindowID>(wxID_ANY), wxT("MyLabel"), wxPoint( -1,-1 ), wxDefaultSize, 0 );
	m_pageCaptionDescription->Wrap( -1 );
	m_pageCaptionDescription->SetForegroundColour( wxColour( 0, 0, 0 ) );

	pageCaptionPanelSizer->Add( m_pageCaptionDescription, 0, wxALL, 6 );

	pageCaptionPanel->SetSizer( pageCaptionPanelSizer );
	pageCaptionPanel->Layout();
	pageCaptionPanelSizer->Fit( pageCaptionPanel );
	pageCaptionSizer->Add( pageCaptionPanel, 0, wxEXPAND | wxALL, 0 );

	mainSizer->Add( pageCaptionSizer, 0, wxEXPAND, 0 );

	wxStaticLine* pageCaptionSeparator;
	pageCaptionSeparator = new wxStaticLine( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( pageCaptionSeparator, 0, wxEXPAND | wxALL, 0 );

	m_proxyBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("Proxy") ), wxVERTICAL );

	wxFlexGridSizer* proxyFlexGridSizer;
	proxyFlexGridSizer = new wxFlexGridSizer( 5, 2, 0, 0 );
	proxyFlexGridSizer->AddGrowableCol( 1 );
	proxyFlexGridSizer->SetFlexibleDirection( wxBOTH );
	proxyFlexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_proxyTypeLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("Proxy type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_proxyTypeLabel->Wrap( -1 );
	proxyFlexGridSizer->Add( m_proxyTypeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_proxyTypeChoices;
	m_proxyType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_proxyTypeChoices, 0 );
	m_proxyType->SetSelection( 0 );
	proxyFlexGridSizer->Add( m_proxyType, 0, wxALL|wxEXPAND, 5 );

	m_proxyHostLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("Proxy host"), wxDefaultPosition, wxDefaultSize, 0 );
	m_proxyHostLabel->Wrap( -1 );
	proxyFlexGridSizer->Add( m_proxyHostLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_proxyHost = new wxTextCtrl( this, static_cast<wxWindowID>(wxID_ANY), wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	proxyFlexGridSizer->Add( m_proxyHost, 0, wxALL|wxEXPAND, 5 );

	m_proxyPortLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("Proxy port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_proxyPortLabel->Wrap( -1 );
	proxyFlexGridSizer->Add( m_proxyPortLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_proxyPort = new wxTextCtrl( this, static_cast<wxWindowID>(wxID_ANY), wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	proxyFlexGridSizer->Add( m_proxyPort, 0, wxALL|wxEXPAND, 5 );

	m_proxyUsernameLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("Proxy user"), wxDefaultPosition, wxDefaultSize, 0 );
	m_proxyUsernameLabel->Wrap( -1 );
	proxyFlexGridSizer->Add( m_proxyUsernameLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_proxyUsername = new wxTextCtrl( this, static_cast<wxWindowID>(wxID_ANY), wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	proxyFlexGridSizer->Add( m_proxyUsername, 0, wxALL|wxEXPAND, 5 );

	m_proxyPasswordLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("Proxy password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_proxyPasswordLabel->Wrap( -1 );
	proxyFlexGridSizer->Add( m_proxyPasswordLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_proxyPassword = new wxTextCtrl( this, static_cast<wxWindowID>(wxID_ANY), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	proxyFlexGridSizer->Add( m_proxyPassword, 0, wxALL|wxEXPAND, 5 );

	m_proxyBoxSizer->Add( proxyFlexGridSizer, 0, wxEXPAND, 5 );

	m_proxyRestrictToUnsafeConnections = new wxCheckBox( this, wxID_ANY, wxT("Restrict to unsafe connections"), wxDefaultPosition, wxDefaultSize, 0 );

	m_proxyBoxSizer->Add( m_proxyRestrictToUnsafeConnections, 0, wxALL, 5 );

	mainSizer->Add( m_proxyBoxSizer, 1, wxALL|wxEXPAND, 2 );

	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );
}

void OptionsWizardPageProxy::initWindow()
{
	m_pageCaptionTitle->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_caption_title"))));
	m_pageCaptionDescription->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_caption_description"))));
	m_pageIcon->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("options_wizard_proxy.png"))), wxBITMAP_TYPE_PNG));
	m_proxyBoxSizer->GetStaticBox()->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_proxy_box"))));
	m_proxyTypeLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_proxy_type"))));
	m_proxyHostLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_proxy_host"))));
	m_proxyPortLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_proxy_port"))));
	m_proxyUsernameLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_proxy_username"))));
	m_proxyPasswordLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_proxy_password"))));
	m_proxyRestrictToUnsafeConnections->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_proxy_restrict_to_unsafe_connections"))));

	m_proxyType->Append(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_proxy_type_none"))), new wxCustomClientData<uint32>(OS_PROXY_TYPE_NONE));
	m_proxyType->Append(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_proxy.label_proxy_type_http"))), new wxCustomClientData<uint32>(OS_PROXY_TYPE_HTTP));
	m_proxyType->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(OptionsWizardPageProxy::onProxySelChanged), NULL, this);
}

void OptionsWizardPageProxy::cleanupWindow()
{
	m_proxyType->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(OptionsWizardPageProxy::onProxySelChanged), NULL, this);
}

void OptionsWizardPageProxy::updateProxyControls()
{
	bool enable = m_proxyType->GetSelection() > 0;
	m_proxyHostLabel->Enable(enable);
	m_proxyHostLabel->Enable(enable);
	m_proxyHost->Enable(enable);
	m_proxyPortLabel->Enable(enable);
	m_proxyPort->Enable(enable);
	m_proxyUsernameLabel->Enable(enable);
	m_proxyUsername->Enable(enable);
	m_proxyPasswordLabel->Enable(enable);
	m_proxyPassword->Enable(enable);
	m_proxyRestrictToUnsafeConnections->Enable(enable);
}

void OptionsWizardPageProxy::loadOptions()
{
	m_proxyType->SetSelection(Options::instance()->getOption<uint32>(Options::net_options::proxy_type));
	m_proxyHost->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(Options::net_options::proxy_host)));
	m_proxyPort->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(Options::net_options::proxy_port)));
	m_proxyUsername->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(Options::net_options::proxy_username)));
	m_proxyPassword->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(Options::net_options::proxy_password)));
	m_proxyRestrictToUnsafeConnections->SetValue(Options::instance()->getOption<bool>(Options::net_options::proxy_restrict_to_unsafe_connections));

	updateProxyControls();
}

void OptionsWizardPageProxy::saveOptions()
{
	int32 proxySelection = m_proxyType->GetSelection();
	OS_ASSERT(proxySelection != -1);
	if(proxySelection != -1)
	{
		wxCustomClientData<uint32> *proxyTypeData = dynamic_cast<wxCustomClientData<uint32> *>(m_proxyType->GetClientObject(proxySelection));
		OS_ASSERT(proxyTypeData != null);
		if(proxyTypeData != null)
			Options::instance()->setOption(Options::net_options::proxy_type, proxyTypeData->getValue());
	}

	Options::instance()->setOption(Options::net_options::proxy_host, conversions::to_utf16(m_proxyHost->GetValue()));
	Options::instance()->setOption(Options::net_options::proxy_port, conversions::from_utf16<uint32>(conversions::to_utf16(m_proxyPort->GetValue())));
	Options::instance()->setOption(Options::net_options::proxy_username, conversions::to_utf16(m_proxyUsername->GetValue()));
	Options::instance()->setOption(Options::net_options::proxy_password, conversions::to_utf16(m_proxyPassword->GetValue()));
	Options::instance()->setOption(Options::net_options::proxy_restrict_to_unsafe_connections, m_proxyRestrictToUnsafeConnections->GetValue());
}

void OptionsWizardPageProxy::onProxySelChanged(wxCommandEvent &e)
{
	updateProxyControls();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
