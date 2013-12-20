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

#ifndef _OS_UI_EXTENSIONSWIZARD_H
#define _OS_UI_EXTENSIONSWIZARD_H

#include "multilanguagewizard.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ExtensionsWizardPageFinish;
class ExtensionsWizardPageGeneral;
class ExtensionsWizardPageTemplate;

//////////////////////////////////////////////////////////////////////

class ExtensionsWizard : public MultilanguageWizard
{
	typedef MultilanguageWizard WizardBase;

	DECLARE_EVENT_TABLE()

// Construction
public:
	ExtensionsWizard(wxWindow *parent = nullptr, const String &title = String::EMPTY);
	virtual ~ExtensionsWizard();

// Operations
public:
	void run();

private:
	void updateControls();
	
// Events
private:
	void OnWizardPageChanged(wxWizardEvent &e);
	void OnWizardFinished(wxWizardEvent &e);

private:
	ExtensionsWizardPageGeneral *m_pageGeneral;
	ExtensionsWizardPageTemplate *m_pageTemplate;
	ExtensionsWizardPageFinish *m_pageFinish;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_EXTENSIONSWIZARD_H
