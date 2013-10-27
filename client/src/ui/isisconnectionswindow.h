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

#ifndef _OS_UI_ISISCONNECTIONSWINDOW_H
#define _OS_UI_ISISCONNECTIONSWINDOW_H

#include "boost/signals/trackable.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "connectionsmanager.h"
#include "multilanguagewindow.h"
#include "singleton.h"
#include "safevar.h"

//////////////////////////////////////////////////////////////////////

#define OS_UI_ISIS_CONNECTIONS_LIMIT					10

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ConnectionInfo;

//////////////////////////////////////////////////////////////////////

class IsisConnectionsWindow : public wxPanel,
							  public MultilanguageWindow,
							  public DynamicSingleton<IsisConnectionsWindow>,
							  public boost::signals::trackable
{
	DECLARE_EVENT_TABLE()

	typedef wxPanel WindowBase;

	typedef Collections<uint32, ConnectionInfo>::shared_collection Connections;

// Enumerated types
private:
	enum ConnectionsColums
	{
		columnStatus			= 0,
		columnPortal			= 1,
		columnName  			= 2,
		columnRequests			= 3,
		columnLastEvent			= 4,
	};

	enum ConnectionEvent
	{
		connectionAdded			= 0,
		connectionRemoved		= 1,
		connectionUpdated		= 2,
	};

// Construction
public:
	IsisConnectionsWindow(wxWindow *parent, wxWindowID id = static_cast<wxWindowID>(wxID_ANY), const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0);
	virtual ~IsisConnectionsWindow();

// Attributes
public:
	shared_ptr<ConnectionInfo> getConnectionInfo(uint32 connectionID) const;
	int32 getConnectionItem(uint32 connectionID) const;

private:
	void getConnections(Connections &connections);
	uint32 getConnectionsLimit() const;

// Operations
private:
	void createLayout();
	void initWindow();

	void resizeWindow();

	bool markConnectionRemoved(uint32 connectionID);

	void refreshConnectionItem(uint32 connectionID);
	void removeConnectionItem(uint32 connectionID);

	void refreshConnections();

	void refreshConnectionInfo(shared_ptr<IConnection> connection, ConnectionEvent e);
	void removeConnectionInfo(uint32 connectionID);

	void doRemoveConnection(uint32 connectionID);

	void handleConnectionEvent(shared_ptr<IConnection> connection, ConnectionsManager::ConnectionEvent e);
	void handleConnectionStateChanged(shared_ptr<IConnection> connection);

	static void connectionEventCallback(shared_ptr<IConnection> connection, ConnectionsManager::ConnectionEvent e);
	static void connectionStateChangedCallback(shared_ptr<IConnection> connection);

// Events
private:
	void onRefreshTimer(wxTimerEvent &e);

private:
	mutable boost::recursive_mutex m_cs;
	wxListCtrl *m_connectionsCtrl;
	Connections m_connections;
	wxTimer *m_timer;
	uint32 m_connectionsLimit;
	int32 m_connectionIcon;
	wxImageList m_imageList;
	static SafeVar<bool> s_valid;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_ISISCONNECTIONSWINDOW_H
