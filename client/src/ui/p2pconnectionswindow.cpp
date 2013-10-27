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
#include "p2pconnectionswindow.h"

#include "application.h"
#include "boost/bind.hpp"
#include "connectioninfo.h"
#include "conversions.h"
#include "iconnection.h"
#include "listctrlrenderer.h"
#include "locked.h"
#include "options.h"
#include "p2pconnection.h"
#include "p2pnodestatus.h"
#include "p2pp2psystem.h"
#include "p2pserver.h"
#include "platformmanager.h"
#include "portalsportal.h"
#include "uiapi.h"
#include "uistandards.h"
#include "skinnedwindow.h"

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SINGLETON(OS_NAMESPACE_NAME::P2PConnectionsWindow)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static const uint32 idRefreshConnection = wxNewId();
static const uint32 idRemoveConnection = wxNewId();
static const uint32 idRefreshTimer = wxNewId();

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(P2PConnectionsWindow, WindowBase)
	EVT_TIMER(idRefreshTimer, P2PConnectionsWindow::onRefreshTimer)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

SafeVar<bool> P2PConnectionsWindow::s_valid(false);

//////////////////////////////////////////////////////////////////////

P2PConnectionsWindow::P2PConnectionsWindow(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style) : WindowBase(parent, id, pos, size, style),
																																  m_connectionsCtrl(null),
																																  m_imageList(16, 16),
																																  m_connectionsLimit(0)
{
	s_valid = true;

	m_timer = new wxTimer(this, idRefreshTimer);

	m_connectionInIcon = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_connection_in.png"))), wxBITMAP_TYPE_PNG));
	m_connectionOutIcon = m_imageList.Add(wxBitmap(conversions::from_utf16<wxString>(Application::instance()->getResourcesFilePath(_S("main_icons_connection_out.png"))), wxBITMAP_TYPE_PNG));

	createLayout();
	initWindow();

	loadOptions();

	shared_ptr<P2PServer> server = p2p::P2PSystem::instance()->getServer();
	if(server != null)
	{
		shared_ptr<ConnectionsManager> connectionsManager = server->getConnectionsManager();
		if(connectionsManager != null)
		{
			// Inizializza le connessioni attive
			Locked<const ConnectionsManager::Connections>::unique activeConnections = connectionsManager->getConnections();
			for(ConnectionsManager::Connections::const_iterator i = activeConnections->begin(); i != activeConnections->end(); ++i)
			{
				refreshConnectionInfo(i->second, connectionAdded);
			}

			// Registra il callback per la modifica delle connessioni (N.B.: tiene attivo "activeConnections" per assicurare la sincronizzazione tenendo loccato il mutex)
			connectionsManager->addListener(boost::bind(&P2PConnectionsWindow::connectionEventCallback, _1, _2));
		}
	}

	Options::instance()->subscribeUpdateHandler(boost::bind(&P2PConnectionsWindow::handleUpdateOptions, this));

	m_timer->Start(OS_UI_CONNECTIONS_PENDING_REFRESH_TIMER);
}

P2PConnectionsWindow::~P2PConnectionsWindow()
{
	m_timer->Stop();

	s_valid = false;

	delete m_timer;
	m_timer = null;
}

shared_ptr<ConnectionInfo> P2PConnectionsWindow::getConnectionInfo(uint32 connectionID) const
{
	OS_LOCK(m_cs);
	return m_connections.get(connectionID);
}

int32 P2PConnectionsWindow::getConnectionItem(uint32 connectionID) const
{
	int32 itemCount = m_connectionsCtrl->GetItemCount();
	for(int32 i = 0; i < itemCount; i++)
	{
		if(static_cast<uint32>(m_connectionsCtrl->GetItemData(i)) == connectionID)
			return i;
	}

	return -1;
}

void P2PConnectionsWindow::getConnections(Connections &connections)
{
	OS_LOCK(m_cs);
	connections = m_connections;
}

uint32 P2PConnectionsWindow::getConnectionsLimit() const
{
	OS_LOCK(m_cs);
	return m_connectionsLimit;
}

void P2PConnectionsWindow::createLayout()
{
	wxBoxSizer* contentSizer;
	contentSizer = new wxBoxSizer( wxHORIZONTAL );

	m_connectionsCtrl = new SkinnedWindow<wxListCtrl, ListCtrlRenderer>( this, static_cast<wxWindowID>(wxID_ANY), wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER|wxLC_REPORT );
	contentSizer->Add( m_connectionsCtrl, 1, wxEXPAND, 0 );

	this->SetSizer( contentSizer );
	this->Layout();
	contentSizer->Fit( this );
}

void P2PConnectionsWindow::initWindow()
{
	OS_ASSERT(m_connectionsCtrl != null);
	m_connectionsCtrl->InsertColumn(columnStatus, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.connections.status"))), wxLIST_FORMAT_LEFT, 100);
	m_connectionsCtrl->InsertColumn(columnPortal, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.connections.portal"))), wxLIST_FORMAT_LEFT, 100);
	m_connectionsCtrl->InsertColumn(columnPeer, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.connections.peer"))), wxLIST_FORMAT_LEFT, 100);
	m_connectionsCtrl->InsertColumn(columnTransfers, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.connections.transfers"))), wxLIST_FORMAT_LEFT, 100);
	m_connectionsCtrl->InsertColumn(columnObjects, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.connections.objects"))), wxLIST_FORMAT_LEFT, 100);
	m_connectionsCtrl->InsertColumn(columnOrigin, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.connections.origin"))), wxLIST_FORMAT_LEFT, 100);
	m_connectionsCtrl->InsertColumn(columnLastEvent, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.connections.last_event"))), wxLIST_FORMAT_LEFT, 200);
	m_connectionsCtrl->SetImageList(&m_imageList, wxIMAGE_LIST_SMALL);
}

void P2PConnectionsWindow::resizeWindow()
{
	if(m_connectionsCtrl == null)
		return;

	m_connectionsCtrl->SetSize(GetSize());
}

void P2PConnectionsWindow::loadOptions()
{
	OS_LOCK(m_cs);

	uint32 maxIncomingConnections = Options::instance()->getOption<uint32>(Options::p2p_options::max_incoming_connections);
	uint32 maxOutgoingConnections = Options::instance()->getOption<uint32>(Options::p2p_options::max_outgoing_connections);

	m_connectionsLimit = (maxIncomingConnections + maxOutgoingConnections) * 2 + 1;
}

bool P2PConnectionsWindow::markConnectionRemoved(uint32 connectionID)
{
	int32 connectionItem = getConnectionItem(connectionID);
	if(connectionItem == -1)
		return false;

	m_connectionsCtrl->SetItemTextColour(connectionItem, OS_UI_CONNECTIONS_REMOVED_COLOUR);
	return true;
}

void P2PConnectionsWindow::refreshConnectionItem(uint32 connectionID)
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
		int32 iconIndex = -1;

		shared_ptr<ConnectionStatus> nodeStatus = connectionInfo->getConnectionStatus();
		OS_ASSERT(nodeStatus != null);
		if(nodeStatus != null)
		{
			OS_ASSERT(boost::dynamic_pointer_cast<p2p::NodeStatus>(nodeStatus) != null);
			iconIndex = boost::dynamic_pointer_cast<p2p::NodeStatus>(nodeStatus)->getOutgoing() ? m_connectionOutIcon : m_connectionInIcon;
		}

		connectionItem = m_connectionsCtrl->InsertItem(m_connectionsCtrl->GetItemCount(), iconIndex);
		if(connectionItem == -1)
		{
			OS_ASSERTFALSE();
			return;
		}
		m_connectionsCtrl->SetItemData(connectionItem, connectionID);

		connectionInfo->setInitialized(true);
	}

	// Aggiorna i dettagli sulla connessione
	shared_ptr<p2p::NodeStatus> connectionStatus = boost::dynamic_pointer_cast<p2p::NodeStatus>(connectionInfo->getConnectionStatus());
	if(connectionStatus == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	String status;
	switch(connectionStatus->getStatus())
	{
	case p2p::NodeStatus::statusInitializing:			status = getText(_S("ui.mainframe.connections.nodestatus.initializing"));
														break;

	case p2p::NodeStatus::statusAttemptingConnection:	status = getText(_S("ui.mainframe.connections.nodestatus.attempting_connection"));
														break;

	case p2p::NodeStatus::statusHandshaking:			status = getText(_S("ui.mainframe.connections.nodestatus.handshaking"));
														break;

	case p2p::NodeStatus::statusExchanging:				status = getText(_S("ui.mainframe.connections.nodestatus.exchanging_data"));
														break;

	case p2p::NodeStatus::statusTerminating:			status = getText(_S("ui.mainframe.connections.nodestatus.terminating"));
														break;

	default:											OS_ASSERTFALSE();
														break;
	}

	m_connectionsCtrl->SetItem(connectionItem, columnStatus, conversions::from_utf16<wxString>(status));

	shared_ptr<Portal> portal = connectionStatus->getPortal();
	m_connectionsCtrl->SetItem(connectionItem, columnPortal, portal != null ? conversions::from_utf16<wxString>(portal->getName()) : conversions::from_utf16<wxString>(_S("?")));

	shared_ptr<IPAddress> peer = connectionStatus->getPeer();
	m_connectionsCtrl->SetItem(connectionItem, columnPeer, (peer == null || peer->isAny()) ? conversions::from_utf16<wxString>(_S("?")) : conversions::from_utf16<wxString>(String::format(_S("%S:%d").c_str(), peer->toString().c_str(), peer->getPort())));

	String tranfers = String::format(_S("D: %S / U: %S").c_str(), utils::formatSize(connectionStatus->getDownloadedBytes()).c_str(), utils::formatSize(connectionStatus->getUploadedBytes()).c_str());
	m_connectionsCtrl->SetItem(connectionItem, columnTransfers, conversions::from_utf16<wxString>(tranfers));

	String objects = String::format(_S("D: %d / U: %d").c_str(), connectionStatus->getDownloadedObjects(), connectionStatus->getUploadedObjects());
	m_connectionsCtrl->SetItem(connectionItem, columnObjects, conversions::from_utf16<wxString>(objects));

	m_connectionsCtrl->SetItem(connectionItem, columnOrigin, conversions::from_utf16<wxString>(getText(_S("ui.mainframe.connections.origin.") + connectionInfo->getOrigin())));

	m_connectionsCtrl->SetItem(connectionItem, columnLastEvent, conversions::from_utf16<wxString>(connectionStatus->getLastEvent()));

	connectionInfo->resetModifiedFlag();
}

void P2PConnectionsWindow::removeConnectionItem(uint32 connectionID)
{
	int32 connectionItem = getConnectionItem(connectionID);
	if(connectionItem == -1)
		return;

	OS_ASSERT(getConnectionInfo(connectionID) == null);
	m_connectionsCtrl->DeleteItem(connectionItem);
}

void P2PConnectionsWindow::refreshConnections()
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

void P2PConnectionsWindow::refreshConnectionInfo(shared_ptr<IConnection> connection, ConnectionEvent e)
{
	OS_LOCK(m_cs);

	shared_ptr<P2PConnection> p2pConnection = boost::dynamic_pointer_cast<P2PConnection>(connection);
	if(p2pConnection == null)
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
								connectionInfo.reset(OS_NEW ConnectionInfo(connectionID, DateTime::now(), p2pConnection->getOrigin()));
								m_connections.push_back(connectionID, connectionInfo);

								// Inizializza lo stato della connessione
								connectionInfo->setConnectionStatus(p2pConnection->takeStatusSnapshot());

								// Inizializza il callback degli eventi sulla connessione
								p2pConnection->addListener(boost::bind(&P2PConnectionsWindow::connectionStateChangedCallback, _1));

								break;

	case connectionUpdated:		if(connectionInfo != null)
								{
									// Aggiorna la data dell'ultimo aggiornamento sulla connessione
									connectionInfo->setLastAction(DateTime::now());
									// Aggiorna lo stato della connessione
									connectionInfo->setConnectionStatus(p2pConnection->takeStatusSnapshot());
								}

								break;

	case connectionRemoved:		if(connectionInfo != null)
									connectionInfo->setRemoved(true);

								break;

	default:					OS_ASSERTFALSE();
								break;
	}
}

void P2PConnectionsWindow::removeConnectionInfo(uint32 connectionID)
{
	OS_LOCK(m_cs);
	m_connections.remove(connectionID);
}

void P2PConnectionsWindow::doRemoveConnection(uint32 connectionID)
{
	removeConnectionInfo(connectionID);
	removeConnectionItem(connectionID);
}

void P2PConnectionsWindow::handleConnectionEvent(shared_ptr<IConnection> connection, ConnectionsManager::ConnectionEvent e)
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

void P2PConnectionsWindow::handleConnectionStateChanged(shared_ptr<IConnection> connection)
{
	// N.B.: il callback può essere chiamato da threads diversi pertanto ogni notifica deve essere fatta via post

	if(connection == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	refreshConnectionInfo(connection, connectionUpdated);
}

void P2PConnectionsWindow::handleUpdateOptions()
{
	loadOptions();
}

void P2PConnectionsWindow::connectionEventCallback(shared_ptr<IConnection> connection, ConnectionsManager::ConnectionEvent e)
{
	// TODO: da pulire...boost::signal non è threadsafe pertanto al momento si utilizza una variabile
	// statica per determinare se l'istanza di P2PConnectionsWindow è ancora valida (il callback altrimenti
	// potrebbe essere chiamato in fase di distruzione della finestra)

	if(s_valid)
		P2PConnectionsWindow::instance()->handleConnectionEvent(connection, e);
}

void P2PConnectionsWindow::connectionStateChangedCallback(shared_ptr<IConnection> connection)
{
	// TODO: da pulire...boost::signal non è threadsafe pertanto al momento si utilizza una variabile
	// statica per determinare se l'istanza di P2PConnectionsWindow è ancora valida (il callback altrimenti
	// potrebbe essere chiamato in fase di distruzione della finestra)

	if(s_valid)
		P2PConnectionsWindow::instance()->handleConnectionStateChanged(connection);
}

void P2PConnectionsWindow::onRefreshTimer(wxTimerEvent &e)
{
	refreshConnections();

	e.Skip();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
