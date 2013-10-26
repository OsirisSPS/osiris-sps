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
#include "optionswizardpagegeneral.h"

#include "application.h"
#include "conversions.h"
#include "languageculture.h"
#include "languagemanager.h"
#include "log.h"
#include "logmessage.h"
#include "options.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static const uint32 checkAutoStartEnabled = wxNewId();

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(OptionsWizardPageGeneral, WizardPageBase)
	EVT_CHECKBOX(checkAutoStartEnabled, OptionsWizardPageGeneral::onAutoStartCheckChanged)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

OptionsWizardPageGeneral::OptionsWizardPageGeneral(wxWizard *parent) : WizardPageBase(parent)
{
	createLayout();
	initWindow();
}

OptionsWizardPageGeneral::~OptionsWizardPageGeneral()
{

}

void OptionsWizardPageGeneral::createLayout()
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
	
	m_languageBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("Language options") ), wxHORIZONTAL );
	
	m_languageLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("Primary language"), wxDefaultPosition, wxDefaultSize, 0 );
	m_languageLabel->Wrap( -1 );
	m_languageBoxSizer->Add( m_languageLabel, 0, wxALIGN_CENTER|wxALL|wxTOP, 5 );
	
	wxArrayString m_languageComboBoxChoices;
	m_languageComboBox = new wxChoice( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, m_languageComboBoxChoices, 0 );
	m_languageComboBox->SetSelection( 0 );
	m_languageBoxSizer->Add( m_languageComboBox, 0, wxALL|wxTOP, 5 );
	
	mainSizer->Add( m_languageBoxSizer, 0, wxALL|wxEXPAND, 2 );
	
	m_startSystem = new wxCheckBox( this, checkAutoStartEnabled, wxT("Enable autostartup"), wxDefaultPosition, wxDefaultSize, 0 );
	
	mainSizer->Add( m_startSystem, 0, wxALL|wxTOP, 5 );
	
	m_startMinimized = new wxCheckBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("Autostartup in backgroud"), wxDefaultPosition, wxDefaultSize, 0 );
	
	mainSizer->Add( m_startMinimized, 0, wxALL, 5 );

	m_associationsProtocols = new wxCheckBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("Protocol association"), wxDefaultPosition, wxDefaultSize, 0 );
	
	mainSizer->Add( m_associationsProtocols, 0, wxALL, 5 );

	m_associationsFiles = new wxCheckBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("Files association"), wxDefaultPosition, wxDefaultSize, 0 );
	
	mainSizer->Add( m_associationsFiles, 0, wxALL, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );
}

void OptionsWizardPageGeneral::initWindow()
{
	m_pageCaptionTitle->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_general.label_caption_title"))));
	m_pageCaptionDescription->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_general.label_caption_description"))));
	m_languageBoxSizer->GetStaticBox()->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_general.label_language_box"))));
	m_languageLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_general.label_language"))));
	m_startSystem->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_general.label_start_system"))));
	m_startMinimized->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_general.label_start_minimized"))));
	m_associationsProtocols->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_general.label_associations_protocols"))));
	m_associationsFiles->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_general.label_associations_files"))));

	m_pageIcon->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("options_wizard_general.png"))), wxBITMAP_TYPE_PNG));

#if OS_PLATFORM == OS_PLATFORM_WIN
	if(PlatformManager::instance()->isPrivilegedUser() == false)
	{
		m_associationsProtocols->Enable(false);
		m_associationsProtocols->SetLabel(m_associationsProtocols->GetLabel() + conversions::from_utf16<wxString>(_S(" - ")) + conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_general.label_require_system_privileges"))));

		m_associationsFiles->Enable(false);
		m_associationsFiles->SetLabel(m_associationsFiles->GetLabel() + conversions::from_utf16<wxString>(_S(" - ")) + conversions::from_utf16<wxString>(getText(_S("ui.wizard_options.page_general.label_require_system_privileges"))));
	}
#endif

#if OS_PLATFORM == OS_PLATFORM_LINUX
	m_startSystem->Hide();
	m_associationsProtocols->Hide();
	m_associationsFiles->Hide();
#endif

#if OS_PLATFORM == OS_PLATFORM_MAC
	m_startSystem->Hide();
	m_associationsProtocols->Hide();
	m_associationsFiles->Hide();
#endif

}

void OptionsWizardPageGeneral::loadCulture(shared_ptr<LanguageCulture> culture)
{
	if(culture == null)
		return;

    if(culture->getTranslationDerivedAvaible() && culture->isNeutralCulture() == false)
	{
		int32 index = m_languageComboBox->Insert(conversions::from_utf16<wxString>(culture->getEnglishName()), m_languageComboBox->GetCount(), new wxStringClientData(conversions::from_utf16<wxString>(culture->getID())));
		if(index != -1)
		{
			if(culture->getID() == Options::instance()->getDefaultLanguage())
			{
				m_languageComboBox->Select(index);
			}
		}
	}

	const LanguageCulture::Cultures &cultures = culture->getCultures();
	for(LanguageCulture::Cultures::const_iterator i = cultures.begin(); i != cultures.end(); ++i)
	{
		loadCulture(i->second);
	}
}

void OptionsWizardPageGeneral::updateControls()
{
	//m_autoStartBackground->Enable(m_autoStartEnabled->GetValue());
}

void OptionsWizardPageGeneral::loadOptions()
{
	loadCulture(LanguageManager::instance()->getInvariant());

	m_startSystem->SetValue(Application::instance()->getAutoStart());
	m_startMinimized->SetValue(Options::instance()->getOption<bool>(Options::ui_options::start_minimized));
	m_associationsProtocols->SetValue(Application::instance()->getAssociationsProtocols());
	m_associationsFiles->SetValue(Application::instance()->getAssociationsFiles());

	if(Options::instance()->isFirstRun())
		if( (m_startSystem->IsShown()) && (m_startSystem->IsEnabled()) )
			m_startSystem->SetValue(true);

	updateControls();
}

void OptionsWizardPageGeneral::saveOptions()
{
	int32 item = m_languageComboBox->GetSelection();
	if(item != -1)
	{
		wxStringClientData *itemData = dynamic_cast<wxStringClientData *>(m_languageComboBox->GetClientObject(item));
		if(itemData != null)
		{
			Options::instance()->setOption(Options::language_options::id, conversions::to_utf16(itemData->GetData()));
		}
	}

	if( (m_startSystem->IsShown()) && (m_startSystem->IsEnabled()) )
	{
		Application::instance()->setupAutostart(m_startSystem->GetValue());
	}

	Options::instance()->setOption(Options::ui_options::start_minimized, m_startMinimized->GetValue());

	if( (m_associationsProtocols->IsShown()) && (m_associationsProtocols->IsEnabled()) )
	{
		Application::instance()->setupProtocolAssociations(m_associationsProtocols->GetValue(), true);
	}

	if( (m_associationsFiles->IsShown()) && (m_associationsFiles->IsEnabled()) )
	{
		Application::instance()->setupFilesAssociations(m_associationsFiles->GetValue(), true);
	}
	
}

void OptionsWizardPageGeneral::onAutoStartCheckChanged(wxCommandEvent &e)
{
	updateControls();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
