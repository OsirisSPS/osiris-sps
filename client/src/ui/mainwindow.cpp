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
#include "mainwindow.h"

#include "application.h"
#include "boost/bind.hpp"
#include "conversions.h"
#include "downloadthread.h"
#include "engine.h"
#include "extensionssystem.h"
#include "extensionswizard.h"
#include "filesystem.h"
#include "httpurl.h"
#include "idesystem.h"
#include "ircwindow.h"
#include "isis.h"
#include "isisconnectionswindow.h"
#include "filesystem.h"
#include "log.h"
#include "logmessage.h"
#include "logwindow.h"
#include "objectssystem.h"
#include "options.h"
#include "optionswizard.h"
#include "osirislink.h"
#include "p2pconnectionswindow.h"
#include "platformmanager.h"
#include "portalsportalssystem.h"
#include "statisticswindow.h"
#include "taskbaricon.h"
#include "uiapi.h"
#include "uistandards.h"
#include "version.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "wxskinbutton.h"

#include <wx/textdlg.h>

#if wxCHECK_VERSION(2, 9, 2)
#include <wx/richmsgdlg.h>
#endif

//////////////////////////////////////////////////////////////////////

#define OS_UI_OPTION_MAIN_WINDOW_POSITION			"ui.main_window.position"
#define OS_UI_OPTION_MAIN_WINDOW_FLAGS				"ui.main_window.flags"

#define OS_UI_OPTION_MAIN_WINDOW_FLAGS_MAXIMIZED	1
#define OS_UI_OPTION_MAIN_WINDOW_FLAGS_ICONIZED		2

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SINGLETON(OS_NAMESPACE_NAME::MainWindow)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const uint32 MainWindow::cmdAbout = wxID_ABOUT;
const uint32 MainWindow::cmdExecuteCommand = wxNewId();
const uint32 MainWindow::cmdPasteLink = wxNewId();
const uint32 MainWindow::cmdImportFile = wxNewId();
const uint32 MainWindow::cmdExit = wxID_EXIT;
const uint32 MainWindow::cmdViewHome = wxNewId();
const uint32 MainWindow::cmdRestore = wxNewId();
const uint32 MainWindow::cmdOpenDataFolder = wxNewId();
const uint32 MainWindow::cmdGotoHomePage = wxNewId();
const uint32 MainWindow::cmdGotoSupportPage = wxNewId();
const uint32 MainWindow::cmdConfigureOptions = wxID_PREFERENCES;
const uint32 MainWindow::cmdToggleP2PEnable = wxNewId();
const uint32 MainWindow::cmdToggleIsisEnable = wxNewId();
const uint32 MainWindow::cmdIconizeWindow = wxNewId();
const uint32 MainWindow::cmdMaximizeWindow = wxNewId();
const uint32 MainWindow::cmdCloseWindow = wxNewId();
const uint32 MainWindow::cmdBackgroundJobsTimer = wxNewId();
//const uint32 MainWindow::cmdSetupFilesAssociations = wxNewId();
//const uint32 MainWindow::cmdSetupRegistryProtocol = wxNewId();
const uint32 MainWindow::cmdDevelCreateExtension = wxNewId();
const uint32 MainWindow::cmdDevelReloadExtensions = wxNewId();
const uint32 MainWindow::cmdDevelOpenLog = wxNewId();

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MainWindow, WindowBase)
	EVT_BUTTON(cmdConfigureOptions, MainWindow::onConfigureOptions)
	EVT_BUTTON(cmdViewHome, MainWindow::onViewHome)
	EVT_BUTTON(cmdIconizeWindow, MainWindow::onIconizeWindow)
	EVT_BUTTON(cmdMaximizeWindow, MainWindow::onMaximizeWindow)
	EVT_BUTTON(cmdCloseWindow, MainWindow::onCloseWindow)
	EVT_BUTTON(cmdToggleP2PEnable, MainWindow::onP2PEnableChanged)
	EVT_BUTTON(cmdToggleIsisEnable, MainWindow::onIsisEnableChanged)
	EVT_CLOSE(MainWindow::onClose)
	EVT_DOWNLOAD(MainWindow::onDownloadNotify)
	EVT_ICONIZE(MainWindow::onIconize)
    EVT_MENU(cmdAbout, MainWindow::onAbout)
	EVT_MENU(cmdExecuteCommand, MainWindow::onExecuteCommand)
	EVT_MENU(cmdPasteLink, MainWindow::onPasteLink)
	EVT_MENU(cmdImportFile, MainWindow::onImportFile)
	EVT_MENU(cmdExit, MainWindow::onExit)
	EVT_MENU(cmdViewHome, MainWindow::onViewHome)
	EVT_MENU(cmdOpenDataFolder, MainWindow::onOpenDataFolder)
	EVT_MENU(cmdGotoHomePage, MainWindow::onGotoHomePage)
	EVT_MENU(cmdGotoSupportPage, MainWindow::onGotoSupportPage)
	EVT_MENU(cmdConfigureOptions, MainWindow::onConfigureOptions)
	//EVT_MENU(cmdSetupFilesAssociations, MainWindow::onSetupFilesAssociations)
	//EVT_MENU(cmdSetupRegistryProtocol, MainWindow::onSetupRegistryProtocol)
	EVT_MENU(cmdDevelCreateExtension, MainWindow::onDevelCreateExtension)
	EVT_MENU(cmdDevelReloadExtensions, MainWindow::onDevelReloadExtensions)
	EVT_MENU(cmdDevelOpenLog, MainWindow::onDevelOpenLog)
	EVT_SIZE(MainWindow::onSize)
	EVT_TIMER(cmdBackgroundJobsTimer, MainWindow::onBackgroundJobsTimer)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

MainWindow::MainWindow() : WindowBase(nullptr, static_cast<wxWindowID>(wxID_ANY), wxEmptyString, wxDefaultPosition, wxSize(500, 400), wxDEFAULT_FRAME_STYLE),
						   m_p2pConnectionsWindow(nullptr),
						   m_isisConnectionsWindow(nullptr),
						   m_logWindow(nullptr),
						   m_statisticsWindow(nullptr),
						   m_ircWindow(nullptr),
						   m_gauge(nullptr),
						   m_pages(nullptr),
						   m_buttonHome(nullptr),
						   m_buttonWizard(nullptr),
						   m_buttonP2PEnable(nullptr),
						   m_buttonIsisEnable(nullptr),
						   m_captionInfo(nullptr),
						   m_captionStatus(nullptr),
						   m_downloading(false),
						   m_initialized(false),
						   m_taskbar(new TaskBarIcon()),
						   m_hasBackgroundJobs(false),
						   m_hiddenInTaskbar(false),
						   m_closingInProgress(false)
{
    CreateStatusBar(1);
	m_gauge = new wxGauge(GetStatusBar(), static_cast<wxWindowID>(wxID_ANY), 0, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL | wxGA_SMOOTH | wxNO_BORDER);

	m_backgroundJobsTimer = new wxTimer(this, cmdBackgroundJobsTimer);
	m_backgroundJobsTimer->Start(OS_UI_BACKGROUNDJOBSTIMER_TIMER, false);
}

MainWindow::~MainWindow()
{
	delete m_backgroundJobsTimer;
	m_backgroundJobsTimer = nullptr;

	delete m_taskbar;
	m_taskbar = nullptr;
}

void MainWindow::configureOptions(bool modeless)
{
	OptionsWizard wizard(modeless ? this : NULL, getText(_S("ui.wizard_options.title")));
	
	wizard.run();
}

void MainWindow::showMode(bool hide)
{
	bool trayUse = Options::instance()->getOption<bool>(Options::ui_options::tray_minimize);
	bool trayAlways = Options::instance()->getOption<bool>(Options::ui_options::tray_always);
	bool trayBalloon = Options::instance()->getOption<bool>(Options::ui_options::tray_balloon);		

	if(hide)
	{		
		if(trayUse)
		{
			Show(false);
			m_taskbar->show(getTrayIcon());
		}
		else
		{
			if(IsIconized() == false)
				Iconize();
			m_taskbar->hide();
		}
	}	
	else
	{
		Show(true);

		if(IsIconized())
			Iconize(false);

		Raise();

		if(trayUse)
		{			
			if(trayAlways)
				m_taskbar->show(getTrayIcon());
			else
				m_taskbar->hide();
		}
		else
			m_taskbar->hide();
	}
}

/*
void MainWindow::hideInTaskbar(bool hide)
{
	if(m_hiddenInTaskbar == hide)
		return;

	m_hiddenInTaskbar = hide;
	if(hide)
	{
		Show(false);
		m_taskbar->show(getTrayIcon());
	}
	else
	{
		bool trayAlways = Options::instance()->getOption<bool>(Options::ui_options::tray_always);

		bool balloon = Options::instance()->getOption<bool>(Options::ui_options::tray_balloon);
		if(balloon)
			trayAlways = true;

		if(trayAlways == false)
			m_taskbar->hide();

		Show(true);

		if(IsIconized())
			Iconize(false);

		Raise();
	}
}
*/

void MainWindow::init(bool startBackground, bool restorePos)
{
	Options::instance()->ensureOption<String>(OS_UI_OPTION_MAIN_WINDOW_POSITION, String::EMPTY, false);
	Options::instance()->ensureOption<uint32>(OS_UI_OPTION_MAIN_WINDOW_FLAGS, 0, false);
	Options::instance()->subscribeUpdateHandler(boost::bind(&MainWindow::onUpdateOptions, this));

	createLayout();

	initMenu();
	initWindow();
	synchronizeCommands();

	SetTitle(conversions::from_utf16<wxString>(Engine::getVersionName(true)));

	Application::instance()->SetTopWindow(this);

	if((restorePos == false) || (restorePosition() == false))
		centerWindow(this);

	if(startBackground)
		showMode(true);
	else
		showMode(false);

	m_initialized = true;
}

void MainWindow::logMessages(const list<shared_ptr<LogMessage> >::type &logs)
{
	if(m_logWindow != nullptr)
		m_logWindow->logMessages(logs);

	if(m_downloading == false && logs.empty() == false)
		setStatusText(logs.back()->getMessage());
}

void MainWindow::setStatusText(const String &status)
{
	GetStatusBar()->SetStatusText(conversions::from_utf16<wxString>(status));

#if OS_PLATFORM == OS_PLATFORM_WIN
	bool balloon = Options::instance()->getOption<bool>(Options::ui_options::tray_balloon);
	if(balloon)
	{
		m_taskbar->show(getTrayIcon());
		m_taskbar->ShowBalloon(conversions::from_utf16<wxString>(_S("Osiris")), conversions::from_utf16<wxString>(status), 4000, 0);
	}
#endif
}

void MainWindow::createLayout()
{
	this->SetSizeHints( wxSize( 400,200 ), wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bordersSizer;
	bordersSizer = new wxBoxSizer( wxVERTICAL );


	bordersSizer->Add( 0, 5, 0, 0, 0 );

	wxBoxSizer* menuPanelSizer;
	menuPanelSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );


	bSizer27->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonHome = new wxSkinButton( this, cmdViewHome, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	bSizer27->Add( m_buttonHome, 0, wxALIGN_RIGHT|wxALL, 0 );


	bSizer27->Add( 5, 0, 0, wxEXPAND, 0 );

	menuPanelSizer->Add( bSizer27, 1, wxEXPAND, 5 );

	bordersSizer->Add( menuPanelSizer, 0, wxALL|wxEXPAND, 0 );


	bordersSizer->Add( 0, 4, 0, 0, 0 );

	wxBoxSizer* commandsPanelSizer;
	commandsPanelSizer = new wxBoxSizer( wxHORIZONTAL );


	commandsPanelSizer->Add( 0, 0, 1, wxEXPAND, 0 );

	m_buttonP2PEnable = new ToggleBitmapButton( this, cmdToggleP2PEnable, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsPanelSizer->Add( m_buttonP2PEnable, 0, wxALL, 0 );


	commandsPanelSizer->Add( 2, 0, 0, wxEXPAND, 0 );

	m_buttonIsisEnable = new ToggleBitmapButton( this, cmdToggleIsisEnable, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsPanelSizer->Add( m_buttonIsisEnable, 0, wxALL, 0 );


	commandsPanelSizer->Add( 2, 0, 0, wxEXPAND, 0 );

	m_buttonWizard = new wxSkinButton( this, cmdConfigureOptions, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsPanelSizer->Add( m_buttonWizard, 0, wxALL, 0 );


	commandsPanelSizer->Add( 5, 0, 0, wxEXPAND, 0 );

	bordersSizer->Add( commandsPanelSizer, 0, wxEXPAND, 0 );


	bordersSizer->Add( 0, 6, 0, wxEXPAND, 0 );

	wxBoxSizer* contentPanelSizer;
	contentPanelSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pages = new wxAuiNotebook( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_MOVE|wxAUI_NB_WINDOWLIST_BUTTON|wxNO_BORDER );
	m_p2pConnectionsWindow = new P2PConnectionsWindow( m_pages, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
	m_pages->AddPage( m_p2pConnectionsWindow, wxT("Connections"), true, wxNullBitmap );
	m_isisConnectionsWindow = new IsisConnectionsWindow( m_pages, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
	m_pages->AddPage( m_isisConnectionsWindow, wxT("Isis"), false, wxNullBitmap );
	m_logWindow = new LogWindow( m_pages, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
	m_pages->AddPage( m_logWindow, wxT("Log"), false, wxNullBitmap );
	m_statisticsWindow = new StatisticsWindow( m_pages, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
	m_pages->AddPage( m_statisticsWindow, wxT("Statistics"), false, wxNullBitmap );
	m_ircWindow = new IRCWindow( m_pages, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
	m_pages->AddPage( m_ircWindow, wxT("Chat"), false, wxNullBitmap );

	contentPanelSizer->Add( m_pages, 1, wxEXPAND | wxALL, 0 );

	bordersSizer->Add( contentPanelSizer, 1, wxEXPAND, 0 );

	mainSizer->Add( bordersSizer, 1, wxALL|wxEXPAND, 0 );

	this->SetSizer( mainSizer );
	this->Layout();
}

void MainWindow::initMenu()
{
	wxMenuBar *menuBar = new wxMenuBar();

	wxMenu *menuFile = new wxMenu();
	wxMenuItem *menuItem;

	menuItem = new wxMenuItem(menuFile, cmdViewHome, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.enter"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("menu_enter.png"))), wxBITMAP_TYPE_PNG));
	menuFile->Append(menuItem);

	menuItem = new wxMenuItem(menuFile, cmdConfigureOptions, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.options_wizard"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("menu_wizard.png"))), wxBITMAP_TYPE_PNG));
	menuFile->Append(menuItem);

	menuFile->AppendSeparator();

#ifdef OS_DEBUG
	menuFile->Append(cmdExecuteCommand, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.execute_command"))));
#endif // OS_DEBUG

	menuItem = new wxMenuItem(menuFile, cmdImportFile, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.import_file"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("import_file.png"))), wxBITMAP_TYPE_PNG));
	menuFile->Append(menuItem);

	menuItem = new wxMenuItem(menuFile, cmdPasteLink, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.paste_link"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("paste_link.png"))), wxBITMAP_TYPE_PNG));
	menuFile->Append(menuItem);

	wxMenu *toolsMenu = new wxMenu();

	menuItem = new wxMenuItem(menuFile, cmdOpenDataFolder, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.data_folder"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("menu_datafolder.png"))), wxBITMAP_TYPE_PNG));
	toolsMenu->Append(menuItem);

	/* 	
	menuItem = new wxMenuItem(toolsMenu, cmdSetupFilesAssociations, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.tools.setup_files_associations"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("setup_files_associations.png"))), wxBITMAP_TYPE_PNG));
	toolsMenu->Append(menuItem);

#if (OS_PLATFORM == OS_PLATFORM_WIN) || (OS_PLATFORM == OS_PLATFORM_MAC)
	menuItem = new wxMenuItem(toolsMenu, cmdSetupRegistryProtocol, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.tools.setup_registry_protocol"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("setup_registry_protocol.png"))), wxBITMAP_TYPE_PNG));
	toolsMenu->Append(menuItem);
#endif
	*/

	toolsMenu->AppendSeparator();

	wxMenu *develMenu = new wxMenu();
	toolsMenu->Append(wxID_ANY, conversions::from_utf16<wxString>(getText("ui.mainframe.mainmenu.tools.devel")), develMenu);

	menuItem = new wxMenuItem(develMenu, cmdDevelCreateExtension, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.tools.devel.create_extension"))));
	develMenu->Append(menuItem);

	develMenu->AppendSeparator();

	menuItem = new wxMenuItem(develMenu, cmdDevelReloadExtensions, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.tools.devel.reload_extensions"))));
	develMenu->Append(menuItem);

	develMenu->AppendSeparator();

	menuItem = new wxMenuItem(develMenu, cmdDevelOpenLog, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.tools.devel.open_log"))));
	develMenu->Append(menuItem);

	wxMenu *helpMenu = new wxMenu();

	menuItem = new wxMenuItem(helpMenu, cmdGotoHomePage, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.help.homepage"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("menu_homepage.png"))), wxBITMAP_TYPE_PNG));
	helpMenu->Append(menuItem);

	menuItem = new wxMenuItem(helpMenu, cmdGotoSupportPage, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.help.support"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("menu_support.png"))), wxBITMAP_TYPE_PNG));
	helpMenu->Append(menuItem);

	helpMenu->AppendSeparator();

	menuItem = new wxMenuItem(helpMenu, cmdAbout, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.help.about"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("menu_about.png"))), wxBITMAP_TYPE_PNG));
	helpMenu->Append(menuItem);

	menuFile->AppendSeparator();

	menuItem = new wxMenuItem(menuFile, cmdExit, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.exit"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("menu_exit.png"))), wxBITMAP_TYPE_PNG));
	menuFile->Append(menuItem);

    String helpMenuTitle = getText(_S("ui.mainframe.mainmenu.help"));

#if OS_PLATFORM	== OS_PLATFORM_MAC
	wxApp::s_macHelpMenuTitleName = conversions::from_utf16<wxString>(helpMenuTitle);
#endif

	menuBar->Append(menuFile, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.file"))));
	menuBar->Append(toolsMenu, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.tools"))));
	menuBar->Append(helpMenu, conversions::from_utf16<wxString>(helpMenuTitle));

	SetMenuBar(menuBar);
}

void MainWindow::initWindow()
{
	// Sotto Windows sembra che supporta tranquillamente bitmap 32bit, mentre sotto Linux sembra che supporta solo bitmap a 8bit (forse con assert di mancanza maschera)
#if OS_PLATFORM	== OS_PLATFORM_WIN
	m_iconNormal.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris.ico"))), wxBITMAP_TYPE_ICO);
	m_iconBusy.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris_busy.ico"))), wxBITMAP_TYPE_ICO);
	m_trayIconNormal.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris_tray.ico"))), wxBITMAP_TYPE_ICO);
	m_trayIconBusy.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris_tray_busy.ico"))), wxBITMAP_TYPE_ICO);
#elif OS_PLATFORM == OS_PLATFORM_MAC // TOTEST
	m_iconNormal.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris.icns"))), wxBITMAP_TYPE_ICO);
	m_iconBusy.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris_busy.icns"))), wxBITMAP_TYPE_ICO);
	m_trayIconNormal.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris_tray.icns"))), wxBITMAP_TYPE_ICO);
	m_trayIconBusy.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris_tray_busy.icns"))), wxBITMAP_TYPE_ICO);
#else
	m_iconNormal.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris.png"))), wxBITMAP_TYPE_PNG);
	m_iconBusy.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris_busy.png"))), wxBITMAP_TYPE_PNG);
	m_trayIconNormal.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris_tray.png"))), wxBITMAP_TYPE_PNG);
	m_trayIconBusy.LoadFile(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("osiris_tray_busy.png"))), wxBITMAP_TYPE_PNG);
#endif

	getRenderer()->setTopLeftImage(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_bg_top_left.png"))), wxBITMAP_TYPE_PNG));
	getRenderer()->setTopImage(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_bg_top_center.png"))), wxBITMAP_TYPE_PNG));
	getRenderer()->setTopRightImage(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_bg_top_right.png"))), wxBITMAP_TYPE_PNG));

	m_buttonHome->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_enter_normal.png"))), wxBITMAP_TYPE_PNG), false);
	m_buttonHome->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_enter_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonHome->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_enter_over.png"))), wxBITMAP_TYPE_PNG));

	m_buttonWizard->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_wizard_normal.png"))), wxBITMAP_TYPE_PNG), false);
	m_buttonWizard->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_wizard_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonWizard->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_wizard_over.png"))), wxBITMAP_TYPE_PNG));

	m_buttonP2PEnable->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_p2p_normal_off.png"))), wxBITMAP_TYPE_PNG), false);
	m_buttonP2PEnable->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_p2p_down_off.png"))), wxBITMAP_TYPE_PNG));
	m_buttonP2PEnable->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_p2p_over_off.png"))), wxBITMAP_TYPE_PNG));
	m_buttonP2PEnable->setImageNormalChecked(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_p2p_normal_on.png"))), wxBITMAP_TYPE_PNG), false);
	m_buttonP2PEnable->setImageDownChecked(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_p2p_down_on.png"))), wxBITMAP_TYPE_PNG));
	m_buttonP2PEnable->setImageOverChecked(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_p2p_over_on.png"))), wxBITMAP_TYPE_PNG));

	m_buttonIsisEnable->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_isis_normal_off.png"))), wxBITMAP_TYPE_PNG), false);
	m_buttonIsisEnable->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_isis_down_off.png"))), wxBITMAP_TYPE_PNG));
	m_buttonIsisEnable->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_isis_over_off.png"))), wxBITMAP_TYPE_PNG));
	m_buttonIsisEnable->setImageNormalChecked(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_isis_normal_on.png"))), wxBITMAP_TYPE_PNG), false);
	m_buttonIsisEnable->setImageDownChecked(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_isis_down_on.png"))), wxBITMAP_TYPE_PNG));
	m_buttonIsisEnable->setImageOverChecked(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_btn_isis_over_on.png"))), wxBITMAP_TYPE_PNG));

	OS_ASSERT(m_pages != nullptr);
	m_pages->SetSelection(0);
	m_pages->SetPageBitmap(0, wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_p2p_connections.png"))), wxBITMAP_TYPE_PNG));
	m_pages->SetPageBitmap(1, wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_isis_connections.png"))), wxBITMAP_TYPE_PNG));
	m_pages->SetPageBitmap(2, wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_logs.png"))), wxBITMAP_TYPE_PNG));
	m_pages->SetPageBitmap(3, wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_statistics.png"))), wxBITMAP_TYPE_PNG));
	m_pages->SetPageBitmap(4, wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_irc.png"))), wxBITMAP_TYPE_PNG));

	OS_ASSERT(m_gauge != nullptr);
	m_gauge->Show(false);

	m_buttonHome->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.enter.tooltip"))));
	m_buttonWizard->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.wizard.tooltip"))));
	m_buttonP2PEnable->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.p2p_enable.tooltip"))));
	m_buttonIsisEnable->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.isis_enable.tooltip"))));

	m_pages->SetPageText(0, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.tab.connections"))));
	m_pages->SetPageText(1, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.tab.isis"))));
	m_pages->SetPageText(2, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.tab.logs"))));
	m_pages->SetPageText(3, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.tab.statistics"))));

	updateBackgroundJobsStatus();
}

void MainWindow::synchronizeCommands()
{
	OS_ASSERT(m_buttonP2PEnable != nullptr);
	m_buttonP2PEnable->setChecked(Options::instance()->getOption<bool>(Options::p2p_options::enable));

	OS_ASSERT(m_buttonIsisEnable != nullptr);
	m_buttonIsisEnable->setChecked(Options::instance()->getOption<bool>(Options::isis_options::enable));
}

void MainWindow::resizeWindow()
{
	if(m_gauge == nullptr)
		return;

	wxRect rect;
	if(GetStatusBar()->GetFieldsCount() > 1 && GetStatusBar()->GetFieldRect(1, rect))
	{
		rect.Deflate(1, 0);
		m_gauge->SetSize(rect);
	}
}

void MainWindow::resetDownloadStatus()
{
	m_downloading = false;
	setStatusText(String::EMPTY);

	m_gauge->Show(false);
	GetStatusBar()->SetFieldsCount(1);
	GetStatusBar()->SetStatusText(wxEmptyString);
}

void MainWindow::updateBackgroundJobsStatus()
{
	SetIcon(getWindowIcon());
	m_taskbar->update(getTrayIcon());
}

void MainWindow::savePosition()
{
	/*
	String position;
	if(IsVisible())
	{
		wxRect rect = GetScreenRect();
		position = String::format(_S("%d,%d,%d,%d").c_str(), rect.x, rect.y, rect.width, rect.height);
		OS_LOG_DEBUG(_S("Saving window position to ") + position);
	}

	Options::instance()->setOption(OS_UI_OPTION_MAIN_WINDOW_POSITION, position);

	uint32 flags = 0;

	if(IsMaximized())
		flags |= OS_UI_OPTION_MAIN_WINDOW_FLAGS_MAXIMIZED;

	if(IsIconized())
		flags |= OS_UI_OPTION_MAIN_WINDOW_FLAGS_ICONIZED;

	Options::instance()->setOption(OS_UI_OPTION_MAIN_WINDOW_FLAGS, flags);
	*/

	uint32 flags = 0;
	if(IsMaximized())
		flags |= OS_UI_OPTION_MAIN_WINDOW_FLAGS_MAXIMIZED;
	else if(IsIconized())
		flags |= OS_UI_OPTION_MAIN_WINDOW_FLAGS_ICONIZED;
	else
	{
		String position;
		if(IsVisible())
		{
			wxRect rect = GetScreenRect();
			position = String::format(_S("%d,%d,%d,%d").c_str(), rect.x, rect.y, rect.width, rect.height);
			//OS_LOG_DEBUG(_S("Saving window position to ") + position);
		}

		Options::instance()->setOption(OS_UI_OPTION_MAIN_WINDOW_POSITION, position);
	}

	Options::instance()->setOption(OS_UI_OPTION_MAIN_WINDOW_FLAGS, flags);
}

bool MainWindow::restorePosition()
{
	/*
	OS_LOG_DEBUG("Reading window position options...");

	std::string position = Options::instance()->getOption<String>(OS_UI_OPTION_MAIN_WINDOW_POSITION).to_ascii();
	if(position.empty())
	{
		OS_LOG_DEBUG("Restoring window position skipped (no option)");
		return false;
	}

	wxRect rect;
	if(sscanf(position.c_str(), "%d,%d,%d,%d", &rect.x, &rect.y, &rect.width, &rect.height) != 4)
	{
		OS_LOG_DEBUG("Restoring window position skipped (invalid option value)");
		return false;
	}

	wxSize displaySize;
	wxDisplaySize(&displaySize.x, &displaySize.y);

	if(rect.width > displaySize.x)
		rect.width = displaySize.x;

	if(rect.height > displaySize.y)
		rect.height = displaySize.y;

	if((rect.x + rect.width) < 0)
		rect.x = 0;

	if((rect.y + rect.height) < 0)
		rect.y = 0;

	OS_LOG_DEBUG(String::format(_S("Restoring window position to %d,%d,%d,%d").c_str(), rect.x, rect.y, rect.width, rect.height));

	SetSize(rect);
	
	uint32 flags = Options::instance()->getOption<uint32>(OS_UI_OPTION_MAIN_WINDOW_FLAGS);

	if((flags & OS_UI_OPTION_MAIN_WINDOW_FLAGS_MAXIMIZED) == OS_UI_OPTION_MAIN_WINDOW_FLAGS_MAXIMIZED)
		Maximize();

	if((flags & OS_UI_OPTION_MAIN_WINDOW_FLAGS_ICONIZED) == OS_UI_OPTION_MAIN_WINDOW_FLAGS_ICONIZED)
		Iconize();
	*/

	uint32 flags = Options::instance()->getOption<uint32>(OS_UI_OPTION_MAIN_WINDOW_FLAGS);

	if((flags & OS_UI_OPTION_MAIN_WINDOW_FLAGS_MAXIMIZED) == OS_UI_OPTION_MAIN_WINDOW_FLAGS_MAXIMIZED)
	{
		// 0.15 : The first time this maybe called without windows visible. WX wrong something about sending onsize events, and a black border on the right are visible.
		// This to avoid that.
		Show(true);

		Maximize();
	}
	else if((flags & OS_UI_OPTION_MAIN_WINDOW_FLAGS_ICONIZED) == OS_UI_OPTION_MAIN_WINDOW_FLAGS_ICONIZED)
		Iconize();
	else
	{
		//OS_LOG_DEBUG("Reading window position options...");

		std::string position = Options::instance()->getOption<String>(OS_UI_OPTION_MAIN_WINDOW_POSITION).to_ascii();
		if(position.empty())
		{
			//OS_LOG_DEBUG("Restoring window position skipped (no option)");
			return false;
		}

		wxRect rect;
		if(sscanf(position.c_str(), "%d,%d,%d,%d", &rect.x, &rect.y, &rect.width, &rect.height) != 4)
		{
			//OS_LOG_DEBUG("Restoring window position skipped (invalid option value)");
			return false;
		}

		wxSize displaySize;
		wxDisplaySize(&displaySize.x, &displaySize.y);

		if(rect.width > displaySize.x)
			rect.width = displaySize.x;

		if(rect.height > displaySize.y)
			rect.height = displaySize.y;

		if((rect.x + rect.width) < 0)
			rect.x = 0;

		if((rect.y + rect.height) < 0)
			rect.y = 0;

		//OS_LOG_DEBUG(String::format(_S("Restoring window position to %d,%d,%d,%d").c_str(), rect.x, rect.y, rect.width, rect.height));

		SetSize(rect);
	}

	return true;
}

void MainWindow::onDownloadStarting(DownloadEvent &e)
{
	setStatusText(getText(_S("ui.mainframe.updating.progress")));
	m_downloading = true;

	int widths[] = {-1, 150};
	GetStatusBar()->SetFieldsCount(2, widths);

	resizeWindow();

	m_gauge->SetRange(e.getFilesize());
	m_gauge->Show(true);
}

void MainWindow::onDownloadCompleted(DownloadEvent &e)
{
	resetDownloadStatus();

	Application::instance()->shell(e.getFilename());

	Close(true);
}

void MainWindow::onDownloadFailed(DownloadEvent &e)
{
	resetDownloadStatus();

	OS_LOG_ERROR(getText(_S("ui.mainframe.updating.error")));
}

void MainWindow::onDownloadProgress(DownloadEvent &e)
{
	m_gauge->SetValue(e.getDownloadedBytes());
}

void MainWindow::onUpdateOptions()
{
	synchronizeCommands();
}

void MainWindow::onAbout(wxCommandEvent &e)
{
    Application::instance()->openLocalUrl(_S("main/about"));
}

void MainWindow::onClose(wxCloseEvent &e)
{
	if(m_closingInProgress)
		return;

	if(m_download != nullptr && m_download->running())
		return;

	m_closingInProgress = true;

	if(e.CanVeto())
	{
		bool ask = Options::instance()->getOption<bool>(Options::ui_options::exit_confirm);
		if(ask)
		{
#if wxCHECK_VERSION(2, 9, 2)
			wxRichMessageDialog dlg(this, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.exit.confirm"))), conversions::from_utf16<wxString>(Engine::getVersionName(true)), wxYES_NO);
			dlg.ShowCheckBox(conversions::from_utf16<wxString>(getText(_S("common.dontaskagain"))));
			bool dontClose = (dlg.ShowModal() == wxID_NO);
			if ( dlg.IsCheckBoxChecked() )
			{
				Options::instance()->setOption(Options::ui_options::exit_confirm, false);
			}
#else
			bool dontClose = (wxMessageBox(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.exit.confirm"))), conversions::from_utf16<wxString>(Engine::getVersionName(true)), wxYES_NO, this) == wxNO);
#endif

			if(dontClose)
			{
                e.Veto();
				m_closingInProgress = false;
				return;
			}
		}
	}

    {
        wxBusyCursor busyCursor;

		savePosition();
		Application::instance()->stop();
    }

	e.Skip();
}

void MainWindow::onConfigureOptions(wxCommandEvent &e)
{
    configureOptions(true);
}

void MainWindow::onDevelCreateExtension(wxCommandEvent &e)
{
	ExtensionsWizard wizard(this, getText("ui.wizard_extensions.title"));
	wizard.run();
}

void MainWindow::onDevelReloadExtensions(wxCommandEvent &e)
{
	Application::instance()->reloadExtensions();
}

void MainWindow::onDevelOpenLog(wxCommandEvent &e)
{
	String logPath = Options::instance()->getLogPath();
	String filename = utils::makeFilePath(logPath, OS_LOG_FILENAME);
	
	bool enable = Options::instance()->getOption<bool>(Options::log_options::enable);
	bool serialize = Options::instance()->getOption<bool>(Options::log_options::serialize);	

	if( (enable == false) || (serialize == false) )
	{
		wxMessageBox(conversions::from_utf16<wxString>(getText("ui.mainframe.log.open.not_enabled")), conversions::from_utf16<wxString>(Engine::getVersionName(true)), wxOK | wxICON_ERROR, this);
		return;
	}

	if(FileSystem::instance()->exists(filename) == false)
	{
		wxMessageBox(conversions::from_utf16<wxString>(getText("ui.mainframe.log.open.not_exists")), conversions::from_utf16<wxString>(Engine::getVersionName(true)), wxOK | wxICON_ERROR, this);
		return;
	}
	
	Application::instance()->shell(filename);
}

void MainWindow::onDownloadNotify(DownloadEvent &e)
{
	switch(e.getStatus())
	{
	case DownloadEvent::dsStarting:		onDownloadStarting(e);
										break;

	case DownloadEvent::dsCompleted:	onDownloadCompleted(e);
										break;

	case DownloadEvent::dsFailed:		onDownloadFailed(e);
										break;

	case DownloadEvent::dsDownloading:	onDownloadProgress(e);
										break;
	}
}

void MainWindow::onIconize(wxIconizeEvent &e)
{
	if(e.Iconized())
	{
		showMode(true);
		//hideInTaskbar(true);
	}
}

void MainWindow::onExecuteCommand(wxCommandEvent &e)
{
	Engine::instance()->executeCommand(conversions::to_utf16(wxGetTextFromUser(conversions::from_utf16<wxString>(_S("Enter command")))));
}

void MainWindow::onPasteLink(wxCommandEvent &e)
{
	wxTextEntryDialog dialog(this, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.mainmenu.paste_link"))));
	if(dialog.ShowModal() != wxID_OK)
		return;

	OsirisLink link;
	if(link.parse(conversions::to_utf16(dialog.GetValue()).to_ascii()))
		Application::instance()->processLink(link, true);
}

void MainWindow::onImportFile(wxCommandEvent &e)
{
	wxFileDialog fileDialog(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxT("Osiris files (*.osiris)|*.osiris|All files (*.*)|*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(fileDialog.ShowModal() != wxID_OK)
		return;

	Application::instance()->processFile(conversions::to_utf16(fileDialog.GetPath()), true);
}

void MainWindow::onExit(wxCommandEvent &e)
{
	Close(true);
}

void MainWindow::onP2PEnableChanged(wxCommandEvent &e)
{
	wxBusyCursor busyCursor;

	Options::instance()->setOption(Options::p2p_options::enable, m_buttonP2PEnable->getChecked());
	Options::instance()->update();
}

void MainWindow::onIsisEnableChanged(wxCommandEvent &e)
{
	wxBusyCursor busyCursor;

	Options::instance()->setOption(Options::isis_options::enable, m_buttonIsisEnable->getChecked());
	Options::instance()->update();
}

void MainWindow::onViewHome(wxCommandEvent &e)
{
	Application::instance()->viewHome();
}

void MainWindow::onOpenDataFolder(wxCommandEvent &e)
{
	Application::instance()->shell(Options::instance()->getDataPath());
}

void MainWindow::onGotoHomePage(wxCommandEvent &e)
{
	Application::instance()->openUrl(Options::instance()->getIsisLink(OS_ISIS_PAGE_LINK, "id=home"));
}

void MainWindow::onGotoSupportPage(wxCommandEvent &e)
{
	Application::instance()->openUrl(Options::instance()->getIsisLink(OS_ISIS_PAGE_LINK, "id=donations"));
}

void MainWindow::onIconizeWindow(wxCommandEvent &e)
{
	Iconize(true);
}

void MainWindow::onMaximizeWindow(wxCommandEvent &e)
{
	//Maximize(IsMaximized() == false);
	Maximize(true);	
}

void MainWindow::onCloseWindow(wxCommandEvent &e)
{
	Close(false);
}

/*
void MainWindow::onSetupFilesAssociations(wxCommandEvent &e)
{
	Application::instance()->setupFilesAssociations(true, true);
}

void MainWindow::onSetupRegistryProtocol(wxCommandEvent &e)
{
    Application::instance()->setupProtocolAssociations(true, true);
}
*/

void MainWindow::onSize(wxSizeEvent &e)
{
	resizeWindow();

	e.Skip();
}

void MainWindow::onBackgroundJobsTimer(wxTimerEvent &e)
{
	bool hasBackgroundJobs = false;
	if(Engine::exists())
		hasBackgroundJobs = Engine::instance()->hasBackgroundJobs();

	if(hasBackgroundJobs != m_hasBackgroundJobs)
	{
		m_hasBackgroundJobs = hasBackgroundJobs;
		updateBackgroundJobsStatus();
	}

	e.Skip();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
