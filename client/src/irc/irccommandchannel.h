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

#ifndef _OS_IRC_IRCCOMMANDCHANNEL_H
#define _OS_IRC_IRCCOMMANDCHANNEL_H

#include "iirccommand.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCExport IRCCommandChannel : public IIRCCommand
{
    typedef IIRCCommand CommandBase;

// Construction
public:
	IRCCommandChannel(shared_ptr<IRCSession> session, const std::string &channelName, const std::string &channelDescription, uint32 channelUsers);
	virtual ~IRCCommandChannel();

// Attributes
public:
	inline const std::string & getChannelName() const;	
	inline const std::string & getChannelDescription() const;
	inline uint32 getChannelUsers() const;		

// IIRCCommand interface
public:
	virtual IRCCommandType getType() const;

private:
     std::string m_channelName;
	 std::string m_channelDescription;
	 uint32 m_channelUsers;	 
};

//////////////////////////////////////////////////////////////////////

inline const std::string & IRCCommandChannel::getChannelName() const { return m_channelName; }
inline const std::string & IRCCommandChannel::getChannelDescription() const { return m_channelDescription; }
inline uint32 IRCCommandChannel::getChannelUsers() const { return m_channelUsers; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCCOMMANDCHANNEL_H
