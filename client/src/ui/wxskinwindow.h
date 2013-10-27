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

#ifndef _OS_SKIN_WXSKINWINDOW_H
#define _OS_SKIN_WXSKINWINDOW_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class wxSkinWindow : public wxWindow
{
	DECLARE_EVENT_TABLE()

// Construction
public:
	wxSkinWindow();
	wxSkinWindow(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxNO_BORDER, const wxString &name = wxEmptyString);
	virtual ~wxSkinWindow();

// Attributes
public:
	inline bool getScaleFill() const;
	inline bool setScaleFill(bool scaleFill);

	const wxImage & getImageNormal() const;
	void setImageNormal(const wxImage &imageNormal, bool deriveDisabled = true);

	const wxImage & getImageOver() const;
	void setImageOver(const wxImage &imageOver);

	const wxImage & getImageDisabled() const;
	void setImageDisabled(const wxImage &imageDisabled);	

	const wxImage & getImageFocus() const;
	void setImageFocus(const wxImage &imageFocus);

// Overridables
protected:
	virtual void DrawCustom(wxDC &dc);
	virtual void OnSetBitmap();

private:
	void OnEraseBackground(wxEraseEvent &e);
	void OnSize(wxSizeEvent &e);
	void OnPaint(wxPaintEvent &e);
	void OnEnterWindow(wxMouseEvent &e);
	void OnLeaveWindow(wxMouseEvent &e);
	
protected:
	wxImage m_imageNormal;
	wxImage m_imageOver;
	wxImage m_imageDisabled;
	wxImage m_imageFocus;
	bool m_scaleFill;
	bool m_inside;
};

//////////////////////////////////////////////////////////////////////

inline bool wxSkinWindow::getScaleFill() const { return m_scaleFill; }
inline bool wxSkinWindow::setScaleFill(bool scaleFill) { m_scaleFill = scaleFill; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_SKIN_WXSKINWINDOW_H
