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
#include "multilanguagewizard.h"

#include "conversions.h"
#include "uiapi.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MultilanguageWizard, WizardBase)
	EVT_WIZARD_PAGE_CHANGED(static_cast<wxWindowID>(wxID_ANY), MultilanguageWizard::OnWizardPageChanged)    
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

MultilanguageWizard::MultilanguageWizard(wxWindow *parent, const String &title) : WizardBase(parent, static_cast<wxWindowID>(wxID_ANY), conversions::from_utf16<wxString>(title))
{
	centerWindow(this);
}

MultilanguageWizard::~MultilanguageWizard()
{

}

void MultilanguageWizard::updateControls()
{
	wxButton *buttonBackward = dynamic_cast<wxButton *>(FindWindow(wxID_BACKWARD));
	if(buttonBackward != nullptr)
		buttonBackward->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard.buttons.back"))));

	wxButton *buttonForward = dynamic_cast<wxButton *>(FindWindow(wxID_FORWARD));
	if(buttonForward != nullptr)
	{
		if(HasNextPage(GetCurrentPage()))
			buttonForward->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard.buttons.next"))));
		else
			buttonForward->SetLabel(conversions::from_utf16<wxString>(getText(_S("ui.wizard.buttons.finish"))));
	}

	wxButton *buttonCancel = dynamic_cast<wxButton *>(FindWindow(wxID_CANCEL));
	if(buttonCancel != nullptr)
		buttonCancel->SetLabel(conversions::from_utf16<wxString>(getText(_S("common.cancel"))));	
}

void MultilanguageWizard::OnWizardPageChanged(wxWizardEvent &e)
{
	e.Skip();

	updateControls();	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
