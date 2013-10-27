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
#include "wxskinbutton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxSkinButton, WindowBase)
	EVT_LEFT_DOWN(wxSkinButton::OnLeftClick)
	EVT_LEFT_UP(wxSkinButton::OnLeftRelease)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxSkinButton, WindowBase)

//////////////////////////////////////////////////////////////////////

wxSkinButton::wxSkinButton()
{

}

wxSkinButton::wxSkinButton(wxWindow *parent, int id, const wxBitmap &bitmap, const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator, const wxString &name) : WindowBase(parent, id, pos, size, style, name)
{

}

wxSkinButton::~wxSkinButton()
{

}

const wxImage & wxSkinButton::getImageDown() const
{
	return m_imageDown;
}

void wxSkinButton::setImageDown(const wxImage &imageDown)
{
	m_imageDown = imageDown;
	OnSetBitmap();
}

void wxSkinButton::SetLabel(const wxString& label)
{
	WindowBase::SetLabel(label);

	Refresh();
}

wxSize wxSkinButton::DoGetBestSize() const
{
	wxString tmp = GetLabel();
	if(tmp.empty() == false)
	{	
		wxClientDC dc(const_cast<wxSkinButton *>(this));
		wxSize extent = dc.GetTextExtent(tmp);
		return wxSize(extent.x + 6, extent.y + 6);
	}
	else
	{
		if(m_imageNormal.IsOk())
			return wxSize(m_imageNormal.GetWidth(), m_imageNormal.GetHeight());
	}

	return wxSize(10, 10);
}

void wxSkinButton::OnLeftClick(wxMouseEvent &e)
{
	if(IsEnabled() && m_imageDown.IsOk())
	{
		wxClientDC dc(this);

		wxSize size = GetSize();
		dc.DrawBitmap(m_imageDown.Scale(size.x, size.y), 0, 0);
	}
}

void wxSkinButton::OnLeftRelease(wxMouseEvent &e)
{
	if(IsEnabled())
	{
		Refresh();
		
    	wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		e.SetEventObject(this);
    	GetEventHandler()->ProcessEvent(e);
	}
}

void wxSkinButton::DrawCustom(wxDC &dc)
{
	WindowBase::DrawCustom(dc);

	wxString label = GetLabel();
	if(label.empty() == false)
	{
		dc.SetFont(GetFont());
		if(IsEnabled())
			dc.SetTextForeground(GetForegroundColour());
		else
			dc.SetTextForeground(wxGREY_PEN->GetColour());

		wxSize size = GetSize();		
		wxSize extent = dc.GetTextExtent(label);

		dc.DrawText(label, (size.x - extent.x) / 2, (size.y - extent.y) / 2);
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
