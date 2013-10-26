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

#ifndef _OS_UI_CHARTPANEL_H
#define _OS_UI_CHARTPANEL_H

#include "multilanguagewindow.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ChartPanel : public wxPanel, 
				   public MultilanguageWindow
{
	DECLARE_EVENT_TABLE()

    typedef wxPanel PanelBase;

// Construction
public:
	ChartPanel(wxWindow *parent, wxWindowID winid = static_cast<wxWindowID>(wxID_ANY), const wxPoint& pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString &name = wxPanelNameStr);
	virtual ~ChartPanel();

// Operations
	void updateStats();

protected:
	void onEraseBackground(wxEraseEvent &e);
	void onChar(wxKeyEvent &e);
    void onPaint(wxPaintEvent &e);
    void onSize(wxSizeEvent &e);

private:
    void drawChart(wxDC &dc, wxRect rect);
	String formatTimeSpan(double time);
	wxColour getChartColor(uint32 chartId);

private:
	uint32 m_chartColorDelta;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_CHARTPANEL_H
