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
#include "optionswizardpagenetwork.h"

#include "application.h"
#include "conversions.h"
#include "engine.h"
#include "httpurl.h"
#include "idesystem.h"
#include "isis.h"
#include "options.h"
#include "platformmanager.h"
#include "upnpmanager.h"
#include "xmldocument.h"
#include "xmlnode.h"

#include <wx/busyinfo.h>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static const uint32 idTestP2PPort = wxNewId();
static const uint32 idTestWebPort = wxNewId();

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(OptionsWizardPageNetwork, WizardPageBase)
	EVT_BUTTON(idTestP2PPort, OptionsWizardPageNetwork::onTestP2PPort)
	EVT_BUTTON(idTestWebPort, OptionsWizardPageNetwork::onTestWebPort)	
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

OptionsWizardPageNetwork::OptionsWizardPageNetwork(wxWizard *parent) : WizardPageBase(parent)
{
	createLayout();
	initWindow();
}

OptionsWizardPageNetwork::~OptionsWizardPageNetwork()
{
	cleanupWindow();
}

void OptionsWizardPageNetwork::createLayout()
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
	
	m_networkBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("Network") ), wxHORIZONTAL );
	
	wxGridBagSizer* networkFlexGridSizer;
	networkFlexGridSizer = new wxGridBagSizer( 0, 0 );
	networkFlexGridSizer->SetFlexibleDirection( wxBOTH );
	networkFlexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_webServerPortLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("Web server port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_webServerPortLabel->Wrap( -1 );
	networkFlexGridSizer->Add( m_webServerPortLabel, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_webServerPort = new wxTextCtrl( this, static_cast<wxWindowID>(wxID_ANY), wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	networkFlexGridSizer->Add( m_webServerPort, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_webServerEnableUpnp = new wxCheckBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("enable web UPNP"), wxDefaultPosition, wxDefaultSize, 0 );
	
	networkFlexGridSizer->Add( m_webServerEnableUpnp, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_testWebPort = new wxButton( this, idTestWebPort, wxT("check port"), wxDefaultPosition, wxDefaultSize, 0 );
	networkFlexGridSizer->Add( m_testWebPort, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_p2pServerPortLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("P2P server port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_p2pServerPortLabel->Wrap( -1 );
	networkFlexGridSizer->Add( m_p2pServerPortLabel, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_p2pServerPort = new wxTextCtrl( this, static_cast<wxWindowID>(wxID_ANY), wxEmptyString, wxPoint( -1,-1 ), wxDefaultSize, 0 );
	networkFlexGridSizer->Add( m_p2pServerPort, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_p2pServerEnableUpnp = new wxCheckBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("enable p2p UPNP"), wxDefaultPosition, wxDefaultSize, 0 );
	
	networkFlexGridSizer->Add( m_p2pServerEnableUpnp, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_testP2PPort = new wxButton( this, idTestP2PPort, wxT("check port"), wxDefaultPosition, wxDefaultSize, 0 );
	networkFlexGridSizer->Add( m_testP2PPort, wxGBPosition( 1, 3 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_upnpServiceLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("UPnP service"), wxDefaultPosition, wxDefaultSize, 0 );
	m_upnpServiceLabel->Wrap( -1 );
	networkFlexGridSizer->Add( m_upnpServiceLabel, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_upnpServiceStatus = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("UPnP status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_upnpServiceStatus->Wrap( -1 );
	networkFlexGridSizer->Add( m_upnpServiceStatus, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_upnpServiceIcon = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	networkFlexGridSizer->Add( m_upnpServiceIcon, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_networkBoxSizer->Add( networkFlexGridSizer, 1, wxEXPAND, 5 );
	
	mainSizer->Add( m_networkBoxSizer, 0, wxALL|wxEXPAND, 2 );
	
	m_connectionsBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("Connections") ), wxVERTICAL );
	
	wxFlexGridSizer* connectionsFlexGridSizer;
	connectionsFlexGridSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
	connectionsFlexGridSizer->AddGrowableCol( 1 );
	connectionsFlexGridSizer->SetFlexibleDirection( wxBOTH );
	connectionsFlexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_maxIncomingConnectionsLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("Max incoming connections"), wxDefaultPosition, wxDefaultSize, 0 );
	m_maxIncomingConnectionsLabel->Wrap( -1 );
	connectionsFlexGridSizer->Add( m_maxIncomingConnectionsLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_maxIncomingConnections = new wxTextCtrl( this, static_cast<wxWindowID>(wxID_ANY), wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	connectionsFlexGridSizer->Add( m_maxIncomingConnections, 0, wxALL|wxEXPAND, 5 );
	
	m_maxOutgoingConnectionsLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("Max outgoing connections"), wxDefaultPosition, wxDefaultSize, 0 );
	m_maxOutgoingConnectionsLabel->Wrap( -1 );
	connectionsFlexGridSizer->Add( m_maxOutgoingConnectionsLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_maxOutgoingConnections = new wxTextCtrl( this, static_cast<wxWindowID>(wxID_ANY), wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	connectionsFlexGridSizer->Add( m_maxOutgoingConnections, 0, wxALL|wxEXPAND, 5 );
	
	m_connectionsBoxSizer->Add( connectionsFlexGridSizer, 0, wxEXPAND, 5 );
	
	mainSizer->Add( m_connectionsBoxSizer, 0, wxALL|wxEXPAND, 2 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );
}

void OptionsWizardPageNetwork::initWindow()
{
	
	{
		 wxBusyInfo wait("UPNP checking...");

		 bool upnpAvailable = UPnPManager::instance()->getServiceAvailable();

		 m_upnpServiceLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_upnp_service"))));
		 m_upnpServiceStatus->SetLabel(conversions::from_utf16<wxString>(getText(upnpAvailable ? _S("ui.wizard_options.page_network.upnp_service_available") : _S("ui.wizard_options.page_network.upnp_service_unavailable"))));
		 m_upnpServiceIcon->SetBitmap(upnpAvailable ? wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("upnp_service_available.png"))), wxBITMAP_TYPE_PNG) : wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("upnp_service_unavailable.png"))), wxBITMAP_TYPE_PNG));
	}

	m_pageCaptionTitle->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_caption_title"))));
	m_pageCaptionDescription->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_caption_description"))));
	m_networkBoxSizer->GetStaticBox()->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_network_box"))));
	m_webServerPortLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_web_server_port"))));
	m_p2pServerPortLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_p2p_server_port"))));
	m_webServerEnableUpnp->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_web_server_enable_upnp"))));
	m_p2pServerEnableUpnp->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_p2p_server_enable_upnp"))));
	m_testP2PPort->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_test_p2p_port"))));
	m_testWebPort->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_test_web_port"))));
	
	m_connectionsBoxSizer->GetStaticBox()->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_connections_box"))));
	m_maxIncomingConnectionsLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_max_incoming_connections"))));
	m_maxOutgoingConnectionsLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_network.label_max_outgoing_connections"))));
	m_pageIcon->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("options_wizard_network.png"))), wxBITMAP_TYPE_PNG));

	m_maxIncomingConnections->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(OptionsWizardPageNetwork::onMaxOutgoingConnectionsChanged), NULL, this);

	updateControls();
}

void OptionsWizardPageNetwork::cleanupWindow()
{
	m_maxIncomingConnections->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(OptionsWizardPageNetwork::onMaxOutgoingConnectionsChanged), NULL, this);
}

void OptionsWizardPageNetwork::testPort2(uint32 port, wxButton* button)
{
	wxBusyCursor busyCursor;

	button->SetBackgroundColour(wxColour(255,255,128));

	bool result = false;
	String message;

	String systemUrl = Options::instance()->getIsisSecureLink(OS_ISIS_PAGE_CHECK, "output=xml&act=port&port=" + conversions::to_string(port));
	if(systemUrl.empty())
	{
		message = _S("Internal error.");
	}
	else
	{
		shared_ptr<boost::asio::io_service> service = createAsioService();
		if(service == nullptr)
		{
			message = _S("Internal error.");
		}
		else
		{
			shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
			if(document->parseUrl(systemUrl.to_ascii(), Engine::getDefaultHttpUserAgent(), service, Engine::instance()->createTCPSocket(service, true, true)) == false)
			{
				message = _S("Cannot open url '") + systemUrl + _S("'");				
			}
			else
			{
				shared_ptr<XMLNode> root = document->getRoot();
				if(root == nullptr)
				{
					message = _S("Unknown response.");
				}
				else
				{
					result = root->getAttributeBool(_S("result"));
					message = root->getAttributeString(_S("message"));
				}
			}
		}
	}
	
	if(result)
	{
		wxMessageBox(conversions::from_utf16<wxString>(message), conversions::from_utf16<wxString>(Engine::getVersionName(true)), wxOK | wxICON_INFORMATION, this);
		button->SetBackgroundColour(wxColour(128,255,128));
	}
	else
	{
		wxMessageBox(conversions::from_utf16<wxString>(message), conversions::from_utf16<wxString>(Engine::getVersionName(true)), wxOK | wxICON_ERROR, this);
		button->SetBackgroundColour(wxColour(255,128,128));
	}
}

void OptionsWizardPageNetwork::updateControls()
{
	String isis = Options::instance()->getOption<String>(Options::system_options::isis);
	m_testWebPort->Enable(isis.empty() == false);

	bool enableP2PControls = conversions::from_utf16<uint32>(conversions::to_utf16(m_maxIncomingConnections->GetValue())) > 0;

	m_p2pServerPortLabel->Enable(enableP2PControls);
	m_p2pServerPort->Enable(enableP2PControls);
	m_p2pServerEnableUpnp->Enable(enableP2PControls);
	m_testP2PPort->Enable(enableP2PControls && (isis.empty() == false));
}

void OptionsWizardPageNetwork::loadOptions()
{
	m_p2pServerPort->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(Options::p2p_options::server_port)));
	m_p2pServerEnableUpnp->SetValue(Options::instance()->getOption<bool>(Options::p2p_options::server_enable_upnp));
	m_webServerPort->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(Options::web_options::server_port)));
	m_webServerEnableUpnp->SetValue(Options::instance()->getOption<bool>(Options::web_options::server_enable_upnp));
	m_maxIncomingConnections->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(Options::p2p_options::max_incoming_connections)));
	m_maxOutgoingConnections->SetValue(conversions::from_utf16<wxString>(Options::instance()->getOption<String>(Options::p2p_options::max_outgoing_connections)));
}

void OptionsWizardPageNetwork::saveOptions()
{
	Options::instance()->setOption(Options::p2p_options::server_port, conversions::from_utf16<uint32>(conversions::to_utf16(m_p2pServerPort->GetValue())));
	Options::instance()->setOption(Options::p2p_options::server_enable_upnp, m_p2pServerEnableUpnp->GetValue());
	Options::instance()->setOption(Options::web_options::server_port, conversions::from_utf16<uint32>(conversions::to_utf16(m_webServerPort->GetValue())));
	Options::instance()->setOption(Options::web_options::server_enable_upnp, m_webServerEnableUpnp->GetValue());
	Options::instance()->setOption(Options::p2p_options::max_incoming_connections, conversions::from_utf16<uint32>(conversions::to_utf16(m_maxIncomingConnections->GetValue())));
	Options::instance()->setOption(Options::p2p_options::max_outgoing_connections, conversions::from_utf16<uint32>(conversions::to_utf16(m_maxOutgoingConnections->GetValue())));
}

void OptionsWizardPageNetwork::onMaxOutgoingConnectionsChanged(wxCommandEvent &e)
{
	e.Skip();

	updateControls();
}

void OptionsWizardPageNetwork::onTestP2PPort(wxCommandEvent &e)
{
	if(Options::instance()->getOption<bool>(Options::p2p_options::enable) == false)
	{
		Application::instance()->reportError(getText("ui.application.port_check_without_p2p"));
		return;
	}

	if(Options::instance()->getOption<uint32>(Options::p2p_options::server_port) != conversions::from_utf16<uint32>(conversions::to_utf16(m_p2pServerPort->GetValue())))
	{
		Application::instance()->reportError(getText("ui.application.port_check_different_port"));
		return;
	}

	testPort2(conversions::from_utf16<uint32>(conversions::to_utf16(m_p2pServerPort->GetValue())), m_testP2PPort);
}

void OptionsWizardPageNetwork::onTestWebPort(wxCommandEvent &e)
{
	testPort2(conversions::from_utf16<uint32>(conversions::to_utf16(m_webServerPort->GetValue())), m_testWebPort);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
