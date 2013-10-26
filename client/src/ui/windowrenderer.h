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

#ifndef _OS_SKIN_WINDOWRENDERER_H
#define _OS_SKIN_WINDOWRENDERER_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class WindowRenderer : public wxEvtHandler
{
	DECLARE_EVENT_TABLE()

	typedef wxEvtHandler EvtHandlerBase;

// Construction
public:
	WindowRenderer(wxWindow *window);
	virtual ~WindowRenderer();

// Attributes
public:
	inline bool getMouseOver() const;
	inline wxWindow * getWindow() const;
	
	template <typename T>
	inline T * getWindow() const;

	inline const wxImage & getBackgroundImage() const;
	inline void setBackgroundImage(const wxImage &backgroundImage);

// Overridables
protected:
	virtual void onEraseBackground(wxEraseEvent &e);
	virtual void onEnterWindow(wxMouseEvent &e);
	virtual void onLeaveWindow(wxMouseEvent &e);
	virtual void onPaint(wxPaintEvent &e);
	virtual void onSize(wxSizeEvent &e);

protected:
	wxWindow *m_window;	
	bool m_mouseOver;
	wxImage m_backgroundImage;
};

//////////////////////////////////////////////////////////////////////

inline bool WindowRenderer::getMouseOver() const { return m_mouseOver; }
inline wxWindow * WindowRenderer::getWindow() const { return m_window; }

template <typename T>
inline T * WindowRenderer::getWindow() const { return dynamic_cast<T *>(m_window); }

inline const wxImage & WindowRenderer::getBackgroundImage() const { return m_backgroundImage; }
inline void WindowRenderer::setBackgroundImage(const wxImage &backgroundImage) { m_backgroundImage = backgroundImage; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_SKIN_WINDOWRENDERER_H
