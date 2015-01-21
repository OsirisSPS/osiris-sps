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
#include "optionswizardpagefinish.h"

#include "application.h"
#include "conversions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(OptionsWizardPageFinish, WizardPageBase)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

OptionsWizardPageFinish::OptionsWizardPageFinish(wxWizard *parent) : WizardPageBase(parent)
{
	createLayout();
	initWindow();
}

OptionsWizardPageFinish::~OptionsWizardPageFinish()
{

}

void OptionsWizardPageFinish::createLayout()
{
	this->SetSizeHints( wxSize( 562,352 ), wxDefaultSize );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxPanel* mainPanel;
	mainPanel = new wxPanel( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* controlsSizer;
	controlsSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_leftIcon = new wxStaticBitmap( mainPanel, static_cast<wxWindowID>(wxID_ANY), wxNullBitmap, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	controlsSizer->Add( m_leftIcon, 0, wxALL, 5 );
	
	wxBoxSizer* rightSizer;
	rightSizer = new wxBoxSizer( wxVERTICAL );
	
	m_finishLabel = new wxStaticText( mainPanel, static_cast<wxWindowID>(wxID_ANY), wxT("End"), wxDefaultPosition, wxDefaultSize, 0 );
	m_finishLabel->Wrap( -1 );
	m_finishLabel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	rightSizer->Add( m_finishLabel, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxStaticLine* finishLabelSeparator;
	finishLabelSeparator = new wxStaticLine( mainPanel, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	rightSizer->Add( finishLabelSeparator, 0, wxEXPAND | wxALL, 0 );
	
	wxBoxSizer* labelsSizer;
	labelsSizer = new wxBoxSizer( wxVERTICAL );
	
	m_descriptionLabel = new wxStaticText( mainPanel, static_cast<wxWindowID>(wxID_ANY), wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_descriptionLabel->Wrap( -1 );
	labelsSizer->Add( m_descriptionLabel, 0, wxALL, 5 );
	
	m_viewHome = new wxCheckBox( mainPanel, static_cast<wxWindowID>(wxID_ANY), wxT("View home"), wxDefaultPosition, wxDefaultSize, 0 );
	m_viewHome->SetValue(true);
	
	labelsSizer->Add( m_viewHome, 0, wxALL, 5 );
	
	
	labelsSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_proceedLabel = new wxStaticText( mainPanel, static_cast<wxWindowID>(wxID_ANY), wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_proceedLabel->Wrap( -1 );
	labelsSizer->Add( m_proceedLabel, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	rightSizer->Add( labelsSizer, 1, wxEXPAND, 5 );
	
	controlsSizer->Add( rightSizer, 1, wxEXPAND, 5 );
	
	mainPanel->SetSizer( controlsSizer );
	mainPanel->Layout();
	controlsSizer->Fit( mainPanel );
	mainSizer->Add( mainPanel, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );
}

void OptionsWizardPageFinish::initWindow()
{
	m_finishLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_finish.label_finish"))));
	m_descriptionLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_finish.label_description"))));
	m_viewHome->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_finish.label_view_home"))));
	m_proceedLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_finish.label_proceed"))));
	m_leftIcon->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris.png"))), wxBITMAP_TYPE_PNG));
}

void OptionsWizardPageFinish::loadOptions()
{

}

void OptionsWizardPageFinish::saveOptions()
{

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
