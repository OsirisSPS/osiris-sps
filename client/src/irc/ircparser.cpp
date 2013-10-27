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
#include "ircparser.h"

#include "algorithms.h"
#include "conversions.h"
#include "ircchannel.h"
#include "irccommandendofnames.h"
#include "irccommandchannel.h"
#include "irccommandjoin.h"
#include "irccommandkick.h"
#include "irccommandmessage.h"
#include "irccommandmode.h"
#include "irccommandnames.h"
#include "irccommandnick.h"
#include "irccommandpart.h"
#include "irccommandping.h"
#include "irccommandquit.h"
#include "irccommandservermessage.h"
#include "irccommandserverresponse.h"
#include "log.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IRCParser::IRCParser() : m_alive(true)
{

}

IRCParser::~IRCParser()
{

}

shared_ptr<IIRCCommand> IRCParser::parse(shared_ptr<IRCSession> session, const std::string &str)
{
	OS_ASSERT(algorithms::trim_copy(str) == str);

	if(session == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	size_t pos = str.find(" ");
	if(pos == std::string::npos)
		return null;

	std::string prefix = algorithms::left(str, pos);
	if(prefix.empty())
		return null;

	if(algorithms::starts_with(prefix, ":"))
	{
		prefix = algorithms::mid(prefix, 1);

		pos++;
		size_t nextPos = str.find(" ", pos);
		if(nextPos != std::string::npos)
		{
			std::string command = algorithms::mid(str, pos, nextPos - pos);
			size_t userPos = prefix.find("!");
			if(userPos != std::string::npos)
			{
				// User message

				std::string user = algorithms::left(prefix, userPos);
				std::string content = algorithms::mid(str, nextPos + 1);

				if(algorithms::compare_no_case(command, "JOIN"))
				{
					return parseCommandJoin(session, user, content);
				}
				else if(algorithms::compare_no_case(command, "KICK"))
				{
					return parseCommandKick(session, user, content);
				}
				else if(algorithms::compare_no_case(command, "MODE"))
				{
					return parseCommandMode(session, user, content);
				}
				else if(algorithms::compare_no_case(command, "NICK"))
				{
					return parseCommandNick(session, user, content);
				}
				else if(algorithms::compare_no_case(command, "PART"))
				{
					return parseCommandPart(session, user, content);
				}
				else if(algorithms::compare_no_case(command, "PRIVMSG"))
				{
					return parseCommandPrivmsg(session, user, content);
				}
				else if(algorithms::compare_no_case(command, "QUIT"))
				{
					return parseCommandQuit(session, user, content);
				}
				else if(algorithms::compare_no_case(command, "TOPIC"))
				{
					
				}
			}
			else
			{
				// Server message

				uint32 code = conversions::from_string<uint32>(command);
				std::string content = algorithms::mid(str, nextPos + 1);
				if(code != 0)
				{
					switch(code)
					{
					case OS_IRC_REPLY_CREATED:
					case OS_IRC_REPLY_MYINFO:
					case OS_IRC_REPLY_BOUNCE:
													break;

					case OS_IRC_REPLY_LISTSTART:	return shared_ptr<IIRCCommand>(OS_NEW IRCCommandServerResponse(session, ircCommandTypeChannelListStart));

					case OS_IRC_REPLY_LIST:			return parseCommandChannel(session, content);

					case OS_IRC_REPLY_LISTEND:		return shared_ptr<IIRCCommand>(OS_NEW IRCCommandServerResponse(session, ircCommandTypeChannelListEnd));

					case OS_IRC_REPLY_NAMES:		return parseCommandNames(session, content);

					case OS_IRC_REPLY_ENDOFNAMES:	return parseCommandEndOfNames(session, content);

					default:						return parseCommandServerMessage(session, content);										
					}
				}
				else
				{
				    if(algorithms::compare_no_case(command, "NOTICE"))
                    {
						return parseCommandServerMessage(session, content);
                    }
				}
			}
		}
	}
	else
	{
	    std::string content = algorithms::left(str, pos + 1);

		if(algorithms::compare_no_case(prefix, "PING"))
		{
		    return parseCommandPing(session, content);
		}
	}

	return null;
}

bool IRCParser::parseNickname(const std::string &str, std::string &name, IRCUserType &type)
{
	name.clear();
	type = ircUserTypeNormal;

	if(str.empty())
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(algorithms::starts_with(str, OS_IRC_USER_OPERATOR_PREFIX))
	{
		name = algorithms::mid(str, std::string(OS_IRC_USER_OPERATOR_PREFIX).length());
		type = ircUserTypeOperator;
	}
	else
	{
		name = str;
	}	

	return true;
}

shared_ptr<IIRCCommand> IRCParser::parseCommandChannel(shared_ptr<IRCSession> session, const std::string &content)
{
	size_t pos = 0;
	
	// Salta il 'target' del comando
	if(algorithms::tokenize_next(content, " ", pos).empty())	
		return null;

	std::string channelName = algorithms::tokenize_next(content, " ", pos);
	if(channelName.empty())
		return null;

	std::string channelUsers = algorithms::tokenize_next(content, " ", pos);
	std::string channelDescription;

	if(pos != std::string::npos)
		channelDescription = algorithms::mid(content, pos + 1);	// Estrae la descrizione saltando i ':'
		
	return shared_ptr<IRCCommandChannel>(OS_NEW IRCCommandChannel(session, channelName, channelDescription, conversions::from_utf16<uint32>(channelUsers)));
}

shared_ptr<IIRCCommand> IRCParser::parseCommandEndOfNames(shared_ptr<IRCSession> session, const std::string &content)
{
	size_t pos = 0;
	
	// Salta il 'target' del comando
	if(algorithms::tokenize_next(content, " ", pos).empty())	
	{
		OS_ASSERTFALSE();
		return null;
	}

	std::string channel = algorithms::tokenize_next(content, " :", pos);
	if(channel.empty())
	{
		OS_ASSERTFALSE();
		return null;
	}

	return shared_ptr<IRCCommandEndOfNames>(OS_NEW IRCCommandEndOfNames(session, channel));	
}

shared_ptr<IIRCCommand> IRCParser::parseCommandJoin(shared_ptr<IRCSession> session, const std::string &user, const std::string &content)
{
	return shared_ptr<IRCCommandJoin>(OS_NEW IRCCommandJoin(session, user, algorithms::trim_left_copy(content, boost::algorithm::is_any_of(":"))));	
}

shared_ptr<IIRCCommand> IRCParser::parseCommandKick(shared_ptr<IRCSession> session, const std::string &user, const std::string &content)
{
	vector<std::string>::type tokens;
	algorithms::split(content, tokens, boost::algorithm::is_any_of(" "), true);
	size_t tokensCount = tokens.size();
	if(tokensCount < 2)
	{
		OS_ASSERTFALSE();
		return null;
	}

	std::string channel = tokens[0];
	std::string target = tokens[1];
	std::string comment;
	if(tokensCount > 2)
	{
		OS_ASSERT(algorithms::starts_with(tokens[2], ":"));
		comment = algorithms::mid(tokens[2], 1);
	}

	shared_ptr<IRCCommandKick> command(OS_NEW IRCCommandKick(session));
	command->setUser(user);
	command->setChannel(channel);
	command->setTarget(target);
	command->setComment(conversions::from_utf8<String>(comment));
	return command;
}

shared_ptr<IIRCCommand> IRCParser::parseCommandMode(shared_ptr<IRCSession> session, const std::string &user, const std::string &content)
{
	vector<std::string>::type tokens;
	algorithms::split(content, tokens, boost::algorithm::is_any_of(" "), true);	
	size_t tokensCount = tokens.size();
	if(tokensCount < 2)
	{
		OS_ASSERTFALSE();
		return null;
	}

	std::string target = tokens[0];	
	std::string mode;
	if(algorithms::starts_with(target, "#"))
		mode = tokens[1];
	else
		mode = algorithms::trim_left_copy(tokens[1], boost::algorithm::is_any_of(":"));

	std::string arg;
	if(tokensCount > 2)
		arg = tokens[2];

	shared_ptr<IRCCommandMode> command(OS_NEW IRCCommandMode(session));
	command->setTarget(target);
	command->setArg(arg);
	
	bool enabled = true;
	for(std::string::const_iterator i = mode.begin(); i != mode.end(); ++i)
	{
		IRCModeType commandMode = ircModeTypeUnknown;
		switch(*i)
		{
		case '+':		enabled = true;
						break;

		case '-':		enabled = false;
						break;

		case 'o':		commandMode = ircModeTypeOperator;	
						break;

		case 'h':		commandMode = ircModeTypeHalfOperator;	
						break;

		case 'v':		commandMode = ircModeTypeVoice;
						break;

		case 'i':		commandMode = ircModeTypeInvisible;
						break;

		case 'b':		commandMode = ircModeTypeBan;
						break;					

		default:		OS_ASSERTFALSE();
						break;
		}

		if(commandMode != ircModeTypeUnknown)
			command->addMode(commandMode, enabled);
	}

	return command;
}

shared_ptr<IIRCCommand> IRCParser::parseCommandNames(shared_ptr<IRCSession> session, const std::string &content)
{
	size_t pos = content.find("#");
	if(pos == std::string::npos)
	{
		OS_ASSERTFALSE();
		return null;
	}

	std::string channel = algorithms::tokenize_next(content, " :", pos);
	if(channel.empty())
	{
		OS_ASSERTFALSE();
		return null;
	}
	
	shared_ptr<IRCCommandNames> command(OS_NEW IRCCommandNames(session, channel));

	vector<std::string>::type users;
	algorithms::split(algorithms::mid(content, pos), users, boost::algorithm::is_any_of(" "), true);
	for(vector<std::string>::type::const_iterator i = users.begin(); i != users.end(); ++i)
	{
		command->addUser(*i);
	}	

	return command;	
}

shared_ptr<IIRCCommand> IRCParser::parseCommandNick(shared_ptr<IRCSession> session, const std::string &user, const std::string &content)
{
	return shared_ptr<IRCCommandNick>(OS_NEW IRCCommandNick(session, user, algorithms::trim_left_copy(content, boost::algorithm::is_any_of(":"))));	
}

shared_ptr<IIRCCommand> IRCParser::parseCommandPart(shared_ptr<IRCSession> session, const std::string &user, const std::string &content)
{
	return shared_ptr<IRCCommandPart>(OS_NEW IRCCommandPart(session, user, content));	
}

shared_ptr<IIRCCommand> IRCParser::parseCommandPing(shared_ptr<IRCSession> session, const std::string &content)
{
	OS_ASSERT(session != null);

	if(content.empty())
	{
		OS_ASSERTFALSE();
		return null;
	}

	return shared_ptr<IIRCCommand>(OS_NEW IRCCommandPing(session, content));
}

shared_ptr<IIRCCommand> IRCParser::parseCommandPrivmsg(shared_ptr<IRCSession> session, const std::string &user, const std::string &content)
{
	// Formato: <target> :<messaggio>

	size_t pos = content.find(" ");
	if(pos == std::string::npos)
		return null;

	std::string target = algorithms::left(content, pos);
	OS_ASSERT(user != target);

	std::string message = algorithms::trim_left_copy(algorithms::mid(content, pos + 1), boost::algorithm::is_any_of(":"));
	// Controlla se il messaggio un è comando CTC
	if(algorithms::starts_with(message, "\x01"))		
		return null;	// TODO: CTC non supportato

	shared_ptr<IRCCommandMessage> command(OS_NEW IRCCommandMessage(session));
	command->setSender(user);
	command->setTarget(target);
	command->setMessage(conversions::from_utf8<String>(message));
	return command;
}

shared_ptr<IIRCCommand> IRCParser::parseCommandServerMessage(shared_ptr<IRCSession> session, const std::string &content)
{
	shared_ptr<IRCCommandServerMessage> command(OS_NEW IRCCommandServerMessage(session));
	size_t pos = content.find(" :");
	if(pos != std::string::npos)
		command->setMessage(conversions::from_utf8<String>(algorithms::mid(content, pos + 2)));
	else
		command->setMessage(conversions::from_utf8<String>(content));

	return command;
}

shared_ptr<IIRCCommand> IRCParser::parseCommandQuit(shared_ptr<IRCSession> session, const std::string &user, const std::string &content)
{
	return shared_ptr<IRCCommandQuit>(OS_NEW IRCCommandQuit(session, user));	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
