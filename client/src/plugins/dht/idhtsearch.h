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

#ifndef _OS_DHT_IDHTSEARCH_H
#define _OS_DHT_IDHTSEARCH_H

#include "base/object.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;

//////////////////////////////////////////////////////////////////////

class IDHTSearch : public Object
{
// Construction
public:
	IDHTSearch(uint32 announcePort = 0);
	virtual ~IDHTSearch();

// Attributes
public:
	inline const Buffer * getID() const;

	inline uint32 getAnnouncePort() const;
	inline void setAnnouncePort(uint32 announcePort);

// Operations
public:
	bool assignID(const std::string &id);
	
// Interface
public:
	virtual void processResult(uint32 ip, uint32 port) = 0;

private:
	scoped_ptr<Buffer> m_id;
	uint32 m_announcePort;
};

//////////////////////////////////////////////////////////////////////

inline const Buffer * IDHTSearch::getID() const { return m_id.get(); }

inline uint32 IDHTSearch::getAnnouncePort() const { return m_announcePort; }
inline void IDHTSearch::setAnnouncePort(uint32 announcePort) { m_announcePort = announcePort; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_DHT_IDHTSEARCH_H
