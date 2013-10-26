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
#include "chartpanel.h"

//#include "boost/thread/recursive_mutex.hpp"
#include "conversions.h"
#include "lock.h"
#include "languageculture.h"
#include "languagemanager.h"
#include "options.h"
#include "realtimestats.h"
#include "statisticswindow.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(ChartPanel, wxPanel)
    EVT_PAINT(ChartPanel::onPaint)
    EVT_ERASE_BACKGROUND(ChartPanel::onEraseBackground)
	EVT_CHAR(ChartPanel::onChar)
    EVT_SIZE(ChartPanel::onSize)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

ChartPanel::ChartPanel(wxWindow *parent, wxWindowID winid, const wxPoint &pos, const wxSize &size, long style, const wxString &name) : PanelBase(parent, winid, pos, size, style, name)
{
	m_chartColorDelta = 0;
}

ChartPanel::~ChartPanel()
{

}

void ChartPanel::onPaint(wxPaintEvent &e)
{
	//RealtimeStatsScopeTimer RSS(_S("Activity"), _S("Paint charts"));

    wxPaintDC dc(this);
	wxBufferedDC bufferedDC(&dc);

	wxSize size = GetClientSize();
	size.SetWidth(size.GetWidth());
	size.SetHeight(size.GetHeight());
	drawChart(bufferedDC, wxRect(wxPoint(0,0),size));
}

void ChartPanel::onEraseBackground(wxEraseEvent &e)
{
	// Impedisce il ridisegnamento dello sfondo
}

void ChartPanel::onChar(wxKeyEvent &e)
{
	// Qui ci starebbero bene un pò di scorciatoie, con un 'h' che mostra help/elenco tasti nel grafico.
	if(e.GetKeyCode() == 99) // 'c' - Colors change
		m_chartColorDelta++;
}

void ChartPanel::updateStats()
{
	OS_LOCK(RealtimeStatsManager::getLock());

	RealtimeStatsManager::align();
	RealtimeStatsManager::update();
}

void ChartPanel::drawChart(wxDC &dc, wxRect rect)
{
	OS_LOCK(RealtimeStatsManager::getLock());

	if(RealtimeStatsManager::m_currentPreset == null)
		return;

	// Costanti:
	wxColour colourGrid = wxColour(70,70,70);
	wxColour colourOrange = wxColour(234,116,17);
	uint32 stepsInGridCell = 10;
	uint32 offsetVerticalPauseText = 40;

	uint32 width=rect.GetWidth();
	uint32 height=rect.GetHeight();

	wxBitmap bitmap(width, height);
	wxMemoryDC mdc;
	mdc.SelectObject(bitmap);

	mdc.SetBrush(wxBrush(*wxBLACK, wxSOLID));
    mdc.DrawRectangle(0, 0, width, height);

	uint32 paddingLeft = 50;
	uint32 paddingBottom = 30;
	uint32 padding = 10;

	// Mouse position
	int mouseX = wxGetMousePosition().x - this->GetScreenPosition().x; 
	int mouseY = wxGetMousePosition().y - this->GetScreenPosition().y;


	wxRect rectChart(paddingLeft, padding, width-padding-paddingLeft, height-paddingBottom-padding);

	mdc.SetBrush(wxBrush(wxColour(50,50,50), wxSOLID));
	mdc.SetPen(wxPen(wxColour(100,100,100), 1, wxSOLID));
    mdc.DrawRectangle(rectChart);

	// Grid
	mdc.SetPen(wxPen(colourGrid, 1, wxDOT));
	uint32 gridStepX = rectChart.GetWidth() / (RealtimeStatsManager::m_maxItems / stepsInGridCell);
	uint32 gridStepY = rectChart.GetHeight() / 10;
	for(int gridCurrentX = rectChart.GetLeft()+gridStepX; gridCurrentX < rectChart.GetRight(); gridCurrentX+=gridStepX)
	{
		mdc.DrawLine(gridCurrentX,rectChart.GetTop(),gridCurrentX,rectChart.GetBottom());
	}
	for(int gridCurrentY = rectChart.GetTop()+gridStepY; gridCurrentY < rectChart.GetBottom(); gridCurrentY+=gridStepY)
	{
		mdc.DrawLine(rectChart.GetLeft(), gridCurrentY, rectChart.GetRight(), gridCurrentY);
	}

	// Primo giro dei contatori:
	// Calcolo l'unit di misura, la scala unificata da usare, info sulla legenda etc.

	String unit; // Un'unica unit di misura
	double globalValueMin = 0;
	double globalValueMax = 0;

	// Legend
	uint32 distanceAxisTexts = 3;
	uint32 paddingLegend = 5;
	uint32 marginLegend = 5;
	uint32 heightLegendSpacer = 2;
	wxPoint pointLegendBodyBottomRight(rectChart.GetRight() - paddingLegend - marginLegend, rectChart.GetTop() + marginLegend + paddingLegend);
	wxPoint pointLegendBodyTopLeft(pointLegendBodyBottomRight.x, rectChart.GetTop() + marginLegend + paddingLegend);
	mdc.SetBrush(*wxTRANSPARENT_BRUSH);
	wxFont fontLegend(7,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,conversions::from_utf16<wxString>(_S("Small Fonts")),wxFONTENCODING_DEFAULT);
	mdc.SetFont(fontLegend);

	//mdc.DrawRectangle(pointLegendBodyTopLeft, pointLegendBodyBottomRight-pointLegendBodyTopLeft);

	int cIndex = 0;
	int nVisibleCounters = 0;
	int nTotalCounters = 0;
	for(RealtimeStatsManager::CountersMap::const_iterator i = RealtimeStatsManager::m_counters.begin(); i != RealtimeStatsManager::m_counters.end(); ++i)
	{
		nTotalCounters++;
		shared_ptr<RealtimeStatsCounter> counter = i->second;
		if(counter->isVisible())
		{
			nVisibleCounters++;

			if(unit.empty())
				unit = counter->m_unit;

			// Calcolo min/max della scala y
			double counterValueMin = counter->m_min;
			double counterValueMax = counter->m_max;
			double counterValueLast = counter->m_last;
			//pCounter->getValueBorder(counterValueMin, counterValueMax, counterValueLast);

			if(cIndex == 0)
			{
				globalValueMin = counterValueMin;
				globalValueMax = counterValueMax;
			}
			else
			{
				globalValueMin = std::min(globalValueMin, counterValueMin);
				globalValueMax = std::max(globalValueMax, counterValueMax);
			}

			wxColour counterColor = getChartColor(counter->getId());
			wxColour counterColorShadow = wxColour(0,0,0);
			//wxPen counterPen(counterColor, 1, wxSOLID);
			//wxPen counterPenShadow(wxColour(0,0,0), 1, wxSOLID);
			//mdc.SetPen(counterPen);

			//String legendText = String::format(_S("%S : %g"), pCounter->m_name.c_str(), counterValueLast);
			String legendText = String::format(getText(_S("ui.mainframe.statistics.legend.row")).c_str(), counter->m_name.c_str(), counter->m_currentValue,counterValueMin,counterValueMax);

			wxSize sizeTextLegend = mdc.GetTextExtent(conversions::from_utf16<wxString>(legendText));

			if(pointLegendBodyBottomRight.x - sizeTextLegend.GetWidth() < pointLegendBodyTopLeft.x)
				pointLegendBodyTopLeft.x = pointLegendBodyBottomRight.x - sizeTextLegend.GetWidth();

			uint32 legendTextX = pointLegendBodyBottomRight.x - sizeTextLegend.GetWidth();
			uint32 legendTextY = pointLegendBodyBottomRight.y;

			mdc.SetTextForeground(counterColorShadow);
			mdc.DrawText(conversions::from_utf16<wxString>(legendText), legendTextX+1, legendTextY+1);
			mdc.SetTextForeground(counterColor);
			mdc.DrawText(conversions::from_utf16<wxString>(legendText), legendTextX, legendTextY);
			pointLegendBodyBottomRight.y += sizeTextLegend.GetHeight() + heightLegendSpacer;

			cIndex++;
		}
	}

	bool haveData = (cIndex != 0);

	if(haveData == false)
	{
		wxFont fontAxis(20,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,conversions::from_utf16<wxString>(_S("Arial")),wxFONTENCODING_DEFAULT);
		mdc.SetFont(fontAxis);
		mdc.SetTextForeground(wxColour(100,100,100));
		String textNoData = getText(_S("ui.mainframe.statistics.no_data"));
		wxSize sizeTextNoData = mdc.GetTextExtent(conversions::from_utf16<wxString>(textNoData));

		mdc.DrawText(conversions::from_utf16<wxString>(textNoData),	(rectChart.GetLeft() + rectChart.GetRight())/2 - sizeTextNoData.GetWidth()/2,
											(rectChart.GetTop() + rectChart.GetBottom())/2 - sizeTextNoData.GetHeight()/2);
	}
	else
	{
		mdc.SetPen(wxPen(wxColour(100,100,100), 1, wxSOLID));
		mdc.DrawRectangle(pointLegendBodyTopLeft.x - paddingLegend, pointLegendBodyTopLeft.y - paddingLegend, pointLegendBodyBottomRight.x - pointLegendBodyTopLeft.x + paddingLegend*2, pointLegendBodyBottomRight.y - pointLegendBodyTopLeft.y + paddingLegend*2);

		double stepWidth = double(rectChart.GetWidth()-1) / double(RealtimeStatsManager::m_maxItems - 1);
		double scaleY = 1;

		double valueRange = 0;
		if( (RealtimeStatsManager::m_currentPreset->m_scaleMin != 0) ||
			(RealtimeStatsManager::m_currentPreset->m_scaleMax != 0) )
		{
			// Fixed scale
			globalValueMin = RealtimeStatsManager::m_currentPreset->m_scaleMin;
			globalValueMax = RealtimeStatsManager::m_currentPreset->m_scaleMax;
		}

		valueRange = globalValueMax - globalValueMin;		
		if(valueRange != 0)
		{
			scaleY = (rectChart.GetHeight()-2) / valueRange;
		}

		cIndex = 0;
		for(RealtimeStatsManager::CountersMap::const_iterator i = RealtimeStatsManager::m_counters.begin(); i != RealtimeStatsManager::m_counters.end(); ++i)
		{
			shared_ptr<RealtimeStatsCounter> counter = i->second;
			if(counter->isVisible())
			{
				wxPen counterPen(getChartColor(counter->getId()), 1, wxSOLID);
				mdc.SetPen(counterPen);

				uint32 deltaStart = 0; // Se ha meno valori di maxSteps, calcolo questo delta per far s che sia allineato a destra

				if(counter->m_values.size() != RealtimeStatsManager::m_maxItems)
					deltaStart = (RealtimeStatsManager::m_maxItems - counter->m_values.size()) * stepWidth;

				// Chart
				uint32 s = 0;
				wxPoint lastPoint(0,0);
				for(RealtimeStatsCounter::Values::const_iterator i = counter->m_values.begin(); i != counter->m_values.end(); ++i)
				{
					double value = *i;

					uint32 x = rectChart.GetLeft() + deltaStart + s * stepWidth;
					uint32 y = rectChart.GetBottom() - (value - globalValueMin) * scaleY;
				
					if(y + cIndex < uint32(rectChart.GetBottom()))
						y += cIndex; // Shift verticale, per vedere distintamente i contatori con valori sovrapposti.

					wxPoint newPoint(x,y);

					if(s != 0)
						mdc.DrawLine(lastPoint, newPoint);

					lastPoint = newPoint;
					s++;
				}

				// Current Value
				{
					//uint32 x1 = rectChart.GetRight();
					uint32 x2 = rectChart.GetRight() + 5;
					uint32 y = rectChart.GetBottom() - (counter->m_currentValue - globalValueMin) * scaleY;
					if(y + cIndex < uint32(rectChart.GetBottom()))
						y += cIndex; // Shift verticale, per vedere distintamente i contatori con valori sovrapposti.
					//mdc.DrawLine(wxPoint(x1,y),wxPoint(x2,y));
					mdc.DrawLine(lastPoint,wxPoint(x2,y));					
				}

				cIndex++;
			}
		}

		// Mouse position
		if(rectChart.Contains(mouseX, mouseY))
		{
			wxColour mouseTooltipColor = wxColour(192,192,192);
			
			mdc.SetPen(wxPen(mouseTooltipColor, 1, wxDOT));
			mdc.DrawLine(mouseX,rectChart.GetTop(),mouseX,rectChart.GetBottom());
			mdc.DrawLine(rectChart.GetLeft(),mouseY,rectChart.GetRight(),mouseY);

			uint32 xx = mouseY - rectChart.GetBottom();
			double mouseStepAgo = (mouseX - rectChart.GetLeft()) / stepWidth;
			double mouseLogicalY = -(mouseY - rectChart.GetBottom()) / scaleY + globalValueMin;

			String stepAgoText = formatTimeSpan(RealtimeStatsManager::m_resolution * (RealtimeStatsManager::m_maxItems - mouseStepAgo));

			String textTooltip = String::format(getText(_S("ui.mainframe.statistics.mouse_tooltip")).c_str(), mouseLogicalY, stepAgoText.c_str());
			
			wxSize sizeTextTooltip = mdc.GetTextExtent(conversions::from_utf16<wxString>(textTooltip));

			int distanceDelta = 15;

			int textX = mouseX + distanceDelta;
			if((mouseX + distanceDelta + sizeTextTooltip.x) > rectChart.GetRight())
				textX = mouseX - distanceDelta - sizeTextTooltip.x;
			int textY = mouseY + distanceDelta;
			if((mouseY + distanceDelta + sizeTextTooltip.y) > rectChart.GetBottom())
				textY = mouseY - distanceDelta - sizeTextTooltip.y;

			
			mdc.SetTextForeground(mouseTooltipColor);
			mdc.DrawText(conversions::from_utf16<wxString>(textTooltip), textX, textY);
		}
	}

	// Texts
	wxFont fontAxis(8,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,conversions::from_utf16<wxString>(_S("Arial")),wxFONTENCODING_DEFAULT);
	mdc.SetFont(fontAxis);
	mdc.SetTextForeground(wxColour(255,255,255));
	String textAxisX = getText(_S("ui.mainframe.statistics.grid_x"));
	String textAxisY = unit;
	String textGridX = String::format(getText(_S("ui.mainframe.statistics.grid_x_left")).c_str(), 
		formatTimeSpan(RealtimeStatsManager::m_resolution * stepsInGridCell).c_str(),
		formatTimeSpan(RealtimeStatsManager::m_resolution).c_str());
	
	String textPreset = String::format(getText(_S("ui.mainframe.statistics.current_preset")).c_str(), RealtimeStatsManager::m_currentPreset->getName().c_str(), nVisibleCounters, nTotalCounters);

	String textValueMin = String::format(_S("%g").c_str(), globalValueMin);
	String textValueMax = String::format(_S("%g").c_str(), globalValueMax);

	wxSize sizeTextAxisX = mdc.GetTextExtent(conversions::from_utf16<wxString>(textAxisX));
	wxSize sizeTextAxisY = mdc.GetTextExtent(conversions::from_utf16<wxString>(textAxisY));
	wxSize sizeTextValueMin = mdc.GetTextExtent(conversions::from_utf16<wxString>(textValueMin));
	wxSize sizeTextValueMax = mdc.GetTextExtent(conversions::from_utf16<wxString>(textValueMax));

	mdc.DrawText(conversions::from_utf16<wxString>(textAxisX), (rectChart.GetLeft() + rectChart.GetRight())/2-sizeTextAxisX.GetWidth()/2, rectChart.GetBottom() + distanceAxisTexts);
	mdc.DrawText(conversions::from_utf16<wxString>(textGridX), rectChart.GetLeft(), rectChart.GetBottom() + distanceAxisTexts);
	mdc.DrawRotatedText(conversions::from_utf16<wxString>(textAxisY), rectChart.GetLeft() - sizeTextAxisY.GetHeight() - distanceAxisTexts, (rectChart.GetTop() + rectChart.GetBottom())/2-sizeTextAxisY.GetWidth()/2, 90);
	mdc.DrawText(conversions::from_utf16<wxString>(textPreset), rectChart.GetLeft() + distanceAxisTexts, rectChart.GetTop() + distanceAxisTexts);

	mdc.DrawText(conversions::from_utf16<wxString>(textValueMax), rectChart.GetLeft() - sizeTextValueMax.GetWidth() - distanceAxisTexts, rectChart.GetTop());
	mdc.DrawText(conversions::from_utf16<wxString>(textValueMin), rectChart.GetLeft() - sizeTextValueMin.GetWidth() - distanceAxisTexts, rectChart.GetBottom() - sizeTextValueMin.GetHeight());

	if(RealtimeStatsManager::getPause())
	{
		wxFont fontAxis(16,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,conversions::from_utf16<wxString>(_S("Arial")),wxFONTENCODING_DEFAULT);
		mdc.SetFont(fontAxis);
		mdc.SetTextForeground(wxColour(100,100,100));
		String textNoData = getText(_S("ui.mainframe.statistics.paused"));
		wxSize sizeTextNoData = mdc.GetTextExtent(conversions::from_utf16<wxString>(textNoData));

		mdc.DrawText(conversions::from_utf16<wxString>(textNoData),	(rectChart.GetLeft() + rectChart.GetRight())/2 - sizeTextNoData.GetWidth()/2,
											(rectChart.GetTop() + rectChart.GetBottom())/2 - sizeTextNoData.GetHeight()/2 + offsetVerticalPauseText);
	}

	dc.Blit(rect.GetLeft(),rect.GetTop(),width,height,&mdc,0,0);
}

void ChartPanel::onSize(wxSizeEvent &e)
{
    e.Skip();

    Refresh();
}

String ChartPanel::formatTimeSpan(double time)
{
	return LanguageManager::instance()->getCulture(Options::instance()->getDefaultLanguage())->formatTime(time);
}

wxColour ChartPanel::getChartColor(uint32 chartId)
{
	switch((chartId - m_chartColorDelta) % 7)
	{
	case 0: return wxColour(255,0,0);
	case 1: return wxColour(255,255,0);
	case 2: return wxColour(80,80,255);
	case 3: return wxColour(0,255,0);
	case 4: return wxColour(0,255,255);
	case 5: return wxColour(255,0,255);
	case 6: return wxColour(255,255,255);
	default: return wxColour(255,255,255);
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
