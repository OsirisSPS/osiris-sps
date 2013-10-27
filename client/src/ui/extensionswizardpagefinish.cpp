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
#include "extensionswizardpagefinish.h"

#include "application.h"
#include "conversions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(ExtensionsWizardPageFinish, WizardPageBase)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

ExtensionsWizardPageFinish::ExtensionsWizardPageFinish(wxWizard *parent) : WizardPageBase(parent)
{
	createLayout();
	initWindow();
}

ExtensionsWizardPageFinish::~ExtensionsWizardPageFinish()
{

}

void ExtensionsWizardPageFinish::createLayout()
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
	
	m_pageCaptionDescription = new wxStaticText( pageCaptionPanel, static_cast<wxWindowID>(wxID_ANY), wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pageCaptionDescription->Wrap( -1 );
	m_pageCaptionDescription->SetForegroundColour( wxColour( 0, 0, 0 ) );
	
	pageCaptionPanelSizer->Add( m_pageCaptionDescription, 0, wxALL, 6 );
	
	pageCaptionPanel->SetSizer( pageCaptionPanelSizer );
	pageCaptionPanel->Layout();
	pageCaptionPanelSizer->Fit( pageCaptionPanel );
	pageCaptionSizer->Add( pageCaptionPanel, 1, wxEXPAND | wxALL, 0 );
	
	mainSizer->Add( pageCaptionSizer, 0, wxEXPAND, 0 );
	
	wxStaticLine* pageCaptionSeparator;
	pageCaptionSeparator = new wxStaticLine( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( pageCaptionSeparator, 0, wxEXPAND | wxALL, 0 );
	
	wxBoxSizer* labelsSizer;
	labelsSizer = new wxBoxSizer( wxVERTICAL );
	
	m_descriptionLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_descriptionLabel->Wrap( -1 );
	labelsSizer->Add( m_descriptionLabel, 0, wxALL, 5 );
	
	m_openExtensionFolder = new wxCheckBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("Open extension folder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_openExtensionFolder->SetValue(true);
	
	labelsSizer->Add( m_openExtensionFolder, 0, wxALL, 5 );
	
	mainSizer->Add( labelsSizer, 1, wxEXPAND, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );
}

void ExtensionsWizardPageFinish::initWindow()
{
	m_pageCaptionTitle->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_extensions.page_finish.label_caption_title"))));
	m_pageCaptionDescription->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_extensions.page_finish.label_caption_description"))));
	m_pageIcon->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("extensions_wizard_finish.png"))), wxBITMAP_TYPE_PNG));
	m_descriptionLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_extensions.page_finish.label_description"))));
	m_openExtensionFolder->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_extensions.page_finish.label_open_extension_folder"))));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
