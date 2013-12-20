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
#include "listctrlrenderer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ListCtrlRenderer::ListCtrlRenderer(wxListCtrl *listCtrl) : RendererBase(listCtrl)
{

}

ListCtrlRenderer::~ListCtrlRenderer()
{

}

void ListCtrlRenderer::onEraseBackground(wxEraseEvent &e)
{
	e.Skip();

	/*
	wxListCtrl *listCtrl = getWindow<wxListCtrl>();
	OS_ASSERT(listCtrl != nullptr);

	// to prevent flickering, erase only content *outside* of the actual list items stuff
	if(listCtrl->GetItemCount() == 0)
	{
		e.Skip();
		return;
	}

	wxDC *dc = e.GetDC();
	OS_ASSERT(dc != nullptr);

	// get some info
	wxCoord width = 0;
	wxCoord height = 0;
	listCtrl->GetClientSize(&width, &height);

	wxCoord x, y, w, h;
	dc->SetClippingRegion(0, 0, width, height);
	dc->GetClippingBox(&x, &y, &w, &h);

	long top_item = listCtrl->GetTopItem();
	long bottom_item = top_item + listCtrl->GetCountPerPage();
	if(bottom_item >= listCtrl->GetItemCount())
	   bottom_item = listCtrl->GetItemCount() - 1;

	// trick: exclude from the clip region from top left to bottom right
	wxRect top_rect;
	listCtrl->GetItemRect(top_item, top_rect, wxLIST_RECT_BOUNDS);

	wxRect bottom_rect;
	listCtrl->GetItemRect(bottom_item, bottom_rect, wxLIST_RECT_BOUNDS);

	// set the new clipping region and do erasing
	wxRect items_rect(top_rect.GetLeftTop(), bottom_rect.GetBottomRight());
	wxRegion reg(wxRegion(x-1, y-1, w+2, h+2));
	reg.Subtract(items_rect);
	dc->DestroyClippingRegion();
	dc->SetClippingRegion(reg);

	// do erasing
	dc->SetBackground(wxBrush(listCtrl->GetBackgroundColour(), wxSOLID));
	dc->Clear();

	// restore old clipping region
	dc->DestroyClippingRegion();
	dc->SetClippingRegion(wxRegion(x, y, w, h));
	*/
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
