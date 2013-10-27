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

#ifndef _OS_UI_STATISTICSWINDOW_H
#define _OS_UI_STATISTICSWINDOW_H

#include "multilanguagewindow.h"
#include "skinnedwindow.h"
#include "wxskinbutton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ChartPanel;

//////////////////////////////////////////////////////////////////////

class StatisticsWindow : public wxPanel,
                         public MultilanguageWindow
{
	DECLARE_EVENT_TABLE()

	typedef wxPanel WindowBase;

// Internal
private:
	class StatisticsWindowTimer : public wxTimer
	{
	public:
		void SetWindow(StatisticsWindow *pWindow, bool forUpdate);
		virtual void Notify();

	protected:
		StatisticsWindow* m_pWindow;
		bool m_forUpdate;
	};

// Construction
public:
	StatisticsWindow(wxWindow *parent, wxWindowID id = static_cast<wxWindowID>(wxID_ANY), const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0);
	virtual ~StatisticsWindow();

// Attributes
	inline ChartPanel* getChartPanel();

// Operations
private:
	void createLayout();
	void initWindow();
	void updateTimers();
	void usePreset(const String &name);

// Events
private:
	void onClear(wxCommandEvent &e);
	void onPause(wxCommandEvent &e);
	void onResolutions(wxCommandEvent &e);
	void onPresets(wxCommandEvent &e);	
	void onChangeResolution(wxCommandEvent &e);
	void onChangePreset(wxCommandEvent &e);

// Private:
private:
	void setResolution(double value);
	String convertResolutionIndexToText(uint32 i);
	uint32 convertResolutionIndexToTime(uint32 i);

	StatisticsWindowTimer m_updateTimer;
	StatisticsWindowTimer m_refreshTimer;

	SkinnedWindow<wxPanel> *m_commandsPanel;
	wxSkinButton *m_buttonClear;
	wxSkinButton *m_buttonPause;
	wxSkinButton *m_buttonResolutions;
	wxSkinButton *m_buttonPresets;
	ChartPanel* m_pnlChart;
};

//////////////////////////////////////////////////////////////////////

inline ChartPanel* StatisticsWindow::getChartPanel() { return m_pnlChart; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_STATISTICSWINDOW_H
