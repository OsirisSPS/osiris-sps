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

#ifndef _OS_SKIN_WXWIDGETS_H
#define _OS_SKIN_WXWIDGETS_H

//////////////////////////////////////////////////////////////////////

#ifdef ssize_t
	#undef ssize_t
#endif

#if OS_PLATFORM	== OS_PLATFORM_WIN
    #define HAVE_SNPRINTF_DECL

	#define OS_WXWIDGETS_ENTRY_POINT_DECLARE_PARAMS		HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
	#define OS_WXWIDGETS_ENTRY_POINT_ENUM_PARAMS		hInstance, hPrevInstance, lpCmdLine, nCmdShow
	#define OS_WXWIDGETS_ENTRY_POINT_RETURN_VALUE		int
	#define OS_WXWIDGETS_ENTRY_POINT()					OS_WXWIDGETS_ENTRY_POINT_RETURN_VALUE WINAPI WinMain(OS_WXWIDGETS_ENTRY_POINT_DECLARE_PARAMS)
#else
	#define OS_WXWIDGETS_ENTRY_POINT_DECLARE_PARAMS		int argc, char *argv[]
	#define OS_WXWIDGETS_ENTRY_POINT_ENUM_PARAMS		argc, argv
	#define OS_WXWIDGETS_ENTRY_POINT_RETURN_VALUE		int
	#define OS_WXWIDGETS_ENTRY_POINT()					OS_WXWIDGETS_ENTRY_POINT_RETURN_VALUE main(OS_WXWIDGETS_ENTRY_POINT_DECLARE_PARAMS)
#endif

//////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(disable : 4267)
#endif

//////////////////////////////////////////////////////////////////////

#include "wx/aui/auibook.h"
#include "wx/dcbuffer.h"
#include "wx/cmdline.h"
#include "wx/event.h"
#include "wx/file.h"
#include "wx/gbsizer.h"
#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/listctrl.h"
#include "wx/mimetype.h"
#include "wx/panel.h"
#include "wx/richtext/richtextctrl.h"
#include "wx/snglinst.h"
#include "wx/socket.h"
#include "wx/sound.h"
#include "wx/splash.h"
#include "wx/splitter.h"
#include "wx/statline.h"
#include "wx/taskbar.h"
#include "wx/url.h"
#include "wx/wizard.h"
#include "wx/wupdlock.h"

#if OS_PLATFORM == OS_PLATFORM_WIN
    #include "wx/msw/registry.h"
#endif

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(default : 4267)	
	
	#include "configcompilermsvcwarnings.h"		// Alcuni headers delle wx riportano lo stato di alcuni warnings (es.: 4275) al valore di default pertanto è necessario ripristinare la configurazione originale
#endif

//////////////////////////////////////////////////////////////////////

#endif // _OS_SKIN_WXWIDGETS_H
