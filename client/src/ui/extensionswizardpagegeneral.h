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

#ifndef _OS_UI_EXTENSIONSSWIZARDPAGEGENERAL_H
#define _OS_UI_EXTENSIONSSWIZARDPAGEGENERAL_H

#include "multilanguagewindow.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ExtensionsWizardPageGeneral : public wxWizardPageSimple,
									public MultilanguageWindow
{
	typedef wxWizardPageSimple WizardPageBase;

	DECLARE_EVENT_TABLE()

// Construction
public:
	ExtensionsWizardPageGeneral(wxWizard *parent);
	virtual ~ExtensionsWizardPageGeneral();

// Attributes
public:
	inline wxString getExtensionName() const;
	inline wxString getExtensionDescription() const;
	inline wxString getExtensionAuthor() const;

// Operations
private:
	void createLayout();
	void initWindow();
	void cleanupWindow();
	void updateControls();

// Events
private:
	void onExtensionNameChanged(wxCommandEvent &e);

private:
	wxStaticBitmap *m_pageIcon;
	wxStaticText *m_pageCaptionTitle;
	wxStaticText *m_pageCaptionDescription;
	wxStaticBoxSizer *m_settingsBoxSizer;
	wxStaticText *m_nameLabel;
	wxTextCtrl *m_name;
	wxStaticText *m_descriptionLabel;
	wxTextCtrl *m_description;
	wxStaticText *m_authorLabel;
	wxTextCtrl *m_author;
};

//////////////////////////////////////////////////////////////////////

inline wxString ExtensionsWizardPageGeneral::getExtensionName() const { return m_name->GetValue(); }
inline wxString ExtensionsWizardPageGeneral::getExtensionDescription() const { return m_description->GetValue(); }
inline wxString ExtensionsWizardPageGeneral::getExtensionAuthor() const { return m_author->GetValue(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_EXTENSIONSSWIZARDPAGEGENERAL_H
