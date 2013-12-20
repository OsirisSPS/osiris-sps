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

#ifndef _OS_UI_SPLASHSCREEN_H
#define _OS_UI_SPLASHSCREEN_H

#include "boost/signals/trackable.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class SplashScreen : public wxFrame,
					 public boost::signals::trackable
{
	DECLARE_EVENT_TABLE()

	typedef wxFrame WindowBase;

// Construction
public:
	SplashScreen(const wxString &title, const wxBitmap &image, wxWindow *parent = nullptr, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxNO_BORDER | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP);
	virtual ~SplashScreen();

// Operations
public:
	void setStatus(const String &status);

private:
	void onNotify(const String &message);

// Events
private:
	void OnEraseBackground(wxEraseEvent &e);
	void OnPaint(wxPaintEvent &e);

private:
	String m_status;
	wxBitmap m_image;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_SPLASHSCREEN_H
