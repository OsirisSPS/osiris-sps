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
#include "splashscreen.h"

#include "application.h"
#include "boost/bind.hpp"
#include "conversions.h"
#include "notificationsmanager.h"
#include "uiapi.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(SplashScreen, WindowBase)
	EVT_ERASE_BACKGROUND(SplashScreen::OnEraseBackground)
	EVT_PAINT(SplashScreen::OnPaint)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

SplashScreen::SplashScreen(const wxString &title, const wxBitmap &image, wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style) : WindowBase(parent, id, title, pos, wxSize(image.GetWidth(), image.GetHeight()), style/*wxDEFAULT_FRAME_STYLE*/),
																																								m_image(image)
{
	NotificationsManager::instance()->getNotifyEvent()->connect(boost::bind(&SplashScreen::onNotify, this, _1));

	SetTransparent(230);
	centerWindow(this);

	Show(true);
}

SplashScreen::~SplashScreen()
{

}

void SplashScreen::setStatus(const String &status)
{
	m_status = status;

	Refresh(false);
	Update();

	// Forza il pump dei messaggi per mostrare la splash
	Application::instance()->dispatchEvents();
}

void SplashScreen::onNotify(const String &message)
{
	setStatus(message);
}

void SplashScreen::OnEraseBackground(wxEraseEvent &e)
{

}

void SplashScreen::OnPaint(wxPaintEvent &e)
{
    wxPaintDC paintDC(this);

    if(m_image.Ok() == false)
	{
		OS_ASSERTFALSE();
		return;
	}

	wxCoord width = m_image.GetWidth();
	wxCoord height = m_image.GetHeight();

	wxBitmap bmp(width, height);
	wxMemoryDC dc(bmp);
	dc.DrawBitmap(m_image, 0, 0);
	wxFont fontSplash(7,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,conversions::from_utf16<wxString>(_S("Small Fonts")),wxFONTENCODING_DEFAULT);
	//dc.SetFont(GetFont());
	dc.SetFont(fontSplash);	
	dc.SetTextForeground(*wxWHITE);
	wxSize sizeText = dc.GetTextExtent(conversions::from_utf16<wxString>(m_status));
	dc.DrawText(conversions::from_utf16<wxString>(m_status), 77, 61 + 15/2 - sizeText.GetY()/2);

	paintDC.Blit(0, 0, width, height, &dc, 0, 0);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
