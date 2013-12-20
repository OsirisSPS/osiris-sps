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
#include "statisticswindow.h"

#include "application.h"
#include "boost/thread/recursive_mutex.hpp"
#include "chartpanel.h"
#include "conversions.h"
#include "languageculture.h"
#include "languagemanager.h"
#include "lock.h"
#include "options.h"
#include "platformmanager.h"
#include "realtimestats.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static const uint32 cmdClear = wxNewId();
static const uint32 cmdPause = wxNewId();
static const uint32 cmdResolutions = wxNewId();
static const uint32 cmdPresets = wxNewId();
static const uint32 cmdChangeResolution = 15000;	// TODO: andrebbero gestiti tramite wxRegisterId
static const uint32 cmdChangePreset = 15021;

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(StatisticsWindow, WindowBase)
	EVT_BUTTON(cmdClear, StatisticsWindow::onClear)
	EVT_BUTTON(cmdPause, StatisticsWindow::onPause)
	EVT_BUTTON(cmdResolutions, StatisticsWindow::onResolutions)
	EVT_BUTTON(cmdPresets, StatisticsWindow::onPresets)
	EVT_MENU_RANGE(cmdChangeResolution, cmdChangeResolution + 20, StatisticsWindow::onChangeResolution)
	EVT_MENU_RANGE(cmdChangePreset, cmdChangePreset + 20, StatisticsWindow::onChangePreset)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

void StatisticsWindow::StatisticsWindowTimer::SetWindow(StatisticsWindow* pWindow, bool forUpdate)
{
	m_pWindow = pWindow;
	m_forUpdate = forUpdate;
}

void StatisticsWindow::StatisticsWindowTimer::Notify()
{
	OS_ASSERT(m_pWindow != NULL);

	if(m_forUpdate)
		m_pWindow->getChartPanel()->updateStats();
	else
		m_pWindow->Refresh();
}

//////////////////////////////////////////////////////////////////////

StatisticsWindow::StatisticsWindow(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style) : WindowBase(parent, id, pos, size, style)
{
	createLayout();
	initWindow();
}

StatisticsWindow::~StatisticsWindow()
{

}

void StatisticsWindow::createLayout()
{
	wxBoxSizer* contentSizer;
	contentSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pnlChart = new ChartPanel( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	contentSizer->Add( m_pnlChart, 1, wxEXPAND | wxALL, 0 );

	m_commandsPanel = new SkinnedWindow<wxPanel>( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxSize( 25,-1 ), wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
	wxBoxSizer* commandsSizer;
	commandsSizer = new wxBoxSizer( wxVERTICAL );

	m_buttonClear = new wxSkinButton( m_commandsPanel, cmdClear, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsSizer->Add( m_buttonClear, 0, wxALL, 2 );

	m_buttonPause = new wxSkinButton( m_commandsPanel, cmdPause, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsSizer->Add( m_buttonPause, 0, wxALL, 2 );

	m_buttonResolutions = new wxSkinButton( m_commandsPanel, cmdResolutions, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsSizer->Add( m_buttonResolutions, 0, wxALL, 2 );

	m_buttonPresets = new wxSkinButton( m_commandsPanel, cmdPresets, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsSizer->Add( m_buttonPresets, 0, wxALL, 2 );

	m_commandsPanel->SetSizer( commandsSizer );
	m_commandsPanel->Layout();
	contentSizer->Add( m_commandsPanel, 0, wxEXPAND, 0 );

	this->SetSizer( contentSizer );
	this->Layout();
}

void StatisticsWindow::initWindow()
{
	OS_ASSERT(m_commandsPanel != nullptr);
	m_commandsPanel->getRenderer()->setBackgroundImage(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("window_commands_background.png"))), wxBITMAP_TYPE_PNG));

	m_buttonClear->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_clear_normal.png"))), wxBITMAP_TYPE_PNG));
	m_buttonClear->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_clear_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonClear->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_clear_over.png"))), wxBITMAP_TYPE_PNG));
	m_buttonClear->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.statistics.clear.tooltip"))));

	m_buttonPause->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_pause_normal.png"))), wxBITMAP_TYPE_PNG));
	m_buttonPause->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_pause_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonPause->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_pause_over.png"))), wxBITMAP_TYPE_PNG));
	m_buttonPause->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.statistics.pause.tooltip"))));

	m_buttonResolutions->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_resolution_normal.png"))), wxBITMAP_TYPE_PNG));
	m_buttonResolutions->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_resolution_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonResolutions->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_resolution_over.png"))), wxBITMAP_TYPE_PNG));
	m_buttonResolutions->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.statistics.resolutions.tooltip"))));

	m_buttonPresets->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_settings_normal.png"))), wxBITMAP_TYPE_PNG));
	m_buttonPresets->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_settings_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonPresets->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("stats_btn_settings_over.png"))), wxBITMAP_TYPE_PNG));
	m_buttonPresets->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.statistics.presets.tooltip"))));

	// Init presets
	RealtimeStatsManager::addPreset(_S("None"), 0, 0);
	RealtimeStatsManager::addPreset(_S("Stability status"), 0, 7);
	RealtimeStatsManager::addPreset(_S("Activity"), 0, 100);
	RealtimeStatsManager::addPreset(_S("Objects"), 0, 0);
	RealtimeStatsManager::addPreset(_S("Isis"), 0, 0);

	if(OS_NAMESPACE_NAME::Options::instance()->getOption<bool>(_S("developer.enabled")))
	{
		RealtimeStatsManager::addPreset(_S("Debug"), 0, 0);
		RealtimeStatsManager::addPreset(_S("All"), 0, 0);
	}

	RealtimeStatsManager::usePreset(_S("None"));

	m_updateTimer.SetWindow(this,true);
	m_refreshTimer.SetWindow(this,false);

	updateTimers();

	m_pnlChart->SetFocus();
}

void StatisticsWindow::updateTimers()
{
	m_updateTimer.Start((int)RealtimeStatsManager::m_resolution, false);
	m_refreshTimer.Start(200, false);
}

void StatisticsWindow::usePreset(const String &name)
{
	RealtimeStatsManager::usePreset(name);
}

void StatisticsWindow::setResolution(double value)
{
	RealtimeStatsManager::m_resolution = value;
	RealtimeStatsManager::reset();
	Refresh();

	updateTimers();
}

String StatisticsWindow::convertResolutionIndexToText(uint32 i)
{
	uint32 time = convertResolutionIndexToTime(i);
	return LanguageManager::instance()->getCulture(Options::instance()->getDefaultLanguage())->formatTime(time);
}

uint32 StatisticsWindow::convertResolutionIndexToTime(uint32 i)
{
	uint32 output = 1000;
	switch(i)
	{
	case 0: output = 1; break;
	case 1: output = 5; break;
	case 2: output = 10; break;
	case 3: output = 30; break;
	case 4: output = 60; break;
	case 5: output = 60*5; break;
	case 6: output = 60*10; break;
	case 7: output = 60*20; break;
	case 8: output = 60*30; break;
	case 9: output = 60*60; break;
	case 10: output = 60*60*5; break;
	case 11: output = 60*60*10; break;
	case 12: output = 60*60*24; break;
	}
	return 1000 * output;
}

void StatisticsWindow::onClear(wxCommandEvent &e)
{
	RealtimeStatsManager::reset();
}

void StatisticsWindow::onPause(wxCommandEvent &e)
{
	RealtimeStatsManager::setPause(!RealtimeStatsManager::getPause());
}

void StatisticsWindow::onResolutions(wxCommandEvent &e)
{
	scoped_ptr<wxMenu> menu(new wxMenu());

	for(int i=0;i<=12;i++)
	{
		menu->Append(new wxMenuItem(menu.get(), cmdChangeResolution + i, conversions::from_utf16<wxString>(convertResolutionIndexToText(i))));
	}

	m_buttonResolutions->PopupMenu(menu.get(), 0, m_buttonResolutions->GetSize().y);
}

void StatisticsWindow::onPresets(wxCommandEvent &e)
{
	scoped_ptr<wxMenu> menu(new wxMenu());

	const RealtimeStatsManager::PresetsVector &presets = RealtimeStatsManager::getPresets();
	for(size_t i = 0; i < presets.size(); i++)
	{
		shared_ptr<RealtimeStatsPreset> preset = presets[i];
		menu->Append(new wxMenuItem(menu.get(), static_cast<int>(cmdChangePreset + i), conversions::from_utf16<wxString>(preset->getName())));
	}

	m_buttonPresets->PopupMenu(menu.get(), 0, m_buttonPresets->GetSize().y);
}

void StatisticsWindow::onChangeResolution(wxCommandEvent &e)
{
    uint32 i = e.GetId() - cmdChangeResolution;

	double newRes = convertResolutionIndexToTime(i);
	setResolution(newRes);	
}

void StatisticsWindow::onChangePreset(wxCommandEvent &e)
{
	const RealtimeStatsManager::PresetsVector &presets = RealtimeStatsManager::getPresets();

	uint32 i = e.GetId() - cmdChangePreset;
	RealtimeStatsManager::usePreset(presets[i]->getName());
	Refresh();
	

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
