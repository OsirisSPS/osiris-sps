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
#include "isisconnectionswindow.h"

#include "application.h"
#include "boost/bind.hpp"
#include "conversions.h"
#include "connectioninfo.h"
#include "isisconnection.h"
#include "isisconnectionstatus.h"
#include "isisservice.h"
#include "listctrlrenderer.h"
#include "locked.h"
#include "platformmanager.h"
#include "portalsportal.h"
#include "portalsportalssystem.h"
#include "uiapi.h"
#include "uistandards.h"
#include "skinnedwindow.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SINGLETON(OS_NAMESPACE_NAME::IsisConnectionsWindow)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static const uint32 idRefreshConnection = wxNewId();
static const uint32 idRemoveConnection = wxNewId();
static const uint32 idRefreshTimer = wxNewId();

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(IsisConnectionsWindow, WindowBase)
	EVT_TIMER(idRefreshTimer, IsisConnectionsWindow::onRefreshTimer)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

SafeVar<bool> IsisConnectionsWindow::s_valid(false);

//////////////////////////////////////////////////////////////////////

IsisConnectionsWindow::IsisConnectionsWindow(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style) : WindowBase(parent, id, pos, size, style),
																																	m_connectionsCtrl(null),
																																	m_imageList(16, 16),
																																	m_connectionsLimit(OS_UI_ISIS_CONNECTIONS_LIMIT)
{
	s_valid = true;

	m_timer = new wxTimer(this, idRefreshTimer);

	m_connectionIcon = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_isis_connection.png"))), wxBITMAP_TYPE_PNG));

	createLayout();
	initWindow();

	shared_ptr<IsisService> isisService = PortalsSystem::instance()->getIsisService();
	if(isisService != null)
	{
		shared_ptr<ConnectionsManager> connectionsManager = isisService->getConnectionsManager();
		if(connectionsManager != null)
		{
			// Inizializza le connessioni attive
			Locked<const ConnectionsManager::Connections>::unique activeConnections = connectionsManager->getConnections();
			for(ConnectionsManager::Connections::const_iterator i = activeConnections->begin(); i != activeConnections->end(); ++i)
			{
				refreshConnectionInfo(i->second, connectionAdded);
			}

			// Registra il callback per la modifica delle connessioni (N.B.: tiene attivo "activeConnections" per assicurare la sincronizzazione tenendo loccato il mutex)
			connectionsManager->addListener(boost::bind(&IsisConnectionsWindow::connectionEventCallback, _1, _2));
		}
	}

	m_timer->Start(OS_UI_CONNECTIONS_PENDING_REFRESH_TIMER);
}

IsisConnectionsWindow::~IsisConnectionsWindow()
{
	m_timer->Stop();

	s_valid = false;

	delete m_timer;
	m_timer = null;
}

shared_ptr<ConnectionInfo> IsisConnectionsWindow::getConnectionInfo(uint32 connectionID) const
{
	OS_LOCK(m_cs);
	return m_connections.get(connectionID);
}

int32 IsisConnectionsWindow::getConnectionItem(uint32 connectionID) const
{
	int32 itemCount = m_connectionsCtrl->GetItemCount();
	for(int32 i = 0; i < itemCount; i++)
	{
		if(static_cast<uint32>(m_connectionsCtrl->GetItemData(i)) == connectionID)
			return i;
	}

	return -1;
}

void IsisConnectionsWindow::getConnections(Connections &connections)
{
	OS_LOCK(m_cs);
	connections = m_connections;
}

uint32 IsisConnectionsWindow::getConnectionsLimit() const
{
	OS_LOCK(m_cs);
	return m_connectionsLimit;
}

void IsisConnectionsWindow::createLayout()
{
	wxBoxSizer* contentSizer;
	contentSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_connectionsCtrl = new SkinnedWindow<wxListCtrl, ListCtrlRenderer>( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER|wxLC_REPORT );
	contentSizer->Add( m_connectionsCtrl, 1, wxEXPAND, 0 );
	
	this->SetSizer( contentSizer );
	this->Layout();
	contentSizer->Fit( this );
}

void IsisConnectionsWindow::initWindow()
{
	OS_ASSERT(m_connectionsCtrl != null);
	m_connectionsCtrl->InsertColumn(columnStatus, conversions::from_utf16<wxString>(getText(_S("ui.isisconnectionswindow.column_status"))), wxLIST_FORMAT_LEFT, 100);
	m_connectionsCtrl->InsertColumn(columnPortal, conversions::from_utf16<wxString>(getText(_S("ui.isisconnectionswindow.column_portal"))), wxLIST_FORMAT_LEFT, 100);
	m_connectionsCtrl->InsertColumn(columnName, conversions::from_utf16<wxString>(getText(_S("ui.isisconnectionswindow.column_name"))), wxLIST_FORMAT_LEFT, 100);
	m_connectionsCtrl->InsertColumn(columnRequests, conversions::from_utf16<wxString>(getText(_S("ui.isisconnectionswindow.requests"))), wxLIST_FORMAT_LEFT, 100);
	m_connectionsCtrl->InsertColumn(columnLastEvent, conversions::from_utf16<wxString>(getText(_S("ui.isisconnectionswindow.last_event"))), wxLIST_FORMAT_LEFT, 200);
	m_connectionsCtrl->SetImageList(&m_imageList, wxIMAGE_LIST_SMALL);
}

void IsisConnectionsWindow::resizeWindow()
{
	if(m_connectionsCtrl == null)
		return;

	m_connectionsCtrl->SetSize(GetSize());
}

bool IsisConnectionsWindow::markConnectionRemoved(uint32 connectionID)
{
	int32 connectionItem = getConnectionItem(connectionID);
	if(connectionItem == -1)
		return false;

	m_connectionsCtrl->SetItemTextColour(connectionItem, OS_UI_CONNECTIONS_REMOVED_COLOUR);
	return true;
}

void IsisConnectionsWindow::refreshConnectionItem(uint32 connectionID)
{
	// Carica i dettagli sulla connessione
	shared_ptr<ConnectionInfo> connectionInfo = getConnectionInfo(connectionID);
	if(connectionInfo == null || connectionInfo->getModified() == false)
		return;		// Il riferimento alla connessione potrebbe essere stato rimosso in seguito al superamento del limite

	int32 connectionItem = -1;
	if(connectionInfo->getInitialized())
	{
		connectionItem = getConnectionItem(connectionID);
	}
	else
	{
		connectionItem = m_connectionsCtrl->InsertItem(m_connectionsCtrl->GetItemCount(), m_connectionIcon);
		if(connectionItem == -1)
		{
			OS_ASSERTFALSE();
			return;
		}
		m_connectionsCtrl->SetItemData(connectionItem, connectionID);

		connectionInfo->setInitialized(true);
	}

	// Aggiorna i dettagli sulla connessione
	shared_ptr<IsisConnectionStatus> connectionStatus = boost::dynamic_pointer_cast<IsisConnectionStatus>(connectionInfo->getConnectionStatus());
	if(connectionStatus == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	String status;
	switch(connectionStatus->getStatus())
	{
	case IsisConnectionStatus::isNone:				status = getText(_S("ui.isisconnectionswindow.status.none"));
													break;

	case IsisConnectionStatus::isSendSystem:		status = getText(_S("ui.isisconnectionswindow.status.send_system"));
													break;

	case IsisConnectionStatus::isReceiveSystem:		status = getText(_S("ui.isisconnectionswindow.status.receive_system"));
													break;

	case IsisConnectionStatus::isSendRequest:		status = getText(_S("ui.isisconnectionswindow.status.send_request"));
													break;

	case IsisConnectionStatus::isReceiveRequest:	status = getText(_S("ui.isisconnectionswindow.status.receive_request"));
													break;

	case IsisConnectionStatus::isReceiveResponse:	status = getText(_S("ui.isisconnectionswindow.status.receive_response"));
													break;

	default:										OS_ASSERTFALSE();
													break;

	}

	m_connectionsCtrl->SetItem(connectionItem, columnStatus, conversions::from_utf16<wxString>(status));

	shared_ptr<Portal> portal = connectionStatus->getPortal();
	m_connectionsCtrl->SetItem(connectionItem, columnPortal, portal != null ? conversions::from_utf16<wxString>(portal->getName()) : conversions::from_utf16<wxString>(_S("?")));

    m_connectionsCtrl->SetItem(connectionItem, columnName, conversions::from_utf16<wxString>(connectionStatus->getName()));

	m_connectionsCtrl->SetItem(connectionItem, columnRequests, wxString::Format(_T("%d"), connectionStatus->getRequests()));

	m_connectionsCtrl->SetItem(connectionItem, columnLastEvent, conversions::from_utf16<wxString>(connectionStatus->getLastEvent()));

	connectionInfo->resetModifiedFlag();
}

void IsisConnectionsWindow::removeConnectionItem(uint32 connectionID)
{
	int32 connectionItem = getConnectionItem(connectionID);
	if(connectionItem == -1)
		return;

	OS_ASSERT(getConnectionInfo(connectionID) == null);
	m_connectionsCtrl->DeleteItem(connectionItem);
}

void IsisConnectionsWindow::refreshConnections()
{
	Connections connections;
	// Effettua una copia per evitare una lock durante il check
	getConnections(connections);

	wxWindowUpdateLocker windowLocker(m_connectionsCtrl);

	uint32 connectionsCount = static_cast<uint32>(connections.size());
	uint32 connectionsLimit = getConnectionsLimit();

	bool pendingRefresh = false;

	for(Connections::iterator i = connections.begin(); i != connections.end(); ++i)
	{
		if(i->second->getRemoved())
		{
			if(i->second->isElapsed() || (connectionsCount > connectionsLimit))
			{
				doRemoveConnection(i->first);
				connectionsCount--;
			}
			else
			{
				markConnectionRemoved(i->first);
			}
		}
		else
		{
			refreshConnectionItem(i->first);
		}
	}
}

void IsisConnectionsWindow::refreshConnectionInfo(shared_ptr<IConnection> connection, ConnectionEvent e)
{
	OS_LOCK(m_cs);

	shared_ptr<IsisConnection> isisConnection = boost::dynamic_pointer_cast<IsisConnection>(connection);
	if(isisConnection == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	uint32 connectionID = connection->getID();
	shared_ptr<ConnectionInfo> connectionInfo = m_connections.get(connectionID);

	switch(e)
	{
	case connectionAdded:		OS_ASSERT(connectionInfo == null);

								// Salva la nuova connessione
								connectionInfo.reset(OS_NEW ConnectionInfo(connectionID, DateTime::now()));
								m_connections.push_back(connectionID, connectionInfo);

								// Inizializza lo stato della connessione
								connectionInfo->setConnectionStatus(isisConnection->takeStatusSnapshot());

								// Inizializza il callback degli eventi sulla connessione
								isisConnection->addListener(boost::bind(&IsisConnectionsWindow::connectionStateChangedCallback, _1));

								break;

	case connectionUpdated:		if(connectionInfo != null)
								{
									// Aggiorna la data dell'ultimo aggiornamento sulla connessione
									connectionInfo->setLastAction(DateTime::now());
									// Aggiorna lo stato della connessione
									connectionInfo->setConnectionStatus(isisConnection->takeStatusSnapshot());
								}

								break;

	case connectionRemoved:		if(connectionInfo != null)
									connectionInfo->setRemoved(true);

								break;

	default:					OS_ASSERTFALSE();
								break;
	}
}

void IsisConnectionsWindow::removeConnectionInfo(uint32 connectionID)
{
	OS_LOCK(m_cs);
	m_connections.remove(connectionID);
}

void IsisConnectionsWindow::doRemoveConnection(uint32 connectionID)
{
	removeConnectionInfo(connectionID);
	removeConnectionItem(connectionID);
}

void IsisConnectionsWindow::handleConnectionEvent(shared_ptr<IConnection> connection, ConnectionsManager::ConnectionEvent e)
{
	// N.B.: il callback può essere chiamato da threads diversi pertanto ogni notifica deve essere fatta via post

	if(connection == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	switch(e)
	{
	case ConnectionsManager::connectionAdded:		// Inizializza lo stato della connessione
													refreshConnectionInfo(connection, connectionAdded);
													break;

	case ConnectionsManager::connectionRemoved:		// Rimuove lo stato della connessione
													refreshConnectionInfo(connection, connectionRemoved);
													break;

	default:										OS_ASSERTFALSE();
													break;
	}
}

void IsisConnectionsWindow::handleConnectionStateChanged(shared_ptr<IConnection> connection)
{
	// N.B.: il callback può essere chiamato da threads diversi pertanto ogni notifica deve essere fatta via post

	if(connection == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	refreshConnectionInfo(connection, connectionUpdated);
}

void IsisConnectionsWindow::connectionEventCallback(shared_ptr<IConnection> connection, ConnectionsManager::ConnectionEvent e)
{
	// TODO: da pulire...boost::signal non è threadsafe pertanto al momento si utilizza una variabile
	// statica per determinare se l'istanza di IsisConnectionsWindow è ancora valida (il callback altrimenti
	// potrebbe essere chiamato in fase di distruzione della finestra)

	if(s_valid)
		IsisConnectionsWindow::instance()->handleConnectionEvent(connection, e);
}

void IsisConnectionsWindow::connectionStateChangedCallback(shared_ptr<IConnection> connection)
{
	// TODO: da pulire...boost::signal non è threadsafe pertanto al momento si utilizza una variabile
	// statica per determinare se l'istanza di IsisConnectionsWindow è ancora valida (il callback altrimenti
	// potrebbe essere chiamato in fase di distruzione della finestra)

	if(s_valid)
		IsisConnectionsWindow::instance()->handleConnectionStateChanged(connection);
}

void IsisConnectionsWindow::onRefreshTimer(wxTimerEvent &e)
{
	refreshConnections();

	e.Skip();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
