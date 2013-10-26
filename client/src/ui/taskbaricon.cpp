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
#include "taskbaricon.h"

#include "application.h"
#include "conversions.h"
#include "engine.h"
#include "mainwindow.h"
#include "uiapi.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(TaskBarIcon, wxTaskBarIcon)
	EVT_MENU(MainWindow::cmdExit, TaskBarIcon::onExit)
	EVT_MENU(MainWindow::cmdViewHome, TaskBarIcon::onViewHome)
	EVT_MENU(MainWindow::cmdRestore, TaskBarIcon::onRestore)
	EVT_TASKBAR_LEFT_DCLICK(TaskBarIcon::onLeftDblClk)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

TaskBarIcon::TaskBarIcon() : m_visible(false)
{

}

TaskBarIcon::~TaskBarIcon()
{

}

void TaskBarIcon::show(const wxIcon &icon)
{
	m_visible = true;
	update(icon);
}

void TaskBarIcon::hide()
{
	RemoveIcon();
	m_visible = false;
}

void TaskBarIcon::update(const wxIcon &icon)
{
	if(m_visible == false)
		return;

	SetIcon(icon, conversions::from_utf16<wxString>(Engine::getVersionName(true)));
}

void TaskBarIcon::_restore()
{
	MainWindow::instance()->showMode(false);
}

wxMenu * TaskBarIcon::CreatePopupMenu()
{
	wxMenuItem *menuItem;

	wxMenu *popup = new wxMenu();

	menuItem = new wxMenuItem(popup, MainWindow::cmdViewHome, conversions::from_utf16<wxString>(MainWindow::instance()->getText(_S("ui.mainframe.traymenu.enter"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("menu_enter.png"))), wxBITMAP_TYPE_PNG));
	popup->Append(menuItem);

	if(MainWindow::instance()->IsShown() == false)
	{
		menuItem = new wxMenuItem(popup, MainWindow::cmdRestore, conversions::from_utf16<wxString>(MainWindow::instance()->getText(_S("ui.mainframe.traymenu.restore"))));
		menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("menu_restore.png"))), wxBITMAP_TYPE_PNG));
		popup->Append(menuItem);
	}

#if OS_PLATFORM != OS_PLATFORM_MAC
	popup->AppendSeparator();

	menuItem = new wxMenuItem(popup, MainWindow::cmdExit, conversions::from_utf16<wxString>(MainWindow::instance()->getText(_S("ui.mainframe.traymenu.exit"))));
	menuItem->SetBitmap(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("menu_exit.png"))), wxBITMAP_TYPE_PNG));
	popup->Append(menuItem);
#endif

	return popup;
}

void TaskBarIcon::onExit(wxCommandEvent &e)
{
	MainWindow::instance()->Close(true);
}

void TaskBarIcon::onLeftDblClk(wxTaskBarIconEvent &e)
{
	_restore();
}

void TaskBarIcon::onViewHome(wxCommandEvent &e)
{
	Application::instance()->viewHome();
}

void TaskBarIcon::onRestore(wxCommandEvent &e)
{
	_restore();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
