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

#ifndef _OS_UI_OPTIONSWIZARDPAGEFINISH_H
#define _OS_UI_OPTIONSWIZARDPAGEFINISH_H

#include "ioptionswizardpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class OptionsWizardPageFinish : public IOptionsWizardPage
{
	typedef IOptionsWizardPage WizardPageBase;

	DECLARE_EVENT_TABLE()

// Construction
public:
	OptionsWizardPageFinish(wxWizard *parent);
	virtual ~OptionsWizardPageFinish();

// Attributes
public:
	inline bool getViewHome() const;

// Operations
private:
	void createLayout();
	void initWindow();

// IOptionsWizardPage interface
public:
	virtual void loadOptions();
	virtual void saveOptions();

private:
	wxStaticText *m_finishLabel;
	wxStaticText *m_descriptionLabel;
	wxStaticText *m_proceedLabel;
	wxCheckBox *m_viewHome;
	wxStaticBitmap *m_leftIcon;
};

//////////////////////////////////////////////////////////////////////

inline bool OptionsWizardPageFinish::getViewHome() const { return m_viewHome->GetValue(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_OPTIONSWIZARDPAGEFINISH_H
