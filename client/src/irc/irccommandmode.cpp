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
#include "irccommandmode.h"

#include "algorithms.h"
#include "constants.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IRCCommandMode::IRCCommandMode(shared_ptr<IRCSession> session) : CommandBase(session)
{

}

IRCCommandMode::~IRCCommandMode()
{

}

IRCTargetType IRCCommandMode::getTargetType() const
{
	if(algorithms::starts_with(m_target, "#"))
		return ircTargetTypeChannel;

	return ircTargetTypeUser;
}

void IRCCommandMode::addMode(IRCModeType mode, bool enabled)
{
	OS_ASSERT(mode != ircModeTypeUnknown);
	m_modeOptions.push_back(std::make_pair(mode, enabled));
}

IRCCommandType IRCCommandMode::getType() const
{
	return ircCommandTypeMode;
}

std::string IRCCommandMode::toString() const
{
	OS_ASSERT(m_target.empty() == false);
	OS_ASSERT(m_modeOptions.empty() == false);

	std::string command("MODE " + m_target + " ");
	
	for(ModeOptions::const_iterator i = m_modeOptions.begin(); i != m_modeOptions.end(); ++i)
	{
		const ModeOption &option = *i;
		if(option.second)
			command.append("+");
		else
			command.append("-");

		switch(option.first)
		{
		case ircModeTypeOperator:		command.append("o");
										break;

		case ircModeTypeHalfOperator:	command.append("h");
										break;

		case ircModeTypeVoice:			command.append("v");
										break;

		case ircModeTypeInvisible:		command.append("i");
										break;

		case ircModeTypeBan:			command.append("b");
										break;

		default:						OS_ASSERTFALSE();
										return constants::empty_string;
		}
	}

	if(m_arg.empty() == false)
	{
		command.append(" ");
		command.append(m_arg);		
	}

	command.append(OS_IRC_COMMANDS_TERMINATOR);

	return command;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
