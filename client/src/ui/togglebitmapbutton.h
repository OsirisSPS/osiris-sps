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

#ifndef _OS_SKIN_TOGGLEBITMAPBUTTON_H
#define _OS_SKIN_TOGGLEBITMAPBUTTON_H

#include "wxskinbutton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ToggleBitmapButton : public wxSkinButton
{
	typedef wxSkinButton ButtonBase;

// Construction
public:
	ToggleBitmapButton(wxWindow *parent, wxWindowID id, const wxBitmap &bitmap = wxNullBitmap, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxBU_AUTODRAW, const wxValidator &validator = wxDefaultValidator, const wxString &name = wxEmptyString);
	virtual ~ToggleBitmapButton();

// Attributes
public:
	bool getChecked() const;
	void setChecked(bool checked);

	bool getAutoToggle() const;
	void setAutoToggle(bool autoToggle);

	const wxImage & getImageNormalChecked() const;
	void setImageNormalChecked(const wxImage &imageNormalChecked, bool deriveDisabled = true);

	const wxImage & getImageDownChecked() const;
    void setImageDownChecked(const wxImage &imageDownChecked);
   
	const wxImage & getImageFocusChecked() const;
	void setImageFocusChecked(const wxImage &imageFocusChecked);

	const wxImage & getImageDisabledChecked() const;
    void setImageDisabledChecked(const wxImage &imageDisabledChecked);

	const wxImage & getImageOverChecked() const;
    void setImageOverChecked(const wxImage &imageOverChecked);

// Operations
private:
	void loadImages();
	void updateImages(bool notify);

// Events
protected:
	void onClicked(wxCommandEvent &e);

// Overrides
protected:
	virtual void OnSetBitmap();

private:
	bool m_autoToggle;
	bool m_checked;
	wxImage m_buttonImageNormal;
	wxImage m_buttonImageDown;
	wxImage m_buttonImageFocus;
	wxImage m_buttonImageDisabled;
	wxImage m_buttonImageOver;
	wxImage m_buttonImageNormalChecked;
	wxImage m_buttonImageDownChecked;
	wxImage m_buttonImageFocusChecked;
	wxImage m_buttonImageDisabledChecked;
	wxImage m_buttonImageOverChecked;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_SKIN_TOGGLEBITMAPBUTTON_H
