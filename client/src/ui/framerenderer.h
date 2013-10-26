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

#ifndef _OS_SKIN_FRAMERENDERER_H
#define _OS_SKIN_FRAMERENDERER_H

#include "windowrenderer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class FrameRenderer : public WindowRenderer
{
	DECLARE_EVENT_TABLE()

	typedef WindowRenderer RendererBase;

// Construction
public:
	FrameRenderer(wxFrame *frame);
	virtual ~FrameRenderer();

// Attributes
public:
	inline bool getDraggable() const;
	inline void setDraggable(bool draggable);

	wxSize getMinSize() const;

// Operations
public:
	void setTopLeftImage(const wxImage &image);
	void setTopImage(const wxImage &image);
	void setTopRightImage(const wxImage &image);

	void setBodyLeftImage(const wxImage &image);
	void setBodyImage(const wxImage &image);
	void setBodyRightImage(const wxImage &image);

	void setBottomLeftImage(const wxImage &image);
	void setBottomImage(const wxImage &image);
	void setBottomRightImage(const wxImage &image);

private:
	void renderFrame(wxDC *dc);

// WindowRenderer overrides
protected:
	virtual void onPaint(wxPaintEvent &e);

protected:
	wxImage m_imageTopLeft;
	wxImage m_imageTop;
	wxImage m_imageTopRight;
	wxImage m_imageBodyLeft;
	wxImage m_imageBody;
	wxImage m_imageBodyRight;
	wxImage m_imageBottomLeft;
	wxImage m_imageBottom;
	wxImage m_imageBottomRight;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_SKIN_FRAMERENDERER_H
