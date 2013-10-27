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

#ifndef _OS_UI_LOGWINDOW_H
#define _OS_UI_LOGWINDOW_H

#include "log.h"
#include "multilanguagewindow.h"
#include "skinnedwindow.h"
#include "togglebitmapbutton.h"
#include "wxskinbutton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class LogMessage;

//////////////////////////////////////////////////////////////////////

class LogWindow : public wxPanel,
                  public MultilanguageWindow
{
	DECLARE_EVENT_TABLE()

	typedef wxPanel WindowBase;

	typedef unordered_map<LogLevel, uint32>::type IconsMap;

// Enumerated types
private:
	enum LogColums
	{
		columnTimestamp			= 0,
		columnMessage			= 1,		
	};

// Construction
public:
    LogWindow(wxWindow *parent, wxWindowID id = static_cast<wxWindowID>(wxID_ANY), const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0);
	virtual ~LogWindow();

// Operations
public:
	void logMessage(shared_ptr<LogMessage> message);
	void logMessages(const list<shared_ptr<LogMessage> >::type &logs);
	void clearLogs();

private:
	void createLayout();
	void initWindow();
	void cleanupWindow();

// Events
private:
	void onClearLog(wxCommandEvent &e);
	void onLogSettings(wxCommandEvent &e);
	void onEnableLog(wxCommandEvent &e);
	void onEnableFileLog(wxCommandEvent &e);
	void onEnableBalloon(wxCommandEvent &e);
	void onChangeLogLevel(wxCommandEvent &e);	
	void onCopyLogMessage(wxCommandEvent &e);	
	void onLogMessageRightClick(wxListEvent &e);
	
private:
	SkinnedWindow<wxPanel> *m_commandsPanel;
	wxSkinButton *m_buttonClearLog;
	wxSkinButton *m_buttonLogSettings;
	wxListCtrl *m_logCtrl;
	wxImageList m_imageList;
	IconsMap m_iconsMap;
	bool m_enableAutoscoll;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_LOGWINDOW_H
