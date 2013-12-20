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

#ifndef _OS_UI_UI_H
#define _OS_UI_UI_H

//////////////////////////////////////////////////////////////////////

#if OS_PLATFORM	== OS_PLATFORM_WIN
    #define OS_UI_ENTRY_POINT_DECLARE_PARAMS	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
	#define OS_UI_ENTRY_POINT_ENUM_PARAMS		hInstance, hPrevInstance, lpCmdLine, nCmdShow
	#define OS_UI_ENTRY_POINT_RETURN_VALUE		int
	#define OS_UI_ENTRY_POINT()					OS_UI_ENTRY_POINT_RETURN_VALUE WINAPI WinMain(OS_UI_ENTRY_POINT_DECLARE_PARAMS)
#else
	#define OS_UI_ENTRY_POINT_DECLARE_PARAMS	int argc, char *argv[]
	#define OS_UI_ENTRY_POINT_ENUM_PARAMS		argc, argv
	#define OS_UI_ENTRY_POINT_RETURN_VALUE		int
	#define OS_UI_ENTRY_POINT()					OS_UI_ENTRY_POINT_RETURN_VALUE main(OS_UI_ENTRY_POINT_DECLARE_PARAMS)
#endif

//////////////////////////////////////////////////////////////////////

extern "C" UIExport OS_UI_ENTRY_POINT_RETURN_VALUE runUI(OS_UI_ENTRY_POINT_DECLARE_PARAMS);

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_UI_H
