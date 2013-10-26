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

#ifndef _OS_UI_OPTIONSWIZARDPAGENETWORK_H
#define _OS_UI_OPTIONSWIZARDPAGENETWORK_H

#include "ioptionswizardpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class OptionsWizardPageNetwork : public IOptionsWizardPage
{
	typedef IOptionsWizardPage WizardPageBase;

	DECLARE_EVENT_TABLE()

// Construction
public:
	OptionsWizardPageNetwork(wxWizard *parent);
	virtual ~OptionsWizardPageNetwork();

// Operations
private:
	void createLayout();
	void initWindow();
	void cleanupWindow();
	//void testPort(uint32 port);
	void testPort2(uint32 port, wxButton* button);
	void updateControls();
	
// IOptionsWizardPage interface
public:
	virtual void loadOptions();
	virtual void saveOptions();

// Events
private:
	void onMaxOutgoingConnectionsChanged(wxCommandEvent &e);
	void onTestP2PPort(wxCommandEvent &e);
	void onTestWebPort(wxCommandEvent &e);		

private:
	wxStaticText *m_pageCaptionTitle;
	wxStaticText *m_pageCaptionDescription;
	wxStaticBoxSizer *m_networkBoxSizer;
	wxStaticText *m_webServerPortLabel;
	wxStaticText *m_p2pServerPortLabel;
	wxTextCtrl *m_p2pServerPort;
	wxCheckBox *m_p2pServerEnableUpnp;
	wxTextCtrl *m_webServerPort;
	wxCheckBox *m_webServerEnableUpnp;
	wxTextCtrl *m_maxIncomingConnections;
	wxTextCtrl *m_maxOutgoingConnections;
	wxButton *m_testP2PPort;
	wxButton *m_testWebPort;	
	wxStaticText *m_upnpServiceLabel;
	wxStaticText *m_upnpServiceStatus;
	wxStaticBitmap *m_upnpServiceIcon;
	wxStaticBoxSizer *m_connectionsBoxSizer;
	wxStaticText *m_maxIncomingConnectionsLabel;
	wxStaticText *m_maxOutgoingConnectionsLabel;
	wxStaticBitmap *m_pageIcon;	
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_OPTIONSWIZARDPAGENETWORK_H
