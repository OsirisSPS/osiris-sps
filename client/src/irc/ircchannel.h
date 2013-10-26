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

#ifndef _OS_IRC_IRCCHANNEL_H
#define _OS_IRC_IRCCHANNEL_H

#include "boost/thread/recursive_mutex.hpp"
#include "iirctarget.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCSession;

//////////////////////////////////////////////////////////////////////

class IRCExport IRCChannel : public IIRCTarget
{	
	typedef IIRCTarget BaseClass;

// Construction
public:
	IRCChannel(shared_ptr<IRCSession> session, const std::string &name, bool available = true);
	virtual ~IRCChannel();

// Attributes
public:
	std::string getDescription() const;
	void setDescription(const std::string &description);

	uint32 getUsers() const;
	void setUsers(uint32 users);

	bool getAvailable() const;
	void setAvailable(bool available);

// IIRCTarget interface
public:
	virtual std::string getName() const;
	virtual IRCTargetType getType() const;

private:
	mutable boost::recursive_mutex m_cs;
	std::string m_name;
	std::string m_description;
	uint32 m_users;
	bool m_available;	
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCCHANNEL_H
