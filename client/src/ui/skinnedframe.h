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

#ifndef _OS_SKIN_SKINNEDFRAME_H
#define _OS_SKIN_SKINNEDFRAME_H

#include "framerenderer.h"
#include "skinnedwindow.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T = wxFrame, typename R = FrameRenderer>
class SkinnedFrame : public SkinnedWindow<T, R>
{
	typedef SkinnedWindow<T, R> FrameBase;

// Construction
public:
	SkinnedFrame();
    SkinnedFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX | wxCLIP_CHILDREN | wxFRAME_SHAPED, const wxString &name = wxFrameNameStr);
	virtual ~SkinnedFrame();
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename R>
SkinnedFrame<T, R>::SkinnedFrame()
{

}

template <typename T, typename R>
SkinnedFrame<T, R>::SkinnedFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name) : FrameBase(parent, id, title, pos, size, style, name)
{

}

template <typename T, typename R>
SkinnedFrame<T, R>::~SkinnedFrame()
{

}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_SKIN_SKINNEDFRAME_H
