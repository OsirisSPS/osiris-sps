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

#ifndef _OS_UI_OPTIONSWIZARDPAGEGENERAL_H
#define _OS_UI_OPTIONSWIZARDPAGEGENERAL_H

#include "ioptionswizardpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class LanguageCulture;

//////////////////////////////////////////////////////////////////////

class OptionsWizardPageGeneral : public IOptionsWizardPage
{
	typedef IOptionsWizardPage WizardPageBase;

	DECLARE_EVENT_TABLE()

// Construction
public:
	OptionsWizardPageGeneral(wxWizard *parent);
	virtual ~OptionsWizardPageGeneral();

// Operations
private:
	void createLayout();
	void initWindow();

	void loadCulture(shared_ptr<LanguageCulture> culture);
	void updateControls();

// IOptionsWizardPage interface
public:
	virtual void loadOptions();
	virtual void saveOptions();

// Events
private:
	void onAutoStartCheckChanged(wxCommandEvent &e);

private:
	wxStaticText *m_pageCaptionTitle;
	wxStaticText *m_pageCaptionDescription;
	wxStaticBoxSizer *m_languageBoxSizer;
	wxStaticText *m_languageLabel;
	wxChoice *m_languageComboBox;
	wxCheckBox *m_startSystem;
	wxCheckBox *m_startMinimized;
	wxCheckBox *m_associationsProtocols;
	wxCheckBox *m_associationsFiles;
	wxCheckBox *m_trayAlways;
	wxCheckBox *m_trayBalloon;
	wxCheckBox *m_trayMinimize;
	wxCheckBox *m_exitConfirm;
	wxCheckBox *m_checkUpdate;
	wxStaticBitmap *m_pageIcon;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_OPTIONSWIZARDPAGEGENERAL_H
