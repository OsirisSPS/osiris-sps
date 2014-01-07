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

#ifndef _OS_SKIN_WXSKINBUTTON_H
#define _OS_SKIN_WXSKINBUTTON_H

#include "wxskinwindow.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class wxSkinButton : public wxSkinWindow
{
	typedef wxSkinWindow WindowBase;

	DECLARE_DYNAMIC_CLASS(wxSkinButton);
	DECLARE_EVENT_TABLE()

// Construction
public:
	wxSkinButton();
	wxSkinButton(wxWindow *parent, int id, const wxBitmap &bitmap = wxNullBitmap, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxNO_BORDER, const wxValidator &validator = wxDefaultValidator, const wxString &name = wxEmptyString);
 	virtual ~wxSkinButton();

// Attributes
public:
	const wxImage & getImageDown() const;
	void setImageDown(const wxImage &imageDown);

// wxSkinWindow interface
protected:
	virtual void DrawCustom(wxDC &dc);

// wxSkinWindow overrides
protected:
	virtual void SetLabel(const wxString &label) OS_OVERRIDE;
	virtual wxSize DoGetBestSize() const OS_OVERRIDE;

// Events
private:
	void OnLeftClick(wxMouseEvent &e);
	void OnLeftRelease(wxMouseEvent &e);

protected:
	wxImage m_imageDown;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_SKIN_WXSKINBUTTON_H
