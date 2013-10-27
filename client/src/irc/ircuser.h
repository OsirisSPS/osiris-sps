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

#ifndef _OS_IRC_IRCUSER_H
#define _OS_IRC_IRCUSER_H

#include "iirctarget.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCSession;

//////////////////////////////////////////////////////////////////////

class IRCExport IRCUser : public IIRCTarget
{	
	typedef IIRCTarget BaseClass;

// Construction
public:
	IRCUser(shared_ptr<IRCSession> session, uint32 id);
	virtual ~IRCUser();

// Attributes
public:
	inline uint32 getID() const;

	bool isLocal() const;

// IIRCTarget interface
public:
	virtual std::string getName() const;
	virtual IRCTargetType getType() const;

private:
	uint32 m_id;
};

//////////////////////////////////////////////////////////////////////

inline uint32 IRCUser::getID() const { return m_id; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCUSER_H
