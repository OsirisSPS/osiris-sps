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
#include "extensionswizardpagetemplate.h"

#include "application.h"
#include "conversions.h"
#include "filesystem.h"
#include "log.h"
#include "utils.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(ExtensionsWizardPageTemplate, WizardPageBase)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

ExtensionsWizardPageTemplate::TemplateDetails::TemplateDetails(const String &folder) : m_folder(folder)
{

}

ExtensionsWizardPageTemplate::TemplateDetails::~TemplateDetails()
{

}

bool ExtensionsWizardPageTemplate::TemplateDetails::addFile(const String &target, const String &source, bool script, bool macro)
{
	if(target.empty() || source.empty())
		return false;

	FileDetails file;
	file.target = target;
	file.source = source;
	file.script = script;
	file.macro = macro;
	m_files.push_back(file);

	return true;
}

//////////////////////////////////////////////////////////////////////

ExtensionsWizardPageTemplate::ExtensionsWizardPageTemplate(wxWizard *parent) : WizardPageBase(parent)
{
	createLayout();
	initWindow();
}

ExtensionsWizardPageTemplate::~ExtensionsWizardPageTemplate()
{
	cleanupWindow();
}

shared_ptr<ExtensionsWizardPageTemplate::TemplateDetails> ExtensionsWizardPageTemplate::getTemplate() const
{
	TemplatesMap::const_iterator i = m_templatesMap.find(m_templateType->GetSelection());
	if(i != m_templatesMap.end())
		return i->second;

	return null;
}

void ExtensionsWizardPageTemplate::createLayout()
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

	m_templateBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, static_cast<wxWindowID>(wxID_ANY), wxT("Template") ), wxVERTICAL );

	wxFlexGridSizer* templateFlexGridSizer;
	templateFlexGridSizer = new wxFlexGridSizer( 3, 2, 0, 0 );
	templateFlexGridSizer->AddGrowableCol( 1 );
	templateFlexGridSizer->SetFlexibleDirection( wxBOTH );
	templateFlexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_templateTypeLabel = new wxStaticText( this, static_cast<wxWindowID>(wxID_ANY), wxT("Template"), wxDefaultPosition, wxDefaultSize, 0 );
	m_templateTypeLabel->Wrap( -1 );
	templateFlexGridSizer->Add( m_templateTypeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_templateTypeChoices;
	m_templateType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_templateTypeChoices, 0 );
	m_templateType->SetSelection( 0 );
	templateFlexGridSizer->Add( m_templateType, 0, wxALL|wxEXPAND, 5 );

	m_templateAuthorLabel = new wxStaticText( this, wxID_ANY, wxT("Author"), wxDefaultPosition, wxDefaultSize, 0 );
	m_templateAuthorLabel->Wrap( -1 );
	templateFlexGridSizer->Add( m_templateAuthorLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_templateAuthor = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_templateAuthor->Wrap( -1 );
	templateFlexGridSizer->Add( m_templateAuthor, 0, wxALL|wxEXPAND, 5 );

	m_templateDescriptionLabel = new wxStaticText( this, wxID_ANY, wxT("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_templateDescriptionLabel->Wrap( -1 );
	templateFlexGridSizer->Add( m_templateDescriptionLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_templateDescription = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_templateDescription->Wrap( -1 );
	templateFlexGridSizer->Add( m_templateDescription, 0, wxALL|wxEXPAND, 5 );

	m_templateBoxSizer->Add( templateFlexGridSizer, 0, wxEXPAND, 5 );

	mainSizer->Add( m_templateBoxSizer, 1, wxEXPAND, 5 );

	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );
}

void ExtensionsWizardPageTemplate::initWindow()
{
	m_pageCaptionTitle->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_extensions.page_template.label_caption_title"))));
	m_pageCaptionDescription->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_extensions.page_template.label_caption_description"))));
	m_pageIcon->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("extensions_wizard_template.png"))), wxBITMAP_TYPE_PNG));
	m_templateTypeLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_extensions.page_template.label_template_type"))));
	m_templateAuthorLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_extensions.page_template.label_template_author"))));
	m_templateDescriptionLabel->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard_extensions.page_template.label_template_description"))));

	m_templateType->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ExtensionsWizardPageTemplate::onTemplateSelChanged), NULL, this);

	loadTemplates();
}

void ExtensionsWizardPageTemplate::cleanupWindow()
{
	m_templateType->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ExtensionsWizardPageTemplate::onTemplateSelChanged), NULL, this);
}

void ExtensionsWizardPageTemplate::loadTemplates()
{
	OS_ASSERT(m_templatesMap.empty());

	m_templateType->Select(addTemplate("None", null));

	String templatesFolder = utils::makeFolderPath(Application::instance()->getTemplatesPath(), "extensions");

	StringList folders;
	if(FileSystem::instance()->getDirectories(templatesFolder, folders, false))
	{
		folders.sort();
		for(StringList::const_iterator i = folders.begin(); i != folders.end(); ++i)
		{
			String folder = utils::makeFolderPath(templatesFolder, *i);
			String filename = utils::makeFilePath(folder, *i + ".xml");
			if(FileSystem::instance()->fileExists(filename))
			{
				shared_ptr<TemplateDetails> details = loadTemplate(folder, filename);
				if(details != null)
				{
					int32 index = m_templateType->Append(conversions::from_utf16<wxString>(details->getName()));
					if(index != -1)
						m_templatesMap[index] = details;
				}
				else
				{
					OS_LOG_ERROR("Invalid extensions template file '" + filename + "'");
				}		
			}
		}
	}

	updateCurrentTemplate();
}

shared_ptr<ExtensionsWizardPageTemplate::TemplateDetails> ExtensionsWizardPageTemplate::loadTemplate(const String &folder, const String &filename)
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	if(document->parseFile(filename) == false)
		return null;

	shared_ptr<XMLNode> root = document->getRoot();
	if(root == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	String name = root->getAttributeString("name");
	if(name.empty())
	{
		OS_ASSERTFALSE();
		return null;
	}

	shared_ptr<TemplateDetails> details(OS_NEW TemplateDetails(folder));

	details->setName(name);
	details->setDescription(root->getAttributeString("description"));
	details->setAuthor(root->getAttributeString("author"));
	
	StringList files;
	
	shared_ptr<XMLNode> nodeFiles = root->getNode("files");
	if(nodeFiles == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	shared_ptr<XMLNodes> nodeFilesNodes = nodeFiles->getNodes();
	if(nodeFiles == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	for(XMLNodes::const_iterator i = nodeFilesNodes->begin(); i != nodeFilesNodes->end(); ++i)
	{
		shared_ptr<XMLNode> nodeFile = *i;
		if(nodeFile->getName() != "file")
			continue;

		if(details->addFile(nodeFile->getAttributeString("target"), nodeFile->getAttributeString("source"), nodeFile->getAttributeBool("script"), nodeFile->getAttributeBool("macro")) == false)
		{
			OS_ASSERTFALSE();
			return null;
		}
	}	

	return details;
}

int32 ExtensionsWizardPageTemplate::addTemplate(const String &name, shared_ptr<TemplateDetails> details)
{
	OS_ASSERT(name.empty() == false);

	int32 index = m_templateType->Append(conversions::from_utf16<wxString>(name));
	if(index != -1)
		m_templatesMap[index] = details;

	return index;
}

void ExtensionsWizardPageTemplate::updateCurrentTemplate()
{
	shared_ptr<TemplateDetails> details;

	int32 index = m_templateType->GetSelection();
	if(index != -1)
	{
		OS_ASSERT(m_templatesMap.find(index) != m_templatesMap.end());
		details = m_templatesMap[index];
	}

	bool enable = details != null;
	
	m_templateAuthorLabel->Enable(enable);
	m_templateAuthor->Enable(enable);
	m_templateAuthor->SetLabel(conversions::from_utf16<wxString>(details != null ? details->getAuthor() : _S("-")));

	m_templateDescriptionLabel->Enable(enable);
	m_templateDescription->Enable(enable);
	m_templateDescription->SetLabel(conversions::from_utf16<wxString>(details != null ? details->getDescription() : _S("-")));
}

void ExtensionsWizardPageTemplate::onTemplateSelChanged(wxCommandEvent &e)
{
	updateCurrentTemplate();	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
