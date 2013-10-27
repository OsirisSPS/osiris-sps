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

#ifndef _OS_DHT_DHTTHREAD_H
#define _OS_DHT_DHTTHREAD_H

#include "boostasio.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ID;
class IDHTSearch;

//////////////////////////////////////////////////////////////////////

class DHTThread : public Thread
{
	typedef Thread ThreadBase;

	typedef unordered_map<std::string, uint16>::type Nodes;
	typedef unordered_map<std::string, shared_ptr<IDHTSearch> >::type Searches;

// Enumerated types
public:
	enum DHTStatus
	{
		dhtStatusBroken       = 0,
		dhtStatusPoor         = 1,
		dhtStatusFirewalled   = 2,
		dhtStatusGood         = 3,
	};

// Construction
public:
	DHTThread(const String &rootPath, boost::asio::ip::udp::socket::native_type socket);
	virtual ~DHTThread();

// Attributes
public:
	DHTStatus getStatus(int *total = 0) const;

// Operations
private:
	shared_ptr<IDHTSearch> peekNextSearch();
	void loadNodesCache();
	void loadRouters();
	void bootstrap();
	bool bootstrap(const std::string &host, uint16 port);

	static void dhtEventCallback(void *closure, int e, unsigned char *id, void *data, size_t data_len);
	static std::string generateSearchID(const std::string &id);
	static std::string getSearchID(unsigned char *id);

	bool processResults(unsigned char *id, void *data, uint32 size);
	bool removeSearch(unsigned char *id);

// Thread interface
protected:
	virtual void run();

// Thread overrides
protected:
	virtual void onLeave();

private:
	String m_rootPath;
	boost::asio::ip::udp::socket::native_type m_socket;
	Nodes m_nodes;
	Searches m_searches;
	uint32 m_portal;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_DHT_DHTTHREAD_H
