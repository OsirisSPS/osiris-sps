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

#ifndef _OS_UI_MAINWINDOW_H
#define _OS_UI_MAINWINDOW_H

#include "boost/signals/trackable.hpp"
#include "downloadevent.h"
#include "multilanguagewindow.h"
#include "skinnedframe.h"
#include "singleton.h"
#include "thread.h"
#include "togglebitmapbutton.h"

//////////////////////////////////////////////////////////////////////

#define OS_UI_BACKGROUNDJOBSTIMER_TIMER			5000

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DraggablePanel;
class IRCWindow;
class IsisConnectionsWindow;
class LogMessage;
class LogWindow;
class P2PConnectionsWindow;
class StatisticsWindow;
class TaskBarIcon;
class wxSkinButton;

//////////////////////////////////////////////////////////////////////

class MainWindow : public SkinnedFrame<>,
                   public MultilanguageWindow,
				   public DynamicSingleton<MainWindow>,
				   public boost::signals::trackable
{
	DECLARE_EVENT_TABLE()

	typedef SkinnedFrame<> WindowBase;

// Construction
public:
    MainWindow();
	virtual ~MainWindow();

// Attributes
public:
	inline bool getInitialized() const;

	inline const wxIcon & getTrayIcon() const;
	inline const wxIcon & getWindowIcon() const;

	inline P2PConnectionsWindow * getP2PConnectionsWindow() const;
	inline LogWindow * getLogWindow() const;
	inline StatisticsWindow * getStatisticsWindow() const;

// Operations
public:
	//void checkForUpdates(); // TOCLEAN
	void configureOptions(bool modal);
	//void hideInTaskbar(bool hide);
	void showMode(bool hide);
	void init(bool startBackground, bool restorePos);
	void logMessages(const list<shared_ptr<LogMessage> >::type &logs);

	void setStatusText(const String &status);

private:
	void createLayout();
	void initMenu();
	void initWindow();
	void synchronizeCommands();
	void resizeWindow();
	void resetDownloadStatus();
	void updateBackgroundJobsStatus();
	void savePosition();
	bool restorePosition();

	void onDownloadStarting(DownloadEvent &e);
	void onDownloadCompleted(DownloadEvent &e);
	void onDownloadFailed(DownloadEvent &e);
	void onDownloadProgress(DownloadEvent &e);

	void onUpdateOptions();

// Events handlers
private:
    void onAbout(wxCommandEvent &e);
	void onClose(wxCloseEvent &e);
	void onConfigureOptions(wxCommandEvent &e);
	void onDevelCreateExtension(wxCommandEvent &e);
	void onDevelReloadExtensions(wxCommandEvent &e);
	void onDevelOpenLog(wxCommandEvent &e);
	void onDownloadNotify(DownloadEvent &e);
	void onIconize(wxIconizeEvent &e);
	void onExecuteCommand(wxCommandEvent &e);	
	void onImportFile(wxCommandEvent &e);
	void onPasteLink(wxCommandEvent &e);
	void onExit(wxCommandEvent &e);
	void onP2PEnableChanged(wxCommandEvent &e);
	void onIsisEnableChanged(wxCommandEvent &e);
	void onViewHome(wxCommandEvent &e);
	void onOpenDataFolder(wxCommandEvent &e);
	void onGotoHomePage(wxCommandEvent &e);
	void onGotoSupportPage(wxCommandEvent &e);
	void onIconizeWindow(wxCommandEvent &e);
	void onMaximizeWindow(wxCommandEvent &e);
	void onCloseWindow(wxCommandEvent &e);
	//void onSetupFilesAssociations(wxCommandEvent &e);
	//void onSetupRegistryProtocol(wxCommandEvent &e);
	void onSize(wxSizeEvent &e);
	void onBackgroundJobsTimer(wxTimerEvent &e);

public:
	static const uint32 cmdAbout;
	static const uint32 cmdExecuteCommand;
	static const uint32 cmdImportFile;
	static const uint32 cmdPasteLink;
	static const uint32 cmdExit;
	static const uint32 cmdViewHome;
	static const uint32 cmdRestore;
	static const uint32 cmdOpenDataFolder;
	static const uint32 cmdGotoHomePage;
	static const uint32 cmdGotoSupportPage;
	static const uint32 cmdConfigureOptions;
	static const uint32 cmdToggleP2PEnable;
	static const uint32 cmdToggleIsisEnable;
	static const uint32 cmdIconizeWindow;
	static const uint32 cmdMaximizeWindow;
	static const uint32 cmdCloseWindow;
	static const uint32 cmdBackgroundJobsTimer;
	//static const uint32 cmdSetupFilesAssociations;
	//static const uint32 cmdSetupRegistryProtocol;
	static const uint32 cmdDevelCreateExtension;
	static const uint32 cmdDevelReloadExtensions;
	static const uint32 cmdDevelOpenLog;

private:
	P2PConnectionsWindow *m_p2pConnectionsWindow;
	IsisConnectionsWindow *m_isisConnectionsWindow;
	LogWindow *m_logWindow;
	StatisticsWindow *m_statisticsWindow;
	IRCWindow *m_ircWindow;
	wxGauge *m_gauge;
	TaskBarIcon *m_taskbar;
	shared_ptr<Thread> m_download;
	wxIcon m_iconNormal;
	wxIcon m_iconBusy;
	wxIcon m_trayIconNormal;
	wxIcon m_trayIconBusy;
	wxAuiNotebook *m_pages;
	wxTimer *m_backgroundJobsTimer;
	bool m_downloading;
	bool m_initialized;
	bool m_hasBackgroundJobs;
	bool m_hiddenInTaskbar;

	bool m_closingInProgress;

	wxSkinButton *m_buttonHome;
	wxSkinButton *m_buttonWizard;
	ToggleBitmapButton *m_buttonP2PEnable;
	ToggleBitmapButton *m_buttonIsisEnable;
	wxStaticText* m_captionInfo;
	wxStaticText* m_captionStatus;

	wxBitmap m_bitmapBackgroundTopLeft;
	wxBitmap m_bitmapBackgroundTopCenter;
	wxBitmap m_bitmapBackgroundTopRight;
	wxBitmap m_bitmapBackgroundMiddleLeft;
	wxBitmap m_bitmapBackgroundMiddleRight;
	wxBitmap m_bitmapBackgroundBottomLeft;
	wxBitmap m_bitmapBackgroundBottomCenter;
	wxBitmap m_bitmapBackgroundBottomRight;
};

//////////////////////////////////////////////////////////////////////

inline bool MainWindow::getInitialized() const { return m_initialized; }

inline const wxIcon & MainWindow::getTrayIcon() const { return m_hasBackgroundJobs ? m_trayIconBusy : m_trayIconNormal; }
inline const wxIcon & MainWindow::getWindowIcon() const { return m_hasBackgroundJobs ? m_iconBusy : m_iconNormal; }

inline P2PConnectionsWindow * MainWindow::getP2PConnectionsWindow() const { return m_p2pConnectionsWindow; }
inline LogWindow * MainWindow::getLogWindow() const { return m_logWindow; }
inline StatisticsWindow * MainWindow::getStatisticsWindow() const { return m_statisticsWindow; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_MAINWINDOW_H
