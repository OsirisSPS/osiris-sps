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
#include "windowrenderer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(WindowRenderer, EvtHandlerBase)
	EVT_ERASE_BACKGROUND(WindowRenderer::onEraseBackground)
	EVT_ENTER_WINDOW(WindowRenderer::onEnterWindow)
	EVT_LEAVE_WINDOW(WindowRenderer::onLeaveWindow)	
	EVT_PAINT(WindowRenderer::onPaint)
	EVT_SIZE(WindowRenderer::onSize)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

WindowRenderer::WindowRenderer(wxWindow *window)
{
	OS_ASSERT(window != nullptr);
	m_window = window;
	m_mouseOver = false;
}

WindowRenderer::~WindowRenderer()
{

}

void WindowRenderer::onEraseBackground(wxEraseEvent &e)
{
//	e.Skip();	
}

void WindowRenderer::onEnterWindow(wxMouseEvent &e)
{
	m_mouseOver = true;

	e.Skip();
}

void WindowRenderer::onLeaveWindow(wxMouseEvent &e)
{
	m_mouseOver = false;

	e.Skip();
}

void WindowRenderer::onPaint(wxPaintEvent &e)
{
	if(m_backgroundImage.IsOk() == false)
	{
		e.Skip();
		return;
	}

	wxPaintDC dc(getWindow());

	wxSize clientSize = getWindow()->GetClientSize();
	dc.DrawBitmap(m_backgroundImage.Scale(clientSize.x, clientSize.y), 0, 0);		
}

void WindowRenderer::onSize(wxSizeEvent &e)
{
	e.Skip();

	getWindow()->Refresh();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
