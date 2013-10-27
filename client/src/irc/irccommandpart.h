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

#ifndef _OS_IRC_IRCCOMMANDPART_H
#define _OS_IRC_IRCCOMMANDPART_H

#include "iirccommand.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCExport IRCCommandPart : public IIRCCommand
{
    typedef IIRCCommand CommandBase;

// Construction
public:
	IRCCommandPart(shared_ptr<IRCSession> session, const std::string &nick, const std::string &channel);
	virtual ~IRCCommandPart();

// Attributes
public:
	inline const std::string & getNick() const;
	inline const std::string & getChannel() const;

// IIRCCommand interface
public:
	virtual IRCCommandType getType() const;

private:
	std::string m_nick;
    std::string m_channel;
};

//////////////////////////////////////////////////////////////////////

inline const std::string & IRCCommandPart::getNick() const { return m_nick; }
inline const std::string & IRCCommandPart::getChannel() const { return m_channel; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCCOMMANDPART_H
