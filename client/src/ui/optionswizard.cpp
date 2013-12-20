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
#include "optionswizard.h"

#include "application.h"
#include "options.h"
#include "optionswizardpagefinish.h"
#include "optionswizardpagegeneral.h"
#include "optionswizardpagenetwork.h"
#include "optionswizardpageproxy.h"
#include "optionswizardpagewelcome.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(OptionsWizard, WizardBase)
	EVT_WIZARD_FINISHED(static_cast<wxWindowID>(wxID_ANY), OptionsWizard::OnWizardFinished)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

OptionsWizard::OptionsWizard(wxWindow *parent, const String &title) : WizardBase(parent, title)
{
	m_pageWelcome = new OptionsWizardPageWelcome(this);
	m_pageGeneral = new OptionsWizardPageGeneral(this);
	m_pageProxy = new OptionsWizardPageProxy(this);
	m_pageFinish = new OptionsWizardPageFinish(this);
	m_pageNetwork = new OptionsWizardPageNetwork(this);	

	wxWizardPageSimple::Chain(m_pageWelcome, m_pageGeneral);
	wxWizardPageSimple::Chain(m_pageGeneral, m_pageNetwork);
	wxWizardPageSimple::Chain(m_pageNetwork, m_pageProxy);
	wxWizardPageSimple::Chain(m_pageProxy, m_pageFinish);

	SetBorder(0);
}

OptionsWizard::~OptionsWizard()
{

}

void OptionsWizard::run()
{
	updatePages(true);

	wxSizer *sizer = GetSizer();
	if(sizer != nullptr)
	{
		// Il bordo della colonna principale nel wizard viene settato a 5px e, non essendo al momento
		// regolabile, viene qui modificato in questo modo...

		wxSizerItemList &sizerItems = sizer->GetChildren();
		if(sizerItems.empty() == false)
		{
			sizerItems[0]->SetBorder(0);
		}
	}

	// Scorre tutte le pagine a partire da quella di welcome e adatta il wizard alla dimensione migliore
	FitToPage(m_pageWelcome);

	Layout();

	// Avvia il wizard mostrando la pagina di welcome
	RunWizard(m_pageWelcome);
}

void OptionsWizard::updatePages(bool load)
{
    wxBusyCursor busyCursor;

	IOptionsWizardPage *currentPage = m_pageWelcome;
	while(currentPage != nullptr)
	{
		if(load)
			currentPage->loadOptions();
		else
			currentPage->saveOptions();

		currentPage = dynamic_cast<IOptionsWizardPage *>(currentPage->GetNext());
	}
}

void OptionsWizard::OnWizardFinished(wxWizardEvent &e)
{
	updatePages(false);

	wxBusyCursor busyCursor;

	Options::instance()->save();
	Options::instance()->update();

	if(m_pageFinish->getViewHome())
		Application::instance()->viewHome();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
