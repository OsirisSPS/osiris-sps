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
#include "logwindow.h"

#include "application.h"
#include "boost/bind.hpp"
#include "conversions.h"
#include "engine.h"
#include "listctrlrenderer.h"
#include "logmessage.h"
#include "options.h"
#include "skinnedwindow.h"
#include "uistandards.h"
#include "wx/clipbrd.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static const uint32 cmdClearLog = wxNewId();
static const uint32 cmdLogSettings = wxNewId();
static const uint32 cmdEnableLog = wxNewId();
static const uint32 cmdEnableFileLog = wxNewId();
static const uint32 cmdEnableBalloon = wxNewId();
static const uint32 cmdCopyLogMessage = wxNewId();
static const uint32 cmdChangeLogLevel = wxNewId();		// Da lasciare come ultimo comando

//////////////////////////////////////////////////////////////////////

OS_STATIC_ASSERT(static_cast<uint32>(logLevelDebug) == 0);

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(LogWindow, WindowBase)
	EVT_BUTTON(cmdClearLog, LogWindow::onClearLog)
	EVT_BUTTON(cmdLogSettings, LogWindow::onLogSettings)
	EVT_MENU(cmdEnableLog, LogWindow::onEnableLog)
	EVT_MENU(cmdEnableFileLog, LogWindow::onEnableFileLog)
	EVT_MENU(cmdEnableBalloon, LogWindow::onEnableBalloon)
	EVT_MENU(cmdCopyLogMessage, LogWindow::onCopyLogMessage)
	EVT_MENU_RANGE(cmdChangeLogLevel, cmdChangeLogLevel + static_cast<uint32>(logLevelCritical), LogWindow::onChangeLogLevel)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

LogWindow::LogWindow(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style) : WindowBase(parent, id, pos, size, style),
																											m_logCtrl(null),
																											m_buttonClearLog(null),
																											m_imageList(16, 16),
																											m_enableAutoscoll(true)
{
	createLayout();
	initWindow();
}

LogWindow::~LogWindow()
{
	cleanupWindow();
}

void LogWindow::createLayout()
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_logCtrl = new SkinnedWindow<wxListCtrl, ListCtrlRenderer>( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER|wxLC_REPORT );
	mainSizer->Add( m_logCtrl, 1, wxEXPAND, 0 );
	
	m_commandsPanel = new SkinnedWindow<wxPanel>( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxSize( 25,-1 ), wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
	wxBoxSizer* commandsSizer;
	commandsSizer = new wxBoxSizer( wxVERTICAL );
	
	m_buttonClearLog = new wxSkinButton( m_commandsPanel, cmdClearLog, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsSizer->Add( m_buttonClearLog, 0, wxALL, 2 );
	
	m_buttonLogSettings = new wxSkinButton( m_commandsPanel, cmdLogSettings, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0|wxNO_BORDER );
	commandsSizer->Add( m_buttonLogSettings, 0, wxALL, 2 );
	
	m_commandsPanel->SetSizer( commandsSizer );
	m_commandsPanel->Layout();
	mainSizer->Add( m_commandsPanel, 0, wxEXPAND, 0 );
	
	this->SetSizer( mainSizer );
	this->Layout();
}

void LogWindow::initWindow()
{
	OS_ASSERT(m_commandsPanel != null);
	m_commandsPanel->getRenderer()->setBackgroundImage(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("window_commands_background.png"))), wxBITMAP_TYPE_PNG));

	m_iconsMap[OS_NAMESPACE_NAME::logLevelDebug] = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_logs_debug.png"))), wxBITMAP_TYPE_PNG));
	m_iconsMap[OS_NAMESPACE_NAME::logLevelNotice] = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_logs_notice.png"))), wxBITMAP_TYPE_PNG));
	m_iconsMap[OS_NAMESPACE_NAME::logLevelInfo] = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_logs_info.png"))), wxBITMAP_TYPE_PNG));
	m_iconsMap[OS_NAMESPACE_NAME::logLevelWarning] = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_logs_warning.png"))), wxBITMAP_TYPE_PNG));
	m_iconsMap[OS_NAMESPACE_NAME::logLevelError] = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_logs_error.png"))), wxBITMAP_TYPE_PNG));
	m_iconsMap[OS_NAMESPACE_NAME::logLevelCritical] = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_logs_critical.png"))), wxBITMAP_TYPE_PNG));

	OS_ASSERT(m_logCtrl != null);
	m_logCtrl->InsertColumn(columnTimestamp, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.log.timestamp"))), wxLIST_FORMAT_LEFT, 120);
	m_logCtrl->InsertColumn(columnMessage, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.log.message"))), wxLIST_FORMAT_LEFT, 260);
	m_logCtrl->SetImageList(&m_imageList, wxIMAGE_LIST_SMALL);
	m_logCtrl->Connect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler(LogWindow::onLogMessageRightClick), NULL, this);

	m_buttonClearLog->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("log_btn_clear_normal.png"))), wxBITMAP_TYPE_PNG));
	m_buttonClearLog->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("log_btn_clear_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonClearLog->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("log_btn_clear_over.png"))), wxBITMAP_TYPE_PNG));
	m_buttonClearLog->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.log.clear.tooltip"))));

	m_buttonLogSettings->setImageNormal(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("log_btn_log_settings_normal.png"))), wxBITMAP_TYPE_PNG));
	m_buttonLogSettings->setImageDown(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("log_btn_log_settings_down.png"))), wxBITMAP_TYPE_PNG));
	m_buttonLogSettings->setImageOver(wxImage(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("log_btn_log_settings_over.png"))), wxBITMAP_TYPE_PNG));
	m_buttonLogSettings->SetToolTip(conversions::from_utf16<wxString>(getText(_S("ui.mainframe.log.settings.tooltip"))));	
}

void LogWindow::cleanupWindow()
{
	m_logCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler(LogWindow::onLogMessageRightClick), NULL, this);
}

void LogWindow::logMessage(shared_ptr<LogMessage> message)
{
	if(message == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	if(Engine::exists() == false) // 0.15
		return;

	int32 index = m_logCtrl->InsertItem(m_logCtrl->GetItemCount(), wxEmptyString);
	if(index != -1)
	{
		uint32 icon = 0;

		IconsMap::const_iterator i = m_iconsMap.find(message->getLevel());
		if(i == m_iconsMap.end())
		{
			OS_ASSERT(m_iconsMap.find(OS_NAMESPACE_NAME::logLevelInfo) != m_iconsMap.end());
			icon = m_iconsMap[OS_NAMESPACE_NAME::logLevelInfo];
		}
		else
		{
			icon = i->second;
		}

		m_logCtrl->SetItemImage(index, icon);
		m_logCtrl->SetItem(index, columnTimestamp, conversions::from_utf16<wxString>(Engine::instance()->formatSystemDate(message->getTimestamp())));

		String messageSingleLine = message->getMessage(); // 0.14 RC2, single-line stuffs.
		messageSingleLine.replace_all(_S("\n\r"),_S("\n"));
		messageSingleLine.replace_all(_S("\r\n"),_S("\n"));
		messageSingleLine.replace_all(_S("\n"),_S(" | "));
		messageSingleLine.replace_all(_S("\r"),_S(" | "));
		messageSingleLine.replace_all(_S("\t"),_S("  "));

		m_logCtrl->SetItem(index, columnMessage, conversions::from_utf16<wxString>(messageSingleLine));
		
		bool ensureVisible = false;

		long firstVisible = m_logCtrl->GetTopItem();
		long lastVisble = 0;
		long countPerPage = 0;
		if(firstVisible != -1)
		{
			countPerPage = m_logCtrl->GetCountPerPage();
			lastVisble = firstVisible + countPerPage;
			ensureVisible = (index >= firstVisible) && (index <= lastVisble);
		}
		
		if(ensureVisible == false)
			String();

		if(m_enableAutoscoll && ensureVisible)
			m_logCtrl->EnsureVisible(index);		
	}

	while(m_logCtrl->GetItemCount() > OS_UI_MAX_LOGS_MESSAGES)
		m_logCtrl->DeleteItem(0);
}

void LogWindow::logMessages(const list<shared_ptr<LogMessage> >::type &logs)
{
	wxWindowUpdateLocker windowLocker(this);

	for(list<shared_ptr<LogMessage> >::type::const_iterator i = logs.begin(); i != logs.end(); ++i)
	{
		logMessage(*i);
	}
}

void LogWindow::clearLogs()
{
	if(m_logCtrl != null)
		m_logCtrl->DeleteAllItems();
}

void LogWindow::onClearLog(wxCommandEvent &e)
{
	clearLogs();
}

void LogWindow::onLogSettings(wxCommandEvent &e)
{
	bool enable = Options::instance()->getOption<bool>(Options::log_options::enable);
	bool serialize = Options::instance()->getOption<bool>(Options::log_options::serialize);	
	uint32 level = Options::instance()->getOption<uint32>(Options::log_options::level);

	bool balloon = Options::instance()->getOption<bool>(Options::ui_options::tray_balloon);

	scoped_ptr<wxMenu> menu(new wxMenu());
	
	wxMenuItem *item;

	// 0.15 : Note the sequence: SetCheckable must be set BEFORE the item is append to menù, otherwise don't work under Linux.

	item =  new wxMenuItem(menu.get(), cmdEnableLog, conversions::from_utf16<wxString>(getText("ui.mainframe.log.enable")));	
	item->SetCheckable(true);
	menu->Append(item);
	item->Check(enable);
	

	item =  new wxMenuItem(menu.get(), cmdEnableFileLog, conversions::from_utf16<wxString>(getText("ui.mainframe.log.serialize")));	
	item->SetCheckable(true);
	menu->Append(item);
	item->Check(serialize);
	item->Enable(enable);
	

	menu->AppendSeparator();

	for(int32 i = 0; i <= static_cast<uint32>(logLevelCritical); i++)
	{
		item = new wxMenuItem(menu.get(), cmdChangeLogLevel + i, conversions::from_utf16<wxString>(getText("ui.mainframe.log.level." + conversions::to_utf16(i))));		
		item->SetCheckable(true);
		menu->Append(item);
		item->Check(i == level);
		item->Enable(enable);
		
	}

#if OS_PLATFORM == OS_PLATFORM_WIN
	menu->AppendSeparator();

	item =  new wxMenuItem(menu.get(), cmdEnableBalloon, conversions::from_utf16<wxString>(getText("ui.mainframe.log.balloon")));	
	item->SetCheckable(true);
	menu->Append(item);
	item->Check(balloon);
	item->Enable(enable);
	
#endif


	m_buttonLogSettings->PopupMenu(menu.get(), 0, m_buttonLogSettings->GetSize().y);
}

void LogWindow::onEnableLog(wxCommandEvent &e)
{
	wxBusyCursor busyCursor;

	Options::instance()->setOption<bool>(Options::log_options::enable, !Options::instance()->getOption<bool>(Options::log_options::enable));
	Options::instance()->update();
}

void LogWindow::onEnableFileLog(wxCommandEvent &e)
{
	wxBusyCursor busyCursor;

	Options::instance()->setOption<bool>(Options::log_options::serialize, !Options::instance()->getOption<bool>(Options::log_options::serialize));
	Options::instance()->update();
}

void LogWindow::onEnableBalloon(wxCommandEvent &e)
{
	wxBusyCursor busyCursor;

	Options::instance()->setOption<bool>(Options::ui_options::tray_balloon, !Options::instance()->getOption<bool>(Options::ui_options::tray_balloon));
	Options::instance()->update();
}

void LogWindow::onChangeLogLevel(wxCommandEvent &e)
{
	wxBusyCursor busyCursor;

	Options::instance()->setOption<uint32>(Options::log_options::level, conversions::toEnumeratedType<LogLevel>(static_cast<uint32>(e.GetId() - cmdChangeLogLevel)));
	Options::instance()->update();
}

void LogWindow::onCopyLogMessage(wxCommandEvent &e)
{
	wxString str;

	long index = 0;
	for(;;)
	{
		index = m_logCtrl->GetNextItem(index, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if(index == -1)
			break;

		wxListItem info;
		info.m_mask = wxLIST_MASK_TEXT; 
		info.m_itemId = index; 
 		info.m_col = 1; 
		if(m_logCtrl->GetItem(info))
		{
			if(str.empty() == false)
				str.append("\r\n");
			
			str.append(info.GetText());
		}
	}

	if(str.empty() == false)
	{
		if(wxTheClipboard->Open())
		{
			wxTheClipboard->SetData(new wxTextDataObject(str));
			wxTheClipboard->Close();
		}
	}
}

void LogWindow::onLogMessageRightClick(wxListEvent &e)
{
	e.Skip();

	m_enableAutoscoll = false;

	scoped_ptr<wxMenu> menu(new wxMenu());
	menu->Append(new wxMenuItem(menu.get(), cmdCopyLogMessage, conversions::from_utf16<wxString>(getText("ui.mainframe.log.menu.copy_message"))));
	m_logCtrl->PopupMenu(menu.get());	
	
	m_enableAutoscoll = true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
