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
#include "uiapi.h"

//////////////////////////////////////////////////////////////////////

DEFINE_LOCAL_EVENT_TYPE(OS_UI_EVENT_NOTIFY) 
DEFINE_LOCAL_EVENT_TYPE(wxEVT_DOWNLOAD)

//////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////

void centerWindow(wxWindow *window)
{
	// Fix MAC (wxWidgets 2.9.0): CenterOnScreen() non funziona, Center() non va in alcuni casi

	OS_ASSERT(window != null);

	wxSize size;
	window->GetSize(&size.x, &size.y);

	wxSize displaySize;
	wxDisplaySize(&displaySize.x, &displaySize.y);

	wxRect rect(displaySize.x/2 - size.x/2, displaySize.y/2 - size.y/2, size.x, size.y);
    window->SetSize(rect);	
}
