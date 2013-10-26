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

#ifndef _OS_ENGINE_PORTALPEER_H
#define _OS_ENGINE_PORTALPEER_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "boostasio.h"
#include "datetime.h"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

#define OS_PEER_ORIGIN_ISIS			"isis"		// bootstrap url
#define OS_PEER_ORIGIN_CACHE		"cache"		// database
#define OS_PEER_ORIGIN_PEX			"pex"		// peer exchange
#define OS_PEER_ORIGIN_INCOMING		"incoming"	// incoming peer
#define OS_PEER_ORIGIN_MANUAL		"manual"	// manually added

#define OS_PEER_MAX_RETRIES			3			// Dopo 3 invialidazioni il nodo viene cancellato

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPAddress;
class Portal;
class PortalsPeersManager;

//////////////////////////////////////////////////////////////////////

class EngineExport PortalsPeer : public Object
{
	friend class pimpl<PortalsPeersManager>;

// Construction
public:
	PortalsPeer(shared_ptr<boost::recursive_mutex> mutex, const boost::asio::ip::tcp::endpoint &endpoint, const String &origin = String::EMPTY);
	virtual ~PortalsPeer();

// Attributes
public:
	// Restituisce l'ID univoco del nodo (ip + porta)
	uint64 getID() const;
	// Restituisce l'indirizzo del nodo
	boost::asio::ip::tcp::endpoint getEndpoint() const;
	// Restituisce l'IP del nodo remoto
	String getIP() const;
	// Restituisce la porta del nodo remoto
	uint32 getPort() const;
	// Restituisce il numero di tentativi effettuati sul nodo
	uint16 getRetries() const;
	// Restituisce l'origine del nodo
	String getOrigin() const;
	// Restituisce la data di ultimo contatto con il nodo
	DateTime getLastContactDate() const;
	// Indica se il peer è un supernodo
	bool getSupernode() const;
	// Indica se il nodo è salvabile nella cache (deve essere un supernodo ed essere stato contattato di recente)
	bool getCacheable() const;

private:
	// Aggiorna lo stato del supernodo
	void setSupernode(bool supernode);
	
// Operations
public:
	static uint64 makeID(const String &ip, uint32 port);
	static uint64 makeID(shared_ptr<IPAddress> address);
	static uint64 makeID(const boost::asio::ip::tcp::endpoint &endpoint);	

private:
	// Resetta il numero di tentativi sul nodo
	void validate();
	// Invalida il nodo, restituisce true se ancora valido altrimenti false nel caso in cui sia stato rimosso
	bool invalidate();

// Operators
public:
	bool operator ==(const PortalsPeer &second) const;
	bool operator <(const PortalsPeer &second) const;

private:
	shared_ptr<boost::recursive_mutex> m_mutex;
	boost::asio::ip::tcp::endpoint m_endpoint;
	uint16 m_retries;
	String m_origin;
	bool m_supernode;
	DateTime m_lastContactDate;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_PORTALPEER_H
