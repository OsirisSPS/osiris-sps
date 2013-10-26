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
#include "irccommandmessage.h"

#include "constants.h"
#include "ircsession.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IRCCommandMessage::IRCCommandMessage(shared_ptr<IRCSession> session) : CommandBase(session)
{

}

IRCCommandMessage::~IRCCommandMessage()
{

}

IRCCommandType IRCCommandMessage::getType() const
{
	return ircCommandTypeMessage;
}

std::string IRCCommandMessage::toString() const
{
	OS_ASSERT(isLocal());

	if(m_target.empty())
	{
		OS_ASSERTFALSE();
		return constants::empty_string;
	}

	if(m_message.empty())
	{
		OS_ASSERTFALSE();
		return constants::empty_string;
	}

	std::string command;
	command.append("PRIVMSG " + m_target + " :");
	command.append(m_message.to_utf8());
	command.append(OS_IRC_COMMANDS_TERMINATOR);

	return command;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
