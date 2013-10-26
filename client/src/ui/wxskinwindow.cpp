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
#include "wxskinwindow.h"

#include "wx/image.h"
#include "wx/dcbuffer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxSkinWindow,wxWindow)
	EVT_ERASE_BACKGROUND(wxSkinWindow::OnEraseBackground)
	EVT_PAINT(wxSkinWindow::OnPaint)
	EVT_SIZE(wxSkinWindow::OnSize)
	EVT_ENTER_WINDOW(wxSkinWindow::OnEnterWindow)
	EVT_LEAVE_WINDOW(wxSkinWindow::OnLeaveWindow)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

wxSkinWindow::wxSkinWindow() : m_inside(false),
							   m_scaleFill(false)
{

}

wxSkinWindow::wxSkinWindow(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name) : wxWindow(parent, id, pos, size, style, name),
																																		m_inside(false),
																																		m_scaleFill(false)
{

}

wxSkinWindow::~wxSkinWindow()
{

}

const wxImage & wxSkinWindow::getImageNormal() const
{
	return m_imageNormal;
}

void wxSkinWindow::setImageNormal(const wxImage &imageNormal, bool deriveDisabled)
{
	m_imageNormal = imageNormal;
	if(deriveDisabled)
		m_imageDisabled = imageNormal.ConvertToGreyscale();		
	else
		m_imageDisabled = imageNormal;
	
	OnSetBitmap();
}

const wxImage & wxSkinWindow::getImageOver() const
{
	return m_imageOver;
}

void wxSkinWindow::setImageOver(const wxImage &imageOver)
{
	m_imageOver = imageOver;
	OnSetBitmap();
}

const wxImage & wxSkinWindow::getImageDisabled() const
{
	return m_imageDisabled;
}

void wxSkinWindow::setImageDisabled(const wxImage &imageDisabled)
{
	m_imageDisabled = imageDisabled;
	OnSetBitmap();
}

const wxImage & wxSkinWindow::getImageFocus() const
{
	return m_imageFocus;
}

void wxSkinWindow::setImageFocus(const wxImage &imageFocus)
{
	m_imageFocus = imageFocus;
	OnSetBitmap();
}		

void wxSkinWindow::OnSize(wxSizeEvent& e)
{
	e.Skip();

	Refresh();	
}

void wxSkinWindow::OnEraseBackground(wxEraseEvent &e)
{

}

void wxSkinWindow::OnPaint(wxPaintEvent& e)
{
	wxBufferedPaintDC dcp(this);

	wxSize size = GetSize();

	if(m_inside && m_imageOver.IsOk())
	{
		dcp.DrawBitmap(m_scaleFill ? wxBitmap(m_imageOver.Scale(size.x, size.y)) : m_imageOver, 0, 0, true);
	}
	else if(IsEnabled() && (m_imageNormal.IsOk() || m_imageFocus.IsOk()))
	{
		if(m_imageFocus.IsOk() && HasFocus())
			dcp.DrawBitmap(m_scaleFill ? wxBitmap(m_imageFocus.Scale(size.x, size.y)) : m_imageFocus, 0, 0, true);
		else
			dcp.DrawBitmap(m_scaleFill ? wxBitmap(m_imageNormal.Scale(size.x, size.y)) : m_imageNormal, 0, 0, true);
	}
	else if(m_imageDisabled.IsOk())
	{
		dcp.DrawBitmap(m_scaleFill ? wxBitmap(m_imageDisabled.Scale(size.x, size.y)) : m_imageDisabled, 0, 0, true);
	}
	
	DrawCustom(dcp);
}

void wxSkinWindow::OnEnterWindow(wxMouseEvent &e)
{
	e.Skip();

	m_inside = true;
	
	if(m_imageOver.IsOk())
		Refresh();	
}

void wxSkinWindow::OnLeaveWindow(wxMouseEvent &e)
{
	e.Skip();

	m_inside = false;

	Refresh();
}

void wxSkinWindow::DrawCustom(wxDC &dc)
{

}

void wxSkinWindow::OnSetBitmap()
{
	Refresh();	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
