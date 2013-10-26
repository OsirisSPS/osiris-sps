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

#ifndef _OS_IRC_IRCCOMMANDNICK_H
#define _OS_IRC_IRCCOMMANDNICK_H

#include "iirccommand.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCExport IRCCommandNick : public IIRCCommand
{
    typedef IIRCCommand CommandBase;

// Construction
public:
	IRCCommandNick(shared_ptr<IRCSession> session, const std::string &oldNick, const std::string &newNick);
	virtual ~IRCCommandNick();

// Attributes
public:
	inline const std::string & getOldNick() const;
	inline const std::string & getNewNick() const;
	
// IIRCCommand interface
public:
	virtual IRCCommandType getType() const;

private:
	std::string m_oldNick;
	std::string m_newNick;
};

//////////////////////////////////////////////////////////////////////

inline const std::string & IRCCommandNick::getOldNick() const { return m_oldNick; }
inline const std::string & IRCCommandNick::getNewNick() const { return m_newNick; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCCOMMANDNICK_H
