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

#ifndef _OS_IRC_IRCCOMMANDNAMES_H
#define _OS_IRC_IRCCOMMANDNAMES_H

#include "iirccommand.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCExport IRCCommandNames : public IIRCCommand
{
    typedef IIRCCommand CommandBase;

public:
	typedef list<std::string>::type Users;

// Construction
public:
	IRCCommandNames(shared_ptr<IRCSession> session, const std::string &channel);
	virtual ~IRCCommandNames();

// Attributes
public:
	inline const std::string & getChannel() const;
	inline const Users & getUsers() const;

// IIRCCommand interface
public:
	virtual IRCCommandType getType() const;

// Operations
public:
	void addUser(const std::string &name);

private:
	std::string m_channel;
	Users m_users;
};

//////////////////////////////////////////////////////////////////////

inline const std::string & IRCCommandNames::getChannel() const { return m_channel; }
inline const IRCCommandNames::Users & IRCCommandNames::getUsers() const { return m_users; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCCOMMANDNAMES_H
